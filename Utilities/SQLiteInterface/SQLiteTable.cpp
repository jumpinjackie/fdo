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
#include "SQLiteTable.h"
#include "SQLiteSqlUpdateCache.h"
#include "SQLiteBTree.h"
#include "SQLiteData.h"
#include "SQLiteCursor.h"
#include "SQLiteSqlUpdateCache.h"
#include "SQLiteQueryResult.h"

SQLiteTable::SQLiteTable(SQLiteDataBase*  db) 
{ 
    mCacheSize = 0;
    mMaxCacheSize = SQLiteDB_MAXCACHESIZE;
    m_pDb = db;
    mTableName = NULL;
    mIsOpen = false;
    mRootKeyPage = mRootDataPage = -1;
    m_nextKey = 1;
    mTabCache = NULL;
    pmCur = NULL;
	mUseIntKey = false;
    mActualUseIntKey = false;
	mCmpHandler = NULL;
}

SQLiteTable::~SQLiteTable() 
{ 
    if( pmCur )
    {
        pmCur->close();
        delete pmCur;
        pmCur = NULL;
    }
    if( NULL != mTableName )
        delete[] mTableName;
    if( mTabCache )
        delete mTabCache;
}

// Replace any special character with an underscore
void SQLiteTable::make_valid_name( char *name )
{
	for(size_t i=strlen(name);i>0;i--)
	{
		switch(name[i])
		{
			case ':':  
				name[i] = '_';
				break;
			case ' ': 
				name[i] = '$';
				break;
			case '?':	
				name[i] = '1';
				break;
			case '\t': 
				name[i] = '2';
				break;
			case '\n': 
				name[i] = '3';
				break;
			case '\f':
				name[i] = '4';
				break;
			case '\r':
				name[i] = '5';
				break;
			case '\\':
				name[i] = '6';
				break;
			case '-':
				name[i] = '7';
				break;
			case '(': 
				name[i] = '8';
				break;
			case ')': 
				name[i] = '9';
				break;
			case ';':
				name[i] = '0';
				break;
			case '+': 
				name[i] = 'A';
				break;
			case '*': 
				name[i] = 'B';
				break;
			case '/': 
				name[i] = 'C';
				break;
		    case '%': 
				name[i] = 'D';
				break;
			case '=': 
				name[i] = 'E';
				break;
			case '<':
				name[i] = 'F';
				break;
			case '>':
				name[i] = 'G';
				break;
			case '!': 
				name[i] = 'H';
				break;
			case '|':
				name[i] = 'I';
				break;
			case ',': 
				name[i] = 'J';
				break;
			case '&':
				name[i] = 'K';
				break;
			case '@':
				name[i] = 'L';
				break;
			case '~': 
				name[i] = 'M';
				break;
			case '#': 
				name[i] = 'N';
				break;
			case '\'':
				name[i] = 'O';
				break;
			case '`':
				name[i] = 'P';
				break;
			case '"':
				name[i] = 'Q';
				break;
			case '[': 
				name[i] = 'R';
				break;
			case ']': 
				name[i] = 'S';
				break;
		}
	}
}

void SQLiteTable::find_root_page( const char   *tabName )
{
	static char* selectStr = "select rootpage from sqlite_master where type='table' and name='%s'";
	char *szSQL = NULL;
	szSQL = (char*)alloca(strlen(selectStr)+strlen(tabName)+10);
	if( szSQL == NULL )
	{
		mRootDataPage = -1;
		return;
	}

	sprintf(szSQL,selectStr,tabName );
    
    SQLiteQueryResult* queryRes;
    if( m_pDb->ExecuteQuery( szSQL, &queryRes) == SQLITE_OK )
    {
        if( queryRes->NextRow() )
        {
            bool isNull, bfound;
            mRootDataPage = queryRes->IntValue( "rootpage", isNull, bfound );
            if( isNull || ! bfound )
                mRootDataPage = -1;
            else
                mActualUseIntKey = true;
        }
        queryRes->Close();
        delete queryRes;
    }
    if( mRootDataPage == -1 )
    {
        sprintf(szSQL,"select rootpage from fdo_master where name='%s'",tabName );
        if( m_pDb->ExecuteQuery( szSQL, &queryRes) == SQLITE_OK )
        {
            if( queryRes->NextRow() )
            {
                bool isNull, bfound;
                mRootDataPage = queryRes->IntValue( "rootpage", isNull, bfound );
                if( isNull || ! bfound )
                    mRootDataPage = -1;
                else
                    mActualUseIntKey = false;
            }
            queryRes->Close();
            delete queryRes;
        }
    }
}

