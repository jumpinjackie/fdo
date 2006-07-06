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
#include "PropertyIndex.h"
#include "SdfIndexedScrollableFeatureReader.h"

#include "RTree.h"
#include "KeyDb.h"
#include "SdfConnection.h"
#include "DataIO.h"
#include "DataDb.h"

#include "SdfDataValidator.h"

SdfIndexedScrollableFeatureReader::SdfIndexedScrollableFeatureReader(SdfConnection* srcConnection, FdoClassDefinition* srcClass, REC_NO *table, int tableSize  )
: SdfSimpleFeatureReader(srcConnection, srcClass, NULL, NULL, NULL, NULL)
{
    m_IndexTable = table;  // Transfer the ownership of this array; need to be freed by this class
	m_TableSize = tableSize;
	m_Sourcedata = srcConnection->GetDataDb( srcClass );
	m_Sourcekey = srcConnection->GetKeyDb( srcClass );
	m_CurrentIndex = -1;

}

SdfIndexedScrollableFeatureReader::~SdfIndexedScrollableFeatureReader()
{
	if( m_IndexTable != NULL )
		delete[] m_IndexTable;
}

void SdfIndexedScrollableFeatureReader::Dispose()
{
    delete this;
}

void SdfIndexedScrollableFeatureReader::InitDataReader()
{
	m_dataReader->Reset((unsigned char*)m_currentData->get_data(), m_currentData->get_size());

	m_currentFeatureRecno = *(REC_NO*)(m_currentKey->get_data());

	if (!m_stringPropsCache.empty())
    {
        std::map<std::wstring, wchar_t*>::iterator iter = m_stringPropsCache.begin();

        for (; iter != m_stringPropsCache.end(); iter++)
            delete [] iter->second;

        m_stringPropsCache.clear();
    }
}
void SdfIndexedScrollableFeatureReader::InitCurrentKey()
{
	m_currentKey->set_size( 4 );
	m_currentKey->set_data( &m_IndexTable[m_CurrentIndex] );
}

bool SdfIndexedScrollableFeatureReader::ReadNext()
{
	m_CurrentIndex++;
	if( m_CurrentIndex >= m_TableSize )
	{
		m_CurrentIndex = -1;
		return false;
	}

	InitCurrentKey();

	if( m_Sourcedata->GetFeatureAt( m_currentKey, m_currentData ) != SQLiteDB_OK )
		return false;

	InitDataReader();

	return true;
}

int SdfIndexedScrollableFeatureReader::Count()
{
	return m_TableSize;
}

bool SdfIndexedScrollableFeatureReader::ReadFirst()
{
	m_CurrentIndex = 0;

	InitCurrentKey();

	if( m_Sourcedata->GetFeatureAt( m_currentKey, m_currentData ) != SQLiteDB_OK )
		return false;

	InitDataReader();

	return true;
}

bool SdfIndexedScrollableFeatureReader::ReadLast()
{
	m_CurrentIndex = m_TableSize - 1;

	InitCurrentKey();

	if( m_Sourcedata->GetFeatureAt( m_currentKey, m_currentData ) != SQLiteDB_OK )
		return false;

	InitDataReader();

	return true;
}

bool SdfIndexedScrollableFeatureReader::ReadPrevious()
{
	m_CurrentIndex--;
	if( m_CurrentIndex < 0 )
	{
		m_CurrentIndex = -1;
		return false;
	}

	InitCurrentKey();

	if( m_Sourcedata->GetFeatureAt( m_currentKey, m_currentData ) != SQLiteDB_OK )
		return false;

	InitDataReader();

	return true;
}

bool SdfIndexedScrollableFeatureReader::ReadAt(FdoPropertyValueCollection* key)
{
	unsigned int recordindex = IndexOf( key );
	if( recordindex == 0 )
		return false;

	return ReadAtIndex( recordindex );
}

bool  SdfIndexedScrollableFeatureReader::ReadAtIndex( unsigned int recordindex )
{
	if( recordindex >= m_TableSize || recordindex < 1 )
		return false;

	m_CurrentIndex = recordindex-1;  // Paramter is 1-based

	InitCurrentKey();

	if( m_Sourcedata->GetFeatureAt( m_currentKey, m_currentData ) != SQLiteDB_OK )
		return false;

	InitDataReader();

	return true;
}

unsigned int SdfIndexedScrollableFeatureReader::IndexOf(FdoPropertyValueCollection* keyVal)
{
	REC_NO rowid = m_Sourcekey->FindRecno( m_class, keyVal );
	
	for(int i=0; i<m_TableSize; i++ )
		if( m_IndexTable[i] == rowid )
			return i+1; // One-based

	return 0;
}
