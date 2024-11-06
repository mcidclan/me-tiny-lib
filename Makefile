BINOUT = ./bin/
PATHSRC = ./
TINYSRC = ./tinyme
VENDOR = ./vendor
PATHOBJS = $(BINOUT)
TARGET = $(BINOUT)tinycounter

CPP_FILES = $(wildcard $(PATHSRC)*.cpp) $(wildcard $(TINYSRC)/*.cpp)
OBJS = $(patsubst %.cpp, $(PATHOBJS)%.o, $(notdir $(CPP_FILES)))

CFLAGS = -Ofast -G0 -Wall -fno-pic -I./kernel/src -I$(TINYSRC) -I./vendor -L./vendor -Wextra -Werror
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti -std=c++11
ASFLAGS = $(CFLAGS)
LIBS = -lpsppower -lpsprtc

PSP_EXPLOIT_LIB := $(shell ld ./vendor/libpspexploit.a -o /dev/null 2>/dev/null && echo "yes" || echo "no")

ifeq ($(PSP_EXPLOIT_LIB), yes)
    LIBS = -lpsppower -lpspexploit -lpsprtc
else
    VENDOR_SRC = $(wildcard $(VENDOR)/*.c)
    ASM_FILES = $(wildcard $(VENDOR)/*.S)
    VENDOR_OBJS = $(patsubst %.c, $(PATHOBJS)%.o, $(notdir $(VENDOR_SRC))) \
                  $(patsubst %.S, $(PATHOBJS)%.o, $(notdir $(ASM_FILES)))
    OBJS += $(VENDOR_OBJS)
endif

PSP_EBOOT_SFO = $(BINOUT)PARAM.SFO
EXTRA_TARGETS = $(BINOUT)EBOOT.PBP
PSP_EBOOT = $(EXTRA_TARGETS)
PSP_EBOOT_TITLE = Tiny Counter
  
PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

$(PATHOBJS)%.o: $(VENDOR)/%.c
	$(CC) -o $@ -c $< $(CFLAGS) -std=c99

$(PATHOBJS)%.o: $(PATHSRC)%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(PATHOBJS)%.o: $(TINYSRC)/%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(PATHOBJS)%.o: $(VENDOR)/%.S
	$(AS) -o $@ -c $< $(ASFLAGS)
  

clean: clean-all

clean-all:
	rm -f $(PATHOBJS)/*.o $(TARGET) $(EXTRA_TARGETS)
