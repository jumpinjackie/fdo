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


SdfSpatialExtentsAggregateReader::SdfSpatialExtentsAggregateReader(SdfConnection* conn, FdoFeatureClass* originalClass, FdoString* extentIdName, FdoString* countName) :
    m_ReaderIndex(-1),
    m_ExtentAliasName(extentIdName),
    m_CountAliasName(countName),
    m_Count(0)
{
    // Get the spatial extents of the given class:
    SdfRTree* rtree = conn->GetRTree(originalClass);
    Bounds bounds = rtree->GetBounds();

    if ( ((const wchar_t*)m_ExtentAliasName)[0] == '\0' || Bounds::IsUndefined(bounds))
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

    // Find the aproximate count value which is the key of the last record in the tabe.
    // This is an aproximation that can be equal or less than the actual count. Deleted 
    // record will not be taken into account.
    if ( ((const wchar_t*)m_CountAliasName)[0] != '\0' )
    {
        DataDb* datadb = conn->GetDataDb(originalClass);
        SQLiteData key;
        SQLiteData data;
        if( datadb->GetLastFeature( &key, &data ) == SQLiteDB_OK )
        {
            m_Count = *(REC_NO*)(key.get_data());
        }
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
    if( ((const wchar_t*)m_CountAliasName)[0] != '\0' )
        return 2;

    return 1;  // At least the extent is always in result set
}

FdoString* SdfSpatialExtentsAggregateReader::GetItemName(FdoInt32 i)
{
    if ( i == 0 )
        return (FdoString*)m_ExtentAliasName;

    if( i == 1 && ((const wchar_t*)m_CountAliasName)[0] != '\0' )
        return (FdoString*)m_CountAliasName;
    
    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_37_INVALID_PROPERTY_INDEX)));
}

FdoInt32 SdfSpatialExtentsAggregateReader::GetItemIndex(FdoString* itemName)  // throw exception if name is invalid
{
    if (0 == wcscmp(itemName, (FdoString*)m_ExtentAliasName) )
        return 0;
    else if ( 0 == wcscmp(itemName, (FdoString*)m_CountAliasName) )
        return 1;
    else
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                               L"SdfSpatialExtentsAggregateReader::GetItemIndex",
                                                              L"itemName"));
}

FdoPropertyType SdfSpatialExtentsAggregateReader::GetItemType(FdoInt32 i)
{
    if ( i==0 )
        return FdoPropertyType_GeometricProperty;
    else if ( i==1 )
        return FdoPropertyType_DataProperty;

    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_37_INVALID_PROPERTY_INDEX)));
}

FdoDataType SdfSpatialExtentsAggregateReader::GetItemDataType(FdoInt32 i)
{
    if ( i==1 ) // for count only
        return FdoDataType_Int64;

    throw FdoException::NLSGetMessage(FDO_NLSID(SDFPROVIDER_95_UNSUPPORTED_FUNCTION),
                                      L"SdfSpatialExtentsAggregateReader::GetItemDataType");
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

void SdfSpatialExtentsAggregateReader::GetInt64ForCache( FdoIdentifier *itemName, FdoInt64 *int64Value, bool *bIsNull )
{
    if (bIsNull)
        *bIsNull = false;
    if (int64Value)
        *int64Value = (FdoInt64)m_Count;
}

// Retrieve information about the row of items:
bool SdfSpatialExtentsAggregateReader::ReaderHasData(void)
{
    return (m_ReaderIndex==0);  // return false IFF before first row or after last row
}
