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

System::Void NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::XmlSpatialContextWriter(NAMESPACE_OSGEO_COMMON_XML::XmlWriter* writer) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlSpatialContextWriter::Create(static_cast<FdoXmlWriter*>(writer->UnmanagedObject.ToPointer())), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::XmlSpatialContextWriter(NAMESPACE_OSGEO_COMMON_XML::XmlWriter* writer, NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags* flags) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlSpatialContextWriter::Create(static_cast<FdoXmlWriter*>(writer->UnmanagedObject.ToPointer()), flags->GetImpObj()), true))
}

FdoXmlSpatialContextWriter* NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::GetImpObj()
{
    return static_cast<FdoXmlSpatialContextWriter*>(__super::UnmanagedObject.ToPointer());
}

System::String* NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::get_Name()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetName())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::set_Name(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetName(StringToUni(value)))
}

System::String* NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::get_Description()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDescription())

		return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::set_Description(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDescription(StringToUni(value)))
}

System::String* NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::get_CoordinateSystem()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCoordinateSystem())

		return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::set_CoordinateSystem(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetCoordinateSystem(StringToUni(value)))
}

System::String* NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::get_CoordinateSystemWkt()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCoordinateSystemWkt())

		return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::set_CoordinateSystemWkt(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetCoordinateSystemWkt(StringToUni(value)))
}

NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::get_ExtentType()
{
	FdoSpatialContextExtentType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetExtentType())

	return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType>(result);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::set_ExtentType(NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetExtentType(static_cast<FdoSpatialContextExtentType>(value)))
}

System::Byte NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::get_Extent() []
{
	FdoByteArray* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetExtent())

	System::Byte mgBuffer __gc[] = FdoByteArrayToByteArray(result->GetData(), result->GetCount());
	result->Release();
	return mgBuffer;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::set_Extent(System::Byte buffer [])
{
	FdoByteArray* umBuffer = ByteArrayToFdoByteArray(buffer);

	EXCEPTION_HANDLER(GetImpObj()->SetExtent(umBuffer))

	umBuffer->Release();
}

System::Double NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::get_XYTolerance()
{
	FdoDouble result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetXYTolerance())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::set_XYTolerance(System::Double value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetXYTolerance(value))
}

System::Double NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::get_ZTolerance()
{
	FdoDouble result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetZTolerance())

		return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::set_ZTolerance(System::Double value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetZTolerance(value))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextWriter::WriteSpatialContext()
{
	EXCEPTION_HANDLER(GetImpObj()->WriteSpatialContext())
}
