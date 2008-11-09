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
*/

#pragma  once

class FdoSchemaAttributeDictionary;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA

/// \ingroup (OSGeoFDOSchema)
/// \brief
/// The SchemaAttributeDictionary class maintains a dictionary of named 
/// attributes that provide a generic extensibility mechanism for objects
/// in the feature schema. This mechanism can be used to dynamically extend 
/// schema elements in either an application- or provider-specific manner.
public __gc class SchemaAttributeDictionary : public NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler
{
public:
    /// \brief
    /// Gets the number of attributes in the dictionary.
    /// 
    /// \return
    /// Returns the number of attributes in the dictionary
    /// 
	__property System::Int32 get_Count();

    /// \brief
    /// Gets an array containing all of the attribute names in the dictionary.
    /// 
    /// \return
    /// Returns an array containing all of the attribute names in the dictionary
    /// 
	__property System::String* get_AttributeNames() [];

    /// \brief
    /// Gets the value of the specified attribute.
    /// 
    /// \param name 
    /// Input attribute name
    /// 
    /// \return
    /// Returns the value of the specified attribute
    /// 
	System::String* GetAttributeValue(System::String* name);

    /// \brief
    /// Sets the value of the specified attribute.
    /// 
    /// \param name 
    /// Input name
    /// \param value 
    /// Input value
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void SetAttributeValue(System::String* name, System::String* value);

    /// \brief
    /// Returns true if the dictionary contains the specified attribute.
    /// 
    /// \param name 
    /// Input attribute name
    /// 
    /// \return
    /// Returns true if the dictionary contains the specified attribute
    /// 
	System::Boolean ContainsAttribute(System::String* name);

    /// \brief
    /// Adds the specified attribute with the specified value to the dictionary.
    /// 
    /// \param name 
    /// Input name
    /// \param value 
    /// Input value
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Add(System::String* name, System::String* value);

    /// \brief
    /// Removes the specified attribute and its value from the dictionary.
    /// 
    /// \param name 
    /// Input name
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Remove(System::String* name);

    /// \brief
    /// Clears all attributes and values from the dictionary.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Clear();

    /// \brief
    /// Constructs a SchemaAttributeDictionary object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	SchemaAttributeDictionary(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler(unmanaged, autoDelete)
	{
		
	}

/// \cond DOXYGEN-IGNORE
protected:
	System::Void ReleaseUnmanagedObject();
/// \endcond

public private:
	inline FdoSchemaAttributeDictionary* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


