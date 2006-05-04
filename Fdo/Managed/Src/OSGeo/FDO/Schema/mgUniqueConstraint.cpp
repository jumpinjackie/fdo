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
#include <Fdo\Schema\UniqueConstraint.h>
#include <Fdo\Schema\ClassType.h>
#include <Fdo\Schema\SchemaElementState.h>
#include <Fdo\Schema\DataType.h>
#include <Fdo\Schema\PropertyType.h>
#include <Fdo\Commands\CommandType.h>
#include <Fdo\Schema\DataPropertyDefinitionCollection.h>

#include "FDO\Schema\mgDataPropertyDefinitionCollection.h"
#include "FDO\Schema\mgUniqueConstraint.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraint::UniqueConstraint(IntPtr unmanaged, Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraint::UniqueConstraint() : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoUniqueConstraint::Create(), true))
}

FdoUniqueConstraint* NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraint::GetImpObj()
{
    return static_cast<FdoUniqueConstraint*>(__super::UnmanagedObject.ToPointer());
}

Void NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraint::Dispose(System::Boolean disposing)
{
	if (disposing)
	{
		// Add your own code here
	}

	if (!Disposed)
	{
		EXCEPTION_HANDLER(GetImpObj()->Release())
		Detach();
	}
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection* NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraint::get_Properties()
{
	FdoDataPropertyDefinitionCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetProperties())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateDataPropertyDefinitionCollection(result, true);
}