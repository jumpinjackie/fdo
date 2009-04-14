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
	EXCEPTION_HANDLER(Attach(FdoComparisonCondition::Create(), true)) 
}

NAMESPACE_OSGEO_FDO_FILTER::ComparisonCondition::ComparisonCondition( NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* leftExpression, NAMESPACE_OSGEO_FDO_FILTER::ComparisonOperations operation, NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* rightExpression) : SearchCondition(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoComparisonCondition::Create(leftExpression->GetImpObj(), static_cast<FdoComparisonOperations>(operation), rightExpression->GetImpObj()), true))
}

FdoComparisonCondition* NAMESPACE_OSGEO_FDO_FILTER::ComparisonCondition::GetImpObj()
{
	return static_cast<FdoComparisonCondition*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_FILTER::ComparisonOperations NAMESPACE_OSGEO_FDO_FILTER::ComparisonCondition::get_Operation()
{
	FdoComparisonOperations unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetOperation())
	return static_cast<ComparisonOperations>(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ComparisonCondition::set_Operation(NAMESPACE_OSGEO_FDO_FILTER::ComparisonOperations operation)
{
	EXCEPTION_HANDLER(GetImpObj()->SetOperation(static_cast<FdoComparisonOperations>(operation)))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* NAMESPACE_OSGEO_FDO_FILTER::ComparisonCondition::get_LeftExpression()
{
	FdoExpression* unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetLeftExpression())
	return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::Expression*>(ObjectFactory::CreateExpression(unobj, true));
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ComparisonCondition::set_LeftExpression(NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetLeftExpression((value == NULL ? NULL : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* NAMESPACE_OSGEO_FDO_FILTER::ComparisonCondition::get_RightExpression()
{
	FdoExpression* unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetRightExpression())
	return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::Expression*>(ObjectFactory::CreateExpression(unobj, true));
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ComparisonCondition::set_RightExpression( NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetRightExpression((value == NULL ? NULL : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::ComparisonCondition::Process(NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessor* processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessorImp*>(processor))->GetImpObj()))
}
