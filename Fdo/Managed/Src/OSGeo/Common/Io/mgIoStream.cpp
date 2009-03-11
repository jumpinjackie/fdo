/*
* 
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

#include "stdafx.h"
#include "Common\mgException.h"
#include "Common\Io\mgIoStream.h"
#include "Common\Io\VirtualFdoIoStream.h"

NAMESPACE_OSGEO_COMMON_IO::IoStream::IoStream() : Disposable(IntPtr(new VirtualFdoIoStream()), true)
{
	static_cast<VirtualFdoIoStream*>(GetImpObj())->SetWrapper(this);
}

NAMESPACE_OSGEO_COMMON_IO::IoStream::IoStream(System::IntPtr unmanaged, System::Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoIoStream* NAMESPACE_OSGEO_COMMON_IO::IoStream::GetImpObj()
{
	return static_cast<FdoIoStream*>(__super::UnmanagedObject.ToPointer());
}

System::UInt32 NAMESPACE_OSGEO_COMMON_IO::IoStream::Read(array<System::Byte>^ buffer, System::UInt32 count)
{
	pin_ptr<FdoByte> upByte = &buffer[0];
	FdoSize rCount;

	EXCEPTION_HANDLER(rCount = GetImpObj()->Read(upByte, count))

	return (unsigned int)rCount;
}


System::Void NAMESPACE_OSGEO_COMMON_IO::IoStream::Write(array<System::Byte>^ buffer, System::UInt32 count)
{
	pin_ptr<FdoByte> upByte = &buffer[0];

	EXCEPTION_HANDLER(GetImpObj()->Write(upByte, count))
}

System::Void NAMESPACE_OSGEO_COMMON_IO::IoStream::Write(NAMESPACE_OSGEO_COMMON_IO::IoStream^ stream)
{
	EXCEPTION_HANDLER(GetImpObj()->Write(stream->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_COMMON_IO::IoStream::Write(NAMESPACE_OSGEO_COMMON_IO::IoStream^ stream, System::UInt32 count)
{
	EXCEPTION_HANDLER(GetImpObj()->Write(stream->GetImpObj(), count))
}

System::Void NAMESPACE_OSGEO_COMMON_IO::IoStream::Length::set(System::Int64 length)
{
	EXCEPTION_HANDLER(GetImpObj()->SetLength(length))
}

System::Int64 NAMESPACE_OSGEO_COMMON_IO::IoStream::Length::get()
{
	FdoInt64 len;

	EXCEPTION_HANDLER(len = GetImpObj()->GetLength())

	return len;
}

System::Void NAMESPACE_OSGEO_COMMON_IO::IoStream::Skip(System::Int64 count)
{
	EXCEPTION_HANDLER(GetImpObj()->Skip(count))
}

System::Boolean NAMESPACE_OSGEO_COMMON_IO::IoStream::CanRead::get()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->CanRead())

	return result;
}

System::Boolean NAMESPACE_OSGEO_COMMON_IO::IoStream::CanWrite::get()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->CanWrite())

	return result;
}

System::Boolean NAMESPACE_OSGEO_COMMON_IO::IoStream::HasContext::get()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->HasContext())

	return result;
}

System::Int64 NAMESPACE_OSGEO_COMMON_IO::IoStream::Index::get()
{
	FdoInt64 index;

	EXCEPTION_HANDLER(index = GetImpObj()->GetIndex())

	return index;
}

System::Void NAMESPACE_OSGEO_COMMON_IO::IoStream::Reset()
{
	EXCEPTION_HANDLER(GetImpObj()->Reset())
}

System::Void NAMESPACE_OSGEO_COMMON_IO::IoStream::Close()
{
    ReleaseUnmanagedObject();
}
