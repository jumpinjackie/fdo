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

#include "FDO\Commands\mgICommandImp.h"
#include "FDO\Commands\LongTransaction\mgIRollbackLongTransactionCheckpoint.h"

class FdoIRollbackLongTransactionCheckpoint;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION

/// \brief
/// The IRollbackLongTransactionCheckpointImp class is a concrete 
/// implementation of IRollbackLongTransactionCheckpoint.
/// The IRollbackLongTransactionCheckpoint interface defines the
/// RollbackLongTransactionCheckpoint command, which allows the user to roll back
/// changes to a named checkpoint for the given long transaction. The user
/// requires the access privilege on the long transaction for which the user tries
/// to perform the operation.
private __gc class IRollbackLongTransactionCheckpointImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, 
                                                           public NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IRollbackLongTransactionCheckpoint
{
public:
    /// \brief
    /// Gets the name of the long transaction for which a checkpoint should
    /// be rolled back.
    /// 
    /// \return
    /// Returns the list of long transaction names.
    /// 
	__property System::String* get_LongTransactionName();

    /// \brief
    /// Sets the name of the long transaction for which a checkpoint should
    /// be rolled back.
    /// 
    /// \param value 
    /// Input the long transaction name.
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_LongTransactionName(System::String* value);

    /// \brief
    /// Gets the name of the checkpoint to which the rollback operation
    /// should be executed.
    /// 
    /// \return
    /// Returns the name of the checkpoint.
    /// 
	__property System::String* get_CheckpointName();

    /// \brief
    /// Sets the name of the checkpoint to which the rollback operation
    /// should be executed.
    /// 
    /// \param value 
    /// Input the name of the checkpoint.
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_CheckpointName(System::String* value);

    /// \brief
    /// Executes the RollbackLongTransactionCheckpoint command.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Execute();

public private:
	IRollbackLongTransactionCheckpointImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoIRollbackLongTransactionCheckpoint* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION


