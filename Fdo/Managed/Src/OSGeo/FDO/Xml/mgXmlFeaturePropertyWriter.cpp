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
#include <Fdo\Commands\PropertyValue.h>
#include <Fdo\Expression\LOBValue.h>
#include <Fdo\Expression\DataValue.h>
#include <Fdo\Schema\ClassDefinition.h>
#include <Fdo\Xml\Flags.h>
#include <Fdo\Raster\RasterDataModel.h>
#include <Fdo\Raster\IRaster.h>
#include <Fdo\Xml\FeatureFlags.h>
#include <Fdo\Xml\FeaturePropertyWriter.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Xml\mgXmlFeaturePropertyWriter.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Xml\mgXmlFlags.h"
#include "FDO\Raster\mgIRasterImp.h"
#include "FDO\Commands\mgPropertyValue.h"
#include "FDO\Schema\mgClassDefinition.h"
#include "FDO\Expression\mgLOBValue.h"

NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter::XmlFeaturePropertyWriter(NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ writer) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlFeaturePropertyWriter::Create(static_cast<FdoXmlWriter*>(writer->UnmanagedObject.ToPointer()))), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter::XmlFeaturePropertyWriter(NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ writer, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlFeaturePropertyWriter::Create(static_cast<FdoXmlWriter*>(writer->UnmanagedObject.ToPointer()), flags->GetImpObj())), true))
}

FdoXmlFeaturePropertyWriter* NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter::GetImpObj()
{
    return static_cast<FdoXmlFeaturePropertyWriter*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter::GetXmlWriter()
{
	FdoXmlWriter* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetXmlWriter())

    return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateXmlWriter(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter::ClassDefinition::get()
{
	FdoClassDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetClassDefinition())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateClassDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter::ClassDefinition::set(NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ classDefinition)
{
	EXCEPTION_HANDLER(GetImpObj()->SetClassDefinition(classDefinition->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter::WriteFeatureStart(System::String^ startTag)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteFeatureStart(StringToUni(startTag)))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter::WriteFeatureEnd()
{
	EXCEPTION_HANDLER(GetImpObj()->WriteFeatureEnd())
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter::WriteProperty(System::String^ name, NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteProperty(StringToUni(name), (value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter::WriteProperty(System::String^ name, NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue^ value, System::Boolean valueOnly)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteProperty(StringToUni(name), (value == nullptr ? nullptr : value->GetImpObj()), valueOnly))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter::WriteProperty(System::String^ name, System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteProperty(StringToUni(name), StringToUni(value)))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter::WriteProperty(System::String^ name, System::String^ value, System::Boolean valueOnly)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteProperty(StringToUni(name), StringToUni(value), valueOnly))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter::WriteProperty(System::String^ name, NAMESPACE_OSGEO_FDO_RASTER::IRaster^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteProperty(StringToUni(name), static_cast<NAMESPACE_OSGEO_FDO_RASTER::IRasterImp^>(value)->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter::WriteProperty(System::String^ name, NAMESPACE_OSGEO_FDO_RASTER::IRaster^ value, System::Boolean valueOnly)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteProperty(StringToUni(name), static_cast<NAMESPACE_OSGEO_FDO_RASTER::IRasterImp^>(value)->GetImpObj(), valueOnly))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter::WriteProperty(System::String^ name, NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteProperty(StringToUni(name), (value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter::WriteProperty(System::String^ name, NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue^ value, System::Boolean valueOnly)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteProperty(StringToUni(name), (value == nullptr ? nullptr : value->GetImpObj()), valueOnly))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter::WriteProperty(System::String^ name, NAMESPACE_OSGEO_COMMON::IStreamReader^ lobReader)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteProperty(StringToUni(name), static_cast<FdoIStreamReader*>(static_cast<NAMESPACE_OSGEO_COMMON::IStreamReaderImp^>(lobReader)->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter::WriteProperty(System::String^ name, NAMESPACE_OSGEO_COMMON::IStreamReader^ lobReader, System::Boolean valueOnly)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteProperty(StringToUni(name), static_cast<FdoIStreamReader*>(static_cast<NAMESPACE_OSGEO_COMMON::IStreamReaderImp^>(lobReader)->UnmanagedObject.ToPointer()), valueOnly))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter::WriteGeometricProperty(System::String^ name, array<System::Byte>^ buffer)
{
	FdoByteArray* umBuffer = ByteArrayToFdoByteArray(buffer);
    try
    {
    	EXCEPTION_HANDLER(GetImpObj()->WriteGeometricProperty(StringToUni(name), umBuffer->GetData(), buffer->Length))
    }
    finally
    {
        if (umBuffer != nullptr)
	        umBuffer->Release();
    }
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter::WriteGeometricProperty(System::String^ name, array<System::Byte>^ buffer, System::Boolean valueOnly)
{
	FdoByteArray* umBuffer = ByteArrayToFdoByteArray(buffer);
    try
    {
	    EXCEPTION_HANDLER(GetImpObj()->WriteGeometricProperty(StringToUni(name), umBuffer->GetData(), buffer->Length, valueOnly))
    }
    finally
    {
        if (umBuffer != nullptr)
	        umBuffer->Release();
    }
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter::WriteObjectPropertyStart(System::String^ name)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteObjectPropertyStart(StringToUni(name)))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter::WriteObjectPropertyEnd()
{
	EXCEPTION_HANDLER(GetImpObj()->WriteObjectPropertyEnd())
}
