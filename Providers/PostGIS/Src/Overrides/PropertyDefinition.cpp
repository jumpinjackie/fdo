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

#include <PostGIS/Override/ColumnDefinition.h>
#include <PostGIS/Override/PropertyDefinition.h>

#include <cassert>

namespace fdo { namespace postgis { namespace ov {

PropertyDefinition::PropertyDefinition()
{
}

PropertyDefinition::~PropertyDefinition()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void PropertyDefinition::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoPhysicalPropertyMapping interface
///////////////////////////////////////////////////////////////////////////////

PropertyDefinition* PropertyDefinition::Create()
{
    PropertyDefinition* def = new PropertyDefinition();
    return def;
}

ColumnDefinition* PropertyDefinition::GetColumn() const
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

void PropertyDefinition::SetColumn(ColumnDefinition * columnDef)
{
    assert(!"NOT IMPLEMENTED");
}

///////////////////////////////////////////////////////////////////////////////
// FdoPhysicalElementMapping interface
///////////////////////////////////////////////////////////////////////////////

void PropertyDefinition::InitFromXml(FdoXmlSaxContext* xmlContext,
                                     FdoXmlAttributeCollection* xmlAttrs)
{
    FDOLOG_MARKER("PropertyDefinition::+InitFromXml");
}

void PropertyDefinition::_writeXml(FdoXmlWriter* xmlWriter, FdoXmlFlags const* xmlFlags)
{
    FDOLOG_MARKER("PropertyDefinition::+_writeXml");
}

///////////////////////////////////////////////////////////////////////////////
// FdoXmlSaxHandler interface
///////////////////////////////////////////////////////////////////////////////

FdoXmlSaxHandler* PropertyDefinition::XmlStartElement(FdoXmlSaxContext *saxContext,
                                                      FdoString* uri,
                                                      FdoString* name,
                                                      FdoString* qname,
                                                      FdoXmlAttributeCollection* xmlAtts)
{
    FDOLOG_MARKER("PropertyDefinition::+XmlStartElement");
    return NULL;
}

FdoBoolean PropertyDefinition::XmlEndElement(FdoXmlSaxContext* saxContext,
                                             FdoString* uri,
                                             FdoString* name,
                                             FdoString* qname)
{
    FDOLOG_MARKER("PropertyDefinition::+XmlEndElement");
    return NULL;
}

}}} // namespace fdo::postgis::ov
