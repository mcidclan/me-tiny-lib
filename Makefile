BINOUT = ./bin/
PATHSRC = ./
TINYSRC = ./tinyme
PATHOBJS = $(BINOUT)
TARGET = $(BINOUT)tinycounter

CPP_FILES = $(shell ls $(PATHSRC)*.cpp) $(shell ls $(TINYSRC)/*.cpp)
PATHFILES = $(CPP_FILES)
OBJS = $(notdir $(patsubst %.cpp, %.o, $(filter %.cpp, $(PATHFILES))))
OBJS := $(sort $(OBJS:%.o=$(PATHOBJS)%.o))

CFLAGS = -Ofast -G0 -Wall -fno-pic -I./kernel/src -I$(TINYSRC) -I./vendor -L./vendor
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti  -std=c++11
ASFLAGS = $(CFLAGS)
LIBS = -lstdc++ -lpsppower -lpspexploit -lpsprtc

PSP_EBOOT_SFO = $(BINOUT)PARAM.SFO
EXTRA_TARGETS = $(BINOUT)EBOOT.PBP
PSP_EBOOT = $(EXTRA_TARGETS)
PSP_EBOOT_TITLE = Tiny Counter

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

$(PATHOBJS)%.o: $(PATHSRC)%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(PATHOBJS)%.o: $(TINYSRC)/%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)
