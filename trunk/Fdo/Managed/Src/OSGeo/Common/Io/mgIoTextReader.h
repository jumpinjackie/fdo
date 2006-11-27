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

#include "Runtime\Disposable.h"

class FdoIoTextReader;

BEGIN_NAMESPACE_OSGEO_COMMON_IO
public __gc class IoStream;

/// \brief
///     IoTextReader reads from a binary stream containing UTF8 characters.
///     It formats these UTF8 characters into Unicode wide characters. This class is 
///     not yet complete, since the Read functions still have to be added.
public __sealed __gc class IoTextReader : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    ///     Constructs a text reader on a file
    /// 
    /// \param fileName 
    /// Input name of the file to read.
    /// 
	IoTextReader(System::String* fileName);

    /// \brief
    ///     Constructs a text reader on a stream
    /// 
    /// \param stream 
    /// Input the stream to read.
    /// 
	IoTextReader(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream);

    /// \brief
    ///     Gets the underlying stream. If a stream was passed to this object
    ///     then this stream is returned.
    ///     Otherwise, an auto-generated stream (wrapped around the  file name 
    ///     that was passed to this object) is returned.
    /// 
    /// \return
    ///     Returns the underlying stream.
    /// 
    ///  
	__property NAMESPACE_OSGEO_COMMON_IO::IoStream* get_Stream();

	IoTextReader(System::IntPtr unmanaged, System::Boolean autoDelete);

protected:
	System::Void ReleaseUnmanagedObject();

public private:
	inline FdoIoTextReader* GetImpObj();
};

END_NAMESPACE_OSGEO_COMMON_IO


