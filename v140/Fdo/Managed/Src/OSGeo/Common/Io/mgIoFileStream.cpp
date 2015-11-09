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
#include "Common\Io\mgIoFileStream.h"

NAMESPACE_OSGEO_COMMON_IO::IoFileStream::IoFileStream(System::String^ fileName, System::String^ accessModes) : NAMESPACE_OSGEO_COMMON_IO::IoStream(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoIoFileStream::Create(StringToUni(fileName), StringToUni(accessModes))), true))
}

NAMESPACE_OSGEO_COMMON_IO::IoFileStream::IoFileStream(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON_IO::IoStream(unmanaged, autoDelete)
{

}

FdoIoFileStream* NAMESPACE_OSGEO_COMMON_IO::IoFileStream::GetImpObj()
{
	return static_cast<FdoIoFileStream*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_COMMON_IO::IoFileStream::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}