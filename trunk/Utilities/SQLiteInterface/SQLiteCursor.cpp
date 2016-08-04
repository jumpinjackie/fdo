// 
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
//  
#include "stdafx.h"
#include "SQLiteCursor.h"
#include "SQLiteDataBase.h"
#include "SQLiteData.h"

SQLiteCursor::SQLiteCursor(void): 
m_pCur(NULL),
m_pKeyBuf(NULL),
m_keyBufSize(0),
m_pDatBuf(NULL),
m_datBufSize(0)
{
}

SQLiteCursor::SQLiteCursor(BtCursor *cur):
m_pCur(cur),
m_pKeyBuf(NULL),
m_keyBufSize(0),
m_pDatBuf(NULL),
m_datBufSize(0)
{
}

SQLiteCursor::~SQLiteCursor(void)
{
    if( m_pKeyBuf != NULL )
        free(m_pKeyBuf);
    if( m_pDatBuf != NULL )
        free(m_pDatBuf);
}

/*
** Move the cursor to the entry with the given key.
*/
int SQLiteCursor::move_to(
  int keylen,
  unsigned char *pKBuf,
  bool & found )
{
  int rc;
  int res;

  found = false;

  if( sqlite3BtreeFlags(m_pCur) & BTREE_INTKEY ){
    rc = sqlite3BtreeMoveto(m_pCur, 0, *((int*)pKBuf), &res);
  }else{
    rc = sqlite3BtreeMoveto(m_pCur, pKBuf, keylen, &res);  
  }
  
  if( res == 0 )
      found = true;
  else
    {
      // advanced to the next entry. If res is 0, then the cursor is not pointing to the last record.
	  sqlite3BtreeNext(m_pCur, &res);
	  if (res == 0)
		rc = SQLiteDB_KEYEMPTY;
	}


  return rc;
}

/*
** Move the cursor to the next entry in the table.  Return 0 on success
** or 1 if the cursor was already on the last entry in the table or if
** the table is empty.
*/
int SQLiteCursor::next( )
{
  int rc;
  int res = 0;

  rc = sqlite3BtreeNext(m_pCur, &res);
  if( rc == SQLITE_OK )
    return res;

  return SQLITE_ERROR;
}

/*
**
** Move the cursor to the previous entry in the table.  Return 0 on
** success and 1 if the cursor was already on the first entry in
** the table or if the table was empty.
*/
int SQLiteCursor::prev( )
{
  int rc;
  int res = 0;
  rc = sqlite3BtreePrevious(m_pCur, &res);
  if( rc == SQLITE_OK )
    return res;

  return SQLITE_ERROR;
}

/*
**
** Move the cursor to the first entry in the table.  Return 0 if the
** cursor was left point to something and 1 if the table is empty.
*/
int SQLiteCursor::first( )
{
  int rc;
  int res = 0;

  rc = sqlite3BtreeFirst(m_pCur, &res);
  if( rc == SQLITE_OK )
    return res;

  return SQLITE_ERROR;
}

/*   
**
** Move the cursor to the last entry in the table.  Return 0 if the
** cursor was left point to something and 1 if the table is empty.
*/
int SQLiteCursor::last( bool & empty )
{
  int rc;
  int res = 0;

  empty = true;

  rc = sqlite3BtreeLast(m_pCur, &res);
  if( rc != SQLITE_OK )
    return SQLITE_ERROR;

  empty = ( res == 1 );
   
  return SQLITE_OK;
}

int SQLiteCursor::delete_current()
{
   return sqlite3BtreeDelete( m_pCur );
}

int SQLiteCursor::get(SQLiteData* key, SQLiteData *data, unsigned int flags)
{
    int   len;
    char *buf;

    if( get_key( &len, &buf ) == SQLITE_OK )
    {
        key->set_size( len );
        key->set_data( buf );
    }
    _ASSERT(false);
    // TODO the use_compression is set to false, this may not be correct.
    // This method is implemented because it's part of BDB inetrface required by the SDF provider.
    // This should be changed in the SDF code.
    if( get_data( &len, &buf, false ) == SQLITE_OK )  
    {
        data->set_size( len );
        data->set_data( buf );

        return SQLITE_OK;
    }

    return SQLITE_ERROR;
}

