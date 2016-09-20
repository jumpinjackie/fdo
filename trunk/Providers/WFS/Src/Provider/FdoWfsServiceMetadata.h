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

#ifndef FDOWFSSERVICEMETADATA_H
#define FDOWFSSERVICEMETADATA_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <OWS/FdoOwsServiceMetadata.h>
#include <OWS/FdoOwsGeographicBoundingBoxCollection.h>

class FdoWfsFeatureTypeList;
class FdoWfsOgcFilterCapabilities;
class FdoWfsFeatureType;

class FdoWfsServiceMetadata : public FdoOwsServiceMetadata
{
    typedef FdoOwsServiceMetadata BaseType;
private:
    FdoPtr<FdoWfsFeatureTypeList>   m_featureTypeList;
    FdoPtr<FdoWfsOgcFilterCapabilities> m_filterCapabilities;
	FdoStringsP m_CRSNames;
	FdoOwsGeographicBoundingBoxCollectionP m_CRSExtents;

	void _buildUpCRS ();
	void _getTotalExtent (FdoOwsGeographicBoundingBox*, const FdoOwsGeographicBoundingBox*);

protected:
    FdoWfsServiceMetadata();
    virtual ~FdoWfsServiceMetadata();
    virtual void Dispose() { delete this; }

public:
    static FdoWfsServiceMetadata* Create();

    FdoWfsFeatureTypeList* GetFeatureTypeList();
    FdoWfsOgcFilterCapabilities* GetOGCFilterCapabilities();

	FdoStringCollection* GetCRSNames ();
	FdoOwsGeographicBoundingBoxCollection* GetCRSExtents ();

    virtual FdoXmlSaxHandler* XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts
    );
    virtual FdoBoolean XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname);

    virtual FdoXmlSaxContext* GetSaxContext();

    FdoWfsFeatureType* GetFeatureType(FdoIdentifier* className);
};



#endif

