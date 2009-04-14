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

/// \ingroup (OSGeoFDOCommandsLongTransaction)
/// \interface OSGeo::FDO::Commands::LongTransaction::ICreateLongTransactionCheckpoint
/// \brief
/// The ICreateLongTransactionCheckpoint interface defines the
/// CreateLongTransactionCheckpoint command, which allows the user to create a
/// checkpoint for any long transaction to which the user has the access privileges.
public __gc __interface ICreateLongTransactionCheckpoint : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommand
{
public:
    /// \brief
    /// Gets the name of the long transaction for which a checkpoint should
    /// be created.
    /// 
    /// \return
    /// Returns the long transaction name
    /// 
	__property System::String* get_LongTransactionName();

    /// \brief
    /// Sets the name of the long transaction for which a checkpoint should
    /// be created.
    /// 
    /// \param value 
    /// Input the name of the long transaction
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_LongTransactionName(System::String* value);

    /// \brief
    /// Gets the name of the checkpoint to be created for the named long
    /// transaction.
    /// 
    /// \return
    /// Returns the checkpoint name
    /// 
	__property System::String* get_CheckpointName();

    /// \brief
    /// Sets the name of the checkpoint to be created for the named long
    /// transaction.
    /// 
    /// \param value 
    /// Input the checkpoint name
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_CheckpointName(System::String* value);

    /// \brief
    /// Gets the description of the checkpoint to be created for the named
    /// long transaction.
    /// 
    /// \return
    /// Returns the description of the checkpoint
    /// 
	__property System::String* get_CheckpointDescription();

    /// \brief
    /// Sets the description of the checkpoint to be created for the named
    /// long transaction.
    /// 
    /// \param value 
    /// Input the description of the checkpoint
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_CheckpointDescription(System::String* value);

    /// \brief
    /// Executes the CreateLongTransactionCheckpoint command.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Execute();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION


