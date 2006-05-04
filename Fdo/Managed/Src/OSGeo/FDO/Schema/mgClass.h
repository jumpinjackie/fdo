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
/// The FdoClass class derives from FdoClassDefinition and can be used to represent any type of
/// non-spatial data within a feature schema. Typically, classes are used in the
/// feature schema to define complex properties of other feature types by
/// containment. However, they can also be used at global scope and
/// instances can be created, deleted, updated, and selected through FDO
/// commands. The Class type does not pre-define any base properties.
public __gc class Class : public NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition
{
public:
    /// \brief
    /// Constructs a default instance of an FdoClass.
    /// 
	Class();

    /// \brief
    /// Constructs an instance of an FdoClass using the specified arguments.
    /// 
    /// \param name 
    /// Input name
    /// \param description 
    /// Input description
    /// 
    /// \return
    /// Returns FdoClass
    /// 
	Class(System::String* name, System::String* description);

    /// \brief
    /// Gets the concrete class type.
    /// 
    /// \return
    /// Returns class type
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::ClassType get_ClassType();

	Class(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition(unmanaged, autoDelete)
	{

	}

public private:
	inline FdoClass* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


