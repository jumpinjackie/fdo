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
#ifndef FDOPOSTGIS_TRANSACTION_H_INCLUDED
#define FDOPOSTGIS_TRANSACTION_H_INCLUDED

#include "Connection.h"

namespace fdo { namespace postgis {

/// Defines a single transaction instances.
/// NOTE: PostgreSQL does not support nested transactions,
/// so the PostGIS provider implements with idea of Soft Transactions
/// The connection manager tracks transaction scope levels, and only outermost scope
/// is a real (hard) transaction.
///
class Transaction : public FdoITransaction
{
public:

    /// Type of FDO smart pointer for the class.
    typedef FdoPtr<Transaction> Ptr;
    
    /// Default constructor.
    Transaction(Connection* conn);
    
    //
    // FdoITransaction interface
    //

    /// Get handle to current connection associated with the transaction.
    FdoIConnection* GetConnection();

    /// Commit the transaction.        
    void Commit();

    /// Roll back a transaction from a pending state.
    void Rollback();

protected:

    /// Destructor.
    virtual ~Transaction();

    //
    // FdoIDisposable interface
    //

    /// Dispose this object.
    void Dispose();

private:
	
	Connection::Ptr mConn;
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_TRANSACTION_H_INCLUDED
