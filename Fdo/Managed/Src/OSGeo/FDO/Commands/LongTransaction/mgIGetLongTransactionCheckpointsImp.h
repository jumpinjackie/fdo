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
#include "FDO\Commands\LongTransaction\mgIGetLongTransactionCheckpoints.h"

class FdoIGetLongTransactionCheckpoints;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION
public __gc __interface ILongTransactionCheckpointReader;

/// \brief
/// The IGetLongTransactionCheckpointsImp class is a 
/// concrete implementation of IGetLongTransactionCheckpoints.
/// The IGetLongTransactionCheckpoints interface defines the
/// GetLongTransactionCheckpoints command, which allows the user to enumerate
/// the checkpoints for a given long transaction. To execute the operation, the
/// user must have access privilege to the long transaction.
private __gc class IGetLongTransactionCheckpointsImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, public NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IGetLongTransactionCheckpoints
{
public:
    /// \brief
    /// Gets the name of the long transaction for which to get the checkpoints.
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::String* get_LongTransactionName();

    /// \brief
    /// Sets the name of the long transaction for which to get the checkpoints.
    /// 
    /// \param value 
    /// Input the name of the long transaction
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_LongTransactionName(System::String* value);

    /// \brief
    /// Executes the get long transaction checkpoints command, returning a
    /// reference to an ILongTransactionCheckpointReader.
    /// 
    /// \return
    /// Returns nothing
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionCheckpointReader* Execute();

public private:
	IGetLongTransactionCheckpointsImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoIGetLongTransactionCheckpoints* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION


