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

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION

/// \brief
/// The IActivateLongTransactionCheckpoint interface defines the
/// IActivateLongTransactionCheckpointImp command, which allows the user to
/// activate the named checkpoint for the given long transaction. As a result, 
/// the long transaction that owns the checkpoint will be activated as well if
/// it is not already the active long transaction. The user requires the access
/// privilege on the long transaction for which the attempt is made to activate a
/// checkpoint.
public __gc __interface IActivateLongTransactionCheckpoint : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommand
{
public:
    /// \brief
    /// Gets the name of the checkpoint to be activated for the named long
    /// transaction. If the property's value is empty the system will
    /// activate the most recent checkpoint of the long transaction.
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::String* get_CheckpointName();

    /// \brief
    /// Sets the name of the checkpoint to be activated for the named long
    /// transaction. If the property's value is empty the system will
    /// activate the most recent checkpoint of the long transaction.
    /// 
    /// \param value 
    /// Input the name of the checkpoint
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_CheckpointName(System::String* value);

    /// \brief
    /// Executes the ActivateLongTransactionCheckpoint command.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Execute();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION


