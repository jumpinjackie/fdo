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
#include "SQLiteBTree.h"
#include "SQLiteCursor.h"

static int local_compare(void *handler,int size1,const void* data1,int size2,const void* data2)
{
	if( handler == NULL )
		return 0;

	return ((SQLiteBTreeCompareHandler*)handler)->compare(size1, data1, size2, data2 );
}

SQLiteBTree::SQLiteBTree(void): m_pBt( NULL )
{
    m_bOwner = true;
}
#ifdef SQLITE_3_1
SQLiteBTree::SQLiteBTree( Btree *bt ) : m_pBt( bt )
{ 
    m_bOwner = false;
}
#else
SQLiteBTree::SQLiteBTree( Btree *bt, sqlite3 *db ) : m_pBt( bt ), m_pDB( db )
{ 
    m_bOwner = false;
}
#endif

SQLiteBTree::~SQLiteBTree(void)
{
    if( m_pBt && m_bOwner )
        sqlite3BtreeClose(m_pBt);
}

/*
** Open a new database
*/
int SQLiteBTree::open( const char* fileName, int nCache, int   flags )
{
  int rc;
#ifdef SQLITE_3_1
  rc = sqlite3BtreeOpen(fileName, &m_pBt, flags);
#else
  rc = sqlite3BtreeOpen(fileName, m_pDB, &m_pBt, flags);
#endif
  if( rc!=SQLITE_OK ){
    return rc;
  }
  sqlite3BtreeSetCacheSize(m_pBt, nCache);
  
  m_bOwner = true;

  return SQLITE_OK;
}

/*
** Close the given database.
*/
int SQLiteBTree::close( )
{
  if( ! m_bOwner )
    return SQLITE_OK;

  int rc;
  rc = sqlite3BtreeClose(m_pBt);
  m_pBt = NULL;
  return rc;
}

/*
** Create a new table in the database
*/
int SQLiteBTree::create_table(  int flags, int *tableId )
{
  int rc, iTable;
  rc = sqlite3BtreeCreateTable(m_pBt, &iTable, flags);
  if( rc!=SQLITE_OK ){
    return rc;
  }
  
  *tableId = iTable;

  return SQLITE_OK;
}

/*
** Create a new table in the database
*/
int SQLiteBTree::drop_table(  int tableId )
{
  int  iMoved;
  return sqlite3BtreeDropTable(m_pBt, tableId, &iMoved);
}

/*
** Remove all entries from the given table but keep the table around.
*/
int SQLiteBTree::clear_table( int tableId )
{
  return sqlite3BtreeClearTable(m_pBt, tableId);
}

/*
** Create a new cursor.  Return the ID for the cursor.
*/
int SQLiteBTree::cursor( int iTable,  SQLiteCursor **pCur_out, int wrFlag )
{
  BtCursor *pCur;
  int rc;
  rc = sqlite3BtreeCursor(m_pBt, iTable, wrFlag, 0, 0, &pCur);
  if( rc != SQLITE_OK ){
    return rc;
  }
  *pCur_out = new SQLiteCursor(pCur);

  return SQLITE_OK;
}

int SQLiteBTree::cursor( int iTable,  SQLiteCursor **pCur_out, int wrFlag, SQLiteBTreeCompareHandler *handler )
{
  BtCursor *pCur;
  int rc;
  if( handler == NULL )
	  return cursor(iTable, pCur_out, wrFlag );

  rc = sqlite3BtreeCursor(m_pBt, iTable, wrFlag, local_compare, (void*)handler, &pCur);
  if( rc != SQLITE_OK ){
    return rc;
  }
  *pCur_out = new SQLiteCursor(pCur);

  return SQLITE_OK;
}
