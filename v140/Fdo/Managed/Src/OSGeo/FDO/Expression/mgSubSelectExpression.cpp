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
#include <Fdo\Expression\JoinType.h>
#include <Fdo\Expression\JoinCriteria.h>
#include <Fdo\Filter\Filter.h>
#include <Fdo\Filter\BinaryLogicalOperations.h>
#include <Fdo\Commands\CommandType.h>
#include <Fdo\Expression\SubSelectExpression.h>

#include "FDO\Expression\mgIdentifier.h"
#include "FDO\Filter\mgFilter.h"
#include "FDO\Expression\mgJoinCriteriaCollection.h"
#include "FDO\Expression\mgSubSelectExpression.h"
#include "FDO\Expression\mgIExpressionProcessorImp.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::SubSelectExpression(IntPtr unmanaged, Boolean autoDelete) : Expression(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::SubSelectExpression() : Expression(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoSubSelectExpression::Create()), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::SubSelectExpression(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ className, NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ propertyName) : Expression(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoSubSelectExpression::Create(className->GetImpObj(), propertyName->GetImpObj())), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::SubSelectExpression(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ className, NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ propertyName, NAMESPACE_OSGEO_FDO_FILTER::Filter^ filter) : Expression(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoSubSelectExpression::Create(className->GetImpObj(), propertyName->GetImpObj(), filter->GetImpObj())), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::SubSelectExpression(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ className, NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ propertyName, NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteriaCollection^ joinCritColl) : Expression(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoSubSelectExpression::Create(className->GetImpObj(), propertyName->GetImpObj(), joinCritColl->GetImpObj())), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::SubSelectExpression(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ className, NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ propertyName, NAMESPACE_OSGEO_FDO_FILTER::Filter^ filter, NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteriaCollection^ joinCritColl) : Expression(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoSubSelectExpression::Create(className->GetImpObj(), propertyName->GetImpObj(), filter->GetImpObj(), joinCritColl->GetImpObj())), true))
}

FdoSubSelectExpression* NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::GetImpObj()
{
	return static_cast<FdoSubSelectExpression*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::FeatureClassName::get()
{
	FdoIdentifier* result;
    EXCEPTION_HANDLER(result = NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::GetImpObj()->GetFeatureClassName())
	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIdentifier(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::FeatureClassName::set(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ value)
{
    EXCEPTION_HANDLER(NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::GetImpObj()->SetFeatureClassName((value == nullptr ? nullptr : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::PropertyName::get()
{
	FdoIdentifier* result;
    EXCEPTION_HANDLER(result = NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::GetImpObj()->GetPropertyName())
	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIdentifier(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::PropertyName::set(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ value)
{
    EXCEPTION_HANDLER(NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::GetImpObj()->SetPropertyName((value == nullptr ? nullptr : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteriaCollection^ NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::JoinCriteria::get()
{
	FdoJoinCriteriaCollection* result;
    EXCEPTION_HANDLER(result = NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::GetImpObj()->GetJoinCriteria())
	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateJoinCriteriaCollection(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::JoinCriteria::set(NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteriaCollection^ value)
{
    EXCEPTION_HANDLER(NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::GetImpObj()->SetJoinCriteria((value == nullptr ? nullptr : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_FILTER::Filter^ NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::Filter::get()
{
	FdoFilter* result;
    EXCEPTION_HANDLER(result = NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::GetImpObj()->GetFilter())
	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateFilter(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::Filter::set(NAMESPACE_OSGEO_FDO_FILTER::Filter^ value)
{
    EXCEPTION_HANDLER(NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::GetImpObj()->SetFilter((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor^ processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessorImp^>(processor))->GetImpObj()))
}

System::String^ NAMESPACE_OSGEO_FDO_EXPRESSION::SubSelectExpression::ToString()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->ToString())
	return CHECK_STRING(unstr);
}
