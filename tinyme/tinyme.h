#pragma once
#include "common.h"
#ifdef USE_PSP_EXPLOIT_LIB
extern "C" {
  #include "libpspexploit.h"
}
#endif

int meInit();
void meDcacheWritebackInvalidateAll();
void meHalt();
void meGetUncached32(volatile u32** const mem, const u32 size);
void meUnlockHwUserRegisters();
void meUnlockMemory();
