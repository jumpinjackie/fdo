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
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvDataPropertyDefinition.h>

FdoSqlServerOvDataPropertyDefinition* FdoSqlServerOvDataPropertyDefinition::Create()
{
    return new FdoSqlServerOvDataPropertyDefinition();
}

FdoSqlServerOvDataPropertyDefinition* FdoSqlServerOvDataPropertyDefinition::Create(FdoString* name)
{
    return new FdoSqlServerOvDataPropertyDefinition(name);
}


FdoSqlServerOvDataPropertyDefinition::FdoSqlServerOvDataPropertyDefinition()
{
}

FdoSqlServerOvDataPropertyDefinition::FdoSqlServerOvDataPropertyDefinition(FdoString* name) :
    FdoRdbmsOvPropertyDefinition(name),
    FdoRdbmsOvDataPropertyDefinition(name),
    FdoSqlServerOvPropertyDefinition(name)
{
}

FdoSqlServerOvDataPropertyDefinition::~FdoSqlServerOvDataPropertyDefinition()
{
}

FdoSqlServerOvColumn* FdoSqlServerOvDataPropertyDefinition::GetColumn()
{
    return (FdoSqlServerOvColumn*) FdoRdbmsOvDataPropertyDefinition::GetColumn();
}

void FdoSqlServerOvDataPropertyDefinition::SetColumn(FdoSqlServerOvColumn *column)
{
    FdoRdbmsOvDataPropertyDefinition::SetColumn(column);
}

void FdoSqlServerOvDataPropertyDefinition::Dispose()
{
    delete this;
}


/*  //TODO: enable this code when FdoSQLServerOvValidPropertyValueConstraint is ready

FdoSQLServerOvValidPropertyValueConstraint* FdoSqlServerOvDataPropertyDefinition::GetValidPropertyValueConstraint()
{
    return (FdoSQLServerOvValidPropertyValueConstraint*) FdoRdbmsOvDataPropertyDefinition::GetValidPropertyValueConstraint();
}

void FdoSqlServerOvDataPropertyDefinition::SetValidPropertyValueConstraint(FdoSQLServerOvValidPropertyValueConstraint* validPropertyValueConstraint)
{
    FdoRdbmsOvDataPropertyDefinition::SetValidPropertyValueConstraint(validPropertyValueConstraint);
}

*/
