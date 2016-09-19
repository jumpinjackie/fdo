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

#include "FdoRdbmsException.h"
#include "FdoRdbmsSpatialContextReader.h"
#include <Inc/Geometry/Fgf/Factory.h>

FdoRdbmsSpatialContextReader::FdoRdbmsSpatialContextReader(
    FdoRdbmsConnection * connection,
    bool act_sc_only)
:
    mFdoConnection(connection),
    mScIndex(-1),
    mAreMoreSC(true),
    mActiveOnly(act_sc_only),
    mFirstRead(true),
    mIsActiveSC(false)
{
}

FdoRdbmsSpatialContextReader::~FdoRdbmsSpatialContextReader()
{
}

FdoString* FdoRdbmsSpatialContextReader::GetName()
{
    return mSc->GetName();
}

FdoString* FdoRdbmsSpatialContextReader::GetDescription()
{
    return mSc->GetDescription();
}

FdoString* FdoRdbmsSpatialContextReader::GetCoordinateSystem()
{
    return mSc->GetCoordinateSystem();
}

FdoString* FdoRdbmsSpatialContextReader::GetCoordinateSystemWkt()
{
    return mSc->GetCoordinateSystemWkt();
}

FdoSpatialContextExtentType FdoRdbmsSpatialContextReader::GetExtentType()
{
    return mSc->GetExtentType();
}

FdoByteArray* FdoRdbmsSpatialContextReader::GetExtent()
{
    return mSc->GetExtent();
}

const double FdoRdbmsSpatialContextReader::GetXYTolerance()
{
    return mSc->GetXYTolerance();
}

const double FdoRdbmsSpatialContextReader::GetZTolerance()
{
    return  mSc->GetZTolerance();
}

const bool FdoRdbmsSpatialContextReader::IsActive()
{
    return  mIsActiveSC;
}

bool FdoRdbmsSpatialContextReader::ReadNext()
{
    try
    {
        FdoSchemaManagerP smgr = mFdoConnection->GetSchemaManager();
        FdoSmLpSpatialContextsP spatialContexts = smgr->GetLpSpatialContexts();
        FdoInt32 numSpatialContexts = spatialContexts->GetCount();

        if ( mActiveOnly )
        {
            // Look for exactly one spatial context.
            FdoString * activeScName = mFdoConnection->GetActiveSpatialContextName();
            mSc = spatialContexts->GetItem(activeScName);

            // Set members so that next ReadNext will signal end of data.
            mActiveOnly = false;
            mScIndex = numSpatialContexts;
        }
        else
        {
            mScIndex++;
            mAreMoreSC = ( mScIndex < numSpatialContexts );

            if (mAreMoreSC)
                mSc = spatialContexts->GetItem(mScIndex);
        }
    }
    catch (FdoRdbmsException *ex)
    {
       throw ex;
    }
    return mAreMoreSC;
}

void FdoRdbmsSpatialContextReader::Dispose()
{
    delete this;
}

