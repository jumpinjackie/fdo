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
#include <Fdo.h>
#include <Fdo\Connections\Capabilities\ThreadCapability.h>

#include "FDO\Connections\Capabilities\mgIConnectionCapabilitiesImp.h"

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::IConnectionCapabilitiesImp(IntPtr unmanaged, Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoIConnectionCapabilities* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::GetImpObj()
{
    return static_cast<FdoIConnectionCapabilities*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ThreadCapability NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::ThreadCapability::get()
{
	FdoThreadCapability unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetThreadCapability())
	return static_cast<NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ThreadCapability>(unobj);
}

array<NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType>^ NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::SpatialContextTypes::get()
{
	FdoSpatialContextExtentType* unobj;
	FdoInt32 unlength;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetSpatialContextTypes(unlength))

    array<NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType>^ result = gcnew array<NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType>(unlength);
	for(FdoInt32 i = 0; i < unlength; i++)
		result[i] = static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType>(*(unobj + i));

    return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::SupportsLocking()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsLocking())
	return rv;
}

array<NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType>^ NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::LockTypes::get()
{
	FdoLockType* unobj;
	FdoInt32 unlength;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetLockTypes(unlength))
    array<NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType>^ result = gcnew array<NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType>(unlength);
	for(FdoInt32 i = 0; i < unlength; i++)
		result[i] = static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType>(*(unobj + i));

    return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::SupportsTimeout()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsTimeout())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::SupportsTransactions()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsTransactions())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::SupportsLongTransactions()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsLongTransactions())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::SupportsSQL()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsSQL())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::SupportsConfiguration()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsConfiguration())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::SupportsMultipleSpatialContexts()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsMultipleSpatialContexts())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::SupportsCSysWKTFromCSysName()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsCSysWKTFromCSysName())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::SupportsFlush()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsFlush())
	return rv;
}
