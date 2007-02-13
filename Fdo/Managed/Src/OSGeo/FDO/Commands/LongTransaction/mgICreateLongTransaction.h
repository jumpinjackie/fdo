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

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION

/// \brief
/// The ICreateLongTransaction interface defines the CreateLongTransaction
/// command which creates a long transaction that is based upon the currently
/// active long transaction. If no long transaction is active, then the long
/// transaction is based upon the root data. Input to the create long
/// transaction command includes a name and description for the new long
/// transaction.
public __gc __interface ICreateLongTransaction : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommand
{
public:
    /// \brief
    /// Gets the name of the long transaction to create as a string.
    /// 
    /// \return
    /// Returns the name of the long transaction
    /// 
	__property System::String* get_Name();

    /// \brief
    /// Sets the name of the long transaction to create as a string.
    /// 
    /// \param value 
    /// Input the name of the long transaction
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Name(System::String* value);

    /// \brief
    /// Gets the description of the long transaction to create as a string.
    /// 
    /// \return
    /// Returns the description of the long transaction
    /// 
	__property System::String* get_Description();

    /// \brief
    /// Sets the description of the long transaction to create as a string.
    /// 
    /// \param value 
    /// Input the description of the long transaction
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Description(System::String* value);

    /// \brief
    /// Executes the create long transaction command.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Execute();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION


