/*
 * 
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
#include "ShpSpatialContextReader.h"


ShpSpatialContextReader::ShpSpatialContextReader (ShpSpatialContextCollection*  spatialContextColl) :
	mSpatialContextColl (spatialContextColl),
	mCurrIndex (0)
{
	FDO_SAFE_ADDREF(mSpatialContextColl.p);
}

ShpSpatialContextReader::~ShpSpatialContextReader (void)
{
}

FdoString* ShpSpatialContextReader::GetName ()
{
    validate ();
    return (mContext->GetName ());
}

FdoString* ShpSpatialContextReader::GetDescription ()
{
    validate ();
    return (mContext->GetDescription ());
}

FdoString* ShpSpatialContextReader::GetCoordinateSystem ()
{
    validate ();
    return (mContext->GetCoordinateSystem ());
}


FdoString* ShpSpatialContextReader::GetCoordinateSystemWkt ()
{
    validate ();
    return (mContext->GetCoordinateSystemWkt ());
}

FdoSpatialContextExtentType ShpSpatialContextReader::GetExtentType ()
{
    validate ();
    return (mContext->GetExtentType ());
}

FdoByteArray* ShpSpatialContextReader::GetExtent ()
{
    validate ();
    return (mContext->GetExtent ());
}

const double ShpSpatialContextReader::GetXYTolerance ()
{
    validate ();
    return (mContext->GetXYTolerance ());
}

const double ShpSpatialContextReader::GetZTolerance ()
{
    validate ();
    return (mContext->GetZTolerance ());
}

const bool ShpSpatialContextReader::IsActive ()
{
    validate ();
    return (true); // only one context that's always active
}

bool ShpSpatialContextReader::ReadNext ()
{
	bool	rc = false;
	if ( mCurrIndex < mSpatialContextColl->GetCount())
	{
		mContext = mSpatialContextColl->GetItem( mCurrIndex++ );
		rc = true;
	}
    return ( rc );
}

void ShpSpatialContextReader::Dispose ()
{
    delete this;
}

void ShpSpatialContextReader::validate ()
{
    if (mContext.p == NULL)
        throw FdoException::Create (NlsMsgGet(SHP_READER_NOT_READY, "'%1$ls' reader not ready.", L"FdoISpatialContextReader"));
}
