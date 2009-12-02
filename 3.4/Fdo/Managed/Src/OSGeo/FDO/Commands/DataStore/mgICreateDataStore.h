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

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE
public __gc __interface IDataStorePropertyDictionary;

/// \ingroup (OSGeoFDOCommandsDataStore)
/// \interface OSGeo::FDO::Commands::DataStore::ICreateDataStore
/// \brief
/// The ICreateDataStore interface defines the create datastore command,
/// which can be used to create new provider specific datastores. The input is provided
/// through a dictionary of name value pairs that is provider dependant. 
public __gc __interface ICreateDataStore : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommand
{
public:
    /// \brief
    /// Gets the IDataStorePropertyDictionary interface that	
    /// can be used to dynamically query and set the properties required	
    /// to create a new datastore.
    /// 
    /// \return
    /// Returns the DataStore property dictionary
    /// 
    __property NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStorePropertyDictionary* get_DataStoreProperties();

    /// \brief
    /// Executes the ICreateDataStore command.
    /// 
    /// \return
    /// Returns nothing
    /// 
    System::Void Execute();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE


