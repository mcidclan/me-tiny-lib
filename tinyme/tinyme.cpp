#include "tinyme.h"
#define ME_HANDLER_BASE 0xbfc00040
#define ME_SIZE_ADDR 0xbfc00160

static TinyMeCom* _tinyMeCom = nullptr;

void tinyMeDCacheWritebackInvalidAll() {
 for (int i = 0; i < 8192; i += 64) {
  asm("cache 0x14, 0(%0)" :: "r"(i));
  asm("cache 0x14, 0(%0)" :: "r"(i));
 }
 asm("sync");
}

static void inline _memcpy(void* d, const void* s, u32 size) {
  u8* dst = (u8*)d;
  const u8* src = (const u8*)s;
  while (size >= 4) {
    *(u32*)dst = *(const u32*)src;
    dst += 4;
    src += 4;
    size -= 4;
  }
  while (size > 0) {
    *dst++ = *src++;
    size--;
  }
}

extern char __start__me_section;
extern char __stop__me_section;
__attribute__((section("_me_section")))
void _handler() {
  reg(0xbc100050) = 0x04;          // 0b100; // enable AW RegB Bus
  reg(0xbc100004) = 0xFFFFFFFF; // clear all interrupts, just usefull
  reg(0xbc100040) = 0x02;       // allow 64MB ram, probably better (default is 16MB)
  asm("sync");
  
  volatile TinyMeCom* const meCom = (volatile TinyMeCom*)(ME_HANDLER_BASE + reg(ME_SIZE_ADDR));
  while (1) {
    tinyMeDCacheWritebackInvalidAll();
    if (!meCom->func()) {
      break;
    }
  }
}

static inline void _tinyMeInit(TinyMeCom* const tinyMeCom) {
  void* start = &__start__me_section;
  const u32 size = (u32)(&__stop__me_section - (u32)start);
  reg(ME_SIZE_ADDR) = size;
  asm("sync");

  volatile TinyMeCom* const _com = (volatile TinyMeCom*)(ME_HANDLER_BASE + size);
  _memcpy((void*)_com, tinyMeCom, sizeof(TinyMeCom));
  _memcpy((void *)ME_HANDLER_BASE, start, size);

  reg(0xBC10004C) |= 0x04;        // 0b0100;  // reset enable, just the me
  asm("sync");
  reg(0xBC10004C) = 0x0;          // disable reset to start the me
  asm("sync"); 
  
  tinyMeDCacheWritebackInvalidAll();
}

static void _kernelInitMe() {
  int k1 = pspSdkSetK1(0);
  _tinyMeInit(_tinyMeCom);
  pspXploitRepairKernel();
  pspSdkSetK1(k1);
}

int tinyMeInit(TinyMeCom* const tinyMeCom) {
  _tinyMeCom = tinyMeCom;
  int res = pspXploitInitKernelExploit();
  if (res == 0) {
    res = pspXploitDoKernelExploit();
    if (res == 0) {
      pspXploitExecuteKernel((u32)_kernelInitMe);
      return 0;
    }
  }
  return -1;
}
