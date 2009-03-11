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
#include <Fdo\Expression\UnaryOperations.h>

#include "FDO\Expression\mgUnaryExpression.h"
#include "FDO\Expression\mgUnaryOperations.h"
#include "FDO\Expression\mgExpression.h"
#include "FDO\Expression\mgIExpressionProcessorImp.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_EXPRESSION::UnaryExpression::UnaryExpression(IntPtr unmanaged, Boolean autoDelete) : Expression(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_EXPRESSION::UnaryExpression::UnaryExpression() : Expression(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoUnaryExpression::Create()), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::UnaryExpression::UnaryExpression(NAMESPACE_OSGEO_FDO_EXPRESSION::UnaryOperations operation, NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ expression) : Expression(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoUnaryExpression::Create(static_cast<FdoUnaryOperations>(operation), expression->GetImpObj())), true))
}

FdoUnaryExpression* NAMESPACE_OSGEO_FDO_EXPRESSION::UnaryExpression::GetImpObj()
{
	return static_cast<FdoUnaryExpression*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_EXPRESSION::UnaryOperations NAMESPACE_OSGEO_FDO_EXPRESSION::UnaryExpression::Operation::get()
{
	FdoUnaryOperations unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetOperation())
	return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::UnaryOperations>(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::UnaryExpression::Operation::set(NAMESPACE_OSGEO_FDO_EXPRESSION::UnaryOperations value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetOperation(static_cast<FdoUnaryOperations>(value)))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ NAMESPACE_OSGEO_FDO_EXPRESSION::UnaryExpression::Expressions::get()
{
	FdoExpression* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetExpression())
	return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^>(ObjectFactory::CreateExpression(IntPtr(result), true));
}

System::Void  NAMESPACE_OSGEO_FDO_EXPRESSION::UnaryExpression::Expressions::set(NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetExpression((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::UnaryExpression::Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor^ processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessorImp^>(processor))->GetImpObj()))
}

System::String^ NAMESPACE_OSGEO_FDO_EXPRESSION::UnaryExpression::ToString()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->ToString())
	return CHECK_STRING(unstr);
}
