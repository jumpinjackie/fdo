// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include "stdafx.h"
#include "SdfConnectionCapabilities.h"

//-------------------------------------------------------
// Constructor / destructor
//-------------------------------------------------------


// default constructor
SdfConnectionCapabilities::SdfConnectionCapabilities()
{
}


// default destructor
SdfConnectionCapabilities::~SdfConnectionCapabilities()
{
}


//-------------------------------------------------------
// FdoIDisposable implementation
//-------------------------------------------------------


// dispose this object
void SdfConnectionCapabilities::Dispose()
{
    delete this;
}


//-------------------------------------------------------
// FdoIConnectionCapabilities implementation
//-------------------------------------------------------


// Gets a ThreadCapability value that declares the feature providers
// level of thread safety.
FdoThreadCapability SdfConnectionCapabilities::GetThreadCapability()
{
    return FdoThreadCapability_PerCommandThreaded;
}


// Gets the list of spatial context extent types supported by the feature
// provider.  The length parameter gives the number of types in the list.
FdoSpatialContextExtentType* SdfConnectionCapabilities::GetSpatialContextTypes(FdoInt32& length)
{
    //we know the extents of features from the R-Tree, se we do not
    //have to dynamically compute them, but we could if we had to
    //so return both.
    //TODO: is this right?
    static FdoSpatialContextExtentType spatialTypes[] =
    {
//      FdoSpatialContextExtentType_Static,
        FdoSpatialContextExtentType_Dynamic
    };

    length = sizeof(spatialTypes) / sizeof(FdoSpatialContextExtentType);
    return spatialTypes;
}


// Returns true if the feature provider supports persistent locking.
bool SdfConnectionCapabilities::SupportsLocking()
{
    return false;
}

// Returns a list of supported lock types; a null list for SDF
FdoLockType* SdfConnectionCapabilities::GetLockTypes(FdoInt32& size)
{
    size = 0;
    return NULL;
}

// Returns true if the feature provider supports connection timeout.
bool SdfConnectionCapabilities::SupportsTimeout()
{
    return false;
}


// Returns true if the feature provider supports transactions.
bool SdfConnectionCapabilities::SupportsTransactions()
{
    return false;
}


// Returns true if the feature provider supports long transactions.
bool SdfConnectionCapabilities::SupportsLongTransactions()
{
    return false;
}


// Returns true if the feature provider supports SQL commands.
bool SdfConnectionCapabilities::SupportsSQL()
{
    return false;
}

bool SdfConnectionCapabilities::SupportsConfiguration()
{
    return false;
}

bool SdfConnectionCapabilities::SupportsMultipleSpatialContexts()
{
    return false;
}

bool SdfConnectionCapabilities::SupportsCSysWKTFromCSysName()
{
    return false;
}

bool SdfConnectionCapabilities::SupportsWrite()
{
    return true;
}

bool SdfConnectionCapabilities::SupportsMultiUserWrite()
{
    return false;
}
