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

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING

/// \ingroup (OSGeoFDOCommandsLocking)
/// \interface OSGeo::FDO::Commands::Locking::ILockOwnersReader
/// \brief
/// The ILockOwnersReader interface provides forward-only, read-only functionality
/// for identifying users. A connected user has status of lock owner even if he has
/// no locks or has released his locks.
/// \remarks
/// A reference to an
/// ILockOwnersReader interface is returned from the GetLockOwners command. The initial
/// position of the ILockOwnersReader interface is prior to the first item. Thus, you
/// must call ReadNext to begin accessing any data.
public __gc __interface ILockOwnersReader : public System::IDisposable
{
public:
    /// \brief
    /// Gets the name of a user.
    /// 
    /// \return
    /// Returns the name of a user
    /// 
	System::String* GetLockOwner();

    /// \brief
    /// Advances the reader to the next item. The default position of the 
    /// reader is prior to the first item. Thus, you must call ReadNext
    /// to begin accessing any data.
    /// 
    /// \return
    /// Returns true if there is a next item.
    /// 
	System::Boolean ReadNext();

    /// \brief
    /// Closes the ILockOwnersReader object, freeing any resources it may be
    /// holding.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Close();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING


