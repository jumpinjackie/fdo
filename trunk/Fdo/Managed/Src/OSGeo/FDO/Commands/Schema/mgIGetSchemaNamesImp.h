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
#include "FDO\Commands\Schema\mgIGetSchemaNames.h"

class FdoIGetSchemaNames;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
ref class FeatureSchemaCollection;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA

/// \ingroup (OSGeoFDOCommandsSchema)
/// \brief
/// The IGetSchemaNamesImp class is a concrete implementation of IGetSchemaNames.
/// The IGetSchemaNames interface defines the GetSchemaNames command, which
/// retrieves the list of available schemas.
/// The Execute operation returns a StringCollection
/// object.
private ref class IGetSchemaNamesImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, 
                                        public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::IGetSchemaNames
{
public:
    /// \brief
    /// Executes the GetSchemaNames command and returns a 
    /// StringCollection.
    /// 
    /// \return
    /// Returns the string collection of the names of the available schemas.
    /// 
    virtual NAMESPACE_OSGEO_COMMON::StringCollection^ Execute();

internal:
	IGetSchemaNamesImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoIGetSchemaNames* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA


