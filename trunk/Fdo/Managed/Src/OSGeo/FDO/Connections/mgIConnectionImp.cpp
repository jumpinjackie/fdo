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
#include <Fdo\Connections\IConnection.h>
#include <Fdo\Connections\ConnectionState.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Connections\mgIConnectionImp.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Connections\Capabilities\mgIConnectionCapabilitiesImp.h"
#include "FDO\Connections\Capabilities\mgISchemaCapabilitiesImp.h"
#include "FDO\Connections\Capabilities\mgICommandCapabilitiesImp.h"
#include "FDO\Connections\Capabilities\mgIFilterCapabilitiesImp.h"
#include "FDO\Connections\Capabilities\mgIExpressionCapabilitiesImp.h"
#include "FDO\Connections\Capabilities\mgIRasterCapabilitiesImp.h"
#include "FDO\Connections\Capabilities\mgITopologyCapabilitiesImp.h"
#include "FDO\Connections\Capabilities\mgIGeometryCapabilitiesImp.h"
#include "FDO\Connections\mgIConnectionInfoImp.h"
#include "FDO\Connections\mgConnectionState.h"
#include "FDO\Connections\mgITransactionImp.h"
#include "FDO\Commands\mgICommandImp.h"
#include "FDO\Commands\Schema\mgPhysicalSchemaMapping.h"


NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::IConnectionImp(IntPtr unmanaged, Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{

}

FdoIConnection* NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::GetImpObj()
{
    return static_cast<FdoIConnection*>(__super::UnmanagedObject.ToPointer());
}

Void NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilities* NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::get_ConnectionCapabilities()
{
	FdoIConnectionCapabilities * unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetConnectionCapabilities())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIConnectionCapabilities(unobj, true);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilities* NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::get_SchemaCapabilities()
{
	FdoISchemaCapabilities * unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetSchemaCapabilities())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateISchemaCapabilities(unobj, true);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilities* NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::get_CommandCapabilities()
{
	FdoICommandCapabilities * unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetCommandCapabilities())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateICommandCapabilities(unobj, true);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IFilterCapabilities* NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::get_FilterCapabilities()
{
	FdoIFilterCapabilities * unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetFilterCapabilities())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIFilterCapabilities(unobj, true);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IExpressionCapabilities* NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::get_ExpressionCapabilities()
{
	FdoIExpressionCapabilities * unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetExpressionCapabilities())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIExpressionCapabilities(unobj, true);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IRasterCapabilities* NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::get_RasterCapabilities()
{
	FdoIRasterCapabilities * unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetRasterCapabilities())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIRasterCapabilities(unobj, true);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ITopologyCapabilities* NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::get_TopologyCapabilities()
{
	FdoITopologyCapabilities * unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetTopologyCapabilities())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateITopologyCapabilities(unobj, true);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IGeometryCapabilities* NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::get_GeometryCapabilities()
{
	FdoIGeometryCapabilities * unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetGeometryCapabilities())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIGeometryCapabilities(unobj, true);
}

System::String* NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::get_ConnectionString()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->GetConnectionString())
	return unstr;
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::set_ConnectionString( System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetConnectionString(StringToUni(value)))
}

NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfo* NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::get_ConnectionInfo()
{
	FdoIConnectionInfo * unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetConnectionInfo())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIConnectionInfo(unobj, true);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS::ConnectionState NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::get_ConnectionState()
{
	FdoConnectionState unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetConnectionState())
	return static_cast<NAMESPACE_OSGEO_FDO_CONNECTIONS::ConnectionState>(unobj);
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::get_ConnectionTimeout()
{
	System::Int32 obj;
	EXCEPTION_HANDLER(obj = GetImpObj()->GetConnectionTimeout())
	return obj;
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::set_ConnectionTimeout(System::Int32 value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetConnectionTimeout(value))
}

NAMESPACE_OSGEO_FDO_CONNECTIONS::ConnectionState NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::Open()
{
	FdoConnectionState unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->Open())
	return static_cast<NAMESPACE_OSGEO_FDO_CONNECTIONS::ConnectionState>(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::Close()
{
	EXCEPTION_HANDLER(GetImpObj()->Close())
}

NAMESPACE_OSGEO_FDO_CONNECTIONS::ITransaction* NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::BeginTransaction()
{
	FdoITransaction* unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->BeginTransaction())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateITransaction(unobj, true);
}

NAMESPACE_OSGEO_FDO_COMMANDS::ICommand* NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::CreateCommand(NAMESPACE_OSGEO_FDO_COMMANDS::CommandType commandType)
{
	FdoICommand* unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->CreateCommand(static_cast<FdoInt32>(commandType)))
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateICommand(unobj, true, commandType);
}

NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::CreateSchemaMapping()
{
	FdoPhysicalSchemaMapping* unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->CreateSchemaMapping())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePhysicalSchemaMapping(unobj, true);
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::set_Configuration( NAMESPACE_OSGEO_COMMON_IO::IoStream* configStream)
{
	EXCEPTION_HANDLER(GetImpObj()->SetConfiguration(static_cast<FdoIoStream*>(configStream->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::Flush()
{
	EXCEPTION_HANDLER(GetImpObj()->Flush())
}
