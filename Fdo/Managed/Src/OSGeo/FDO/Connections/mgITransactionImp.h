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

#include "FDO\Connections\mgITransaction.h"

#include <Fdo\Connections\ITransaction.h>

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS

/// \brief
/// The ITransaction interface represents a transaction to be performed in a DataStore, and is implemented
/// by feature providers that support transactions.
private __gc class ITransactionImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_FDO_CONNECTIONS::ITransaction
{
public:
    /// \brief
    ///  Gets the IConnection object associated with the transaction.
    /// 
    /// \return
    /// Returns IConnection object
    /// 
	__property NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnection* get_Connection();

    /// \brief
    ///  Commits the transaction.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Commit();

    /// \brief
    ///  Rolls back a transaction from a pending state.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Rollback();

public private:
	ITransactionImp(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoITransaction* GetImpObj();

/// \cond DOXYGEN-IGNORE
protected:
	System::Void ReleaseUnmanagedObject();
/// \endcond
};

END_NAMESPACE_OSGEO_FDO_CONNECTIONS


