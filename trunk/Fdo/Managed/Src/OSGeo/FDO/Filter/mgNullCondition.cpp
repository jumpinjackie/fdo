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
#include <Fdo\Filter\NullCondition.h>
#include <Fdo\Filter\BinaryLogicalOperations.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Filter\mgNullCondition.h"
#include "FDO\Expression\mgIdentifier.h"
#include "FDO\Filter\mgIFilterProcessorImp.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_FILTER::NullCondition::NullCondition(System::IntPtr unmanaged, System::Boolean autoDelete) : SearchCondition(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_FILTER::NullCondition::NullCondition() : SearchCondition(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoNullCondition::Create(), true)) 
}

NAMESPACE_OSGEO_FDO_FILTER::NullCondition::NullCondition(System::String* propertyName) : SearchCondition(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoNullCondition::Create(StringToUni(propertyName)), true)) 
}

NAMESPACE_OSGEO_FDO_FILTER::NullCondition::NullCondition(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier* propertyName) : SearchCondition(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoNullCondition::Create(propertyName->GetImpObj()), true))
}

FdoNullCondition* NAMESPACE_OSGEO_FDO_FILTER::NullCondition::GetImpObj()
{
	return static_cast<FdoNullCondition*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier* NAMESPACE_OSGEO_FDO_FILTER::NullCondition::get_PropertyName()
{
	FdoIdentifier* unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetPropertyName())
	return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier*>(ObjectFactory::CreateExpression(unobj, true));
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::NullCondition::set_PropertyName(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetPropertyName((value == NULL ? NULL : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::NullCondition::Process(NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessor* processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessorImp*>(processor))->GetImpObj()))
}
