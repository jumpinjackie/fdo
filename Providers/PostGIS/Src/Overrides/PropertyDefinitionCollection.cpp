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
#include <PostGIS/Override/PropertyDefinitionCollection.h>
#include <PostGIS/Override/PropertyDefinition.h>

#include <cassert>

namespace fdo { namespace postgis { namespace ov {

PropertyDefinitionCollection::PropertyDefinitionCollection()
{
}

PropertyDefinitionCollection::PropertyDefinitionCollection(FdoPhysicalElementMapping* parent)
    : Base(parent)
{
}

PropertyDefinitionCollection::~PropertyDefinitionCollection()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void PropertyDefinitionCollection::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// PropertyDefinitionCollection interface
///////////////////////////////////////////////////////////////////////////////

PropertyDefinitionCollection* PropertyDefinitionCollection::Create()
{
    PropertyDefinitionCollection* pdc = new PropertyDefinitionCollection();
    return pdc;
}

PropertyDefinitionCollection* PropertyDefinitionCollection::Create(FdoPhysicalElementMapping* parent)
{
    PropertyDefinitionCollection* pdc = new PropertyDefinitionCollection(parent);
    return pdc;
}

}}} // namespace fdo::postgis::ov
