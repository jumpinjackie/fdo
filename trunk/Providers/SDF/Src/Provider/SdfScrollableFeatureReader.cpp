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
#include "SdfScrollableFeatureReader.h"

#include "RTree.h"
#include "KeyDb.h"
#include "SdfConnection.h"
#include "DataIO.h"
#include "DataDb.h"

#include "SdfDataValidator.h"

SdfScrollableFeatureReader::SdfScrollableFeatureReader(SdfConnection* connection, 
                                                   FdoClassDefinition* classDef )
: SdfSimpleFeatureReader(connection, classDef, NULL, NULL, NULL, NULL)
{
    m_data = connection->GetDataDb(classDef);
	m_validCursorPosition = false;
}

SdfScrollableFeatureReader::SdfScrollableFeatureReader( SdfConnection* connection, FdoClassDefinition* classDef, SdfConnection* srcConnection, FdoClassDefinition* srcClassDef )
: SdfSimpleFeatureReader(srcConnection, srcClassDef, NULL, NULL, NULL, NULL)
{
    m_data = connection->GetDataDb(classDef);
	m_validCursorPosition = false;
}

SdfScrollableFeatureReader::~SdfScrollableFeatureReader()
{
}

void SdfScrollableFeatureReader::Dispose()
{
    delete this;
}

void SdfScrollableFeatureReader::InitDataReader()
{
	m_dataReader->Reset((unsigned char*)m_currentData->get_data(), m_currentData->get_size());

	if (!m_stringPropsCache.empty())
    {
        std::map<std::wstring, wchar_t*>::iterator iter = m_stringPropsCache.begin();

        for (; iter != m_stringPropsCache.end(); iter++)
            delete [] iter->second;

        m_stringPropsCache.clear();
    }
}

int SdfScrollableFeatureReader::Count()
{
	int count = 0;
	SQLiteData key;
	if( m_validCursorPosition )
	{
		// Save the existing cursor
		key.set_data( m_currentKey->get_data() );
		key.set_size( m_currentKey->get_size() );
	}
	int ret = m_dbData->GetFirstFeature( m_currentKey, m_currentData );
	if( ret != SQLiteDB_OK )
		return count;

	do{
		count++;
	} while (m_dbData->GetNextFeature( m_currentKey, m_currentData ) == SQLiteDB_OK );

	// Put back the cursor at the original position
	if( m_validCursorPosition )
	{
		m_currentKey->set_data( key.get_data() );
		m_currentKey->set_size( key.get_size() );
		if( m_dbData->GetFeature( m_currentKey, m_currentData ) == SQLiteDB_OK )
			InitDataReader();
		else
			m_validCursorPosition = false;
	}
	return count;
}

//overloaded ReadNext() updates features after positioning reader 
bool SdfScrollableFeatureReader::ReadNext()
{
	int ret;
	if( ! m_validCursorPosition )
		ret = m_dbData->GetFirstFeature( m_currentKey, m_currentData );
	else
		ret = m_dbData->GetNextFeature( m_currentKey, m_currentData );

	if( ret != SQLiteDB_OK )
	{
		m_validCursorPosition = false;
		return false;
	}

	m_validCursorPosition = true;
	InitDataReader();

	return true;
}

bool SdfScrollableFeatureReader::ReadFirst()
{
	int ret = m_dbData->GetFirstFeature( m_currentKey, m_currentData );
	
	if( ret != SQLiteDB_OK )
	{
		m_validCursorPosition = false;
		return false;
	}

	m_validCursorPosition = true;
	InitDataReader();

	return true;
}

bool SdfScrollableFeatureReader::ReadLast()
{
	int ret = m_dbData->GetLastFeature( m_currentKey, m_currentData );
	
	if( ret != SQLiteDB_OK )
	{
		m_validCursorPosition = false;
		return false;
	}

	m_validCursorPosition = true;
	InitDataReader();

	return true;
}

bool SdfScrollableFeatureReader::ReadPrevious()
{
	int ret;
	if( ! m_validCursorPosition )
		ret = m_dbData->GetLastFeature( m_currentKey, m_currentData );
	else
		ret = m_dbData->GetPreviousFeature( m_currentKey, m_currentData );

	if( ret != SQLiteDB_OK )
	{
		m_validCursorPosition = false;
		return false;
	}

	m_validCursorPosition = true;
	InitDataReader();

	return true;
}

bool SdfScrollableFeatureReader::ReadAt(FdoPropertyValueCollection* key)
{
	if( m_dbData->GetFeatureAt( m_currentKey, m_currentData, key ) != SQLiteDB_OK )
		return false;

	m_validCursorPosition = true;
	InitDataReader();

	return true;
}

bool SdfScrollableFeatureReader::ReadAtIndex( unsigned int recordindex )
{
	int count = 1;

	// Need to make a table scan
	int ret = m_dbData->GetFirstFeature( m_currentKey, m_currentData );
	if( ret != SQLiteDB_OK )
		return false;

	do{
		count++;
	} while ( m_dbData->GetNextFeature( m_currentKey, m_currentData ) == SQLiteDB_OK && count != recordindex );

	if( count != recordindex )
		return false;

	m_validCursorPosition = true;
	InitDataReader();

	return true;
}

unsigned int SdfScrollableFeatureReader::IndexOf(FdoPropertyValueCollection* keyVal)
{
	unsigned char* keyData = NULL;
	int index = 0;
	if( m_dbData->GetFeatureAt( m_currentKey, m_currentData, keyVal ) != SQLiteDB_OK )
		return 0; // One-based; so zero is an error condition

	keyData = new unsigned char[m_currentKey->get_size()];
	memcpy(keyData,m_currentKey->get_data(),m_currentKey->get_size() );

	int ret = m_dbData->GetFirstFeature( m_currentKey, m_currentData );
	if( ret != SQLiteDB_OK )
	{
		delete[] keyData;
		return index;
	}
	bool found = false;
	do{
		index++;
		if( memcmp(keyData, m_currentKey->get_data(), m_currentKey->get_size() ) == 0 )
		{
			found = true;
			break;
		}
	} while ( m_dbData->GetNextFeature( m_currentKey, m_currentData ) == SQLiteDB_OK );

	delete[] keyData;

	if( found )
		return index;

	return 0;
}

SQLiteData* SdfScrollableFeatureReader::GetCurrentKey()
{
	return m_currentKey;
}
