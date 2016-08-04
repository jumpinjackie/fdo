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
#include <Rdbms/Override/MySQL/MySqlOvDataPropertyDefinition.h>

FdoMySQLOvDataPropertyDefinition* FdoMySQLOvDataPropertyDefinition::Create()
{
    return new FdoMySQLOvDataPropertyDefinition();
}

FdoMySQLOvDataPropertyDefinition* FdoMySQLOvDataPropertyDefinition::Create(FdoString* name)
{
    return new FdoMySQLOvDataPropertyDefinition(name);
}


FdoMySQLOvDataPropertyDefinition::FdoMySQLOvDataPropertyDefinition()
{
}

FdoMySQLOvDataPropertyDefinition::FdoMySQLOvDataPropertyDefinition(FdoString* name) :
    FdoRdbmsOvPropertyDefinition(name),
    FdoRdbmsOvDataPropertyDefinition(name),
    FdoMySQLOvPropertyDefinition(name)
{
}

FdoMySQLOvDataPropertyDefinition::~FdoMySQLOvDataPropertyDefinition()
{
}

FdoMySQLOvColumn* FdoMySQLOvDataPropertyDefinition::GetColumn()
{
    return (FdoMySQLOvColumn*) FdoRdbmsOvDataPropertyDefinition::GetColumn();
}

void FdoMySQLOvDataPropertyDefinition::SetColumn(FdoMySQLOvColumn *column)
{
    FdoRdbmsOvDataPropertyDefinition::SetColumn(column);
}

void FdoMySQLOvDataPropertyDefinition::Dispose()
{
    delete this;
}


/*  //TODO: enable this code when FdoMySQLOvValidPropertyValueConstraint is ready

FdoMySQLOvValidPropertyValueConstraint* FdoMySQLOvDataPropertyDefinition::GetValidPropertyValueConstraint()
{
    return (FdoMySQLOvValidPropertyValueConstraint*) FdoRdbmsOvDataPropertyDefinition::GetValidPropertyValueConstraint();
}

void FdoMySQLOvDataPropertyDefinition::SetValidPropertyValueConstraint(FdoMySQLOvValidPropertyValueConstraint* validPropertyValueConstraint)
{
    FdoRdbmsOvDataPropertyDefinition::SetValidPropertyValueConstraint(validPropertyValueConstraint);
}

*/
