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
#include "KeyDb.h"
#include "DataIO.h"
#include "PropertyIndex.h"


KeyDb::KeyDb(SQLiteDataBase* env, const char* filename, const char* dbname, bool bReadOnly, bool bUseIntKey)
{
    int res;

    m_db = new SQLiteTable(env);
	
	m_IndexNeedsRegen = false;

    int readOnlyFlag = bReadOnly ? SQLiteDB_RDONLY : 0;

    //try to open a database that already exists
    if (res = m_db->open(0, filename, dbname, readOnlyFlag, 0, bUseIntKey) != 0)
    {
        //must close even if open failed
        m_db->close(0);
        delete m_db;

        if (bReadOnly)
            throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_4_CONNECTION_IS_READONLY)));

        m_db = new SQLiteTable(env);

        //if that fails, create one
        if (res = m_db->open(0, filename, dbname, SQLiteDB_CREATE, 0, bUseIntKey) != 0)
        {
            //printf("%s\n", env->strerror(res));
            throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_10_ERROR_ACCESSING_SDFDB)));
        }
    }    
}

KeyDb::~KeyDb()
{
    m_db->close(0);
    delete m_db;
}


void KeyDb::InsertKey(SQLiteData* key, REC_NO recno)
{
    BinaryWriter wrtdata(4);
    wrtdata.WriteInt32(recno);

    SQLiteData data(wrtdata.GetData(), wrtdata.GetDataLen());
    
    if (m_db->put(0, key, &data, 0) != 0)
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_16_ERROR_INSERTING_KEY)));
}

void KeyDb::DeleteKey(FdoClassDefinition* fc, FdoIFeatureReader* reader)
{
    BinaryWriter wrtkey(64);

    DataIO::MakeKey(fc, reader, wrtkey);    

    SQLiteData key(wrtkey.GetData(), wrtkey.GetDataLen());

    if (m_db->del(0, &key, 0) != 0)
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_17_ERROR_DELETING_KEY)));
}


void KeyDb::DeleteKey(SQLiteData* key)
{
    if (m_db->del(0, key, 0) != 0)
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_17_ERROR_DELETING_KEY)));
}


void KeyDb::DeleteKey(FdoClassDefinition* fc, PropertyIndex* pi, FdoPropertyValueCollection* pvc, REC_NO recno)
{
    BinaryWriter wrtkey(64);

    DataIO::MakeKey(fc, pi, pvc, wrtkey, recno);    

    SQLiteData key(wrtkey.GetData(), wrtkey.GetDataLen());
       
    if (m_db->del(0, &key, 0) != 0)
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_17_ERROR_DELETING_KEY)));
}


REC_NO KeyDb::FindRecno(FdoClassDefinition* fc, FdoPropertyValueCollection* pvc)
{
    BinaryWriter wrtkey(64);

    //In this case we assume the property value collection contains
    //an entry for the autogenerated properties also, since the use
    //case for this function is in places where we get the feature
    //record from the feature database.
    //In the case of NULL pi, the key will be generated in the ordinary 
    //way as if it has no autogenerated values.
    DataIO::MakeKey(fc, NULL, pvc, wrtkey, 0);    

    SQLiteData key(wrtkey.GetData(), wrtkey.GetDataLen());
    SQLiteData data;

    if (m_db->get(0, &key, &data, 0) != 0  )
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_18_KEY_NOT_FOUND)));

	if( data.get_size() != sizeof(REC_NO) )
	{
		m_IndexNeedsRegen = true;
		return 0; // record number start from 1; this means something happened to the index table that need to re-gened
	}
    return *(REC_NO*)data.get_data();
}


bool KeyDb::KeyExists(SQLiteData* key)
{
    SQLiteData data;

    int ret = m_db->get(0, key, &data, 0);
    
    if (ret == 0)
        return true;

    if (ret == SQLiteDB_NOTFOUND || ret == SQLiteDB_KEYEMPTY)
        return false;

    //any error other than NOTFOUND is bad news
    throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_10_ERROR_ACCESSING_SDFDB)));
}

void KeyDb::CloseCursor()
{
	m_db->close_cursor();
}

