ifndef BUILD_TARGET

CROSS    = arm-none-eabi-
EXTRA_MAKEFILES := $(EXTRA_MAKEFILES) target/common_devo/Makefile.inc

LINKFILE = target/$(TARGET)/$(TARGET).ld
PROTO_LINKFILE = target/$(TARGET)/protocol.ld

LIBOPENCM3 = libopencm3/lib/libopencm3_stm32f1.a

SRC_C    = $(wildcard target/$(TARGET)/*.c) $(wildcard target/common_devo/*.c) $(wildcard target/common_devo/msc2/*.c) $(wildcard target/common_devo/msc2/lib/*.c)

ifdef MODULAR
PROTO_EXTRA_C = $(wildcard target/common_devo/protocol/*.c)
else
SRC_C   += $(wildcard target/common_devo/protocol/*.c) \
           $(wildcard target/common_devo/hid/*.c)
endif

CFLAGS   = -D"assert_param(x)=" -DSTM32F10X_HD -DSTM32F1 -mcpu=cortex-m3 -mthumb -mfix-cortex-m3-ldrd -fdata-sections -ffunction-sections -Itarget/common_devo/msc2/lib -Itarget/common_devo/msc2 -Ilibopencm3/include -fno-builtin-printf -Os
MODULE_FLAGS = -fno-builtin

#LFLAGS   = -nostartfiles -Wl,-gc-sections -Wl,-Map=$(TARGET).map,--cref -nostdlib
LFLAGS   = -nostartfiles -Wl,-gc-sections -Wl,-Map=$(TARGET).map,--cref -lc
LFLAGS2  = -Wl,-T$(LINKFILE)
LFLAGS2OPT  = -Wl,-Ttarget/$(TARGET)/$(TARGET)_opt.ld

#-lnosys

ALL = $(TARGET).dfu

else #BUILD_TARGET

$(TARGET).bin: $(TARGET).elf
ifdef OPTIMIZE_DFU
	@echo " + Optimizing placement and re-linking"
	../utils/repack_ld.pl -mapfile $(TARGET).map -size $(CRC_OFFSET) > objs/optimize.ld
	$(CC) -o $< $(OBJS) $(LIBOPENCM3) $(LFLAGS) $(LFLAGS2OPT) $(CFLAGS)
endif
ifdef MODULAR # the following builds a symbol file with thumb addresses (address+1 for functions, address+0 for variables)
	$(DUMP) -t $< | ../utils/build_symbolfile.pl > $(SYMBOL_FILE)
endif
	$(CP) -O binary $< $@
	$(DUMP) -S $< > $(TARGET).list

$(LIBOPENCM3):
	make -C libopencm3 TARGETS=stm32/f1 lib

$(TARGET).dfu: $(TARGET).bin
	../utils/dfu.py --name "$(HGVERSION) Firmware" $(DFU_ARGS):$< $@
	../utils/get_mem_usage.pl $(TARGET).map

###################################
#The protocol modules (if needed) #
###################################
$(ODIR)/%.bin: $(ODIR)/%.o_ $(PROTO_LINKFILE) $(PROTO_EXTRA_OBJS) $(TARGET).bin
	@echo " + Building '$@'"
	$(LD) -o $(@:.bin=.elf) $< $(PROTO_EXTRA_OBJS) -gc-sections -Map=$(@:.bin=.map) \
		--cref -T$(PROTO_LINKFILE) --just-symbols=$(SYMBOL_FILE)
	$(DUMP) -t $(@:.bin=.elf) | grep -q _from_thumb; if [ $$? -eq 0 ]; then echo "ERROR: Bad address found"; false; else true; fi
	$(CP) -O binary  $(@:.bin=.elf) $@

endif #BUILD_TARGET
