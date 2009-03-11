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

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SQL
interface class ISQLDataReader;

/// \ingroup (OSGeoFDOCommandsSQL)
/// \interface OSGeo::FDO::Commands::SQL::ISQLCommand
/// \brief
/// The ISQLCommand interface defines the SQL command, which supports the
/// execution of a SQL statement against an underlying RDBMS. Two execute
/// methods are provided to distinguish between statements that return table data
/// versus those that execute non query type operations.
public interface class ISQLCommand : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommand
{
public:
    /// \brief
    /// Gets the SQL statement to be executed as a string.
    /// 
    /// \return
    /// Returns the SQL statement.
    /// 
    /// \brief
    /// Sets the SQL statement to be executed as a string.
    /// 
    /// \param value 
    /// Input the SQL statement.
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::String^ SQLStatement
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// \brief
    /// Executes the SQL statement against the connection object and returns
    /// the number of rows affected.
    /// 
    /// \return
    /// Returns the number of rows affected.
    /// 
	System::Int32 ExecuteNonQuery();

    /// \brief
    /// Executes the SQL statement against the connection object and returns
    /// an ISQLDataReader.
    /// 
    /// \return
    /// Returns the data reader.
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReader^ ExecuteReader();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SQL


