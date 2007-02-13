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

#pragma once

#include "FDO\Commands\Feature\mgIReader.h"

#include "FDO\Schema\mgDataType.h"
#include "FDO\Schema\mgPropertyType.h"

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE

/// \brief
/// The IDataReader interface provides a forward-only, read-only
/// iterator for reading relational table data. A reference to an
/// IDataReader is returned from the ISQLCommands ExecuteReader method.
/// The initial position of the IDataReader interface is prior to the first item.
/// Thus, you must call ReadNext to begin accessing any data.
public __gc __interface IDataReader : public NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IReader
{
public:
    /// \brief
    /// Gets the number of propertys in the result set.
    /// 
    /// \return
    /// Returns the number of propertys.
    /// 
	System::Int32 GetPropertyCount();

    /// \brief
    /// Gets the name of the property at the given ordinal position.
    /// 
    /// \param index 
    /// Input the position of the property.
    /// 
    /// \return
    /// Returns the property name
    /// 
	System::String* GetPropertyName(System::Int32 index);

    /// \brief
    /// Gets the data type of the property with the specified name.
    /// 
    /// \param name 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the data type of the property corresponding to the property name.
    /// 
	NAMESPACE_OSGEO_FDO_SCHEMA::DataType GetDataType(System::String* name);

    /// \brief
    /// Gets the property type of a given property. This is used
    /// to indicate if a given property is a geometric property or a data property. If the property is
    /// a PropertyType_DataProperty, then GetDataType can be used to to find the data type of the property.
    /// 
    /// \param name 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the property type corresponding to the property name.
    /// 
	NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType GetPropertyType(System::String* name);
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE


