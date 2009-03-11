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
#include <RdbmsOvAssociationPropertyDefinition.h>
#include <RdbmsOvPropertyDefinitionCollection.h>
#include "mgOvPropertyDefinition.h"
#include "mgOvReadOnlyPropertyDefinitionCollection.h"
#include "mgOvAssociationPropertyDefinition.h"
#include "mgObjectFactory.h"

FdoRdbmsOvAssociationPropertyDefinition* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvAssociationPropertyDefinition::GetImpObj()
{
	return dynamic_cast<FdoRdbmsOvAssociationPropertyDefinition*>((FdoIDisposable*)__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvAssociationPropertyDefinition::OvAssociationPropertyDefinition(IntPtr unmanaged, Boolean autoDelete) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyDefinition(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvReadOnlyPropertyDefinitionCollection^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvAssociationPropertyDefinition::Properties::get()
{
	FdoRdbmsOvReadOnlyPropertyDefinitionCollection * result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetProperties())

	return NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::ObjectFactory::CreateOvReadOnlyPropertyDefinitionCollection(IntPtr(result), true);
}
