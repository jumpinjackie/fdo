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
#include "FDO\Connections\Capabilities\mgITopologyCapabilitiesImp.h"

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ITopologyCapabilitiesImp::ITopologyCapabilitiesImp(IntPtr unmanaged, Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoITopologyCapabilities* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ITopologyCapabilitiesImp::GetImpObj()
{
    return static_cast<FdoITopologyCapabilities*>(__super::UnmanagedObject.ToPointer());
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ITopologyCapabilitiesImp::SupportsTopology()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsTopology())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ITopologyCapabilitiesImp::SupportsTopologicalHierarchy()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->SupportsTopologicalHierarchy())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ITopologyCapabilitiesImp::BreaksCurveCrossingsAutomatically()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->BreaksCurveCrossingsAutomatically())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ITopologyCapabilitiesImp::ActivatesTopologyByArea()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->ActivatesTopologyByArea())
	return rv;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ITopologyCapabilitiesImp::ConstrainsFeatureMovements()
{
	System::Boolean rv;
	EXCEPTION_HANDLER(rv = !!GetImpObj()->ConstrainsFeatureMovements())
	return rv;
}
