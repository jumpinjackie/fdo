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
#include <Fdo\Filter\InCondition.h>
#include <Fdo\Filter\BinaryLogicalOperations.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Filter\mgInCondition.h"
#include "FDO\Expression\mgIdentifier.h"
#include "FDO\Filter\mgIFilterProcessorImp.h"
#include "FDO\Commands\mgIdentifierCollection.h"
#include "FDO\Filter\mgValueExpressionCollection.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_FILTER::InCondition::InCondition(System::IntPtr unmanaged, System::Boolean autoDelete) : SearchCondition(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_FILTER::InCondition::InCondition() : SearchCondition(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoInCondition::Create()), true))
}

FdoInCondition* NAMESPACE_OSGEO_FDO_FILTER::InCondition::GetImpObj()
{
	return static_cast<FdoInCondition*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_FILTER::InCondition::InCondition(System::String^ propertyName, array<System::String^>^ values) : SearchCondition(IntPtr::Zero, false)
{
	FdoString** unvalue = UnwrapStringArray(values);
    try
    {
	    FdoInt32 unlength = values->Length;
	    EXCEPTION_HANDLER(Attach(IntPtr(FdoInCondition::Create(StringToUni(propertyName), unvalue, unlength)), true))
    }
    finally
    {
        if (unvalue != nullptr)
	        delete[] unvalue;
    }
}

NAMESPACE_OSGEO_FDO_FILTER::InCondition::InCondition(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ propertyName, array<System::String^>^ values) : SearchCondition(IntPtr::Zero, false)
{
	FdoString** unvalue = UnwrapStringArray(values);
    try
    {
	    FdoInt32 unlength = values->Length;
	    EXCEPTION_HANDLER(Attach(IntPtr(FdoInCondition::Create(propertyName->GetImpObj(), unvalue, unlength)), true))
    }
    finally
    {
        if (unvalue != nullptr)
	        delete[] unvalue;
    }
}

NAMESPACE_OSGEO_FDO_FILTER::InCondition::InCondition(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ propertyName, NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection^ collection) : SearchCondition(IntPtr::Zero, false)
{
	FdoInCondition* obj = FdoInCondition::Create(propertyName->GetImpObj(), collection->GetImpObj());
	Attach(IntPtr(obj), true); 
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ NAMESPACE_OSGEO_FDO_FILTER::InCondition::PropertyName::get()
{
	FdoIdentifier* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetPropertyName())
	return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^>(ObjectFactory::CreateExpression(IntPtr(result), true));
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::InCondition::PropertyName::set(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetPropertyName((value == nullptr ? nullptr : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection^ NAMESPACE_OSGEO_FDO_FILTER::InCondition::Values::get()
{
	FdoValueExpressionCollection* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetValues())
	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateValueExpressionCollection(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::InCondition::Process(NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessor^ processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessorImp^>(processor))->GetImpObj()))
}

System::String^ NAMESPACE_OSGEO_FDO_FILTER::InCondition::ToStringInternal( NAMESPACE_OSGEO_FDO_COMMANDS::IdentifierCollection^ idCollection )
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->ToStringInternal(idCollection->GetImpObj()))
    return CHECK_STRING(unstr);
}
