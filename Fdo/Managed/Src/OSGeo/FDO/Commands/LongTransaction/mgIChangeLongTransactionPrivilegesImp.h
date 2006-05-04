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
#include "FDO\Commands\LongTransaction\mgIChangeLongTransactionPrivileges.h"
#include "FDO\Commands\LongTransaction\mgLongTransactionPrivilegeOperations.h"

class FdoIChangeLongTransactionPrivileges;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION

/// \brief
/// The FdoIChangeLongTransactionPrivileges interface defines the
/// ChangeLongTransactionPrivileges command, which grants or revokes long
/// transaction privileges for the named user. The ability to change the 
/// privileges might be restricted by the datastore security if the long 
/// transaction doesn’t belong to the user executing the command.
private __gc class IChangeLongTransactionPrivilegesImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, public NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IChangeLongTransactionPrivileges
{
public:
    /// \brief
    /// Gets the name of the long transaction for which privileges should be
    /// changed.
    /// 
    /// \return
    /// Returns the name of the long transaction
    /// 
	__property System::String* get_LongTransactionName();

    /// \brief
    /// Sets the name of the long transaction for which privileges should be
    /// changed.
    /// 
    /// \param value 
    /// Input the name of the long transaction
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_LongTransactionName(System::String* value);

    /// \brief
    /// Gets the name of the user for which to grant or revoke privileges.
    /// 
    /// \return
    /// Returns the user name
    /// 
	__property System::String* get_UserName();

    /// \brief
    /// Sets the name of the user for which to grant or revoke privileges.
    /// 
    /// \param value 
    /// Input the user name
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_UserName(System::String* value);

    /// \brief
    /// Gets the privilege flags to grant or revoke.The returned value may be
    /// any combination of the values from the FdoLongTransactionPrivileges
    /// enumeration combined via a bit-wise or operation.
    /// 
    /// \return
    /// Returns the privileges as a bit masked value based on FdoLongTransactionPrivileges enumeration
    /// 
	__property System::Int32 get_Privileges();

    /// \brief
    /// Sets the privilege flags to grant or revoke. The privileges value may be
    /// any combination of the values from the FdoLongTransactionPrivileges
    /// enumeration combined via a bit-wise or operation.
    /// 
    /// \param value 
    /// Input the privileges as a bit masked value based on FdoLongTransactionPrivileges enumeration
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Privileges(System::Int32 value);

    /// \brief
    /// Gets the operation to perform, grant, or revoke.
    /// 
    /// \return
    /// Returns the operation to perform
    /// 
	__property NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::LongTransactionPrivilegeOperations get_Operation();

    /// \brief
    /// Sets the operation to perform, grant, or revoke.
    /// 
    /// \param value 
    /// Input the operation to perform
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Operation(NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::LongTransactionPrivilegeOperations value);

    /// \brief
    /// Executes the ChangeLongTransactionPrivilege command.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Execute();

	System::Void Dispose();

public private:
	IChangeLongTransactionPrivilegesImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoIChangeLongTransactionPrivileges* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION


