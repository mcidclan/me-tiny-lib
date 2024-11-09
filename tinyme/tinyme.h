#pragma once
#include <pspkernel.h>
#ifdef USE_PSP_EXPLOIT_LIB
extern "C" {
  #include "libpspexploit.h"
}
#endif
#define regPtr       volatile u32*
#define reg(addr)    (*((regPtr)(addr)))
typedef int (*TinyMeFunc)(void);
typedef struct c {
  u32* data;
  TinyMeFunc func;
} TinyMeCom;
int tinyMeInit(TinyMeCom* const tinyMeCom);
void tinyMeDCacheWritebackInvalidAll();
