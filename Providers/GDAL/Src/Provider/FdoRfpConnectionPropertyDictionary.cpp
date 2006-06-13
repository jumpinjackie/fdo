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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpConnectionPropertyDictionary.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#include "FDORFP.h"
#include "FdoRfpConnectionPropertyDictionary.h"
#include "FdoRfpConnection.h"

FdoRfpConnectionPropertyDictionary::FdoRfpConnectionPropertyDictionary(FdoRfpConnection* connection) :
	m_connection(connection)
{
}
FdoRfpConnectionPropertyDictionary::~FdoRfpConnectionPropertyDictionary()
{
}
void FdoRfpConnectionPropertyDictionary::Dispose()
{
	delete this;
}

/// <summary> Gets the names of all the properties that can appear in a connection string
/// for this feature provider as an array of Strings. The order of the property
/// names in the resulting array dictate the order in which they need to be 
/// specified. This is especially important for the success of the 
/// EnumeratePropertyValues method because properties that occur earlier in the array
/// may be required for successful enumeration of properties that appear later.</summary>
/// <param name="count">Output the number of parameters</param> 
/// <returns>Returns the list of parameter names</returns> 
FdoString** FdoRfpConnectionPropertyDictionary::GetPropertyNames(FdoInt32& count)
{
	return m_connection->GetPropertyNames(count);
}

/// <summary>Gets the value of the specified property.</summary>
/// <param name="name">Input the property name.</param> 
/// <returns>Returns the property value.</returns> 
FdoString* FdoRfpConnectionPropertyDictionary::GetProperty(FdoString* name)
{
    if (name == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));

	return m_connection->GetProperty(name);
}

/// <summary>Sets the value of the specified property. An exception is thrown if the connection is currently open.</summary>
/// <param name="name">Input the property name</param> 
/// <param name="value">Input the property value</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpConnectionPropertyDictionary::SetProperty(FdoString* name, FdoString* value)
{
    if (name == NULL || value == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));

	m_connection->SetProperty(name, value);
}

/// <summary>Gets the default value for the specified property.</summary>
/// <param name="name">Input the property name</param> 
/// <returns>Returns the property default value</returns> 
FdoString* FdoRfpConnectionPropertyDictionary::GetPropertyDefault(FdoString* name)
{
    if (name == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));

	return m_connection->GetPropertyDefault(name);
}

/// <summary>Determines if the specified property is required.</summary>
/// <param name="name">Input the property name</param> 
/// <returns>Returns true if the specified property is required</returns> 
bool FdoRfpConnectionPropertyDictionary::IsPropertyRequired(FdoString* name)
{
    if (name == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));

	return m_connection->IsPropertyRequired(name);
}

/// <summary> Indicates if the property is a password or other protected field
/// that should be kept secure.</summary>
/// <param name="name">Input the property name.</param> 
/// <returns>Returns true if the property is a password or other protected field
/// that should be kept secure.</returns> 
bool FdoRfpConnectionPropertyDictionary::IsPropertyProtected(FdoString* name)
{
    if (name == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));
        
	return m_connection->IsPropertyProtected(name);
}

/// <summary>Determines if the possible values for the specified property can be enumerated via the EnumeratePropertyValues method.</summary>
/// <param name="name">Input the property name</param> 
/// <returns>Returns true if the possible values for the specified property can be enumerated.</returns> 
bool FdoRfpConnectionPropertyDictionary::IsPropertyEnumerable(FdoString* name)
{
    if (name == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));
	return m_connection->IsPropertyEnumerable(name);
}

/// <summary> Returns an array of possible values for the specified property.</summary>
/// <param name="name">Input the property name.</param> 
/// <param name="count">Output the number of values.</param> 
/// <returns>Returns the list of values for this property.</returns> 
FdoString** FdoRfpConnectionPropertyDictionary::EnumeratePropertyValues(FdoString* name, FdoInt32& count)
{
    if (name == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));
	return m_connection->EnumeratePropertyValues(name, count);
}

/// <summary> Gets a localized name for the property (for NLS purposes).</summary>
/// <param name="name">Input the property name.</param> 
/// <returns>Returns the localized name for the property (for NLS purposes).</returns> 
FdoString* FdoRfpConnectionPropertyDictionary::GetLocalizedName(FdoString* name)
{
    if (name == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));
	return m_connection->GetLocalizedName(name);
}

/// <summary>Determines if the specified property represents a file name.</summary>
/// <param name="name">Input the property name</param> 
/// <returns>Returns true if the specified property is a file name</returns> 
bool FdoRfpConnectionPropertyDictionary::IsPropertyFileName(FdoString* name)
{
    if (name == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));

	return m_connection->IsPropertyFileName(name);
}

/// <summary>Determines if the specified property represents a path name.</summary>
/// <param name="name">Input the property name</param> 
/// <returns>Returns true if the specified property is a path name</returns> 
bool FdoRfpConnectionPropertyDictionary::IsPropertyFilePath(FdoString* name)
{
    if (name == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));

	return m_connection->IsPropertyFilePath(name);
}

/// <summary>Determines if the specified property represents a datastore name.</summary>
/// <param name="name">Input the property name</param> 
/// <returns>Returns true if the specified property is a datastore name</returns> 
bool FdoRfpConnectionPropertyDictionary::IsPropertyDatastoreName(FdoString* name)
{
    if (name == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));

	return m_connection->IsPropertyDatastoreName(name);
}