int SQLiteTable::open( SQLiteTransaction *txnid,
    const char *dbFilePath, const char *mbcsName, const char *utf8Name, unsigned int open_flag, int, bool bNoIntKey)
{ 
    char   *mbcsNewTabName = NULL;
	char   *utf8NewTabName = new char[strlen(utf8Name)+1];
    bool   useNoIntKey = true;
	
	bool useMbcsName = (strcmp(mbcsName,utf8Name) != 0 );
	                         
    SQLiteCursor *mCur = NULL;
    char *szSQL = NULL;

    useNoIntKey = bNoIntKey;
	int sizeForAlloc = strlen( utf8Name );
	if( useMbcsName )
	{
		mbcsNewTabName = new char[strlen(mbcsName)+1];
		strcpy(mbcsNewTabName,mbcsName);
		make_valid_name( mbcsNewTabName );
		int size = strlen( mbcsNewTabName );
		if( size > sizeForAlloc )
			sizeForAlloc = size;
	}

	strcpy(utf8NewTabName,utf8Name);
	make_valid_name( utf8NewTabName );
    
	mIsReadOnly = (open_flag == SQLiteDB_RDONLY);

    if( m_pDb->commit( ) != SQLITE_OK )
        goto the_exit;
    
    m_pDb->remove_table( this );
    
    if( ! m_pDb->IsOpen() )
        m_pDb->openDB(dbFilePath);

	find_root_page( utf8NewTabName );
	if( mRootDataPage == -1 && useMbcsName )
		find_root_page( mbcsNewTabName ); // Older versions used mbcs encoded table names.
 
    if( (open_flag & SQLiteDB_CREATE)  && mRootDataPage == -1 )
    {
		static char* selectStr = "select rootpage from sqlite_master where type='table' and name='%s'";
		static char* insertStr = "insert into fdo_master(name, rootpage) values ('%s',%d)";
		static char* createFrm = "create table '%s'(data blob);";

		szSQL = (char*)alloca(strlen(selectStr)+sizeForAlloc+10);
		if( szSQL == NULL )
			goto the_exit;

        m_pDb->close_all_read_cursors();
        if( useNoIntKey )
        {
            int tabId;
            if( m_pDb->begin_transaction( ) != SQLITE_OK )
                return SQLITE_ERROR;  
            if( m_pDb->BTree()->create_table( 0, &tabId ) == SQLITE_OK )
            {
	            sprintf(szSQL,insertStr,utf8NewTabName,tabId );
                if( m_pDb->ExecuteNonQuery( szSQL ) != SQLITE_OK )
                {
                    m_pDb->commit();
                    return SQLITE_ERROR;
                }

				if( useMbcsName )
				{
					// Insert the mbcs version of the table name. This is needed by previous versions of the provider that
					// did not use the UTF8 table names( i.e Support forward compatibility )
					sprintf(szSQL,insertStr,mbcsNewTabName,tabId );
					m_pDb->ExecuteNonQuery( szSQL ); // Do not fail as this is not required by new providers.
				}

                mRootDataPage = tabId;
            }
            m_pDb->commit();   
        }
        else
        {
            sprintf(szSQL,createFrm,utf8NewTabName);
            m_pDb->ExecuteNonQuery( szSQL );

            SQLiteQueryResult* queryRes;
        	sprintf(szSQL,selectStr,utf8NewTabName );
            if( m_pDb->ExecuteQuery( szSQL, &queryRes) == SQLITE_OK )
            {
                if( queryRes->NextRow() )
                {
                    bool isNull, bfound;
                    mRootDataPage = queryRes->IntValue( "rootpage", isNull, bfound );
                    if( isNull || ! bfound )
                        mRootDataPage = -1;
                }
                queryRes->Close();
                delete queryRes;
            }
			if( mRootDataPage != -1 && useMbcsName )
			{
				// Insert the mbcs version of the table name. This is needed by previous versions of the provider that
				// did not use the UTF8 table names( i.e Support forward compatibility )
				sprintf(szSQL,insertStr,mbcsNewTabName,mRootDataPage );
                (void)m_pDb->ExecuteNonQuery( szSQL ); // Do not fail as this is not required by new providers.
			}
        }

        // Actual integer key status always matches intended status right after table is created.
        mActualUseIntKey = ! useNoIntKey;
    }
    if( mRootDataPage == -1 )
        goto the_exit;
    
    // Find the next available key
    mCur = NULL;
    if( m_pDb->BTree()->cursor( mRootDataPage, &mCur, 0, mCmpHandler )  == SQLITE_OK ) // Open a read-only cursor
    {
        bool  empty;
        if( mCur->last( empty ) == SQLITE_OK && ! empty )
        {
            char* buf = NULL;
            int key;

            if( mCur->get_key( &key, &buf ) == SQLITE_OK  )
            {
				m_nextKey = (*((int *)(buf)))+1;
            }
        }
        mCur->close();
        delete mCur;
    }
    if( strncmp(utf8NewTabName,"RTREE", 5 ) == 0 )
        mMaxCacheSize = SQLiteDB_MAXCACHESIZE*5;
 	
	mUseIntKey = ! useNoIntKey;
	
    mIsOpen = true;
    m_pDb->add_table( this );
    mTableName = new char[strlen(utf8NewTabName)+1];
    strcpy(mTableName,utf8NewTabName);

the_exit:
    if( utf8NewTabName )
        delete[] utf8NewTabName; 
    
	if( mbcsNewTabName )
        delete[] mbcsNewTabName; 

    return ((mRootDataPage == -1)?1:0);
}

