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
#include <Fdo\Xml\SpatialContextReader.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Xml\mgXmlSpatialContextReader.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Xml\mgXmlSpatialContextFlags.h"

NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextReader::XmlSpatialContextReader(NAMESPACE_OSGEO_COMMON_XML::XmlReader^ reader) : NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlSpatialContextReader::Create(static_cast<FdoXmlReader*>(reader->UnmanagedObject.ToPointer()))), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextReader::XmlSpatialContextReader(NAMESPACE_OSGEO_COMMON_XML::XmlReader^ reader, NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags^ flags) : NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlSpatialContextReader::Create(static_cast<FdoXmlReader*>(reader->UnmanagedObject.ToPointer()), flags->GetImpObj())), true))
}

FdoXmlSpatialContextReader* NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextReader::GetImpObj()
{
    return static_cast<FdoXmlSpatialContextReader*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_COMMON_XML::XmlReader^ NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextReader::GetXmlReader()
{
	FdoXmlReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetXmlReader())

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateXmlReader(IntPtr(result), true);
}

System::String^ NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextReader::GetName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetName())

	return CHECK_STRING(result);
}

System::String^ NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextReader::GetDescription()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDescription())

	return CHECK_STRING(result);
}

System::String^ NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextReader::GetCoordinateSystem()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCoordinateSystem())

	return CHECK_STRING(result);
}

System::String^ NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextReader::GetCoordinateSystemWkt()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCoordinateSystemWkt())

	return CHECK_STRING(result);
}

NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextReader::GetExtentType()
{
	FdoSpatialContextExtentType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetExtentType())

	return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType>(result);
}

array<System::Byte>^ NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextReader::GetExtent()
{
    FdoByteArray* arr = nullptr;
    array<System::Byte>^ result;
    try
    {
	    EXCEPTION_HANDLER(arr = GetImpObj()->GetExtent())
	    result = FdoByteArrayToByteArray(arr->GetData(), arr->GetCount());
    }
    finally
    {
        if (arr != nullptr)
            arr->Release();
    }
	return result;
}

System::Double NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextReader::GetXYTolerance()
{
	System::Double result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetXYTolerance())

	return result;
}

System::Double NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextReader::GetZTolerance()
{
	System::Double result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetZTolerance())

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextReader::IsActive()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->IsActive())

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextReader::ReadNext()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->ReadNext())

	return result;
}
