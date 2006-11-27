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

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING
public __gc __interface ILockOwnersReader;

/// \brief
/// The FdoIGetLockOwners interface defines the GetLockOwners command, which gets a
/// list of all lock owners. A connected user has the status of being a lock owner as
/// does a user who has released all of his locks.
public __gc __interface IGetLockOwners : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommand
{
public:
    /// \brief
    /// Executes the GetLockOwners command, returning an FdoILockOwnersReader.
    /// 
    /// \return
    /// Returns the lock owners reader.
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockOwnersReader* Execute();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING


