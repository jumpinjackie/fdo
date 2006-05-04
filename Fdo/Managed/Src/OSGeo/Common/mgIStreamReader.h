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

BEGIN_NAMESPACE_OSGEO_COMMON

public __value enum StreamReaderType
{
	StreamReaderType_Byte = FdoStreamReaderType_Byte,
	StreamReaderType_Char = FdoStreamReaderType_Char,
};

/// \brief
///     The IStreamReader class is a top level interface for stream reader classes. 
///     The API it provides allows the user to read large streams of data in blocks of items. 
public __gc __interface IStreamReader : public System::IDisposable
{
public:
	__property StreamReaderType get_Type();
};

END_NAMESPACE_OSGEO_COMMON


