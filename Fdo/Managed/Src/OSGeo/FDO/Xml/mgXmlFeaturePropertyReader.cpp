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
#include <Fdo\Schema\FeatureSchemaCollection.h>
#include <Fdo\Xml\FeatureFlags.h>
#include <Fdo\Xml\FeaturePropertyReader.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Xml\mgXmlFeaturePropertyReader.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Xml\mgXmlFeatureFlags.h"
#include "FDO\Xml\mgXmlFeatureContext.h"
#include "FDO\Xml\mgXmlFeatureHandler.h"
#include "FDO\Schema\mgFeatureSchemaCollection.h"

NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyReader::XmlFeaturePropertyReader(NAMESPACE_OSGEO_COMMON_XML::XmlReader^ reader) : NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlFeaturePropertyReader::Create(static_cast<FdoXmlReader*>(reader->UnmanagedObject.ToPointer()))), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyReader::XmlFeaturePropertyReader(NAMESPACE_OSGEO_COMMON_XML::XmlReader^ reader, NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags^ flags) : NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlFeaturePropertyReader::Create(static_cast<FdoXmlReader*>(reader->UnmanagedObject.ToPointer()), flags->GetImpObj())), true))
}

FdoXmlFeaturePropertyReader* NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyReader::GetImpObj()
{
    return static_cast<FdoXmlFeaturePropertyReader*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_COMMON_XML::XmlReader^ NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyReader::GetXmlReader()
{
	FdoXmlReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetXmlReader())

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateXmlReader(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection^ NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyReader::FeatureSchemas::get()
{
	FdoFeatureSchemaCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFeatureSchemas())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateFeatureSchemaCollection(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyReader::FeatureSchemas::set(NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection^ schemas)
{
	EXCEPTION_HANDLER(GetImpObj()->SetFeatureSchemas(schemas->GetImpObj()))
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyReader::Parse()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->Parse(nullptr, nullptr, false))

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyReader::Parse(NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler^ featureHandler)
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->Parse((featureHandler == nullptr ? nullptr : featureHandler->GetImpObj()), nullptr, false))

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyReader::Parse(NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler^ featureHandler, NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ featureContext)
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->Parse((featureHandler == nullptr ? nullptr : featureHandler->GetImpObj()), (featureContext == nullptr ? nullptr : featureContext->GetImpObj()), false))

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyReader::Parse(NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler^ featureHandler, NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ featureContext, System::Boolean incremental)
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->Parse((featureHandler == nullptr ? nullptr : featureHandler->GetImpObj()), (featureContext == nullptr ? nullptr : featureContext->GetImpObj()), incremental))

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyReader::GetEOF()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetEOF())

	return result;
}
