#pragma once
#include <pspkernel.h>
#include <psppower.h>
#include <malloc.h>
#include <cstring>

#define u8  unsigned char
#define u16 unsigned short int
#define u32 unsigned int
#define u64 uint64_t

#define hwr         volatile u32*
#define hw(addr)    (*((hwr)(addr)))

#define UNCACHED_USER_MASK    0x40000000
#define CACHED_KERNEL_MASK    0x80000000
#define UNCACHED_KERNEL_MASK  0xA0000000
