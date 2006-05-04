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

/// \brief
/// The FdoISQLCommand interface defines the SQL command, which supports the
/// execution of a SQL statement against an underlying RDBMS. Two execute
/// methods are provided to distinguish between statements that return table data
/// versus those that execute non query type operations.
private __gc class IDataStorePropertyDictionaryImp : public NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionaryImp, public NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStorePropertyDictionary
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
    System::Boolean IsPropertyFileName(System::String* name);

    /// \brief
    /// Determines if the specified property represents a path name.
    /// 
    /// \param name 
    /// Input the property name
    /// 
    /// \return
    /// Returns true if the specified property is a path name
    /// 
    System::Boolean IsPropertyFilePath(System::String* name);

	System::Void Dispose();

public private:
	IDataStorePropertyDictionaryImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionaryImp(unmanaged, autoDelete)
	{

	}

	inline FdoIDataStorePropertyDictionary* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE


