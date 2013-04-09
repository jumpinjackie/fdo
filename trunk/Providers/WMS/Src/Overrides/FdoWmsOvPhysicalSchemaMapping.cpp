/*
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
#include "FdoWmsXmlGlobals.h"
#include <WMS/Override/FdoWmsOvPhysicalSchemaMapping.h>

FdoWmsOvPhysicalSchemaMapping::FdoWmsOvPhysicalSchemaMapping(void)
{
    m_ClassMappings = FdoWmsOvClassCollection::Create(this);
}

FdoWmsOvPhysicalSchemaMapping::~FdoWmsOvPhysicalSchemaMapping(void)
{
}

FdoWmsOvPhysicalSchemaMapping* FdoWmsOvPhysicalSchemaMapping::Create()
{
    return new FdoWmsOvPhysicalSchemaMapping();
}

FdoString* FdoWmsOvPhysicalSchemaMapping::GetProvider()
{
    return FdoWmsXmlGlobals::g_WmsProviderName; 
}

FdoWmsOvClassCollection* FdoWmsOvPhysicalSchemaMapping::GetClasses()
{
    return FDO_SAFE_ADDREF(m_ClassMappings.p);
}

void FdoWmsOvPhysicalSchemaMapping::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    return BaseType::InitFromXml(pContext, attrs);
}

FdoXmlSaxHandler* FdoWmsOvPhysicalSchemaMapping::XmlStartElement(
                FdoXmlSaxContext* context, 
                FdoString* uri, 
                FdoString* name, 
                FdoString* qname, 
                FdoXmlAttributeCollection* atts)
{
    FdoXmlSaxHandler* pRet = NULL;

    try 
    {
        VALIDATE_ARGUMENT(name);
        VALIDATE_ARGUMENT(context);
		
		pRet = BaseType::XmlStartElement(context, uri, name, qname, atts);
		if (pRet == NULL) {
            if (FdoCommonStringUtil::StringCompareNoCase(name, FdoWmsXmlGlobals::g_WmsClassDefinitionElement) == 0) {
				FdoWmsOvClassDefinitionP newClass = FdoWmsOvClassDefinition::Create();
				newClass->InitFromXml(context, atts);
				m_ClassMappings->Add(newClass);
				pRet = newClass;
			}
		}
	}
    catch (FdoException* e) 
    {
        context->AddError(e);
        e->Release();
    }

    return pRet;
}

FdoBoolean FdoWmsOvPhysicalSchemaMapping::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    return BaseType::XmlEndElement(context, uri, name, qname);
}

void FdoWmsOvPhysicalSchemaMapping::_writeXml( FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags )
{
    VALIDATE_ARGUMENT(xmlWriter);
    VALIDATE_ARGUMENT(flags);
	
	xmlWriter->WriteStartElement(FdoWmsXmlGlobals::g_WmsSchemaMapping);	//start SchemaMapping
	BaseType::_writeXml(xmlWriter, flags);	//Call base _writeXml to write name and provider attribute
    xmlWriter->WriteAttribute(FdoXml::mXmlnsPref, FdoWmsXmlGlobals::g_WmsXmlnsValue);  //write WMS XMLNS

	FdoInt32 count = m_ClassMappings->GetCount(); //write all classes
	for (int i = 0; i < count; i++)	{
		FdoWmsOvClassDefinitionP classDef = m_ClassMappings->GetItem(i);
		classDef->_writeXml(xmlWriter, flags);
	}

	xmlWriter->WriteEndElement(); //end of SchemaMapping
}

void FdoWmsOvPhysicalSchemaMapping::Dispose(void)
{
    delete this;
}
