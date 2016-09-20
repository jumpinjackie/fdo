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
#include "FDO\Commands\DataStore\mgICreateDataStore.h"

class FdoICreateDataStore;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE
interface class IDataStorePropertyDictionary;

/// \ingroup (OSGeoFDOCommandsDataStore)
/// \brief
/// The ICreateDataStoreImp class is a concrete implementation of interface ICreateDataStore.
/// ICreateDataStore defines the create datastore command, which can be used to create new 
/// provider specific datastores. The input is provided through a dictionary of name value 
/// pairs that is provider dependant. 
private ref class ICreateDataStoreImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, public NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::ICreateDataStore
{
public:
    /// \brief
    /// Gets the IDataStorePropertyDictionary interface that	
    /// can be used to dynamically query and set the properties required	
    /// to create a new datastore.
    /// 
    /// \return
    /// Returns the property dictionary
    /// 
    property NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStorePropertyDictionary^ DataStoreProperties
    {
        virtual NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStorePropertyDictionary^ get();
    }

    /// \brief
    /// Executes the ICreateDataStore command.
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual System::Void Execute();

internal:
	ICreateDataStoreImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoICreateDataStore* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE


