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
#include "FDO\Commands\LongTransaction\mgIDeactivateLongTransaction.h"

class FdoIDeactivateLongTransaction;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION

/// \ingroup (OSGeoFDOCommandsLongTransaction)
/// \brief
/// The IDeactivateLongTransactionImp class is a 
/// concrete implementation of IDeactivateLongTransaction.
/// The IDeactivateLongTransaction interface defines the 
/// DeactivateLongTransaction command, which deactivates the active long 
/// transaction where feature manipulation commands operate on it. If the 
/// active long transaction is the root long transaction, then no long
/// transaction will be deactivated.
private ref class IDeactivateLongTransactionImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, 
                                                   public NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IDeactivateLongTransaction
{
public:
    /// \brief
    /// Executes the deactivate long transaction command.
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void Execute();

internal:
	IDeactivateLongTransactionImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoIDeactivateLongTransaction* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION


