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

#ifndef FDOWFSOGCFILTERCAPABILITIES_H
#define FDOWFSOGCFILTERCAPABILITIES_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

class FdoWfsOgcSpatialCapabilities;
class FdoWfsOgcScalarCapabilities;

class FdoWfsOgcFilterCapabilities : 
    public FdoIDisposable,
    public virtual FdoXmlSaxHandler
{
    typedef FdoXmlSaxHandler BaseType;
private:
    FdoPtr<FdoWfsOgcSpatialCapabilities> m_spatialCapabilities;
    FdoPtr<FdoWfsOgcScalarCapabilities> m_scalarCapabilities;

protected:
    FdoWfsOgcFilterCapabilities();
    virtual ~FdoWfsOgcFilterCapabilities();
    virtual void Dispose() { delete this; }


public:
    static FdoWfsOgcFilterCapabilities* Create();

    FdoWfsOgcSpatialCapabilities* GetSpatialCapabilities();
    FdoWfsOgcScalarCapabilities* GetScalarCapabilities();

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

typedef FdoPtr<FdoWfsOgcFilterCapabilities> FdoWfsOgcFilterCapabilitiesP;

#endif

