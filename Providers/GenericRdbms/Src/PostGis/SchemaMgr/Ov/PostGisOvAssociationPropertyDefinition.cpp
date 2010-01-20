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
#include <Rdbms/Override/PostGis/PostGisOvAssociationPropertyDefinition.h>

FdoPostGISOvAssociationPropertyDefinition* FdoPostGISOvAssociationPropertyDefinition::Create()
{
    return (new FdoPostGISOvAssociationPropertyDefinition());
}

FdoPostGISOvAssociationPropertyDefinition* FdoPostGISOvAssociationPropertyDefinition::Create(
    FdoString* name)
{
    return (new FdoPostGISOvAssociationPropertyDefinition(name));
}

FdoPostGISOvAssociationPropertyDefinition::FdoPostGISOvAssociationPropertyDefinition()
{
    Init();
}

FdoPostGISOvAssociationPropertyDefinition::FdoPostGISOvAssociationPropertyDefinition(
    FdoString* name) : FdoRdbmsOvAssociationPropertyDefinition(name),
    FdoPostGISOvPropertyDefinition(name)
{
    Init();
}

FdoPostGISOvAssociationPropertyDefinition::~FdoPostGISOvAssociationPropertyDefinition()
{
    // idle
}

FdoPostGISOvPropertyDefinitionCollection* FdoPostGISOvAssociationPropertyDefinition::GetIdentityProperties()
{
    typedef FdoPostGISOvPropertyDefinitionCollection colletion_t;
    colletion_t* col = static_cast<colletion_t*>(mPropertyDefinitions);

    FDO_SAFE_ADDREF(col);
    return col;
}

void FdoPostGISOvAssociationPropertyDefinition::Dispose()
{
    delete this;
}

void FdoPostGISOvAssociationPropertyDefinition::Init()
{
    mPropertyDefinitions = FdoPostGISOvPropertyDefinitionCollection::Create(
        FdoRdbmsOvPropertiesP(
            FdoRdbmsOvAssociationPropertyDefinition::GetRdbmsProperties()
        )
    );
}
