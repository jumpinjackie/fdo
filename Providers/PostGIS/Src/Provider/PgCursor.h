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
class PgCursor : public FdoIDisposable //private boost::noncopyable
{
public:

    /// Type of FDO smart pointer for PgCursor class.
    typedef FdoPtr<PgCursor> Ptr;

    /// Type of pointer to PostgreSQL result set accessible through the cursor.
    typedef PGresult const* ResultPtr;

    /// Enumeration describes supported directions for moving cursor.
    /// \todo Not used yet!
    enum Direction
    {
        eForward = 0,
        eBackward,
    };

    /// Constructor creates a named cursor associated with given connection.
    /// It throws an exception if connection is invalid.
    PgCursor(Connection* conn, std::string const& name);

    /// Get name of a cursor.
    /// \return String with name used in DECLARE statement.
    char const* GetName() const;

    /// Get read-only pointer to results associated with FETCH command.
    ResultPtr GetFetchResult() const;

    /// Get number of columns (fields) returned in each row of the query result.
    FdoSize GetFieldsCount() const;

    /// Get column name associated with the given column number.
    /// Column numbers start at 0.
    FdoStringP GetFieldName(FdoSize number) const;

    /// Get column number associated with the given column name.
    FdoSize GetFieldNumber(FdoStringP const& name) const;
    
    /// Get FDO type of field associated with the given column name.
    FdoDataType GetFieldType(FdoStringP const& name) const;

    /// Declare a cursor using given SELECT query.
    /// \remarks
    /// It's equivalent of sequence of following SQL commands:
    /// \code
    /// BEGIN work;
    /// DECLARE <name> CURSOR FOR SELECT <...>;
    /// \endcode
    ///
    void Declare(char const* query);

    /// Close a cursor.
    /// \remarks
    /// It's equivalent of sequence of following SQL commands:
    /// \code
    /// COMMIT work;
    /// CLOSE <name>; \endcode
    ///
    void Close();

    /// Fetch tuples.
    ResultPtr FetchNext();


protected:

    /// Destructor closes a cursor on destroy.
    virtual ~PgCursor();

    //
    // FdoIDisposable interface
    //

    /// Dispose this object.
    void Dispose();

private:

    //
    // Private data members
    //

    // Pointer to connection object of current session.
    Connection::Ptr mConn;

    // Name of declared cursor.
    FdoStringP mName;

    // Handle to result of describe portal command executed on the cursor.
    PGresult* mDescRes;

    // Handle to result of FETCH command executed on the cursor.
    PGresult* mFetchRes;

    // Flag indicating if cursor is closed or active.
    bool mIsClosed;

    //
    // Private operations
    //

    // Check pre-conditions before running cursor operations.
    void Validate();

    // Release resources associated with describe portal result.
    void ClearDescribeResult();

    // Release resources associated with fetch data result.
    void ClearFetchResult();

    // Request to re-run describe portal command.
    void Describe();

};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_PGCURSOR_H_INCLUDED
