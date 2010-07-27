/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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
#include "DbObject.h"
#include "Owner.h"
#include "Rd/ColumnReader.h"
#include "Rd/PkeyReader.h"
#include "Rd/IndexReader.h"
#include "Rd/FkeyReader.h"
#include "Rd/BaseObjectReader.h"
#include "Rd/DbObjectReader.h"
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
#include "ColumnBlob.h"
#include "ColumnGeom.h"
#include "ColumnUnknown.h"
#include "SpatialIndex.h"
#include "Index.h"

FdoSmPhSqsDbObject::FdoSmPhSqsDbObject(
    FdoStringP name,
    const FdoSmPhOwner* pOwner,
    FdoSmPhRdDbObjectReader* reader
) :
    FdoSmPhDbObject(name, pOwner ),
    mIdentityIncrement(1),
    mIdentitySeed(0),
    mIdentityIsGloballyUnique(false),
    mTextInRow(SqlServerOvTextInRowOption_Default)
{
    // Read info from Reader that is specific to sql server tables/views/indexes/etc:
    if (reader != NULL)
    {
        mTableFilegroup    = reader->GetString(L"", L"tablefilegroup_name");
        mTextFilegroup     = reader->GetString(L"", L"textfilegroup_name");
        mIndexFilegroup    = L""; // There is no such thing as *the* index filegroup of a table, since each index can be in a separate filegroup.
        mIdentityColumn    = reader->GetString(L"", L"idcolumn_name");
        mIdentitySeed      = reader->GetInteger(L"", L"idcolumn_seed");
        // TODO: there is an issue where FdoSmPhRdDbObjectReader has no "IsNull" method to help us
        //       distinguish NULL values from 0 or empty string.  So column values like idcolumn_increment
        //       will be 0 which is different from the default of 1.
        mIdentityIncrement = reader->GetInteger(L"", L"idcolumn_increment");
        mIdentityIsGloballyUnique = reader->GetBoolean(L"", L"idcolumn_isrowguid");
        mTextInRow = reader->GetInteger(L"", L"textinrow") == 0 ? SqlServerOvTextInRowOption_NotInRow : SqlServerOvTextInRowOption_InRow;
    }
}

FdoSmPhSqsDbObject::~FdoSmPhSqsDbObject(void)
{
}

FdoStringP FdoSmPhSqsDbObject::GetDbName() const
{
    FdoStringP fullName(GetName());
    FdoStringP userName;
    FdoStringP objectName;
    FdoStringP dbName;

    if ( fullName.Contains(L".") ) {
        userName = fullName.Left(L".");
        objectName = fullName.Right(L".");
    }
    else {
        userName = L"dbo";
        objectName = fullName;
    }

    dbName = FdoStringP(L"\"") + userName + L"\".\"" + objectName + L"\"";

    return dbName;
}

FdoStringP FdoSmPhSqsDbObject::GetDbQName() const
{
    FdoStringP dbQName = GetDbName();

    if ( ((FdoSmPhSqsDbObject*) this)->GetManager()->GetDefaultOwnerName().ICompare(GetParent()->GetName()) != 0 ) 
        dbQName = ((const FdoSmPhDbElement*) GetParent())->GetDbName() + L"." + dbQName;

    return dbQName;
}

bool FdoSmPhSqsDbObject::GetSupportsWrite() const
{
    // It has been decided to return "true" as the default value because it has been deemed
    // to expensive to determine whether or not the class is writable. 
    return true;
}

FdoStringP FdoSmPhSqsDbObject::GetBestSchemaName() const
{
    FdoStringP objName( GetName() );
    objName = objName.Contains( L"." ) ? objName.Left(L".") : FdoStringP(L"dbo");

    // Filter out characters not allowed in schema element names.
    return objName.Replace( L":", L"_" );
}

FdoStringP FdoSmPhSqsDbObject::GetBestClassName() const
{
    FdoStringP objName( GetName() );
    objName = objName.Contains( L"." ) ? objName.Right(L".") : objName;

    // Filter out characters not allowed in schema element names.
    return objName.Replace(L":",L"_").Replace(L".",L"_");
}

FdoBoolean FdoSmPhSqsDbObject::GetPolygonVertexOrderStrictness(FdoString* propName) const
{
    const FdoSmPhColumnCollection* columns = RefColumns();
    const FdoSmPhColumn* column = columns->RefItem(propName);
    if ( NULL != column && FdoSmPhColType_Geom == column->GetType())
    {
        // SQL Server 2008 Spatial has two data types that support geometry. 
        // They are Geometry and Geography. Both of these support polygons 
        // but the Geography type has a constraint that the vertex order 
        // around loops must be counterclockwise for outer loops and clockwise 
        // for inner loops. Polygons that fail this test will be rejected. 
        FdoStringP typeName = column->GetTypeName();
        if (typeName == L"geography")
            return true;
    }
    return false;
}

void FdoSmPhSqsDbObject::ActivateOwnerAndExecute( FdoStringP sqlStmt )
{
    FdoSmPhSqsOwner*        objOwner       = static_cast<FdoSmPhSqsOwner*>((FdoSmPhSchemaElement*) GetParent());

    objOwner->ActivateAndExecute( sqlStmt );
}


FdoSmPhColumnP FdoSmPhSqsDbObject::NewColumnBLOB(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhSqsColumnBLOB( columnName, elementState, this, bNullable, rootColumnName, colRdr );
}

FdoSmPhColumnP FdoSmPhSqsDbObject::NewColumnChar(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    int length,
    FdoStringP rootColumnName,
	FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhSqsColumnChar(columnName, elementState, this, bNullable, length, rootColumnName, defaultValue, colRdr);
}

