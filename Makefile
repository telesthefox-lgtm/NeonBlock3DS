ifeq ($(strip $(DEVKITPRO)),)
$(error "DEVKITPRO is not set. Install devkitPro and open its MSYS2 terminal.")
endif
include $(DEVKITPRO)/libctru/default_makefile
TARGET := NeonBlock3DS
BUILD := build
SOURCES := source
INCLUDES :=
GRAPHICS :=
DATA :=
ARCH := -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft
CFLAGS := -g -Wall -O2 -mword-relocations -ffunction-sections $(ARCH) $(INCLUDES)
CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions
LDFLAGS := -specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)
LIBS := -lcitro2d -lcitro3d -lctru -lm
.PHONY: all clean
all: $(TARGET).3dsx
include $(DEVKITPRO)/3ds_rules
