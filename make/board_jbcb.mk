
## PLATFORM incs
## -------------------------------------------------------------------------------------------------

PLATFORM_INCS := $(CROSS_TARGET)/usr/local/include

## PLATFORM libs
## -------------------------------------------------------------------------------------------------

PLATFORM_LIBS := -L$(CROSS_TARGET)/usr/local/lib
PLATFORM_LIBS += -lpthread -lrt -ldl -lm -lcJSON -lFFdecsa -lucsi -lsqlite3

## PLATFORM cflags
## -------------------------------------------------------------------------------------------------

PLATFORM_CFLAGS := -march=armv7-a -mtune=cortex-a8 -mfpu=neon -ftree-vectorize -mthumb

## PLATFORM u-boot
## -------------------------------------------------------------------------------------------------

uboot_platform:
	@$(MAKE) -C $(CROSS_UBOOT) am335x_boneblack -j$(HOST_N_CORES)

uboot_clean_platform:
	@$(MAKE) -C $(CROSS_UBOOT) distclean

## PLATFORM kernel
## -------------------------------------------------------------------------------------------------

kernel_platform:
	@$(CP) $(PROJECT_MAKE)/kconfig/$(TARGET_CHIPSET)_$(TARGET_BOARD)_kconfig $(CROSS_KDIR)/.config
ifeq "$(MENUCONFIG)" "1"
	@$(MAKE) -C $(CROSS_KDIR) menuconfig -j$(HOST_N_CORES)
	@$(CP) $(CROSS_KDIR)/.config $(PROJECT_MAKE)/kconfig/$(TARGET_CHIPSET)_$(TARGET_BOARD)_kconfig
endif
	@$(MAKE) -C $(CROSS_KDIR) uImage dtbs modules -j$(HOST_N_CORES)
	@$(MAKE) -C $(CROSS_KDIR) uImage-dtb.am335x-boneblack -j$(HOST_N_CORES)
	@$(MAKE) -C $(CROSS_KDIR) INSTALL_MOD_PATH=$(CROSS_TARGET) modules_install -j$(HOST_N_CORES)

kernel_clean_platform:
	@$(MAKE) -C $(CROSS_KDIR) distclean

## PLATFORM debug
## -------------------------------------------------------------------------------------------------

build_script_debug:
	@$(ECHO) file $(FINAL_TARGET_NAME) > $(TARGET_BOARD)_$(TARGET_CHIPSET)_gdb.cmd
	@$(ECHO) target remote $(TARGET_BOARD_IP):$(TARGET_DEBUG_PORT) >> $(TARGET_BOARD)_$(TARGET_CHIPSET)_gdb.cmd
	@$(ECHO) tbreak main >> $(TARGET_BOARD)_$(TARGET_CHIPSET)_gdb.cmd

debug_platform: build_script_debug
	@$(ECHO) "==================================================================================================="
	@$(ECHO) "INFO: You are trying to debug the SDK application in user space..."
	@$(ECHO) "INFO: Please note that you need first to run the gdbserver on the target platform with:"
	@$(ECHO) "INFO: gdbserver :$(TARGET_DEBUG_PORT) target_file_name"
	@$(ECHO) "==================================================================================================="
	@$(DB) --command $(TARGET_BOARD)_$(TARGET_CHIPSET)_gdb.cmd
	@$(RM) $(TARGET_BOARD)_$(TARGET_CHIPSET)_gdb.cmd

## toolset
## -------------------------------------------------------------------------------------------------

include $(PROJECT_MAKE)/toolset.mk

## end of file
