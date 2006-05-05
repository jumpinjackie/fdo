// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
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
#include "DataDb.h"
#include "DataIO.h"


DataDb::DataDb(SQLiteDataBase* env, const char* filename, const char* dbname, bool bReadOnly)
: m_wrtData(256),m_lastRec( 0 )
{
    m_db = new SQLiteTable(env);

    int res;

    int readOnlyFlag = bReadOnly ? SQLiteDB_RDONLY : 0;

    //try to open a database that already exists
    if (res = m_db->open(0, filename, dbname, readOnlyFlag, 0) != 0)
    {
        //must close even if open failed
        m_db->close(0);
        delete m_db;

        m_db = new SQLiteTable(env);

        if (bReadOnly)
            throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_4_CONNECTION_IS_READONLY)));

        //if that fails, create one
        if (res = m_db->open(0, filename, dbname, SQLiteDB_CREATE, 0) != 0)
        {
            //printf("%s\n", env->strerror(res));
            throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_10_ERROR_ACCESSING_SDFDB)));
        }
    }
}

DataDb::~DataDb()
{
    m_db->close(0);
    delete m_db;
}



REC_NO DataDb::InsertFeature(FdoClassDefinition* clas, PropertyIndex* pi, FdoPropertyValueCollection* pvc)
{
    m_wrtData.Reset();

    DataIO::MakeDataRecord(clas, pi, pvc, m_wrtData);

    //insert feature into database
    SQLiteData keyfeat;
    SQLiteData datafeat(m_wrtData.GetData(), m_wrtData.GetDataLen());

    if (m_db->put(0, &keyfeat, &datafeat, SQLiteDB_APPEND) != 0)
        return (REC_NO)0;

    //endian
    return *(REC_NO*)keyfeat.get_data();
}

//overwrites a feature record with the given data
void DataDb::UpdateFeature(REC_NO recno, SQLiteData* data)
{
    SQLiteData key(&recno, sizeof(REC_NO));

    //overwrite feature record
    if (m_db->put(0, &key, data, 0) != 0)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_11_ERROR_INSERTING_FEATURE)));
}


//removes the feature with the given record number
void DataDb::DeleteFeature(REC_NO recno)
{    
    SQLiteData key(&recno, sizeof(REC_NO));
    if (m_db->del(0, &key, 0) != 0)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_12_ERROR_DELETING_FEATURE)));
}


int DataDb::Cursor(SQLiteCursor** cursor, bool write)
{
	return m_db->cursor(0, cursor, write);
}

void DataDb::CloseCursor()
{
	m_db->close_cursor();
}

bool DataDb::CurrentDataIsValid()
{
	return m_db->cursor_is_valid();
}


// The DataDb can be shared by many reader. The tag is used to identify the
// last reader that used the DataDb. 
void DataDb::SetTag( void* tag )
{
	m_tag = tag;
}

void* DataDb::GetTag()
{
	return m_tag;
}

int DataDb::GetFirstFeature( SQLiteData* key, SQLiteData* data )
{
	SQLiteCursor *cur = NULL;
	if( Cursor( &cur, FALSE ) != SQLiteDB_OK || cur == NULL )
		return SQLiteDB_ERROR;
	
	if( cur->first() == SQLiteDB_OK )
	{
		int size;
		char *dat;
		if( cur->get_data(&size, &dat, false ) ==  SQLiteDB_OK )
		{
			data->set_size(size);
			data->set_data(dat);
			if( cur->get_key(&size, &dat ) !=  SQLiteDB_OK )
				return SQLiteDB_ERROR;

			key->set_size(size);
			key->set_data(dat);
			m_lastRec = *(REC_NO*)(dat);
			return SQLiteDB_OK;
		}
	}

	return SQLiteDB_NOTFOUND;
}

int DataDb::GetNextFeature( SQLiteData* key, SQLiteData* data )
{
	SQLiteCursor *cur = NULL;
	if( Cursor( &cur, FALSE ) != SQLiteDB_OK || cur == NULL )
		return SQLiteDB_ERROR;
	
	if( m_lastRec != *(REC_NO*)(key->get_data() ) )
	{
		bool found;
		// Another reader moved the cursor since the previous call or since the call to first.
		// Need to set the cursor to the previous position
		if( cur->move_to( key->get_size(), (unsigned char*)key->get_data(), found ) != SQLiteDB_OK || ! found )
			return SQLiteDB_ERROR;
	}

	if( cur->next() == SQLiteDB_OK )
	{
		int size;
		char *dat;
		if( cur->get_data(&size, &dat, false ) ==  SQLiteDB_OK )
		{
			data->set_size(size);
			data->set_data(dat);
			if( cur->get_key(&size, &dat ) !=  SQLiteDB_OK )
				return SQLiteDB_ERROR;

			key->set_size(size);
			key->set_data(dat);
			m_lastRec = *(REC_NO*)(dat);

			return SQLiteDB_OK;
		}
	}

	return SQLiteDB_NOTFOUND;
}
