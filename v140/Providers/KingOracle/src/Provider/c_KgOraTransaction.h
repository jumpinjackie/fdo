/*
* Copyright (C) 200t  SL-King d.o.o
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
*/

#ifndef _c_KgOraTransaction_h
#define _c_KgOraTransaction_h

#include "c_KgOraConnection.h"


class c_KgOraTransaction : public FdoITransaction
{
public:

    /// Type of FDO smart pointer for the class.
    typedef FdoPtr<c_KgOraTransaction> Ptr;
    
    /// Default constructor.
    c_KgOraTransaction(c_KgOraConnection* Connection);
    
    //
    // FdoIc_KgOraTransaction interface
    //

    /// Get handle to current connection associated with the c_KgOraTransaction.
    FdoIConnection* GetConnection();

    /// Commit the c_KgOraTransaction.        
    void Commit();

    /// Roll back a c_KgOraTransaction from a pending state.
    void Rollback();

protected:

    /// Destructor.
    virtual ~c_KgOraTransaction();

    //
    // FdoIDisposable interface
    //

    /// Dispose this object.
    void Dispose();

private:
	
	FdoPtr<c_KgOraConnection> m_Connection;
};



#endif // FDOPOSTGIS_c_KgOraTransaction_H_INCLUDED
