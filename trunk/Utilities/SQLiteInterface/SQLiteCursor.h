#ifndef _SQLITECURSOR_H
#define _SQLITECURSOR_H
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

struct BtCursor;
class SQLiteData;

class SQLiteCursor
{
private:
    BtCursor *m_pCur;
    char     *m_pKeyBuf;
    int       m_keyBufSize;
    char     *m_pDatBuf;
    int       m_datBufSize;

public:
    SQLiteCursor(void);
    SQLiteCursor(BtCursor *cur);
    ~SQLiteCursor(void);

    /*
    ** Close a cursor opened using btree_cursor.
    */
    int close();

    /*
    ** Move the cursor to the entry with the given key.
    */
    int move_to( int keylen, unsigned char *pKBuf, bool & found );

    /*
    **
    ** Move the cursor to the first entry in the table.  Return 0 if the
    ** cursor was left point to something and 1 if the table is empty.
    */
    int first( );

    /*
    ** Move the cursor to the next entry in the table.  Return 0 on success
    ** or 1 if the cursor was already on the last entry in the table or if
    ** the table is empty.
    */
    int next( );

    /*
    **
    ** Move the cursor to the previous entry in the table.  Return 0 on
    ** success and 1 if the cursor was already on the first entry in
    ** the table or if the table was empty.
    */
    int prev( );

    /*   
    ** Move the cursor to the last entry in the table.  Return 0 if the
    ** cursor was left point to something and 1 if the table is empty.
    */
    int last( bool & empty );

    /*
    ** Delete the current row
    */
    int delete_current();

    /*
    ** Return the key and data for the entry at which the cursor is pointing.
    */
    int get(SQLiteData* key, SQLiteData *data, unsigned int flags);

    /*
    ** Return the key for the entry at which the cursor is pointing.
    */
    int get_key( int      *keylen, char     **keydata );

    /*
    ** Return the data for the entry at which the cursor is pointing.
    ** Allocate an internal buffer for the data.
    */
    int get_data( int      *size, char     **data, bool useCompression );

    /*
    ** Return the data for the entry at which the cursor is pointing.
    ** The data points to the db memory. TODO, not used because could not get it to work.
    */
    int get_fast_data( int      *size, char     **data, bool useCompression );

    /*
    ** Create a new entry with the given key and data.  If an entry already
    ** exists with the same key the old entry is overwritten.
    */
    int insert( int keylen, unsigned char *pKBuf, int dlen, unsigned char *pDBuf, bool useCompression );
};

#endif

