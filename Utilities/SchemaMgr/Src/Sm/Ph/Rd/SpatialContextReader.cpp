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
#include <Sm/Ph/Rd/SpatialContextReader.h>
#include <Sm/Ph/Table.h>

const double FdoSmPhRdSpatialContextReader::mDefaultXMin = -2000000;
const double FdoSmPhRdSpatialContextReader::mDefaultYMin = -2000000;
const double FdoSmPhRdSpatialContextReader::mDefaultXMax = 2000000;
const double FdoSmPhRdSpatialContextReader::mDefaultYMax = 2000000;;


FdoSmPhRdSpatialContextReader::FdoSmPhRdSpatialContextReader()
{
}

FdoSmPhRdSpatialContextReader::FdoSmPhRdSpatialContextReader(FdoSmPhOwnerP owner) :
	FdoSmPhReader(owner->GetManager(), (FdoSmPhRowCollection*) NULL )  
{
    // If this class is overridden, one can use "mgr" to access any
    // spatial metaschema information in the datastore.
}


FdoSmPhRdSpatialContextReader::~FdoSmPhRdSpatialContextReader(void)
{
}

FdoString* FdoSmPhRdSpatialContextReader::GetName()
{
	return L"Default";
}

FdoString* FdoSmPhRdSpatialContextReader::GetDescription()
{
	return L"Default Database Spatial Context";
}

FdoStringP FdoSmPhRdSpatialContextReader::GetGeomTableName()
{
	return L"";
}

FdoStringP FdoSmPhRdSpatialContextReader::GetGeomColumnName()
{
	return L"";
}

bool FdoSmPhRdSpatialContextReader::GetHasElevation()
{
	return false;
}

bool FdoSmPhRdSpatialContextReader::GetHasMeasure()
{
	return false;
}

FdoString* FdoSmPhRdSpatialContextReader::GetCoordinateSystem()
{
	return L"";
}

FdoString* FdoSmPhRdSpatialContextReader::GetCoordinateSystemWkt()
{
	return L"";
}

FdoInt64 FdoSmPhRdSpatialContextReader::GetSrid()
{
	return 0;
}

FdoSpatialContextExtentType FdoSmPhRdSpatialContextReader::GetExtentType()
{
	return FdoSpatialContextExtentType_Static;
}

FdoByteArray* FdoSmPhRdSpatialContextReader::GetExtent()
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIEnvelope>          env = gf->CreateEnvelopeXY( mDefaultXMin, mDefaultYMin, mDefaultXMax, mDefaultYMax );
    FdoPtr<FdoIGeometry> geom = gf->CreateGeometry(env); 

    return  (gf->GetFgf(geom));
}

const double FdoSmPhRdSpatialContextReader::GetXYTolerance()
{
	return 0.001;
}

const double FdoSmPhRdSpatialContextReader::GetZTolerance()
{
	return 0.001;
}

const bool FdoSmPhRdSpatialContextReader::IsActive()
{
	return false;
}

bool FdoSmPhRdSpatialContextReader::ReadNext()
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

