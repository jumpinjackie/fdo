#ifndef _SQLITETABLE_H
#define _SQLITETABLE_H
// 
//  Copyright (C) 2004-2005  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#ifdef _WIN32
#pragma once
#endif

#include "SQLiteDataBase.h"
#include "SQLiteData.h"

class SQLiteUpdateCache;
class SQLiteCursor;
class SQLiteTransaction;

/*
** This class reprents a SQLite table.
*/
class SQLiteTable
{
    friend class SQLiteDataBase;

private:
    SQLiteUpdateCache   *mTabCache;
    SQLiteDataBase      *m_pDb;
    char                *mTableName;
    bool                mIsOpen;
    int                 mRootDataPage;
    int                 mRootKeyPage;
    SQLiteCursor        *pmCur;
    SQLiteRecNumbDef          m_nextKey;
    unsigned int           mCacheSize;
    unsigned int           mMaxCacheSize;
    bool                mIsReadOnly;
	bool                mUseIntKey;
    bool                mUseCompression;
    SQLiteData          mDd;
    SQLiteRecNumbDef          mMykey;

public:
    SQLiteTable( SQLiteDataBase* db );
    ~SQLiteTable(void);

    int open(SQLiteTransaction *txnid,
        const char *, const char *subname, unsigned int, int, bool bUseIntKey = true);

    int close(unsigned int flags);

    int put(SQLiteTransaction *, SQLiteData *, SQLiteData *, unsigned int);

    int get(SQLiteTransaction *txnid, SQLiteData *key, SQLiteData *data, unsigned int flags, bool flush=true);

    int del(SQLiteTransaction *txnid, SQLiteData *key, unsigned int flags);

    int cursor(SQLiteTransaction *txnid, SQLiteCursor **cursorp, bool write );

    void close_cursor();

	bool cursor_is_valid();

	bool needs_a_flush();

	void flush();

	void sync_id_pool();

	int Recreate();

private:

	void make_valid_name( char *name );
};

#endif


