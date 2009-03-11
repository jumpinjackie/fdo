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
#include "Common\mgObjectFactory.h"
#include "Common\Io\mgIoStream.h"
#include "Common\Io\mgIoTextReader.h"

NAMESPACE_OSGEO_COMMON_IO::IoTextReader::IoTextReader(System::String^ fileName) : Disposable(System::IntPtr::Zero, false)
{
    EXCEPTION_HANDLER(Attach(IntPtr(FdoIoTextReader::Create(StringToUni(fileName))), true))
}

NAMESPACE_OSGEO_COMMON_IO::IoTextReader::IoTextReader(NAMESPACE_OSGEO_COMMON_IO::IoStream^ stream) : Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoIoTextReader::Create((FdoIoStream*)stream->GetImpObj())), true))
}

NAMESPACE_OSGEO_COMMON_IO::IoTextReader::IoTextReader(System::IntPtr unmanaged, System::Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoIoTextReader* NAMESPACE_OSGEO_COMMON_IO::IoTextReader::GetImpObj()
{
	return static_cast<FdoIoTextReader*>(UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_COMMON_IO::IoStream^ NAMESPACE_OSGEO_COMMON_IO::IoTextReader::Stream::get()
{
	FdoIoStream* stream;

	EXCEPTION_HANDLER(stream = GetImpObj()->GetStream())
	
	return ObjectFactory::CreateIoStream(IntPtr(stream), true);
}
