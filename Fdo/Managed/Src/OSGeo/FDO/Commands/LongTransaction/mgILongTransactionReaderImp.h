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

#include "FDO\Commands\LongTransaction\mgILongTransactionReader.h"

class FdoILongTransactionReader;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION
public __gc __interface ILongTransactionReader;

/// \brief
/// The FdoILongTransactionReader interface provides forward only,
/// read-only functionality for enumerating long transactions.  A reference
/// to an FdoILongTransactionReader is returned from the GetLongTransactions
/// command. The initial position of the FdoILongTransactionReader is prior
/// to the first item. Thus, you must call ReadNext to begin accessing any
/// data.
private __gc class ILongTransactionReaderImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionReader
{
public:
    /// \brief
    /// Retrieves the name of the long transaction currently being
    /// read.
    /// 
    /// \return
    /// Returns the name of the long transaction.
    /// 
	System::String* GetName();

    /// \brief
    /// Retrieves the description of the long transaction currently
    /// being read.
    /// 
    /// \return
    /// Returns the description of the long transaction.
    /// 
	System::String* GetDescription();

    /// \brief
    /// Provides access to the direct descendent long transactions 
    /// for the long transaction being currently read. The caller can use the
    /// returned reader to get to the next level of descendents if required.
    /// The returned reader is empty if there are no descendents for the long
    /// transction.
    /// 
    /// \return
    /// Returns a long transaction reader with the direct descendents
    /// for the long transaction currently being read.
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionReader* GetChildren();

    /// \brief
    /// Provides access to the direct ascendant long transactions for
    /// the long transaction being currently read. The caller can use the 
    /// returned reader to get to the next level of ascendants if required.
    /// The returned reader is empty if there are no ascendants for the long
    /// transaction
    /// 
    /// \return
    /// Returns a long transaction reader with the direct ascendants 
    /// for the long transaction currently being read.
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionReader* GetParents();

    /// \brief
    /// Retrieves the owner of the long transaction currently being
    /// read.
    /// 
    /// \return
    /// Returns the owner name.
    /// 
	System::String* GetOwner();

    /// \brief
    /// Retrieves the creation date of the long transaction currently
    /// being read.
    /// 
    /// \return
    /// Returns the date when the long transaction was created.
    /// 
	System::DateTime GetCreationDate();

    /// \brief
    /// Returns true if the long transaction currently being read is
    /// the active long transaction.
    /// 
    /// \return
    /// Returns true if the long transaction is active.
    /// 
	System::Boolean IsActive();

    /// \brief
    /// Returns true if the long transaction currently being read is
    /// frozen.
    /// 
    /// \return
    /// Returns true if the long transaction is frozen.
    /// 
	System::Boolean IsFrozen();

    /// \brief
    /// Advances the reader to the next item. The default position of
    /// the reader is prior to the first item. Thus, you must call ReadNext
    /// to begin accessing any data.
    /// 
    /// \return
    /// Returns true if there is a next item.
    /// 
	System::Boolean ReadNext();

    /// \brief
    /// Closes the FdoILongTransactionReader object, freeing any
    /// resources it may be holding.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Close();

protected:
	__sealed System::Void ReleaseUnmanagedObject();

public private:
	ILongTransactionReaderImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
	{

	}

	inline FdoILongTransactionReader* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION


