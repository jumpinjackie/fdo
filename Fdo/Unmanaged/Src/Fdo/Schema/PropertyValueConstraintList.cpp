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


#include <Fdo/Schema/PropertyValueConstraintList.h>

// Constructs a default instance of a FdoPropertyValueConstraintList.
FdoPropertyValueConstraintList::FdoPropertyValueConstraintList():m_constraintList( NULL )
{

}

FdoPropertyValueConstraintList::~FdoPropertyValueConstraintList()
{
	FDO_SAFE_RELEASE( m_constraintList );
}

	
///<summary>Constructs an empty instance of an FdoPropertyValueConstraintList.</summary>
/// <returns>Returns an FdoPropertyValueConstraintList</returns> 
FdoPropertyValueConstraintList* FdoPropertyValueConstraintList::Create( )
{
	return new FdoPropertyValueConstraintList();
}

///<summary>Returns FdoPropertyValueConstraintType_Range type.</summary>
///<returns>Returns the constraint type</returns>
FdoPropertyValueConstraintType FdoPropertyValueConstraintList::GetConstraintType()
{
	return FdoPropertyValueConstraintType_List;
}

///<summary>Returns the list of allowed values for a particular property.</summary>
/// <returns>Returns na instance of FdoDataValueCollection.</returns> 
FdoDataValueCollection* FdoPropertyValueConstraintList::GetConstraintList()
{
	if( m_constraintList == NULL )
		m_constraintList =  FdoDataValueCollection::Create();

	FDO_SAFE_ADDREF( m_constraintList );

	return m_constraintList;
}


