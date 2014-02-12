
## PLATFORM incs
## -------------------------------------------------------------------------------------------------

PLATFORM_INCS := $(CROSS_TARGET)/usr/local/include

## PLATFORM libs
## -------------------------------------------------------------------------------------------------

PLATFORM_LIBS := -L$(CROSS_TARGET)/usr/local/lib
PLATFORM_LIBS +=  -lcJSON -lFFdecsa -lucsi -lsqlite3 -lpthread -lrt -ldl -lm

## PLATFORM cflags
## -------------------------------------------------------------------------------------------------

PLATFORM_CFLAGS :=

## PLATFORM u-boot
## -------------------------------------------------------------------------------------------------

uboot_platform:
	@(ECHO) "Nothing to do ..."

uboot_clean_platform:
	@(ECHO) "Nothing to do ..."

## PLATFORM kernel
## -------------------------------------------------------------------------------------------------

kernel_platform:
	@(ECHO) "Nothing to do ..."

kernel_clean_platform:
	@(ECHO) "Nothing to do ..."

## PLATFORM debug
## -------------------------------------------------------------------------------------------------

debug_platform:
	@$(DB) $(FINAL_TARGET_NAME)

## toolset
## -------------------------------------------------------------------------------------------------

include $(PROJECT_MAKE)/toolset.mk

## end of file
