##
## Copyright (C) 2013  Autodesk, Inc.
## 
## This library is free software; you can redistribute it and/or
## modify it under the terms of version 2.1 of the GNU Lesser
## General Public License as published by the Free Software Foundation.
## 
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
## 
## You should have received a copy of the GNU Lesser General Public
## License along with this library; if not, write to the Free Software
## Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
##

AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libSqlite3.la

libSqlite3_la_SOURCES = \
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
	os.c \
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

INCLUDES = \
  -I/usr/include

libSqlite3_la_LDFLAGS =

CFLAGS = @CXXFLAGS_SQLITE@ -DSQLITE_ENABLE_COLUMN_METADATA -DSQLITE_OMIT_TRACE -DSQLITE_OMIT_PROGRESS_CALLBACK -DSQLITE_OMIT_AUTHORIZATION -DSQLITE_OMIT_UTF16 -DSQLITE_OMIT_SHARED_CACHEXXX -DSQLITE_THREADSAFE=2 -DSQLITE_ENABLE_RTREE -DSQLITE_CORE -DSQLITE_SOUNDEX

CC = gcc

