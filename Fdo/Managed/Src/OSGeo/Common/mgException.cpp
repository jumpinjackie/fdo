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

NAMESPACE_OSGEO_COMMON::Exception^ NAMESPACE_OSGEO_COMMON::Exception::Create(System::IntPtr ex)
{
	//TODO: maybe some type-check code need adding here.

    if (IntPtr::Zero == ex)
	{
		FdoException* e = (FdoException*) ex.ToPointer();
		NAMESPACE_OSGEO_COMMON::Exception^ mg = Create(IntPtr(e->GetCause()));
		if (nullptr != mg)
            return gcnew NAMESPACE_OSGEO_COMMON::Exception(gcnew String(e->GetExceptionMessage()), mg);
		else
			return gcnew NAMESPACE_OSGEO_COMMON::Exception(gcnew String(e->GetExceptionMessage()));
	}

	return nullptr;
}
