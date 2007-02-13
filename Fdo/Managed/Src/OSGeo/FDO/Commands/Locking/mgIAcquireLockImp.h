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

#include "FDO\Commands\mgIFeatureCommandImp.h"
#include "FDO\Commands\Locking\mgIAcquireLock.h"
#include "FDO\Commands\Locking\mgLockType.h"
#include "FDO\Commands\Locking\mgLockStrategy.h"

class FdoIAcquireLock;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING
public __gc __interface ILockConflictReader;

/// \brief
/// The IAcquireLockImp class is a concrete implementation of IAcquireLock.
/// The IAcquireLock interface locks feature instances of a given class 
/// that match the specified criteria. Input to the acquire lock command
/// includes the name of the class, the lock type, the lock strategy, and
/// filter criteria by which to identify the instances to be locked. The
/// filter may be specified either as text or as an expression tree (most 
/// likely produced by a query builder). The lock type value is one of 
/// <p> - LockType_Shared
/// <p> - LockType_Exclusive
/// <p> - LockType_Transaction
/// <p> - LockType_LongTransactionExclusive
/// <p> - LockType_AllLongTransactionExclusive
/// \remarks
/// If the lock strategy has a value of all and at least one of the feature
/// instances specified in the filter is locked by someone else, the command
/// returns an ILockConflictReader list which contains all of the feature
/// instances specified in the filter.
/// <p>If the lock strategy has a value of partial, the command returns an
/// ILockConflictReader list which contains all of the feature instances
/// specified in the filter that are already locked by someone else.
/// The command returns an empty ILockConflictReader list if all of the
/// feature instances specified in the filter were successfully locked.
private __gc class IAcquireLockImp : public NAMESPACE_OSGEO_FDO_COMMANDS::IFeatureCommandImp, 
                                     public NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::IAcquireLock
{
public:
    /// \brief
    /// Gets the LockType value.
    /// 
    /// \return
    /// Returns the lock type.
    /// 
	__property NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType get_LockType();

    /// \brief
    /// Sets the LockType value.
    /// 
    /// \param value 
    /// Input the lock type
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_LockType(NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType value);

    /// \brief
    /// Gets the LockStrategy value.
    /// 
    /// \return
    /// Returns the strategy value (all or partial).
    /// 
	__property NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockStrategy get_LockStrategy();

    /// \brief
    /// Sets the LockStrategy value (all or partial).
    /// 
    /// \param value 
    /// Input the lock strategy used for locking objects
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_LockStrategy(NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockStrategy value);

    /// \brief
    /// Executes the AcquireLock command, returning an ILockConflictReader.
    /// 
    /// \return
    /// Returns a list of feature instances which could not be locked.
    /// The list could be empty indicating that all of the feature instances
    /// specified in the filter have been locked, or it could contain the list
    /// of feature instances that could not be locked because they are already
    /// locked by someone or because the locking strategy is all and at least
    /// one of the feature instances specified in the filter is already locked
    /// by someone else.
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockConflictReader* Execute();

public private:
	IAcquireLockImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::IFeatureCommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoIAcquireLock* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING


