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

#pragma  once

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION

/// \brief
/// The FdoLongTransactionSetOperations enumeration defines the list of
/// available operations that can be performed by the ChangeLongTransactionSet command.
/// <param name="FdoLongTransactionSetOperations_Add">
/// Add the specified long transaction to the selection set.
/// </param>
/// <param name="FdoLongTransactionSetOperations_Remove">
/// Remove the specified long transaction from the selection set.
/// </param>
/// <param name="FdoLongTransactionSetOperations_Clear">
/// Clear the selection set.
/// </param>
[System::FlagsAttribute]
public __value enum LongTransactionSetOperations
{
    /// Add the specified long transaction to the selection set.
	LongTransactionSetOperations_Add = FdoLongTransactionSetOperations_Add,

    /// Remove the specified long transaction from the selection set.
	LongTransactionSetOperations_Remove = FdoLongTransactionSetOperations_Remove,

    /// Clear the selection set.
	LongTransactionSetOperations_Clear = FdoLongTransactionSetOperations_Clear
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION


