/*
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
 
#include "stdafx.h"
#include <Sm/Ph/TableComponentReader.h>
#include <Sm/Ph/Mgr.h>

FdoSmPhTableComponentReader::FdoSmPhTableComponentReader(
    FdoStringP tableName, 
    FdoStringP tableRowName, 
    FdoStringP tableFieldName, 
    FdoSmPhReaderP reader
) :
    FdoSmPhGroupReader( tableName, reader ),
    mTableRowName(tableRowName),
    mTableFieldName(tableFieldName)
{
}

FdoSmPhTableComponentReader::~FdoSmPhTableComponentReader(void)
{
}

FdoStringP FdoSmPhTableComponentReader::GetGroupName()
{
    return GetString( mTableRowName, mTableFieldName );
}

FdoSmPhTableIndexReader::FdoSmPhTableIndexReader(
    FdoStringP tableName, 
    FdoPtr<FdoSmPhRdIndexReader> reader
) :
    FdoSmPhTableComponentReader( tableName, L"", L"table_name", reader->SmartCast<FdoSmPhReader>() ),
    mIndexReader(reader)
{
}

FdoSmPhTableIndexReader::~FdoSmPhTableIndexReader(void)
{
}

FdoSmPhIndexType FdoSmPhTableIndexReader::GetIndexType()
{
    return mIndexReader->GetIndexType();
}

FdoSmPhTableColumnReader::FdoSmPhTableColumnReader(
    FdoStringP tableName, 
    FdoPtr<FdoSmPhRdColumnReader> reader
) :
    FdoSmPhTableComponentReader( tableName, L"", L"table_name", reader->SmartCast<FdoSmPhReader>() ),
    mColumnReader(reader)
{
}

FdoSmPhTableColumnReader::~FdoSmPhTableColumnReader(void)
{
}

FdoSmPhColType FdoSmPhTableColumnReader::GetType()
{
    return mColumnReader->GetType();
}

FdoPtr<FdoSmPhRdColumnReader> FdoSmPhTableColumnReader::GetColumnReader()
{
    return mColumnReader;
}

FdoSmPhTableDependencyReader::FdoSmPhTableDependencyReader(
    FdoStringP tableName, 
    FdoStringP rowName,
    FdoStringP fieldName,
    FdoPtr<FdoSmPhDependencyReader> reader
) :
    FdoSmPhTableComponentReader( tableName, rowName, fieldName, reader->SmartCast<FdoSmPhReader>() ),
    mDependencyReader(reader)
{
}

FdoSmPhTableDependencyReader::~FdoSmPhTableDependencyReader(void)
{
}

FdoPtr<FdoSmPhDependencyReader> FdoSmPhTableDependencyReader::GetDependencyReader()
{
    return mDependencyReader;
}
