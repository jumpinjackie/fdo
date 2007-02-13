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

#include "FDO\Commands\LongTransaction\mgILongTransactionSetReader.h"

class FdoILongTransactionSetReader;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION

/// \brief
/// The ILongTransactionSetReaderImp class is a concrete implementation of ILongTransactionSetReader.
/// The ILongTransactionSetReader interface provides forward only, read-only
/// functionality for enumerating the versions in the long transaction selection
/// set.  A reference to an ILongTransactionSetReader is returned from the
/// GetLongTransactionsInSet command. The initial position of the
/// ILongTransactionSetReader is prior to the first item. Thus, you must
/// call ReadNext to begin accessing any data.
private __gc class ILongTransactionSetReaderImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionSetReader
{
public:
    /// \brief
    /// Gets the name of the long transaction in the selection set currently
    /// being read.
    /// 
    /// \return
    /// Returns the long transaction name
    /// 
	System::String* GetLongTransactionName();

    /// \brief
    /// Gets the owner of the long transaction in the selection set currently
    /// being read.
    /// 
    /// \return
    /// Returns the owner of the long transaction
    /// 
	System::String* GetOwner();

    /// \brief
    /// Advances the reader to the next item. The default position of the
    /// reader is prior to the first item. Thus, you must call ReadNext
    /// to begin accessing any data.
    /// 
    /// \return
    /// Returns true if there is a next item.
    /// 
	System::Boolean ReadNext();

    /// \brief
    /// Closes the ILongTransactionSetReader object, freeing any resources
    /// it may be holding.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Close();

/// \cond DOXYGEN-IGNORE
protected:
	System::Void ReleaseUnmanagedObject();
/// \endcond

public private:
	ILongTransactionSetReaderImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
	{

	}

	inline FdoILongTransactionSetReader* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION


