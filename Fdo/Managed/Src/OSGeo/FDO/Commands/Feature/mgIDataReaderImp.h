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

#include "FDO\Commands\Feature\mgIReaderImp.h"
#include "FDO\Commands\Feature\mgIDataReader.h"

#include "FDO\Schema\mgDataType.h"
#include "FDO\Schema\mgPropertyType.h"

class FdoIDataReader;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE

/// \ingroup (OSGeoFDOCommandsFeature)
/// \brief
/// The IDataReaderImp class is a concrete implementation class for 
/// IDataReader. The IDataReader interface provides a forward-only, read-only
/// iterator for reading relational table data. A reference to an
/// IDataReader is returned from the SQLCommands ExecuteReader method.
/// The initial position of the IDataReader interface is prior to the first item.
/// Thus, you must call ReadNext to begin accessing any data.
private ref class IDataReaderImp : public NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IReaderImp, public NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IDataReader
{
public:
    /// \brief
    /// Gets the number of propertys in the result set.
    /// 
    /// \return
    /// Returns the number of propertys.
    /// 
	virtual System::Int32 GetPropertyCount();

    /// \brief
    /// Gets the data type of the property with the specified name.
    /// 
    /// \param name 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the data type of the property corresponding to the property name.
    /// 
	virtual NAMESPACE_OSGEO_FDO_SCHEMA::DataType GetDataType(System::String^ name);

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
	virtual NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType GetPropertyType(System::String^ name);

    /// \brief
    /// Gets the data type of the property at the specified index position.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the data type of the property corresponding to the index.
    /// 
    virtual NAMESPACE_OSGEO_FDO_SCHEMA::DataType GetDataType(System::Int32 index);

    /// \brief
    /// Gets the property type of a given property at the specified index. This is used
    /// to indicate if a given property is a geometric property or a data property. If the property is
    /// a PropertyType_DataProperty, then GetDataType can be used to to find the data type of the property.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the property type corresponding to the property name.
    /// 
    virtual NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType GetPropertyType(System::Int32 index);

internal:
	IDataReaderImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IReaderImp(unmanaged, autoDelete)
	{

	}

	inline FdoIDataReader* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE


