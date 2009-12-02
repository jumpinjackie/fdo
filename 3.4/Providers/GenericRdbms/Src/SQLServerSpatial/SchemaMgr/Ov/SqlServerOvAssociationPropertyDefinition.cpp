/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvAssociationPropertyDefinition.h>

FdoSqlServerOvAssociationPropertyDefinition* FdoSqlServerOvAssociationPropertyDefinition::Create()
{
    return new FdoSqlServerOvAssociationPropertyDefinition();
}

FdoSqlServerOvAssociationPropertyDefinition* FdoSqlServerOvAssociationPropertyDefinition::Create(FdoString* name)
{
    return new FdoSqlServerOvAssociationPropertyDefinition(name);
}

FdoSqlServerOvAssociationPropertyDefinition::FdoSqlServerOvAssociationPropertyDefinition()
{
    Init();
}

FdoSqlServerOvAssociationPropertyDefinition::FdoSqlServerOvAssociationPropertyDefinition(FdoString* name) :
    FdoRdbmsOvAssociationPropertyDefinition(name),
    FdoSqlServerOvPropertyDefinition(name)
{
    Init();
}

FdoSqlServerOvAssociationPropertyDefinition::~FdoSqlServerOvAssociationPropertyDefinition()
{
}

FdoSqlServerOvPropertyDefinitionCollection *FdoSqlServerOvAssociationPropertyDefinition::GetIdentityProperties()
{
    return FDO_SAFE_ADDREF( (FdoSqlServerOvPropertyDefinitionCollection*) mPropertyDefinitions );
}

void FdoSqlServerOvAssociationPropertyDefinition::Dispose()
{
    delete this;
}

void FdoSqlServerOvAssociationPropertyDefinition::Init()
{
    mPropertyDefinitions = FdoSqlServerOvPropertyDefinitionCollection::Create(
        FdoRdbmsOvPropertiesP(
            FdoRdbmsOvAssociationPropertyDefinition::GetRdbmsProperties()
        )
    );
}
