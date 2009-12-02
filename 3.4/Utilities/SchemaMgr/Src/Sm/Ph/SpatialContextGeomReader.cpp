/*
 * Copyright (C) 2006  Autodesk, Inc.
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
#include <Sm/Ph/SpatialContextGeomReader.h>
#include <Sm/Ph/SpatialContextGeomWriter.h>
#include <Sm/Ph/Mt/SpatialContextGeomReader.h>
#include <Sm/Ph/Rd/SpatialContextGeomReader.h>

FdoSmPhSpatialContextGeomReader::FdoSmPhSpatialContextGeomReader(FdoSmPhMgrP physicalSchema) : 
	FdoSmPhReader( MakeReader(physicalSchema) )
{
}

FdoSmPhSpatialContextGeomReader::~FdoSmPhSpatialContextGeomReader(void)
{
}

FdoInt64 FdoSmPhSpatialContextGeomReader::GetScId()
{
	return( GetLong(L"", L"scid") );
}

FdoStringP FdoSmPhSpatialContextGeomReader::GetGeomTableName()
{
	return(GetManager()->GetRealDbObjectName(GetString(L"", L"geomtablename")));
}

FdoStringP FdoSmPhSpatialContextGeomReader::GetGeomColumnName()
{
	return(GetString(L"", L"geomcolumnname"));
}

FdoInt32 FdoSmPhSpatialContextGeomReader::GetDimensionality()
{
	return(GetLong(L"", L"dimensionality"));
}

FdoSmPhReaderP FdoSmPhSpatialContextGeomReader::MakeReader( FdoSmPhMgrP mgr )
{
    FdoSmPhReaderP pSubReader;

    // Create the row collection describing spatial context association attributes.
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();

    FdoSmPhRowP scRow = FdoSmPhSpatialContextGeomWriter::MakeRow(mgr);
    rows->Add( scRow );

    // TODO: add handling for spatial contexts in configuration files.

    // FdoSchemaMappingsP mappings = mgr->GetConfigMappings();
    // 
    // if ( mappings  )
    // {
    //     // Config document is set so read spatial contexts from it.
    //     ...
    // }
    // else
    // {
        if ( FdoSmPhOwnerP(mgr->GetOwner())->GetHasMetaSchema() &&
             FdoSmPhDbObjectP(scRow->GetDbObject())->GetExists() ) {
            // F_SPATIALCONTEXTGEOM exists; read from MetaSchema
            pSubReader = MakeMtReader( rows, mgr );
        }
        else {
            // F_SPATIALCONTEXTGEOM does not exist; read from native physical schema.
            pSubReader = MakeRdReader( mgr );
        }
    // }

    return pSubReader;
}

FdoSmPhReaderP FdoSmPhSpatialContextGeomReader::MakeMtReader( FdoSmPhRowsP rows, FdoSmPhMgrP mgr )
{
    return new FdoSmPhMtSpatialContextGeomReader( rows, mgr );
}

FdoSmPhReaderP FdoSmPhSpatialContextGeomReader::MakeRdReader( FdoSmPhMgrP mgr )
{
    return (FdoSmPhReader*) NULL; //new FdoSmPhRdSpatialContextGeomReader( mgr );
}

