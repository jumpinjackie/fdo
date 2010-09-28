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
#include "FDO\Commands\Feature\mgIDelete.h"

class FdoIDelete;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING
interface class ILockConflictReader;
END_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE

/// \ingroup (OSGeoFDOCommandsFeature)
/// \brief
/// The IDeleteImp class is a concrete implementation class for interface IDelete.
/// The IDelete interface defines the FDO Delete command, which deletes instances of
/// a given class that match the specified criteria. Input to the delete command
/// includes the name of the class, and filter criteria by which to identify the
/// instances to be deleted. The filter may be specified either as text or as an
/// expression tree (most likely produced by a query builder). The delete
/// command can delete instances at global scope or instances/values nested
/// within an object collection property. Instances at global scope are referred
/// to simply by the class name. Instances at a nested scope (i.e. instances
/// within a object collection property) are referred to by the containing class
/// name, followed by a '.', followed by the object collection property name.
private ref class IDeleteImp : public NAMESPACE_OSGEO_FDO_COMMANDS::IFeatureCommandImp, 
                                public NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IDelete
{
public:
    /// \brief
    /// Executes the delete command and returns the number of instances
    /// deleted
    /// 
    /// \return
    /// Returns the number of instances deleted.
    /// 
	virtual System::Int32 Execute();

    /// \brief
    /// Deleting objects might result in lock conflicts if objects
    /// to be deleted are not exclusively locked for the user attempting to
    /// delete the object. A lock conflict report is generated.
    /// This function returns a lock conflict reader that provides
    /// access to the list of lock conflicts that occurred during the execution
    /// of the delete operation.
    /// 
    /// \return
    /// Returns a lock conflict reader.
    /// 
    property NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockConflictReader^ LockConflicts
    {
        virtual NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockConflictReader^ get();
    }

internal:
	IDeleteImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::IFeatureCommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoIDelete* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE


