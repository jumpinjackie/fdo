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
#include <Fdo\Filter\Filter.h>
#include <Fdo\Filter\BinaryLogicalOperations.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Filter\mgFilter.h"
#include "FDO\Filter\mgIFilterProcessorImp.h"
#include "FDO\Filter\mgBinaryLogicalOperations.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_FILTER::Filter::Filter(IntPtr unmanaged, Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoFilter* NAMESPACE_OSGEO_FDO_FILTER::Filter::GetImpObj()
{
	return static_cast<FdoFilter*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_FILTER::Filter::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_FILTER::Filter^ NAMESPACE_OSGEO_FDO_FILTER::Filter::Parse(System::String^ filterText)
{
	FdoFilter* result;
	EXCEPTION_HANDLER(result = FdoFilter::Parse(StringToUni(filterText)))
	return static_cast<NAMESPACE_OSGEO_FDO_FILTER::Filter^>(ObjectFactory::CreateFilter(IntPtr(result), true));
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::Filter::Process(NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessor^ processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessorImp^>(processor))->GetImpObj()))
}

System::String^ NAMESPACE_OSGEO_FDO_FILTER::Filter::ToString()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->ToString())
    return CHECK_STRING(unstr);
}

NAMESPACE_OSGEO_FDO_FILTER::Filter^ NAMESPACE_OSGEO_FDO_FILTER::Filter::Combine( NAMESPACE_OSGEO_FDO_FILTER::Filter^ leftFilter, NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperations operation, NAMESPACE_OSGEO_FDO_FILTER::Filter^ rightFilter)
{
	FdoFilter* result;
	EXCEPTION_HANDLER(result = GetImpObj()->Combine(leftFilter->GetImpObj(), static_cast<FdoBinaryLogicalOperations>(operation), rightFilter->GetImpObj()))
	return static_cast<NAMESPACE_OSGEO_FDO_FILTER::Filter^>(ObjectFactory::CreateFilter(IntPtr(result), true));
}

NAMESPACE_OSGEO_FDO_FILTER::Filter^ NAMESPACE_OSGEO_FDO_FILTER::Filter::Combine(System::String^ leftFilter, NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperations operation, NAMESPACE_OSGEO_FDO_FILTER::Filter^ rightFilter)
{
	FdoFilter* result;
	EXCEPTION_HANDLER(result = GetImpObj()->Combine(StringToUni(leftFilter), static_cast<FdoBinaryLogicalOperations>(operation), rightFilter->GetImpObj()))
	return static_cast<NAMESPACE_OSGEO_FDO_FILTER::Filter^>(ObjectFactory::CreateFilter(IntPtr(result), true));
}

NAMESPACE_OSGEO_FDO_FILTER::Filter^ NAMESPACE_OSGEO_FDO_FILTER::Filter::Combine(NAMESPACE_OSGEO_FDO_FILTER::Filter^ leftFilter, NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperations operation, System::String^ rightFilter)
{
	FdoFilter* result;
	EXCEPTION_HANDLER(result = GetImpObj()->Combine(leftFilter->GetImpObj(),  static_cast<FdoBinaryLogicalOperations>(operation), StringToUni(rightFilter)))
	return static_cast<NAMESPACE_OSGEO_FDO_FILTER::Filter^>(ObjectFactory::CreateFilter(IntPtr(result), true));
}

NAMESPACE_OSGEO_FDO_FILTER::Filter^ NAMESPACE_OSGEO_FDO_FILTER::Filter::Combine(System::String^ leftFilter, NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperations operation, System::String^ rightFilter)
{
	FdoFilter* result;
	EXCEPTION_HANDLER(result = GetImpObj()->Combine(StringToUni(leftFilter),  static_cast<FdoBinaryLogicalOperations>(operation), StringToUni(rightFilter)))
	return static_cast<NAMESPACE_OSGEO_FDO_FILTER::Filter^>(ObjectFactory::CreateFilter(IntPtr(result), true));
}
