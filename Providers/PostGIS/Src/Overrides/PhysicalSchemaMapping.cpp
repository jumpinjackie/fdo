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
#include <PostGIS/Override/PhysicalSchemaMapping.h>
#include "../Provider/PostGisProvider.h"

#include <cassert>

namespace fdo { namespace postgis { namespace ov {

PhysicalSchemaMapping::PhysicalSchemaMapping() : mClasses(NULL)
{
    mClasses = ClassCollection::Create(this);
}

PhysicalSchemaMapping::~PhysicalSchemaMapping()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void PhysicalSchemaMapping::Dispose()
{
    delete this;
}

PhysicalSchemaMapping* PhysicalSchemaMapping::Create()
{
    PhysicalSchemaMapping* psm = new PhysicalSchemaMapping();
    return psm;
}

FdoString* PhysicalSchemaMapping::GetProvider()
{
    // TODO: Replace hardcoded literal with common definition
    // fdo::postgis::ProviderName
    return L"OSGeo.PostGIS.3.2";
}

ClassCollection* PhysicalSchemaMapping::GetClasses() const
{
    FDO_SAFE_ADDREF(mClasses.p);
    return mClasses.p;
}

ClassDefinition* PhysicalSchemaMapping::FindByClassName(FdoStringP const& name)
{
    // TODO: do case-insensitive compare on WIN32 platform,
    //       case-sensitive on other platforms

    FdoInt32 const size = mClasses->GetCount();
    for (FdoInt32 i = 0; i < size; ++i)
    {
        FdoPtr<ClassDefinition> classDef(mClasses->GetItem(i));
        if (name == classDef->GetName())
        {
            FDO_SAFE_ADDREF(classDef.p);
            return classDef.p;
        }
    }
    
    return 0;
}

}}} // namespace fdo::postgis::ov
