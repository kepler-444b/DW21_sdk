
# -----------------------------------------------------------------------------------------
#Purpose:one click build
#Copyright CompanyNameMagicTag 2018-2019.All rights reserved
#Author: CompanyName
#------------------------------------------------------------------------------------------

# Dependency script dependency.xml, 
# which starts compilation according to the configuration of dependency.xml.

include $(MAIN_TOPDIR)/build_tmp/config/soc_product_config.mk

ifeq ($(PRODUCT_CFG_BUILD_RELEASE), yes)
MAIN_OUT_DIR_NAME = $(project_name)_release
else
MAIN_OUT_DIR_NAME = $(project_name)_debug
endif



MAIN_BUILD_TMP_OUT :=	$(MAIN_TOPDIR)/build_tmp/$(MAIN_OUT_DIR_NAME)
ALL_LIBS := $(MAIN_BUILD_TMP_OUT)/all/lib

MAIN_OUTPUT :=  $(MAIN_TOPDIR)/output/$(MAIN_OUT_DIR_NAME)

LD_PATH := $(MAIN_TOPDIR)/target/socbuilder/ld_scripts
SYM_PATH := $(MAIN_TOPDIR)/target/socbuilder/symtab
HUPGDIR := $(MAIN_OUTPUT)/mk_image/image

SYM_TABLE := $(SYM_PATH)/Firmware_Base.sym

LDFLAGS = -nostdlib -static --gc-sections -L$(ALL_LIBS) -L$(LD_PATH) -T$(LD_PATH)/LiteOS_UseRemap_Firmware.lds
ifeq ($(HB_LITEOS_COMPILE_TESTCASE), YES)
LDFLAGS = --nostartfiles -nostdlib -static --gc-sections -L$(ALL_LIBS) -L$(LD_PATH) -T$(LD_PATH)/LiteOS_UseRemap_ROM_Test.lds
endif

empty :=   
comma :=,
app_list := $(subst $(comma), $(empty), $(app_str))
dev_str := $(subst $(comma), $(empty), $(dev_list))

RELATIVE_DIR := ../
MOD_INCLUDE += $(addprefix -I $(RELATIVE_DIR),$(shell cat $(RELATIVE_DIR)/Makefile.inc))

.PHONY: prepare build_ver link hupg

prepare:
	@$(foreach X,$(dev_str),\
	rm -rf $(MAIN_BUILD_TMP_OUT)/$(X)/bin;\
	mkdir -p $(MAIN_BUILD_TMP_OUT)/$(X)/bin;)
# Generate MAGIC values ??for different apps
	@python3  $(MAIN_TOPDIR)/build/magic/magic.py  $(app_str)

