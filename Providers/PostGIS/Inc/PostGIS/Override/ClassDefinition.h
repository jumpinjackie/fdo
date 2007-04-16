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
#ifndef FDOPOSTGIS_CLASSDEFINITION_H_INCLUDED
#define FDOPOSTGIS_CLASSDEFINITION_H_INCLUDED

#include "PropertyDefinitionCollection.h"

namespace fdo { namespace postgis { namespace ov {

/// \todo To be documented
class ClassDefinition : public FdoPhysicalClassMapping
{
public:
    
    /// Type of FDO smart pointer for the class.
    typedef FdoPtr<ClassDefinition> Ptr;

    //
    // FdoPhysicalClassMapping interface
    //
    FDOPOSTGIS_API static ClassDefinition* Create();

    FDOPOSTGIS_API PropertyDefinitionCollection* GetProperties() const;
    
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

    //
    // ClassDefinition custom interface
    //

    /// Get name of schema (FDO datastore).
    /// The name is NOT wrapped with quotes here.
    FDOPOSTGIS_API FdoStringP GetSchemaName() const;

    /// Get name of table.
    /// The name is NOT wrapped with quotes here.
    FDOPOSTGIS_API FdoStringP GetTableName() const;

    /// Get fully qualified table path in following format "schema"."table" .
    /// Both names are wrapped with double quotes automatically
    /// according to the <strong>always quote</strong> strategy.
    /// 
    /// \return String with full path of table.
    ///
    /// For more details about quoting SQL identifiers in PostgreSQL database,
    /// see following chapter from the manual:
    /// <a href="http://www.postgresql.org/docs/8.2/interactive/sql-syntax-lexical.html#SQL-SYNTAX-IDENTIFIERS">4.1.1. Identifiers and Key Words</a>.
    ///
    FDOPOSTGIS_API FdoStringP GetTablePath() const;

protected:
    
    /// Default constructor.
    ClassDefinition();
    
    /// Destructor.
    virtual ~ClassDefinition();

    //
    // FdoIDisposable
    //

    void Dispose();
    
private:

    typedef FdoPhysicalClassMapping BaseType;
    
    PropertyDefinitionCollection::Ptr mProperties;
};

}}} // namespace fdo::postgis::ov

#endif // FDOPOSTGIS_CLASSDEFINITION_H_INCLUDED
