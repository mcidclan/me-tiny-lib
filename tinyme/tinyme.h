#pragma once
#include "common.h"
#ifdef USE_PSP_EXPLOIT_LIB
extern "C" {
  #include "libpspexploit.h"
}
#endif

int meInit();
void meHalt();
u32* meGetUncached32(const u32);
void meDCacheWritebackInvalidAll();
