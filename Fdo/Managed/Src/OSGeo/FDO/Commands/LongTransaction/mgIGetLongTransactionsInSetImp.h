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
#include "FDO\Commands\LongTransaction\mgIGetLongTransactionsInSet.h"

class FdoIGetLongTransactionsInSet;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION
public __gc __interface ILongTransactionSetReader;

/// \brief
/// The FdoIGetLongTransactionsInSet interface defines the GetLongTransactionsInSet
/// command, which allows the user to enumerate the list of versions
/// participating in the long transaction selection set.
private __gc class IGetLongTransactionsInSetImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, public NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IGetLongTransactionsInSet
{
public:
    /// \brief
    /// Executes the GetLongTransactionsInSet command, returning a reference
    /// to an FdoILongTransactionSetReader.
    /// 
    /// \return
    /// Returns the long transaction reader
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionSetReader* Execute();

public private:
	IGetLongTransactionsInSetImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoIGetLongTransactionsInSet* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION


