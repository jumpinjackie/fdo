##
## Copyright (C) 2004-2006  Autodesk, Inc.
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

EXTRA_DIST = sqlite.vcproj

noinst_LTLIBRARIES = libSqlite3Engine.la

libSqlite3Engine_la_SOURCES = \
  alter.c \
  analyze.c \
  attach.c \
  auth.c \
  backup.c \
  bitvec.c \
  btmutex.c \
  btree.c \
  build.c \
  callback.c \
  complete.c \
  date.c \
  delete.c \
  expr.c \
  fault.c \
  fts3_expr.c \
  func.c \
  global.c \
  hash.c \
  insert.c \
  journal.c \
  legacy.c \
  loadext.c \
  main.c \
  malloc.c \
  mem0.c \
  mem1.c \
  mem2.c \
  mem3.c \
  mem5.c \
  memjournal.c \
  mutex.c \
  mutex_noop.c \
  mutex_unix.c \
  notify.c \
  opcodes.c \
  os.c \
  os_unix.c \
  pager.c \
  parse.c \
  pcache.c \
  pcache1.c \
  pragma.c \
  prepare.c \
  printf.c \
  random.c \
  resolve.c \
  rowset.c \
  rtree.c \
  select.c \
  shell.c \
  status.c \
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
  vdbeblob.c \
  vdbemem.c \
  vtab.c \
  walker.c \
  where.c \
  vdbetrace.c \
  ctime.c \
  fkey.c \
  fts3_snippet.c \
  fts3_write.c \
  wal.c

noinst_HEADERS = $(libSqlite3Engine_la_SOURCES) \
  btree.h \
  btreeInt.h \
  fts3.h \
  fts3_expr.h \
  fts3_hash.h \
  fts3_tokenizer.h \
  hash.h \
  hwtime.h \
  keywordhash.h \
  mutex.h \
  opcodes.h \
  os.h \
  os_common.h \
  pager.h \
  parse.h \
  pcache.h \
  rtree.h \
  sqlite3.h \
  sqlite3ext.h \
  sqliteicu.h \
  sqliteInt.h \
  sqliteLimit.h \
  vdbe.h \
  fts3Int.h \
  wal.h \
  vdbeInt.h

INCLUDES = \
  -I/usr/include


libSqlite3Engine_la_LDFLAGS =

CFLAGS = @CXXFLAGS_SQLITE@ -DSQLITE_ENABLE_COLUMN_METADATA -DSQLITE_OMIT_TRACE -DSQLITE_OMIT_PROGRESS_CALLBACK -DSQLITE_OMIT_AUTHORIZATION -DSQLITE_OMIT_UTF16 -DSQLITE_OMIT_SHARED_CACHEXXX -DSQLITE_THREADSAFE=2 -DSQLITE_ENABLE_RTREE -DSQLITE_CORE -DSQLITE_SOUNDEX
##To be consistent with internal build, force the compiler to be g++. 
CC = gcc

