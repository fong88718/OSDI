import argparse
import os
import serial
import numpy as np

parse = argparse.ArgumentParser()
parse.add_argument("image")
parse.add_argument("tty")
args = parse.parse_args()

def checksum(bytecodes):
    return sum(bytecodes)

def main():
    try:
        ser = serial.Serial(args.tty, 115200)
    except:
        print("Serial init failed!")
        exit(1)
    file_path = args.image
    file_size = os.path.getsize(file_path)
    
    with open(file_path, "rb") as f:
        bytecodes = f.read()
    
    file_checksum = checksum(bytecodes)
    ser.write(file_size.to_bytes(4, byteorder='big'))
    ser.write(file_checksum.to_bytes(4, byteorder='big'))

    print(f"Image Size: {file_size}, Checksum: {file_checksum}")

    per_chunk = 256
    chunk_count = file_size // per_chunk + (file_size % per_chunk > 0)
    
    for i in range(chunk_count):
        print(f"\r{i+1}/{chunk_count}", end="", flush=True)
        ser.write(bytecodes[i*per_chunk:(i+1)*per_chunk])

if __name__ == "__main__":
    main()

