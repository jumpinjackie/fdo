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
#include <Sm/Ph/Rd/SpatialContextGeomReader.h>
#include <Sm/Ph/Table.h>

FdoSmPhRdSpatialContextGeomReader::FdoSmPhRdSpatialContextGeomReader() 
{
}

FdoSmPhRdSpatialContextGeomReader::FdoSmPhRdSpatialContextGeomReader( FdoSmPhMgrP mgr ) 
{
    // If this class is overridden, one can use "mgr" to access any
    // spatial metaschema information in the datastore.
}

FdoSmPhRdSpatialContextGeomReader::~FdoSmPhRdSpatialContextGeomReader(void)
{
}

FdoStringP FdoSmPhRdSpatialContextGeomReader::GetGeomTableName()
{
	return L"";
}

FdoStringP FdoSmPhRdSpatialContextGeomReader::GetGeomColumnName()
{
	return L"";
}

FdoInt32 FdoSmPhRdSpatialContextGeomReader::GetDimensionality()
{
	return -1;
}

FdoString* FdoSmPhRdSpatialContextGeomReader::GetCoordinateSystem()
{
	return L"";
}

FdoString* FdoSmPhRdSpatialContextGeomReader::GetCoordinateSystemWkt()
{
	return L"";
}

FdoInt64 FdoSmPhRdSpatialContextGeomReader::GetSrid()
{
	return 0;
}

FdoByteArray* FdoSmPhRdSpatialContextGeomReader::GetExtent()
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIEnvelope>          env = gf->CreateEnvelopeXY( -2000000, -2000000, 2000000, 2000000 );
    FdoPtr<FdoIGeometry> geom = gf->CreateGeometry(env); 

    return  (gf->GetFgf(geom));
}

const double FdoSmPhRdSpatialContextGeomReader::GetXYTolerance()
{
	return 0.001;
}

const double FdoSmPhRdSpatialContextGeomReader::GetZTolerance()
{
	return 0.001;
}

bool FdoSmPhRdSpatialContextGeomReader::ReadNext()
{
    // This is the default implementation, with only default data.
    // Arrange for a single row (already initialized to the defaults)
    // to be read.

    if ( IsBOF() )
    {
        SetBOF(false);
    }
    else
    {
        SetEOF(true);
    }

    return(!IsEOF());
}

