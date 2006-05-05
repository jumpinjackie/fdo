#ifndef _SQLITESQLUPDATECACHE_H
#define _SQLITESQLUPDATECACHE_H
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

#include "SQLiteUpdateCache.h"
#include "SQLiteFileDataBase.h"

class SQLiteTransaction;
class SQLiteData;
class SQLiteTable;
class SQLiteCursor;

class SQLiteSqlUpdateCache :
    public SQLiteUpdateCache
{
private:
    SQLiteDataBase     *m_pTargetDb; // persistent db; usually a file used to flush the content of the cache
    SQLiteDataBase     *m_pDb;       // The in-memory db used as a cache
    SQLiteRecNumbDef          m_nextKey;   // This is used for autogen int key only
    Btree               *mBtree;
    int                 mTabId;
    SQLiteCursor        *mCur;
    int                 mRootPage;
    bool                mUseCompression;
    bool                mUseIntKey; // Set to true for table with integer keys(rowid); this optimized by SQLITE
    SQLiteRecNumbDef          mMykey;
public:
    SQLiteSqlUpdateCache(SQLiteDataBase *db, unsigned int, int rootPage, unsigned int nextKey, bool useCompression, bool useIntKey );
    virtual ~SQLiteSqlUpdateCache(void);

    /*
    ** This is usufull to indicate to the caller that the data returned is actually the memory location of the original data.
    ** The caller can make in-place modification.
    */
    bool IsMemCache() { return false; }

    /*
    ** Save the content to the target db and empty the cache.
    */
    int flush( );
    
    /*
    ** Returns true if the cache can add the row identified by the key.
    */
    bool CanPut(SQLiteData *key);
    
    /*
    ** Add a new row
    */
    int put(SQLiteTransaction *, SQLiteData *, SQLiteData *, unsigned int);
    
    /*
    ** get the row identified by the key.
    */
    int get(SQLiteTransaction *txnid, SQLiteData *key, SQLiteData *data, unsigned int flags);

    /*
    ** Delete the row identified by key.
    */
    int del( SQLiteData *key );

	void set_next_id( SQLiteRecNumbDef nextId ) { m_nextKey = nextId; }
};
#endif

