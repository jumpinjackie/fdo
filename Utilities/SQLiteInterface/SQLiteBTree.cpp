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

SQLiteBTree::SQLiteBTree(void): m_pBt( NULL )
{
    m_bOwner = true;
}

SQLiteBTree::SQLiteBTree( Btree *bt ) : m_pBt( bt )
{ 
    m_bOwner = false;
}

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
  
  rc = sqlite3BtreeOpen(fileName, &m_pBt, flags);
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

