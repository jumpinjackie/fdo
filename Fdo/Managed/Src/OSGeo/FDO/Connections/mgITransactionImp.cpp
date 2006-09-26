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
    return static_cast<FdoITransaction*>(__super::UnmanagedObject.ToPointer());
}

Void NAMESPACE_OSGEO_FDO_CONNECTIONS::ITransactionImp::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnection* NAMESPACE_OSGEO_FDO_CONNECTIONS::ITransactionImp::get_Connection()
{
	FdoIConnection* unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetConnection())
    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIConnection(unobj, true);
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS::ITransactionImp::Commit()
{
	EXCEPTION_HANDLER(GetImpObj()->Commit())
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS::ITransactionImp::Rollback()
{
	EXCEPTION_HANDLER(GetImpObj()->Rollback())
}
