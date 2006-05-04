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

#include "FDO\Commands\Locking\mgIGetLockedObjects.h"
#include "FDO\Commands\mgICommandImp.h"

class FdoIGetLockedObjects;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING
public __gc __interface ILockedObjectReader;

/// \brief
/// The FdoIGetLockedObjects interface defines the GetLockedObjects command, which
/// gets a list of all objects that are currently locked by a particular user.
private __gc class IGetLockedObjectsImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, public NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::IGetLockedObjects
{
public:
    /// \brief
    /// Gets the name of the user whose locked objects you want to list.
    /// 
    /// \return
    /// Returns the name of the user
    /// 
	__property System::String* get_LockOwner();

    /// \brief
    /// Sets the name of the user whose locked objects you want to list.
    /// 
    /// \param value 
    /// Input the name of the user
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_LockOwner(System::String* value);

    /// \brief
    /// Executes the GetLockedObjects command, returning an FdoILockedObjectReader.
    /// 
    /// \return
    /// Returns a list of all of the objects currently locked by a particular user.i The list might be empty if the user does not have anything locked.
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockedObjectReader* Execute();

	System::Void Dispose();

public private:
	IGetLockedObjectsImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoIGetLockedObjects* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING


