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
#include "FDO\Commands\LongTransaction\mgLongTransactionSetOperations.h"
#include "FDO\Commands\LongTransaction\mgIChangeLongTransactionSet.h"

class FdoIChangeLongTransactionSet;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION

/// \ingroup (OSGeoFDOCommandsLongTransaction)
/// \brief
/// The IChangeLongTransactionSetImp class is a 
/// concrete implementation of IChangeLongTransactionSet.
/// The IChangeLongTransactionSet interface defines the ChangeLongTransactionSet
/// command, which allows the user to add a transaction to, remove a transaction
/// from, or clear the long transaction selection set. Users can add or remove
/// long transactions to the selection set for which they have access
/// privileges. Any attempt to add a long transaction for which the user does
/// not have the access privilege will result in a failure of the command.
private __gc class IChangeLongTransactionSetImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, 
                                                  public NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IChangeLongTransactionSet
{
public:
    /// \brief
    /// Gets the name of the long transaction to add or remove from the
    /// selection set. 
    /// 
    /// \return
    /// Returns the name of the long transaction
    /// 
    /// \remarks
    /// Note: This function is not required for the Clear operation.
	__property System::String* get_LongTransactionName();

    /// \brief
    /// Sets the name of the long transaction to add or remove from the
    /// selection set. 
    /// 
    /// \param value 
    /// Input the name of the long transaction
    /// 
    /// \return
    /// Returns nothing
    /// 
    /// \remarks
    /// Note: This function is not required for the Clear operation.
	__property System::Void set_LongTransactionName(System::String* value);

    /// \brief
    /// Gets the operation to perform, add, remove, or clear.
    /// 
    /// \return
    /// Returns the operation to perform
    /// 
	__property NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::LongTransactionSetOperations get_Operation();

    /// \brief
    /// Sets the operation to perform, add, remove, or clear.
    /// 
    /// \param value 
    /// Input the operation to perform
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Operation(NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::LongTransactionSetOperations value);

    /// \brief
    /// Executes the ChangeLongTransactionSet command.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Execute();

public private:
	IChangeLongTransactionSetImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoIChangeLongTransactionSet* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION


