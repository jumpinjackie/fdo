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
#include "SdfConnection.h"
#include "SdfSpatialExtentsAggregateReader.h"


SdfSpatialExtentsAggregateReader::SdfSpatialExtentsAggregateReader(SdfConnection* conn, FdoFeatureClass* originalClass, FdoString* aliasName) :
    m_ReaderIndex(-1),
    m_AliasName(aliasName)
{
    // Get the spatial extents of the given class:
    SdfRTree* rtree = conn->GetRTree(originalClass);
    Bounds bounds = rtree->GetBounds();

    if (Bounds::IsUndefined(bounds))
        m_Extents = NULL;
    else
    {
        // Get the Geometry property from the class used to construct the query
        FdoPtr<FdoGeometricPropertyDefinition> geomProp = originalClass->GetGeometryProperty();
        
        // Get the dimensionality of the geometry
        FdoDimensionality geomDim = 
            (FdoDimensionality)(geomProp->GetHasElevation() ? FdoDimensionality_XY | FdoDimensionality_Z : FdoDimensionality_XY);

        // Copy the extent values to an array of doubles
        FdoInt32 i=0;
        double ordinates[15];
	    ordinates[i++] = bounds.minx; // WestBoundLongitude
	    ordinates[i++] = bounds.miny; // SouthBoundLatitude
        if (geomProp->GetHasElevation())
            ordinates[i++] = 0.0;  // no way to read elevation at the moment

	    ordinates[i++] = bounds.maxx; // EastBoundLongitude
	    ordinates[i++] = bounds.miny; // SouthBoundLatitude
        if (geomProp->GetHasElevation())
            ordinates[i++] = 0.0;  // no way to read elevation at the moment

	    ordinates[i++] = bounds.maxx; // EastBoundLongitude
	    ordinates[i++] = bounds.maxy; // NorthBoundLatitude
        if (geomProp->GetHasElevation())
            ordinates[i++] = 0.0;  // no way to read elevation at the moment

	    ordinates[i++] = bounds.minx; // WestBoundLongitude
	    ordinates[i++] = bounds.maxy; // NorthBoundLatitude
        if (geomProp->GetHasElevation())
            ordinates[i++] = 0.0;  // no way to read elevation at the moment

	    ordinates[i++] = bounds.minx; // WestBoundLongitude
	    ordinates[i++] = bounds.miny; // SouthBoundLatitude
        if (geomProp->GetHasElevation())
            ordinates[i++] = 0.0;  // no way to read elevation at the moment

        // Create a linear ring using the bounding box ordinates 
	    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
	    FdoPtr<FdoILinearRing> linearRing = gf->CreateLinearRing(geomDim, i, ordinates);

        // Create a polygon geometry representing the extents from the linear ring
	    m_Extents = gf->CreatePolygon(linearRing, NULL);
    }
}

SdfSpatialExtentsAggregateReader::~SdfSpatialExtentsAggregateReader()
{
}

void SdfSpatialExtentsAggregateReader::Dispose()
{
    delete this;
}

bool SdfSpatialExtentsAggregateReader::ReadNext()
{
    m_ReaderIndex++;
    if (m_ReaderIndex==0)
        PopulatePropertyValueCache();
    return (m_ReaderIndex==0);
}

void SdfSpatialExtentsAggregateReader::Close()
{
}

FdoInt32 SdfSpatialExtentsAggregateReader::GetItemCount(void)
{
    return 1;  // always one property in result set
}

FdoString* SdfSpatialExtentsAggregateReader::GetItemName(FdoInt32 i)
{
    if (i!=0)
        throw FdoException::NLSGetMessage(FDO_NLSID(FDO_5_INDEXOUTOFBOUNDS));
    return (FdoString*)m_AliasName;
}

FdoInt32 SdfSpatialExtentsAggregateReader::GetItemIndex(FdoString* itemName)  // throw exception if name is invalid
{
    if (0!=wcscmp(itemName, (FdoString*)m_AliasName))
        throw FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER));
    return 0;
}

FdoPropertyType SdfSpatialExtentsAggregateReader::GetItemType(FdoInt32 i)
{
    if (i!=0)
        throw FdoException::NLSGetMessage(FDO_NLSID(FDO_5_INDEXOUTOFBOUNDS));
    return FdoPropertyType_GeometricProperty;
}

FdoDataType SdfSpatialExtentsAggregateReader::GetItemDataType(FdoInt32 i)
{
    // We only support geometry values (spatialextents to be precise), so any call to this function will fail
    throw FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER));
}

// Retrieve data values; these are guaranteed to only be called once per row of data:
void SdfSpatialExtentsAggregateReader::GetGeometryForCache(FdoIdentifier *itemName, FdoByteArray **byteArray, bool *bIsNull)
{
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoByteArray* geomFGF = !m_Extents ? NULL : gf->GetFgf(m_Extents);

    if (bIsNull)
        *bIsNull = !m_Extents;
    if (byteArray)
        *byteArray = geomFGF;
}

// Retrieve information about the row of items:
bool SdfSpatialExtentsAggregateReader::ReaderHasData(void)
{
    return (m_ReaderIndex==0);  // return false IFF before first row or after last row
}
