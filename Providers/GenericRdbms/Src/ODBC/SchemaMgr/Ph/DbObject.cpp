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
#include "DbObject.h"
#include "Owner.h"
#include "Rd/ColumnReader.h"
#include "Rd/DbObjectReader.h"
#include "Sm/Ph/Rd/DbObjectReader.h"
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

FdoSmPhOdbcDbObject::FdoSmPhOdbcDbObject(
    FdoStringP name,
    const FdoSmPhOwner* pOwner,
    FdoSmPhRdDbObjectReader* reader
) :
    FdoSmPhDbObject(name, pOwner )
{
}

FdoSmPhOdbcDbObject::~FdoSmPhOdbcDbObject(void)
{
}

FdoStringP FdoSmPhOdbcDbObject::GetDbQName() const
{
    FdoStringP dbQName = GetDbName();

    if ( ((FdoSmPhOdbcDbObject*) this)->GetManager()->GetDefaultOwnerName().ICompare(GetParent()->GetName()) != 0 ) 
        dbQName = ((const FdoSmPhDbElement*) GetParent())->GetDbName() + L"." + dbQName;

    return dbQName;
}

void FdoSmPhOdbcDbObject::ActivateOwnerAndExecute( FdoStringP sqlStmt )
{
    FdoSmPhOdbcOwner*        objOwner       = static_cast<FdoSmPhOdbcOwner*>((FdoSmPhSchemaElement*) GetParent());

    objOwner->ActivateAndExecute( sqlStmt );
}


FdoSmPhColumnP FdoSmPhOdbcDbObject::NewColumnBLOB(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhOdbcColumnBLOB( columnName, elementState, this, bNullable, rootColumnName, colRdr );
}

FdoSmPhColumnP FdoSmPhOdbcDbObject::NewColumnChar(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    int length,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhOdbcColumnChar(columnName, elementState, this, bNullable, length, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhOdbcDbObject::NewColumnDate(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhOdbcColumnDate(columnName, elementState, this, bNullable, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhOdbcDbObject::NewColumnDecimal(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    int length,
    int scale,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhOdbcColumnDecimal(columnName, elementState, this, bNullable, length, scale, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhOdbcDbObject::NewColumnSingle(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhOdbcColumnSingle(columnName, elementState, this, bNullable, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhOdbcDbObject::NewColumnDouble(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhOdbcColumnDouble(columnName, elementState, this, bNullable, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhOdbcDbObject::NewColumnGeom(
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
    return new FdoSmPhOdbcColumnGeom(columnName, elementState, this, AssociatedSCInfo, bNullable, bHasElevation, bHasMeasure, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhOdbcDbObject::NewColumnBool(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhOdbcColumnBool(columnName, elementState, this, bNullable, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhOdbcDbObject::NewColumnByte(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhOdbcColumnByte(columnName, elementState, this, bNullable, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhOdbcDbObject::NewColumnInt16(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    bool bIsAutoincremented,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhOdbcColumnInt16(columnName, elementState, this, bNullable, bIsAutoincremented, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhOdbcDbObject::NewColumnInt32(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    bool bIsAutoincremented,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhOdbcColumnInt32(columnName, elementState, this, bNullable, bIsAutoincremented, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhOdbcDbObject::NewColumnInt64(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    bool bIsAutoincremented,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhOdbcColumnInt64(columnName, elementState, this, bNullable, bIsAutoincremented, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhOdbcDbObject::NewColumnUnknown(
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
    return new FdoSmPhOdbcColumnUnknown(columnName, typeName, elementState, this, bNullable, length, scale, rootColumnName, colRdr);
}

FdoSmPhColumnP FdoSmPhOdbcDbObject::NewColumnDbObject(
    FdoStringP columnName,
    FdoSchemaElementState elementState,
    bool bNullable,
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* colRdr
)
{
    return new FdoSmPhOdbcColumnChar(
        columnName, 
        elementState, 
        this, 
        bNullable, 
        (int) GetManager()->DbObjectNameMaxLen(), 
        rootColumnName,
        colRdr
    );
}

FdoPtr<FdoSmPhRdColumnReader> FdoSmPhOdbcDbObject::CreateColumnReader()
{
    return new FdoSmPhRdOdbcColumnReader( GetManager(), FDO_SAFE_ADDREF(this) );
}
