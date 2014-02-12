## ------------------------------------------------------------------------------------------------------------------ ##
## spidercast.dev@gmail.com
## ------------------------------------------------------------------------------------------------------------------ ##

include $(PROJECT_MAKE)/board_$(TARGET_BOARD).mk

.PHONY: $(TARGET_LIB)

## help ------------------------------------------------------------------------------------------------------------- ##

help:
	@$(ECHO) "##########################################"
	@$(ECHO) "## make all                             ##"
	@$(ECHO) "## make debug                           ##"
	@$(ECHO) "##########################################"

## component - ext -------------------------------------------------------------------------------------------------- ##

ext: create_export_dir
	@$(MAKE) -C $(PROJECT_EXT)/mini_httpd all
	@$(MAKE) -C $(PROJECT_EXT)/cJSON all
	@$(MAKE) -C $(PROJECT_EXT)/mxml all
	@$(MAKE) -C $(PROJECT_EXT)/dvb-apps all install
	@$(MAKE) -C $(PROJECT_EXT)/FFdecsa all
	@$(MAKE) -C $(PROJECT_EXT)/sqlite3 all

clean_ext:
	@$(MAKE) -C $(PROJECT_EXT)/mini_httpd clean
	@$(MAKE) -C $(PROJECT_EXT)/cJSON clean
	@$(MAKE) -C $(PROJECT_EXT)/mxml clean
	@$(MAKE) -C $(PROJECT_EXT)/dvb-apps clean
	@$(MAKE) -C $(PROJECT_EXT)/FFdecsa clean
	@$(MAKE) -C $(PROJECT_EXT)/sqlite3 clean
	@$(RMDIR) $(CROSS_TARGET)/usr

## component - STB -------------------------------------------------------------------------------------------------- ##

STB_PARTS := frontend scan demux player

PRJ_STB_BUILD := $(LIB_PREFIX)stb$(LIB_SUFFIX)
PRJ_STB_CLEAN = $(STB_CLEAN)

STB_BUILD := $(addprefix $(LIB_PREFIX),$(addsuffix $(LIB_SUFFIX),$(STB_PARTS)))
STB_CLEAN := $(addsuffix _clean,$(STB_PARTS))

$(STB_BUILD):
	@$(MAKE) -C $(wildcard $(PROJECT_STB)/$(strip $(patsubst $(LIB_PREFIX)%$(LIB_SUFFIX),%,$(@F)))) all

$(STB_CLEAN):
	@$(MAKE) -C $(wildcard $(PROJECT_STB)/$(strip $(patsubst %_clean,%,$(@F)))) clean

$(PRJ_STB_BUILD): $(STB_BUILD)
	@$(ECHO) Creating STB library
	@$(ARCHIVE_LIBS)

stb: $(PRJ_STB_BUILD)

clean_stb: $(PRJ_STB_CLEAN)

## component - svc -------------------------------------------------------------------------------------------------- ##

SVC_PARTS := server streamer epg

PRJ_SVC_BUILD := $(LIB_PREFIX)svc$(LIB_SUFFIX)
PRJ_SVC_CLEAN = $(SVC_CLEAN)

SVC_BUILD := $(addprefix $(LIB_PREFIX),$(addsuffix $(LIB_SUFFIX),$(SVC_PARTS)))
SVC_CLEAN := $(addsuffix _clean,$(SVC_PARTS))

$(SVC_BUILD):
	@$(MAKE) -C $(wildcard $(PROJECT_SVC)/$(strip $(patsubst $(LIB_PREFIX)%$(LIB_SUFFIX),%,$(@F)))) all

$(SVC_CLEAN):
	@$(MAKE) -C $(wildcard $(PROJECT_SVC)/$(strip $(patsubst %_clean,%,$(@F)))) clean

$(PRJ_SVC_BUILD): $(SVC_BUILD)
	@$(ECHO) Creating SVC library
	@$(ARCHIVE_LIBS)

svc: $(PRJ_SVC_BUILD)

