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

/// \cond DOXYGEN-IGNORE
BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION
public __gc __interface ILongTransactionSetReader;
/// \endcond

/// \ingroup (interfaces)
/// \interface IGetLongTransactionsInSet
/// \brief
/// The IGetLongTransactionsInSet interface defines the GetLongTransactionsInSet
/// command, which allows the user to enumerate the list of versions
/// participating in the long transaction selection set.
public __gc __interface IGetLongTransactionsInSet : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommand
{
public:
    /// \brief
    /// Executes the GetLongTransactionsInSet command, returning a reference
    /// to an ILongTransactionSetReader.
    /// 
    /// \return
    /// Returns the long transaction reader
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionSetReader* Execute();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION


