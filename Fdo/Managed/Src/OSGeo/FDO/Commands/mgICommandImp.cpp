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
#include <Fdo\Commands\ICommand.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\mgICommandImp.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Connections\mgIConnectionImp.h"
#include "FDO\Connections\mgITransactionImp.h"
#include "FDO\Commands\mgParameterValueCollection.h"

NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp::ICommandImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp::ICommandImp(NAMESPACE_OSGEO_FDO_COMMANDS::ICommand^ command, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
    if (nullptr == command)
		return;

    ICommandImp^ impObj = dynamic_cast<ICommandImp^>(command);

	EXCEPTION_HANDLER(Attach(IntPtr(impObj->GetImpObj()), autoDelete))
}

FdoICommand* NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp::GetImpObj()
{
    return static_cast<FdoICommand*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnection^ NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp::Connection::get()
{
	FdoIConnection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetConnection())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIConnection(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS::ITransaction^ NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp::Transaction::get()
{
	FdoITransaction* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetTransaction())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateITransaction(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp::Transaction::set(NAMESPACE_OSGEO_FDO_CONNECTIONS::ITransaction^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetTransaction((static_cast<NAMESPACE_OSGEO_FDO_CONNECTIONS::ITransactionImp^>(value))->GetImpObj()))
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp::CommandTimeOut::get()
{
	FdoInt32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCommandTimeout())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp::CommandTimeOut::set(System::Int32 value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetCommandTimeout(value))
}

NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection^ NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp::ParameterValues::get()
{
	FdoParameterValueCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetParameterValues())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateParameterValueCollection(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp::Prepare()
{
	EXCEPTION_HANDLER(GetImpObj()->Prepare())
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp::Cancel()
{
	EXCEPTION_HANDLER(GetImpObj()->Cancel())
}
