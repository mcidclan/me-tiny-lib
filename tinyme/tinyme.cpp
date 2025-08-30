#include "tinyme.h"
#define ME_HANDLER_BASE 0xbfc00000

extern int meLoop();

void meDcacheWritebackInvalidateAll() {
 asm("sync");
 for (int i = 0; i < 8192; i += 64) {
  asm("cache 0x14, 0(%0)" :: "r"(i));
  asm("cache 0x14, 0(%0)" :: "r"(i));
 }
 asm("sync");
}

void meHalt() {
  asm volatile(".word 0x70000000");
}

void meGetUncached32(volatile u32** const mem, const u32 size) {
  static void* _base = nullptr;
  if (!_base) {
    const u32 byteCount = size * 4;
    _base = memalign(16, byteCount);
    memset(_base, 0, byteCount);
    sceKernelDcacheWritebackInvalidateAll();
    *mem = (u32*)(UNCACHED_USER_MASK | (u32)_base);
    __asm__ volatile (
      "cache 0x1b, 0(%0)  \n"
      "sync               \n"
      : : "r" (mem) : "memory"
    );
    return;
  } else if (!size) {
    free(_base);
  }
  *mem = nullptr;
  return;
}

void meUnlockHwUserRegisters() {
  const u32 START = 0xbc000030;
  const u32 END   = 0xbc000044;
  for(u32 reg = START; reg <= END; reg+=4) {
    hw(reg) = 0xffffffff;
  }
  asm volatile("sync");
}

void meUnlockMemory() {
  const u32 START = 0xbc000000;
  const u32 END   = 0xbc00002c;
  for(u32 reg = START; reg <= END; reg+=4) {
    hw(reg) = 0xffffffff;
  }
  asm volatile("sync");
}

static inline void vmeSetMinimalConfig() {
  hw(0xBCC00000) = -1;
  hw(0xBCC00030) = 1;
  hw(0xBCC00040) = 1;
  asm volatile("sync");
}

extern char __start__me_section;
extern char __stop__me_section;
__attribute__((section("_me_section")))
void meHandler() {
  hw(0xbc100040) = 0x02;       // allow 64MB ram
  hw(0xbc100050) = 0x0f;       // 0b1111, enable vme and AW bus clocks
  hw(0xbc100004) = 0xffffffff; // enable NMI interrupts in the global hardware context
  asm("sync");
  
  vmeSetMinimalConfig();
  
  asm volatile(
    "li          $k0, 0x30000000     \n"
    "mtc0        $k0, $12            \n"
    "sync                            \n"
    "la          $k0, %0             \n"
    "li          $k1, 0x80000000     \n"
    "or          $k0, $k0, $k1       \n"
    "cache       0x8, 0($k0)         \n"
    "sync                            \n"
    "jr          $k0                 \n"
    "nop                             \n"
    :
    : "i" (meLoop)
    : "k0", "k1", "memory"
  );
  
}

static void kernelMeInit() {
  int k1 = pspSdkSetK1(0);
  #define me_section_size (&__stop__me_section - &__start__me_section)
  memcpy((void *)ME_HANDLER_BASE, (void*)&__start__me_section, me_section_size);
  sceKernelDcacheWritebackInvalidateAll();
  // Hardware reset the me & vme
  hw(0xbc10004c) = 0x14;
  hw(0xbc10004c) = 0x0;
  asm volatile("sync");
  pspXploitRepairKernel();
  pspSdkSetK1(k1);
}

int meInit() {
  int res = pspXploitInitKernelExploit();
  if (res == 0) {
    res = pspXploitDoKernelExploit();
    if (res == 0) {
      pspXploitExecuteKernel((u32)kernelMeInit);
      return 0;
    }
  }
  return -1;
}
