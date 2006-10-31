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
    return static_cast<FdoIConnectionCapabilities*>(__super::UnmanagedObject.ToPointer());
}

Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ThreadCapability NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::get_ThreadCapability()
{
	FdoThreadCapability unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetThreadCapability())
	return static_cast<NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ThreadCapability>(unobj);
}

NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::get_SpatialContextTypes() []
{
	FdoSpatialContextExtentType* unobj;
	FdoInt32 unlength;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetSpatialContextTypes(unlength))
	NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType rv [] = new NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType[unlength];
	for(FdoInt32 i = 0; i < unlength; i++)
	{
		rv[i] = static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType>(*(unobj + i));
	}
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::SupportsLocking()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsLocking())
	return rv;
}

NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::get_LockTypes() []
{
	FdoLockType* unobj;
	FdoInt32 unlength;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetLockTypes(unlength))
	NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType rv [] = new NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType[unlength];
	for(FdoInt32 i = 0; i < unlength; i++)
	{
		rv[i] = static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType>(*(unobj + i));
	}
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::SupportsTimeout()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsTimeout())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::SupportsTransactions()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsTransactions())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::SupportsLongTransactions()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsLongTransactions())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::SupportsSQL()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsSQL())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::SupportsConfiguration()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsConfiguration())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::SupportsMultipleSpatialContexts()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsMultipleSpatialContexts())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::SupportsCSysWKTFromCSysName()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsCSysWKTFromCSysName())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp::SupportsFlush()
{
	FdoBoolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsFlush())
	return rv;
}
