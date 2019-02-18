#pragma once

extern "C"
{
    extern void *malloc(size_t size);
    extern void *calloc(size_t size);
    extern int free(void *ptr); 
    extern void *realloc(void *ptr, size_t size);
}
