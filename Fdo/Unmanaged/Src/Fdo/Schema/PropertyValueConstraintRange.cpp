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

#include <Fdo/Schema/PropertyValueConstraintRange.h>


// Constructs a default instance of a FdoPropertyValueConstraintRange.
FdoPropertyValueConstraintRange::FdoPropertyValueConstraintRange():
m_minValue( NULL ),
m_maxValue( NULL ),
m_isMinInclusive( false ),
m_isMaxInclusive( false )
{
}

// Constructs an instance of a FdoPropertyValueConstraintRange using the specified
// arguments.
FdoPropertyValueConstraintRange::FdoPropertyValueConstraintRange(FdoDataValue  *minValue, FdoDataValue* maxValue):
m_minValue( minValue ),
m_maxValue( maxValue ),
m_isMinInclusive( false ),
m_isMaxInclusive( false )
{
	FDO_SAFE_ADDREF( m_minValue );
	FDO_SAFE_ADDREF( m_maxValue );
}

FdoPropertyValueConstraintRange::~FdoPropertyValueConstraintRange()
{
	FDO_SAFE_RELEASE( m_minValue );
	FDO_SAFE_RELEASE( m_maxValue );
}


///<summary>Constructs an empty instance of an FdoPropertyValueConstraintRange.</summary>
/// <returns>Returns an FdoPropertyValueConstraintRange</returns> 
FdoPropertyValueConstraintRange* FdoPropertyValueConstraintRange::Create( )
{
	return new FdoPropertyValueConstraintRange( );
}

///<summary>Constructs and populates an instance of an FdoPropertyValueConstraintRange.</summary>
/// <param name="minValue">Minimum allowed value</param> 
/// <param name="maxValue">Maximum allowed value</param> 
/// <returns>Returns an FdoPropertyValueConstraintRange</returns> 
FdoPropertyValueConstraintRange* FdoPropertyValueConstraintRange::Create( FdoDataValue  *minValue, FdoDataValue* maxValue )
{
	return new FdoPropertyValueConstraintRange( minValue, maxValue );
}

///<summary>Returns FdoPropertyValueConstraintType_Range type.</summary>
///<returns>Returns the constraint type</returns>
FdoPropertyValueConstraintType FdoPropertyValueConstraintRange::GetConstraintType()
{
	return FdoPropertyValueConstraintType_Range;
}

///<summary>Get the minimum allowed value. The type of this is the same as the type of the property. 
///E.g. if the property is int32, then this value is int32.</summary>
/// <returns>Returns the minimum value</returns> 
FdoDataValue* FdoPropertyValueConstraintRange::GetMinValue()
{
	FDO_SAFE_ADDREF( m_minValue );
	return m_minValue;
}

///<summary>Set the minimum allowed value.</summary>
/// <param name="value">Minimum allowed value</param> 
/// <returns>Returns nothing</returns> 
void FdoPropertyValueConstraintRange::SetMinValue(FdoDataValue* value)
{
	FDO_SAFE_RELEASE( m_minValue );
	m_minValue = value;
	FDO_SAFE_ADDREF( m_minValue );
}

///<summary>Returns a bool to indicate if the minimum value is inclusive or exclusive. This is the difference between >= and >. 
///This is a boolean type where true means inclusive.</summary>
/// <returns>Returns true if the value is inclusive. false otherwise</returns> 
bool FdoPropertyValueConstraintRange::GetMinInclusive()
{
	return m_isMinInclusive;
}

///<summary>Set the minimum value to inclusive or exclusive. This is the difference between >= and >. 
/// <param name="value">This is a boolean type where true means inclusive.</summary>
/// <returns>Returns nothing</returns> 
void FdoPropertyValueConstraintRange::SetMinInclusive(bool value)
{
	m_isMinInclusive = value;
}

///<summary>Get the maximum allowed value. The type of this is the same as the type of the property. 
///E.g. if the property is int32, then this value is int32.</summary>
/// <returns>Returns the maximum value</returns> 
FdoDataValue* FdoPropertyValueConstraintRange::GetMaxValue()
{
	FDO_SAFE_ADDREF( m_maxValue );
	return m_maxValue;
}

///<summary>Set the maximum allowed value.</summary>
/// <param name="value">Maximum allowed value</param> 
/// <returns>Returns nothing</returns> 
void FdoPropertyValueConstraintRange::SetMaxValue(FdoDataValue* value)
{
	FDO_SAFE_RELEASE( m_maxValue );
	m_maxValue = value;
	FDO_SAFE_ADDREF( m_maxValue );
}

///<summary>Returns a bool to indicate if the maximum value is inclusive or exclusive. This is the difference between <= and <. 
///This is a boolean type where true means inclusive.</summary>
/// <returns>Returns true if the value is inclusive. false otherwise</returns> 
bool FdoPropertyValueConstraintRange::GetMaxInclusive()
{
	return m_isMaxInclusive;
}

///<summary>Returns a bool to indicate if the maximum value is inclusive or exclusive. This is the difference between <= and <. 
///This is a boolean type where true means inclusive.</summary>
/// <returns>Returns true if the value is inclusive. false otherwise</returns>
void FdoPropertyValueConstraintRange::SetMaxInclusive(bool value)
{
	m_isMaxInclusive = value;
}

