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

#include "FDO\Commands\mgICommandImp.h"
#include "FDO\Commands\Schema\mgIGetClassNames.h"

class FdoIGetClassNames;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
ref class FeatureSchemaCollection;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA

/// \ingroup (OSGeoFDOCommandsSchema)
/// \brief
/// The IGetClassNamesImp class is a concrete implementation of IGetClassNames.
/// The IGetClassNames interface defines the GetClassNames command, which
/// retrieves the list of available class names from the connection.
/// The Execute operation returns an StringCollection object.
private ref class IGetClassNamesImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, 
                                        public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::IGetClassNames
{
public:
    /// \brief
    /// Gets the name of the schema for the enumeration. This function is optional;
    /// if not specified, execution of the command will enumerate the classes in all schemas.
    /// 
    /// \return
    /// Returns the schema name
    /// 
    /// \brief
    /// Sets the name of the schema for the enumeration. This function is optional; if not
    /// specified execution of the command will enumerate the classes in all schemas.
    /// 
    /// \param value 
    /// Input the schema name
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual property System::String^ SchemaName
    {
        System::String^ get();
        System::Void set(String^ value);
    }

    /// \brief
    /// Executes the GetClassNames command and returns a 
    /// StringCollection. If the specified schema name does not exist,
    /// the Execute method throws an exception.
    /// 
    /// \return
    /// Returns the string collection of the feature classes for the specified schema.
    /// 
    virtual NAMESPACE_OSGEO_COMMON::StringCollection^ Execute();

internal:
	IGetClassNamesImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoIGetClassNames* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA


