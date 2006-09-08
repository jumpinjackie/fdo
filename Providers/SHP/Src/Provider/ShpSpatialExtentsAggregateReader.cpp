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
#include "ShpConnection.h"
#include "ShpSpatialExtentsAggregateReader.h"


ShpSpatialExtentsAggregateReader::ShpSpatialExtentsAggregateReader(ShpConnection* conn, FdoFeatureClass* originalClass, FdoString* aliasName) :
    m_ReaderIndex(-1),
    m_AliasName(aliasName)
{
    // Get the ShapeFile for this class:
    FdoPtr<ShpLpClassDefinition> lpClass = ShpSchemaUtilities::GetLpClassDefinition (conn, originalClass->GetName());
    ShpFileSet* fileset = lpClass->GetPhysicalFileSet ();
    ShapeFile* shpFile = fileset->GetShapeFile();

    if ((shpFile->GetBoundingBoxMinX()==fNO_DATA)
        || (shpFile->GetBoundingBoxMinY()==fNO_DATA)
        || (shpFile->GetBoundingBoxMaxX()==fNO_DATA)
        || (shpFile->GetBoundingBoxMaxY()==fNO_DATA))
        m_Extents = NULL;
    else
    {
        // Build an extents geometry out of the spatial extents;
        // We ignore M dimensionality since FdoIEnvelope only exposes XYZ envelopes
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoGeometricPropertyDefinition> geomProp = originalClass->GetGeometryProperty();
        FdoBoolean hasElevation = geomProp->GetHasElevation();
        FdoDimensionality geomDim = (FdoDimensionality)
            (hasElevation ? FdoDimensionality_XY | FdoDimensionality_Z : FdoDimensionality_XY);

        // Copy the extent values to an array of doubles
        FdoInt32 i=0;
        FdoDouble ordinates[15];
	    ordinates[i++] = shpFile->GetBoundingBoxMinX(); 
	    ordinates[i++] = shpFile->GetBoundingBoxMinY(); 
        if (hasElevation)
	        ordinates[i++] = shpFile->GetBoundingBoxMinZ(); 

	    ordinates[i++] = shpFile->GetBoundingBoxMaxX(); 
	    ordinates[i++] = shpFile->GetBoundingBoxMinY(); 
        if (hasElevation)
	        ordinates[i++] = shpFile->GetBoundingBoxMaxZ(); 

	    ordinates[i++] = shpFile->GetBoundingBoxMaxX(); 
	    ordinates[i++] = shpFile->GetBoundingBoxMaxY(); 
        if (hasElevation)
	        ordinates[i++] = shpFile->GetBoundingBoxMaxZ(); 

	    ordinates[i++] = shpFile->GetBoundingBoxMinX(); 
	    ordinates[i++] = shpFile->GetBoundingBoxMaxY(); 
        if (hasElevation)
	        ordinates[i++] = shpFile->GetBoundingBoxMinZ(); 

	    ordinates[i++] = shpFile->GetBoundingBoxMinX(); 
	    ordinates[i++] = shpFile->GetBoundingBoxMinY(); 
        if (hasElevation)
	        ordinates[i++] = shpFile->GetBoundingBoxMinZ(); 

        // Create a linear ring using the bounding box ordinates 
	    FdoPtr<FdoILinearRing> linearRing = gf->CreateLinearRing(geomDim, i, ordinates);

        // Create a polygon geometry representing the extents from the linear ring
	    m_Extents = gf->CreatePolygon(linearRing, NULL);
    }
}

ShpSpatialExtentsAggregateReader::~ShpSpatialExtentsAggregateReader()
{
}

void ShpSpatialExtentsAggregateReader::Dispose()
{
    delete this;
}

bool ShpSpatialExtentsAggregateReader::ReadNext()
{
    m_ReaderIndex++;
    if (m_ReaderIndex==0)
        PopulatePropertyValueCache();
    return (m_ReaderIndex==0);
}

void ShpSpatialExtentsAggregateReader::Close()
{
}

FdoInt32 ShpSpatialExtentsAggregateReader::GetItemCount(void)
{
    return 1;  // always one property in result set
}

FdoString* ShpSpatialExtentsAggregateReader::GetItemName(FdoInt32 i)
{
    if (i!=0)
        throw FdoException::NLSGetMessage(FDO_NLSID(FDO_5_INDEXOUTOFBOUNDS));
    return (FdoString*)m_AliasName;
}

FdoInt32 ShpSpatialExtentsAggregateReader::GetItemIndex(FdoString* itemName)  // throw exception if name is invalid
{
    if (0!=wcscmp(itemName, (FdoString*)m_AliasName))
        throw FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER));
    return 0;
}

FdoPropertyType ShpSpatialExtentsAggregateReader::GetItemType(FdoInt32 i)
{
    if (i!=0)
        throw FdoException::NLSGetMessage(FDO_NLSID(FDO_5_INDEXOUTOFBOUNDS));
    return FdoPropertyType_GeometricProperty;
}

FdoDataType ShpSpatialExtentsAggregateReader::GetItemDataType(FdoInt32 i)
{
    // We only support geometry values (spatialextents to be precise), so any call to this function will fail
    throw FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER));
}

// Retrieve data values; these are guaranteed to only be called once per row of data:
void ShpSpatialExtentsAggregateReader::GetGeometryForCache(FdoIdentifier *itemName, FdoByteArray **byteArray, bool *bIsNull)
{
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoByteArray* geomFGF = !m_Extents ? NULL : gf->GetFgf(m_Extents);

    if (bIsNull)
        *bIsNull = !m_Extents;
    if (byteArray)
        *byteArray = geomFGF;
}

// Retrieve information about the row of items:
bool ShpSpatialExtentsAggregateReader::ReaderHasData(void)
{
    return (m_ReaderIndex==0);  // return false IFF before first row or after last row
}
