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
#include <Fdo\Raster\RasterDataModel.h>
#include <Fdo\Expression\DataValue.h>
#include <Fdo\Commands\Feature\IFeatureReader.h>
#include <Fdo\Xml\FeatureFlags.h>
#include <Fdo\Xml\FeatureReader.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\mgObjectFactory.h"
#include "FDO\Xml\mgXmlFeatureReader.h"
#include "FDO\Xml\mgXmlFeatureFlags.h"
#include "FDO\Xml\mgXmlFeaturePropertyReader.h"
#include "FDO\Schema\mgFeatureSchemaCollection.h"
#include "FDO\Schema\mgClassDefinition.h"
#include "FDO\Expression\mgLOBValue.h"
#include "FDO\Raster\mgIRasterImp.h"

NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::XmlFeatureReader(NAMESPACE_OSGEO_COMMON_XML::XmlReader* reader) : NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler(System::IntPtr::Zero, false)
{
	if(reader == NULL)
	{
		EXCEPTION_HANDLER(Attach(FdoXmlFeatureReader::Create(NULL), true))
	}
	else
	{
		EXCEPTION_HANDLER(Attach(FdoXmlFeatureReader::Create(static_cast<FdoXmlReader*>(reader->UnmanagedObject.ToPointer())), true))
	}
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::XmlFeatureReader(NAMESPACE_OSGEO_COMMON_XML::XmlReader* reader, NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags* flags) : NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler(System::IntPtr::Zero, false)
{
	FdoXmlReader * unreader;
	if(reader == NULL)
	{
		unreader = NULL;
	}
	else
	{
		unreader = static_cast<FdoXmlReader*>(reader->UnmanagedObject.ToPointer());
	}
	FdoXmlFeatureFlags* unflag;
	if(flags == NULL)
	{
		unflag = NULL;
	}
	else
	{
		unflag = flags->GetImpObj();
	}
	EXCEPTION_HANDLER(Attach(FdoXmlFeatureReader::Create(unreader, unflag), true))
}

FdoXmlFeatureReader* NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::GetImpObj()
{
    return static_cast<FdoXmlFeatureReader*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyReader* NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::GetFeaturePropertyReader()
{
	FdoXmlFeaturePropertyReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFeaturePropertyReader())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlFeaturePropertyReader(result, true);
}

NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection* NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::get_FeatureSchemas()
{
	FdoFeatureSchemaCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFeatureSchemas())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateFeatureSchemaCollection(result, true);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::set_FeatureSchemas(NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection* schemas)
{
	EXCEPTION_HANDLER(GetImpObj()->SetFeatureSchemas(schemas->GetImpObj()))
}

/*
	IFeatureReader implementation
*/
NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition* NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::GetClassDefinition()
{
	FdoClassDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetClassDefinition())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateClassDefinition(result, true);
}

System::Int32 NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::GetDepth()
{
	FdoInt32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDepth())

		return result;
}

NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReader* NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::GetFeatureObject(System::String* propertyName)
{
	FdoIFeatureReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFeatureObject(StringToUni(propertyName)))

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlFeatureReader(result, true);
}

/*
	IReader implementation
*/
System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::GetBoolean(System::String* name)
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetBoolean(StringToUni(name)))

	return result;
}

System::Byte NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::GetByte(System::String* name)
{
	FdoByte result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetByte(StringToUni(name)))

	return result;
}

System::DateTime NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::GetDateTime(System::String* name)
{
	FdoDateTime result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDateTime(StringToUni(name)))

	return FdoDateTimeToDateTime(result);
}

System::Double NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::GetDouble(System::String* name)
{
	FdoDouble result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDouble(StringToUni(name)))

	return result;
}

System::Int16 NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::GetInt16(System::String* name)
{
	FdoInt16 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetInt16(StringToUni(name)))

	return result;
}

System::Int32 NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::GetInt32(System::String* name)
{
	FdoInt32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetInt32(StringToUni(name)))

	return result;
}

System::Int64 NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::GetInt64(System::String* name)
{
	FdoInt64 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetInt64(StringToUni(name)))

	return result;
}

System::Single NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::GetSingle(System::String* name)
{
	FdoFloat result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetSingle(StringToUni(name)))

	return result;
}

System::String* NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::GetString(System::String* name)
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetString(StringToUni(name)))

	return result;
}

NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue* NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::GetLOB(System::String* name)
{
	FdoLOBValue* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLOB(StringToUni(name)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateLOBValue(result, true);
}

NAMESPACE_OSGEO_COMMON::IStreamReader* NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::GetLOBStreamReader(System::String* name)
{
	FdoIStreamReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLOBStreamReader(StringToUni(name)))

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateIStreamReader(result, true);
}

NAMESPACE_OSGEO_FDO_RASTER::IRaster* NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::GetRaster(System::String* name)
{
	FdoIRaster* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetRaster(StringToUni(name)))

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIRaster(result, true);
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::IsNull(System::String* name)
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->IsNull(StringToUni(name)))

	return result;
}

System::Byte NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::GetGeometry(System::String* name) []
{
	FdoByteArray* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetGeometry(StringToUni(name)))

	System::Byte mgBuffer __gc[] = FdoByteArrayToByteArray(result->GetData(), result->GetCount());
	result->Release();
	return mgBuffer;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::ReadNext()
{
	FdoBoolean result;

    EXCEPTION_HANDLER(result = GetImpObj()->ReadNext())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader::Close()
{
	EXCEPTION_HANDLER(GetImpObj()->Close())
}

