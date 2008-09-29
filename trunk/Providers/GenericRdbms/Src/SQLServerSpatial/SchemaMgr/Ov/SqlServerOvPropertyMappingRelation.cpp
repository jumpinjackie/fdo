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
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPropertyMappingClass.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPropertyMappingConcrete.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPropertyMappingSingle.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvObjectPropertyDefinition.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvClassDefinition.h>

class FdoSqlServerOvClassDefinition;

FdoSqlServerOvPropertyMappingRelation::FdoSqlServerOvPropertyMappingRelation() 
{
}

FdoSqlServerOvPropertyMappingRelation::~FdoSqlServerOvPropertyMappingRelation()
{
}

FdoSqlServerOvClassDefinition* FdoSqlServerOvPropertyMappingRelation::GetInternalClass()
{
    return (FdoSqlServerOvClassDefinition*) FdoRdbmsOvPropertyMappingRelation::GetInternalClass();
}

void FdoSqlServerOvPropertyMappingRelation::SetInternalClass(FdoSqlServerOvClassDefinition* classDefinition)
{
    FdoRdbmsOvPropertyMappingRelation::SetInternalClass( classDefinition );
}

FdoRdbmsOvClassDefinition* FdoSqlServerOvPropertyMappingRelation::CreateInternalClass(
    bool attach
)
{
    FdoSqlServerOvClassP internalClass = FdoSqlServerOvClassDefinition::Create();
    if ( attach ) 
        SetInternalClass( internalClass );

    return FDO_SAFE_ADDREF(internalClass.p);
}
