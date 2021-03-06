#
# embedXcode
# ----------------------------------
# Embedded Computing on Xcode
#
# Copyright © Rei VILO, 2010-2015
# http://embedxcode.weebly.com
# All rights reserved
#
#
# Last update: Jul 09, 2014 release 300






# Edison x86 specifics
# ----------------------------------
#
PLATFORM         := IntelArduino
BUILD_CORE       := x86
PLATFORM_TAG      = ARDUINO=10600 __ARDUINO_X86__ EMBEDXCODE=$(RELEASE_NOW)
APPLICATION_PATH := $(GALILEO_PATH)

APP_TOOLS_PATH   := $(APPLICATION_PATH)/hardware/tools/i686/pokysdk/usr/bin/i586-poky-linux
CORE_LIB_PATH    := $(APPLICATION_PATH)/hardware/intel/i686/cores/arduino
APP_LIB_PATH     := $(APPLICATION_PATH)/hardware/intel/i686/libraries
BOARDS_TXT       := $(APPLICATION_PATH)/hardware/intel/i686/boards.txt

# Version check
#
w001 = $(APPLICATION_PATH)/lib/version.txt
VERSION_CHECK = $(shell if [ -f $(w001) ] ; then cat $(w001) ; fi)
ifneq ($(VERSION_CHECK),1.6.0+Intel)
    $(error Intel Arduino IDE release 1.6.0 required.)
endif

# Uploader
#
#ifneq ($(BOARD_PORT),ssh)
#    WARNING_MESSAGE = 'NOT RECOMMENDED. USE INTEL EDISON (WIFI) INSTEAD'
#endif

REMOTE_FOLDER    = /sketch

UPLOADER         = izmirdl
UPLOADER_PATH    = $(APPLICATION_PATH)/hardware/intel/i686/tools/izmir
UPLOADER_EXEC    = $(UTILITIES_PATH)/uploader_izmir.sh
UPLOADER_OPTS    = $(APPLICATION_PATH)/hardware/tools/x86/bin

# Sketchbook/Libraries path
# wildcard required for ~ management
# ?ibraries required for libraries and Libraries
#
ifeq ($(USER_LIBRARY_DIR)/Arduino/preferences.txt,)
    $(error Error: run Arduino once and define the sketchbook path)
endif

ifeq ($(wildcard $(SKETCHBOOK_DIR)),)
    SKETCHBOOK_DIR = $(shell grep sketchbook.path $(USER_LIBRARY_DIR)/Arduino15/preferences.txt | cut -d = -f 2)
endif

ifeq ($(wildcard $(SKETCHBOOK_DIR)),)
   $(error Error: sketchbook path not found)
endif

USER_LIB_PATH  = $(wildcard $(SKETCHBOOK_DIR)/?ibraries)

# Rules for making a c++ file from the main sketch (.pde)
#
PDEHEADER      = \\\#include \"Arduino.h\"

# Tool-chain names
#
CC      = $(APP_TOOLS_PATH)/i586-poky-linux-gcc
CXX     = $(APP_TOOLS_PATH)/i586-poky-linux-g++
AR      = $(APP_TOOLS_PATH)/i586-poky-linux-ar
OBJDUMP = $(APP_TOOLS_PATH)/i586-poky-linux-objdump
OBJCOPY = $(APP_TOOLS_PATH)/i586-poky-linux-objcopy
SIZE    = $(APP_TOOLS_PATH)/i586-poky-linux-size
NM      = $(APP_TOOLS_PATH)/i586-poky-linux-nm
STRIP   = $(APP_TOOLS_PATH)/i586-poky-linux-strip

# Specific AVRDUDE location and options
#
AVRDUDE_COM_OPTS  = -D -p$(MCU) -C$(AVRDUDE_CONF)

