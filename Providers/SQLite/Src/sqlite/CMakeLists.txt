set( sqlitefdo3_SRCS 
	alter.c
	analyze.c
	attach.c
	auth.c
	bitvec.c
	btmutex.c
	btree.c
	build.c
	backup.c
	callback.c
	complete.c
	date.c
	delete.c
	expr.c
	fault.c
	fts3.c
	fts3_expr.c
	fts3_hash.c
	fts3_icu.c
	fts3_porter.c
	fts3_tokenizer1.c
	fts3_tokenizer.c
	func.c
	global.c
	hash.c
	insert.c
	journal.c
	legacy.c
	loadext.c
	main.c
	malloc.c
	mem0.c
	mem1.c
	mem2.c
	mem3.c
	mem5.c
	memjournal.c
	mutex.c
	mutex_noop.c
	mutex_unix.c
	notify.c
	opcodes.c
	os.c
	os_unix.c
	pager.c
	parse.c
	pcache.c
	pcache1.c
	pragma.c
	prepare.c
	printf.c
	random.c
	resolve.c
	rowset.c
	rtree.c
	select.c
	shell.c
	status.c
	table.c
	tokenize.c
	trigger.c
	update.c
	utf.c
	util.c
	vacuum.c
	vdbeapi.c
	vdbeaux.c
	vdbeblob.c
	vdbe.c
	vdbemem.c
	vtab.c
	walker.c
	where.c
) 

add_definitions( 
	-DSQLITE_ENABLE_COLUMN_METADATA 
	-DSQLITE_OMIT_TRACE 
	-DSQLITE_OMIT_EXPLAIN 
	-DSQLITE_OMIT_PROGRESS_CALLBACK 
	-DSQLITE_OMIT_AUTHORIZATION
	-DSQLITE_OMIT_UTF16 
	-DSQLITE_OMIT_SHARED_CACHEXXX 
	-DSQLITE_THREADSAFE=2 
	-DSQLITE_ENABLE_RTREE 
	-DSQLITE_CORE
	)

add_library ( sqlitefdo3 SHARED  ${sqlitefdo3_SRCS} )

set_target_properties( sqlitefdo3 PROPERTIES VERSION 3 SOVERSION 6 )

target_link_libraries( sqlitefdo3
	pthread
	dl
	)


