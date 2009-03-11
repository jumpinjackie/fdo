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
#include <Fdo\Commands\CommandType.h>
#include <Fdo\Schema\ClassType.h>
#include <Fdo\Xml\FeatureHandler.h>
#include <Fdo\Schema\SchemaElementState.h>

#include "FDO\Xml\mgXmlFeatureHandler.h"

#include "FDO\mgObjectFactory.h"
#include "FDO\Raster\mgIRasterImp.h"
#include "FDO\Schema\mgClassDefinition.h"
#include "FDO\Xml\mgXmlFeatureContext.h"

FdoXmlFeatureHandler* NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::GetImpObj()
{
    return static_cast<FdoXmlFeatureHandler*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler^ NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureDocumentStart(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context)
{
	FdoXmlFeatureHandler* result;

	EXCEPTION_HANDLER(result = GetImpObj()->FeatureDocumentStart(context->GetImpObj()))

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlFeatureHandler(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureDocumentEnd(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context)
{
	EXCEPTION_HANDLER(GetImpObj()->FeatureDocumentEnd(context->GetImpObj()))
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler^ NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureCollectionStart(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ definition)
{
	FdoXmlFeatureHandler* result;

	EXCEPTION_HANDLER(result = GetImpObj()->FeatureCollectionStart(context->GetImpObj(), definition->GetImpObj()))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlFeatureHandler(IntPtr(result), true);
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureCollectionEnd(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context)
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->FeatureCollectionEnd(context == nullptr ? nullptr : context->GetImpObj()))

	return result;
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler^ NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureStart(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ definition)
{
	FdoXmlFeatureHandler* result;

	EXCEPTION_HANDLER(result = GetImpObj()->FeatureStart(context->GetImpObj(), definition->GetImpObj()))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlFeatureHandler(IntPtr(result), true);
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureEnd(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context)
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->FeatureEnd(context == nullptr ? nullptr : context->GetImpObj()))

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, System::Boolean value)
{
	FdoBoolean result;
	
	EXCEPTION_HANDLER(result = !!GetImpObj()->FeatureProperty((context == nullptr ? nullptr : context->GetImpObj()), StringToUni(name), value))

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, System::Byte value)
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->FeatureProperty((context == nullptr ? nullptr : context->GetImpObj()), StringToUni(name), value))

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, System::DateTime value)
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->FeatureProperty((context == nullptr ? nullptr : context->GetImpObj()), StringToUni(name), SystemDateToFdoDateTime(value)))

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, System::Double value)
{
	FdoBoolean result;
	
	EXCEPTION_HANDLER(result = !!GetImpObj()->FeatureProperty((context == nullptr ? nullptr : context->GetImpObj()), StringToUni(name), value))

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, System::Int16 value)
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->FeatureProperty((context == nullptr ? nullptr : context->GetImpObj()), StringToUni(name), value))

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, System::Int32 value)
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->FeatureProperty((context == nullptr ? nullptr : context->GetImpObj()), StringToUni(name), value))

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, System::Int64 value)
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->FeatureProperty((context == nullptr ? nullptr : context->GetImpObj()), StringToUni(name), value))

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, System::Single value)
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->FeatureProperty((context == nullptr ? nullptr : context->GetImpObj()), StringToUni(name), value))

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, System::String^ value)
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->FeatureProperty((context == nullptr ? nullptr : context->GetImpObj()), StringToUni(name), StringToUni(value)))

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureStartRasterProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, NAMESPACE_OSGEO_FDO_RASTER::IRaster^ value)
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->FeatureStartRasterProperty((context == nullptr ? nullptr : context->GetImpObj()), StringToUni(name), (value == nullptr ? nullptr : static_cast<NAMESPACE_OSGEO_FDO_RASTER::IRasterImp^>(value)->GetImpObj())))

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureEndRasterProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context)
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->FeatureEndRasterProperty(context == nullptr ? nullptr : context->GetImpObj()))

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureStartLobProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name)
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->FeatureStartLobProperty((context == nullptr ? nullptr : context->GetImpObj()), StringToUni(name)))

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureEndLobProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context)
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->FeatureEndLobProperty(context == nullptr ? nullptr : context->GetImpObj()))

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureGeometricProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, array<System::Byte>^ buffer)
{
	FdoBoolean result;
	FdoByteArray* arr = ByteArrayToFdoByteArray(buffer);
	try
    {
	    EXCEPTION_HANDLER(result = !!GetImpObj()->FeatureGeometricProperty((context == nullptr ? nullptr : context->GetImpObj()), StringToUni(name), arr->GetData(), buffer->Length))
    }
    finally
    {
        if (arr != nullptr)
	        arr->Release();
    }
	return result;
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler^ NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureStartObjectProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ value)
{
	FdoXmlFeatureHandler* result;

	EXCEPTION_HANDLER(result = GetImpObj()->FeatureStartObjectProperty(context->GetImpObj(), StringToUni(name), (value == nullptr ? nullptr : value->GetImpObj())))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlFeatureHandler(IntPtr(result), true);
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureEndObjectProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context)
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->FeatureEndObjectProperty(context == nullptr ? nullptr : context->GetImpObj()))

	return result;
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler^ NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureStartAssociationProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ value)
{
	FdoXmlFeatureHandler* result;

	EXCEPTION_HANDLER(result = GetImpObj()->FeatureStartAssociationProperty(context->GetImpObj(), StringToUni(name), (value == nullptr ? nullptr : value->GetImpObj())))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlFeatureHandler(IntPtr(result), true);
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureEndAssociationProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context)
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->FeatureEndAssociationProperty(context == nullptr ? nullptr : context->GetImpObj()))

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureCharacters(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ value)
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->FeatureCharacters((context == nullptr ? nullptr : context->GetImpObj()), StringToUni(value)))

	return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::FeatureBinaryData(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, array<System::Byte>^ buffer)
{
	FdoBoolean result;
	FdoByteArray* arr = ByteArrayToFdoByteArray(buffer);
    try
    {
	    EXCEPTION_HANDLER(result = !!GetImpObj()->FeatureBinaryData((context == nullptr ? nullptr : context->GetImpObj()), arr->GetData(), buffer->Length))
    }
    finally
    {
        if (arr != nullptr)
	        arr->Release();
    }
	return result;
}


NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler::XmlFeatureHandler(System::IntPtr unmanaged, System::Boolean autoDelete) :NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{

}