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
ref class ClassDefinition;

/// \ingroup (OSGeoFDOSchema)
/// \brief
/// The ClassCapabilites class describes various capabilities
/// for a particular FDO Class Definition and an FDO Provider datastore.
public ref class ClassCapabilities : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    /// Constructs an ClassCapabilities instance. This function
    /// should only be called from an FDO Provider.
    /// 
    /// \param parent 
    /// Input The FDO Class Definition that owns these capabilities.
    /// 
	ClassCapabilities(NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ parent);

    /// \brief
    /// Gets the locking support capability.
    /// 
    /// \return
    /// Returns true if objects of this class can be locked
    /// 
    /// \brief
    /// Sets the locking support capability. This function should only 
    /// be called from an FDO Provider.
    /// 
    /// \param value 
    /// Input The locking support capability
    /// 
    property System::Boolean SupportsLocking
    {
        System::Boolean get();
        System::Void set(System::Boolean value);
    }

    /// \brief
    /// Gets an array of LockType values supported by the feature provider.
    /// 
    /// \return
    /// Returns the list of lock types
    /// 
    /// \brief
    /// Sets the locking types support capability. This function should only 
    /// be called from an FDO Provider.
    /// 
    /// \param types 
    /// The supported array of lock types
    /// 
    property array<NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType>^ LockTypes
    {
        array<NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType>^ get();
        System::Void set(array<NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType>^ value);
    }

    /// \brief
    /// Gets the long transaction support capability.
    /// 
    /// \return
    /// Returns true if long transactions can be created on objects
    /// of this class
    /// 
    /// \brief
    /// Sets the long transaction support capability. This function should
    /// only be called from an FDO Provider.
    /// 
    /// \param value 
    /// Input The long transaction capability
    /// 
    property System::Boolean SupportsLongTransactions
    {
        System::Boolean get();
        System::Void set(System::Boolean value);
    }

    /// \brief
    /// Gets the parent of this Class Capabilites object.
    /// 
    /// \return
    /// Returns ClassDefinition
    /// 
    property NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ Parent
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ get();
    }

    /// \brief
    /// Returns true if the class supports write.
    /// 
    property System::Boolean SupportsWrite
    {
        System::Boolean get();
    }

internal:
	ClassCapabilities(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoClassCapabilities* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;

private:
	typedef FdoArray<FdoLockType> FdoLockTypeArray;

	inline array<NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType>^ WrapFdoLockTypeArray(const FdoLockType* umArray, FdoInt32 len)
	{
		array<NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType>^ mgArray = gcnew array<NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType>(len);
		for (FdoInt32 i = 0; i < len; i++)
			mgArray[i] = static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType>(umArray[i]);

		return mgArray;
	}

	inline FdoLockTypeArray* UnwrapLockTypeArray(array<NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType>^ mgArray)
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


