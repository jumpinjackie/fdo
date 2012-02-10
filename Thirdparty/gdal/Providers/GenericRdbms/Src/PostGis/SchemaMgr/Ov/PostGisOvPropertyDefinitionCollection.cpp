//
// Copyright (C) 2006 Refractions Research, Inc.
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
#include "stdafx.h"
#include <Rdbms/Override/PostGis/PostGisOvPropertyDefinitionCollection.h>

FdoPostGISOvPropertyDefinitionCollection* FdoPostGISOvPropertyDefinitionCollection::Create()
{
    return (new FdoPostGISOvPropertyDefinitionCollection());
}

FdoPostGISOvPropertyDefinitionCollection* FdoPostGISOvPropertyDefinitionCollection::Create(
    FdoRdbmsOvPropertyDefinitionCollection* base)
{
    return (new FdoPostGISOvPropertyDefinitionCollection(base));
}

FdoPostGISOvPropertyDefinitionCollection::FdoPostGISOvPropertyDefinitionCollection()
{
    // idle
}

FdoPostGISOvPropertyDefinitionCollection::FdoPostGISOvPropertyDefinitionCollection(
    FdoRdbmsOvPropertyDefinitionCollection* base)
    : FdoPostGISOvPropertyDefinitionCollectionBase(base)
{
    // idle
}

FdoPostGISOvPropertyDefinitionCollection::~FdoPostGISOvPropertyDefinitionCollection()
{
    // idle
}

void FdoPostGISOvPropertyDefinitionCollection::Dispose()
{
    delete this;
}

FdoPostGISOvPropertyDefinition* FdoPostGISOvPropertyDefinitionCollection::DownCast(
    FdoRdbmsOvPropertyDefinition* value) const
{
    return dynamic_cast<FdoPostGISOvPropertyDefinition*>(value);
}
