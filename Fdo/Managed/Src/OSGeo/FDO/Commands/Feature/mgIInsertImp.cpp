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
#include <Fdo\Commands\Feature\IInsert.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\Feature\mgIInsertImp.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Expression\mgIdentifier.h"
#include "FDO\Commands\mgPropertyValueCollection.h"
#include "FDO\Commands\mgBatchParameterValueCollection.h"
#include "FDO\Commands\Feature\mgIFeatureReaderImp.h"

FdoIInsert* NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IInsertImp::GetImpObj()
{
    return static_cast<FdoIInsert*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier* NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IInsertImp::get_FeatureClassName()
{
	FdoIdentifier* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFeatureClassName())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIdentifier(result, true);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IInsertImp::set_FeatureClassName(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetFeatureClassName((value == NULL ? NULL : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IInsertImp::SetFeatureClassName(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetFeatureClassName(StringToUni(value)))
}

NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection* NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IInsertImp::get_PropertyValues()
{
	FdoPropertyValueCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetPropertyValues())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyValueCollection(result, true);
}

NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection* NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IInsertImp::get_BatchParameterValues()
{
	FdoBatchParameterValueCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetBatchParameterValues())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateBatchParameterValueCollection(result, true);
}

NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReader* NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IInsertImp::Execute()
{
	FdoIFeatureReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->Execute())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIFeatureReader(result, true);
}
