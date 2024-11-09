#pragma once
#include <pspkernel.h>

#define nrp          u32*
#define nrg(addr)    (*((nrp)(addr)))

#define vrp          volatile u32*
#define vrg(addr)    (*((vrp)(addr)))
