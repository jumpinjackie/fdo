
LIB =  ../linux/lib/sqlite3.a

FLAG = -DFULLPROTO -D__USE_GNU -DLINUX -DTHREADSAFE=1
INC = 

ANSI_C = 	\
alter.c \
analyze.c \
attach.c \
auth.c \
btree.c \
build.c \
callback.c \
complete.c \
date.c \
delete.c \
expr.c \
fts1.c \
fts1_hash.c \
fts1_porter.c \
fts1_tokenizer1.c \
func.c \
hash.c \
insert.c \
legacy.c \
loadext.c \
main.c \
opcodes.c \
os_unix.c \
pager.c \
parse.c \
pragma.c \
prepare.c \
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
vdbe.c \
vdbeapi.c \
vdbeaux.c \
vdbefifo.c \
vdbemem.c \
vtab.c \
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
