
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

#ifndef ARCSDEINDEXMAPPING_H
#define ARCSDEINDEXMAPPING_H

#ifdef _WIN32
#pragma once
#endif // _WIN32


class ArcSDEIndexMapping : public FdoPhysicalElementMapping
{
protected:
    /// \brief
    /// The constructor for the ArcSDEIndexMapping class.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API ArcSDEIndexMapping(void);

public:
    /// \brief
    /// The destructor for the ArcSDEIndexMapping class.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API virtual ~ArcSDEIndexMapping(void);

    /// \brief
    /// Creates a new instance of the ArcSDEIndexMapping class.
    /// 
    /// \return
    /// Returns a new instance of the ArcSDEIndexMapping class.
    /// 
    static FDOSDE_API ArcSDEIndexMapping* Create(void);

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
    /// Returns true if index enforces uniqueness, false otherwise.
    /// 
    /// \return
    /// Returns true if index enforces uniqueness, false otherwise.
    /// 
    FDOSDE_API bool IsUnique();

    /// \brief
    /// Set this to true if index should enforce uniqueness, false otherwise.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API void SetUnique(bool value);

    /// \brief
    /// Returns true if index enforces ascending order, false otherwise.
    /// 
    /// \return
    /// Returns true if index enforces ascending order, false otherwise.
    /// 
    FDOSDE_API bool IsAscending();

    /// \brief
    /// Set this to true if index should enforce ascending order, false otherwise.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API void SetAscending(bool value);

    /// \brief
    /// Returns the collection of FDO property names this index applies to.  Must be non-empty when executing ApplySchema.
    /// 
    /// \return
    /// Returns the collection of FDO property names this index applies to.
    /// 
    FDOSDE_API FdoStringCollection* GetPropertyNames();


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
    /// Whether or not this index enforces uniqueness.
    /// 
    bool m_Unique;

    /// \brief
    /// Whether or not this index enforces ascending order.
    /// 
    bool m_Ascending;

    /// \brief
    /// The properties this index applies to.
    /// 
    FdoPtr<FdoStringCollection> m_IndexPropertyMappings;
};


class ArcSDEIndexMappingCollection : public FdoPhysicalElementMappingCollection<ArcSDEIndexMapping>
{
protected:
    /// 0-argument constructor to please FdoPtr::operator->() :
    ArcSDEIndexMappingCollection () { ArcSDEIndexMappingCollection(NULL); }

    /// \brief
    /// The constructor for the ArcSDEIndexMappingCollection class.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API ArcSDEIndexMappingCollection(ArcSDEClassMapping *parent) : FdoPhysicalElementMappingCollection<ArcSDEIndexMapping>((FdoPhysicalElementMapping*)parent) {};

public:
    /// \brief
    /// The destructor for the ArcSDEIndexMappingCollection class.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API virtual ~ArcSDEIndexMappingCollection(void)  {};

    /// \brief
    /// Creates a new instance of the ArcSDEIndexMappingCollection class.
    /// 
    /// \return
    /// Returns a new instance of the ArcSDEIndexMappingCollection class.
    /// 
    static FDOSDE_API ArcSDEIndexMappingCollection* Create(ArcSDEClassMapping *parent);

    /// \brief
    /// Dispose this object.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API void Dispose(void)  { delete this; }
};



#endif // ARCSDEINDEXMAPPING_H


