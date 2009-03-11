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

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilities^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::ConnectionCapabilities::get()
{
	FdoIConnectionCapabilities * result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetConnectionCapabilities())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIConnectionCapabilities(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilities^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::SchemaCapabilities::get()
{
	FdoISchemaCapabilities * result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetSchemaCapabilities())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateISchemaCapabilities(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilities^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::CommandCapabilities::get()
{
	FdoICommandCapabilities * result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetCommandCapabilities())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateICommandCapabilities(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IFilterCapabilities^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::FilterCapabilities::get()
{
	FdoIFilterCapabilities * result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetFilterCapabilities())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIFilterCapabilities(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IExpressionCapabilities^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::ExpressionCapabilities::get()
{
	FdoIExpressionCapabilities * result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetExpressionCapabilities())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIExpressionCapabilities(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IRasterCapabilities^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::RasterCapabilities::get()
{
	FdoIRasterCapabilities * result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetRasterCapabilities())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIRasterCapabilities(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ITopologyCapabilities^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::TopologyCapabilities::get()
{
	FdoITopologyCapabilities * result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetTopologyCapabilities())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateITopologyCapabilities(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IGeometryCapabilities^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::GeometryCapabilities::get()
{
	FdoIGeometryCapabilities * result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetGeometryCapabilities())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIGeometryCapabilities(IntPtr(result), true);
}

System::String^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::ConnectionString::get()
{
	FdoString* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetConnectionString())
	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::ConnectionString::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetConnectionString(StringToUni(value)))
}

NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfo^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::ConnectionInfo::get()
{
	FdoIConnectionInfo * result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetConnectionInfo())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIConnectionInfo(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS::ConnectionState NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::ConnectionState::get()
{
	FdoConnectionState result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetConnectionState())
	return static_cast<NAMESPACE_OSGEO_FDO_CONNECTIONS::ConnectionState>(result);
}

System::Int32 NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::ConnectionTimeout::get()
{
	System::Int32 obj;
	EXCEPTION_HANDLER(obj = GetImpObj()->GetConnectionTimeout())
	return obj;
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::ConnectionTimeout::set(System::Int32 value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetConnectionTimeout(value))
}

NAMESPACE_OSGEO_FDO_CONNECTIONS::ConnectionState NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::Open()
{
	FdoConnectionState result;
	EXCEPTION_HANDLER(result = GetImpObj()->Open())
	return static_cast<NAMESPACE_OSGEO_FDO_CONNECTIONS::ConnectionState>(result);
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::Close()
{
	EXCEPTION_HANDLER(GetImpObj()->Close())
}

NAMESPACE_OSGEO_FDO_CONNECTIONS::ITransaction^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::BeginTransaction()
{
	FdoITransaction* result;
	EXCEPTION_HANDLER(result = GetImpObj()->BeginTransaction())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateITransaction(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_COMMANDS::ICommand^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::CreateCommand(NAMESPACE_OSGEO_FDO_COMMANDS::CommandType commandType)
{
	FdoICommand* result;
	EXCEPTION_HANDLER(result = GetImpObj()->CreateCommand(static_cast<FdoInt32>(commandType)))
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateICommand(IntPtr(result), true, commandType);
}

NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::CreateSchemaMapping()
{
	FdoPhysicalSchemaMapping* result;
	EXCEPTION_HANDLER(result = GetImpObj()->CreateSchemaMapping())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePhysicalSchemaMapping(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::Configuration::set( NAMESPACE_OSGEO_COMMON_IO::IoStream^ configStream)
{
	EXCEPTION_HANDLER(GetImpObj()->SetConfiguration(static_cast<FdoIoStream*>(configStream->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp::Flush()
{
	EXCEPTION_HANDLER(GetImpObj()->Flush())
}