FdoSmPhColumnP FdoSmPhSqsDbObject::NewColumnDate(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    FdoStringP rootColumnName,
	FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhSqsColumnDate(columnName, elementState, this, bNullable, rootColumnName, defaultValue, colRdr);
}

FdoSmPhColumnP FdoSmPhSqsDbObject::NewColumnDecimal(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    int length,
    int scale,
    FdoStringP rootColumnName,
	FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhSqsColumnDecimal(columnName, elementState, this, bNullable, length, scale, rootColumnName, defaultValue, colRdr);
}

FdoSmPhColumnP FdoSmPhSqsDbObject::NewColumnSingle(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    FdoStringP rootColumnName,
	FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhSqsColumnSingle(columnName, elementState, this, bNullable, rootColumnName, defaultValue, colRdr);
}

FdoSmPhColumnP FdoSmPhSqsDbObject::NewColumnDouble(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    FdoStringP rootColumnName,
	FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhSqsColumnDouble(columnName, elementState, this, bNullable, rootColumnName, defaultValue, colRdr);
}

FdoSmPhColumnP FdoSmPhSqsDbObject::NewColumnGeom(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    FdoSmPhScInfoP AssociatedSCInfo,
    bool bNullable,
    bool bHasElevation,
    bool bHasMeasure,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhSqsColumnGeom(columnName, elementState, this, AssociatedSCInfo, bNullable, bHasElevation, bHasMeasure, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhSqsDbObject::NewColumnBool(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    FdoStringP rootColumnName,
	FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhSqsColumnBool(columnName, elementState, this, bNullable, rootColumnName, defaultValue, colRdr);
}

FdoSmPhColumnP FdoSmPhSqsDbObject::NewColumnByte(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    FdoStringP rootColumnName,
	FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhSqsColumnByte(columnName, elementState, this, bNullable, rootColumnName, defaultValue, colRdr);
}

FdoSmPhColumnP FdoSmPhSqsDbObject::NewColumnInt16(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    bool bIsAutoincremented,
    FdoStringP rootColumnName,
	FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhSqsColumnInt16(columnName, elementState, this, bNullable, bIsAutoincremented, rootColumnName, defaultValue, colRdr);
}

FdoSmPhColumnP FdoSmPhSqsDbObject::NewColumnInt32(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    bool bIsAutoincremented,
    FdoStringP rootColumnName,
	FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhSqsColumnInt32(columnName, elementState, this, bNullable, bIsAutoincremented, rootColumnName, defaultValue, colRdr);
}

FdoSmPhColumnP FdoSmPhSqsDbObject::NewColumnInt64(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    bool bIsAutoincremented,
    FdoStringP rootColumnName,
	FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhSqsColumnInt64(columnName, elementState, this, bNullable, bIsAutoincremented, rootColumnName, defaultValue, colRdr);
}

FdoSmPhColumnP FdoSmPhSqsDbObject::NewColumnUnknown(
    FdoStringP columnName,
    FdoStringP typeName,
    FdoSchemaElementState elementState,
    bool bNullable,
    int length,
    int scale,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhSqsColumnUnknown(columnName, typeName, elementState, this, bNullable, length, scale, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhSqsDbObject::NewColumnDbObject(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhSqsColumnChar(
        columnName, 
        elementState, 
        this, 
        bNullable, 
        (int) GetManager()->DbObjectNameMaxLen(), 
        rootColumnName,
		(FdoDataValue*) NULL,
        colRdr
    );
}

FdoSmPhIndexP FdoSmPhSqsDbObject::NewIndex(
    FdoStringP name,
    bool isUnique,
    FdoSchemaElementState elementState
)
{
    return new FdoSmPhSqsIndex( name, this, isUnique, elementState );
}

FdoSmPhIndexP FdoSmPhSqsDbObject::NewSpatialIndex(
    FdoStringP name,
    bool isUnique,
    FdoSchemaElementState elementState
)
{
    return new FdoSmPhSqsSpatialIndex( name, this, elementState );
}


FdoPtr<FdoSmPhRdColumnReader> FdoSmPhSqsDbObject::CreateColumnReader()
{
    return new FdoSmPhRdSqsColumnReader( GetManager(), FDO_SAFE_ADDREF(this) );
}

FdoPtr<FdoSmPhRdBaseObjectReader> FdoSmPhSqsDbObject::CreateBaseObjectReader() const
{
    FdoSmPhSqsDbObject* pDbObject = (FdoSmPhSqsDbObject*) this;

    return new FdoSmPhRdSqsBaseObjectReader( FDO_SAFE_ADDREF(pDbObject) );
}

FdoPtr<FdoSmPhRdPkeyReader> FdoSmPhSqsDbObject::CreatePkeyReader() const
{
    FdoSmPhSqsDbObject* pDbObject = (FdoSmPhSqsDbObject*) this;

    return new FdoSmPhRdSqsPkeyReader( FDO_SAFE_ADDREF(pDbObject) );
}

FdoPtr<FdoSmPhRdIndexReader> FdoSmPhSqsDbObject::CreateIndexReader() const
{
    FdoSmPhSqsDbObject* pDbObject = (FdoSmPhSqsDbObject*) this;

    return new FdoSmPhRdSqsIndexReader( pDbObject->GetManager(), FDO_SAFE_ADDREF(pDbObject) );
}

FdoPtr<FdoSmPhRdFkeyReader> FdoSmPhSqsDbObject::CreateFkeyReader() const
{
    FdoSmPhSqsDbObject* pDbObject = (FdoSmPhSqsDbObject*) this;

    return new FdoSmPhRdSqsFkeyReader( pDbObject->GetManager(), FDO_SAFE_ADDREF(pDbObject) );
}

