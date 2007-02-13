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

#include "FDO\Connections\mgIConnectionPropertyDictionary.h"

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE

/// \brief
/// The IDataStorePropertyDictionary exposes a dictionary style interface
/// to the datastore properties. The properties required to create or destroy a datastore
/// can be determined and their values can be set through this interface.
public __gc __interface IDataStorePropertyDictionary : public NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionary
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
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE


