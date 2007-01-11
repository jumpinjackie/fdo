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
#include "ShpOptimizedAggregateReader.h"


ShpOptimizedAggregateReader::ShpOptimizedAggregateReader(ShpConnection* conn, FdoFeatureClass* originalClass, aggr_list *selAggrList) :
    m_ReaderIndex(-1),
    m_SelAggrList(selAggrList),
	m_Count(0)
{
    // Get the ShapeFile for this class:
    FdoPtr<ShpLpClassDefinition> lpClass = ShpSchemaUtilities::GetLpClassDefinition (conn, originalClass->GetName());
    ShpFileSet* fileset = lpClass->GetPhysicalFileSet ();
    ShapeFile* shpFile = fileset->GetShapeFile();

	for ( size_t i = 0; i < m_SelAggrList->size() && (m_Extents == NULL); i++ )
	{ 
		AggregateElement	*id = m_SelAggrList->at(i);
		if ( id->type != FdoPropertyType_GeometricProperty )
			continue;

		if ((shpFile->GetBoundingBoxMinX()==fNO_DATA)
			|| (shpFile->GetBoundingBoxMinY()==fNO_DATA)
			|| (shpFile->GetBoundingBoxMaxX()==fNO_DATA)
			|| (shpFile->GetBoundingBoxMaxY()==fNO_DATA))
			m_Extents = NULL;
		else
		{
			// Use a tolerance to compensate the bounding box storage as floats while the ordinates are doubles. 
			FdoPtr<FdoGeometricPropertyDefinition> geomProp = originalClass->GetGeometryProperty();
			double	xyRes = conn->GetToleranceXY( geomProp )/2.0;

			// Build an extents geometry out of the spatial extents;
			// We ignore M dimensionality since FdoIEnvelope only exposes XYZ envelopes
			FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
			FdoBoolean hasElevation = geomProp->GetHasElevation();
			FdoDimensionality geomDim = (FdoDimensionality)
				(hasElevation ? FdoDimensionality_XY | FdoDimensionality_Z : FdoDimensionality_XY);

			// Copy the extent values to an array of doubles
			FdoInt32 i=0;
			FdoDouble ordinates[15];
			ordinates[i++] = shpFile->GetBoundingBoxMinX() - xyRes; 
			ordinates[i++] = shpFile->GetBoundingBoxMinY() - xyRes; 
			if (hasElevation)
				ordinates[i++] = shpFile->GetBoundingBoxMinZ(); 

			ordinates[i++] = shpFile->GetBoundingBoxMaxX() + xyRes; 
			ordinates[i++] = shpFile->GetBoundingBoxMinY() - xyRes; 
			if (hasElevation)
				ordinates[i++] = shpFile->GetBoundingBoxMaxZ(); 

			ordinates[i++] = shpFile->GetBoundingBoxMaxX() + xyRes; 
			ordinates[i++] = shpFile->GetBoundingBoxMaxY() + xyRes; 
			if (hasElevation)
				ordinates[i++] = shpFile->GetBoundingBoxMaxZ(); 

			ordinates[i++] = shpFile->GetBoundingBoxMinX() - xyRes; 
			ordinates[i++] = shpFile->GetBoundingBoxMaxY() + xyRes; 
			if (hasElevation)
				ordinates[i++] = shpFile->GetBoundingBoxMinZ(); 

			ordinates[i++] = shpFile->GetBoundingBoxMinX() - xyRes; 
			ordinates[i++] = shpFile->GetBoundingBoxMinY() - xyRes; 
			if (hasElevation)
				ordinates[i++] = shpFile->GetBoundingBoxMinZ(); 

			// Create a linear ring using the bounding box ordinates 
			FdoPtr<FdoILinearRing> linearRing = gf->CreateLinearRing(geomDim, i, ordinates);

			// Create a polygon geometry representing the extents from the linear ring
			m_Extents = gf->CreatePolygon(linearRing, NULL);
		}
	}

	// Do Count()
	for ( size_t i = 0; i < m_SelAggrList->size() && (m_Count == 0); i++ )
	{
		AggregateElement	*id = m_SelAggrList->at(i);
		if ( id->type != FdoPropertyType_DataProperty )
			continue;

		m_Count = fileset->GetShapeIndexFile ()->GetNumObjects ();
 	}
}

ShpOptimizedAggregateReader::~ShpOptimizedAggregateReader()
{
	// Clean up
	for ( size_t i = 0; i < m_SelAggrList->size(); i++ )
		delete m_SelAggrList->at(i);
	delete m_SelAggrList;
}

void ShpOptimizedAggregateReader::Dispose()
{
    delete this;
}

bool ShpOptimizedAggregateReader::ReadNext()
{
    m_ReaderIndex++;
    if (m_ReaderIndex==0)
        PopulatePropertyValueCache();

    return (m_ReaderIndex==0);
}

void ShpOptimizedAggregateReader::Close()
{
}

FdoInt32 ShpOptimizedAggregateReader::GetItemCount(void)
{
    return (FdoInt32)m_SelAggrList->size();  // always one property in result set
}

FdoString* ShpOptimizedAggregateReader::GetItemName(FdoInt32 i)
{
    if ( i > (FdoInt32)m_SelAggrList->size() )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                        L"ShpOptimizedAggregateReader::GetItemName",
														L"index"));
	return m_SelAggrList->at(i)->name; 
}

FdoInt32 ShpOptimizedAggregateReader::GetItemIndex(FdoString* itemName)  // throw exception if name is invalid
{
	bool		found = false;
	FdoInt32	index;

	for ( FdoInt32 i = 0; i < (FdoInt32)m_SelAggrList->size() && !found; i++ )
	{
		found = ( wcscmp(m_SelAggrList->at(i)->name, itemName ) == 0 );
		index = i;
	}

	if ( !found )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                        L"ShpOptimizedAggregateReader::GetItemIndex",
														L"itemName"));
	return index;
}

FdoPropertyType ShpOptimizedAggregateReader::GetItemType(FdoInt32 i)
{
	return m_SelAggrList->at(i)->type;
}

FdoDataType ShpOptimizedAggregateReader::GetItemDataType(FdoInt32 i)
{
    if ( i > (FdoInt32)m_SelAggrList->size() )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                        L"ShpOptimizedAggregateReader::GetItemDataType",
														L"index"));

	// So any call to this function will fail if not for Count()
	if ( m_SelAggrList->at(i)->type != FdoPropertyType_DataProperty )
		throw FdoException::NLSGetMessage(FDO_NLSID(SHP_UNSUPPORTED_FUNCTION), 
														L"ShpOptimizedAggregateReader::GetItemDataType");

	return	FdoDataType_Int64;
}

// Retrieve data values; these are guaranteed to only be called once per row of data:
void ShpOptimizedAggregateReader::GetGeometryForCache(FdoIdentifier *itemName, FdoByteArray **byteArray, bool *bIsNull)
{
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoByteArray* geomFGF = !m_Extents ? NULL : gf->GetFgf(m_Extents);

    if (bIsNull)
        *bIsNull = !m_Extents;
    if (byteArray)
        *byteArray = geomFGF;
}

void ShpOptimizedAggregateReader::GetInt64ForCache( FdoIdentifier *itemName, FdoInt64 *int64Value, bool *bIsNull )
{
    if (bIsNull)
        *bIsNull = false;
    if (int64Value)
        *int64Value = m_Count;
}

// Retrieve information about the row of items:
bool ShpOptimizedAggregateReader::ReaderHasData(void)
{
    return (m_ReaderIndex==0);  // return false IFF before first row or after last row
}

