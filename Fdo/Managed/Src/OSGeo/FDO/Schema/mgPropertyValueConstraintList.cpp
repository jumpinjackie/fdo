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
#include <Fdo\Schema\ClassType.h>
#include <Fdo\Schema\SchemaElementState.h>
#include <Fdo\Schema\DataType.h>
#include <Fdo\Schema\PropertyType.h>
#include <Fdo\Commands\CommandType.h>
#include <Fdo\Schema\PropertyValueConstraintType.h>
#include <Fdo\Schema\PropertyValueConstraintList.h>
#include <Fdo\Expression\DataValueCollection.h>

#include "FDO\Schema\mgPropertyValueConstraint.h"
#include <Fdo\Expression\mgDataValueCollection.h>
#include "FDO\Schema\mgPropertyValueConstraintList.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintList::PropertyValueConstraintList(IntPtr unmanaged, Boolean autoDelete) : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraint(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintList::PropertyValueConstraintList() : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraint(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoPropertyValueConstraintList::Create()), true))
}

FdoPropertyValueConstraintList* NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintList::GetImpObj()
{
    return static_cast<FdoPropertyValueConstraintList*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DataValueCollection^ NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintList::ConstraintList::get()
{
	FdoDataValueCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetConstraintList())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateEDataValueCollection(IntPtr(result), true);
}
