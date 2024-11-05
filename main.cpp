#include "tinyme.h"
#include <psppower.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <malloc.h>
#include <cstring>

PSP_MODULE_INFO("tiny-sample", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);

int* mem = nullptr;
bool stop = false;

int meLoop() {
  mem[0]++;
  mem[1] = 777;
  return !stop;
}

int main(int argc, char **argv) {
  scePowerSetClockFrequency(333, 333, 166);

  // Uncache memory with 0x40000000 to avoid cache line conflicts
  void* const _mem = memalign(16, sizeof(int) * 4);
  mem = (int* const) (0x40000000 | (u32)_mem);
  memset((void*)mem, 0, sizeof(int) * 4);
  ((u32*)_mem)[2] = 888;
  
  pspDebugScreenInit();
  pspDebugScreenSetXY(0, 1);
  pspDebugScreenPrintf("Init Media Engine...");
    
  TinyMeCom meCom = {
    nullptr,
    meLoop,
  };
  tinyMeInit(&meCom);
  
  SceCtrlData ctl;
  do {
    sceCtrlPeekBufferPositive(&ctl, 1);
    pspDebugScreenSetXY(0, 1);
    pspDebugScreenPrintf("Counters %i; %i; %i; %i", mem[0], mem[1], mem[2], mem[3]++);
    sceDisplayWaitVblankStart();
  } while(!(ctl.Buttons & PSP_CTRL_HOME));
  
  stop = true;
  sceKernelDcacheWritebackInvalidateAll();
  
  pspDebugScreenClear();
  pspDebugScreenSetXY(0, 1);
  pspDebugScreenPrintf("Exiting...");
  sceKernelDelayThread(1000000);
  free(_mem);
  sceKernelExitGame();
  return 0;
}
