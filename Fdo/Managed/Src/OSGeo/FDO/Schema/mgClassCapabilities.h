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

#pragma  once

#include "FDO\Commands\Locking\mgLockType.h"

class FdoClassCapabilities;

template <typename T>
class FdoArray;

enum FdoLockType;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
public __gc class ClassDefinition;

/// \brief
/// The FdoClassCapabilites class describes various capabilities
/// for a particular FDO Class Definition and an FDO Provider datastore.
public __gc class ClassCapabilities : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    /// Constructs an FdoClassCapabilities instance. This function
    /// should only be called from an FDO Provider.
    /// 
    /// \param parent 
    /// Input The Fdo Class Definition that owns these
    /// capabilities.
    /// 
	ClassCapabilities(NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition* parent);

    /// \brief
    /// Gets the locking support capability.
    /// 
    /// \return
    /// Returns true if objects of this class can be locked
    /// 
	__property System::Boolean get_SupportsLocking();

    /// \brief
    /// Sets the locking support capability. This function should only 
    /// be called from an FDO Provider.
    /// 
    /// \param value 
    /// Input The locking support capability
    /// 
	__property System::Void set_SupportsLocking(System::Boolean value);

    /// \brief
    /// Gets an array of the FdoLockType values supported by the feature provider.
    /// 
    /// \param size 
    /// Output the number of lock types
    /// 
    /// \return
    /// Returns the list of lock types
    /// 
	__property NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType get_LockTypes() [];

    /// \brief
    /// Sets the locking types support capability. This function should only 
    /// be called from an FDO Provider.
    /// 
    /// \param types 
    /// The supported array of lock types
    /// \param size 
    /// The number of lock types
    /// 
	__property System::Void set_LockTypes(NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType types[]);

    /// \brief
    /// Gets the long transaction support capability.
    /// 
    /// \return
    /// Returns true if long transactions can be created on objects
    /// of this class
    /// 
	__property System::Boolean get_SupportsLongTransactions();

    /// \brief
    /// Sets the long transaction support capability. This function should
    /// only be called from an FDO Provider.
    /// 
    /// \param value 
    /// Input The long transaction capability
    /// 
	__property System::Void set_SupportsLongTransactions(System::Boolean value );

    /// \brief
    /// Gets the parent of this Class Capabilites object.
    /// 
    /// \return
    /// Returns FdoClassDefinition
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition* get_Parent();

    /// \brief
    /// Returns true if the class supports write.
    /// 
    __property System::Boolean get_SupportsWrite();

public private:
	ClassCapabilities(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoClassCapabilities* GetImpObj();

protected:
	System::Void ReleaseUnmanagedObject();

private:
	typedef FdoArray<FdoLockType> FdoLockTypeArray;

	inline NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType WrapFdoLockTypeArray(const FdoLockType* umArray, FdoInt32 len) []
	{
		NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType mgArray __gc[] = __gc new NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType[len];
		for (FdoInt32 i = 0; i < len; i++)
		{
			mgArray[i] = static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType>(umArray[i]);
		}
		return mgArray;
	}

	inline FdoLockTypeArray* UnwrapLockTypeArray(NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType mgArray __gc[])
	{
		FdoLockType* umArray = new FdoLockType[mgArray->Length];
		for (FdoInt32 i = 0; i < mgArray->Length; i++)
		{
			umArray[i] = static_cast<FdoLockType>(mgArray[i]);
		}
		return FdoLockTypeArray::Create(umArray, mgArray->Length);
	}

};

END_NAMESPACE_OSGEO_FDO_SCHEMA


