#pragma once

#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#define BIT(x) (1 << (x))

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define vu8 volatile u8
#define vu16 volatile u16
#define vu32 volatile u32
#define vu64 volatile u64

//CSND Compatability shit
#define vs8 volatile int8_t  
#define vs16 volatile int16_t 
#define vs32 volatile int32_t 
#define vs64 volatile int64_t 

#define ALIGN(a)       __attribute__((aligned(a))) // Use alignas() instead.
#define NAKED          __attribute__((naked))
#define NOINLINE       __attribute__((noinline))
#define ALWAYS_INLINE  __attribute__((always_inline)) static inline
#define PACKED         __attribute__((packed))
#define TARGET_ARM     __attribute__((target("arm")))
#define TARGET_THUMB   __attribute__((target("thumb")))
#define UNUSED         __attribute__((unused))
#define USED           __attribute__((used))
#define WEAK           __attribute__((weak))
//lmao

#define max(a,b) \
    (((a) > (b)) ? (a) : (b))
#define min(a,b) \
    (((a) < (b)) ? (a) : (b))
#define getbe16(d) \
    ((((u8*)d)[0]<<8) | ((u8*)d)[1])
#define getbe32(d) \
    ((((u32) getbe16(d))<<16) | ((u32) getbe16(d+2)))
#define getbe64(d) \
    ((((u64) getbe32(d))<<32) | ((u64) getbe32(d+4)))
#define getle16(d) \
    ((((u8*)d)[1]<<8) | ((u8*)d)[0])
#define getle32(d) \
    ((((u32) getle16(d+2))<<16) | ((u32) getle16(d)))
#define getle64(d) \
    ((((u64) getle32(d+4))<<32) | ((u64) getle32(d)))
#define align(v,a) \
    (((v) % (a)) ? ((v) + (a) - ((v) % (a))) : (v))
	

// standard work area, size must be a multiple of 0x200 (512)
#define BUFFER_ADDRESS  ((u8*) 0x21000000)
#define BUFFER_MAX_SIZE ((u32) (1 * 1024 * 1024))
    
// log file name
#define LOG_FILE "Decrypt9.log"

static inline u32 strchrcount(const char* str, char symbol) {
    u32 count = 0;
    for (u32 i = 0; str[i] != '\0'; i++) {
        if (str[i] == symbol)
            count++;
    }
    return count;
}


typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;