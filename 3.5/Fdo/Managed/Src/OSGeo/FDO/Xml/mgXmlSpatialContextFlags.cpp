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
#include <Fdo\Xml\SpatialContextFlags.h>

#include "FDO\Xml\mgXmlSpatialContextFlags.h"

NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags::XmlSpatialContextFlags() : NAMESPACE_OSGEO_FDO_XML::XmlFlags(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlSpatialContextFlags::Create()), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags::XmlSpatialContextFlags(System::String^ location) : NAMESPACE_OSGEO_FDO_XML::XmlFlags(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlSpatialContextFlags::Create(StringToUni(location))), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags::XmlSpatialContextFlags(System::String^ location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel) : NAMESPACE_OSGEO_FDO_XML::XmlFlags(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlSpatialContextFlags::Create(StringToUni(location), static_cast<FdoXmlFlags::ErrorLevel>(errorLevel))), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags::XmlSpatialContextFlags(System::String^ location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel, System::Boolean nameAdjust) : NAMESPACE_OSGEO_FDO_XML::XmlFlags(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlSpatialContextFlags::Create(StringToUni(location), static_cast<FdoXmlFlags::ErrorLevel>(errorLevel), nameAdjust)), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags::XmlSpatialContextFlags(System::String^ location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel, System::Boolean nameAdjust, NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags::ConflictOption conflictOption) : NAMESPACE_OSGEO_FDO_XML::XmlFlags(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlSpatialContextFlags::Create(StringToUni(location), static_cast<FdoXmlFlags::ErrorLevel>(errorLevel), nameAdjust, static_cast<FdoXmlSpatialContextFlags::ConflictOption>(conflictOption))), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags::XmlSpatialContextFlags(System::String^ location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel, System::Boolean nameAdjust, NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags::ConflictOption conflictOption, System::Boolean includeDefault) : NAMESPACE_OSGEO_FDO_XML::XmlFlags(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlSpatialContextFlags::Create(StringToUni(location), static_cast<FdoXmlFlags::ErrorLevel>(errorLevel), nameAdjust, static_cast<FdoXmlSpatialContextFlags::ConflictOption>(conflictOption), includeDefault)), true))
}

inline FdoXmlSpatialContextFlags* NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags::GetImpObj()
{
	return static_cast<FdoXmlSpatialContextFlags*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags::SetConflictOption(NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags::ConflictOption conflictOption)
{
	EXCEPTION_HANDLER(GetImpObj()->SetConflictOption(static_cast<FdoXmlSpatialContextFlags::ConflictOption>(conflictOption)))
}

NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags::ConflictOption NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags::GetConflictOption()
{
	FdoXmlSpatialContextFlags::ConflictOption result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetConflictOption())

	return static_cast<NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags::ConflictOption>(result);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags::IncludeDefault::set(System::Boolean includeDefault)
{
	EXCEPTION_HANDLER(GetImpObj()->SetIncludeDefault(includeDefault))
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags::IncludeDefault::get()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetIncludeDefault())

	return result;
}
