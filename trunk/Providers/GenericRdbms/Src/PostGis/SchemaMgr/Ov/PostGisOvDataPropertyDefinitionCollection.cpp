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
#include <Rdbms/Override/PostGis/PostGisOvDataPropertyDefinitionCollection.h>

FdoPostGISOvDataPropertyDefinitionCollection* FdoPostGISOvDataPropertyDefinitionCollection::Create()
{
    return (new FdoPostGISOvDataPropertyDefinitionCollection());
}

FdoPostGISOvDataPropertyDefinitionCollection* FdoPostGISOvDataPropertyDefinitionCollection::Create(
    FdoPhysicalElementMapping* parent)
{
    return (new FdoPostGISOvDataPropertyDefinitionCollection(parent));
}

FdoPostGISOvDataPropertyDefinitionCollection::FdoPostGISOvDataPropertyDefinitionCollection()
{
    // idle
}

FdoPostGISOvDataPropertyDefinitionCollection::FdoPostGISOvDataPropertyDefinitionCollection(
    FdoPhysicalElementMapping* parent)
    : FdoPostGISOvDataPropertyDefinitionCollectionBase(parent)
{
    // idle
}

FdoPostGISOvDataPropertyDefinitionCollection::~FdoPostGISOvDataPropertyDefinitionCollection()
{
    // idle
}

void FdoPostGISOvDataPropertyDefinitionCollection::Dispose()
{
    delete this;
}
