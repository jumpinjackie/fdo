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
#include <Fdo/Filter/GeometricCondition.h>
#include "StringUtility.h"

// Constructs a default instance of a GeometricCondition.
FdoGeometricCondition::FdoGeometricCondition()
{
	m_geometricPropertyName = NULL;
}

// Constructs an instance of a GeometricCondition using the specified
// arguments.
FdoGeometricCondition::FdoGeometricCondition(FdoString* propertyName)
{
	m_geometricPropertyName = FdoIdentifier::Create(propertyName);
}

// Constructs an instance of a GeometricCondition using the specified
// arguments.
FdoGeometricCondition::FdoGeometricCondition(FdoIdentifier* propertyName)
{
	m_geometricPropertyName = FDO_SAFE_ADDREF(propertyName);
}

FdoGeometricCondition::~FdoGeometricCondition()
{
    FDO_SAFE_RELEASE(m_geometricPropertyName);
}

// Gets the name of the geometric property.
FdoIdentifier* FdoGeometricCondition::GetPropertyName()
{
    return FDO_SAFE_ADDREF(m_geometricPropertyName);
}

// Sets the name of the geometric property.
void FdoGeometricCondition::SetPropertyName(FdoIdentifier* value)
{
    FDO_SAFE_RELEASE(m_geometricPropertyName);
	m_geometricPropertyName = FDO_SAFE_ADDREF(value);
}

