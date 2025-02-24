#include "tinyme.h"
#define ME_HANDLER_BASE 0xbfc00000

extern int meLoop();

void meDCacheWritebackInvalidAll() {
 for (int i = 0; i < 8192; i += 64) {
  asm("cache 0x14, 0(%0)" :: "r"(i));
  asm("cache 0x14, 0(%0)" :: "r"(i));
 }
 asm("sync");
}

u32* meGetUncached32(const u32 size) {
  static void* _base = nullptr;
  if (!_base) {
    _base = memalign(16, size*4);
    memset(_base, 0, size);
    sceKernelDcacheWritebackInvalidateAll();
    return (u32*)(UNCACHED_USER_MASK | (u32)_base);
  } else if (!size) {
    free(_base);
  }
  return nullptr;
}

void meHalt() {
  asm volatile(".word 0x70000000");
}


extern char __start__me_section;
extern char __stop__me_section;
__attribute__((section("_me_section")))
void meHandler() {
  hw(0xbc100050) = 0x0f;       // 0b1111, enable me and AW buses
  hw(0xbc100004) = 0xffffffff; // enable NMI interrupts in the global hardware context
  hw(0xbc100040) = 0x02;       // allow 64MB ram
  asm("sync");
  
  asm volatile(
    "li          $k0, 0x30000000\n"
    "mtc0        $k0, $12\n"
    "sync\n"
    "la          $k0, %0\n"
    "li          $k1, 0x80000000\n"
    "or          $k0, $k0, $k1\n"
    "jr          $k0\n"
    "nop\n"
    :
    : "i" (meLoop)
    : "k0"
  );
  
}

static void kernelMeInit() {
  int k1 = pspSdkSetK1(0);
  #define me_section_size (&__stop__me_section - &__start__me_section)
  memcpy((void *)ME_HANDLER_BASE, (void*)&__start__me_section, me_section_size);
  meDCacheWritebackInvalidAll();
  // reset me
  hw(0xbc10004c) = 0x04;
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
