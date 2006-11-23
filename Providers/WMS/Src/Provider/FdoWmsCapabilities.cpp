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

#include <stdafx.h>
#include "FdoWmsCapabilities.h"
#include <FdoWmsRequestMetadata.h>
#include "FdoWmsXmlGlobals.h"
#include "FdoWmsGlobals.h"

FdoWmsCapabilities::FdoWmsCapabilities(void)
{
    mLayers = FdoWmsLayerCollection::Create();
}

FdoWmsCapabilities::~FdoWmsCapabilities(void)
{
}

void FdoWmsCapabilities::Dispose()
{
    delete this;
}

FdoWmsCapabilities* FdoWmsCapabilities::Create()
{
    return new FdoWmsCapabilities();
}

FdoXmlSaxHandler* FdoWmsCapabilities::XmlStartElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname, FdoXmlAttributeCollection* atts)
{
    FdoXmlSaxHandler* pRet = NULL;

    try {
        VALIDATE_ARGUMENT(context);
        VALIDATE_ARGUMENT(name);

		pRet = BaseType::XmlStartElement (context, uri, name, qname, atts);

        if (pRet == NULL) {
            if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesLayer) == 0) {
                FdoWmsLayerP pLayer = FdoWmsLayer::Create();
                pLayer->InitFromXml(context, atts);

                FdoWmsLayerCollectionP pLayers = GetLayers();
                pLayers->Add(pLayer);

                pRet = pLayer;
            }
        }
    }
    catch (FdoException* e) 
    {
        context->AddError(e);
        e->Release();
    }

    return pRet;
}

FdoBoolean FdoWmsCapabilities::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
	return BaseType::XmlEndElement(context, uri, name, qname);
}

FdoWmsLayerCollection* FdoWmsCapabilities::GetLayers() const
{
    return FDO_SAFE_ADDREF(mLayers.p);
}

/// <summary>Gets all supported spatial context names supported by the server.</summary>
/// <returns>Returns a collection of the spatial contexts supported.</returns>
FdoStringCollection* FdoWmsCapabilities::GetCRSNames ()
{	
	_buildUpCRS ();	
	return FDO_SAFE_ADDREF (mCRSNames.p);
}

/// <summary>Gets the extents of the supported spatial contexts. 
/// For each spatial context item returned by 'GetCRSNames()', there's 
/// a corresponding extent item in the returned extents collection.</summary>
/// <returns>Returns a collection of the spatial context extents.</returns>
FdoWmsBoundingBoxCollection* FdoWmsCapabilities::GetCRSExtents ()
{
	_buildUpCRS ();
	return FDO_SAFE_ADDREF (mCRSExtents.p);
}

void FdoWmsCapabilities::_buildUpCRS ()
{
	if (mCRSNames && mCRSExtents)
	{
		// The CRS information has been built up already.
		return;
	}
	else
	{
		mCRSNames = FdoStringCollection::Create ();	
		mCRSExtents = FdoWmsBoundingBoxCollection::Create ();
	}

	// Determine the unique set of CRS names that exist across all layers returned from the WMS.
	FdoPtr<FdoWmsLayerCollection> layers = GetLayers ();
	
	// Resolve FdoWmsCapabiites exposing multiple Root layers. According the WMS specification, only one root layer is allowed.
    // If there are layers retrievable from the WMS Server
    if (layers->GetCount() > 0) 
    {
        FdoPtr<FdoWmsLayer> rootLayer = layers->GetItem (0);
	    _processLayerSRSName (rootLayer, mCRSNames);
    	
	    // For each CRS, iterate over all layer definitions to determine which layers have a bounding box 
	    // that references the CRS. For each Layer/CRS combination calculate the Bounding Box Min/Max values
	    FdoInt32 cntCRSNames = mCRSNames->GetCount ();

	    for (FdoInt32 i=0; i<cntCRSNames; i++)
	    {
		    FdoString* crsName = mCRSNames->GetString (i);
		    FdoPtr<FdoWmsBoundingBox> extent = FdoWmsBoundingBox::Create ();
		    mCRSExtents->Add (extent);
		    bool bFirstMatch = true;
    		
		    _processLayerSRSExtent (rootLayer, crsName, extent, bFirstMatch);
	    }
    }

	// Remove the CRS(es) which are not referenced by any layer.
	_removeNonReferedSRS ();
}

void FdoWmsCapabilities::_getTotalExtent (FdoWmsBoundingBox* desBox, const FdoWmsBoundingBox* srcBox)
{
	if (desBox->GetMinX () > srcBox->GetMinX ())
		desBox->SetMaxX (srcBox->GetMinX ());
	if (desBox->GetMinY () > srcBox->GetMinY ())
		desBox->SetMaxY (srcBox->GetMinY ());
	if (desBox->GetMaxX () < srcBox->GetMaxX ())
		desBox->SetMaxX (srcBox->GetMaxX ());
	if (desBox->GetMaxY () < srcBox->GetMaxY ())
		desBox->SetMaxY (srcBox->GetMaxY ());
}

// helper function to process the SRS information of the WMS layer
void FdoWmsCapabilities::_processLayerSRSName (FdoWmsLayer* layer, FdoStringCollection* names)
{
	FdoStringsP crsNames = layer->GetCoordinateReferenceSystems ();
	FdoInt32 cntNames = crsNames->GetCount ();
	for (FdoInt32 i=0; i<cntNames; i++)
	{
		FdoStringP crsName = crsNames->GetString (i);
		if (names->IndexOf (crsName) == -1)
		{
			names->Add (crsName);
		}
	}

	FdoWmsLayerCollectionP childLayers = layer->GetLayers ();
	FdoInt32 cntChildLayers = childLayers->GetCount ();
	for (FdoInt32 j=0; j<cntChildLayers; j++)
	{
		FdoPtr<FdoWmsLayer> child = childLayers->GetItem (j);
		_processLayerSRSName (child.p, names);
	}
}


