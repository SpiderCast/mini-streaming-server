## ------------------------------------------------------------------------------------------------------------------ ##
## spidercast.dev@gmail.com
## ------------------------------------------------------------------------------------------------------------------ ##

include $(PROJECT_MAKE)/board_$(TARGET_BOARD).mk

LIBS := -L$(CROSS_TARGET)/usr/local/lib -lsqlite3 -lmxml -lpthread -ldl

OBJS += $(PROJECT_SYS)/string/ixsys_getstr.o

all: dbmaker
	@$(CP) $< $(CROSS_TARGET)/usr/local/bin/

clean:
	@$(RM) *.o
	@$(RM) dbmaker

dbmaker: $(OBJS)
	$(CC) $(INCS) $(CFLAGS) $(OBJS) $(LIBS) -o $@
	@$(RM) $(PROJECT_SYS)/string/ixsys_getstr.o

## ------------------------------------------------------------------------------------------------------------------ ##

DIR_INCS := inc $(PROJECT_INC)/sys $(PLATFORM_INCS)/mxml
DIR_CFLAGS :=

## EOF -------------------------------------------------------------------------------------------------------------- ##
