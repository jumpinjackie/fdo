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

#include "PostGisProvider.h"
#include "ListDataStores.h"

#include <cassert>

namespace fdo { namespace postgis {

ListDataStores::ListDataStores()
{
    assert(!"NOT IMPLEMENTED");
}

ListDataStores::~ListDataStores()
{
    assert(!"NOT IMPLEMENTED");
}

bool ListDataStores::GetIncludeNonFdoEnabledDatastores()
{
    return mIncludeNonFdo;
}

void ListDataStores::SetIncludeNonFdoEnabledDatastores(bool includeNonFdo)
{
    mIncludeNonFdo = includeNonFdo;
}

FdoIDataStoreReader* ListDataStores::Execute()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

}} // namespace fdo::postgis
