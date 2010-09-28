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
#include <Fdo\Commands\CommandType.h>

#include "FDO\Expression\mgExpression.h"
#include "FDO\Expression\mgIExpressionProcessorImp.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_EXPRESSION::Expression::Expression(IntPtr unmanaged, Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoExpression* NAMESPACE_OSGEO_FDO_EXPRESSION::Expression::GetImpObj()
{
	return static_cast<FdoExpression*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_EXPRESSION::Expression::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ NAMESPACE_OSGEO_FDO_EXPRESSION::Expression::Parse(System::String^ expressionText)
{
	FdoExpression* result;

	EXCEPTION_HANDLER(result = FdoExpression::Parse(StringToUni(expressionText)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateExpression(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::Expression::Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor^ processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessorImp^>(processor))->GetImpObj()))
}

System::String^ NAMESPACE_OSGEO_FDO_EXPRESSION::Expression::ToString()
{
	FdoString* unstr;

	EXCEPTION_HANDLER(unstr = GetImpObj()->ToString())

	return CHECK_STRING(unstr);
}
