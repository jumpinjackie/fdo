 //
 // Copyright (C) 2004-2006  Autodesk, Inc.
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


#include "stdafx.h"

#include "FdoRdbmsConnection.h"
#include "FdoRdbmsException.h"
#include "FdoRdbmsCreateSpatialContext.h"

FdoRdbmsCreateSpatialContext::FdoRdbmsCreateSpatialContext(FdoIConnection *connection)
: FdoRdbmsCommand<FdoICreateSpatialContext>(connection)
{
    mExtentType = FdoSpatialContextExtentType_Static;
    mExtent = NULL;
    mXYTolerance = 0;
    mZTolerance = 0;
    mUpdateExisting = false;
}

FdoRdbmsCreateSpatialContext::~FdoRdbmsCreateSpatialContext()
{
}

FdoString* FdoRdbmsCreateSpatialContext::GetName()
{
    return mSCName;
}

void FdoRdbmsCreateSpatialContext::SetName(FdoString* value)
{
    mSCName = value;
}

FdoString* FdoRdbmsCreateSpatialContext::GetDescription()
{
    return mDescription;
}

void FdoRdbmsCreateSpatialContext::SetDescription(FdoString* value)
{
    mDescription = value;
}

FdoString* FdoRdbmsCreateSpatialContext::GetCoordinateSystem()
{
    return mCoordSysName;
}

void FdoRdbmsCreateSpatialContext::SetCoordinateSystem(FdoString* value)
{
    mCoordSysName = value;
}

FdoString* FdoRdbmsCreateSpatialContext::GetCoordinateSystemWkt()
{
    return mCoordSysWkt;
}

void FdoRdbmsCreateSpatialContext::SetCoordinateSystemWkt(FdoString* value)
{
    mCoordSysWkt = value;
}

FdoSpatialContextExtentType FdoRdbmsCreateSpatialContext::GetExtentType()
{
    return mExtentType;
}

void FdoRdbmsCreateSpatialContext::SetExtentType(FdoSpatialContextExtentType value)
{
    mExtentType = value;
}

FdoByteArray* FdoRdbmsCreateSpatialContext::GetExtent()
{
    return FDO_SAFE_ADDREF( mExtent.p );
}

void FdoRdbmsCreateSpatialContext::SetExtent(FdoByteArray* value)
{
    mExtent = FDO_SAFE_ADDREF( value );
}

const double FdoRdbmsCreateSpatialContext::GetXYTolerance()
{
    return mXYTolerance;
}

void FdoRdbmsCreateSpatialContext::SetXYTolerance(double value)
{
    mXYTolerance = value;
}

const double FdoRdbmsCreateSpatialContext::GetZTolerance()
{
    return mZTolerance;
}

void FdoRdbmsCreateSpatialContext::SetZTolerance(const double value)
{
    mZTolerance = value;
}

const bool FdoRdbmsCreateSpatialContext::GetUpdateExisting()
{
    return mUpdateExisting;
}

void FdoRdbmsCreateSpatialContext::SetUpdateExisting(const bool value)
{
    mUpdateExisting = value;
}

void FdoRdbmsCreateSpatialContext::Execute()
{
    FdoSchemaManagerP smgr = mFdoConnection->GetSchemaManager();

    smgr->CreateSpatialContext(
        this->mSCName, this->mDescription,
        this->mCoordSysName, this->mCoordSysWkt,
        this->mExtentType, this->mExtent,
        this->mXYTolerance, this->mZTolerance,
		mUpdateExisting);
}
