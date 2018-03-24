# TODO Debug Define
# ----- Basic config ----- #
# Project Name
PROJ_NAME = Tracked_Vehicle
# ST-Link directory
STLINK = $(ENV_STLINK)
# STM32F4 library directory
STM_COMMON = $(ENV_STM_COM_DIR)
ARMDIR = $(STM_COMMON)/Libraries/STM32F4xx_StdPeriph_Driver/src
# CppUTest directory
CPPUTEST_HOME = /home/felix/Software/cpputest
# Directories
SRCDIR = src
LIBDIR = lib
INCDIR = inc
BUILDDIR = build
TESTDIR = test

# ----- Compiler ----- #
# General C-Flags
CFLAGS_GEN = -Wall
# ----- Target ----- #
CC_TARGET = arm-none-eabi-gcc
OBJCOPY_TARGET = arm-none-eabi-objcopy
GDB_TARGET = arm-none-eabi-gdb
CFLAGS_TARGET = $(CFLAGS_GEN) -Werror  
CFLAGS_TARGET += -Tstm32_flash.ld -mlittle-endian
CFLAGS_TARGET += -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS_TARGET += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS_TARGET += -D USE_STDPERIPH_DRIVER
# ---------- Target Includes ---------- #
CFLAGS_TARGET += -I $(LIBDIR)/$(INCDIR)
CFLAGS_TARGET += -I $(INCDIR)
CFLAGS_TARGET += -isystem $(STM_COMMON)/Utilities/STM32F4-Discovery
CFLAGS_TARGET += -isystem $(STM_COMMON)/Libraries/CMSIS/Include
CFLAGS_TARGET += -isystem $(STM_COMMON)/Libraries/CMSIS/ST/STM32F4xx/Include
CFLAGS_TARGET += -isystem $(STM_COMMON)/Libraries/STM32F4xx_StdPeriph_Driver/inc

# ------ Host ----- #
CC_HOST = gcc
CPPC_HOST = g++
OBJCOPY_HOST = objcopy
CFLAGS_HOST = $(CFLAGS_GEN)
CFLAGS_HOST += -g
CFLAGS_HOST += -I$(CPPUTEST_HOME)/include/CppUTest
CFLAGS_HOST += -I$(CPPUTEST_HOME)/include/CppUTestExt
CFLAGS_HOST += -I$(LIBDIR)/$(INCDIR)
CFLAGS_HOST += -I$(TESTDIR)
CFLAGS_HOST += -I$(INCDIR)
CFLAGS_HOST += -isystem $(STM_COMMON)/Utilities/STM32F4-Discovery
CFLAGS_HOST += -isystem $(STM_COMMON)/Libraries/CMSIS/Include
CFLAGS_HOST += -isystem $(STM_COMMON)/Libraries/CMSIS/ST/STM32F4xx/Include
CFLAGS_HOST += -isystem $(STM_COMMON)/Libraries/STM32F4xx_StdPeriph_Driver/inc
CFLAGS_HOST += -D TESTING -D USE_STDPERIPH_DRIVER
CPP_FLAGS_HOST = -Wno-c++14-compat
CPPLIBS_HOST += -L$(CPPUTEST_HOME)/cpputest_build/lib -lCppUTest -lCppUTestExt
# ----- End config ----- #

# vpath %.c $(SRCDIR):$(LIBDIR)/$(SRCDIR)


# vpath %.o $(BUILDDIR)/test:$(BUILDDIR)/release:$(BUILDDIR)/debug
	
# ---- Source Files ----
SRCS = $(wildcard $(SRCDIR)/*.c)
SRCS += $(wildcard $(LIBDIR)/src/*.c)
SRCS += $(wildcard $(ARMDIR)/*.c)
SRCS += $(STM_COMMON)/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s
SRCS_TEST_CPP = $(wildcard $(TESTDIR)/*.cpp)
SRCS_TEST_C = $(LIBDIR)/src/led_driver.c $(LIBDIR)/src/pwd_driver.c
OBJS_TEST = $(patsubst $(LIBDIR)/src/%.c, $(BUILDDIR)/test/%.o, $(SRCS_TEST_C))
#SRCS_TEST_ARM_C += $(SRCDIR)/system_stm32f4xx.c
#OBJS_TEST_ARM += $(BUILDDIR)/test/system_stm32f4xx.o

.PHONY: proj clean burn burn_release debug test run_test release

proj: debug

clean: 
	@rm -f $(BUILDDIR)/*/*.o $(BUILDDIR)/*/*.elf $(BUILDDIR)/*/*.bin $(BUILDDIR)/*/*.hex $(BUILDDIR)/*/*.map $(BUILDDIR)/test/*

burn: debug
	@$(STLINK)/build/Release/st-flash write $(BUILDDIR)/debug/$(PROJ_NAME).bin 0x8000000

burn_release: release
	@$(STLINK)/build/Release/st-flash write $(BUILDDIR)/release/$(PROJ_NAME).bin 0x8000000

debug: CC = $(CC_TARGET)
debug: OBJCOPY = $(OBJCOPY_TARGET)
debug: CFLAGS = $(CFLAGS_TARGET) -Xlinker -Map=$(BUILDDIR)/debug/$(PROJ_NAME).map -g
debug: $(BUILDDIR)/debug/$(PROJ_NAME).elf

release: CC = $(CC_TARGET)
release: OBJCOPY = $(OBJCOPY_TARGET)
release: CFLAGS = $(CFLAGS_TARGET)
release: $(BUILDDIR)/release/$(PROJ_NAME).elf

$(BUILDDIR)/debug/$(PROJ_NAME).elf: $(SRCS)
	@$(CC) $(CFLAGS) $^ -o $@
	@$(OBJCOPY) -O ihex $(BUILDDIR)/debug/$(PROJ_NAME).elf $(BUILDDIR)/debug/$(PROJ_NAME).hex
	@$(OBJCOPY) -O binary $(BUILDDIR)/debug/$(PROJ_NAME).elf $(BUILDDIR)/debug/$(PROJ_NAME).bin

$(BUILDDIR)/release/$(PROJ_NAME).elf: $(SRCS)
	@$(CC) $(CFLAGS) $^ -o $@
	@$(OBJCOPY) -O ihex $(BUILDDIR)/release/$(PROJ_NAME).elf $(BUILDDIR)/release/$(PROJ_NAME).hex
	@$(OBJCOPY) -O binary $(BUILDDIR)/release/$(PROJ_NAME).elf $(BUILDDIR)/release/$(PROJ_NAME).bin


test: CC = $(CC_HOST)
test: CPPC = $(CPPC_HOST)
test: CFLAGS = $(CFLAGS_HOST)
test: $(BUILDDIR)/test/$(PROJ_NAME)

$(OBJS_TEST): $(SRCS_TEST_C)
	@$(foreach src_file, $(SRCS_TEST_C), $(CC) $(CFLAGS) -c $(src_file) -o $(patsubst $(LIBDIR)/src/%.c, $(BUILDDIR)/test/%.o, $(src_file));)
	# $(CC) $(CFLAGS) -c $^ -o $@

$(BUILDDIR)/test/$(PROJ_NAME): $(OBJS_TEST_ARM) $(OBJS_TEST) $(SRCS_TEST_CPP) 
	@$(CPPC) $(CFLAGS) $(CPP_FLAGS_HOST) $^ -o $@  $(CPPLIBS_HOST)
	@$(BUILDDIR)/test/$(PROJ_NAME)

run_test:
	$(BUILDDIR)/test/$(PROJ_NAME)
