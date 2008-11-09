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

#include "Common\mgIStreamReaderImp.h"

class FdoIoByteStreamReader;

BEGIN_NAMESPACE_OSGEO_COMMON_IO
public __gc class IoStream;

/// \ingroup (OSGeoFDOCommonIo)
/// \brief
///     IoByteStreamReader is an FDO reader for a Byte stream.
public __sealed __gc class IoByteStreamReader : public NAMESPACE_OSGEO_COMMON::IStreamReaderImp
{
public:
    /// \brief
    ///     Creates a Byte Stream reader. Reads binary data from an IoStream
    /// 
    /// \param stream 
    ///     Input the stream to read from
    /// 
	IoByteStreamReader(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream);
	
    /// \brief
    ///     Skips a number of items
    /// 
    /// \param offset 
    ///     Input number of items to skip
    /// 
	System::Void Skip(System::Int32 offset);

    /// \brief
    ///     Resets the current index to the stream start. Allows re-reading.
    /// 
	System::Void Reset();  

    /// \brief
    ///     Reads in the next block of items. The caller is responsible to allocate a buffer large 
    ///     enough to store data.
    /// 
    /// \param buffer 
    ///     Output array holding data read in.
    ///
    /// \return
    ///     Returns the number of items actually read in. When 0 (zero) then the 
    ///     end-of-stream was reached.
    /// 
	System::Int32 ReadNext(System::Byte buffer[]);

    /// \brief
    ///     Reads in the next block of items. Use ReadNext( buffer) to read in 
    ///     the entire stream. The caller is responsible to allocate a buffer large 
    ///     enough to store data.
    /// 
    /// \param buffer 
    ///     Output array holding data read in.
    ///
    /// \param offset 
    ///     Input index in the array indicating the beginning of the output buffer. 
    ///     If zero, then the buffer is reused. If the end of the buffer, then read in 
    ///     appending mode. Throws "out-of-bounds" exception if not in this range.
    ///
    /// \return
    ///     Returns the number of items actually read in. When 0 (zero) then the 
    ///     end-of-stream was reached.
    /// 
	System::Int32 ReadNext(System::Byte buffer[], System::Int32 offset);

    /// \brief
    ///     Reads in the next block of items. Use ReadNext( buffer) to read in 
    ///     the entire stream. The caller is responsible to allocate a buffer large 
    ///     enough to store data.
    /// 
    /// \param buffer 
    ///     Output array holding data read in.
    ///
    /// \param offset 
    ///     Input index in the array indicating the beginning of the output buffer. 
    ///     If zero, then the buffer is reused. If the end of the buffer, then read in 
    ///     appending mode. Throws "out-of-bounds" exception if not in this range.
    ///
    /// \param count 
    ///     Input number of items to be read in. If -1 read the entire stream.
    ///     Throws "out-of-bounds"  exception if not a positive value or -1.
    /// 
    /// \return
    ///     Returns the number of items actually read in. When 0 (zero) then the 
    ///     end-of-stream was reached.
    /// 
    System::Int32 ReadNext(System::Byte buffer[], System::Int32 offset, System::Int32 count);

    /// \brief
    ///     Gets the stream length
    /// 
    /// \return
    ///     Returns the size of the data source in number of items 
    /// 
    ///  
	__property System::Int64 get_Length();   

    /// \brief
    ///     Gets the current stream position. Position is 
    ///     measured by number of items from the stream start
    /// 
    /// \return
    ///     Returns the current index in the stream
    /// 
	__property System::Int64 get_Index();  

    /// \brief
    ///     Constructs a stream reader based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    ///     Input A Pointer to the unmanaged stream reader.
    /// 
    /// \param autoDelete 
    ///     Input Indicates if the constructed object should be automatically deleted 
    ///     once it no longer referenced.
    /// 
	IoByteStreamReader(System::IntPtr unmanaged, System::Boolean autoDelete);

/// \cond DOXYGEN-IGNORE
public private:
	inline FdoIoByteStreamReader* GetImpObj();
/// \endcond
};

END_NAMESPACE_OSGEO_COMMON_IO


