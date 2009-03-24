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
#include "FDO\Commands\LongTransaction\mgICreateLongTransactionCheckpoint.h"

class FdoICreateLongTransactionCheckpoint;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION

/// \ingroup (OSGeoFDOCommandsLongTransaction)
/// \brief
/// The ICreateLongTransactionCheckpointImp class is a 
/// concrete implementation of ICreateLongTransactionCheckpoint.
/// The ICreateLongTransactionCheckpoint interface defines the
/// CreateLongTransactionCheckpoint command, which allows the user to create a
/// checkpoint for any long transaction to which the user has the access privileges.
private ref class ICreateLongTransactionCheckpointImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, public NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ICreateLongTransactionCheckpoint
{
public:
    /// \brief
    /// Gets the name of the long transaction for which a checkpoint should
    /// be created.
    /// 
    /// \return
    /// Returns the long transaction name
    /// 
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
    virtual property System::String^ LongTransactionName
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// \brief
    /// Gets the name of the checkpoint to be created for the named long
    /// transaction.
    /// 
    /// \return
    /// Returns the checkpoint name
    /// 
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
    virtual property System::String^ CheckpointName
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// \brief
    /// Gets the description of the checkpoint to be created for the named
    /// long transaction.
    /// 
    /// \return
    /// Returns the description of the checkpoint
    /// 
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
    virtual property System::String^ CheckpointDescription
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// \brief
    /// Executes the CreateLongTransactionCheckpoint command.
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void Execute();

internal:
	ICreateLongTransactionCheckpointImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoICreateLongTransactionCheckpoint* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION


