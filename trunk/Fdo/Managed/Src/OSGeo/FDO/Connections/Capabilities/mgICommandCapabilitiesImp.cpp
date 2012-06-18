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
    return static_cast<FdoICommandCapabilities*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilitiesImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

array<System::Int32>^ NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilitiesImp::Commands::get()
{
	FdoInt32 unsize;
	FdoInt32* unbuffer;
	EXCEPTION_HANDLER(unbuffer = GetImpObj()->GetCommands(unsize))
	array<System::Int32>^ mgBuffer = gcnew array<System::Int32>(unsize);
    if (unsize != 0)
    {
        pin_ptr<FdoInt32> umgBuffer = &mgBuffer[0];
        memcpy(umgBuffer, unbuffer, unsize * sizeof(FdoInt32));
    }
	return mgBuffer;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilitiesImp::SupportsParameters()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsParameters())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilitiesImp::SupportsTimeout()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsTimeout())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilitiesImp::SupportsSelectExpressions()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsSelectExpressions())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilitiesImp::SupportsSelectFunctions()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsSelectFunctions())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilitiesImp::SupportsSelectDistinct()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsSelectDistinct())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilitiesImp::SupportsSelectOrdering()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsSelectOrdering())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilitiesImp::SupportsSelectGrouping()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsSelectGrouping())
	return rv;
}
