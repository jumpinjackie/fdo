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
#include <Fdo\Filter\DistanceCondition.h>
#include <Fdo\Filter\DistanceOperations.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Filter\mgDistanceCondition.h"
#include "FDO\Filter\mgDistanceOperations.h"
#include "FDO\Expression\mgExpression.h"
#include "FDO\Expression\mgIdentifier.h"
#include "FDO\Filter\mgIFilterProcessorImp.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_FILTER::DistanceCondition::DistanceCondition(System::IntPtr unmanaged, System::Boolean autoDelete) : GeometricCondition(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_FILTER::DistanceCondition::DistanceCondition() : GeometricCondition(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoDistanceCondition::Create()), true))
}

NAMESPACE_OSGEO_FDO_FILTER::DistanceCondition::DistanceCondition(System::String^ propertyName, NAMESPACE_OSGEO_FDO_FILTER::DistanceOperations operation, NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ geometry, System::Double distance) : GeometricCondition(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoDistanceCondition::Create(StringToUni(propertyName), static_cast<FdoDistanceOperations>(operation), geometry->GetImpObj(), distance)), true))
}

NAMESPACE_OSGEO_FDO_FILTER::DistanceCondition::DistanceCondition(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ propertyName, NAMESPACE_OSGEO_FDO_FILTER::DistanceOperations operation, NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ geometry, System::Double distance) : GeometricCondition(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoDistanceCondition::Create(propertyName->GetImpObj(), static_cast<FdoDistanceOperations>(operation), geometry->GetImpObj(), distance)), true))
}

FdoDistanceCondition* NAMESPACE_OSGEO_FDO_FILTER::DistanceCondition::GetImpObj()
{
	return static_cast<FdoDistanceCondition*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_FILTER::DistanceOperations NAMESPACE_OSGEO_FDO_FILTER::DistanceCondition::Operation::get()
{
	FdoDistanceOperations unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetOperation())
	return static_cast<DistanceOperations>(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::DistanceCondition::Operation::set(NAMESPACE_OSGEO_FDO_FILTER::DistanceOperations value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetOperation(static_cast<FdoDistanceOperations>(value)))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ NAMESPACE_OSGEO_FDO_FILTER::DistanceCondition::Geometry::get()
{
	FdoExpression* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetGeometry())
	return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^>(ObjectFactory::CreateExpression(IntPtr(result), true));
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::DistanceCondition::Geometry::set(NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetGeometry((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Double NAMESPACE_OSGEO_FDO_FILTER::DistanceCondition::Distance::get()
{
	System::Double d;
	EXCEPTION_HANDLER(d = GetImpObj()->GetDistance())
	return d;
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::DistanceCondition::Distance::set(System::Double value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDistance(value))
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::DistanceCondition::Process(NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessor^ processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessorImp^>(processor))->GetImpObj()))
}
