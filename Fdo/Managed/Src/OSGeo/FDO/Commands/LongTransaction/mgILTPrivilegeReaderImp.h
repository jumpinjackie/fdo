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

#include "FDO\Commands\LongTransaction\mgILongTransactionPrivilegeReader.h"

class FdoILongTransactionPrivilegeReader;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION

/// \brief
/// The ILongTransactionPrivilegeReaderImp class is a concrete
/// implementation of ILongTransactionPrivilegeReader.
/// The ILongTransactionPrivilegeReader interface provides forward only,
/// read-only functionality for enumerating long transaction privileges.
/// A reference to an FdoILongTransactionPrivilegeReader is returned from
/// the GetLongTransactionPrivileges command. The initial position of
/// the ILongTransactionPrivilegeReader is prior to the first item.
/// Thus, you must call ReadNext to begin accessing any data.
private __gc class ILongTransactionPrivilegeReaderImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, 
                                                        public NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionPrivilegeReader
{
public:
    /// \brief
    /// Gets the name of the user currently being read.
    /// 
    /// \return
    /// Returns the user name
    /// 
	System::String* GetUsername();

    /// \brief
    /// Gets the privileges granted to the user currently being read. The
    /// returned value may be any combination of the values from the
    /// LongTransactionPrivileges enumeration combined via a bit-wise or
    /// operation.
    /// 
    /// \return
    /// Returns the privileges as a bit masked value based on LongTransactionPrivileges enumeration
    /// 
	System::Int32 GetPrivileges();

    /// \brief
    /// Advances the reader to the next item. The default position of the
    /// reader is prior to the first item. Therefore you must call ReadNext
    /// to begin accessing any data.
    /// 
    /// \return
    /// Returns true if there is a next item
    /// 
	System::Boolean ReadNext();

    /// \brief
    /// Closes the ILongTransactionPrivilegesReader object, freeing any
    /// resources it may be holding.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Close();

public private:
	ILongTransactionPrivilegeReaderImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
	{

	}

	inline FdoILongTransactionPrivilegeReader* GetImpObj();

/// \cond DOXYGEN-IGNORE
protected:
	__sealed System::Void ReleaseUnmanagedObject();
/// \endcond

};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION


