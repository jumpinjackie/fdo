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
#ifndef FDOPOSTGIS_PGCURSOR_H_INCLUDED
#define FDOPOSTGIS_PGCURSOR_H_INCLUDED

#include "Connection.h"
#include <string>
#include <boost/noncopyable.hpp>
#include <libpq-fe.h>

namespace fdo { namespace postgis {

/// Implementation of cursor logic for querying PostgreSQL database.
/// This class declares cursor in frame of a transaction,
/// also provides interface to fetch and access data.
///
class PgCursor : private boost::noncopyable
{
public:

    /// Constructor creates a named cursor associated with given connection.
    /// It throws an exception if connection is invalid.
    PgCursor(Connection* conn, std::string const& name);

    /// Destructor closes a cursor on destroy.
    ~PgCursor();

    /// Declare a cursor using given SELECT query.
    /// \remarks
    /// It's equivalent of sequence of following SQL commands:
    /// \code
    /// BEGIN work;
    /// DECLARE <name> CURSOR FOR SELECT <...>;
    /// \endcode
    ///
    void Declare(std::string const& query);
    void Declare(char const* query);

    /// Close a cursor.
    /// \remarks
    /// It's equivalent of sequence of following SQL commands:
    /// \code
    /// COMMIT work;
    /// CLOSE <name>; \endcode
    ///
    void Close();

private:

    //
    // Private data members
    //

    // Pointer to connection object of current session.
    FdoPtr<Connection> mConn;

    // Name of declared cursor.
    std::string mName;

    // Pointer to result structure used by cursor.
    PGresult* mPgResult;

    //
    // Private operations
    //

    // Check pre-conditions before running cursor operations.
    void Validate();

};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_PGCURSOR_H_INCLUDED
