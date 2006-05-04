
LIB =  ../linux/lib/sqlite3.a

FLAG = -DFULLPROTO -D__USE_GNU -DLINUX -DTHREADSAFE=1
INC = 

ANSI_C = 	\
alter.c \
attach.c \
auth.c \
btree.c \
build.c \
date.c \
delete.c \
expr.c \
func.c \
hash.c \
insert.c \
legacy.c \
main.c \
opcodes.c \
os_unix.c \
os_win.c \
pager.c \
parse.c \
pragma.c \
printf.c \
random.c \
select.c \
shell.c \
table.c \
tokenize.c \
trigger.c \
update.c \
utf.c \
util.c \
vacuum.c \
vdbeapi.c \
vdbeaux.c \
vdbe.c \
vdbemem.c \
where.c

ANSI_C-OBJ = $(patsubst %.c,$(LIB)(%.o),$(ANSI_C))

all:: init_dirs $(LIB)

lib:: $(LIB)

bin:: 

init_dirs::
	@mkdir -p ../linux/lib

share::

clean::
	@rm -f $(ANSI_C:$(suffix $(firstword $(ANSI_C)))=.d)
	@rm -f $(LIB)

$(LIB):: $(ANSI_C-OBJ)

-include $(ANSI_C:$(suffix $(firstword $(ANSI_C)))=.d)

$(subst $(suffix $(firstword $(ANSI_C))),.d, $(ANSI_C)): %.d : %$(suffix $(firstword $(ANSI_C)))
	@gcc $(FLAG) $(INC) $(CF) -c -MMD -MT $(LIB)\($*\.o\) $<

$(filter $(LIB)(%.o), $(ANSI_C-OBJ)): $(LIB)(%.o) : %.c
	@gcc $(FLAG) $(INC) $(CF) -c $<
	@ar r $@ $% 
	@rm $%
