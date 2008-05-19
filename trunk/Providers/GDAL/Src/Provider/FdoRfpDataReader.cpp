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
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpDataReader.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#include "FDORFP.h"
#include "FdoRfpDataReader.h"

FdoRfpDataReader::FdoRfpDataReader(const FdoPtr<FdoRfpQueryResult>& queryResult) :
											FdoRfpCommonReader<FdoIDataReader>(queryResult)
{
}

FdoRfpDataReader* FdoRfpDataReader::Create(const FdoPtr<FdoRfpQueryResult>& queryResult)
{
	FdoRfpDataReaderP fr = new FdoRfpDataReader(queryResult);

	return FDO_SAFE_ADDREF(fr.p);
}

/// <summary>Gets the number of propertys in the result set.</summary>
/// <returns>Returns the number of propertys.</returns> 
FdoInt32 FdoRfpDataReader::GetPropertyCount()
{
	return (FdoInt32)m_queryResult->identifiers.size();	
}

/// <summary>Gets the name of the property at the given ordinal position.</summary>
/// <param name="index">Input the position of the property.</param> 
/// <returns>Returns the property name</returns> 
FdoString* FdoRfpDataReader::GetPropertyName(FdoInt32 index)
{
	FdoString* propertyName;
	int numColumns = (int)m_queryResult->identifiers.size();
	if (index >= numColumns)
		throw FdoCommandException::Create(NlsMsgGet(GRFP_80_INDEX_OUT_OF_RANGE, "Index out of range."));
	propertyName = (*(m_queryResult->identifiers[index]))[0];
	return propertyName;
}

/// <summary>Gets the data type of the property with the specified name.</summary>
/// <param name="propertyName">Input the property name.</param> 
/// <returns>Returns the type of the property.</returns> 
FdoDataType FdoRfpDataReader::GetDataType(FdoString* propertyName)
{
	int numColumns = (int)m_queryResult->identifiers.size();
	for (int col = 0; col < numColumns; col++)
	{
		std::vector<FdoStringP>& ids = *m_queryResult->identifiers[col];
		for (std::vector<FdoStringP>::iterator it = ids.begin(); it != ids.end(); it++)
			if (STRCASEEQ(propertyName, *it))
				return m_queryResult->propertyTypes[col] == PropertyType_ID ? FdoDataType_String :
																				FdoDataType_BLOB;
	}
	// not found, throw out exception
	throw FdoCommandException::Create(NlsMsgGet1(GRFP_56_ILLEGAL_REQUESTED_PROPERTY, "Illegal requested property '%1$ls'.", propertyName));
	return FdoDataType_BLOB;
}

/// <summary>Gets the FDO property type of a given property. This is used
//  to indicate if a given property is a geometric property or a data property. If the property is
//  a FdoPropertyType_Data, then GetDataType can be used to to find the data type of the property.</summary>
/// <param name="propertyName">Input the property name.</param> 
/// <returns>Returns the FDO property type.</returns> 
FdoPropertyType FdoRfpDataReader::GetPropertyType(FdoString* propertyName)
{
	int numColumns = (int)m_queryResult->identifiers.size();
	for (int col = 0; col < numColumns; col++)
	{
		std::vector<FdoStringP>& ids = *m_queryResult->identifiers[col];
		for (std::vector<FdoStringP>::iterator it = ids.begin(); it != ids.end(); it++)
			if (STRCASEEQ(propertyName, *it))
				return m_queryResult->propertyTypes[col] == PropertyType_ID ? FdoPropertyType_DataProperty :
																				FdoPropertyType_RasterProperty;
	}
	// not found, throw out exception
	throw FdoCommandException::Create(NlsMsgGet1(GRFP_56_ILLEGAL_REQUESTED_PROPERTY, "Illegal requested property '%1$ls'.", propertyName));
	return FdoPropertyType_RasterProperty;
}


