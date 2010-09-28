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

#include "FDO\Commands\Feature\mgIBaseSelectImp.h"
#include "FDO\Commands\Feature\mgISelect.h"

class FdoISelect;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE

/// \ingroup (OSGeoFDOCommandsFeature)
/// \brief
/// The ISelectImp class is a concrete implementation of interface ISelect.
/// The ISelect interface defines the Select command, which queries for features
/// of a given class that match the specified criteria. Input to the select
/// command includes the name of the class, the list of properties to be
/// returned, and a filter. All but the class name is optional. If the list of
/// properties to be returned is empty then all properties are returned. The
/// filter may be specified either as text or as an expression tree (most likely
/// produced by a query builder). The result of executing a select command is an
/// IFeatureReader reference (see "Reading Features"). If the feature provider
/// supports locking, then the select command can optionally lock all of the
/// features selected, via the ExecuteWithLock method (see "Locking
/// Commands" for more information on locking features).
private ref class ISelectImp : public NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IBaseSelectImp, public NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelect
{
public:
    /// \brief
    /// Gets the LockType value (see "Locking Commands").
    /// 
    /// \return
    /// Returns the lock type.
    /// 
    /// \brief
    /// Sets the LockType value (see "Locking Commands").
    /// 
    /// \param value 
    /// Input the lock type.
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual property NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType LockType
    {
        NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType get();
        System::Void set(NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType value);
    }

    /// \brief
    /// Gets the LockStrategy value (see "Locking Commands").
    /// 
    /// \return
    /// Returns the lock strategy.
    /// 
    /// \brief
    /// Sets the LockStrategy value (see "Locking Commands").
    /// 
    /// \param value 
    /// Input the lock strategy.
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual property NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockStrategy LockStrategy
    {
        NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockStrategy get();
        System::Void set(NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockStrategy value);
    }

    /// \brief
    /// Executes the select command and returns a reference to an IFeatureReader.
    /// 
    /// \return
    /// Returns the feature reader.
    /// 
	virtual NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReader^ Execute();

    /// \brief
    /// Executes the select command and returns a reference to an
    /// IFeatureReader.
    /// 
    /// \return
    /// Returns the feature reader.
    /// 
	virtual NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReader^ ExecuteWithLock();

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
    property NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockConflictReader^ LockConflicts
    {
        virtual NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockConflictReader^ get();
    }

internal:
	ISelectImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IBaseSelectImp(unmanaged, autoDelete)
	{

	}

	inline FdoISelect* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE


