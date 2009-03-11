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
interface class ILockedObjectReader;

/// \ingroup (OSGeoFDOCommandsLocking)
/// \interface OSGeo::FDO::Commands::Locking::IGetLockedObjects
/// \brief
/// The IGetLockedObjects interface defines the GetLockedObjects command, which
/// gets a list of all objects that are currently locked by a particular user.
public interface class IGetLockedObjects : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommand
{
public:
    /// \brief
    /// Gets the name of the user whose locked objects you want to list.
    /// 
    /// \return
    /// Returns the name of the user
    /// 
    /// \brief
    /// Sets the name of the user whose locked objects you want to list.
    /// 
    /// \param value 
    /// Input the name of the user
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::String^ LockOwner
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// \brief
    /// Executes the GetLockedObjects command, returning an ILockedObjectReader.
    /// 
    /// \return
    /// Returns a list of all of the objects currently locked by a particular user. 
    /// The list might be empty if the user does not have anything locked.
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockedObjectReader^ Execute();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING


