//
// Copyright (C) 2006 Refractions Research, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#include "stdafx.h"
#include "DbObject.h"
#include "Owner.h"
#include "Rd/BaseObjectReader.h"
#include "Rd/ColumnReader.h"
#include "Rd/IndexReader.h"
#include "ColumnChar.h"
#include "ColumnBool.h"
#include "ColumnByte.h"
#include "ColumnInt16.h"
#include "ColumnInt32.h"
#include "ColumnInt64.h"
#include "ColumnDecimal.h"
#include "ColumnDate.h"
#include "ColumnSingle.h"
#include "ColumnDouble.h"
#include "ColumnBLOB.h"
#include "ColumnGeom.h"
#include "ColumnUnknown.h"
#include "Index.h"
#include "SpatialIndex.h"
#include <Sm/Ph/TableComponentReader.h>
#include <FdoCommonStringUtil.h>

FdoSmPhPostGisDbObject::FdoSmPhPostGisDbObject()
{
    // idle
}

FdoSmPhPostGisDbObject::FdoSmPhPostGisDbObject(FdoStringP name,
    const FdoSmPhOwner* owner,
    FdoSmPhRdDbObjectReader* reader)
    : FdoSmPhDbObject(name, owner)
{
    // idle
}

FdoSmPhPostGisDbObject::~FdoSmPhPostGisDbObject()
{
    // idle
}

FdoStringP FdoSmPhPostGisDbObject::GetDbQName() const
{
    FdoStringP fullName(GetName());
    FdoStringP userName;
    FdoStringP objectName;
    FdoStringP dbName;

    if ( fullName.Contains(L".") ) {
        userName = fullName.Left(L".");
        objectName = fullName.Right(L".");
        dbName = FdoStringP(L"\"") + userName + L"\".\"" + objectName + L"\"";
    }
    else
    {
        dbName = FdoStringP(L"\"") + fullName + L"\"";
    }
    return dbName;
}

bool FdoSmPhPostGisDbObject::GetSupportsWrite() const
{
    // It has been decided to return "true" as the default
    // value because it has been deemed to expensive to determine
    // whether or not the class is writable.

    return true;
}

FdoStringP FdoSmPhPostGisDbObject::GetBestSchemaName() const
{
    FdoStringP objName( GetName() );
    objName = objName.Contains( L"." ) ? objName.Left(L".") : FdoStringP(L"public");

    // Filter out characters not allowed in schema element names.
    return objName.Replace( L":", L"_" );
}

FdoStringP FdoSmPhPostGisDbObject::GetBestClassName() const
{
    FdoStringP objName( GetName() );
    objName = objName.Contains( L"." ) ? objName.Right(L".") : objName;

    // Filter out characters not allowed in schema element names.
    return objName.Replace(L":",L"_").Replace(L".",L"_");
}

FdoSmPhColumnP FdoSmPhPostGisDbObject::Position2Column( FdoInt32 position )
{
    FdoSmPhPostGisColumnP column;
    FdoSmPhColumnsP columns = GetColumns();

    if ( position > 0 ) {

        // Do quick retrieval, assuming that column position in database matches that of the cache.
        if ( position <= columns->GetCount() ) {
            column = columns->GetItem( position - 1 )->SmartCast<FdoSmPhPostGisColumn>(true);
        
            // Verify that we go the column at the right position. If a column was deleted
            // from this DbObject then we can end up with the wrong column. Deleted columns
            // keep their positions in the database but they are not cached.
            if ( column->GetPosition() != position ) 
                column = NULL;
        }

        if ( !column ) {
            // Quick retrieve didn't work, do a linear search.
            for ( FdoInt32 ix = 0; ix < columns->GetCount(); ix++ ) {
                FdoSmPhPostGisColumnP currColumn = columns->GetItem(ix)->SmartCast<FdoSmPhPostGisColumn>(true);
                if ( currColumn->GetPosition() == position ) {
                    column = currColumn;
                    break;
                }
            }
        }
    }

    return column;
}

void FdoSmPhPostGisDbObject::ActivateOwnerAndExecute(FdoStringP sqlStmt)
{
    // Remove const-qualifier
    FdoSmSchemaElement* smElement = NULL;
    smElement = const_cast<FdoSmSchemaElement*>(GetParent());

    FdoSmPhPostGisOwner* pgOwner = NULL;
    pgOwner = static_cast<FdoSmPhPostGisOwner*>(smElement);

    pgOwner->ActivateAndExecute(sqlStmt);
}

FdoPtr<FdoSmPhIndex> FdoSmPhPostGisDbObject::CreateIndex(
    FdoPtr<FdoSmPhTableIndexReader> rdr
)
{
    FdoSmPhIndexP index;
    FdoSmPhColumnGeomP columnGeom;

    // Determine index type. It's a spatial index if it has only one
    // column and that column is geometric.

    // Column name contains space-separated list of positions of index columns.
    // Positions are 1-based.
    FdoStringP columnPosition = rdr->GetString(L"",L"column_name");

    if ( columnPosition.Right(L" ") == L"" ) {
        // Index has only one column.
        FdoInt32 columnIndex = columnPosition.ToLong();

        if ( (columnIndex > 0) && (columnIndex <= GetColumns()->GetCount()) )
            // Get the index column.
            columnGeom = GetColumns()->GetItem(columnIndex - 1)->SmartCast<FdoSmPhColumnGeom>(true);
    }

    if  ( columnGeom ) {
        // Index column is geometric, create a spatial index
        index = NewSpatialIndex(
            rdr->GetString(L"",L"index_name"), 
            (rdr->GetString(L"",L"uniqueness") == L"UNIQUE") ? true : false,
            FdoSchemaElementState_Unchanged
        );
    }
    else {
        // Otherwise, create regular index.
        index = NewIndex(
            rdr->GetString(L"",L"index_name"), 
            (rdr->GetString(L"",L"uniqueness") == L"UNIQUE") ? true : false,
            FdoSchemaElementState_Unchanged
        );
    }

    return index;
}



FdoSmPhColumnP FdoSmPhPostGisDbObject::NewColumnBLOB(FdoStringP columnName,
    FdoSchemaElementState state,
    bool isNullable,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colReader)
{
    FdoSmPhPostGisColumnBLOB* col = NULL;
    col = new FdoSmPhPostGisColumnBLOB(columnName, state, this,
        isNullable, rootColumnName, colReader);

    return col;
}

FdoSmPhColumnP FdoSmPhPostGisDbObject::NewColumnChar(FdoStringP columnName,
    FdoSchemaElementState state,
    bool isNullable,
    int length,
    FdoStringP rootColumnName,
    FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* colReader)
{
    FdoSmPhPostGisColumnChar* col = NULL;
    col = new FdoSmPhPostGisColumnChar(columnName, state, this,
        isNullable, length, rootColumnName, defaultValue, colReader);

    return col;
}

FdoSmPhColumnP FdoSmPhPostGisDbObject::NewColumnDate(FdoStringP columnName,
    FdoSchemaElementState state,
    bool isNullable,
    FdoStringP rootColumnName,
    FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* colReader)
{
    FdoSmPhPostGisColumnDate* col = NULL;
    col = new FdoSmPhPostGisColumnDate(columnName, state, this,
        isNullable, rootColumnName, defaultValue, colReader);

    return col;
}

FdoSmPhColumnP FdoSmPhPostGisDbObject::NewColumnDecimal(FdoStringP columnName,
    FdoSchemaElementState state,
    bool isNullable,
    int length,
    int scale,
    FdoStringP rootColumnName,
    FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* colReader)
{
    FdoSmPhPostGisColumnDecimal* col = NULL;
    col = new FdoSmPhPostGisColumnDecimal(columnName, state, this,
        isNullable, length, scale, rootColumnName, defaultValue, colReader);

    return col;
}

FdoSmPhColumnP FdoSmPhPostGisDbObject::NewColumnSingle(FdoStringP columnName,
    FdoSchemaElementState state,
    bool isNullable,
    FdoStringP rootColumnName,
    FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* colReader)
{
    FdoSmPhPostGisColumnSingle* col = NULL;
    col = new FdoSmPhPostGisColumnSingle(columnName, state, this,
        isNullable, rootColumnName, defaultValue, colReader);
    
    return col;
}

FdoSmPhColumnP FdoSmPhPostGisDbObject::NewColumnDouble(FdoStringP columnName,
    FdoSchemaElementState state,
    bool isNullable,
    FdoStringP rootColumnName,
    FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* colReader)
{
    FdoSmPhPostGisColumnDouble* col = NULL;
    col = new FdoSmPhPostGisColumnDouble(columnName, state, this,
        isNullable, rootColumnName, defaultValue, colReader);

    return col;
}

FdoSmPhColumnP FdoSmPhPostGisDbObject::NewColumnGeom(FdoStringP columnName,
    FdoSchemaElementState state,
    FdoSmPhScInfoP AssociatedSCInfo,
    bool isNullable,
    bool hasElevation,
    bool hasMeasure,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colReader)
{
    FdoSmPhPostGisColumnGeom* col = NULL;
    col = new FdoSmPhPostGisColumnGeom(columnName, state, this,
        AssociatedSCInfo, isNullable, hasElevation, hasMeasure,
        rootColumnName, colReader);

    return col;
}

FdoSmPhColumnP FdoSmPhPostGisDbObject::NewColumnBool(FdoStringP columnName,
    FdoSchemaElementState state,
    bool isNullable,
    FdoStringP rootColumnName,
    FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* colReader)
{
    FdoSmPhPostGisColumnBool* col = NULL;
    col = new FdoSmPhPostGisColumnBool(columnName, state, this,
        isNullable, rootColumnName, defaultValue, colReader);
    
    return col;
}

FdoSmPhColumnP FdoSmPhPostGisDbObject::NewColumnByte(FdoStringP columnName,
    FdoSchemaElementState state,
    bool isNullable,
    FdoStringP rootColumnName,
    FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* colReader)
{
    FdoSmPhPostGisColumnByte* col = NULL;
    col = new FdoSmPhPostGisColumnByte(columnName, state, this,
        isNullable, rootColumnName, defaultValue, colReader);

    return col;
}

FdoSmPhColumnP FdoSmPhPostGisDbObject::NewColumnInt16(FdoStringP columnName,
    FdoSchemaElementState state,
    bool isNullable,
    bool isAutoincremented,    // Not supported
    FdoStringP rootColumnName,
    FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* colReader)
{
    FdoSmPhPostGisColumnInt16* col = NULL;
    col = new FdoSmPhPostGisColumnInt16(columnName, state, this,
        isNullable, rootColumnName, defaultValue, colReader);

    return col;
}

FdoSmPhColumnP FdoSmPhPostGisDbObject::NewColumnInt32(FdoStringP columnName,
    FdoSchemaElementState state,
    bool isNullable,
    bool isAutoincremented,    // Not supported
    FdoStringP rootColumnName,
    FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* colReader)
{
    FdoSmPhPostGisColumnInt32* col = NULL;
    col = new FdoSmPhPostGisColumnInt32(columnName, state, this,
        isNullable,isAutoincremented, rootColumnName, defaultValue, colReader);

    return col;
}

FdoSmPhColumnP FdoSmPhPostGisDbObject::NewColumnInt64(FdoStringP columnName,
    FdoSchemaElementState state,
    bool isNullable,
	bool isAutoincremented,
    FdoStringP rootColumnName,
    FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* colReader)
{
    FdoSmPhPostGisColumnInt64* col = NULL;
    col = new FdoSmPhPostGisColumnInt64(columnName, state, this,
        isNullable, isAutoincremented, rootColumnName, defaultValue, colReader);

    return col;
}

FdoSmPhColumnP FdoSmPhPostGisDbObject::NewColumnUnknown(FdoStringP columnName,
    FdoStringP typeName,
    FdoSchemaElementState state,
    bool isNullable,
    int length,
    int scale,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colReader)
{
    FdoSmPhPostGisColumnUnknown* col = NULL;
    col = new FdoSmPhPostGisColumnUnknown(columnName, typeName, state, this,
        isNullable, length, scale, rootColumnName, colReader);

    return col;
}

FdoSmPhColumnP FdoSmPhPostGisDbObject::NewColumnDbObject(FdoStringP columnName,
    FdoSchemaElementState state,
    bool isNullable,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colReader)
{
    // TODO: mloskot - Should 'int' be replaced with FdoInt32?

    FdoSmPhPostGisColumnChar* col = NULL;
    col = new FdoSmPhPostGisColumnChar(columnName, state, this,
        isNullable, static_cast<int>(GetManager()->DbObjectNameMaxLen()),
        rootColumnName, (FdoDataValue*) NULL, colReader);

    return col;
}

