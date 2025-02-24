#include "tinyme.h"
#include <pspdisplay.h>
#include <pspctrl.h>

PSP_MODULE_INFO("tiny-sample", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);

static volatile u32* mem = nullptr;
#define scCounter         (mem[0])
#define meCounter         (mem[1])
#define meStop            (mem[2])

float meFloat = 0.4f;
void meLoop() {
  meFloat += 0.5f;
  do {
    meDCacheWritebackInvalidAll();
  } while(!mem);
  
  do {
    meCounter++;
  } while(!meStop);
  meHalt();
}

int main() {
  scePowerSetClockFrequency(333, 333, 166);

  mem = meGetUncached32(4);
  
  pspDebugScreenInit();
  pspDebugScreenSetXY(0, 1);
  pspDebugScreenPrintf("Init me...");
  
  meInit();
  
  SceCtrlData ctl;
  do {
    sceCtrlPeekBufferPositive(&ctl, 1);
    pspDebugScreenSetXY(0, 1);
    pspDebugScreenPrintf("sc counter 0x%x   ", scCounter++);
    pspDebugScreenSetXY(0, 2);
    pspDebugScreenPrintf("me counter 0x%x   ", meCounter);
    pspDebugScreenSetXY(0, 3);
    pspDebugScreenPrintf("me float %f   ", meFloat);
    sceDisplayWaitVblankStart();
  } while(!(ctl.Buttons & PSP_CTRL_HOME));
  
  meStop = true;
  pspDebugScreenClear();
  pspDebugScreenSetXY(0, 1);
  pspDebugScreenPrintf("Exiting...");
  sceKernelDelayThread(100000);
  
  meGetUncached32(0);
  sceKernelExitGame();
  return 0;
}
