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
   static FdoWmsCapabilities* Create();

public:
    FdoWmsLayerCollection* GetLayers() const;
    FdoXmlSaxHandler* XmlStartElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname, FdoXmlAttributeCollection* atts);
    FdoBoolean XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname);
	FdoStringCollection* GetMIMETypes ();

    /// \brief
    /// Gets all supported spatial context names supported by the server.
    /// 
    /// \return
    /// Returns a collection of the spatial contexts supported.
    /// 
	FdoStringCollection* GetCRSNames ();

    /// \brief
    /// Gets the extents of the supported spatial contexts. 
    /// For each spatial context item returned by 'GetCRSNames()', there's 
    /// a corresponding extent item in the returned extents collection.
    /// 
    /// \return
    /// Returns a collection of the spatial context extents.
    /// 
	FdoWmsBoundingBoxCollection* GetCRSExtents ();
    
protected:
    FdoWmsCapabilities(void);
    virtual ~FdoWmsCapabilities(void);
    void Dispose();

private:
	void _buildUpCRS ();
	void _getTotalExtent (FdoWmsBoundingBox*, const FdoWmsBoundingBox*);
	void _processLayerSRSName (FdoWmsLayer*, FdoStringCollection*);
	void _processLayerSRSExtent (FdoWmsLayer*, FdoString*, FdoWmsBoundingBox*, bool);
	void _removeNonReferedSRS ();
	void _calcAllGeographicBoundingBox (FdoWmsBoundingBox* bbox);
	void _calcLayerGeographicBoundingBox (FdoWmsLayer* layer, FdoWmsBoundingBox* bbox);

    FdoWmsLayerCollectionP                 mLayers;
	FdoXmlCharDataHandlerP                 mXmlContentHandler;
	FdoStringsP                            mMIMETypes;
	FdoBoolean                             mbParsingMIMETypes;
	FdoStringsP                            mCRSNames;
	FdoWmsBoundingBoxCollectionP           mCRSExtents; 
};

typedef FdoPtr<FdoWmsCapabilities> FdoWmsCapabilitiesP;

#endif // FDOWMSCAPABILITY_H


