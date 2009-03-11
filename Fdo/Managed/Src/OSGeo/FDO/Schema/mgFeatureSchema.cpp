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
#include <Fdo\Xml\Serializable.h>
#include <Fdo\Schema\FeatureSchema.h>
#include <Fdo\Xml\Flags.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Schema\mgFeatureSchema.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Schema\mgClassCollection.h"
#include "FDO\Xml\mgXmlFlags.h"

NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema::FeatureSchema() : NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoFeatureSchema::Create()), true))
}

NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema::FeatureSchema(System::String^ name, System::String^ description) : NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoFeatureSchema::Create(StringToUni(name), StringToUni(description))), true))
}

FdoFeatureSchema* NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema::GetImpObj()
{
	return static_cast<FdoFeatureSchema*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection^ NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema::Classes::get()
{
	FdoClassCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetClasses())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateClassCollection(IntPtr(result), true);
}

// TODO: I can't find the delcaration and definition files of class FdoRelationCollection in
//       in unmanaged FDO API.
//FDO_API FdoRelationCollection^ GetRelations();

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema::AcceptChanges()
{
	EXCEPTION_HANDLER(GetImpObj()->AcceptChanges())
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema::RejectChanges()
{
	EXCEPTION_HANDLER(GetImpObj()->RejectChanges())
}

/*
	Implementation of XmlSerializable
*/

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema::WriteXml(String^ fileName)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(StringToUni(fileName)))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema::WriteXml(String^ fileName, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(StringToUni(fileName), flags->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema::WriteXml(NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ xmlWriter)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(static_cast<FdoXmlWriter*>(xmlWriter->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema::WriteXml(NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ xmlWriter, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(static_cast<FdoXmlWriter*>(xmlWriter->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema::WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter^ textWriter)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(static_cast<FdoIoTextWriter*>(textWriter->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema::WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter^ textWriter, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(static_cast<FdoIoTextWriter*>(textWriter->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema::WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoStream^ stream)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(static_cast<FdoIoStream*>(stream->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema::WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoStream^ stream, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags)
{
	EXCEPTION_HANDLER(GetImpObj()->WriteXml(static_cast<FdoIoStream*>(stream->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

NAMESPACE_OSGEO_COMMON_XML::XmlReader^ NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema::GetFromInternalStylesheet()
{
	FdoXmlReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFromInternalStylesheet())

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateXmlReader(IntPtr(result), true);
}
