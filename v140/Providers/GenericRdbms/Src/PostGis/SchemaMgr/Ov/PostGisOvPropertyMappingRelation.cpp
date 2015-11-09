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
#include <Rdbms/Override/PostGis/PostGisOvPropertyMappingRelation.h>
#include <Rdbms/Override/PostGis/PostGisOvPropertyMappingClass.h>
#include <Rdbms/Override/PostGis/PostGisOvPropertyMappingConcrete.h>
#include <Rdbms/Override/PostGis/PostGisOvPropertyMappingSingle.h>
#include <Rdbms/Override/PostGis/PostGisOvObjectPropertyDefinition.h>
#include <Rdbms/Override/PostGis/PostGisOvClassDefinition.h>

FdoPostGISOvPropertyMappingRelation::FdoPostGISOvPropertyMappingRelation() 
{
    // idle
}

FdoPostGISOvPropertyMappingRelation::~FdoPostGISOvPropertyMappingRelation()
{
    // idle
}

FdoPostGISOvClassDefinition* FdoPostGISOvPropertyMappingRelation::GetInternalClass()
{
    FdoRdbmsOvClassDefinition* classDef = NULL;
    classDef = FdoRdbmsOvPropertyMappingRelation::GetInternalClass();

    return (static_cast<FdoPostGISOvClassDefinition*>(classDef));
}

void FdoPostGISOvPropertyMappingRelation::SetInternalClass(
    FdoPostGISOvClassDefinition* classDef)
{
    FdoRdbmsOvPropertyMappingRelation::SetInternalClass(classDef);
}

FdoRdbmsOvClassDefinition* FdoPostGISOvPropertyMappingRelation::CreateInternalClass(
    bool attach)
{
    FdoPostGISOvClassP internalClass(FdoPostGISOvClassDefinition::Create());

    if (attach)
    {
        SetInternalClass(internalClass);
    }

    FDO_SAFE_ADDREF(internalClass.p);
    return internalClass.p;
}
