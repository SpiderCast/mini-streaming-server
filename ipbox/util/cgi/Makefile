## ------------------------------------------------------------------------------------------------------------------ ##
## spidercast.dev@gmail.com
## ------------------------------------------------------------------------------------------------------------------ ##

include $(PROJECT_MAKE)/board_$(TARGET_BOARD).mk

LIBS := -L$(CROSS_TARGET)/usr/local/lib -lsqlite3 -lcJSON -lpthread -lm -ldl
LIBS +=

OBJS += $(PROJECT_SYS)/string/ixsys_getstr.o

all: index
	@$(CP) $<.cgi $(CROSS_TARGET)/var/www/cgi-bin/

clean:
	@$(RM) *.o
	@$(RM) *.cgi

index: $(OBJS)
	$(CC) $(INCS) $(CFLAGS) $(OBJS) $(LIBS) -o $@.cgi
	@$(RM) $(PROJECT_SYS)/string/ixsys_getstr.o

## ------------------------------------------------------------------------------------------------------------------ ##

DIR_INCS := inc $(PROJECT_INC)/sys
DIR_CFLAGS :=

## EOF -------------------------------------------------------------------------------------------------------------- ##
