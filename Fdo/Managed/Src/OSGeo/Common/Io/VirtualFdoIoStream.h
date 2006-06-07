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

#include <FdoCommon.h>
#include "Runtime\ProtocolExt.h"
#include "Common\Io\mgIoStream.h"

class VirtualFdoIoStream : public VirtualObject <NAMESPACE_OSGEO_COMMON_IO::IoStream, FdoIoStream>
{
public:
	FdoSize Read(FdoByte* buffer, FdoSize count);

	FdoVoid Write(FdoByte* buffer, FdoSize count);

	FdoVoid Write(FdoIoStream* stream, FdoSize count);

	FdoVoid SetLength(FdoInt64 length);

	FdoInt64 GetLength();

	FdoVoid Skip(FdoInt64 offset);

	FdoInt64 GetIndex();

	FdoVoid Reset();

    FdoBoolean CanRead();

	FdoBoolean CanWrite();

	FdoBoolean HasContext();

private:
	enum WrapperCallBits
	{
		ReadBit              = 0x01,
		WriteBufferBit        = 0x01 << 1,
		WriteStreamBit        = 0x01 << 2,
		SetLengthBit          = 0x01 << 3,
		GetLengthBit          = 0x01 << 4,
		SkipBit               = 0x01 << 5,
		GetIndexBit           = 0x01 << 6,
		ResetBit              = 0x01 << 7,
		CanReadBit            = 0x01 << 8,
		CanWriteBit           = 0x01 << 9,
		HasContextBit         = 0x01 << 10
	};

	mutable FdoInt32 wrapperCallBits;

};


