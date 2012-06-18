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

#include "FDO\Expression\mgIdentifier.h"
#include "FDO\Filter\mgFilter.h"
#include "FDO\Expression\mgJoinCriteria.h"
#include "FDO\Expression\mgIExpressionProcessorImp.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::JoinCriteria(IntPtr unmanaged, Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::JoinCriteria() : Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoJoinCriteria::Create()), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::JoinCriteria(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ joinClass, NAMESPACE_OSGEO_FDO_EXPRESSION::JoinType joinType) : Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoJoinCriteria::Create(joinClass->GetImpObj(), (FdoJoinType)joinType)), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::JoinCriteria(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ joinClass, NAMESPACE_OSGEO_FDO_EXPRESSION::JoinType joinType, NAMESPACE_OSGEO_FDO_FILTER::Filter^ filter) : Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoJoinCriteria::Create(joinClass->GetImpObj(), (FdoJoinType)joinType, filter->GetImpObj())), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::JoinCriteria(System::String^ alias, NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ joinClass, NAMESPACE_OSGEO_FDO_EXPRESSION::JoinType joinType) : Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoJoinCriteria::Create(StringToUni(alias), joinClass->GetImpObj(), (FdoJoinType)joinType)), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::JoinCriteria(System::String^ alias, NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ joinClass, NAMESPACE_OSGEO_FDO_EXPRESSION::JoinType joinType, NAMESPACE_OSGEO_FDO_FILTER::Filter^ filter) : Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoJoinCriteria::Create(StringToUni(alias), joinClass->GetImpObj(), (FdoJoinType)joinType, filter->GetImpObj())), true))
}

FdoJoinCriteria* NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::GetImpObj()
{
	return static_cast<FdoJoinCriteria*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::String^ NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::Alias::get()
{
	FdoString* unstr;
    EXCEPTION_HANDLER(unstr = NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::GetImpObj()->GetAlias())
	return CHECK_STRING(unstr);
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::Alias::set(System::String^ value)
{
	EXCEPTION_HANDLER(NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::GetImpObj()->SetAlias(StringToUni(value)))
}

System::Boolean NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::HasAlias::get()
{
	System::Boolean ret;
    EXCEPTION_HANDLER(ret = NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::GetImpObj()->HasAlias())
	return ret;
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::JoinClass::get()
{
	FdoIdentifier* result;
    EXCEPTION_HANDLER(result = NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::GetImpObj()->GetJoinClass())
	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIdentifier(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::JoinClass::set(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ value)
{
    EXCEPTION_HANDLER(NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::GetImpObj()->SetJoinClass((value == nullptr ? nullptr : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::JoinType NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::JoinType::get()
{
	FdoJoinType ret;
    EXCEPTION_HANDLER(ret = NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::GetImpObj()->GetJoinType())
	return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::JoinType>(ret);
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::JoinType::set(NAMESPACE_OSGEO_FDO_EXPRESSION::JoinType value)
{
    EXCEPTION_HANDLER(NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::GetImpObj()->SetJoinType((FdoJoinType)value))
}

NAMESPACE_OSGEO_FDO_FILTER::Filter^ NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::Filter::get()
{
	FdoFilter* result;
    EXCEPTION_HANDLER(result = NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::GetImpObj()->GetFilter())
	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateFilter(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::Filter::set(NAMESPACE_OSGEO_FDO_FILTER::Filter^ value)
{
    EXCEPTION_HANDLER(NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteria::GetImpObj()->SetFilter((value == nullptr ? nullptr : value->GetImpObj())))
}

