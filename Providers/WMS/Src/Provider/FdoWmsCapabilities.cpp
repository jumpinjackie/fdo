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

void FdoWmsCapabilities::_processLayerSRSExtent (FdoWmsLayer* layer, FdoString* crsName, FdoWmsBoundingBox* extent, bool& bFirstMatch)
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
	            if (extent->GetMinX () > bbox->GetMinX ())
		            extent->SetMinX (bbox->GetMinX ());
	            if (extent->GetMinY () > bbox->GetMinY ())
		            extent->SetMinY (bbox->GetMinY ());
	            if (extent->GetMaxX () < bbox->GetMaxX ())
		            extent->SetMaxX (bbox->GetMaxX ());
	            if (extent->GetMaxY () < bbox->GetMaxY ())
		            extent->SetMaxY (bbox->GetMaxY ());
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

// search a bounding box for a SRS
FdoWmsBoundingBox* FdoWmsCapabilities::_SearchBoundingBox(FdoWmsBoundingBoxCollection* boundingBoxes, FdoString* SRSName)
{
    if (boundingBoxes->GetCount() != 0)
    {
        for(int i = 0; i < boundingBoxes->GetCount(); i++)
        {
            FdoPtr<FdoWmsBoundingBox> pItem = boundingBoxes->GetItem(i);
            if (!wcscmp(pItem->GetCRS(), SRSName))
                return FDO_SAFE_ADDREF(pItem.p);
        }
    }
    return NULL;
}

//Get the default SRS even is from parent. FdoBoolean returns if we have it from parent
FdoString* FdoWmsCapabilities::GetDefaultSRS(FdoWmsLayer* layer, FdoBoolean& isParentDefault)
{
    FdoPtr<FdoStringCollection> pSRSColl = layer->GetCoordinateReferenceSystems();
    if (pSRSColl != NULL && pSRSColl->GetCount() != 0)
    {
        FdoPtr<FdoStringElement> elemString = pSRSColl->GetItem(0);
        return elemString->GetString();
    }
    FdoPtr<FdoWmsLayer> parentLayer = layer->GetParent();
    if (parentLayer)
    {
        isParentDefault = true;
        return GetDefaultSRS(parentLayer, isParentDefault);
    }
    return NULL;
}

// returns if a SRS is supported by the layer (is looking in parents also if is necessarry)
bool FdoWmsCapabilities::IsSRSSupportedbyLayer(FdoWmsLayer* layer, FdoString* SRSName)
{
    FdoPtr<FdoStringCollection> pSRSColl = layer->GetCoordinateReferenceSystems();
    for (FdoInt32 i=0; i < pSRSColl->GetCount (); i++)
    {
        FdoPtr<FdoStringElement> elemString = pSRSColl->GetItem(i);
        if (elemString->GetString() == SRSName)
            return true;
    }
    FdoPtr<FdoWmsLayer> parentLayer = layer->GetParent();
    return (parentLayer != NULL) ? IsSRSSupportedbyLayer(parentLayer, SRSName) : false;
}

// returns parent bounding boxes. if parent layer was not processed it will be 
// using _processGeographicDataLayer but withou processing sub-layers
FdoWmsBoundingBoxCollection* FdoWmsCapabilities::GetParentBoundingBoxes(FdoWmsLayer* layer)
{
    FdoPtr<FdoWmsLayer> parentLayer = layer->GetParent();
    if (parentLayer)
    {
        if (!parentLayer->GetLayerProcessed())
            _processGeographicDataLayer(parentLayer, false);

        FdoPtr<FdoWmsBoundingBoxCollection> boundingBoxes = parentLayer->GetBoundingBoxes();
        return FDO_SAFE_ADDREF(boundingBoxes.p);
    }
    return NULL;
}

// search a bounding box for a SRS in the parent layers
FdoWmsBoundingBox* FdoWmsCapabilities::_SearchParentBoundingBox(FdoWmsLayer* layer, FdoString* SRSName)
{
    FdoPtr<FdoWmsLayer> parentLayer = layer->GetParent();
    if (parentLayer)
    {
        FdoPtr<FdoWmsBoundingBoxCollection> boundingBoxes = parentLayer->GetBoundingBoxes();
        FdoPtr<FdoWmsBoundingBox> pBox = _SearchBoundingBox(boundingBoxes, SRSName);
        if (pBox)
            return FDO_SAFE_ADDREF(pBox.p);
        else
            return _SearchParentBoundingBox(parentLayer, SRSName);
    }
    return NULL;
}