BOARD    = $(call PARSE_BOARD,$(BOARD_TAG),board)
#LDSCRIPT = $(call PARSE_BOARD,$(BOARD_TAG),build.ldscript)
VARIANT  = $(call PARSE_BOARD,$(BOARD_TAG),build.variant)
VARIANT_PATH = $(APPLICATION_PATH)/hardware/intel/i686/variants/$(VARIANT)
VARIANT_CPP_SRCS  = $(wildcard $(VARIANT_PATH)/*.cpp) # */  $(VARIANT_PATH)/*/*.cpp #*/
VARIANT_OBJ_FILES = $(VARIANT_CPP_SRCS:.cpp=.cpp.o)
VARIANT_OBJS      = $(patsubst $(APPLICATION_PATH)/%,$(OBJDIR)/%,$(VARIANT_OBJ_FILES))

#SYSTEM_LIB  = $(call PARSE_BOARD,$(BOARD_TAG),build.variant_system_lib)
SYSTEM_PATH = $(VARIANT_PATH)
SYSTEM_OBJS = $(SYSTEM_PATH)/$(SYSTEM_LIB)

# Two locations for Arduino libraries
# The libraries USBHost WiFi SD Servo Ethernet are duplicated
#
APP_LIB_PATH_1    = $(APPLICATION_PATH)/hardware/intel/i686/libraries
APP_LIB_PATH_2    = $(APPLICATION_PATH)/libraries
APP_LIB_PATH      = $(APP_LIB_PATH_1) $(APP_LIB_PATH_2)

APP_LIBS_LIST_1   = $(APP_LIBS_LIST)
APP_LIBS_LIST_2   = $(filter-out USBHost WiFi SD Servo Ethernet,$(APP_LIBS_LIST))

a1001    = $(foreach dir,$(APP_LIB_PATH_1),$(patsubst %,$(dir)/%,$(APP_LIBS_LIST_1)))
a1001   += $(foreach dir,$(APP_LIB_PATH_1),$(patsubst %,$(dir)/%/utility,$(APP_LIBS_LIST_1)))
a1001   += $(foreach dir,$(APP_LIB_PATH_1),$(patsubst %,$(dir)/%/src,$(APP_LIBS_LIST_1)))
a1001   += $(foreach dir,$(APP_LIB_PATH_1),$(patsubst %,$(dir)/%/src/utility,$(APP_LIBS_LIST_1)))
a1001   += $(foreach dir,$(APP_LIB_PATH_1),$(patsubst %,$(dir)/%/src/arch/$(BUILD_CORE),$(APP_LIBS_LIST_1)))

a1002    = $(foreach dir,$(APP_LIB_PATH_2),$(patsubst %,$(dir)/%,$(APP_LIBS_LIST_2)))
a1002   += $(foreach dir,$(APP_LIB_PATH_2),$(patsubst %,$(dir)/%/utility,$(APP_LIBS_LIST_2)))
a1002   += $(foreach dir,$(APP_LIB_PATH_2),$(patsubst %,$(dir)/%/src,$(APP_LIBS_LIST_2)))
a1002   += $(foreach dir,$(APP_LIB_PATH_2),$(patsubst %,$(dir)/%/src/utility,$(APP_LIBS_LIST_2)))
a1002   += $(foreach dir,$(APP_LIB_PATH_2),$(patsubst %,$(dir)/%/src/arch/$(BUILD_CORE),$(APP_LIBS_LIST_2)))

APP_LIB_CPP_SRC = $(foreach dir,$(a1001) $(a1002),$(wildcard $(dir)/*.cpp)) # */
APP_LIB_C_SRC   = $(foreach dir,$(a1001) $(a1002),$(wildcard $(dir)/*.c)) # */
APP_LIB_H_SRC   = $(foreach dir,$(a1001) $(a1002),$(wildcard $(dir)/*.h)) # */

APP_LIB_OBJS     = $(patsubst $(APPLICATION_PATH)/%.cpp,$(OBJDIR)/%.cpp.o,$(APP_LIB_CPP_SRC))
APP_LIB_OBJS    += $(patsubst $(APPLICATION_PATH)/%.c,$(OBJDIR)/%.c.o,$(APP_LIB_C_SRC))

BUILD_APP_LIBS_LIST = $(subst $(BUILD_APP_LIB_PATH)/, ,$(APP_LIB_CPP_SRC))

APP_LIBS_LOCK = 1


# MCU options
#
MCU_FLAG_NAME   = march
MCU             = $(call PARSE_BOARD,$(BOARD_TAG),build.mcu)
    

    OPTIMISATION   = -Os


# Include paths
#
#INCLUDE_PATH     = $(APPLICATION_PATH)/hardware/arduino/samd/system/libsam
#INCLUDE_PATH    += $(APPLICATION_PATH)/hardware/arduino/samd/system/libsam/include
#INCLUDE_PATH    += $(APPLICATION_PATH)/hardware/tools/CMSIS/CMSIS/Include/
#INCLUDE_PATH    += $(APPLICATION_PATH)/hardware/tools/CMSIS/Device/ATMEL/
INCLUDE_PATH     = $(CORE_LIB_PATH) $(VARIANT_PATH)
INCLUDE_PATH    += $(sort $(dir $(APP_LIB_CPP_SRC) $(APP_LIB_C_SRC) $(APP_LIB_H_SRC)))


# Flags for gcc, g++ and linker
# ----------------------------------
#
# Common CPPFLAGS for gcc, g++, assembler and linker
#
CPPFLAGS     = $(OPTIMISATION) $(WARNING_FLAGS)
CPPFLAGS    += $(call PARSE_BOARD,$(BOARD_TAG),build.f_cpu) -$(MCU_FLAG_NAME)=$(MCU) 
CPPFLAGS    += --sysroot=$(APPLICATION_PATH)/hardware/tools/i686/core2-32-poky-linux
CPPFLAGS    += -w -ffunction-sections -fdata-sections -MMD
CPPFLAGS    += $(addprefix -D, $(PLATFORM_TAG))
CPPFLAGS    += $(addprefix -I, $(INCLUDE_PATH))

# Specific CFLAGS for gcc only
# gcc uses CPPFLAGS and CFLAGS
#
CFLAGS       =

# Specific CXXFLAGS for g++ only
# g++ uses CPPFLAGS and CXXFLAGS
#
CXXFLAGS     = -fno-exceptions

# Specific ASFLAGS for gcc assembler only
# gcc assembler uses CPPFLAGS and ASFLAGS
#
ASFLAGS      = -Xassembler

# Specific LDFLAGS for linker only
# linker uses CPPFLAGS and LDFLAGS
#
LDFLAGS      = $(OPTIMISATION) $(WARNING_FLAGS)
LDFLAGS     +=  $(call PARSE_BOARD,$(BOARD_TAG),build.f_cpu) -$(MCU_FLAG_NAME)=$(MCU)
LDFLAGS     += --sysroot=$(APPLICATION_PATH)/hardware/tools/i686/core2-32-poky-linux
LDFLAGS     += -Wl,--gc-sections -march=i586
#LDFLAGS     += -T $(VARIANT_PATH)/$(LDSCRIPT)


# Specific OBJCOPYFLAGS for objcopy only
# objcopy uses OBJCOPYFLAGS only
#
#OBJCOPYFLAGS  = -v -Obinary

# Target
#
TARGET_HEXBIN = $(TARGET_DOT)


# Commands
# ----------------------------------
# Link command
#
COMMAND_LINK    = $(CXX) $(LDFLAGS) $(OUT_PREPOSITION)$@ $(LOCAL_OBJS) $(TARGET_A) -L$(OBJDIR) -lm -lpthread


