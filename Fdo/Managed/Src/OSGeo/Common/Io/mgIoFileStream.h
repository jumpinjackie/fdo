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

#pragma  once

#include "Common\Io\mgIoStream.h"

class FdoIoFileStream;

BEGIN_NAMESPACE_OSGEO_COMMON_IO

/// \ingroup (OSGeoFDOCommonIo)
/// \brief
///     IoFileStream provides streamed I/O on a disk file.
public ref class IoFileStream sealed : public NAMESPACE_OSGEO_COMMON_IO::IoStream
{
public:
    /// \brief
    ///     creates the stream for a named file.
    /// 
    /// \param fileName 
    ///     Input the file name
    ///
    /// \param accessModes 
    ///     Input File access modes. Same as the fopen() C run-time library function
    ///     (mode parameter).
    /// 
	IoFileStream(System::String^ fileName, System::String^ accessModes);

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
	IoFileStream(System::IntPtr unmanaged, System::Boolean autoDelete);

/// \cond DOXYGEN-IGNORE
internal:
	inline FdoIoFileStream* GetImpObj();
/// \endcond
};

END_NAMESPACE_OSGEO_COMMON_IO


