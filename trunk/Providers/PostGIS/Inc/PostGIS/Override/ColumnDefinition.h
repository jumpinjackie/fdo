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
#ifndef FDOPOSTGIS_COLUMNDEFINITION_H_INCLUDED
#define FDOPOSTGIS_COLUMNDEFINITION_H_INCLUDED

namespace fdo { namespace postgis { namespace ov {

/// \todo To be documented
class ColumnDefinition : public FdoPhysicalElementMapping
{
public:
    
    typedef FdoPtr<ColumnDefinition> Ptr;

    /// Factory creates instance of column definition object.
    FDOPOSTGIS_API static ColumnDefinition* Create();

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

    ColumnDefinition();
    virtual ~ColumnDefinition();

    //
    // FdoIDisposable interface
    //

    virtual void Dispose();

private:

    typedef FdoPhysicalElementMapping BaseType;

};

}}} // namespace fdo::postgis::ov

#endif // FDOPOSTGIS_COLUMNDEFINITION_H_INCLUDED
