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
#include <Sm/Ph/SpatialContextReader.h>
#include <Sm/Ph/SpatialContextWriter.h>
#include <Sm/Ph/Mt/SpatialContextReader.h>
#include <Sm/Ph/Rd/SpatialContextReader.h>

FdoSmPhSpatialContextReader::FdoSmPhSpatialContextReader(FdoSmPhMgrP physicalSchema) : 
	FdoSmPhReader( MakeReader(physicalSchema) )
{
}

FdoSmPhSpatialContextReader::~FdoSmPhSpatialContextReader(void)
{
}

FdoInt64 FdoSmPhSpatialContextReader::GetId()
{
	return( GetLong(L"", L"scid") );
}

FdoStringP FdoSmPhSpatialContextReader::GetName()
{
	return(GetString(L"", L"scname"));
}

FdoStringP FdoSmPhSpatialContextReader::GetDescription()
{
	return(GetString(L"", L"description"));
}

FdoInt64 FdoSmPhSpatialContextReader::GetGroupId()
{
	return(GetLong(L"", L"scgid"));
}

FdoSmPhReaderP FdoSmPhSpatialContextReader::MakeReader( FdoSmPhMgrP mgr )
{
    FdoSmPhReaderP pSubReader;

    // Create the row collection describing spatial context attributes.
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();

    FdoSmPhRowP scRow = FdoSmPhSpatialContextWriter::MakeRow(mgr);
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
        if ( mgr->GetOwner()->GetHasMetaSchema() &&
             scRow->GetDbObject()->GetExists() ) {
            // F_SPATIALCONTEXT exists; read from MetaSchema
            pSubReader = MakeMtReader( rows, mgr );
        }
        else {
            // F_SPATIALCONTEXT does not exist; read from native physical schema.
            pSubReader = MakeRdReader( mgr );
        }
    // }

    return pSubReader;
}

FdoSmPhReaderP FdoSmPhSpatialContextReader::MakeMtReader( FdoSmPhRowsP rows, FdoSmPhMgrP mgr )
{
    return new FdoSmPhMtSpatialContextReader( rows, mgr );
}

FdoSmPhReaderP FdoSmPhSpatialContextReader::MakeRdReader( FdoSmPhMgrP mgr )
{
	// Caller should check for NULL.
    return (FdoSmPhReader*) NULL; //mgr->CreateRdSpatialContextReader();
}

