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

#include "FDO\Connections\mgIConnectionPropertyDictionaryImp.h"
#include "FDO\Commands\DataStore\mgIDataStorePropertyDictionary.h"

class FdoIDataStorePropertyDictionary;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE

/// \ingroup (OSGeoFDOCommandsDataStore)
/// \brief
/// The IDataStorePropertyDictionaryImp class represents a concret implementation of interface 
/// IDataStorePropertyDictionary. IDataStorePropertyDictionary exposes a dictionary style interface
/// to the datastore properties. The properties required to create or destroy a datastore
/// can be determined and their values can be set through this interface.
private ref class IDataStorePropertyDictionaryImp : public NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionaryImp, public NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStorePropertyDictionary
{
public:
    /// \brief
    /// Determines if the specified property represents a file name.
    /// 
    /// \param name 
    /// Input the property name
    /// 
    /// \return
    /// Returns true if the specified property is a file name
    /// 
    virtual System::Boolean IsPropertyFileName(System::String^ name) override;

    /// \brief
    /// Determines if the specified property represents a path name.
    /// 
    /// \param name 
    /// Input the property name
    /// 
    /// \return
    /// Returns true if the specified property is a path name
    /// 
    virtual System::Boolean IsPropertyFilePath(System::String^ name) override;

internal:
	IDataStorePropertyDictionaryImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionaryImp(unmanaged, autoDelete)
	{

	}

	inline FdoIDataStorePropertyDictionary* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE


