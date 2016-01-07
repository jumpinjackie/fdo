/*
 * Copyright (C) 2006  Haris Kurtagic
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
 */
#include "stdafx.h"
#include <c_KgInfOverPropertyDefinitionCollection.h>

c_KgInfOverPropertyDefinitionCollection* c_KgInfOverPropertyDefinitionCollection::Create()
{
    return c_KgInfOverPropertyDefinitionCollection::Create(NULL);
}

c_KgInfOverPropertyDefinitionCollection* c_KgInfOverPropertyDefinitionCollection::Create(FdoPhysicalElementMapping *Parent)
{
    c_KgInfOverPropertyDefinitionCollection* pObject = new c_KgInfOverPropertyDefinitionCollection(Parent);
    
    return pObject;
}

c_KgInfOverPropertyDefinitionCollection::c_KgInfOverPropertyDefinitionCollection()
{
}

c_KgInfOverPropertyDefinitionCollection::c_KgInfOverPropertyDefinitionCollection(FdoPhysicalElementMapping *Parent) :
    FdoPhysicalElementMappingCollection<c_KgInfOverPropertyDefinition>(Parent)
{
}

c_KgInfOverPropertyDefinitionCollection::~c_KgInfOverPropertyDefinitionCollection(void)
{
}

void c_KgInfOverPropertyDefinitionCollection::Dispose()
{
    delete this;
}

