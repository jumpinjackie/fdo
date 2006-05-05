/*
 * 
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
#include "ShpFilterExecutor.h"

ShpFilterExecutor::ShpFilterExecutor (FdoIReader* reader, FdoIdentifierCollection* compIdents) :
    FdoCommonFilterExecutor (reader, compIdents)
{
    ShpFeatureReader* my_reader;
    FdoPtr<ShpConnection> connection;
    FdoPtr<FdoClassDefinition> definition;

    my_reader = (ShpFeatureReader*)m_reader;
    connection = my_reader->GetConnection ();
    definition = ShpSchemaUtilities::GetLogicalClassDefinition(connection, my_reader->GetClassName(), NULL);
    mProperties = definition->GetProperties ();
}

ShpFilterExecutor::~ShpFilterExecutor (void)
{
}

ShpFilterExecutor* ShpFilterExecutor::Create (FdoIReader* reader, FdoIdentifierCollection* selected)
{    
    return new ShpFilterExecutor (reader, selected);
}

FdoPropertyDefinition* ShpFilterExecutor::GetPropInfo (FdoString* name)
{
    return (mProperties->FindItem (name));
}


