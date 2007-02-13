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

#include "Common\Io\mgIoStream.h"

class FdoIoMemoryStream;

BEGIN_NAMESPACE_OSGEO_COMMON_IO

/// \brief
///     IoMemoryStream provides streamed I/O to and from memory.
public __sealed __gc class IoMemoryStream : public NAMESPACE_OSGEO_COMMON_IO::IoStream
{
public:
    /// \brief
    ///     creates the memory stream with its default size. The stream manages its own storage.
    /// 
	IoMemoryStream();

    /// \brief
    ///     creates the memory stream. The stream manages its own storage.
    /// 
    /// \param bufferSize 
    ///     Input memory allocation increment. Amount of new memory to 
    ///     allocated when the stream needs more memory.
    /// 
	IoMemoryStream(System::UInt32 bufferSize);

    /// \brief
    ///     Constructs a stream based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    ///     Input A Pointer to the unmanaged stream.
    /// 
    /// \param autoDelete 
    ///     Input Indicates if the constructed object should be automatically deleted 
    ///     once it no longer referenced.
    /// 
	IoMemoryStream(System::IntPtr unmanaged, System::Boolean autoDelete);

/// \cond DOXYGEN-IGNORE
public private:
	inline FdoIoMemoryStream* GetImpObj();
/// \endcond
};

END_NAMESPACE_OSGEO_COMMON_IO


