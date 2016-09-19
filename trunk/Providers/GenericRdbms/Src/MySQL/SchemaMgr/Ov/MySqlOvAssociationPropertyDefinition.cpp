/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "stdafx.h"
#include <Rdbms/Override/MySQL/MySqlOvAssociationPropertyDefinition.h>

FdoMySQLOvAssociationPropertyDefinition* FdoMySQLOvAssociationPropertyDefinition::Create()
{
    return new FdoMySQLOvAssociationPropertyDefinition();
}

FdoMySQLOvAssociationPropertyDefinition* FdoMySQLOvAssociationPropertyDefinition::Create(FdoString* name)
{
    return new FdoMySQLOvAssociationPropertyDefinition(name);
}

FdoMySQLOvAssociationPropertyDefinition::FdoMySQLOvAssociationPropertyDefinition()
{
    Init();
}

FdoMySQLOvAssociationPropertyDefinition::FdoMySQLOvAssociationPropertyDefinition(FdoString* name) :
    FdoRdbmsOvAssociationPropertyDefinition(name),
    FdoMySQLOvPropertyDefinition(name)
{
    Init();
}

FdoMySQLOvAssociationPropertyDefinition::~FdoMySQLOvAssociationPropertyDefinition()
{
}

FdoMySQLOvPropertyDefinitionCollection *FdoMySQLOvAssociationPropertyDefinition::GetIdentityProperties()
{
    return FDO_SAFE_ADDREF( (FdoMySQLOvPropertyDefinitionCollection*) mPropertyDefinitions );
}

void FdoMySQLOvAssociationPropertyDefinition::Dispose()
{
    delete this;
}

void FdoMySQLOvAssociationPropertyDefinition::Init()
{
    mPropertyDefinitions = FdoMySQLOvPropertyDefinitionCollection::Create(
        FdoRdbmsOvPropertiesP(
            FdoRdbmsOvAssociationPropertyDefinition::GetRdbmsProperties()
        )
    );
}
