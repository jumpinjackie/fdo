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
#include <Fdo\Commands\IFeatureCommand.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\mgIFeatureCommandImp.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Filter\mgFilter.h"
#include "FDO\Expression\mgIdentifier.h"

NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ NAMESPACE_OSGEO_FDO_COMMANDS::IFeatureCommandImp::FeatureClassName::get()
{
	FdoIdentifier* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFeatureClassName())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIdentifier(IntPtr(result), true);
}

FdoIFeatureCommand* NAMESPACE_OSGEO_FDO_COMMANDS::IFeatureCommandImp::GetImpObj()
{
    return static_cast<FdoIFeatureCommand*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_COMMANDS::IFeatureCommandImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::IFeatureCommandImp::FeatureClassName::set(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetFeatureClassName((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::IFeatureCommandImp::SetFeatureClassName(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetFeatureClassName(StringToUni(value)))
}

NAMESPACE_OSGEO_FDO_FILTER::Filter^ NAMESPACE_OSGEO_FDO_COMMANDS::IFeatureCommandImp::Filter::get()
{
	FdoFilter* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFilter())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateFilter(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::IFeatureCommandImp::Filter::set(NAMESPACE_OSGEO_FDO_FILTER::Filter^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetFilter((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS::IFeatureCommandImp::SetFilter(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetFilter(StringToUni(value)))
}
