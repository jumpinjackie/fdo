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
#include "Common\Io\mgIoStream.h"
#include "Common\Io\VirtualFdoIoStream.h"

using namespace System::Runtime::InteropServices;

FdoSize VirtualFdoIoStream::Read(FdoByte* buffer, FdoSize count)
{
	if (!WrapperCallWrapper::IsCalling(wrapperCallBits, ReadBit))
	{
		WrapperCallWrapper ctx(wrapperCallBits, ReadBit);

		Byte mgBuffer[] = new Byte[count];
		Int32 rCount = GetWrapper()->Read(mgBuffer, count);
		IntPtr upBuffer = buffer;
		Marshal::Copy(upBuffer, mgBuffer, 0, rCount);
		return rCount;
	}
	else
	{
		return 0;
	}
}

FdoVoid VirtualFdoIoStream::Write(FdoByte* buffer, FdoSize count)
{
	if (!WrapperCallWrapper::IsCalling(wrapperCallBits, WriteBufferBit))
	{
		WrapperCallWrapper ctx(wrapperCallBits, WriteBufferBit);

		Byte mgBuffer[] = new Byte[count];
		for (UInt32 i = 0; i < count; i++)
		{
			mgBuffer[i] = *(buffer + i);
		}

		GetWrapper()->Write(mgBuffer, count);
	}
}

FdoVoid VirtualFdoIoStream::Write(FdoIoStream* stream, FdoSize count)
{
	if (!WrapperCallWrapper::IsCalling(wrapperCallBits, WriteStreamBit))
	{
		WrapperCallWrapper ctx(wrapperCallBits, WriteStreamBit);

		GetWrapper()->Write(new NAMESPACE_OSGEO_COMMON_IO::IoStream(stream, true), count);
	}
}

FdoVoid VirtualFdoIoStream::SetLength(FdoInt64 length)
{
	if (!WrapperCallWrapper::IsCalling(wrapperCallBits, SetLengthBit))
	{
		WrapperCallWrapper ctx(wrapperCallBits, SetLengthBit);

		GetWrapper()->set_Length(length);
	}
}

FdoInt64 VirtualFdoIoStream::GetLength()
{
	if (!WrapperCallWrapper::IsCalling(wrapperCallBits, GetLengthBit))
	{
		WrapperCallWrapper ctx(wrapperCallBits, GetLengthBit);

		return GetWrapper()->get_Length();
	}

	return 0;
}

FdoVoid VirtualFdoIoStream::Skip(FdoInt64 offest)
{
	if (!WrapperCallWrapper::IsCalling(wrapperCallBits, SkipBit))
	{
		WrapperCallWrapper ctx(wrapperCallBits, SkipBit);

		GetWrapper()->Skip(offest);
	}
}

FdoInt64 VirtualFdoIoStream::GetIndex()
{
	if (!WrapperCallWrapper::IsCalling(wrapperCallBits, GetIndexBit))
	{
		WrapperCallWrapper ctx(wrapperCallBits, GetIndexBit);

		return GetWrapper()->get_Index();
	}

	return 0;
}

FdoVoid VirtualFdoIoStream::Reset()
{
	if (!WrapperCallWrapper::IsCalling(wrapperCallBits, ResetBit))
	{
		WrapperCallWrapper ctx(wrapperCallBits, ResetBit);

		GetWrapper()->Reset();
	}
}

FdoBoolean VirtualFdoIoStream::CanRead()
{
	if (!WrapperCallWrapper::IsCalling(wrapperCallBits, CanReadBit))
	{
		WrapperCallWrapper ctx(wrapperCallBits, CanReadBit);

		return GetWrapper()->get_CanRead();
	}

	return true;
}

FdoBoolean VirtualFdoIoStream::CanWrite()
{
	if (!WrapperCallWrapper::IsCalling(wrapperCallBits, CanWriteBit))
	{
		WrapperCallWrapper ctx(wrapperCallBits, CanWriteBit);

		return GetWrapper()->get_CanWrite();
	}

	return true;
}

FdoBoolean VirtualFdoIoStream::HasContext()
{
	if (!WrapperCallWrapper::IsCalling(wrapperCallBits, HasContextBit))
	{
		WrapperCallWrapper ctx(wrapperCallBits, HasContextBit);

		return GetWrapper()->get_HasContext();
	}

	return true;
}
