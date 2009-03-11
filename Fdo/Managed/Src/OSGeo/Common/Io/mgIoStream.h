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

class VirtualFdoIoStream;
class FdoIoStream;

BEGIN_NAMESPACE_OSGEO_COMMON_IO

/// \ingroup (OSGeoFDOCommonIo)
/// \brief
///    IoStream defines the interface for all FDO streams. Streamed I/O support
///    for various locations, such as files, memory, etc. can be provided by classes
///    that implement this interface.
public ref class IoStream : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    ///     reads the number of bytes indicated by count into the given buffer, 
    ///     or the number of bytes after the current stream position, whichever is less. 
    ///     The stream’s current position is moved ahead by the number of bytes read.
    /// 
    /// \param buffer 
    ///     Output read into this buffer
    ///
    /// \param count 
    ///     Input read this number of bytes into the buffer.
    ///     The caller is responsible for allocating a buffer that is large enough to hold the bytes.
    /// 
    /// \return
    ///     Returns the number of bytes that were read. 0 if already at the
    ///     end of the stream.
    /// 
	virtual System::UInt32 Read(array<System::Byte>^ buffer, System::UInt32 count);

    /// \brief
    ///     writes the number of bytes indicated by count, from the given buffer, 
    ///     to the stream. The current position is moved ahead by the number of bytes 
    ///     written.
    /// 
    /// \param buffer 
    ///     Intput write from this buffer
    ///
    /// \param count 
    ///     Input number of bytes to write
    /// 
    /// \remarks
    ///     Note: Write will overwrite some of the contents of the stream 
    ///     if the current position is not at the end of the stream.
    /// 
	virtual System::Void Write(array<System::Byte>^ buffer, System::UInt32 count);

    /// \brief
    ///     reads the number of bytes from the given stream, and writes them 
    ///     to this stream. 
    /// 
    /// \param stream 
    ///     Input write from this buffer
    ///
	virtual System::Void Write(NAMESPACE_OSGEO_COMMON_IO::IoStream^ stream);

    /// \brief
    ///     reads the number of bytes from the given stream, and writes them 
    ///     to this stream. 
    /// 
    /// \param stream 
    ///     Input write from this buffer
    ///
    /// \param count 
    ///     Input the maximum number of bytes to read and write.
    ///     When 0, all remaining bytes are read from the given stream.
    /// 
	virtual System::Void Write(NAMESPACE_OSGEO_COMMON_IO::IoStream^ stream, System::UInt32 count);

    /// \brief
    ///     sets the position to the start of the stream.
    /// 
	virtual System::Void Reset();

    /// \brief
    ///     skips over part of the stream.
    /// 
    /// \param count 
    ///     The number of bytes to skip. if positive then 
    ///     the current position is moved forward. If negative, the position is
    ///     moved backward. The position will remain between the start and end 
    ///     of the stream. The position is set to the end of the stream if the 
    ///     given offset would put it past the end. Similarily, if the position is 
    ///     set to the start if the offset would put it before the start.
    /// 
	virtual System::Void Skip(System::Int64 count);

    /// \brief
    ///     truncates the stream to the indicated length.
    /// 
    /// \param length 
    ///     Input the new length (in bytes) for the stream. If this is 
    ///     longer than the current stream length then the stream's length
    ///     is not changed.
    /// 
    /// \brief
    ///     gets the current length of the stream.
    /// 
    /// \return
    ///     Returns the length in bytes. Returns -1 if the length is unknown
    ///     or undefined.
    /// 
    virtual property System::Int64 Length
    {
        System::Int64 get();
        System::Void set(System::Int64 length);
    }

    /// \brief
    ///     gets the current position for the stream.
    /// 
    /// \return
    ///     Returns the position in bytes from the start. When 0, the 
    ///     the position is at the start of the stream. When Index() ==
    ///     Length() the position is at the end of the stream.
    /// 
    virtual property System::Int64 Index
    {
        System::Int64 get();
    }

    /// \brief
    ///     Gets the reading capability of the stream.
    /// 
    /// \return
    ///     Returns true if the stream can be read.
    /// 
    virtual property System::Boolean CanRead
    {
        System::Boolean get();
    }

    /// \brief
    ///     Gets the writing capability of the stream.
    /// 
    /// \return
    ///     Returns true if the stream can be written to.
    /// 
    virtual property System::Boolean CanWrite
    {
        System::Boolean get();
    }

    /// \brief
    ///     returns whether the stream has any positional context.
    ///     Streams without context can only be read and written in a forward-only
    ///     manner. These streams do not allow position and length changes.
    /// 
    /// \return
    ///    Returns true if the stream has context. Returns false if it does 
    ///    not. When a stream does not have context, the SetLength(), Skip() and 
    ///    Reset() functions are not supported.
    /// 
    ///  
    virtual property System::Boolean HasContext
    {
        System::Boolean get();
    }

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
	IoStream(System::IntPtr unmanaged, System::Boolean autoDelete);

    /// \brief
    ///     Closes the current stream and releases any resources (such as sockets and file handles) associated with the current stream. 
    /// 
    /// \remarks
    ///     Note: A call to Close is required for proper operation of a stream. 
    ///     Following a call to Close, other operations on the stream could throw exceptions. 
    ///     If the stream is already closed, a call to Close throws no exceptions.
    /// 
	System::Void Close();

/// \cond DOXYGEN-IGNORE
protected:
	IoStream();

internal:
	inline FdoIoStream* GetImpObj();
/// \endcond
};

END_NAMESPACE_OSGEO_COMMON_IO


