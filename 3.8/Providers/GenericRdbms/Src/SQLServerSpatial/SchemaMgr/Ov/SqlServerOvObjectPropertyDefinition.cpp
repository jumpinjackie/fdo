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

#include <Rdbms/Override/SQLServerSpatial/SqlServerOvObjectPropertyDefinition.h>

FdoSqlServerOvObjectPropertyDefinition* FdoSqlServerOvObjectPropertyDefinition::Create()
{
    return new FdoSqlServerOvObjectPropertyDefinition();
}

FdoSqlServerOvObjectPropertyDefinition* FdoSqlServerOvObjectPropertyDefinition::Create(FdoString* name)
{
    return new FdoSqlServerOvObjectPropertyDefinition(name);
}

FdoSqlServerOvObjectPropertyDefinition::FdoSqlServerOvObjectPropertyDefinition()
{
}

FdoSqlServerOvObjectPropertyDefinition::FdoSqlServerOvObjectPropertyDefinition(FdoString* name) :
    FdoRdbmsOvPropertyDefinition(name),
    FdoRdbmsOvObjectPropertyDefinition(name),
    FdoSqlServerOvPropertyDefinition(name)
{
}

FdoSqlServerOvObjectPropertyDefinition::~FdoSqlServerOvObjectPropertyDefinition()
{
}

FdoSqlServerOvPropertyMappingDefinition* FdoSqlServerOvObjectPropertyDefinition::GetMappingDefinition()
{
    return dynamic_cast<FdoSqlServerOvPropertyMappingDefinition*>(FdoRdbmsOvObjectPropertyDefinition::GetMappingDefinition());
}

void FdoSqlServerOvObjectPropertyDefinition::SetMappingDefinition(FdoSqlServerOvPropertyMappingDefinition* propertyMappingDefinition)
{
    FdoRdbmsOvObjectPropertyDefinition::SetMappingDefinition( propertyMappingDefinition );
}

void FdoSqlServerOvObjectPropertyDefinition::Dispose()
{
    delete this;
}

FdoRdbmsOvPropertyMappingSingle* FdoSqlServerOvObjectPropertyDefinition::CreateSingleMapping(
    bool attach
)
{
    FdoSqlServerOvPropertyMappingSingleP pMapping = FdoSqlServerOvPropertyMappingSingle::Create();
    if ( attach ) 
        SetMappingDefinition( pMapping );

    return FDO_SAFE_ADDREF(dynamic_cast<FdoRdbmsOvPropertyMappingSingle*>(pMapping.p));
}

FdoRdbmsOvPropertyMappingClass* FdoSqlServerOvObjectPropertyDefinition::CreateClassMapping(
    bool attach
)
{
    FdoSqlServerOvPropertyMappingClassP pMapping = FdoSqlServerOvPropertyMappingClass::Create();
    if ( attach ) 
        SetMappingDefinition( pMapping );

    return FDO_SAFE_ADDREF(dynamic_cast<FdoRdbmsOvPropertyMappingClass*>(pMapping.p));
}

FdoRdbmsOvPropertyMappingConcrete* FdoSqlServerOvObjectPropertyDefinition::CreateConcreteMapping(
    bool attach
)
{
    FdoSqlServerOvPropertyMappingConcreteP pMapping = FdoSqlServerOvPropertyMappingConcrete::Create();
    if ( attach ) 
        SetMappingDefinition( pMapping );

    return FDO_SAFE_ADDREF(dynamic_cast<FdoRdbmsOvPropertyMappingConcrete*>(pMapping.p));
}

