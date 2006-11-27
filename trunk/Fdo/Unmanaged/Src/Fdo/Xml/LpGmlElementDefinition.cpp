// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  

#include <Fdo.h>
#include "LpGmlElementDefinition.h"
#include "LpClassDefinition.h"

FdoXmlLpGmlElementDefinition::FdoXmlLpGmlElementDefinition( FdoXmlLpClassDefinition* classDefinition, FdoXmlElementMapping* mapping ) :
m_classDef(classDefinition), m_elementMapping(mapping) {
    FDO_SAFE_ADDREF(m_classDef.p);
    FDO_SAFE_ADDREF(m_elementMapping.p);
}

FdoXmlLpGmlElementDefinition::~FdoXmlLpGmlElementDefinition() {
}

FdoXmlLpGmlElementDefinition* FdoXmlLpGmlElementDefinition::Create( 
    FdoXmlLpClassDefinition* classDefinition,    // can be NULL
    FdoXmlElementMapping* mapping           // can be NULL
    ) {
    return new FdoXmlLpGmlElementDefinition(classDefinition, mapping);
}

// returns the LogicalPhysical class definition.
// NULL if not available
FdoXmlLpClassDefinition* FdoXmlLpGmlElementDefinition::GetClassDefinition() const {
    return FDO_SAFE_ADDREF(m_classDef.p);
}

FdoString* FdoXmlLpGmlElementDefinition::GetName() {
    return m_elementMapping->GetName();
}

// returns the GML element mapping.
// NULL if not available
FdoXmlElementMapping* FdoXmlLpGmlElementDefinition::GetElementMapping() const {
    return FDO_SAFE_ADDREF(m_elementMapping.p);
}

FdoXmlLpGmlElementCollection* FdoXmlLpGmlElementCollection::Create(FdoXmlLpSchemaElement* parent) {
    return new FdoXmlLpGmlElementCollection(parent);
}

