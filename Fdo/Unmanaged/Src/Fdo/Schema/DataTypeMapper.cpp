/*
 * 
* Copyright (C) 2004-2006  Autodesk, Inc.
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of version 2.1 of the GNU Lesser
* General Public License as published by the Free Software Foundation.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
* 
 */

#include "DataTypeMapper.h"

std::map<std::wstring,FdoDataType> FdoDataTypeMapper::mStdMap;

static FdoDataTypeMapper::MapInitializer InitializeMapSingleton;

FdoDataTypeMapper::FdoDataTypeMapper(void)
{
}

FdoDataTypeMapper::~FdoDataTypeMapper(void)
{
}

FdoDataType FdoDataTypeMapper::String2Type( const FdoString* dataTypeString, bool* pbFound )
{
    if ( pbFound )
        (*pbFound) = true;

	std::map <std::wstring,FdoDataType>::iterator iter = mStdMap.find( dataTypeString );
	if( iter != mStdMap.end() )  
        return iter->second;

    if ( pbFound ) {
        (*pbFound) = false;
    }
    else {
        throw FdoSchemaException::Create(
            FdoSchemaException::NLSGetMessage( 
                FDO_NLSID(SCHEMA_33_BADDATAPROPSTRING), 
                dataTypeString              
            )
        );
    }


    // Get to here only if type not found and pbFound
    // was set.
    // Return value is actually meaningless in this 
    // case but need to return something to prevent a 
    // compilation warning.
    return(FdoDataType_String);
}

FdoStringP FdoDataTypeMapper::Type2String( FdoDataType dataType )
{
	std::map <std::wstring,FdoDataType>::iterator iter = mStdMap.begin();
    for( iter = mStdMap.begin(); iter != mStdMap.end(); iter++ )
        if( dataType == iter->second )
            break;

    if( iter != mStdMap.end() )  
        return FdoStringP(iter->first.c_str());

    throw FdoSchemaException::Create(
        FdoSchemaException::NLSGetMessage( 
            FDO_NLSID(SCHEMA_34_BADDATAPROPTYPE), 
            dataType              
        )
    );

}

void FdoDataTypeMapper::InitMap()
{
    // Add all FDO data types.

	mStdMap.insert( std::pair<std::wstring,FdoDataType> ( L"boolean", FdoDataType_Boolean ) );

	mStdMap.insert( std::pair<std::wstring,FdoDataType> ( L"byte", FdoDataType_Byte ) );

	mStdMap.insert( std::pair<std::wstring,FdoDataType> ( L"datetime", FdoDataType_DateTime ) );

	mStdMap.insert( std::pair<std::wstring,FdoDataType> ( L"decimal", FdoDataType_Decimal ) );

	mStdMap.insert( std::pair<std::wstring,FdoDataType> ( L"double", FdoDataType_Double ) );

	mStdMap.insert( std::pair<std::wstring,FdoDataType> ( L"int16", FdoDataType_Int16 ) );

	mStdMap.insert( std::pair<std::wstring,FdoDataType> ( L"int32", FdoDataType_Int32 ) );

	mStdMap.insert( std::pair<std::wstring,FdoDataType> ( L"int64", FdoDataType_Int64 ) );

	mStdMap.insert( std::pair<std::wstring,FdoDataType> ( L"single", FdoDataType_Single ) );

	mStdMap.insert( std::pair<std::wstring,FdoDataType> ( L"string", FdoDataType_String ) );

	mStdMap.insert( std::pair<std::wstring,FdoDataType> ( L"blob", FdoDataType_BLOB ) );

	mStdMap.insert( std::pair<std::wstring,FdoDataType> ( L"clob", FdoDataType_CLOB ) );
}

