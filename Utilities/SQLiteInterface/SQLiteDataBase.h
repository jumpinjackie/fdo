#ifndef _SQLITEDATABASE_H
#define _SQLITEDATABASE_H
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

#include <list>


#define	SQLiteDB_CREATE	            0x0000001	/* Create file new file */
#define	SQLiteDB_RDONLY	            0x0000010	/* Read-only. */

#define	SQLiteDB_APPEND				1	
#define	SQLiteDB_NEXT			    2	

#define	SQLiteDB_OK					0	
#define	SQLiteDB_ERROR			    1	
#define	SQLiteDB_NOTFOUND			(-2)	/* Key/data pair not found */
#define	SQLiteDB_KEYEMPTY			(-3)	/* Key/data deleted or not created yet. */

#define SQLiteDB_PAGESIZE			40960   /* Page size maximum (65K)  */
#define SQLiteDB_MAXCACHESIZE		500 // maximium number of feature in the write cache

typedef	unsigned int	SQLiteRecNumbDef;	/* Record number type. */

class SQLiteTable;
class SQLiteBTree;
class SQLiteQueryResult;
struct sqlite3;

class SQLiteDataBase
{

private:
    char            *m_FullPath;
    bool            mDbOpen;
    std::list<SQLiteTable*> mActiveTabs;
    SQLiteBTree*    m_pBtree;
    bool            mSartedTransaction;
    sqlite3         *mpDB;

public:

    int open(const char *fullPath, unsigned int create, int);

    int openDB( const char *fullPath ); // It's only needed because the SDF code does not use open to open/create the sdf file

    SQLiteDataBase();
    ~SQLiteDataBase(void);
    
    /*
    ** Returns the last error. If an SQLIte function fails, this method can be called to return an error message.
    ** This error message can be used for debugging but not for end user display as it is not localized.
    */
    const char* ErrorMessage();

    /*
    ** Executes a SQL92 query. The result is returned by the queryRes output paramater. This is used to select from 
    ** an SQLite table.
    */
    int ExecuteQuery( const char* sqlStr, SQLiteQueryResult** queryRes );

    /*
    ** Executes an SQL92 statement that does not return data such as insert,update,delete
    */
    int ExecuteNonQuery( const char* sqlStr, int *changes = NULL );

    /*
    ** return true if the db is open, false otherwise.
    */
    bool IsOpen() { return mDbOpen; }

    /*
    ** close the db.
    */
    int close(unsigned int);
    
    /*
    ** Start a transaction if one is not already started. This is required before modifying the database.
    */
    int begin_transaction();

    /*
    ** Commit the transaction if one already active.
    */
    int commit( );

    /*
    ** Returns the BTree object of the open DB
    */
    SQLiteBTree* BTree();

    /*
    ** Closes all read cursors. Handy when attempting to modify the DB
    */
    void close_all_read_cursors();
    
    /*
    ** Register active tables. This list is only used to close the read cursors
    */
    void add_table( SQLiteTable *tab );
    
    /*
    ** Un-register an active table.
    */
    void remove_table( SQLiteTable *tab );

	bool transaction_started() { return mSartedTransaction; }
};

#endif

