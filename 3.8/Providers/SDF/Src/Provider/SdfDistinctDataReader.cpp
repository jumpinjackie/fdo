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
#include "SdfDistinctDataReader.h"
#include "BinaryReader.h"
#include "BinaryWriter.h"
#include "DataIO.h"
#include "PropertyIndex.h"

SdfDistinctDataReader::SdfDistinctDataReader(FdoIFeatureReader* reader, FdoIdentifierCollection* idents)
: SdfDataReader()
{
    m_idents = idents;
    FDO_SAFE_ADDREF(m_idents);

    m_reader = reader;
    FDO_SAFE_ADDREF(m_reader);

    //get the updated class definition. It should only contain
    //properties which are relevant to the select disticnt
    m_class = reader->GetClassDefinition();

    //now create a temporary PropertyIndex structure corresponding
    //to that class. Normally we do not use temporary PropertyIndex
    //structures since they are used for serialization of real
    //properties (not computed) but in this case we are going to
    //be serializing computed properties in the process of determining
    //distinct so it is ok.
    //Use 0 as feature class ID, since this is not a fully defined
    //class definition.
    m_propIndex = new PropertyIndex(m_class, 0);

    //find disticnt values
    RunQuery();

    _ASSERT(m_db);

    //obtain a cursor we will use to traverse the db
    int ret = m_db->cursor(0, &m_dbc, 0);

    _ASSERT(ret == 0);

    m_binReader = new BinaryReader(NULL, 0, m_propIndex->GetNumProps() );

    m_currentKey = new SQLiteData();

    m_currentData = new SQLiteData();
    m_currentData->set_size(sizeof(int)); 

}

SdfDistinctDataReader::~SdfDistinctDataReader()
{
    FDO_SAFE_RELEASE(m_idents);
    FDO_SAFE_RELEASE(m_reader);
    FDO_SAFE_RELEASE(m_class);
    delete m_binReader;
    delete m_currentKey;
    delete m_currentData;
    Close();
}

void SdfDistinctDataReader::Dispose()
{
    delete this;
}

void SdfDistinctDataReader::Close()
{
    if (m_db)
    {
        m_dbc->close();
        m_db->close(0);
        delete m_db;
        m_db = NULL;
        m_dbc = NULL;
    }
}
 
BinaryReader* SdfDistinctDataReader::GetBinaryReader()
{
    return m_binReader;
}


bool SdfDistinctDataReader::ReadNext()
{
    //read a next record making sure we are not at the end
    //of the database
    if (m_dbc->get(m_currentKey, m_currentData, SQLiteDB_NEXT) == SQLiteDB_NOTFOUND)
        return false;

    m_binReader->Reset((unsigned char*)m_currentKey->get_data(), m_currentKey->get_size());

    return true;
}


//goes over the feature reader determining distinct values.
void SdfDistinctDataReader::RunQuery()
{
    _ASSERT(m_reader);
    _ASSERT(m_idents);
    
    if (m_idents->GetCount() == 0)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_44_NEED_PROPERTY_FOR_DISTINCT)));

    //create a hashtable database we will use
    //to weed out duplicates
    SQLiteTable* db = new SQLiteTable(NULL);
    
    if (db->open(0, NULL, NULL, NULL, SQLiteDB_CREATE, 0) != 0)
    {
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_10_ERROR_ACCESSING_SDFDB)));
    }

    BinaryWriter wrt(256);

    //just a placeholder data value... all
    //we will care about are the key values of 
    //the temporary database
    int placeholder = 1;
    SQLiteData data(&placeholder, sizeof(int));

    while (m_reader->ReadNext())
    {
        wrt.Reset();

        //create a feature data record for the properties we need
        //based on the data in the feature reader
        DataIO::UpdateDataRecord(m_class, m_propIndex, NULL, m_reader, wrt);

        SQLiteData key(wrt.GetData(), wrt.GetDataLen());

        if (db->put(0, &key, &data, 0) != 0)
        {
            db->close(0);
            delete db;
            db = NULL;

            throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_10_ERROR_ACCESSING_SDFDB)));
        }
    }

    m_db = db;
}


