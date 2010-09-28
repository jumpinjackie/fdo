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

#include <Rdbms/Override/PostGis/PostGisOvObjectPropertyDefinition.h>

FdoPostGISOvObjectPropertyDefinition* FdoPostGISOvObjectPropertyDefinition::Create()
{
    return (new FdoPostGISOvObjectPropertyDefinition());
}

FdoPostGISOvObjectPropertyDefinition* FdoPostGISOvObjectPropertyDefinition::Create(
    FdoString* name)
{
    return (new FdoPostGISOvObjectPropertyDefinition(name));
}

FdoPostGISOvObjectPropertyDefinition::FdoPostGISOvObjectPropertyDefinition()
{
    // idle
}

FdoPostGISOvObjectPropertyDefinition::FdoPostGISOvObjectPropertyDefinition(
    FdoString* name) : FdoRdbmsOvPropertyDefinition(name),
    FdoRdbmsOvObjectPropertyDefinition(name),
    FdoPostGISOvPropertyDefinition(name)
{
    // idle
}

FdoPostGISOvObjectPropertyDefinition::~FdoPostGISOvObjectPropertyDefinition()
{
    // idle
}

FdoPostGISOvPropertyMappingDefinition* FdoPostGISOvObjectPropertyDefinition::GetMappingDefinition()
{
    FdoRdbmsOvPropertyMappingDefinition* rdbmsDef = NULL;
    rdbmsDef = FdoRdbmsOvObjectPropertyDefinition::GetMappingDefinition();

    FdoPostGISOvPropertyMappingDefinition* postgisDef = NULL;
    postgisDef = dynamic_cast<FdoPostGISOvPropertyMappingDefinition*>(rdbmsDef);

    return postgisDef;
}

void FdoPostGISOvObjectPropertyDefinition::SetMappingDefinition(
    FdoPostGISOvPropertyMappingDefinition* mappingDef)
{
    FdoRdbmsOvObjectPropertyDefinition::SetMappingDefinition(mappingDef);
}

void FdoPostGISOvObjectPropertyDefinition::Dispose()
{
    delete this;
}

FdoRdbmsOvPropertyMappingSingle* FdoPostGISOvObjectPropertyDefinition::CreateSingleMapping(
    bool attach)
{
    FdoPostGISOvPropertyMappingSingleP mapping(FdoPostGISOvPropertyMappingSingle::Create());

    if (attach)
    {
        SetMappingDefinition(mapping);
    }

    FdoRdbmsOvPropertyMappingSingle* rdbmsMapping = NULL;
    rdbmsMapping = dynamic_cast<FdoRdbmsOvPropertyMappingSingle*>(mapping.p);
    
    FDO_SAFE_ADDREF(rdbmsMapping);
    return rdbmsMapping;
}

FdoRdbmsOvPropertyMappingClass* FdoPostGISOvObjectPropertyDefinition::CreateClassMapping(
    bool attach)
{
    FdoPostGISOvPropertyMappingClassP mapping(FdoPostGISOvPropertyMappingClass::Create());
    
    if (attach)
    {
        SetMappingDefinition(mapping);
    }

    FdoRdbmsOvPropertyMappingClass* rdbmsMapping = NULL;
    rdbmsMapping = dynamic_cast<FdoRdbmsOvPropertyMappingClass*>(mapping.p);
    
    FDO_SAFE_ADDREF(rdbmsMapping);
    return rdbmsMapping;
}

FdoRdbmsOvPropertyMappingConcrete* FdoPostGISOvObjectPropertyDefinition::CreateConcreteMapping(
    bool attach)
{
    FdoPostGISOvPropertyMappingConcreteP mapping(FdoPostGISOvPropertyMappingConcrete::Create());

    if (attach)
    {
        SetMappingDefinition(mapping);
    }

    FdoRdbmsOvPropertyMappingConcrete* rdbmsMapping = NULL;
    rdbmsMapping = dynamic_cast<FdoRdbmsOvPropertyMappingConcrete*>(mapping.p);
    
    FDO_SAFE_ADDREF(rdbmsMapping);
    return rdbmsMapping;
}

