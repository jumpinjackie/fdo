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
#include <Fdo\Filter\ComparisonCondition.h>
#include <Fdo\Filter\ComparisonOperations.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Filter\mgComparisonCondition.h"
#include "FDO\Expression\mgExpression.h"
#include "FDO\Filter\mgComparisonOperations.h"
#include "FDO\Filter\mgIFilterProcessorImp.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_FILTER::ComparisonCondition::ComparisonCondition(IntPtr unmanaged, Boolean autoDelete) : SearchCondition(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_FILTER::ComparisonCondition::ComparisonCondition() : SearchCondition(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoComparisonCondition::Create()), true)) 
}

NAMESPACE_OSGEO_FDO_FILTER::ComparisonCondition::ComparisonCondition( NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ leftExpression, NAMESPACE_OSGEO_FDO_FILTER::ComparisonOperations operation, NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ rightExpression) : SearchCondition(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoComparisonCondition::Create(leftExpression->GetImpObj(), static_cast<FdoComparisonOperations>(operation), rightExpression->GetImpObj())), true))
}

FdoComparisonCondition* NAMESPACE_OSGEO_FDO_FILTER::ComparisonCondition::GetImpObj()
{
	return static_cast<FdoComparisonCondition*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_FILTER::ComparisonCondition::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_FILTER::ComparisonOperations NAMESPACE_OSGEO_FDO_FILTER::ComparisonCondition::Operation::get()
{
	FdoComparisonOperations unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetOperation())
	return static_cast<ComparisonOperations>(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ComparisonCondition::Operation::set(NAMESPACE_OSGEO_FDO_FILTER::ComparisonOperations operation)
{
	EXCEPTION_HANDLER(GetImpObj()->SetOperation(static_cast<FdoComparisonOperations>(operation)))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ NAMESPACE_OSGEO_FDO_FILTER::ComparisonCondition::LeftExpression::get()
{
	FdoExpression* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetLeftExpression())
	return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^>(ObjectFactory::CreateExpression(IntPtr(result), true));
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ComparisonCondition::LeftExpression::set(NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetLeftExpression((value == nullptr ? nullptr : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ NAMESPACE_OSGEO_FDO_FILTER::ComparisonCondition::RightExpression::get()
{
	FdoExpression* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetRightExpression())
	return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^>(ObjectFactory::CreateExpression(IntPtr(result), true));
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ComparisonCondition::RightExpression::set( NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetRightExpression((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ComparisonCondition::Process(NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessor^ processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessorImp^>(processor))->GetImpObj()))
}
