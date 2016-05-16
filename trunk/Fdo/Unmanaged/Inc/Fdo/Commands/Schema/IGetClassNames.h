#ifndef _IGETCLASSNAMES_H_
#define _IGETCLASSNAMES_H_
// 

//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifdef _WIN32
#pragma once
#endif

#include <FdoStd.h>
#include <Fdo/Commands/ICommand.h>
#include <Fdo/Schema/FeatureSchemaCollection.h>
#include <Fdo/Connections/IConnection.h>

/// \brief
/// The FdoIGetClassNames interface defines the GetClassNames command, which
/// retrieves the list of available class names, qualified by the feature schema name.
/// The Execute operation returns an FdoStringCollection
/// object.
class FdoIGetClassNames : public FdoICommand
{
    friend class FdoIConnection;

public:
    /// \brief
    /// Gets the name of the schema for the enumeration. This function is optional;
    /// if not specified, execution of the command will enumerate the classes in all schemas.
    /// 
    /// \return
    /// Returns the schema name
    /// 
    FDO_API virtual FdoString* GetSchemaName() = 0;

    /// \brief
    /// Sets the name of the schema for the enumeration. This function is optional; if not
    /// specified execution of the command will enumerate the classes in all schemas.
    /// 
    /// \param value 
    /// Input the schema name
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API virtual void SetSchemaName(FdoString* value) = 0;

    /// \brief
    /// Executes the GetClassNames command and returns a 
    /// FdoStringCollection. If the specified schema name does not exist,
    /// the Execute method throws an exception.
    /// 
    /// \return
    /// Returns the string collection of the feature classes for the specified schema.
    FDO_API virtual FdoStringCollection* Execute() = 0;
};
#endif


