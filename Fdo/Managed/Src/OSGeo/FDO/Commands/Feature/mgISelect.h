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

#include "FDO\Commands\Feature\mgIBaseSelect.h"
#include "FDO\Commands\Locking\mgLockType.h"
#include "FDO\Commands\Locking\mgLockStrategy.h"

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING
public __gc __interface ILockConflictReader;
END_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE
public __gc __interface IFeatureReader;

/// \brief
/// The FdoISelect interface defines the Select command, which queries for features
/// of a given class that match the specified criteria. Input to the select
/// command includes the name of the class, the list of properties to be
/// returned, and a filter. All but the class name is optional. If the list of
/// properties to be returned is empty then all properties are returned. The
/// filter may be specified either as text or as an expression tree (most likely
/// produced by a query builder). The result of executing a select command is an
/// FdoIFeatureReader reference (see "Reading Features"). If the feature provider
/// supports locking, then the select command can optionally lock all of the
/// features selected, via the ExecuteWithLock method (see "Locking
/// Commands" for more information on locking features).
public __gc __interface ISelect : public NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IBaseSelect
{
public:
    /// \brief
    /// Gets the FdoLockType value (see "Locking Commands").
    /// 
    /// \return
    /// Returns the lock type.
    /// 
	__property NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType get_LockType();

    /// \brief
    /// Sets the FdoLockType value (see "Locking Commands").
    /// 
    /// \param value 
    /// Input the lock type.
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_LockType(NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType value);

    /// \brief
    /// Gets the FdoLockStrategy value (see "Locking Commands").
    /// 
    /// \return
    /// Returns the lock strategy.
    /// 
	__property NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockStrategy get_LockStrategy();

    /// \brief
    /// Sets the FdoLockStrategy value (see "Locking Commands").
    /// 
    /// \param value 
    /// Input the lock strategy.
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_LockStrategy(NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockStrategy value);

    /// \brief
    /// Executes the select command and returns a reference to an FdoIFeatureReader.
    /// 
    /// \return
    /// Returns the feature reader.
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReader* Execute();

    /// \brief
    /// Executes the select command and returns a reference to an
    /// IFeatureReader.
    /// 
    /// \return
    /// Returns the feature reader.
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReader* ExecuteWithLock();

    /// \brief
    ///  When executing the operation ExecuteWithLock lock 
    /// conflicts may occur. Those lock conflicts are reported. The 
    /// function GetLockConflicts returns a lock conflict reader 
    /// providing access to the list of lock conflicts that occurred 
    /// during the execution of the operation.
    /// 
    /// \return
    /// Returns a lock conflict reader.
    /// 
	__property NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockConflictReader* get_LockConflicts();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE


