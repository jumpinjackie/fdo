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

#ifndef FDOWMSXMLLAYER_H
#define FDOWMSXMLLAYER_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "FdoWmsStyleCollection.h"
#include "FdoWmsDimensionCollection.h"
#include "FdoWmsBoundingBoxCollection.h"
#include <OWS/FdoOwsGeographicBoundingBox.h>

class FdoWmsLayerCollection;

class FdoWmsLayer : public FdoXmlSaxHandler, public FdoIDisposable
{
	typedef FdoXmlSaxHandler BaseType;

public:
    static FdoWmsLayer* Create(void);

public:
    FdoString* GetName() const;
    void SetName(FdoString* name);

    FdoBoolean GetQueryable() const;
    void SetQueryable(FdoBoolean queryable);

    FdoBoolean GetOpaque() const;
    void SetOpaque(FdoBoolean opaque);

    FdoBoolean GetNoSubsets() const;
    void SetNoSubsets(FdoBoolean noSubsets);

    FdoInt32 GetFixedWidth() const;
    void SetFixedWidth(FdoInt32 width);

    FdoInt32 GetFixedHeight() const;
    void SetFixedHeight(FdoInt32 height);

    FdoString* GetTitle() const;
    void SetTitle(FdoString* title);

    FdoString* GetAbstract() const;
    void SetAbstract(FdoString* abstract);

    FdoDouble GetMinScaleDemoninator() const;
    void SetMinScaleDemoninator(FdoDouble scale);

    FdoDouble GetMaxScaleDemoninator() const;
    void SetMaxScaleDemoninator(FdoDouble scale);

    FdoOwsGeographicBoundingBox* GetGeographicBoundingBox() const;
    void SetGeographicBoundingBox(FdoOwsGeographicBoundingBox* value);

    FdoWmsBoundingBoxCollection* GetBoundingBoxes() const;
    FdoWmsLayerCollection* GetLayers() const;
    FdoWmsStyleCollection* GetStyles() const;
    FdoWmsDimensionCollection* GetDimensions() const;
    FdoStringCollection* GetKeyordList() const;
    FdoStringCollection* GetCoordinateReferenceSystems() const;

	FdoWmsLayer* GetParent ();
	void SetParent (FdoWmsLayer* parent);

	bool FindCoordinateReferenceSystem (FdoString* coordSystem);

    void InitFromXml(FdoXmlSaxContext* context, FdoXmlAttributeCollection* attrs);

    virtual FdoXmlSaxHandler* XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts
    );

    FdoBoolean XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname);
    
protected:
    FdoWmsLayer(void);
    virtual ~FdoWmsLayer(void);
    void Dispose();

private:
    FdoInt32 mFixedWidth;
    FdoInt32 mFixedHeight;
    FdoDouble mMinScaleDenominator;
    FdoDouble mMaxScaleDenominator;
    FdoStringP mName;
    FdoBoolean mQueryable;
    FdoBoolean mOpaque;
    FdoBoolean mNoSubsets;
    FdoStringP mTitle;
    FdoStringP mAbstract;
    FdoStringsP mKeywordList;
    FdoStringsP mCoordinateReferenceSystems;
    FdoXmlCharDataHandlerP mXmlContentHandler;
    FdoWmsBoundingBoxCollectionP mBoundingBoxes;
    FdoWmsStyleCollectionP mStyles;
    FdoWmsDimensionCollectionP mDimensions;
    FdoOwsGeographicBoundingBoxP mGeographicBoundingBox;
    FdoPtr<FdoWmsLayerCollection> mChildLayers;
	FdoWmsLayer* mParent;
};

typedef FdoPtr<FdoWmsLayer> FdoWmsLayerP;

#endif // FDOWMSXMLLAYER_H


