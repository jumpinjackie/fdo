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
#include <Fdo\Xml\SpatialContextWriter.h>
#include <Fdo\Xml\mgXmlSpatialContextWriter.h>
#include <Fdo\Xml\mgXmlSpatialContextFlags.h>

NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::XmlSpatialContextWriter(NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ writer) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlSpatialContextWriter::Create(static_cast<FdoXmlWriter*>(writer->UnmanagedObject.ToPointer()))), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::XmlSpatialContextWriter(NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ writer, NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags^ flags) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlSpatialContextWriter::Create(static_cast<FdoXmlWriter*>(writer->UnmanagedObject.ToPointer()), flags->GetImpObj())), true))
}

FdoXmlSpatialContextWriter* NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::GetImpObj()
{
    return static_cast<FdoXmlSpatialContextWriter*>(__super::UnmanagedObject.ToPointer());
}

System::String^ NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::Name::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetName())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::Name::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetName(StringToUni(value)))
}

System::String^ NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::Description::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDescription())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::Description::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDescription(StringToUni(value)))
}

System::String^ NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::CoordinateSystem::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCoordinateSystem())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::CoordinateSystem::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetCoordinateSystem(StringToUni(value)))
}

System::String^ NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::CoordinateSystemWkt::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCoordinateSystemWkt())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::CoordinateSystemWkt::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetCoordinateSystemWkt(StringToUni(value)))
}

NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::ExtentType::get()
{
	FdoSpatialContextExtentType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetExtentType())

	return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType>(result);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::ExtentType::set(NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetExtentType(static_cast<FdoSpatialContextExtentType>(value)))
}

array<System::Byte>^ NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::Extent::get()
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

System::Void NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::Extent::set(array<System::Byte>^ buffer)
{
    FdoByteArray* arr = nullptr;
    try
    {
        arr = ByteArrayToFdoByteArray(buffer);
    	EXCEPTION_HANDLER(GetImpObj()->SetExtent(arr))
    }
    finally
    {
        if (arr != nullptr)
            arr->Release();
    }
}

System::Double NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::XYTolerance::get()
{
	System::Double result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetXYTolerance())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::XYTolerance::set(System::Double value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetXYTolerance(value))
}

System::Double NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::ZTolerance::get()
{
	System::Double result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetZTolerance())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::ZTolerance::set(System::Double value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetZTolerance(value))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::WriteSpatialContext()
{
	EXCEPTION_HANDLER(GetImpObj()->WriteSpatialContext())
}
