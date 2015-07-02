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

#ifndef FDOWFSOGCSPATIALCAPABILITIES_H
#define FDOWFSOGCSPATIALCAPABILITIES_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

class FdoWfsOgcSpatialCapabilities : 
    public FdoIDisposable,
    public virtual FdoXmlSaxHandler
{
    typedef FdoXmlSaxHandler BaseType;
public:
    enum SpatialOperators
    {
        SpatialOperators_Intersect = 0x1,
        SpatialOperators_BBOX = 0x1 << 1,
        SpatialOperators_Contains = 0x1 << 2,
        SpatialOperators_Crosses = 0x1 << 3,
        SpatialOperators_Disjoint = 0x1 << 4,
        SpatialOperators_Equals = 0x1 << 5,
        SpatialOperators_Overlaps = 0x1 << 6,
        SpatialOperators_Touches = 0x1 << 7,
        SpatialOperators_Within = 0x1 << 8,
        SpatialOperators_DWithin = 0x1 << 9,
        SpatialOperators_Beyond = 0x1 << 10,
    };
private:
    FdoInt32 m_spatialOperators;

protected:
    FdoWfsOgcSpatialCapabilities();
    virtual ~FdoWfsOgcSpatialCapabilities();
    virtual void Dispose() { delete this; }


public:
    static FdoWfsOgcSpatialCapabilities* Create();

    FdoInt32 GetSpatialOperators() { return m_spatialOperators; }

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

typedef FdoPtr<FdoWfsOgcSpatialCapabilities> FdoWfsOgcSpatialCapabilitiesP;


#endif