int SQLiteTable::Recreate()
{
	char  szSQL[128];
	if( mRootDataPage == -1 )
		return SQLITE_ERROR;

	int oldRootDataPage = mRootDataPage;

	if( m_pDb->begin_transaction( ) != SQLITE_OK )
        return SQLITE_ERROR;  
    
    int tabId;
   
	int flags = (mUseIntKey)?BTREE_INTKEY:0;
	if( m_pDb->BTree()->create_table( flags, &tabId ) == SQLITE_OK )
	{
		m_pDb->BTree()->drop_table( mRootDataPage );
		mRootDataPage = tabId;
		sprintf(szSQL,"update fdo_master set rootpage = %d where rootpage = %d",mRootDataPage, oldRootDataPage );
		if( m_pDb->ExecuteNonQuery( szSQL ) != SQLITE_OK ) 
		{
			m_pDb->commit();
			return SQLITE_ERROR;
		}
		
		if( mTabCache )
			delete mTabCache;
		mTabCache = NULL;
		m_nextKey = 1;
		
		mTabCache = new SQLiteSqlUpdateCache( m_pDb, (unsigned int) -1, mRootDataPage, m_nextKey, false, mUseIntKey, NULL  );
	}
	
    m_pDb->commit();   

	return SQLITE_OK;
}

int SQLiteTable::Drop()
{
	char  szSQL[128];
	if( mRootDataPage == -1 )
		return SQLITE_ERROR;

	if( m_pDb->begin_transaction( ) != SQLITE_OK )
        return SQLITE_ERROR;  
    
    if ( mActualUseIntKey ) {
        sprintf(szSQL,"drop table '%s'", mTableName );
	    if( m_pDb->ExecuteNonQuery( szSQL ) != SQLITE_OK ) 
	    {
		    m_pDb->commit();
		    return SQLITE_ERROR;
	    }		
    }
    else 
    	m_pDb->BTree()->drop_table( mRootDataPage );
	
	// Cleanup any entry for this table from the fdo master table
    sprintf(szSQL,"delete from fdo_master where rootpage = %d",mRootDataPage );
    if( m_pDb->ExecuteNonQuery( szSQL ) != SQLITE_OK ) 
    {
	    m_pDb->commit();
	    return SQLITE_ERROR;
    }
    
	mRootDataPage = -1;

    if( mTabCache )
		delete mTabCache;
	mTabCache = NULL;
	
    m_pDb->commit();   

	return SQLITE_OK;
}