// update SRSes and bounding boxes for a layer (recursive mode)
void FdoWmsCapabilities::_processGeographicDataLayer(FdoWmsLayer* layer, FdoBoolean processLayers)
{
    if (processLayers)
    {
        // start with sub-layers in case we must process them
        FdoPtr<FdoWmsLayerCollection> layers = layer->GetLayers();
	    for (FdoInt32 i=0; i < layers->GetCount (); i++)
	    {
		    FdoPtr<FdoWmsLayer> _layer = layers->GetItem (i);
		    _processGeographicDataLayer (_layer);
	    }
    }
    // if somehow the layer was already processed don't do it anymore 
    if (layer->GetLayerProcessed())
        return;
    FdoPtr<FdoOwsGeographicBoundingBox> geoBox = layer->GetGeographicBoundingBox();
    FdoPtr<FdoWmsBoundingBoxCollection> boundingBoxes = layer->GetBoundingBoxes();
    if (geoBox != NULL)
    {
        // in case we have a GeographicBoundingBox try to fill up all supported BoundingBoxes
        FdoBoolean isParentDefault = false;
        // in case default SRS has a bounding box look if global default SRS is supported by layer
        // if yes generate a new one using global default SRS and GeographicBoundingBox
        FdoStringP defaultSRS = FdoWmsGlobals::DefaultEPSGCode;
        bool isSRSSupported = IsSRSSupportedbyLayer(layer, defaultSRS);
        if (!isSRSSupported)
        {
            defaultSRS = FdoWmsGlobals::DefaultEPSGCode2;
            isSRSSupported = IsSRSSupportedbyLayer(layer, defaultSRS);
            if (!isSRSSupported) // get the default SRS even is from parent
                defaultSRS = GetDefaultSRS(layer, isParentDefault);
        }
        if (defaultSRS.GetLength() != 0)
        {
            if (isSRSSupported)
            {
                // add EPSG:4326
                FdoPtr<FdoWmsBoundingBox> pBox = FdoWmsBoundingBox::Create();
                pBox->SetCRS(defaultSRS);

                // generate a new one using default SRS and GeographicBoundingBox
                pBox->SetMinY(geoBox->GetSouthBoundLatitude ());
                pBox->SetMinX(geoBox->GetWestBoundLongitude ());
                pBox->SetMaxX(geoBox->GetEastBoundLongitude ());
                pBox->SetMaxY(geoBox->GetNorthBoundLatitude ());
                
                boundingBoxes->Add(pBox);
            }
            else
            {
                // look if we have an bounding box for this default SRS
                FdoPtr<FdoWmsBoundingBox> pBox = _SearchBoundingBox(boundingBoxes, defaultSRS);
                // NOTE: For WMS 1.1.1, Layers may have 0+ <BoundingBox> elements
                // For WMS 1.3.0, WMS service metadata shall declare 1+ bounding boxes for each Layer. 
                if (pBox == NULL)
                {
                    // in case we don't have a bounding box for the default SRS, try to generate one
                    FdoPtr<FdoWmsBoundingBox> parentBox;

                    // A Bounding Box metadata element may either be stated explicitly or may be inherited from a parent Layer.
                    parentBox = _SearchParentBoundingBox(layer, defaultSRS);

                    pBox = FdoWmsBoundingBox::Create();
                    if (parentBox)
                    {
                        // add parent bounding box 
                        pBox->SetCRS(defaultSRS);
                        pBox->SetMinY(parentBox->GetMinY ());
                        pBox->SetMinX(parentBox->GetMinX ());
                        pBox->SetMaxX(parentBox->GetMaxX ());
                        pBox->SetMaxY(parentBox->GetMaxY ());
                        boundingBoxes->Add(pBox);
                    }
                    else if (geoBox != NULL)
                    {
                        // we do have a GeographicBoundingBox and we should use it since no other SRS was found.
                        // with a valid BoundingBox
                        pBox->SetCRS(FdoWmsGlobals::DefaultEPSGCode);

                        // generate a new one using default SRS and GeographicBoundingBox
                        pBox->SetMinY(geoBox->GetSouthBoundLatitude ());
                        pBox->SetMinX(geoBox->GetWestBoundLongitude ());
                        pBox->SetMaxX(geoBox->GetEastBoundLongitude ());
                        pBox->SetMaxY(geoBox->GetNorthBoundLatitude ());

                        boundingBoxes->Add(pBox);
                        
                        // add DefaultEPSGCode to the supported list
                        FdoStringsP crsNames = layer->GetCoordinateReferenceSystems();
	                    if (crsNames != NULL && (crsNames->IndexOf (FdoWmsGlobals::DefaultEPSGCode) == -1))
                            crsNames->Add(FdoWmsGlobals::DefaultEPSGCode);
                    }
                }
            }
        }
    }
    else
    {
        // we don't have a GeographicBoundingBox so try to see what we can support
        if (boundingBoxes->GetCount() == 0)
        {
            // in case layer don't have anything try get data from parent
            FdoPtr<FdoWmsBoundingBoxCollection> parentBoxes = GetParentBoundingBoxes(layer);
            if (parentBoxes != NULL && parentBoxes->GetCount())
            {
                for(FdoInt32 i = 0; i < parentBoxes->GetCount(); i++)
                {
                    FdoPtr<FdoWmsBoundingBox> pItem = parentBoxes->GetItem(i);
                    FdoPtr<FdoWmsBoundingBox> pBox = FdoWmsBoundingBox::Create();
                    pBox->SetCRS(pItem->GetCRS());
                    pBox->SetMinX(pItem->GetMinX());
                    pBox->SetMinY(pItem->GetMinY());
                    pBox->SetMaxX(pItem->GetMaxX());
                    pBox->SetMaxY(pItem->GetMaxY());
                    boundingBoxes->Add(pBox);
                }
            }
        }
    }
    // do not clean the root layer
    FdoPtr<FdoWmsLayerCollection> layers = GetLayers ();
    FdoPtr<FdoWmsLayer> rootLayer = layers->GetItem (0);
    if (layer != rootLayer.p)
    {
        // copy root bounding boxes (all layer must have root bounding boxes)
        if (!rootLayer->GetLayerProcessed())
            _processGeographicDataLayer(rootLayer, false);

        FdoPtr<FdoWmsBoundingBoxCollection> rootBoxes = rootLayer->GetBoundingBoxes();
        for(FdoInt32 i = 0; i < rootBoxes->GetCount(); i++)
        {
            FdoPtr<FdoWmsBoundingBox> pItem = rootBoxes->GetItem(i);
            FdoPtr<FdoWmsBoundingBox> pBox = _SearchBoundingBox(boundingBoxes, pItem->GetCRS());
            if (pBox == NULL)
            {
                pBox = FdoWmsBoundingBox::Create();
                pBox->SetCRS(pItem->GetCRS());
                pBox->SetMinY(pItem->GetMinY ());
                pBox->SetMinX(pItem->GetMinX ());
                pBox->SetMaxX(pItem->GetMaxX ());
                pBox->SetMaxY(pItem->GetMaxY ());
                boundingBoxes->Add(pBox);
            }
        }
        // rebuild supported SRS list
        FdoPtr<FdoStringCollection> pSRSColl = layer->GetCoordinateReferenceSystems();
        pSRSColl->Clear();
        for(FdoInt32 i = 0; i < boundingBoxes->GetCount(); i++)
        {
            FdoPtr<FdoWmsBoundingBox> pItem = boundingBoxes->GetItem(i);
            pSRSColl->Add(pItem->GetCRS());
        }
    }
    layer->SetLayerProcessed(true);
}

