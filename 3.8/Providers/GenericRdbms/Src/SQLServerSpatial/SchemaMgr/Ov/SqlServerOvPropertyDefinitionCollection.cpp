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
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPropertyDefinitionCollection.h>

FdoSqlServerOvPropertyDefinitionCollection* FdoSqlServerOvPropertyDefinitionCollection::Create()
{
    return new FdoSqlServerOvPropertyDefinitionCollection();
}

FdoSqlServerOvPropertyDefinitionCollection* FdoSqlServerOvPropertyDefinitionCollection::Create(FdoRdbmsOvPropertyDefinitionCollection* baseCollection)
{
    return new FdoSqlServerOvPropertyDefinitionCollection(baseCollection);
}

FdoSqlServerOvPropertyDefinitionCollection::FdoSqlServerOvPropertyDefinitionCollection()
{
}

FdoSqlServerOvPropertyDefinitionCollection::FdoSqlServerOvPropertyDefinitionCollection(FdoRdbmsOvPropertyDefinitionCollection* baseCollection) : 
    FdoRestrictedNamedCollection<FdoRdbmsOvPropertyDefinition,FdoSqlServerOvPropertyDefinition,FdoRdbmsOvPropertyDefinitionCollection >(
        baseCollection
    )
{
}

FdoSqlServerOvPropertyDefinitionCollection::~FdoSqlServerOvPropertyDefinitionCollection()
{
}

void FdoSqlServerOvPropertyDefinitionCollection::Dispose()
{
    delete this;
}

FdoSqlServerOvPropertyDefinition* FdoSqlServerOvPropertyDefinitionCollection::DownCast( FdoRdbmsOvPropertyDefinition* value ) const
{
    return dynamic_cast<FdoSqlServerOvPropertyDefinition*>(value);
}
