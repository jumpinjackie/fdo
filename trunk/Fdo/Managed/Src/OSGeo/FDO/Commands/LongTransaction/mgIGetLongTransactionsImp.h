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
#include "FDO\Commands\LongTransaction\mgIGetLongTransactions.h"

class FdoIGetLongTransactions;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION
public __gc __interface ILongTransactionReader;

/// \brief
/// The FdoIGetLongTransactions interface defines the GetLongTransactions
/// command. It allows a user to retrieve long transaction information for all or
/// a specific long transaction. The command returns a FdoILongTransactionReader
/// object enumerating the identified long transactions.
private __gc class IGetLongTransactionsImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, public NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IGetLongTransactions
{
public:
    /// \brief
    /// Sets the name of the long transaction for which information
    /// should be retrieved. The function can also be used to nullify the
    /// property by using a NULL pointer as the function's parameter. If the
    /// the property is not set, the function returns information for all
    /// available long transactions.
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Name(System::String* value);

    /// \brief
    /// Gets the currently set name of the long transaction for which
    /// the long transaction information should be retrieved. If the property
    /// is not set, the function returns a NULL pointer.
    /// 
    /// \return
    /// Returns a long transaction name.
    /// 
	__property System::String* get_Name();

    /// \brief
    /// Executes the GetLongTransactions command returning a reference to an
    /// FdoILongTransactionReader.
    /// 
    /// \return
    /// Returns a long transaction reader
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionReader* Execute();

public private:
	IGetLongTransactionsImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoIGetLongTransactions* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION


