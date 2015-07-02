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

#include "FDO\Commands\mgIFeatureCommand.h"

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS
ref class PropertyValueCollection;
END_NAMESPACE_OSGEO_FDO_COMMANDS

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING
interface class ILockConflictReader;
END_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE

/// \ingroup (OSGeoFDOCommandsFeature)
/// \interface OSGeo::FDO::Commands::Feature::IUpdate
/// \brief
/// The IUpdate interface defines the Update command, which modifies instances
/// of a given class that match the specified criteria. Input to the update
/// command includes the name of the class, the list of property name/value
/// pairs to be updated, and the filter criteria by which to identify the
/// instances to be updated. The filter may be specified either as text or as an
/// expression tree (most likely produced by a query builder). The update
/// command can update instances at global scope or nested within an
/// object collection property. Instances at global scope are referred to simply
/// by the class name. Instances at a nested scope (i.e., instances within an
/// object collection property) are referred to by the containing class name,
/// followed by a '.', followed by the object collection property name.
public interface class IUpdate : public NAMESPACE_OSGEO_FDO_COMMANDS::IFeatureCommand
{
public:
    /// \brief
    /// Gets the PropertyValueCollection that specifies the names and values of the 
    /// properties to be updated.
    /// 
    /// \return
    /// Returns the list of properties and their values.
    /// 
    property NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection^ PropertyValues
    {
        NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection^ get();
    }
    /// \brief
    /// Executes the update command and returns the number of modified 
    /// instances.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Int32 Execute();

    /// \brief
    /// Updating objects might result in lock conflicts if objects
    /// to be updated are not exclusively locked for the user attempting to
    /// update the object. If objects to be updated are not exclusively locked for the 
    /// user attempting to update the object, a lock conflict report is generated.
    /// This function returns a lock conflict reader providing
    /// access to the list of lock conflicts that occurred during the execution
    /// of the update operation.
    /// 
    /// \return
    /// Returns a lock conflict reader.
    /// 
    property NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockConflictReader^ LockConflicts
    {
        NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockConflictReader^ get();
    }
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE


