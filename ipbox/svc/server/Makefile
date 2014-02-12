## ------------------------------------------------------------------------------------------------------------------ ##
## spidercast.dev@gmail.com
## ------------------------------------------------------------------------------------------------------------------ ##

include $(PROJECT_MAKE)/board_$(TARGET_BOARD).mk

TARGET := server
SUBDIRS :=

all: $(TARGET_LIB)

$(TARGET_LIB): make_objdir make_objs
	@$(BUILD_SUBDIRS)
	@$(BUILD_LIB)

clean:
	@$(RM) $(TARGET_LIB)
	@$(RM) $(OBJS)
	@$(CLEAN_SUBDIRS)
	@$(RMDIR) $(OBJ_DIR)

## ------------------------------------------------------------------------------------------------------------------ ##

DIR_INCS := $(PROJECT_INC)/svc $(PROJECT_INC)/sys $(PROJECT_INC)/stb
DIR_CFLAGS :=

make_objdir:
	@$(MKDIR) $(OBJ_DIR)

make_objs: $(OBJS)
	@$(EXPORT_OBJS)

## EOF -------------------------------------------------------------------------------------------------------------- ##
