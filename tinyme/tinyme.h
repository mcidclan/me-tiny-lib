#ifndef _ME_LIB_H
#define _ME_LIB_H
#include <pspkernel.h>
#ifdef __cplusplus
extern "C" {
#endif
  #include "libpspexploit.h"
  #define regPtr       volatile u32*
  #define reg(addr)    (*((regPtr)(addr)))
  typedef int (*TinyMeFunc)(void);
  typedef struct c {
    u32* data;
    TinyMeFunc func;
  } TinyMeCom;
  int tinyMeInit(TinyMeCom* const tinyMeCom);
  void tinyMeDCacheWritebackInvalidAll();
#ifdef __cplusplus
}
#endif
#endif
