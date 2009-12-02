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
#include "stdafx.h"

#include <PostGIS/Override/ClassDefinition.h>

#include <cassert>

namespace fdo { namespace postgis { namespace ov {

ClassDefinition::ClassDefinition()
{
    mProperties = PropertyDefinitionCollection::Create(this);
}

ClassDefinition::~ClassDefinition()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void ClassDefinition::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoPhysicalClassMapping interface
///////////////////////////////////////////////////////////////////////////////

ClassDefinition* ClassDefinition::Create()
{
    ClassDefinition* def = new ClassDefinition();
    return def;
}

PropertyDefinitionCollection* ClassDefinition::GetProperties() const
{
    FDO_SAFE_ADDREF(mProperties.p);
    return mProperties.p;
}

///////////////////////////////////////////////////////////////////////////////
// FdoPhysicalElementMapping interface
///////////////////////////////////////////////////////////////////////////////

void ClassDefinition::InitFromXml(FdoXmlSaxContext* xmlContext,
    FdoXmlAttributeCollection* xmlAttrs)
{
    FDOLOG_MARKER("ClassDefinition::+InitFromXml");
}

void ClassDefinition::_writeXml(FdoXmlWriter* xmlWriter, FdoXmlFlags const* xmlFlags)
{
    FDOLOG_MARKER("ClassDefinition::+_writeXml");
}

///////////////////////////////////////////////////////////////////////////////
// FdoXmlSaxHandler interface
///////////////////////////////////////////////////////////////////////////////

FdoXmlSaxHandler* ClassDefinition::XmlStartElement(FdoXmlSaxContext *saxContext,
                                                   FdoString* uri,
                                                   FdoString* name,
                                                   FdoString* qname,
                                                   FdoXmlAttributeCollection* xmlAtts)
{
    FDOLOG_MARKER("ClassDefinition::+XmlStartElement");
    return NULL;
}

FdoBoolean ClassDefinition::XmlEndElement(FdoXmlSaxContext* saxContext,
                                          FdoString* uri,
                                          FdoString* name,
                                          FdoString* qname)
{
    FDOLOG_MARKER("ClassDefinition::+XmlEndElement");
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// ClassDefinition custom interface
///////////////////////////////////////////////////////////////////////////////

void ClassDefinition::SetSchemaName(FdoString* value)
{
  m_schema = value;
}

FdoStringP ClassDefinition::GetSchemaName() const
{
    return m_schema;
}

FdoStringP ClassDefinition::GetTableName() const
{
    // NOTE: We need this dirty const_cast hack due to lack of
    // const-correctness in FdoPhysicalClassMapping::GetName() class.
    // This note also applies to functions below. 
    FdoStringP name(const_cast<ClassDefinition*>(this)->GetName());

    return name;
}

FdoStringP ClassDefinition::GetTablePath() const
{
    FdoStringP schema(const_cast<ClassDefinition*>(this)->GetSchemaName());
    FdoStringP table(const_cast<ClassDefinition*>(this)->GetTableName());

    /// just in case schema is empty
    FdoStringP path;
    if (schema.GetLength() && table.GetLength()) 
    {
      path = FdoStringP::Format(L"\"%ls\".\"%ls\"",
          static_cast<FdoString*>(schema), static_cast<FdoString*>(table));
    } 
    else
    {
      path = table;
    }
    return path;
}

}}} // namespace fdo::postgis::ov
