
## include
## -------------------------------------------------------------------------------------------------

BASIC_INCS := $(PROJECT_INC)

## compile flags
## -------------------------------------------------------------------------------------------------

BASIC_CFLAGS := -Wall -fno-strict-aliasing
ifeq "$(PROJECT_DEBUG)" "1"
  BASIC_CFLAGS += -g -DPROJECT_DEBUG
endif
ifneq "$(PROJECT_OPTLEVEL)" ""
  BASIC_CFLAGS += -O$(PROJECT_OPTLEVEL)
else
  BASIC_CFLAGS += -O0
endif

## include / compile option
## -------------------------------------------------------------------------------------------------

INCS = $(addprefix -I,$(BASIC_INCS)) $(addprefix -I,$(PLATFORM_INCS)) $(addprefix -I,$(DIR_INCS))
CFLAGS = $(BASIC_CFLAGS) $(PLATFORM_CFLAGS) $(DIR_CFLAGS)

## link flags
## -------------------------------------------------------------------------------------------------

LDFLAGS := $(PLATFORM_LIBS)

## object rules
## -------------------------------------------------------------------------------------------------

OBJS = $(C_OBJS) $(CXX_OBJS)

C_OBJS   := $(patsubst %.c,%.o,$(wildcard *.c))
CXX_OBJS := $(patsubst %.cc,%.o,$(wildcard *.cc))
CXX_OBJS += $(patsubst %.cpp,%.o,$(wildcard *.cpp))

## library rules
## -------------------------------------------------------------------------------------------------

TARGET_LIB = $(LIB_PREFIX)$(TARGET)$(LIB_SUFFIX)

## export path
## -------------------------------------------------------------------------------------------------

OBJ_DIR = $(CURDIR)/_obj

## target name
## -------------------------------------------------------------------------------------------------

FINAL_TARGET_NAME := $(PROJECT_NAME).out

## end of file
