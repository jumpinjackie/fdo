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

#ifndef FDOWMSCAPABILITY_H
#define FDOWMSCAPABILITY_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <OWS/FdoOwsCapabilities.h>
#include <Common/StringCollection.h>
#include "FdoWmsLayerCollection.h"

class FdoWmsCapabilities : public FdoOwsCapabilities
{
	typedef FdoOwsCapabilities BaseType;

public:
   FDOWMS_API static FdoWmsCapabilities* Create();

public:
    FDOWMS_API FdoWmsLayerCollection* GetLayers() const;
    FDOWMS_API FdoXmlSaxHandler* XmlStartElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname, FdoXmlAttributeCollection* atts);
    FDOWMS_API FdoBoolean XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname);

    /// \brief
    /// Gets all supported spatial context names supported by the server.
    /// 
    /// \return
    /// Returns a collection of the spatial contexts supported.
    /// 
	FDOWMS_API FdoStringCollection* GetCRSNames ();

    /// \brief
    /// Gets the extents of the supported spatial contexts. 
    /// For each spatial context item returned by 'GetCRSNames()', there's 
    /// a corresponding extent item in the returned extents collection.
    /// 
    /// \return
    /// Returns a collection of the spatial context extents.
    /// 
	FDOWMS_API FdoWmsBoundingBoxCollection* GetCRSExtents ();
    
    // Update all SRS-es and all bounding boxes for layers.
    // update them each layer has all information without querying the parent
    void FillUpGeographicDataLayers();

	// update the BBox order in 1.3.0 and later version
	// this is a bit tricky
	// for the same data, the BBox value in capability document is different like below:
	// <BoundingBox CRS="EPSG:4326" minx="-90" miny="-180" maxx="90" maxy="180"> under 1.3.0 version
	// <BoundingBox CRS="EPSG:4326" minx="-180" miny="-90" maxx="180" maxy="90"> under previous version
	//
	// since what we’re querying and the data that we’re seeing coming back 
	// are in different coordinate systems.
	// we should reverse the BBox value for 1.3.0 or later version here
	// and later, if user use the default bbox in GetMap request
	// it will be reversed to meet the server's taste.
	//
	void AdjustBBoxOrder(FdoString* version);

protected:
    FdoWmsCapabilities(void);
    virtual ~FdoWmsCapabilities(void);
    void Dispose();
    virtual FdoOwsRequestMetadata* OnCreateRequestMetadata(FdoString* name);

private:
    // update SRSes and bounding boxes for a layer (recursive mode)
    void _processGeographicDataLayer(FdoWmsLayer* layer, FdoBoolean processLayers = true);
    //Get the default SRS even is from parent. FdoBoolean returns if we have it from parent
    FdoString* GetDefaultSRS(FdoWmsLayer* layer, FdoBoolean& isParentDefault);
    // search a bounding box for a SRS
    FdoWmsBoundingBox* _SearchBoundingBox(FdoWmsBoundingBoxCollection* boundingBoxes, FdoString* SRSName);
    // search a bounding box for a SRS in the parent layers
    FdoWmsBoundingBox* _SearchParentBoundingBox(FdoWmsLayer* layer, FdoString* SRSName);
    // returns if a SRS is supported by the layer (is looking in parents also if is necessarry)
    bool IsSRSSupportedbyLayer(FdoWmsLayer* layer, FdoString* SRSName);
    // returns parent bounding boxes. if parent layer was not processed it will be 
    // using _processGeographicDataLayer but withou processing sub-layers
    FdoWmsBoundingBoxCollection* GetParentBoundingBoxes(FdoWmsLayer* layer);

	void _buildUpCRS ();
	void _processLayerSRSName (FdoWmsLayer*, FdoStringCollection*);
	void _processLayerSRSExtent (FdoWmsLayer*, FdoString*, FdoWmsBoundingBox*, bool &);
	void _removeNonReferedSRS ();
	void _calcAllGeographicBoundingBox (FdoWmsBoundingBox* bbox);
	void _calcLayerGeographicBoundingBox (FdoWmsLayer* layer, FdoWmsBoundingBox* bbox);

    FdoWmsLayerCollectionP                 mLayers;
	FdoXmlCharDataHandlerP                 mXmlContentHandler;
	FdoStringsP                            mCRSNames;
	FdoWmsBoundingBoxCollectionP           mCRSExtents; 
};

typedef FdoPtr<FdoWmsCapabilities> FdoWmsCapabilitiesP;

#endif // FDOWMSCAPABILITY_H