const char* SQLiteTable::GetName()
{
    return mTableName;
}

bool SQLiteTable::GetUseIntKey()
{
    return mUseIntKey;
}

bool SQLiteTable::GetActualUseIntKey()
{
    return mActualUseIntKey;
}

int SQLiteTable::close(unsigned int flags) 
{

    close_cursor();

    if( mCacheSize  && mRootDataPage != -1 )
    {
        mTabCache->flush( );
        mCacheSize = 0;
    }
    
    (void)m_pDb->commit( );
    
    if( pmCur )
    {
        pmCur->close();
        delete pmCur;
        pmCur = NULL;
    }

    mIsOpen = false;      
    mRootDataPage = -1;
    m_pDb->remove_table( this );

    if( mTabCache )
        delete mTabCache;

    mTabCache = NULL;

    return 0; 
}


int SQLiteTable::put(SQLiteTransaction *txid, SQLiteData *key, SQLiteData *data, unsigned int flags) 
{ 
    int rc = 1;

    if( ! mIsOpen )
        return SQLITE_ERROR;

    _ASSERT( mRootDataPage != -1 );

	if( mTabCache == NULL )
		mTabCache = new SQLiteSqlUpdateCache( m_pDb, (unsigned int) -1, mRootDataPage, m_nextKey, false, mUseIntKey, mCmpHandler );

    if( mCacheSize >= mMaxCacheSize && mTabCache )
    {
        mTabCache->flush();
        mCacheSize = 0;
    }
    if( (flags != SQLiteDB_APPEND) && (key->get_size() != 0) )
    {
        // Existing record to be updated. Just override the memory location if the size is the same or less.
        if( mCacheSize && mTabCache->get(txid, key, &mDd, flags ) == 0 )
        {
            if( mTabCache->IsMemCache() && mDd.get_size() == data->get_size() )
            {
                memcpy(mDd.get_data(), data->get_data(), data->get_size() );
                return 0;
            }
            else if ( ! mTabCache->IsMemCache() )
            {
                return mTabCache->put(txid, key, data, flags ); // overwrite the existing record
            }
            else
            {
                mTabCache->del(key);
                return mTabCache->put(txid, key, data, flags );
            }
        }
    }

    if( mTabCache )
    {
        if( mTabCache->put(txid, key, data, flags ) )
            return SQLITE_ERROR;

        mCacheSize++;

        return 0;
    }
    
    // update directly  the database record       
    if( m_pDb->begin_transaction( ) != SQLITE_OK )
        return SQLITE_ERROR;  

	m_pDb->close_all_read_cursors(); // all read cursors need to be closed before a write cursor is aquired.

    if( pmCur )
	{
        delete pmCur;
		pmCur = NULL;
	}

	if( m_pDb->BTree()->cursor( mRootDataPage, &pmCur, 1, mCmpHandler ) )
		return SQLITE_ERROR;

    if (key->get_size() == 0)
    {
        sync_id_pool();
        mMykey = m_nextKey;
        key->set_size( sizeof(SQLiteRecNumbDef) );
        key->set_data(&mMykey );
    }

    rc = pmCur->insert( key->get_size(), (unsigned char*)key->get_data(), data->get_size(), (unsigned char*)data->get_data(), false);


	m_pDb->commit(); 
		
	close_cursor(); // Don't leave a write cursor open

    return rc; 
    
}

int SQLiteTable::put_exclusive(SQLiteTransaction *txid, SQLiteData *key, SQLiteData *data, unsigned int flags) 
{ 
    int rc = 1;

    if( ! mIsOpen )
        return SQLITE_ERROR;

    _ASSERT( mRootDataPage != -1 );
    
    // start a transaction if one is not already started       
    if( m_pDb->begin_transaction( ) != SQLITE_OK )
        return SQLITE_ERROR;  

	if( pmCur == NULL )
		if( m_pDb->BTree()->cursor( mRootDataPage, &pmCur, 1, mCmpHandler ) )
			return SQLITE_ERROR;

    if (key->get_size() == 0)
    {
        // new record
        mMykey = m_nextKey++;
        key->set_size( sizeof(SQLiteRecNumbDef) );
        key->set_data(&mMykey );
    }

    return pmCur->insert( key->get_size(), (unsigned char*)key->get_data(), data->get_size(), (unsigned char*)data->get_data(), false);
}

