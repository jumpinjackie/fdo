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
#include "Column.h"
#include "DbObject.h"

FdoSmPhPostGisColumn::FdoSmPhPostGisColumn()
{
    // idle
}

FdoSmPhPostGisColumn::FdoSmPhPostGisColumn(FdoSmPhRdColumnReader* reader)
{
}

FdoSmPhPostGisColumn::~FdoSmPhPostGisColumn()
{
    // idle
}


FdoSmPhColumnP FdoSmPhPostGisColumn::GetBaseColumn()
{
    if ( !mBaseColumn ) {
        FdoSmPhDbObjectP parent = FDO_SAFE_ADDREF( (FdoSmPhDbObject*)(FdoSmPhDbElement*)(GetParent()) );

        mBaseColumn = GetBaseColumn( parent );
    }

    return mBaseColumn;
}

FdoSmPhColumnP FdoSmPhPostGisColumn::GetBaseColumn( FdoSmPhDbObjectP dbObject )
{
    FdoInt32 idx;
    FdoSmPhBaseObjectsP baseObjects = dbObject->GetBaseObjects();
    FdoSmPhColumnP baseColumn;

    for ( idx = 0; idx < baseObjects->GetCount(); idx++ ) {
        FdoSmPhDbObjectP baseObject = FdoSmPhBaseObjectP(baseObjects->GetItem(idx))->GetDbObject();

        if ( baseObject ) {
            baseColumn = baseObject->GetColumns()->FindItem( GetName() );

            if ( baseColumn ) 
                break;

            baseColumn = GetBaseColumn( baseObject );

            if ( baseColumn ) 
                break;
        }
    }

    return baseColumn;
}

void FdoSmPhPostGisColumn::SetElementState(FdoSchemaElementState elementState)
{
    GetBaseColumn();
    
    FdoSmPhColumn::SetElementState( elementState );
}
