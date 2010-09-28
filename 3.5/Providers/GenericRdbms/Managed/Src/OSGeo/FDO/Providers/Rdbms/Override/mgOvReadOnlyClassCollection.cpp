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
#include <RdbmsOv.h>
#include <RdbmsOvClassDefinition.h>
#include <RdbmsOvClassCollection.h>
#include <Utilities/SchemaMgr/Overrides/TableMappingType.h>

#include "mgOvTableMappingType.h"
#include "mgOvClassDefinition.h"
#include "mgOvReadOnlyClassCollection.h"
#include "mgObjectFactory.h"

FdoRdbmsOvReadOnlyClassCollection* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyClassCollection::GetImpObj()
{
    return static_cast<FdoRdbmsOvReadOnlyClassCollection*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyClassCollection::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyClassCollection::OvReadOnlyClassCollection(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON::CollectionReadOnlyBase(unmanaged, autoDelete)
{

}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyClassCollection::CopyTo(array<NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition^>^ pArray, System::Int32 index)
{
	if (nullptr == pArray)
		throw gcnew System::ArgumentNullException();
	if (index < 0)
		throw gcnew System::ArgumentOutOfRangeException();
	if (pArray->Rank != 1 || index >= pArray->Length || this->Count + index > pArray->Length)
		throw gcnew System::ArgumentException();

	for (System::Int32 i = 0; i < this->Count; i++)
        pArray[index+i] = this->Item[i];
}

System::Int32 NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyClassCollection::Count::get(System::Void)
{
	System::Int32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

	return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyClassCollection::IndexOf(OvClassDefinition^ value)
{
	System::Int32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == nullptr ? nullptr : value->GetImpObj())))

	return index;
}

System::Boolean NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyClassCollection::Contains(OvClassDefinition^ value)
{
	System::Boolean exist;

	EXCEPTION_HANDLER(exist = (!!GetImpObj()->Contains((value == nullptr ? nullptr : value->GetImpObj()))))

	return exist;
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyClassCollection::Item::get(System::Int32 index)
{
	FdoRdbmsOvClassDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::ObjectFactory::CreateOvClassDefinition(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyClassCollection::Item::get(System::String^ index)
{
	FdoRdbmsOvClassDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetItem(StringToUni(index)))

	return NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::ObjectFactory::CreateOvClassDefinition(IntPtr(result), true);
}

System::Object^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyClassCollection::IndexInternal::get(System::Int32 index)
{
	return this->Item[index];
}
