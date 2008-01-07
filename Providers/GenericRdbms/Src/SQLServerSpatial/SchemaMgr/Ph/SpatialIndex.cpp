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


FdoSmPhSqsSpatialIndex::FdoSmPhSqsSpatialIndex(
        FdoStringP name,
        const FdoSmPhTable* pTable,
        FdoSchemaElementState elementState
) :
    FdoSmPhSpatialIndex( name, pTable, false, elementState ),
    FdoSmPhSqsDbObject( name, (const FdoSmPhOwner*) pTable->GetParent(), NULL ),
    FdoSmPhDbObject( name, (const FdoSmPhOwner*) pTable->GetParent(), elementState ),
    mIsRtree(true),
    mIs3d(false)
{
}

FdoSmPhSqsSpatialIndex::~FdoSmPhSqsSpatialIndex(void)
{
}

FdoStringP FdoSmPhSqsSpatialIndex::GetDDLName() const
{
    const FdoSmPhSqsTable* table = static_cast<const FdoSmPhSqsTable*>(RefTable());
    FdoStringP name = GetName();
    if ( name.Contains(L".") ) 
        name = name.Right(L".");

    return FdoStringP(L"\"") + name + L"\"";
}

FdoPtr<FdoSmPhRdColumnReader> FdoSmPhSqsSpatialIndex::CreateColumnReader()
{
    return FdoSmPhIndex::CreateColumnReader();
}

bool FdoSmPhSqsSpatialIndex::GetIsRtree() const
{
    return mIsRtree;
}

void FdoSmPhSqsSpatialIndex::SetIsRtree( bool isRtree )
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

bool FdoSmPhSqsSpatialIndex::GetIs3d() const
{
    return mIs3d;
}

void FdoSmPhSqsSpatialIndex::SetIs3d( bool is3d )
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

FdoSchemaExceptionP FdoSmPhSqsSpatialIndex::Errors2Exception(FdoSchemaException* pFirstException ) const
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
    const FdoSmPhSqsColumnGeom* column = 
        dynamic_cast<const FdoSmPhSqsColumnGeom*>(columns->RefItem(0));

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

bool FdoSmPhSqsSpatialIndex::Add()
{
    FdoSmPhSqsMgrP mgr = GetManager()->SmartCast<FdoSmPhSqsMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();

    FdoSmPhSqsTable* table = static_cast<FdoSmPhSqsTable*>((FdoSmPhTable*)(RefTable()));
    FdoSmPhOwner* owner = (FdoSmPhOwner*)(dynamic_cast<const FdoSmPhOwner*>(GetParent()));
    FdoSmPhColumnsP columns = GetColumns();

    if ( columns->GetCount() == 1 ) {

        FdoSmPhSqsColumnGeomP column = 
            columns->GetItem(0)->SmartCast<FdoSmPhSqsColumnGeom>(true);

        if ( column ) {
            // Skip silently if not exactly one column that is geometric.
            // Errors2Exception already caught these errors
            FdoStringP sqlStmt;
            FdoStringP tableName = table->GetDDLName();

            FdoStringP dbQName = GetDbQName();
            FdoStringP tableDbQName = table->GetDDLQName();

              // Use same defaults as the Default spatial context.
            FdoInt64   srid = 0L;
            double minx = -2000000;
            double miny = -2000000;
            double maxx = 2000000;
            double maxy = 2000000;

            //Get the root table name
            tableName = tableName.Right(L".");

            FdoSmPhScInfoP scInfo = column->GetSpatialContextInfo();
            if (scInfo != NULL) {

                // If we are given spatial context information, it must be valid.
                FdoPtr<FdoByteArray> ba = scInfo->mExtent;
         
                if ( ba == NULL)
                    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDORDBMS_487),
                                                                           L"FdoSmPhSqsColumnGeom::Add",
                                                                           L"srid/xytolerance/ztolerance/ba"));

                FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
                FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromFgf(ba);
                FdoPtr<FdoIEnvelope> env = geom->GetEnvelope();
                minx = env->GetMinX();
                miny = env->GetMinY();
                maxx = env->GetMaxX();
                maxy = env->GetMaxY();
            }

            wchar_t minxStr[NUMBER_STRING_SIZE];
            wchar_t minyStr[NUMBER_STRING_SIZE];
            wchar_t maxxStr[NUMBER_STRING_SIZE];
            wchar_t maxyStr[NUMBER_STRING_SIZE];

            FdoCommonStringUtil::FormatDouble(minx, minxStr, NUMBER_STRING_SIZE-1);
            FdoCommonStringUtil::FormatDouble(miny, minyStr, NUMBER_STRING_SIZE-1);
            FdoCommonStringUtil::FormatDouble(maxx, maxxStr, NUMBER_STRING_SIZE-1);
            FdoCommonStringUtil::FormatDouble(maxy, maxyStr, NUMBER_STRING_SIZE-1);

            FdoStringP  indexName = GetName();
            indexName = indexName.Right(L"."); // no dbo.

            FdoStringP  dropExisting = L""; // L", DROP_EXISTING = ON"; - Doens't work on fresh tables.
            FdoStringP  otherOptions = FdoStringP(L" GRIDS = (LEVEL_1 = MEDIUM, LEVEL_2 = MEDIUM, LEVEL_3 = MEDIUM, LEVEL_4 = MEDIUM), CELLS_PER_OBJECT = 16");
            
            if ( column->GetTypeName().ICompare(L"geometry") == 0 ) {
                sqlStmt = FdoStringP::Format(
                    L"CREATE SPATIAL INDEX %ls ON %ls(%ls) USING GEOMETRY_GRID WITH ( BOUNDING_BOX = (%ls, %ls, %ls, %ls), %ls %ls);",
                    (FdoString*) indexName,
                    (FdoString*) tableDbQName, // no dbo. prefix etc.
                    column->GetName(),
                    minxStr, minyStr, maxxStr, maxyStr,
                    (FdoString*) otherOptions,
                    (FdoString*) dropExisting 
                );
            }
            else {
                // Bounds cannot be specified for geography column.
                sqlStmt = FdoStringP::Format(
                    L"CREATE SPATIAL INDEX %ls ON %ls(%ls) USING GEOGRAPHY_GRID WITH ( %ls %ls);",
                    (FdoString*) indexName,
                    (FdoString*) tableDbQName, // no dbo. prefix etc.
                    column->GetName(),
                    (FdoString*) otherOptions,
                    (FdoString*) dropExisting 
                );
            }

            table->ExecuteDDL( sqlStmt, NULL, false );
        }
    }

    return true;
}

bool FdoSmPhSqsSpatialIndex::Delete()
{
    const FdoSmPhTable* table = RefTable();

    // Index name must be qualified by table name.
    FdoStringP sqlStmt = FdoStringP::Format(
        L"drop index %ls on %ls",
        (FdoString*) GetDDLName(),
        (FdoString*) ((FdoSmPhTable*)table)->GetDbName()
    );

    // SqlServer does not allow qualified name for index to create.
    // Therefore, must switch to the index's owning database before deleting
    // it.
    ActivateOwnerAndExecute( sqlStmt );


    return( true );
}
