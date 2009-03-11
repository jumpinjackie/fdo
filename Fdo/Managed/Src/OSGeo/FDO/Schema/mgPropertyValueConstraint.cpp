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
#include <Fdo\Schema\PropertyValueConstraint.h>

#include "FDO\Schema\mgPropertyValueConstraint.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraint::PropertyValueConstraint(IntPtr unmanaged, Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{

}

FdoPropertyValueConstraint* NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraint::GetImpObj()
{
    return static_cast<FdoPropertyValueConstraint*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintType NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraint::ConstraintType::get()
{
	FdoPropertyValueConstraintType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetConstraintType())

	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintType>(result);
}