void SQLiteTable::close_cursor()
{
    if( ! pmCur ) 
        return;

    (void) pmCur->close();
    delete pmCur;
    pmCur = NULL;
}

int SQLiteTable::get(SQLiteTransaction *txnid, SQLiteData *key, SQLiteData *data, unsigned int flags, bool flush ) 
{ 
    int rc = 1;

    if( ! mIsOpen )
        return SQLITE_ERROR;

    if( mRootDataPage == -1 )
        return SQLITE_ERROR;

    if( mCacheSize && mTabCache->get(txnid, key, data, flags ) == 0 )
        return 0;
 
    if( pmCur == NULL )
        if( m_pDb->BTree()->cursor(  mRootDataPage, &pmCur, 0, mCmpHandler ) )
            return SQLITE_ERROR;
    bool found;
    rc = pmCur->move_to( key->get_size(), (unsigned char*)key->get_data(), found );

    if( !found && rc == SQLITE_OK )
        rc = SQLiteDB_NOTFOUND;
    
    char* buf = NULL;
    int size=0;

    if( rc == 0 && pmCur->get_data( &size, &buf, false ) )
        rc = 1;


    if( rc == 0 )
    {
        data->set_size( size );
        data->set_data( (void*)buf );
    }

    return rc;
}

int SQLiteTable::del(SQLiteTransaction *txnid, SQLiteData *key, unsigned int flags) 
{ 
    SQLiteData data;
    int rc = SQLITE_ERROR;
    SQLiteCursor *pCur = NULL;
    bool found;

       if( ! mIsOpen )
        return SQLITE_ERROR;

    if( mRootDataPage == -1 )
        return SQLITE_ERROR;

    m_pDb->close_all_read_cursors(); // all read cursors need to be closed before a write cursor is aquired.

    if( mTabCache )
    {
        mTabCache->flush();
        mCacheSize = 0;
    }
    if( m_pDb->BTree()->cursor(  mRootDataPage, &pCur, 1, mCmpHandler ) )
        return SQLITE_ERROR;

    rc = pCur->move_to( key->get_size(), (unsigned char*)key->get_data(), found );
    if( found )
    { 
        if( ! m_pDb->transaction_started() )
        {
            if( m_pDb->begin_transaction( ) == SQLITE_OK ) 
            {
                rc = pCur->delete_current();
                m_pDb->commit();
            }
            else 
                rc = SQLITE_ERROR;
        }
        else
            rc = pCur->delete_current();
    }
    pCur->close();
    delete pCur;

    return rc;
}

int SQLiteTable::cursor(SQLiteTransaction *txnid, SQLiteCursor **cursorp, bool write )  
{ 
	if( pmCur == NULL )
        if( m_pDb->BTree()->cursor(  mRootDataPage, &pmCur, 0, mCmpHandler ) )
			return SQLITE_ERROR;
    *cursorp = pmCur;
    return SQLITE_OK; 
}

bool SQLiteTable::cursor_is_valid()
{
	return (pmCur != NULL);
}

bool SQLiteTable::needs_a_flush( bool partial )
{
	if( partial )
		return (mCacheSize > 0 );

	return ( (mCacheSize+1) >= mMaxCacheSize && mTabCache );
}

void SQLiteTable::flush()
{
	if( mTabCache )
    {
        mTabCache->flush();
        sync_id_pool();
        mCacheSize = 0;
    }
}

void SQLiteTable::sync_id_pool()
{
	SQLiteCursor *pCur = NULL;

	if( m_pDb == NULL )
		return;

	if( m_pDb->BTree()->cursor( mRootDataPage, &pCur, 0, mCmpHandler )  == SQLITE_OK ) // Open a read-only cursor
    {
        bool  empty;
        if( pCur->last( empty ) == SQLITE_OK && ! empty )
        {
            char* buf = NULL;
            int key;

            if( pCur->get_key( &key, &buf ) == SQLITE_OK  )
            {
				m_nextKey = (*((int *)(buf)))+1;
				if( mTabCache != NULL )
					mTabCache->set_next_id( m_nextKey );
            }
        }
        pCur->close();
        delete pCur;
    }
}

