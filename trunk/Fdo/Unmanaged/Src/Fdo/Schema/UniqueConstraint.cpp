//
// 
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
// 
//
#include <Fdo/Schema/UniqueConstraint.h>

FdoUniqueConstraint::FdoUniqueConstraint():m_properties( NULL )
{
}

FdoUniqueConstraint::~FdoUniqueConstraint()
{
	FDO_SAFE_RELEASE( m_properties );
}

    
///<summary>Constructs an empty instance of an FdoUniqueConstraint.</summary>
/// <returns>Returns an FdoUniqueConstraint.</returns> 
FdoUniqueConstraint* FdoUniqueConstraint::Create( )
{
	return new FdoUniqueConstraint();
}

///<summary>Returns the list of properties that define the unique constraint. The caller can modify the list by adding or removing properties</summary>
/// <returns>Returns the collection that holds the properties that defines the constraint.</returns> 
FdoDataPropertyDefinitionCollection* FdoUniqueConstraint::GetProperties()
{
	if( m_properties == NULL )
		m_properties =  FdoDataPropertyDefinitionCollection::Create( NULL );

	FDO_SAFE_ADDREF( m_properties );

	return m_properties;
}


