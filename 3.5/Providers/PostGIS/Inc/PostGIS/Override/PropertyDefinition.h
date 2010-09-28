//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#ifndef FDOPOSTGIS_PROPERTYDEFINITION_H_INCLUDED
#define FDOPOSTGIS_PROPERTYDEFINITION_H_INCLUDED

#include <Fdo.h>
#include <PostGIS/Override/ColumnDefinition.h>

namespace fdo { namespace postgis { namespace ov {

/// \todo To be documented
class PropertyDefinition : public FdoPhysicalPropertyMapping
{
public:
    
    typedef FdoPtr<PropertyDefinition> Ptr;

    //
    // FdoPhysicalPropertyMapping interface
    //
    FDOPOSTGIS_API static PropertyDefinition* Create();

    FDOPOSTGIS_API ColumnDefinition* GetColumn() const;
    
    FDOPOSTGIS_API void SetColumn(ColumnDefinition* columnDef);

    //
    // FdoPhysicalElementMapping interface
    //

    /// Initializes this class from its XML attributes.
    FDOPOSTGIS_API virtual void InitFromXml(FdoXmlSaxContext* xmlContext,
        FdoXmlAttributeCollection* xmlAttrs);

    /// Writes this class to XML.
    FDOPOSTGIS_API virtual void _writeXml(FdoXmlWriter* xmlWriter,
        FdoXmlFlags const* xmlFlags);

    //
    // FdoXmlSaxHandler interface
    //

    /// SAX callback called when the FdoXmlReader reads the start tag for
    /// an XML element in the document.
    FDOPOSTGIS_API virtual FdoXmlSaxHandler* XmlStartElement(FdoXmlSaxContext *saxContext,
        FdoString* uri, FdoString* name, FdoString* qname,
        FdoXmlAttributeCollection* xmlAtts);

    /// SAX callback called when the FdoXmlReader reads the end tag for
    /// an XML element in the document. 
    FDOPOSTGIS_API virtual FdoBoolean XmlEndElement(FdoXmlSaxContext* saxContext,
        FdoString* uri, FdoString* name, FdoString* qname);

protected:

    PropertyDefinition();
    virtual ~PropertyDefinition();

    //
    // FdoIDisposable interface
    //

    virtual void Dispose();

private:

    typedef FdoPhysicalElementMapping BaseType;
};

}}} // namespace fdo::postgis::ov

#endif // FDOPOSTGIS_PROPERTYDEFINITION_H_INCLUDED
