#ifndef IGETIMAGEFORMATS_H
#define IGETIMAGEFORMATS_H
// 

//
// Copyright (C) 2004-2007  Autodesk, Inc.
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifdef _WIN32
#pragma once
#endif

#include <FdoStd.h>
#include <Fdo/Commands/ICommand.h>
#include <Fdo/Connections/IConnection.h>

/// \brief
/// The FdoWmsIGetImagefromats interface defines the 
/// FdoWmsGetImageFormats command, which retrieves the supported image 
/// formats supported by the connection. The Execute operation 
/// returns an FdoStringCollection object containing the ImageFormat names.
class FdoWmsIGetImagefromats : public FdoICommand
{
    friend class FdoIConnection;

public:
    /// \brief
    /// Executes the FdoWmsIGetImagefromats command and returns a 
    /// FdoStringCollection, which contains all supported image 
	/// formats supported by the WMS server.
    /// 
    /// \return
    /// Returns a collection of image formats supported by the WMS server
    /// 
    FDO_API virtual FdoStringCollection * Execute() = 0;
};
#endif
