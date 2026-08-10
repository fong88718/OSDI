#ifndef QUEUE_H
#define QUEUE_H

#define MAX_SIZE 1024

typedef struct _Queue{
    int front;
    int rear;
    char buf[MAX_SIZE];
}Queue;

void queue_push(Queue *q, char c);
char queue_pop(Queue *q);
int queue_empty(Queue *q);
int queue_full(Queue *q);
void queue_init(Queue *q);

#endif