FdoSmPhIndexP FdoSmPhPostGisDbObject::NewIndex(FdoStringP name,
    bool isUnique,
    FdoSchemaElementState state)
{
    FdoSmPhPostGisIndex* index = NULL;
    index = new FdoSmPhPostGisIndex(name, this, isUnique, state);

    return index;
}

FdoSmPhIndexP FdoSmPhPostGisDbObject::NewSpatialIndex(FdoStringP name,
    bool isUnique,
    FdoSchemaElementState state)
{
    return new FdoSmPhPostGisSpatialIndex( name, this, state );
}

FdoPtr<FdoSmPhRdBaseObjectReader> FdoSmPhPostGisDbObject::CreateBaseObjectReader() const
{
    FdoSmPhPostGisDbObject* pDbObject = (FdoSmPhPostGisDbObject*) this;

    return new FdoSmPhRdPostGisBaseObjectReader( FDO_SAFE_ADDREF(pDbObject) );
}

FdoPtr<FdoSmPhRdColumnReader> FdoSmPhPostGisDbObject::CreateColumnReader()
{
    FdoSmPhRdPostGisColumnReader* colReader = NULL;
    FdoSmPhOwnerP owner = FDO_SAFE_ADDREF(static_cast<FdoSmPhOwner*>(const_cast<FdoSmSchemaElement*>(GetParent())));
    colReader = new FdoSmPhRdPostGisColumnReader(owner,
        FDO_SAFE_ADDREF(this));

    return colReader;
}

FdoPtr<FdoSmPhRdIndexReader> FdoSmPhPostGisDbObject::CreateIndexReader() const
{
    // Remove const qualifiers cleanly
    FdoSmPhPostGisDbObject* thisTable = NULL;
    thisTable = const_cast<FdoSmPhPostGisDbObject*>(this);
    FDO_SAFE_ADDREF(thisTable);

    FdoSmPhOwner* owner = NULL;
    owner = const_cast<FdoSmPhOwner*>((const FdoSmPhOwner*) GetParent());
    FDO_SAFE_ADDREF(owner);

    // Create index reader
    FdoSmPhRdPostGisIndexReader* reader = NULL;
    reader = new FdoSmPhRdPostGisIndexReader(
        owner, thisTable);

    return reader;
}

void FdoSmPhPostGisDbObject::LoadPkeyColumn( FdoSmPhReaderP pkeyRdr, FdoSmPhColumnsP pkeyColumns )
{
    // columnName actually contains list of positions for index columns.
    FdoStringP columnName = pkeyRdr->GetString(L"",L"column_name");
    // Strip out enclosing {}.
    columnName = columnName.Mid( 1, columnName.GetLength() - 2 );

    // Parse the position list (comma-separated elements).
    FdoStringsP columnPositions = FdoStringCollection::Create( columnName, L"," );
    FdoInt32 ix;
    
    for ( ix = 0; ix < columnPositions->GetCount(); ix++ ) {
        FdoInt32 columnPosition = FdoStringP(columnPositions->GetString(ix)).ToLong();
        
        FdoSmPhColumnP column = Position2Column(columnPosition);
 
        if ( column == NULL ) {
            // Primary Key column must be in this table.
            if ( GetElementState() != FdoSchemaElementState_Deleted )
	            AddPkeyColumnError( columnName );
        }
        else  {
            mPkeyColumns->Add(column);
        }
    }
}

void FdoSmPhPostGisDbObject::LoadIndexColumn( FdoSmPhTableIndexReaderP indexRdr, FdoSmPhIndexP index )
{
    // columnName actually contains list of positions for index columns.
    FdoStringP columnName = indexRdr->GetString(L"",L"column_name");

    // Parse the position list (space-separated elements).
    FdoStringsP columnPositions = FdoStringCollection::Create( columnName, L" " );
    FdoInt32 ix;
    
    for ( ix = 0; ix < columnPositions->GetCount(); ix++ ) {
        FdoInt32 columnPosition = FdoStringP(columnPositions->GetString(ix)).ToLong();
        FdoSmPhColumnP column = Position2Column(columnPosition);

        if ( column ) {
            // Add the column to the current index.
            index->AddColumn( column );
        }
        else {
            // Index column must be in this table.
            if ( GetElementState() != FdoSchemaElementState_Deleted )
                AddIndexColumnError( index->GetName() );
        }
    }
}

