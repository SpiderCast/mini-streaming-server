include $(PROJECT_MAKE)/baserule.mk

## slash
## -------------------------------------------------------------------------------------------------

UNIX_SLASH := /
DOS_SLASH  := $(subst /,\,$(UNIX_SLASH))

## shell environment
## -------------------------------------------------------------------------------------------------

DATE  := $(shell date '+%Y%m%d-%H%M')
CD    := cd
CP    := cp -fp 2>/dev/null
ECHO  := echo
RM    := rm -f 2>/dev/null
RMDIR := rm -rf 2>/dev/null
MKDIR := mkdir -p 2>/dev/null
REN   := mv 2>/dev/null

GOOD_SLASH := $(UNIX_SLASH)
BAD_SLASH  := $(DOS_SLASH)
SLASH      := $(UNIX_SLASH)

## toolchain
## -------------------------------------------------------------------------------------------------

CC      := $(CROSS_COMPILE)gcc
CXX     := $(CROSS_COMPILE)g++
AR      := $(CROSS_COMPILE)ar
LD      := $(CROSS_COMPILE)gcc
LINK    := $(CROSS_COMPILE)gcc
OBJCOPY := $(CROSS_COMPILE)objcopy
OBJDUMP := $(CROSS_COMPILE)objdump
DB      := $(CROSS_COMPILE)gdb

## toolchain
## -------------------------------------------------------------------------------------------------

OBJ_SUFFIX := .o
LIB_PREFIX := lib
LIB_SUFFIX := .a
EXE_SUFFIX := .out

## default build rules
## -------------------------------------------------------------------------------------------------

%.o: %.c
	@$(ECHO) Compiling $<
ifeq "$(PROJECT_DEBUG)" "1"
	$(COMPILE_C)
else
	@$(COMPILE_C)
endif

%.o: %.cc
	@$(ECHO) Compiling $<
ifeq "$(PROJECT_DEBUG)" "1"
	$(COMPILE_CC)
else
	@$(COMPILE_CC)
endif

%.o: %.cpp
	@$(ECHO) Compiling $<
ifeq "$(PROJECT_DEBUG)" "1"
	$(COMPILE_CC)
else
	@$(COMPILE_CC)
endif

## build macros
## -------------------------------------------------------------------------------------------------

define COMPILE_C
	$(CC) $(INCS) $(CFLAGS) -c $< -o $@
endef

define COMPILE_CC
	$(CXX) $(INCS) $(CFLAGS) -c $< -o $@
endef

define BUILD_LIB
	$(ECHO) Building Library - $@
	$(AR) rcs $(OBJ_DIR)/$@ $(OBJ_DIR)/*$(OBJ_SUFFIX)
	$(CP) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(OBJ_DIR)/$@) $(PROJECT_EXPORT)
endef

define BUILD_SUBDIRS
	@for DIR in $(SUBDIRS); do \
		$(MAKE) -C $$DIR all OBJ_DIR=$(subst $(BAD_SLASH),$(GOOD_SLASH),$(OBJ_DIR)); \
	done
endef

define CLEAN_SUBDIRS
	@for DIR in $(SUBDIRS); do \
		$(MAKE) -C $$DIR clean; \
	done
endef

define EXPORT_OBJS
	$(CP) $(OBJS) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(OBJ_DIR))
endef

define ARCHIVE_LIBS
$(ECHO) Creating Library - $@
$(if $(wildcard $@),@$(RM) $@)
$(if $(wildcard ar.mac),@$(RM) ar.mac)
$(if $(filter lib%.a,$^),
	$(ECHO) CREATE $(PROJECT_EXPORT)/$@ > ar.mac
	$(ECHO) SAVE >> ar.mac
	$(ECHO) END >> ar.mac
	$(AR) -M < ar.mac
)
$(if $(filter %$(OBJ_SUFFIX),$^),@$(AR) -q $@ $(filter %$(OBJ_SUFFIX),$^))
$(if $(filter $(LIB_PREFIX)%$(LIB_SUFFIX),$^),
	$(ECHO) OPEN $(PROJECT_EXPORT)/$@ > ar.mac
	for LIB in $(filter $(LIB_PREFIX)%$(LIB_SUFFIX),$^); do echo ADDLIB $(PROJECT_EXPORT)/"$$LIB" >> ar.mac; done
	$(ECHO) SAVE >> ar.mac
	$(ECHO) END >> ar.mac
	$(AR) -M < ar.mac
)
$(AR) -s $(PROJECT_EXPORT)/$@
$(RM) ar.mac
endef

## end of file
