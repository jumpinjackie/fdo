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
#include <Sm/Ph/SpatialContextGroupReader.h>
#include <Sm/Ph/SpatialContextGroupWriter.h>
#include <Sm/Ph/Mt/SpatialContextGroupReader.h>
#include <Sm/Ph/Rd/SpatialContextGroupReader.h>

FdoSmPhSpatialContextGroupReader::FdoSmPhSpatialContextGroupReader(FdoSmPhMgrP physicalSchema) : 
	FdoSmPhReader( MakeReader(physicalSchema) )
{
}

FdoSmPhSpatialContextGroupReader::~FdoSmPhSpatialContextGroupReader(void)
{
}

FdoInt64 FdoSmPhSpatialContextGroupReader::GetId()
{
	return( GetLong(L"", L"scgid") );
}

FdoStringP FdoSmPhSpatialContextGroupReader::GetCrsName()
{
	return(GetString(L"", L"crsname"));
}

FdoStringP FdoSmPhSpatialContextGroupReader::GetCrsWkt()
{
	return(GetString(L"", L"crswkt"));
}

FdoInt64 FdoSmPhSpatialContextGroupReader::GetSrid()
{
	return( GetLong(L"", L"srid") );
}

double FdoSmPhSpatialContextGroupReader::GetXTolerance()
{
    return( GetDouble(L"", L"xtolerance") );
}

double FdoSmPhSpatialContextGroupReader::GetZTolerance()
{
    return( GetDouble(L"", L"ztolerance") );
}

double FdoSmPhSpatialContextGroupReader::GetXMin()
{
    return( GetDouble(L"", L"xmin") );
}

double FdoSmPhSpatialContextGroupReader::GetYMin()
{
    return( GetDouble(L"", L"ymin") );
}

double FdoSmPhSpatialContextGroupReader::GetZMin()
{
    return( GetDouble(L"", L"zmin") );
}

double FdoSmPhSpatialContextGroupReader::GetXMax()
{
    return( GetDouble(L"", L"xmax") );
}

double FdoSmPhSpatialContextGroupReader::GetYMax()
{
    return( GetDouble(L"", L"ymax") );
}

double FdoSmPhSpatialContextGroupReader::GetZMax()
{
    return( GetDouble(L"", L"zmax") );
}

FdoStringP FdoSmPhSpatialContextGroupReader::GetExtentType()
{
	return(GetString(L"", L"extenttype"));
}

FdoSmPhReaderP FdoSmPhSpatialContextGroupReader::MakeReader( FdoSmPhMgrP mgr )
{
    FdoSmPhReaderP pSubReader;

    // Create the row collection describing class attributes.
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();

    FdoSmPhRowP scgRow = mgr->GetSpatialContextGroupWriter()->MakeRow(mgr);
    rows->Add( scgRow );

    // TODO: add handling for spatial contexts in configuration files.

    // FdoSchemaMappingsP mappings = mgr->GetConfigMappings();

    // if ( mappings )
    // {
        //     // Config document is set so read spatial context groups from it.
        // ...
    // }
    // else
    // {
        if ( FdoSmPhOwnerP(mgr->GetOwner())->GetHasMetaSchema() &&
             FdoSmPhDbObjectP(scgRow->GetDbObject())->GetExists() ) {
            // F_SPATIALCONTEXTGROUP exists, read from MetaSchema
            pSubReader = MakeMtReader( rows, mgr );
        }
        else {
            // F_SPATIALCONTEXTGROUP does not exist, read from native physical schema.
            pSubReader = MakeRdReader( rows, mgr );
        }
    // }

    return pSubReader;
}

FdoSmPhReaderP FdoSmPhSpatialContextGroupReader::MakeMtReader( FdoSmPhRowsP rows, FdoSmPhMgrP mgr )
{
    return new FdoSmPhMtSpatialContextGroupReader( rows, mgr );
}

FdoSmPhReaderP FdoSmPhSpatialContextGroupReader::MakeRdReader( FdoSmPhRowsP rows, FdoSmPhMgrP mgr )
{
	// The caller should check for NULL 
    return (FdoSmPhReader *) NULL; //new FdoSmPhRdSpatialContextGroupReader( rows, mgr );
}
