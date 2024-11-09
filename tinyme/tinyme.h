#pragma once
#include "common.h"
#ifdef USE_PSP_EXPLOIT_LIB
extern "C" {
  #include "libpspexploit.h"
}
#endif
typedef int (*TinyMeFunc)(void);
typedef struct c {
  u32* data;
  TinyMeFunc func;
} TinyMeCom;
int tinyMeInit(TinyMeCom* const tinyMeCom);
void tinyMeDCacheWritebackInvalidAll();
