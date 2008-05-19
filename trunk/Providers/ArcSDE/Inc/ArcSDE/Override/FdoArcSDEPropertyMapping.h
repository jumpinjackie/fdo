
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
//

#ifndef ARCSDEPROPERTYMAPPING_H
#define ARCSDEPROPERTYMAPPING_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

class ArcSDEPropertyMappingCollection;

#include <ArcSDE/Override/FdoArcSDEClassMapping.h>


class ArcSDEPropertyMapping : public FdoPhysicalPropertyMapping
{
protected:
    /// \brief
    /// The constructor for the ArcSDEPropertyMapping class.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API ArcSDEPropertyMapping(void);

public:
    /// \brief
    /// The destructor for the ArcSDEPropertyMapping class.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API virtual ~ArcSDEPropertyMapping(void);

    /// \brief
    /// Creates a new instance of the ArcSDEPropertyMapping class.
    /// \brief
    /// /// returns a new instance of the ArcSDEPropertyMapping class.
    /// 
    /// 
    static FDOSDE_API ArcSDEPropertyMapping* Create(void);

    /// \brief
    /// Dispose this object.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API void Dispose(void)  { delete this; }


    ///////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////
    /// Non-inherited public methods:

    /// \brief
    /// Returns the ArcSDE column name override for this property.
    /// 
    /// \return
    /// Returns the ArcSDE column name override for this property.
    /// 
    FDOSDE_API FdoString* GetColumnName();

    /// \brief
    /// Sets the ArcSDE column name override for this property.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API void SetColumnName(FdoString*);

    /// \brief
    /// Returns the ArcSDE column type override for this property.
    ///          Returns -1 if no override is currently set.
    /// 
    /// \return
    /// Returns the ArcSDE column name override for this property.
    /// 
    FDOSDE_API LONG GetColumnType();

    /// \brief
    /// Sets the ArcSDE column type override for this property.
    ///          Set to -1 if no override is desired.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API void SetColumnType(LONG sde_type);


    ///////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////
    /// Functions for XML support:

    /// \brief
    /// Initialize this element from its XML attributes.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API virtual void InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs);

    /// \brief
    /// Handle the start of a sub-element.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API virtual FdoXmlSaxHandler* XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts
    );

    /// \brief
    /// Handle the end of a sub-element.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API virtual FdoBoolean XmlEndElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname
    );

protected:
    /// \brief
    /// The ArcSDE column name override for this property.
    /// 
    FdoStringP m_ColumnName;

    /// \brief
    /// The ArcSDE column type override for this property.
    /// 
    LONG m_ColumnType;
};


class ArcSDEPropertyMappingCollection : public FdoPhysicalElementMappingCollection<ArcSDEPropertyMapping>
{
protected:
    /// 0-argument constructor to please FdoPtr::operator->() :
    ArcSDEPropertyMappingCollection () { ArcSDEPropertyMappingCollection(NULL); }

    /// \brief
    /// The constructor for the ArcSDEPropertyMappingCollection class.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API ArcSDEPropertyMappingCollection(ArcSDEClassMapping *parent) : FdoPhysicalElementMappingCollection<ArcSDEPropertyMapping>((FdoPhysicalElementMapping*)parent) {};

public:
    /// \brief
    /// The destructor for the ArcSDEPropertyMappingCollection class.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API virtual ~ArcSDEPropertyMappingCollection(void)  {};

    /// \brief
    /// Creates a new instance of the ArcSDEPropertyMappingCollection class.
    /// \brief
    /// /// returns a new instance of the ArcSDEPropertyMappingCollection class.
    /// 
    /// 
    static FDOSDE_API ArcSDEPropertyMappingCollection* Create(ArcSDEClassMapping *parent);

    /// \brief
    /// Dispose this object.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API void Dispose(void)  { delete this; }
};


#endif // ARCSDEPROPERTYMAPPING_H


