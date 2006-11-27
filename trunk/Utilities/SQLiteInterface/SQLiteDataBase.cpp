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
#include "SQLiteDataBase.h"
#include "SQLiteTable.h"
#include "SQLiteBTree.h"
#include "SQLiteQueryResult.h"


SQLiteDataBase::SQLiteDataBase()
{ 
    mDbOpen = false;
    mSartedTransaction = false;
    m_pBtree = NULL; 
}
SQLiteDataBase::~SQLiteDataBase() 
{ 
    if( mDbOpen )
        close(0);

    if( m_pBtree )
        delete m_pBtree;

    mActiveTabs.clear();

}

SQLiteBTree* SQLiteDataBase::BTree()
{
    if( m_pBtree == NULL )
        m_pBtree = new SQLiteBTree( mpDB->aDb[0].pBt );
    return m_pBtree; 
}

int SQLiteDataBase::open(unsigned int create, int)
{ 
    mDbOpen = false;
    return SQLITE_OK; 
}

int SQLiteDataBase::openDB( const char *fullPath )
{
    if( mDbOpen )  // Should this be an error?
        return SQLITE_OK;

    if( sqlite3_open(fullPath, &mpDB) == SQLITE_OK )
    {
        ExecuteNonQuery("create table fdo_master(name text, rootpage integer);"); // May fail is the table was already created
        sqlite3BtreeSetPageSize( mpDB->aDb[0].pBt, SQLiteDB_PAGESIZE, -1 );
        sqlite3BtreeSetSafetyLevel( mpDB->aDb[0].pBt, 1);
        sqlite3BtreeSetAutoVacuum( mpDB->aDb[0].pBt, 0 );
		sqlite3_busy_timeout(mpDB, 1000*60); // 60 seconds


        mDbOpen = true;


        return SQLITE_OK;
    }

    return SQLITE_ERROR;
}

int SQLiteDataBase::close(unsigned int)
{
    sqlite3_close(mpDB);
    mDbOpen = false;
    return SQLITE_OK; 
}

void SQLiteDataBase::close_all_read_cursors()
{
    for (std::list<SQLiteTable*>::iterator iter=mActiveTabs.begin(); iter != mActiveTabs.end(); iter++)
        (*iter)->close_cursor();
}

void SQLiteDataBase::add_table( SQLiteTable *tab )
{
    mActiveTabs.push_back(tab);
}

void SQLiteDataBase::remove_table( SQLiteTable *tab )
{
    mActiveTabs.remove(tab);
}

int SQLiteDataBase::begin_transaction( )
{
  
  int rc;
  if( mSartedTransaction ) // We already have one started
      return SQLITE_OK;

  rc = sqlite3BtreeBeginTrans(mpDB->aDb[0].pBt, 1);
  if( rc == SQLITE_OK )
    mSartedTransaction = true;
  
  return rc;
}

int SQLiteDataBase::commit( ) 
{ 
  if( ! mSartedTransaction ) // We don't have a transaction started
      return SQLITE_OK;

  mSartedTransaction = false;

  return sqlite3BtreeCommit(mpDB->aDb[0].pBt);
}

int SQLiteDataBase::rollback( ) 
{ 
  if( ! mSartedTransaction ) // We don't have a transaction started
      return SQLITE_OK;

  mSartedTransaction = false;

  return sqlite3BtreeRollback(mpDB->aDb[0].pBt);
}

const char*  SQLiteDataBase::ErrorMessage()
{
    return sqlite3_errmsg( mpDB );
}

int SQLiteDataBase::ExecuteQuery(const char* sqlStr, SQLiteQueryResult** queryRes )
{
	const char* szTail=0;
	sqlite3_stmt* pStmt;

    *queryRes = NULL;

	int rc = sqlite3_prepare(mpDB, sqlStr, -1, &pStmt, &szTail);

	if (rc == SQLITE_OK)
	{
        *queryRes = new SQLiteQueryResult(mpDB, pStmt );
	}
    return rc;
}

int SQLiteDataBase::ExecuteNonQuery( const char* sqlStr, int *changes )
{
    char* szError=NULL;

    if( changes != NULL )
        *changes = 0;

	int rc = sqlite3_exec(mpDB, sqlStr, 0, 0, &szError);
	if (rc == SQLITE_OK && changes != NULL )
	{
		*changes = sqlite3_changes(mpDB);
	}
    if (szError)
        free(szError);
	return rc;
}


