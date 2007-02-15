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

/// \ingroup (enums)
/// \brief
/// The LongTransactionPrivilegeOperations enumeration defines the list
/// of available operations that can be performed by the
/// ChangeLongTransactionPrivileges command.
[System::FlagsAttribute]
public __value enum LongTransactionPrivilegeOperations
{
    /// Indicates the user is to be granted the specified privileges.
	LongTransactionPrivilegeOperations_Grant = FdoLongTransactionPrivilegeOperations_Grant,

    /// Indicates the specified privileges are to be revoked from the user.
	LongTransactionPrivilegeOperations_Revoke = FdoLongTransactionPrivilegeOperations_Revoke
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION


