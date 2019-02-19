#pragma once
#include <boss.h>

extern "C"
{
    extern void* malloc(size_t size);
    #if BOSS_WINDOWS
        extern void free(void* ptr);
    #else
        extern int free(void* ptr);
    #endif
}
