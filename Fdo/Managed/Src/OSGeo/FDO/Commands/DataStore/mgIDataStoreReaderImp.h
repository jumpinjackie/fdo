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

#include "FDO\Commands\DataStore\mgIDataStoreReader.h"

class FdoIDataStoreReader;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE
interface class IDataStorePropertyDictionary;

/// \ingroup (OSGeoFDOCommandsDataStore)
/// \brief
/// The IDataStoreReaderImp class is a concrete implementation class of the
/// IDataStoreReader interface. IDataStoreReader provides a forward-only, read-only iterator
/// for reading feature data.  A reference to an IDataStoreReader is returned
/// from the IListDataStores command. Because the initial position of the
/// IDataStoreReader is prior to the first item, you must call
/// ReadNext to begin accessing any data.
private ref class IDataStoreReaderImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStoreReader
{
public:
    /// \brief
    /// Gets the name of the datastore currently being read.
    /// 
    /// \return
    /// Returns the description of the datastore.
    /// 
	virtual System::String^ GetName();

    /// \brief
    /// Gets the description of the datastore currently being read.
    /// 
    /// \return
    /// Returns the description of the datastore.
    /// 
	virtual System::String^ GetDescription();

    /// \brief
    /// Gets the flag that indicates whether the datastore is FDO enabled.
    /// Non-FDO databases are also referred to as “Foreign?datastores.
    /// 
    /// \return
    /// Returns the flag
    /// 
	virtual System::Boolean GetIsFdoEnabled();

    /// \brief
    /// Gets the IDataStorePropertyDictionary interface that 
    /// includes the properties of the datastores set at datastore create 
    /// time, with the exception of password.
    /// 
    /// \return
    /// Returns the property dictionary
    /// 
	virtual NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStorePropertyDictionary^ GetDataStoreProperties();

    /// \brief
    /// Advances the reader to the next item and returns true if there is
    /// another object to read or false if reading is complete. The default
    /// position of the reader is prior to the first item. Thus you must
    /// call ReadNext to begin accessing any data.
    /// 
    /// \return
    /// Returns true if there is a next item.
    /// 
	virtual System::Boolean ReadNext();

    /// \brief
    /// Closes the IDataStoreReader object, freeing any resources it may be holding.
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void Close();

internal:
	IDataStoreReaderImp(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoIDataStoreReader* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE


