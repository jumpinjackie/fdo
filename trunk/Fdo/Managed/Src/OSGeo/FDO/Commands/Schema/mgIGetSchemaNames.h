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

#include "FDO\Commands\mgICommand.h"

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
public __gc class FeatureSchemaCollection;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA

/// \ingroup (OSGeoFDOCommandsSchema)
/// \interface OSGeo::FDO::Commands::Schema::IGetSchemaNames
/// \brief
/// The IGetSchemaNames interface defines the GetSchemaNames command.
/// The GetSchemaNames command retrieves a list all schemas available from
/// the connection. The Execute operation returns a StringCollection object.
public __gc __interface IGetSchemaNames : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommand
{
public:
    /// \brief
    /// Executes the GetSchemaNames command and returns a 
    /// FdoStringCollection. 
    /// 
    /// \return
    /// Returns the string collection of the names of the available schemas.
    /// 
	NAMESPACE_OSGEO_COMMON::StringCollection* Execute();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA


