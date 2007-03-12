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
#include "SchemaDescription.h"

namespace fdo { namespace postgis {

SchemaDescription::SchemaDescription()
    : mLogicalSchemas(NULL), mSchemaMapping(NULL), mSpatialContexts(NULL)
{
}

SchemaDescription::~SchemaDescription()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void SchemaDescription::Dispose()
{
    FDOLOG_MARKER("SchemaDescription::#Dispose");

    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// SchemaDescription interface
///////////////////////////////////////////////////////////////////////////////

SchemaDescription* SchemaDescription::Create()
{
    SchemaDescription::Ptr sd(new SchemaDescription());
    
    FDO_SAFE_ADDREF(sd.p);
    return sd.p;
}

void SchemaDescription::SetLogicalSchemas(FdoFeatureSchemaCollection* logicalSchemas)
{
    mLogicalSchemas = logicalSchemas;
    FDO_SAFE_ADDREF(mLogicalSchemas.p);
}

void SchemaDescription::SetSchemaMapping(ov::PhysicalSchemaMapping* phSchemaMapping)
{
    mSchemaMapping = phSchemaMapping;
    FDO_SAFE_ADDREF(mSchemaMapping.p);
}

void SchemaDescription::SetSpatialContexts(SpatialContextCollection* spContexts)
{
    mSpatialContexts = spContexts;
    FDO_SAFE_ADDREF(mSpatialContexts.p);
}

FdoFeatureSchemaCollection* SchemaDescription::GetLogicalSchemas() const
{
    FDO_SAFE_ADDREF(mLogicalSchemas.p);
    return mLogicalSchemas.p;
}

ov::PhysicalSchemaMapping* SchemaDescription::GetSchemaMapping() const
{
    FDO_SAFE_ADDREF(mSchemaMapping.p);
    return mSchemaMapping.p;
}

SpatialContextCollection* SchemaDescription::GetSpatialContexts() const
{
    FDO_SAFE_ADDREF(mSpatialContexts.p);
    return mSpatialContexts.p;
}

FdoClassDefinition* SchemaDescription::FindClassDefinition(FdoIdentifier* id)
{
    assert(NULL != id);
    
    FdoPtr<FdoFeatureSchemaCollection> logicalSchemas(GetLogicalSchemas());
    FdoPtr<FdoClassDefinition> classDef;

    if (NULL != logicalSchemas && logicalSchemas->GetCount() > 0)
    {
        FdoStringP name(id->GetText());
        FdoPtr<FdoIDisposableCollection> featClasses(logicalSchemas->FindClass(name));
        if (NULL != featClasses)
        {
            classDef = static_cast<FdoClassDefinition*>(featClasses->GetItem(0));
        }
    }

    FDO_SAFE_ADDREF(classDef.p);            
    return classDef.p;
}

ov::ClassDefinition* SchemaDescription::FindClassMapping(FdoIdentifier* id)
{
    assert(NULL != id);
    
    // TODO: What to do if none class found?
    return mSchemaMapping->FindByClassName(id->GetName());
}

}} // namespace fdo::postgis
