//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#ifndef FDOPOSTGIS_SQLCOMMAND_H_INCLUDED
#define FDOPOSTGIS_SQLCOMMAND_H_INCLUDED

#include "Command.h"
#include <Fdo/Commands/Sql/ISQLCommand.h>

//
// Forward declarations
//
class FdoISQLDataReader;

namespace fdo { namespace postgis {

// Forward declarations of internal types
class Connection;

/// Implementation of SQL command interface.
/// This command is used for execution of SQL statement against
/// currently connected PostgreSQL/PostGIS datastore.
///
class SQLCommand : public Command<FdoISQLCommand>
{
public:

    /// Constructor create new SQL command associated with given connection.
    SQLCommand(Connection* Connection);

    //
    // FdoISQLCommand interface
    //

    /// Get a string with SQL statement assigned to the command.
    virtual FdoString* GetSQLStatement();

    /// Set new SQL statement to be executed by the command.
    virtual void SetSQLStatement(FdoString* sql);
    
    /// Execute assigned SQL statement against current connection.
    ///
    /// \return
    /// Number of rows (tuples) affected by the command execution.
    ///
    virtual FdoInt32 ExecuteNonQuery();

    /// Execute assigned SQL statement against current connection and
    /// create data reader associated with the result set.
    /// 
    /// \return
    /// Returns the data reader.
    /// 
    virtual FdoISQLDataReader* ExecuteReader();

protected:

    // Destructor.
    virtual ~SQLCommand();

private:
	
    //
    // Private data members
    //

    typedef Command<FdoISQLCommand> Base;

    FdoStringP mSql;

    //
    // Private operations
    //

};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_SQLCOMMAND_H_INCLUDED
