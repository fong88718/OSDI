extern unsigned char _begin, _end, _boot_loader;

__attribute__((section(".text.relocate")))
void relocate()
{
    volatile void* _boot_loader_addr = (void*)&_boot_loader;
    long long kernel_size = &_end - &_begin;
    unsigned char* new_addr = (unsigned char*)&_boot_loader;
    unsigned char* old_addr = (unsigned char*)&_begin;

    while(kernel_size-- > 0)
    {
        *new_addr++ = *old_addr;
        *old_addr++ = 0;
    }
    void (*start)(void) = _boot_loader_addr;
    start();
}