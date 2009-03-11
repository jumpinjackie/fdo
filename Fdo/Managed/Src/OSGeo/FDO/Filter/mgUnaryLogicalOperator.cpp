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
#include <Fdo\Filter\UnaryLogicalOperator.h>
#include <Fdo\Filter\UnaryLogicalOperations.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Filter\mgUnaryLogicalOperator.h"
#include "FDO\Filter\mgFilter.h"
#include "FDO\Filter\mgUnaryLogicalOperations.h"
#include "FDO\Filter\mgIFilterProcessorImp.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_FILTER::UnaryLogicalOperator::UnaryLogicalOperator(System::IntPtr unmanaged, System::Boolean autoDelete) : LogicalOperator(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_FILTER::UnaryLogicalOperator::UnaryLogicalOperator() : LogicalOperator(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoUnaryLogicalOperator::Create()), true))
}

NAMESPACE_OSGEO_FDO_FILTER::UnaryLogicalOperator::UnaryLogicalOperator(NAMESPACE_OSGEO_FDO_FILTER::Filter^ operand, NAMESPACE_OSGEO_FDO_FILTER::UnaryLogicalOperations operation) : LogicalOperator(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoUnaryLogicalOperator::Create(operand->GetImpObj(), static_cast<FdoUnaryLogicalOperations>(operation))), true)) 
}

FdoUnaryLogicalOperator* NAMESPACE_OSGEO_FDO_FILTER::UnaryLogicalOperator::GetImpObj()
{
	return static_cast<FdoUnaryLogicalOperator*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_FILTER::UnaryLogicalOperations NAMESPACE_OSGEO_FDO_FILTER::UnaryLogicalOperator::Operation::get()
{
	FdoUnaryLogicalOperations unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetOperation())
	return static_cast<UnaryLogicalOperations>(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::UnaryLogicalOperator::Operation::set(NAMESPACE_OSGEO_FDO_FILTER::UnaryLogicalOperations value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetOperation(static_cast<FdoUnaryLogicalOperations>(value)))
}

NAMESPACE_OSGEO_FDO_FILTER::Filter^ NAMESPACE_OSGEO_FDO_FILTER::UnaryLogicalOperator::Operand::get()
{
	FdoFilter* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetOperand())
	return static_cast<NAMESPACE_OSGEO_FDO_FILTER::Filter^>(ObjectFactory::CreateFilter(IntPtr(result), true));
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::UnaryLogicalOperator::Operand::set(NAMESPACE_OSGEO_FDO_FILTER::Filter^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetOperand((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::UnaryLogicalOperator::Process(NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessor^ processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessorImp^>(processor))->GetImpObj()))
}
