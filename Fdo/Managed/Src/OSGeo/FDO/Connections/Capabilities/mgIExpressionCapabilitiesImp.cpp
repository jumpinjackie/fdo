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
    return static_cast<FdoIExpressionCapabilities*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IExpressionCapabilitiesImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

array<NAMESPACE_OSGEO_FDO_EXPRESSION::ExpressionType>^ NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IExpressionCapabilitiesImp::ExpressionTypes::get()
{
	FdoExpressionType* unobj;
	FdoInt32 unlength;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetExpressionTypes(unlength))
	array<NAMESPACE_OSGEO_FDO_EXPRESSION::ExpressionType>^ result = gcnew array<NAMESPACE_OSGEO_FDO_EXPRESSION::ExpressionType>(unlength);
	for(FdoInt32 i = 0; i < unlength; i++)
		result[i] = static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::ExpressionType>(*(unobj + i));

    return result;
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection^ NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IExpressionCapabilitiesImp::Functions::get()
{
	FdoFunctionDefinitionCollection* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetFunctions())
	return ObjectFactory::CreateFunctionDefinitionCollection(IntPtr(result), true);
}
