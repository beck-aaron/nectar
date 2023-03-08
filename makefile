#
# BUILD INSTRUCTIONS FOR NECTAR
#
# Change GCCLIB to the include directory of the installation of the
# arm-gcc compiler toolchain.
#
# When using the "bear" target, comment out CC & LD variables so that the
# compile_commands.json file can be generated properly.
#

GCCLIB    = /opt/arm-gcc/arm-none-eabi
ASFDIR    = lib
SOURCEDIR = src
BUILDDIR  = build

CC=arm-none-eabi-gcc
LD=arm-none-eabi-g++

CPPFLAGS_INCLUDES = -I$(ASFDIR) \
		-I$(ASFDIR)/common/boards                                      \
		-I$(ASFDIR)/common/services/clock                              \
		-I$(ASFDIR)/common/services/delay                              \
		-I$(ASFDIR)/common/services/delay/sam                          \
		-I$(ASFDIR)/common/services/gpio                               \
		-I$(ASFDIR)/common/services/ioport                             \
		-I$(ASFDIR)/common/services/ioport/sam                         \
		-I$(ASFDIR)/common/services/serial                             \
		-I$(ASFDIR)/common/services/serial/sam_uart                    \
		-I$(ASFDIR)/common/services/spi                                \
		-I$(ASFDIR)/common/services/spi/sam_spi                        \
		-I$(ASFDIR)/common/utils                                       \
		-I$(ASFDIR)/common/utils/stdio/stdio_serial                    \
		-I$(ASFDIR)/sam/boards                                         \
		-I$(ASFDIR)/sam/boards/same70_xplained                         \
		-I$(ASFDIR)/sam/drivers/efc                                    \
		-I$(ASFDIR)/sam/drivers/mpu                                    \
		-I$(ASFDIR)/sam/drivers/matrix                                 \
		-I$(ASFDIR)/sam/drivers/pio                                    \
		-I$(ASFDIR)/sam/drivers/pmc                                    \
		-I$(ASFDIR)/sam/drivers/rtt                                    \
		-I$(ASFDIR)/sam/drivers/uart                                   \
		-I$(ASFDIR)/sam/drivers/usart                                  \
		-I$(ASFDIR)/sam/services/flash_efc                             \
		-I$(ASFDIR)/sam/utils                                          \
		-I$(ASFDIR)/sam/utils/cmsis/same70/include                     \
		-I$(ASFDIR)/sam/utils/cmsis/same70/source/templates            \
		-I$(ASFDIR)/sam/utils/fpu                                      \
		-I$(ASFDIR)/sam/utils/header_files                             \
		-I$(ASFDIR)/sam/utils/preprocessor                             \
		-I$(ASFDIR)/thirdparty/CMSIS/Include                           \
		-I$(ASFDIR)/thirdparty/CMSIS/Lib/GCC 						   \
		-I$(GCCLIB)/include              	 						   \
		-I$(SOURCEDIR)/config										   \
		-I$(SOURCEDIR)/logger  										   \
		-I$(SOURCEDIR)/sensors

CPPFLAGS_DEFINES = -D ARM_MATH_CM7=true -D BOARD=SAME70_XPLAINED -D __SAME70Q21__

CFLAGS_WARNINGS = -Wall -Wstrict-prototypes -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith -Wchar-subscripts -Wcomment -Wformat=2 -Wimplicit-int -Wmain -Wparentheses -Wsequence-point -Wreturn-type -Wswitch -Wtrigraphs -Wunused -Wuninitialized -Wunknown-pragmas -Wfloat-equal -Wundef -Wshadow -Wbad-function-cast -Wwrite-strings -Wsign-compare -Waggregate-return -Wmissing-declarations -Wformat -Wmissing-format-attribute -Wno-deprecated-declarations -Wpacked -Wredundant-decls -Wnested-externs -Winline -Wlong-long -Wunreachable-code -Wcast-align

CFLAGS  = -mcpu=cortex-m7 -mthumb -mfloat-abi=softfp -mfpu=fpv5-sp-d16 -g3 -O1 -std=gnu99 -fno-strict-aliasing -ffunction-sections -fdata-sections --param max-inline-insns-single=500

LDLIBS  = -larm_cortexM7lfsp_math_softfp -lm
LDFLAGS = -mcpu=cortex-m7 -mthumb -Wl,--entry=Reset_Handler -Wl,--gc-sections -Wl,-T $(ASFDIR)/sam/utils/linker_scripts/same70/same70q21/gcc/flash.ld -L$(ASFDIR)/thirdparty/CMSIS/Lib/GCC

TARGET = $(BUILDDIR)/nectar-driver.elf

OBJECTS = $(BUILDDIR)/common/services/clock/same70/sysclk.o                         \
		  $(BUILDDIR)/common/services/serial/usart_serial.o                         \
		  $(BUILDDIR)/common/services/delay/sam/cycle_counter.o          			\
          $(BUILDDIR)/common/utils/interrupt/interrupt_sam_nvic.o                   \
          $(BUILDDIR)/common/utils/stdio/read.o                                     \
          $(BUILDDIR)/common/utils/stdio/write.o                                    \
          $(BUILDDIR)/sam/boards/same70_xplained/init.o                             \
          $(BUILDDIR)/sam/drivers/pmc/pmc.o                                         \
          $(BUILDDIR)/sam/drivers/mpu/mpu.o                                         \
          $(BUILDDIR)/sam/drivers/uart/uart.o                                       \
          $(BUILDDIR)/sam/drivers/usart/usart.o                                     \
		  $(BUILDDIR)/sam/drivers/matrix/matrix.o                        			\
          $(BUILDDIR)/sam/utils/cmsis/same70/source/templates/gcc/startup_same70.o  \
          $(BUILDDIR)/sam/utils/cmsis/same70/source/templates/system_same70.o       \
          $(BUILDDIR)/sam/utils/syscalls/gcc/syscalls.o                             \
		  $(BUILDDIR)/logger/logger.o												\
		  $(BUILDDIR)/sensors/coz-ir.o       										\
		  $(BUILDDIR)/sensors/telaire-t6615.o										\
		  $(BUILDDIR)/sensors/trisonica-mini.o										\
          $(BUILDDIR)/main.o

.PHONY : clean default upload bear

default : $(TARGET)

# Rule to build object files from C files found in the ASF directory
$(BUILDDIR)/%.o : $(ASFDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS_DEFINES) $(CPPFLAGS_INCLUDES) $(CFLAGS) $(CFLAGS_WARNINGS) -c $< -o $@

# Rule to build object files from C files found in the source directory
$(BUILDDIR)/%.o : $(SOURCEDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS_DEFINES) $(CPPFLAGS_INCLUDES) $(CFLAGS) $(CFLAGS_WARNINGS) -c $< -o $@

$(TARGET) : $(OBJECTS)
	$(LD) $(LDFLAGS) $(LDLIBS) $^ -o $@

upload : $(TARGET)
	openocd -f board/atmel_same70_xplained.cfg -c "program $< verify reset exit"

clean :
	$(RM) -r $(BUILDDIR)

bear :
	make clean
	CC=/opt/arm-gcc/bin/arm-none-eabi-gcc LD=/opt/arm-gcc/bin/arm-none-eabi-g++ bear -- make -j
