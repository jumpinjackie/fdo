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
#include <Fdo.h>
#include <Fdo\Xml\FeatureWriter.h>

#include "FDO\Xml\mgXmlFeatureFlags.h"
#include "FDO\Xml\mgXmlFeaturePropertyWriter.h"
#include "FDO\Schema\mgClassDefinition.h"
#include "FDO\Commands\mgPropertyValue.h"
#include "FDO\Xml\mgXmlFeatureWriter.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter::XmlFeatureWriter(NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter* writer, NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags* flags) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlFeatureWriter::Create(writer->GetImpObj(), flags->GetImpObj()), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter::XmlFeatureWriter(NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter* writer) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlFeatureWriter::Create(writer->GetImpObj(), NULL), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter::XmlFeatureWriter(NAMESPACE_OSGEO_COMMON_XML::XmlWriter* writer, NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags* flags) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlFeatureWriter::Create(static_cast<FdoXmlWriter*>(writer->UnmanagedObject.ToPointer()), flags->GetImpObj()), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter::XmlFeatureWriter(NAMESPACE_OSGEO_COMMON_XML::XmlWriter* writer) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlFeatureWriter::Create(static_cast<FdoXmlWriter*>(writer->UnmanagedObject.ToPointer()), NULL), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter* NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter::get_FeaturePropertyWriter()
{
	FdoXmlFeaturePropertyWriter* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFeaturePropertyWriter())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlFeaturePropertyWriter(result, true);
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition* NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter::get_ClassDefinition()
{
	FdoClassDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetClassDefinition())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateClassDefinition(result, true);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter::set_ClassDefinition(NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition* classDefinition)
{
	EXCEPTION_HANDLER(GetImpObj()->SetClassDefinition(classDefinition->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter::set_Property(NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue* propertyValue)
{
	EXCEPTION_HANDLER(GetImpObj()->SetProperty(propertyValue->GetImpObj()))
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter* NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter::get_ObjectWriter(System::String* propertyName)
{
	FdoXmlFeatureWriter* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetObjectWriter(StringToUni(propertyName)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlFeatureWriter(result, true);
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter* NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter::get_AssociationWriter(System::String* propertyName)
{
	FdoXmlFeatureWriter* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetAssociationWriter(StringToUni(propertyName)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlFeatureWriter(result, true);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter::WriteFeature(System::String* elementTag)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteFeature(StringToUni(elementTag)))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter::WriteFeature()
{
	EXCEPTION_HANDLER(GetImpObj()->WriteFeature(NULL))
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter::XmlFeatureWriter(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{
	
}

FdoXmlFeatureWriter* NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter::GetImpObj()
{
	return static_cast<FdoXmlFeatureWriter*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}