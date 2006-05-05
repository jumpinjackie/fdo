
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

#ifndef ARCSDESCHEMAMAPPING_H
#define ARCSDESCHEMAMAPPING_H

#ifdef _WIN32
#pragma once
#endif // _WIN32


class ArcSDESchemaMapping;

#include <ArcSDE/Override/FdoArcSDEClassMapping.h>


class ArcSDESchemaMapping :
    public FdoPhysicalSchemaMapping
{
protected:
    /// \brief
    /// Constructor for the ArcSDESchemaMapping class.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API ArcSDESchemaMapping(void);

public:
    /// \brief
    /// Destructor for the ArcSDESchemaMapping class.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API virtual ~ArcSDESchemaMapping(void);

    /// \brief
    /// Creates a new instance of the ArcSDESchemaMapping class.
    /// \brief
    /// /// returns a new instance of the ArcSDESchemaMapping class.
    /// 
    /// 
    static FDOSDE_API ArcSDESchemaMapping* Create(void);

    /// \brief
    /// Dispose this object.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDOSDE_API void Dispose(void)  { delete this; }

    /// \brief
    /// Gets the name of the applicable FDO provider.
    /// 
    /// \return
    /// Returns the name of FDO provider to which this Schema Override set 
    /// applies. The provider name must conform to the format:
    /// "[Company].[Provider].[Version]".
    /// 
    FDOSDE_API virtual FdoString* GetProvider();


    ////////////////////////////////////////////
    ////////////////////////////////////////////
    /// Non-inherited public methods:

    /// \brief
    /// Get the collection of class overrides for this schema mapping.
    /// 
    /// \return
    /// Returns the collection of class overrides in this schema mapping.
    /// 
    FDOSDE_API ArcSDEClassMappingCollection* GetClasses();  


    ////////////////////////////////////////////
    ////////////////////////////////////////////
    /// Functions to support XML deserialization:

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
    ////////////////////////////////////////////
    ////////////////////////////////////////////
    /// Functions to support XML serialization:

    /// \brief
    /// Write this element to XML.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    virtual void _writeXml( FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags );


protected:
    /// \brief
    /// The class mapping collection for this schema.
    /// 
    FdoPtr<ArcSDEClassMappingCollection> m_ClassMappings;
};

#endif // ArcSDESchemaMapping_H


