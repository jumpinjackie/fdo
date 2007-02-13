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

#include "FDO\Schema\mgClassDefinition.h"

#include "FDO\Schema\mgClassType.h"

class FdoClass;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA

/// \brief
/// Class derives ClassDefinition and can be used to represent any type of
/// non-spatial data within a feature schema. Typically, classes are used in the
/// feature schema to define complex properties of other feature types by
/// containment. However, they can also be used at global scope and
/// instances can be created, deleted, updated, and selected through FDO
/// commands. The Class type does not pre-define any base properties.
public __gc class Class : public NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition
{
public:
    /// \brief
    /// Constructs a default instance of an Class.
    /// 
	Class();

    /// \brief
    /// Constructs an instance of an FDO Class using the specified arguments.
    /// 
    /// \param name 
    /// Input the name of the class
    /// \param description 
    /// Input the description of the class
    /// 
    /// \return
    /// Returns an FDO Class
    /// 
	Class(System::String* name, System::String* description);

    /// \brief
    /// Gets the concrete FDO class type.
    /// 
    /// \return
    /// Returns the FDO class type
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::ClassType get_ClassType();

    /// \brief
    /// Constructs an FDO Class object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	Class(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition(unmanaged, autoDelete)
	{

	}

public private:
	inline FdoClass* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


