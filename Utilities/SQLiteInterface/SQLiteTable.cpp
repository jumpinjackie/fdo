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
    mUseCompression = false;
	mUseIntKey = false;
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
	for(int i=strlen(name);i>0;i--)
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

int SQLiteTable::open( SQLiteTransaction *txnid,
    const char *dbFilePath, const char *subname, unsigned int open_flag, int, bool bUseIntKey)
{ 
    char   *newTabName = new char[strlen(subname)+1];
    bool   useIntKey = true;
	static char* formatStr = "select rootpage from sqlite_master where type='table' and name='%s'";
	                         
    SQLiteCursor *mCur = NULL;
    char *szSQL = NULL;

    useIntKey = bUseIntKey;

    strcpy(newTabName,subname);

    mIsReadOnly = (open_flag == SQLiteDB_RDONLY);

    if( m_pDb->commit( ) != SQLITE_OK )
        goto the_exit;
    
    m_pDb->remove_table( this );

	make_valid_name( newTabName );
    
    if( ! m_pDb->IsOpen() )
        m_pDb->openDB(dbFilePath);

    szSQL = (char*)alloca(strlen(formatStr)+strlen(newTabName)+10);
	if( szSQL == NULL )
		goto the_exit;

	sprintf(szSQL,formatStr,newTabName );
    
    SQLiteQueryResult* queryRes;
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
    if( mRootDataPage == -1 )
    {
        sprintf(szSQL,"select rootpage from fdo_master where name='%s'",newTabName );
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
    }
    if( (open_flag & SQLiteDB_CREATE)  && mRootDataPage == -1 )
    {
        m_pDb->close_all_read_cursors();
        if( strncmp(newTabName,"KEY_", 4 ) == 0 )
        {
            int tabId;
            if( m_pDb->begin_transaction( ) != SQLITE_OK )
                return SQLITE_ERROR;  
            int flags = (useIntKey)?BTREE_INTKEY:0;
            if( m_pDb->BTree()->create_table( flags, &tabId ) == SQLITE_OK )
            {
	            sprintf(szSQL,"insert into fdo_master(name, rootpage) values ('%s',%d)",newTabName,tabId );
                if( m_pDb->ExecuteNonQuery( szSQL ) != SQLITE_OK )
                {
                    //printf("Error: %s \n", m_pDb->ErrorMessage()  );
                    return SQLITE_ERROR;
                }
                mRootDataPage = tabId;
            }
            m_pDb->commit();   
            
        }
        else
        {
			static char* createFrm = "create table %s(data blob);";
			char   *stmt = (char*) alloca(strlen(createFrm)+strlen(newTabName)+1);
			if( stmt == NULL )
				goto the_exit;
            sprintf(stmt,createFrm,newTabName);
       
            m_pDb->ExecuteNonQuery( stmt );
            SQLiteQueryResult* queryRes;
        	sprintf(szSQL,"select rootpage from sqlite_master where type='table' and name='%s'",newTabName );

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
        }
    }
    if( mRootDataPage == -1 )
        goto the_exit;
    
    // Find the next available key
    mCur = NULL;
    if( m_pDb->BTree()->cursor( mRootDataPage, &mCur, 0 )  == SQLITE_OK ) // Open a read-only cursor
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
    if( strncmp(newTabName,"DATA_", 5 ) == 0 ) // Need a better test or get the hint from the provider code
    {
        mUseCompression = false;
    }
    else if( strncmp(newTabName,"RTREE", 5 ) == 0 )
    {
        mMaxCacheSize = 2500;
        mUseCompression = false;
    }
    else 
    {
        mUseCompression = false;
    }
 
    mTabCache = new SQLiteSqlUpdateCache( m_pDb, (unsigned int) -1, mRootDataPage, m_nextKey, mUseCompression, useIntKey );

	mUseIntKey = useIntKey;
    mIsOpen = true;
    m_pDb->add_table( this );
    mTableName = new char[strlen(newTabName)+1];
    strcpy(mTableName,newTabName);

the_exit:
    if( newTabName )
        delete[] newTabName; 
    
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
		m_nextKey = 1;
		mTabCache = new SQLiteSqlUpdateCache( m_pDb, (unsigned int) -1, mRootDataPage, m_nextKey, mUseCompression, mUseIntKey );
		
	}
	
    m_pDb->commit();   
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
        delete pmCur;

    if( m_pDb->BTree()->cursor( mRootDataPage, &pmCur, 1 ) )
        return SQLITE_ERROR;

    if (key->get_size() == 0)
    {
        // new record
        mMykey = m_nextKey++;
        key->set_size( sizeof(SQLiteRecNumbDef) );
        key->set_data(&mMykey );
    }

    rc = pmCur->insert( key->get_size(), (unsigned char*)key->get_data(), data->get_size(), (unsigned char*)data->get_data(), this->mUseCompression);

	m_pDb->commit(); 

    close_cursor(); // Don't leave a write cursor open

    return rc; 
    
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
        if( m_pDb->BTree()->cursor(  mRootDataPage, &pmCur, 0 ) )
            return SQLITE_ERROR;
    bool found;
    rc = pmCur->move_to( key->get_size(), (unsigned char*)key->get_data(), found );

    if( !found && rc == SQLITE_OK )
        rc = SQLiteDB_NOTFOUND;
    
    char* buf = NULL;
    int size=0;

    if( rc == 0 && pmCur->get_data( &size, &buf, mUseCompression ) )
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

    if( mCacheSize && mTabCache->get(txnid, key, &data, flags ) == 0 )
    {
        mTabCache->del( key );
    }

    if( m_pDb->BTree()->cursor(  mRootDataPage, &pCur, 1 ) )
        return SQLITE_ERROR;

    rc = pCur->move_to( key->get_size(), (unsigned char*)key->get_data(), found );
    if( found )
    { 
        if( m_pDb->begin_transaction( ) == SQLITE_OK ) 
        {
            rc = pCur->delete_current();
            m_pDb->commit();
        }
        else 
            rc = SQLITE_ERROR;
    }
    pCur->close();
    delete pCur;

    return rc;
}

int SQLiteTable::cursor(SQLiteTransaction *txnid, SQLiteCursor **cursorp, bool write )  
{ 
	if( pmCur == NULL )
        if( m_pDb->BTree()->cursor(  mRootDataPage, &pmCur, 0 ) )
			return SQLITE_ERROR;
    *cursorp = pmCur;
    return SQLITE_OK; 
}

bool SQLiteTable::cursor_is_valid()
{
	return (pmCur != NULL);
}

bool SQLiteTable::needs_a_flush()
{
	return ( (mCacheSize+1) >= mMaxCacheSize && mTabCache );
}

void SQLiteTable::flush()
{
	if( mTabCache )
    {
        mTabCache->flush();
        mCacheSize = 0;
    }
}

void SQLiteTable::sync_id_pool()
{
	SQLiteCursor *pCur = NULL;

	if( m_pDb == NULL )
		return;

	if( m_pDb->BTree()->cursor( mRootDataPage, &pCur, 0 )  == SQLITE_OK ) // Open a read-only cursor
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

