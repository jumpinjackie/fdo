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

#include "FDO\Schema\mgPropertyDefinition.h"
#include "FDO\Schema\mgObjectType.h"
#include "FDO\Schema\mgOrderType.h"

class FdoObjectPropertyDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
public __gc class ClassDefinition;
public __gc class DataPropertyDefinition;

/// \brief
/// The FdoObjectPropertyDefinition class derives from FdoPropertyDefinition and represents
/// containment of an object or a collection of objects within another class.
/// The class of the contained object must already be defined in the feature
/// schema and cannot be abstract.
public __gc class ObjectPropertyDefinition : public NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition
{
public:
    /// \brief
    /// Constructs a default instance of an FdoObjectPropertyDefinition.
    /// 
	ObjectPropertyDefinition();

    /// \brief
    /// Constructs an instance of an FdoObjectPropertyDefinition using the specified
    /// arguments.
    /// 
    /// \param name 
    /// Input name
    /// \param description 
    /// Input description
    /// \param system 
    /// Input true if this is a system generated property, otherwise false.
    /// <p><b>Note:</b> A client would never set system to true, only a provider.
    /// 
	ObjectPropertyDefinition(System::String* name, System::String* description);

	ObjectPropertyDefinition(System::String* name, System::String* description, System::Boolean system);

    /// \brief
    /// Gets a reference to the FdoClassDefinition that defines the type of this
    /// property.
    /// 
    /// \return
    /// Returns class definition
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition* get_Class();

    /// \brief
    /// Sets a reference to the FdoClassDefinition that defines the type of this
    /// property.
    /// 
    /// \param value 
    /// Input class definition
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Class(NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition* value);

    /// \brief
    /// Gets a reference to an FdoDataPropertyDefinition to use for uniquely identifying
    /// instances of the contained class within a single parent object instance.
    /// This value is only used for the FdoObjectType_Collection and FdoObjectType_OrderedCollection object
    /// property types. The FdoDataPropertyDefinition must belong to the FdoClassDefinition that
    /// defines the type of this property.
    /// 
    /// \return
    /// Returns data property definition
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition* get_IdentityProperty();

    /// \brief
    /// Sets a reference to an FdoDataPropertyDefinition to use for uniquely identifying
    /// instances of the contained class within a single parent object instance.
    /// This value is only used for the FdoObjectType_Collection and FdoObjectType_OrderedCollection object
    /// property types. The FdoDataPropertyDefinition must belong to the FdoClassDefinition that
    /// defines the type of this property.
    /// 
    /// \param value 
    /// Input data property definition
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_IdentityProperty(NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition* value);

    /// \brief
    /// Gets the type of this object property (value, collection, or ordered 
    /// collection).
    /// 
    /// \return
    /// Returns the type of this object property
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::ObjectType get_ObjectType();

    /// \brief
    /// Sets the type of this object property (value, collection, or ordered 
    /// collection).
    /// 
    /// \param value 
    /// Input the type of this object property
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_ObjectType(NAMESPACE_OSGEO_FDO_SCHEMA::ObjectType value);

    /// \brief
    /// Gets the order type of this object property (ascending or descending). 
    /// This property is only applicable if the property type is set to 
    /// FdoObjectType_OrderedCollection.
    /// 
    /// \return
    /// Returns the order type
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::OrderType get_OrderType();

    /// \brief
    /// Sets the order type of this object property (ascending or descending). 
    /// This property is only applicable if the property type is set to 
    /// FdoObjectType_OrderedCollection.
    /// 
    /// \param value 
    /// Input the order type
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_OrderType(NAMESPACE_OSGEO_FDO_SCHEMA::OrderType value);

	ObjectPropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(unmanaged, autoDelete)
	{

	}

public private:
	inline FdoObjectPropertyDefinition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


