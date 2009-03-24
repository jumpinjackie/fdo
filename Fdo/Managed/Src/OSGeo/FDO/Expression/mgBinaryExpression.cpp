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
#include <Fdo\Expression\BinaryExpression.h>
#include <Fdo\Expression\BinaryOperations.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Expression\mgBinaryExpression.h"
#include "FDO\Expression\mgBinaryOperations.h"
#include "FDO\Expression\mgIExpressionProcessorImp.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_EXPRESSION::BinaryExpression::BinaryExpression(IntPtr unmanaged, Boolean autoDelete) : Expression(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_EXPRESSION::BinaryExpression::BinaryExpression() : Expression(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoBinaryExpression::Create()), true)) 
}

NAMESPACE_OSGEO_FDO_EXPRESSION::BinaryExpression::BinaryExpression(NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ leftExpression, NAMESPACE_OSGEO_FDO_EXPRESSION::BinaryOperations operation, NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ rightExpression) : Expression(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoBinaryExpression::Create(leftExpression->GetImpObj(), static_cast<FdoBinaryOperations>(operation), rightExpression->GetImpObj())), true)) 
}

FdoBinaryExpression* NAMESPACE_OSGEO_FDO_EXPRESSION::BinaryExpression::GetImpObj()
{
	return static_cast<FdoBinaryExpression*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_EXPRESSION::BinaryExpression::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_EXPRESSION::BinaryOperations NAMESPACE_OSGEO_FDO_EXPRESSION::BinaryExpression::Operation::get()
{
	FdoBinaryOperations unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetOperation())
	return static_cast<BinaryOperations>(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::BinaryExpression::Operation::set(NAMESPACE_OSGEO_FDO_EXPRESSION::BinaryOperations value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetOperation(static_cast<FdoBinaryOperations>(value)))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ NAMESPACE_OSGEO_FDO_EXPRESSION::BinaryExpression::LeftExpression::get()
{
	FdoExpression* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetLeftExpression())
	return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^>(ObjectFactory::CreateExpression(IntPtr(result), true));
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::BinaryExpression::LeftExpression::set(NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetLeftExpression((value == nullptr ? nullptr : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ NAMESPACE_OSGEO_FDO_EXPRESSION::BinaryExpression::RightExpression::get()
{
	FdoExpression* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetRightExpression())
	return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^>(ObjectFactory::CreateExpression(IntPtr(result), true));
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::BinaryExpression::RightExpression::set(NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetRightExpression((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::BinaryExpression::Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor^ processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessorImp^>(processor))->GetImpObj()))
}

System::String^ NAMESPACE_OSGEO_FDO_EXPRESSION::BinaryExpression::ToString()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->ToString())
	return CHECK_STRING(unstr);
}
