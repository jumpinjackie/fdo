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
#include <Fdo\Filter\SpatialCondition.h>
#include <Fdo\Filter\SpatialOperations.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Filter\mgSpatialCondition.h"
#include "FDO\Filter\mgSpatialOperations.h"
#include "FDO\Expression\mgExpression.h"
#include "FDO\Expression\mgIdentifier.h"
#include "FDO\Filter\mgIFilterProcessorImp.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_FILTER::SpatialCondition::SpatialCondition(System::IntPtr unmanaged, System::Boolean autoDelete) : GeometricCondition(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_FILTER::SpatialCondition::SpatialCondition() : GeometricCondition(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoSpatialCondition::Create()), true))
}

NAMESPACE_OSGEO_FDO_FILTER::SpatialCondition::SpatialCondition(System::String^ propertyName, NAMESPACE_OSGEO_FDO_FILTER::SpatialOperations operation, NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ geometry) : GeometricCondition(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoSpatialCondition::Create(StringToUni(propertyName), static_cast<FdoSpatialOperations>(operation), geometry->GetImpObj())), true))
}

NAMESPACE_OSGEO_FDO_FILTER::SpatialCondition::SpatialCondition(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ propertyName, NAMESPACE_OSGEO_FDO_FILTER::SpatialOperations operation,  NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ geometry): GeometricCondition(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoSpatialCondition::Create(propertyName->GetImpObj(), static_cast<FdoSpatialOperations>(operation), geometry->GetImpObj())), true))
}

FdoSpatialCondition* NAMESPACE_OSGEO_FDO_FILTER::SpatialCondition::GetImpObj()
{
	return static_cast<FdoSpatialCondition*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_FILTER::SpatialOperations NAMESPACE_OSGEO_FDO_FILTER::SpatialCondition::Operation::get()
{
	FdoSpatialOperations unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetOperation())
	return static_cast<SpatialOperations>(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::SpatialCondition::Operation::set(NAMESPACE_OSGEO_FDO_FILTER::SpatialOperations value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetOperation(static_cast<FdoSpatialOperations>(value)))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ NAMESPACE_OSGEO_FDO_FILTER::SpatialCondition::Geometry::get()
{
	FdoExpression* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetGeometry())
	return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^>(ObjectFactory::CreateExpression(IntPtr(result), true));
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::SpatialCondition::Geometry::set(NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetGeometry((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::SpatialCondition::Process(NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessor^ processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessorImp^>(processor))->GetImpObj()))
}
