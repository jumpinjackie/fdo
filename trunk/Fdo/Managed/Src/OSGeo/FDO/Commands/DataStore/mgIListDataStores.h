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
interface class IDataStoreReader;

/// \ingroup (OSGeoFDOCommandsDataStore)
/// \interface OSGeo::FDO::Commands::DataStore::IListDataStores
/// \brief
/// The IListDataStores interface defines the list datastores command,
/// which provides a way to get a list of datastores at a particular server. 
public interface class IListDataStores : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommand
{
public:
    /// \brief
    /// Gets the flag that indicates whether to include or
    /// not include Non-FDO enabled datastores in the returned list.
    /// These are also referred to as 'Foreign' datastores.
    /// 
    /// \return
    /// Returns the flag
    /// 
    /// \brief
    /// Sets the flag to indicate whether to include or not include
    /// Non-FDO enabled datastores in the returned list. These are also
    /// referred to as 'Foreign' datastores.
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::Boolean IncludeNonFdoEnabledDatastores
    {
        System::Boolean get();
        System::Void set(System::Boolean include);
    }

    /// \brief
    /// Executes the IListDataStores command.
    /// 
    /// \return
    /// Returns nothing
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStoreReader^ Execute();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE


