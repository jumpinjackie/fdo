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

#include "FDO\Connections\mgITransactionImp.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Connections\mgIConnectionImp.h"

NAMESPACE_OSGEO_FDO_CONNECTIONS::ITransactionImp::ITransactionImp(IntPtr unmanaged, Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoITransaction* NAMESPACE_OSGEO_FDO_CONNECTIONS::ITransactionImp::GetImpObj()
{
    return static_cast<FdoITransaction*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_CONNECTIONS::ITransactionImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnection^ NAMESPACE_OSGEO_FDO_CONNECTIONS::ITransactionImp::Connection::get()
{
	FdoIConnection* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetConnection())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIConnection(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS::ITransactionImp::Commit()
{
	EXCEPTION_HANDLER(GetImpObj()->Commit())
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS::ITransactionImp::Rollback()
{
	EXCEPTION_HANDLER(GetImpObj()->Rollback())
}