make_libs:
	@echo "**********make libs**************"
	-@mkdir -p $(ALL_LIBS)
	if [ -d "$(MAIN_TOPDIR)/target/libs/sdk_libs/all" ]; \
	then \
	cp -rf  $(MAIN_TOPDIR)/target/libs/sdk_libs/all/*  $(ALL_LIBS) ;\
	fi

	$(foreach X,$(dev_str),\
	if [ -d "$(MAIN_TOPDIR)/target/libs/sdk_libs/$(X)" ] ;\
	then \
	cp -rf  $(MAIN_TOPDIR)/target/libs/sdk_libs/$(X)/* $(MAIN_BUILD_TMP_OUT)/$(X)/lib ;\
	fi;)

build_ver:
	@echo "**********build version***********"
	$(Q)$(foreach X,$(app_list),\
	$(RM) $(ALL_LIBS)/hb_ver_cfg.o; \
	$(CC) $(CFLAGS) $(MOD_INCLUDE) -D$(X) -c $(MAIN_TOPDIR)/target/socbuilder/hb_cfg/hb_ver_cfg.c -o	$(ALL_LIBS)/hb_ver_cfg.o; \
	$(foreach Y,$(dev_str), \
	$(AR) -d $(MAIN_BUILD_TMP_OUT)/$(Y)/lib/app/lib$(X).a hb_ver_cfg.o; \
	$(AR) -q $(MAIN_BUILD_TMP_OUT)/$(Y)/lib/app/lib$(X).a $(ALL_LIBS)/hb_ver_cfg.o;))

# It took too long to delete, so it is no longer deleted.
# @rm -rf $(ALL_LIBS)/libgcc/*
# Parse xxx_build_target.xml
link: prepare build_ver make_libs
	@echo "start link"
	$(Q) $(RM) $(LD_PATH)/system_config.ld
	$(CC) -E -I $(MAIN_TOPDIR)/rom/bsp/system/os/Huawei_LiteOS/kernel/include  -I $(MAIN_TOPDIR)/bsp/system/os/Huawei_LiteOS/targets/dw21v100/config  $(LD_PATH)/system_config.ld.S -o $(LD_PATH)/system_config.ld -P	

	$(foreach X,$(app_list),\
	$(foreach Y,$(dev_str), \
	$(LD) -N --just-symbols=${SYM_TABLE} $(LDFLAGS) -L$(MAIN_BUILD_TMP_OUT)/$(Y)/lib/ -L$(MAIN_BUILD_TMP_OUT)/$(Y)/lib/app/ -Map=$(MAIN_BUILD_TMP_OUT)/$(Y)/bin/dw21_$(Y)_$(X).map -o $(MAIN_BUILD_TMP_OUT)/$(Y)/bin/dw21_$(Y)_$(X).out --start-group $(patsubst %,-l%,$(subst lib, ,$(basename $(notdir $(wildcard  $(MAIN_BUILD_TMP_OUT)/$(Y)/lib/*.a $(MAIN_BUILD_TMP_OUT)/$(Y)/lib/app/lib$(X).a $(ALL_LIBS)/*.a))))) --end-group;\
	$(OBJCOPY) -O binary  $(MAIN_BUILD_TMP_OUT)/$(Y)/bin/dw21_$(Y)_$(X).out $(MAIN_BUILD_TMP_OUT)/$(Y)/bin/dw21_$(Y)_$(X).bin;\
	$(OBJDUMP) -d $(MAIN_BUILD_TMP_OUT)/$(Y)/bin/dw21_$(Y)_$(X).out >$(MAIN_BUILD_TMP_OUT)/$(Y)/bin/dw21_$(Y)_$(X).asm;\
	dd if=$(MAIN_BUILD_TMP_OUT)/$(Y)/bin/dw21_$(Y)_$(X).bin of=$(MAIN_BUILD_TMP_OUT)/$(Y)/bin/dw21_$(Y)_$(X)_flash.bin count=1 bs=1024;\
	dd if=$(MAIN_TOPDIR)/target/libs/rom_bin/dw21_rom_data.bin of=$(MAIN_BUILD_TMP_OUT)/$(Y)/bin/dw21_$(Y)_$(X)_flash.bin seek=1 bs=1024;\
	dd if=$(MAIN_BUILD_TMP_OUT)/$(Y)/bin/dw21_$(Y)_$(X).bin of=$(MAIN_BUILD_TMP_OUT)/$(Y)/bin/dw21_$(Y)_$(X)_flash.bin skip=4 seek=4 bs=1024;\
	mv $(MAIN_BUILD_TMP_OUT)/$(Y)/bin/dw21_$(Y)_$(X)_flash.bin $(MAIN_BUILD_TMP_OUT)/$(Y)/bin/dw21_$(Y)_$(X).bin;\
	python3 $(SYM_PATH)/../scripts/arm_thumb_asm_check.py $(MAIN_BUILD_TMP_OUT)/$(Y)/bin/dw21_$(Y)_$(X).asm $(SYM_PATH)/Firmware_Base.sym $(SYM_PATH)/rom_expt.json;))

	#$(Q)$(foreach X,$(app_list),\
	#$(foreach Y,$(dev_str), \
	#if (cmp -s $(MAIN_BUILD_TMP_OUT)/$(Y)/bin/dw21_$(Y)_$(X)_rom.bin $(MAIN_TOPDIR)/target/libs/rom_bin/dw21_rom.bin);\
	#then echo "#########DW21 $(Y)  $(X) ROM BIN IS THE SAME!#########";\
	#else echo "#########DW21 $(Y)  $(X) ROM BIN IS DIFFERENT!#########";\
	#exit 1;\
	#fi;))

# $(foreach Y,$(dev_str), \
# rm -rf  $(MAIN_BUILD_TMP_OUT)/$(Y)/debug;\
# mkdir -p  $(MAIN_BUILD_TMP_OUT)/$(Y)/debug;\
# cp $(ALL_LIBS)/* $(MAIN_BUILD_TMP_OUT)/$(Y)/debug/;\
# cp $(MAIN_BUILD_TMP_OUT)/$(Y)/lib/* $(MAIN_BUILD_TMP_OUT)/$(Y)/debug/;\
# rm $(MAIN_BUILD_TMP_OUT)/$(Y)/debug/librom.o;)


# @echo "*****************ELF SEGMENGT SIZE BEFORE LINK*******************"
# @$(foreach Y,$(dev_str), \
# size -t $(MAIN_BUILD_TMP_OUT)/$(Y)/debug/*.o $(MAIN_BUILD_TMP_OUT)/$(Y)/debug/*.a > $(MAIN_BUILD_TMP_OUT)/$(Y)/debug/before_link_size.log;\
# head -n 1  $(MAIN_BUILD_TMP_OUT)/$(Y)/debug/before_link_size.log;\
# tail -n 1  $(MAIN_BUILD_TMP_OUT)/$(Y)/debug/before_link_size.log;)
# @echo "*****************************************************************"

link_check:
	$(Q)$(foreach X,$(app_list),\
	$(foreach Y,$(dev_str), \
	if [ ! -f $(MAIN_BUILD_TMP_OUT)/$(Y)/bin/dw21_$(Y)_$(X).bin ] || [ ! -f $(MAIN_BUILD_TMP_OUT)/$(Y)/bin/dw21_$(Y)_$(X).out ] || [ ! -f $(MAIN_BUILD_TMP_OUT)/$(Y)/bin/dw21_$(Y)_$(X).map ]; \
	then \
	echo -n "HB BUILD link [FAILURE]"; \
	exit 1;\
	fi;))
	echo -n "HB BUILD link [SUCCESS]" ;
	exit 0;

hupg:
	-@rm -rf $(MAIN_OUTPUT)/mk_image
	$(foreach Y,$(dev_str), \
	rm -rf $(HUPGDIR)/*$(Y)*.hupg;\
	rm -rf $(HUPGDIR)/*$(Y)*.hbin;)
	python3 hupg/pkt_builder.py

FILE = $(MAIN_OUTPUT)/mk_image/image/image_list.txt
IMAGELIST = $(shell awk '{print $$1}' $(FILE))

# Verify the installation package is correct
hupg_check:
	@if [ -f $(FILE) ]; \
	then \
	echo -n "image_listalready generated"; \
	else \
	echo -n "image_list not generated"; \
	echo -n "HB BUILD hupg_check [FAILURE]"; \
	exit 1;\
	fi;
	@for image in $(IMAGELIST);\
	do if [ -f $(HUPGDIR)/$$image ]; \
	then \
	echo $$image;\
	else \
	echo -n "HB BUILD hupg_check [FAILURE]" ;\
	exit 1;\
	fi;\
	done
	@echo -n "HB BUILD hupg_check [SUCCESS]"
