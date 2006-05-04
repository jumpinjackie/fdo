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

#include "FDO\Connections\Capabilities\mgIExpressionCapabilitiesImp.h"
#include "FDO\Connections\Capabilities\mgFunctionDefinitionCollection.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IExpressionCapabilitiesImp::IExpressionCapabilitiesImp(IntPtr unmanaged, Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoIExpressionCapabilities* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IExpressionCapabilitiesImp::GetImpObj()
{
    return static_cast<FdoIExpressionCapabilities*>(__super::UnmanagedObject.ToPointer());
}

Void NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IExpressionCapabilitiesImp::Dispose(Boolean disposing)
{
	if (disposing)
	{
		// Add your own code here
	}

	if (!Disposed)
	{
		EXCEPTION_HANDLER(GetImpObj()->Release())
		Detach();
	}
}

NAMESPACE_OSGEO_FDO_EXPRESSION::ExpressionType NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IExpressionCapabilitiesImp::get_ExpressionTypes() []
{
	FdoExpressionType* unobj;
	FdoInt32 unlength;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetExpressionTypes(unlength))
	NAMESPACE_OSGEO_FDO_EXPRESSION::ExpressionType rv [] = new NAMESPACE_OSGEO_FDO_EXPRESSION::ExpressionType[unlength];
	for(FdoInt32 i = 0; i < unlength; i++)
	{
		rv[i] = static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::ExpressionType>(*(unobj + i));
	}
	return rv;
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection* NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IExpressionCapabilitiesImp::get_Functions()
{
	FdoFunctionDefinitionCollection * unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetFunctions())
	return ObjectFactory::CreateFunctionDefinitionCollection(unobj, true);
}