#pragma warning(push)
#pragma warning(disable: 4018)  // '<' : signed/unsigned mismatch
#pragma warning(disable: 4244)  // conversion from 'i64' to 'size_t', possible loss of data

/*
**
** Return the key for the entry at which the cursor is pointing.
*/
int SQLiteCursor::get_key(
  int      *key,
  char     **data )
{
  int rc;
  i64 n;

  rc = sqlite3BtreeKeySize(m_pCur, &n);
  if( rc )
      return 1;

  if( sqlite3BtreeFlags(m_pCur) & BTREE_INTKEY )
  {
    if( 4 > m_keyBufSize )
    {
        if( m_pKeyBuf != NULL )
            free(m_pKeyBuf);
        m_pKeyBuf = (char *)malloc( 5 );
        m_keyBufSize = 4;
    }
    *key = 4;
    memcpy(m_pKeyBuf,&n,4);
    *data = m_pKeyBuf; 
  }
  else
  {
    if( n > m_keyBufSize )
    {
        if( m_pKeyBuf != NULL )
            free(m_pKeyBuf);
        m_pKeyBuf = (char *)malloc( n+1 );
        m_keyBufSize = n;
    }
    rc = sqlite3BtreeKey(m_pCur, 0, n, m_pKeyBuf);
    if( rc != SQLITE_OK )
      return rc;
    
    m_pKeyBuf[n] = 0;
    *key = n; // It's the size
    *data = m_pKeyBuf; 
  }

  return SQLITE_OK;
}


/*
** Return the data for the entry at which the cursor is pointing.
*/
int SQLiteCursor::get_data(
  int      *size,
  char     **data,
  bool useCompression )
{
  int rc;
  u32 n;

  rc = sqlite3BtreeDataSize(m_pCur, &n);
  if( rc != 0 )
      return 1;

  if( m_pDatBuf == NULL )
  {
      int size = n+1;
      m_pDatBuf = (char *)malloc(size);
      m_datBufSize = size;
  }
  else if( n>=m_datBufSize )
  {
      free(m_pDatBuf);
      m_datBufSize = n+1024;  // add one k to avoid reallocating so often
      m_pDatBuf = (char *)malloc(m_datBufSize);
  }
  rc = sqlite3BtreeData(m_pCur, 0, n, m_pDatBuf);
  if( rc != SQLITE_OK )
  {
    *data = NULL;
    *size = 0;
    return 1;
  }
  *data = m_pDatBuf;
  *size = n;
  return SQLITE_OK;
}

#pragma warning(pop) // '<' : signed/unsigned mismatch

int SQLiteCursor::get_fast_data(
  int      *size,
  char     **data,
  bool useCompression )
{
    u32 n;

    sqlite3BtreeDataSize(m_pCur, &n);
    if( n >= SQLiteDB_PAGESIZE )
        return get_data( size, data, useCompression );
    *data = (char *)sqlite3BtreeDataFetch(m_pCur, size);
    
    return SQLITE_OK;
}


/*
** Close a cursor opened using btree_cursor.
*/
int SQLiteCursor::close( )
{
  return sqlite3BtreeCloseCursor(m_pCur);
}

/*
** Create a new entry with the given key and data.  If an entry already
** exists with the same key the old entry is overwritten.
*/
int SQLiteCursor::insert(
  int keylen,
  unsigned char *pKBuf,
  int dlen,
  unsigned char *pDBuf,
  bool useCompression
){
  int rc;
  if( sqlite3BtreeFlags(m_pCur) & BTREE_INTKEY )
    rc = sqlite3BtreeInsert(m_pCur, 0, *((int*)pKBuf), pDBuf, dlen);
  else
    rc = sqlite3BtreeInsert(m_pCur, pKBuf, keylen, pDBuf, dlen);

  return rc;
}

