include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

add_definitions( -DTHREADSAFE=1 )

set( sqlite3fdo_SRCS
	alter.c
	analyze.c
	attach.c
	auth.c
	btree.c
	build.c
	callback.c
	complete.c
	date.c
	delete.c
	expr.c
	fts1.c
	fts1_hash.c
	fts1_porter.c
	fts1_tokenizer1.c
	func.c
	hash.c
	insert.c
	legacy.c
	loadext.c
	main.c
	opcodes.c
	os.c
	os_unix.c
	pager.c
	parse.c
	pragma.c
	prepare.c
	printf.c
	random.c
	select.c
	shell.c
	table.c
	tokenize.c
	trigger.c
	update.c
	utf.c
	util.c
	vacuum.c
	vdbe.c
	vdbeapi.c
	vdbeaux.c
	vdbefifo.c
	vdbemem.c
	vtab.c
	where.c
)

add_library( sqlite3fdo STATIC ${sqlite3fdo_SRCS} )
