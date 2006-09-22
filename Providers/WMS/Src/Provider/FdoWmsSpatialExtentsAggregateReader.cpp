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
#include "FdoWmsGlobals.h"
#include "FdoWmsConnection.h"
#include "FdoWmsSpatialExtentsAggregateReader.h"
#include "FdoWmsServiceMetadata.h"
#include <WMS/Override/FdoWmsOvOverrides.h>

FdoWmsSpatialExtentsAggregateReader::FdoWmsSpatialExtentsAggregateReader(FdoWmsConnection* conn, FdoIdentifier* classIdentifier, FdoString* aliasName) :
    m_ReaderIndex(-1),
    m_AliasName(aliasName)
{
    FdoWmsServiceMetadataP serviceMetadata = conn->GetWmsServiceMetadata ();
    FdoOwsCapabilitiesP capabilities = serviceMetadata->GetCapabilities ();
    FdoWmsCapabilities* wmsCapabilities = static_cast<FdoWmsCapabilities*>(capabilities.p);
    FdoWmsLayerCollectionP layers = wmsCapabilities->GetLayers();
    FdoStringP className = classIdentifier->GetName();
    FdoStringP schemaName = classIdentifier->GetSchemaName();
    FdoWmsBoundingBoxP bbox = FdoWmsBoundingBox::Create();;
    FdoStringP crsName;

	if (!conn->IsConfigured ())
	{
		FdoDictionaryP namedLayerMappings = conn->GetNamedLayerMappings();
		FdoDictionaryElementP layerElement = namedLayerMappings->FindItem(className);
        FdoWmsLayerP layer = conn->FindLayer(layers, layerElement->GetName());
        FdoPtr<FdoStringCollection> crsNames = layer->GetCoordinateReferenceSystems();
        if (crsNames->GetCount() == 0) {
            crsName = FdoWmsGlobals::DefaultEPSGCode;
        }
        else {
            FdoStringElementP crsElement = crsNames->GetItem(0);
            crsName = crsElement->GetString();
        }
 
        _calcLayerBoundingBox (layer, crsName, bbox);
	}
	else
	{
        bool bFound = false;
        FdoWmsLayerCollectionP classLayers = FdoWmsLayerCollection::Create();
		FdoSchemaMappingsP schemaMappings = conn->GetSchemaMappings ();
		for (FdoInt32 i=0; i<schemaMappings->GetCount(); i++)
		{
			FdoPtr<FdoPhysicalSchemaMapping> schemaMapping = schemaMappings->GetItem (i);
			if (schemaName.GetLength () > 0) {
                if (wcscmp (schemaMapping->GetName (), schemaName) != 0) {
					continue;
                }
			}

			FdoWmsOvPhysicalSchemaMapping* physicalMapping = static_cast<FdoWmsOvPhysicalSchemaMapping *> (schemaMapping.p);
			FdoWmsOvClassesP classMappings = physicalMapping->GetClasses ();
			FdoWmsOvClassDefinitionP clsDefinition = classMappings->FindItem (className);
            if (clsDefinition) 
            {
			    bFound = true;

                FdoWmsOvRasterDefinitionP rasterDefinition = clsDefinition->GetRasterDefinition ();
    			FdoStringP crsName = rasterDefinition->GetSpatialContextName ();
                if (crsName.GetLength() == 0) {
                    crsName = FdoWmsGlobals::DefaultEPSGCode;
                }

                FdoWmsOvLayersP layerDefinitions = rasterDefinition->GetLayers();
		        for (FdoInt32 j=0; j<layerDefinitions->GetCount(); j++)
		        {
                    FdoWmsOvLayerDefinitionP layerDefinition = layerDefinitions->GetItem(j);
                    FdoStringP layerDefName = layerDefinition->GetName();
                    FdoWmsLayerP layer = conn->FindLayer(layers, layerDefinition->GetName());
                    classLayers->Add(layer);
                }

                _calcLayersBoundingBox (classLayers, crsName, bbox);
                break;
            }
		}

        if (!bFound) 
        {
            throw FdoCommandException::Create (
                NlsMsgGet(FDO_NLSID(FDOWMS_NAMED_SCHEMACLASS_NOT_FOUND), (FdoString*)className));
        }
	}

    // Copy the extent values to an array of doubles
    double ordinates[10];
    ordinates[0] = bbox->GetMinX ();
    ordinates[1] = bbox->GetMinY ();
    ordinates[2] = bbox->GetMaxX ();
    ordinates[3] = bbox->GetMinY ();
    ordinates[4] = bbox->GetMaxX ();
    ordinates[5] = bbox->GetMaxY ();
    ordinates[6] = bbox->GetMinX ();
    ordinates[7] = bbox->GetMaxY ();
    ordinates[8] = bbox->GetMinX ();
    ordinates[9] = bbox->GetMinY ();

    // Create a linear ring using the bounding box ordinates 
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoILinearRing> linearRing = gf->CreateLinearRing(FdoDimensionality_XY, 10, ordinates);

    // Create a polygon geometry representing the extents from the linear ring
    m_Extents = gf->CreatePolygon(linearRing, NULL);
}

FdoWmsSpatialExtentsAggregateReader::~FdoWmsSpatialExtentsAggregateReader()
{
}

void FdoWmsSpatialExtentsAggregateReader::Dispose()
{
    delete this;
}

bool FdoWmsSpatialExtentsAggregateReader::ReadNext()
{
    m_ReaderIndex++;

    if (m_ReaderIndex==0) {
        PopulatePropertyValueCache();
    }

    return (m_ReaderIndex==0);
}

void FdoWmsSpatialExtentsAggregateReader::Close()
{
}

FdoInt32 FdoWmsSpatialExtentsAggregateReader::GetItemCount(void)
{
    return 1;  // always one property in result set
}

FdoString* FdoWmsSpatialExtentsAggregateReader::GetItemName(FdoInt32 i)
{
    if (i!=0) {
        throw FdoException::NLSGetMessage(FDO_NLSID(FDO_5_INDEXOUTOFBOUNDS));
    }

    return (FdoString*)m_AliasName;
}

FdoInt32 FdoWmsSpatialExtentsAggregateReader::GetItemIndex(FdoString* itemName)  
{
    if (0!=wcscmp(itemName, (FdoString*)m_AliasName)) {
        throw FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)); 
    }

    return 0;
}

// We only support geometry values (spatialextents to be precise)
FdoPropertyType FdoWmsSpatialExtentsAggregateReader::GetItemType(FdoInt32 i)
{
    if (i!=0) {
        throw FdoException::NLSGetMessage(FDO_NLSID(FDO_5_INDEXOUTOFBOUNDS));
    }

    return FdoPropertyType_GeometricProperty;
}

// We only support geometry values (spatialextents to be precise), so any call to this function will fail
FdoDataType FdoWmsSpatialExtentsAggregateReader::GetItemDataType(FdoInt32 i)
{
    throw FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER));
}

// Retrieve data values; these are guaranteed to only be called once per row of data:
void FdoWmsSpatialExtentsAggregateReader::GetGeometryForCache(FdoIdentifier *itemName, FdoByteArray **byteArray, bool *bIsNull)
{
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoByteArray* geomFGF = !m_Extents ? NULL : gf->GetFgf(m_Extents);

    if (bIsNull) {
        *bIsNull = !m_Extents;
    }

    if (byteArray) {
        *byteArray = geomFGF;
    }
}

// Retrieve information about the row of items:
bool FdoWmsSpatialExtentsAggregateReader::ReaderHasData(void)
{
    // return false IF before first row or after last row
    return (m_ReaderIndex==0);  
}