clean_svc: $(PRJ_SVC_CLEAN)

## component - sys -------------------------------------------------------------------------------------------------- ##

SYS_PARTS := init msgq db string

PRJ_SYS_BUILD := $(LIB_PREFIX)sys$(LIB_SUFFIX)
PRJ_SYS_CLEAN = $(SYS_CLEAN)

SYS_BUILD := $(addprefix $(LIB_PREFIX),$(addsuffix $(LIB_SUFFIX),$(SYS_PARTS)))
SYS_CLEAN := $(addsuffix _clean,$(SYS_PARTS))

$(SYS_BUILD):
	@$(MAKE) -C $(wildcard $(PROJECT_SYS)/$(strip $(patsubst $(LIB_PREFIX)%$(LIB_SUFFIX),%,$(@F)))) all

$(SYS_CLEAN):
	@$(MAKE) -C $(wildcard $(PROJECT_SYS)/$(strip $(patsubst %_clean,%,$(@F)))) clean

$(PRJ_SYS_BUILD): $(SYS_BUILD)
	@$(ECHO) Creating SYS library
	@$(ARCHIVE_LIBS)

sys: $(PRJ_SYS_BUILD)

clean_sys: $(PRJ_SYS_CLEAN)

## component - util ------------------------------------------------------------------------------------------------- ##

UTIL_PARTS := cgi dbmaker

UTIL_BUILD := $(UTIL_PARTS)
UTIL_CLEAN := $(addsuffix _clean,$(UTIL_PARTS))

$(UTIL_BUILD): create_export_dir
	@$(MAKE) -C $(wildcard $(PROJECT_UTIL)/$@) all

$(UTIL_CLEAN):
	@$(MAKE) -C $(wildcard $(PROJECT_UTIL)/$(strip $(patsubst %_clean,%,$(@F)))) clean

util: $(UTIL_BUILD)

clean_util: $(UTIL_CLEAN)

## target ----------------------------------------------------------------------------------------------------------- ##

TARGET_COMPONENT := sys svc stb

TARGET_LIB = $(LIB_PREFIX)$(PROJECT_NAME)$(LIB_SUFFIX)

TARGET_COMPONENT_CLEAN = $(addprefix clean_,$(TARGET_COMPONENT))

all:
	@$(MAKE) create_export_dir --quiet --ignore-errors
	@$(MAKE) $(TARGET_LIB)
	@$(ECHO) Creating $(PROJECT_NAME) target
	@$(LINK) $(PROJECT_EXPORT)/$(TARGET_LIB) $(LDFLAGS) -o $(FINAL_TARGET_NAME) -Wl,-Map=$(PROJECT_NAME).map
	@$(CP) $(FINAL_TARGET_NAME) $(CROSS_TARGET)/root

$(TARGET_LIB): $(addprefix $(LIB_PREFIX),$(addsuffix $(LIB_SUFFIX),$(TARGET_COMPONENT)))
	@$(ECHO) Creating TARGET library
	@$(ARCHIVE_LIBS)

clean: $(TARGET_COMPONENT_CLEAN)
	-@$(RM) $(FINAL_TARGET_NAME)
	-@$(RM) $(PROJECT_NAME).map
	-@$(RM) $(PROJECT_NAME).dbg
	-@$(RM) $(PROJECT_EXPORT)/*.a
	-@$(RMDIR) $(CROSS_TARGET)/root

create_export_dir:
	@tar xfz target.tar.gz -C $(PROJECT_EXPORT)

install:
	@sudo $(CP) -r $(CROSS_TARGET)/* $(TARGET_ROOTFS)/

## uboot & kernel---------------------------------------------------------------------------------------------------- ##

uboot: uboot_platform

uboot_clean: uboot_clean_platform

kernel: kernel_platform

kernel_clean: kernel_clean_platform

## debugging -------------------------------------------------------------------------------------------------------- ##

debug: debug_platform

## EOF -------------------------------------------------------------------------------------------------------------- ##
