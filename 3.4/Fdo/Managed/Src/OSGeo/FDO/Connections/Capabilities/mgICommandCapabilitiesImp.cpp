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
#include "FDO\Connections\Capabilities\mgICommandCapabilitiesImp.h"

#include <Fdo.h>

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilitiesImp::ICommandCapabilitiesImp(IntPtr unmanaged, Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoICommandCapabilities* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilitiesImp::GetImpObj()
{
    return static_cast<FdoICommandCapabilities*>(__super::UnmanagedObject.ToPointer());
}

Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilitiesImp::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilitiesImp::get_Commands() []
{
	FdoInt32 unsize;
	FdoInt32* unbuffer;
	EXCEPTION_HANDLER(unbuffer = GetImpObj()->GetCommands(unsize))
	System::Int32 buffer [] = new System::Int32[unsize];
	for(FdoInt32 i = 0; i < unsize; i ++)
	{
		buffer[i] = *(unbuffer + i);
	}
	return buffer;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilitiesImp::SupportsParameters()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsParameters())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilitiesImp::SupportsTimeout()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsTimeout())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilitiesImp::SupportsSelectExpressions()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsSelectExpressions())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilitiesImp::SupportsSelectFunctions()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsSelectFunctions())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilitiesImp::SupportsSelectDistinct()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsSelectDistinct())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilitiesImp::SupportsSelectOrdering()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsSelectOrdering())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilitiesImp::SupportsSelectGrouping()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsSelectGrouping())
	return rv;
}