void FdoWmsCapabilities::_processLayerSRSExtent (FdoWmsLayer* layer, FdoString* crsName, FdoWmsBoundingBox* extent, bool bFirstMatch)
{
	FdoPtr<FdoWmsBoundingBoxCollection> bboxes = layer->GetBoundingBoxes ();
	FdoInt32 cntBoxes = bboxes->GetCount ();
	for (FdoInt32 i=0; i<cntBoxes; i++)
	{
		FdoPtr<FdoWmsBoundingBox> bbox = bboxes->GetItem (i);
		FdoString* name = bbox->GetCRS ();
		if (wcscmp (crsName, name) == 0)
		{
			if (bFirstMatch)
			{
				extent->SetMinX (bbox->GetMinX ());
				extent->SetMinY (bbox->GetMinY ());
				extent->SetMaxX (bbox->GetMaxX ());
				extent->SetMaxY (bbox->GetMaxY ());

				bFirstMatch = false;
			}
			else
			{
				// get the total extents of all the BoundingBoxes referenced to the CRS
				_getTotalExtent (extent, bbox);
			}
		}
	}

	// process the child layers recursively
	FdoPtr<FdoWmsLayerCollection> childLayers = layer->GetLayers ();
	FdoInt32 cntLayers = childLayers->GetCount ();
	for (FdoInt32 j=0; j<cntLayers; j++)
	{
		FdoPtr<FdoWmsLayer> childLayer = childLayers->GetItem (j);
		_processLayerSRSExtent (childLayer, crsName, extent, bFirstMatch);
	}
}

void FdoWmsCapabilities::_removeNonReferedSRS ()
{
	if (mCRSNames && mCRSExtents)
	{
		FdoInt32 cntCRS = mCRSNames->GetCount ();
		for (FdoInt32 i=cntCRS-1; i>=0; i--)
		{
			FdoPtr<FdoWmsBoundingBox> bbox = mCRSExtents->GetItem (i);
			double areaSize = (bbox->GetMaxX () - bbox->GetMinX ()) * (bbox->GetMaxY () - bbox->GetMinY ());
			if (areaSize == 0)
			{
				// if the CRS is EPSG:4326 and there are <LatLonBoundingBox> tags in the metadata
				// we can make use of these tags to calculate approximate extent.
				FdoStringP crsName = mCRSNames->GetString (i);
				if (crsName == FdoWmsGlobals::DefaultEPSGCode || crsName == FdoWmsGlobals::DefaultEPSGCode2)
				{
					// calculate the extent through <LatLonBoundingBox> tags.
					FdoWmsBoundingBoxP llBox = FdoWmsBoundingBox::Create ();
					_calcAllGeographicBoundingBox (llBox);

					bbox->SetMinX (llBox->GetMinX ());
					bbox->SetMinY (llBox->GetMinY ());
					bbox->SetMaxX (llBox->GetMaxX ());
					bbox->SetMaxY (llBox->GetMaxY ());
					areaSize = (bbox->GetMaxX () - bbox->GetMinX ()) * (bbox->GetMaxY () - bbox->GetMinY ());
				}
				
				if (areaSize == 0)
				{
					mCRSExtents->RemoveAt (i);
					mCRSNames->RemoveAt (i);
				}				
			}
		}
	}
}

void FdoWmsCapabilities::_calcAllGeographicBoundingBox (FdoWmsBoundingBox* bbox)
{
	VALIDATE_ARGUMENT (bbox);
	FdoPtr<FdoWmsLayerCollection> layers = GetLayers ();

	for (FdoInt32 i=0; i < layers->GetCount (); i++)
	{
		FdoPtr<FdoWmsLayer> layer = layers->GetItem (i);
		_calcLayerGeographicBoundingBox (layer, bbox);
	}
}

void FdoWmsCapabilities::_calcLayerGeographicBoundingBox (FdoWmsLayer* layer, FdoWmsBoundingBox* bbox)
{
	VALIDATE_ARGUMENT (layer);
	VALIDATE_ARGUMENT (bbox);

	FdoOwsGeographicBoundingBoxP geoBox = layer->GetGeographicBoundingBox ();
	if (geoBox)
	{
		double minx = geoBox->GetWestBoundLongitude ();
		double miny = geoBox->GetSouthBoundLatitude ();
		double maxx = geoBox->GetEastBoundLongitude ();
		double maxy = geoBox->GetNorthBoundLatitude ();

		if (minx < bbox->GetMinX ()) bbox->SetMinX (minx);
		if (maxx > bbox->GetMaxX ()) bbox->SetMaxX (maxx);
		if (miny < bbox->GetMinY ()) bbox->SetMinY (miny);
		if (maxy > bbox->GetMaxY ()) bbox->SetMaxY (maxy);
	}

	FdoWmsLayerCollectionP childLayers = layer->GetLayers ();
	for (FdoInt32 i=0; i<childLayers->GetCount(); i++)
	{
		FdoWmsLayerP childLayer = childLayers->GetItem (i);
		_calcLayerGeographicBoundingBox (childLayer, bbox);
	}
}

FdoOwsRequestMetadata* FdoWmsCapabilities::OnCreateRequestMetadata(FdoString* name)
{
    return FdoWmsRequestMetadata::Create(name);
}
