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
#include "SpatialIndex.h"
#include "Mgr.h"
#include "Table.h"
#include "ColumnGeom.h"
#include <FdoCommonStringUtil.h>

#define NUMBER_STRING_SIZE  (100)


FdoSmPhPostGisSpatialIndex::FdoSmPhPostGisSpatialIndex(
        FdoStringP name,
        FdoSmPhDbObject* pParent,
        FdoSchemaElementState elementState
) :
    FdoSmPhSpatialIndex( name, pParent, false, elementState ),
    FdoSmPhPostGisDbObject( name, (const FdoSmPhOwner*) pParent->GetParent(), NULL ),
    FdoSmPhDbObject( name, (const FdoSmPhOwner*) pParent->GetParent(), elementState ),
    mIsRtree(true),
    mIs3d(false)
{
}

FdoSmPhPostGisSpatialIndex::~FdoSmPhPostGisSpatialIndex(void)
{
}

FdoStringP FdoSmPhPostGisSpatialIndex::GetDDLName() const
{
    FdoStringP name = GetName();
    if ( name.Contains(L".") ) 
        name = name.Right(L".");

    return FdoStringP(L"\"") + name + L"\"";
}

FdoPtr<FdoSmPhRdColumnReader> FdoSmPhPostGisSpatialIndex::CreateColumnReader()
{
    return FdoSmPhIndex::CreateColumnReader();
}

bool FdoSmPhPostGisSpatialIndex::GetIsRtree() const
{
    return mIsRtree;
}

void FdoSmPhPostGisSpatialIndex::SetIsRtree( bool isRtree )
{
    if ( GetElementState() != FdoSchemaElementState_Added ) 
        throw FdoSchemaException::Create( 
			NlsMsgGet1(
				FDORDBMS_132,
				"Cannot modify index type for spatial index %1$ls, modification not supported.", 
                (FdoString*) GetQName()
            )
		);

    mIsRtree = isRtree;
}

bool FdoSmPhPostGisSpatialIndex::GetIs3d() const
{
    return mIs3d;
}

void FdoSmPhPostGisSpatialIndex::SetIs3d( bool is3d )
{
  //  if ( GetElementState() != FdoSchemaElementState_Added ) 
  //      throw FdoSchemaException::Create( 
		//	NlsMsgGet1(
		//		FDORDBMS_133,
		//		"Cannot modify dimensionality for spatial index %1$ls, modification not supported.", 
  //              (FdoString*) GetQName()
  //          )
		//);

   // mIs3d = is3d;
}

FdoSchemaExceptionP FdoSmPhPostGisSpatialIndex::Errors2Exception(FdoSchemaException* pFirstException ) const
{
    const FdoSmPhColumnCollection* columns = RefColumns();

	// Tack on errors for this element
	FdoSchemaExceptionP pException = FdoSmPhIndex::Errors2Exception(pFirstException);

    // A Spatial index must have exactly one column
    if ( columns->GetCount() != 1) {
        pException = FdoSchemaException::Create(
			NlsMsgGet1(
				FDORDBMS_134,
				"Spatial Index '%1$ls' must have a single column (currently has 0 or multiple columns).", 
                (FdoString*) GetQName()
            ),
            pException
        );
    }

    // The spatial index column must be a geometric column.
    const FdoSmPhPostGisColumnGeom* column = 
        dynamic_cast<const FdoSmPhPostGisColumnGeom*>(columns->RefItem(0));

    if ( !column ) {
        pException = FdoSchemaException::Create(
			NlsMsgGet2(
				FDORDBMS_135,
				"Column '%1$ls' for Spatial Index '%2$ls' is not geometric.",
                (FdoString*)(columns->RefItem(0)->GetQName()),
                (FdoString*) GetQName()
            ),
            pException
        );
    }

    return pException;
}

bool FdoSmPhPostGisSpatialIndex::Add()
{

    FdoSmPhDbObjectP dbObject = GetDbObject();
    FdoSmPhColumnsP columns = GetColumns();

    if ( columns->GetCount() == 1 ) {

        FdoSmPhPostGisColumnGeomP column = 
            columns->GetItem(0)->SmartCast<FdoSmPhPostGisColumnGeom>(true);

        if ( column ) {
            // Skip silently if not exactly one column that is geometric.
            // Errors2Exception already caught these errors
            FdoStringP sqlStmt;
            FdoStringP tableName = dbObject->GetDDLName();

            FdoStringP dbQName = GetDbQName();
            FdoStringP tableDbQName = dbObject->GetDDLQName();

            FdoStringP  indexName = GetName();
            if ( indexName.Contains(L".") ) 
                indexName = indexName.Right(L"."); // no schema

            sqlStmt = FdoStringP::Format(
                L"CREATE INDEX \"%ls\"  ON %ls USING GIST (\"%ls\" GIST_GEOMETRY_OPS)",
                (FdoString*) indexName,
                (FdoString*) tableDbQName, // no dbo. prefix etc.
                column->GetName()
            );

            dbObject->ExecuteDDL( sqlStmt, NULL, false );
        }
    }

    return true;
}

bool FdoSmPhPostGisSpatialIndex::Delete()
{
    FdoSmPhDbObjectP dbObject = GetDbObject();

    // Index name must be qualified by table name.
    FdoStringP sqlStmt = FdoStringP::Format(
        L"drop index %ls on %ls",
        (FdoString*) GetDDLName(),
        (FdoString*) dbObject->GetDbName()
    );

    // SqlServer does not allow qualified name for index to create.
    // Therefore, must switch to the index's owning database before deleting
    // it.
    ActivateOwnerAndExecute( sqlStmt );


    return( true );
}
