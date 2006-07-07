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

#ifndef FDOWFSFEATURETYPE_H
#define FDOWFSFEATURETYPE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <OWS/FdoOwsGeographicBoundingBoxCollection.h>

class FdoWfsFeatureType :     
    public FdoIDisposable,
    public virtual FdoXmlSaxHandler
{
    typedef FdoXmlSaxHandler BaseType;
private:
    FdoStringP m_name;
    FdoStringP m_srs;
    FdoStringP m_title;
	FdoStringP m_abstract;
	FdoStringP m_keywords;
	FdoStringsP m_metadataURL;
    FdoInt32 m_operations;

	FdoOwsGeographicBoundingBoxCollectionP m_SRSExtents;

protected:
    FdoWfsFeatureType();
    virtual ~FdoWfsFeatureType();
    virtual void Dispose() { delete this; }


public:
    static FdoWfsFeatureType* Create();

    FdoString* GetName() { return m_name; }
    FdoString* GetTitle() { return m_title; }
    FdoString* GetAbstract() { return m_abstract; }
    FdoString* GetSRS() { return m_srs; }
    FdoInt32 GetOperations() { return m_operations; }

	FdoOwsGeographicBoundingBoxCollection* GetSRSExtents ();

    FdoBoolean CanSetName() { return false; }


	virtual void InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs);
    virtual FdoXmlSaxHandler* XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts
    );
    virtual FdoBoolean XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname);


};

typedef FdoPtr<FdoWfsFeatureType> FdoWfsFeatureTypeP;

class FdoWfsFeatureTypeCollection : public FdoNamedCollection<FdoWfsFeatureType, FdoException>
{
protected:
    FdoWfsFeatureTypeCollection();
    virtual ~FdoWfsFeatureTypeCollection();
    virtual void Dispose() { delete this; }

public:
    static FdoWfsFeatureTypeCollection* Create();
};

typedef FdoPtr<FdoWfsFeatureTypeCollection> FdoWfsFeatureTypesP;

#endif

