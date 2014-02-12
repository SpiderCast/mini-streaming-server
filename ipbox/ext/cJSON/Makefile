## ------------------------------------------------------------------------------------------------------------------ ##
## spidercast.dev@gmail.com
## ------------------------------------------------------------------------------------------------------------------ ##

include $(PROJECT_MAKE)/board_$(TARGET_BOARD).mk

TARGET := cJSON

all: $(TARGET_LIB)
	@$(MKDIR) $(CROSS_TARGET)/usr/local/include/$(TARGET)
	@install -m 644 cJSON.h $(CROSS_TARGET)/usr/local/include/$(TARGET)/
	@install -m 644 lib$(TARGET).so $(CROSS_TARGET)/usr/local/lib/

$(TARGET_LIB): make_objdir make_objs
	@$(CC) -shared -o lib$(TARGET).so $(OBJS)

clean:
	@$(RM) $(TARGET_LIB)
	@$(RM) $(OBJS) *.so
	@$(RMDIR) $(OBJ_DIR)

## ------------------------------------------------------------------------------------------------------------------ ##

DIR_INCS :=
DIR_CFLAGS := -fPIC

make_objdir:
	@$(MKDIR) $(OBJ_DIR)

make_objs: $(OBJS)
	@$(EXPORT_OBJS)

## EOF -------------------------------------------------------------------------------------------------------------- ##