// Update all SRS-es and all bounding boxes for layers.
// update them each layer has all information without querying the parent
void FdoWmsCapabilities::FillUpGeographicDataLayers()
{
	FdoPtr<FdoWmsLayerCollection> layers = GetLayers ();
    // Start with root to get all sub-layers
    if (layers->GetCount() > 0) 
    {
        FdoPtr<FdoWmsLayer> rootLayer = layers->GetItem (0);
	    _processGeographicDataLayer (rootLayer);
	}
}

// update the BBox order in 1.3.0 and later version
void FdoWmsCapabilities::AdjustBBoxOrder(FdoString* version)
{
	// only handle it in 1.3.0 and later version
	if (wcscmp(FdoWmsGlobals::WmsVersion100,version) == 0 ||
		wcscmp(FdoWmsGlobals::WmsVersion110,version) == 0 ||
		wcscmp(FdoWmsGlobals::WmsVersion111,version) == 0)
		return;
	
	FdoPtr<FdoWmsLayerCollection> layers = GetLayers ();
    for (FdoInt32 i=0; i < layers->GetCount (); i++)
	{
		FdoPtr<FdoWmsLayer> layer = layers->GetItem (i);
		FdoPtr<FdoWmsBoundingBoxCollection> boundingBoxes = layer->GetBoundingBoxes();

		// reverse it if needed
		for (FdoInt32 j = 0; j<boundingBoxes->GetCount(); j++)
		{
			FdoPtr<FdoWmsBoundingBox> bbox = boundingBoxes->GetItem(j);
			FdoStringP crs = bbox->GetCRS();
			if (crs.Contains(FdoWmsGlobals::ESPGPrefix) && _reverseCheck(crs))
			{
				FdoDouble minx = bbox->GetMinX();
				FdoDouble miny = bbox->GetMinY();
				FdoDouble maxx = bbox->GetMaxX();
				FdoDouble maxy = bbox->GetMaxY();

				//reverse it
				bbox->SetMinX(miny);
				bbox->SetMinY(minx);
				bbox->SetMaxX(maxy);
				bbox->SetMaxY(maxx);
			}
		}
	}
}

