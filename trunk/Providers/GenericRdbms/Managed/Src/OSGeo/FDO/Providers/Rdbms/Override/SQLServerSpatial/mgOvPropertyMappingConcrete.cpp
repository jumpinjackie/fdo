//
// Copyright (C) 2004-2008  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#include "stdafx.h"
#include "Rdbms\Override\RdbmsOv.h"
#include "SQLServerSpatial\SqlServerOvPropertyMappingConcrete.h"

#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvPropertyMappingConcrete.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgObjectFactory.h"
#include "FDO\Providers\Rdbms\Override\SQLServerSpatial\mgOvClassDefinition.h"

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyMappingConcrete::OvPropertyMappingConcrete() : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingConcrete(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr((FdoIDisposable*)FdoSqlServerOvPropertyMappingConcrete::Create()), true))
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyMappingConcrete::OvPropertyMappingConcrete(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvPropertyMappingConcrete(unmanaged, autoDelete)
{

}

FdoSqlServerOvPropertyMappingConcrete* NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyMappingConcrete::GetImpObj()
{
	return dynamic_cast<FdoSqlServerOvPropertyMappingConcrete*>((FdoIDisposable*)UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyMappingConcrete::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition^ NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyMappingConcrete::InternalClass::get()
{
	FdoSqlServerOvClassDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetInternalClass())
	
	return NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::ObjectFactory::CreateOvClassDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyMappingConcrete::InternalClass::set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetInternalClass((nullptr == value ? nullptr : value->GetImpObj())))
}