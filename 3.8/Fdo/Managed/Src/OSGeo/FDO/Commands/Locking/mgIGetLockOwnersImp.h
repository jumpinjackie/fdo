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

#include "FDO\Commands\Locking\mgIGetLockOwners.h"
#include "FDO\Commands\mgICommandImp.h"

class FdoIGetLockOwners;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING
interface class ILockOwnersReader;

/// \ingroup (OSGeoFDOCommandsLocking)
/// \brief
/// The IGetLockOwnersImp class is a concrete implementation of IGetLockedObjects.
/// The IGetLockOwners interface defines the GetLockOwners command, which gets a
/// list of all lock owners. A connected user has the status of being a lock owner as
/// does a user who has released all of his locks.
private ref class IGetLockOwnersImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, 
                                       public NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::IGetLockOwners
{
public:
    /// \brief
    /// Executes the GetLockOwners command, returning an ILockOwnersReader.
    /// 
    /// \return
    /// Returns the lock owners reader.
    /// 
	virtual NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockOwnersReader^ Execute();

internal:
	IGetLockOwnersImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoIGetLockOwners* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING


