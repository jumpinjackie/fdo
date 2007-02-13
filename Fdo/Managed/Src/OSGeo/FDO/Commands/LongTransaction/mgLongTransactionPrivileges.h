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
/// The LongTransactionPrivileges enumeration defines the long transaction
/// privilege flags.
///
/// \param LongTransactionPrivileges_Access
/// The access privilege is required for a user if the user needs to access
/// a long transaction.
///
/// \param LongTransactionPrivileges_Create
/// A user must have access and create privileges on the active long
/// transaction if the user wants to create a new long transaction as a
/// descendent of the active long transaction.
///
/// \param LongTransactionPrivileges_Commit
/// A user must have access and commit privileges on a long transaction
/// in order to commit it. In addition, the user needs access privilege to
/// the long transaction's parent. The user also needs remove privilege on
/// the long transaction to be committed if the operation to be executed
/// is a commit and not a partial commit.
///
/// \param LongTransactionPrivileges_Rollback
/// A user must have access and rollback privileges on a long transaction
/// in order to roll it back. In addition, the user requires access
/// privilege for each of the descendent versions involved in the operation. The user also requires the remove privilege for each of the versions involved in the operation if a rollback and not a partial rollback operation is executed.
///
/// \param LongTransactionPrivileges_Remove
/// The long transaction privilege is required if the user should be allowed to remove the
/// long transaction in the progress of a commit or roll back operation.
///
/// \param LongTransactionPrivileges_Freeze
/// The user requires the access and freeze privileges on a long
/// transaction for which the user wants to gain exclusive access to a
/// long transaction.
///
[System::FlagsAttribute]
public __value enum LongTransactionPrivileges
{
	LongTransactionPrivileges_Access = FdoLongTransactionPrivileges_Access,
	LongTransactionPrivileges_Create = FdoLongTransactionPrivileges_Create,
	LongTransactionPrivileges_Commit = FdoLongTransactionPrivileges_Commit,
	LongTransactionPrivileges_Rollback = FdoLongTransactionPrivileges_Rollback,
	LongTransactionPrivileges_Remove = FdoLongTransactionPrivileges_Remove,
	LongTransactionPrivileges_Freeze = FdoLongTransactionPrivileges_Freeze
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION


