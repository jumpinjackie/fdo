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
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Overrides/FdoGrfpPhysicalSchemaMapping.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-26 13:02:46 -0500 (Thu, 26 Jan 2006) $
 * $Revision: 24 $
 *
 */
#include "Overrides.h"
#include "FdoRfpGlobals.h"
#include "FdoGrfpXmlGlobals.h"
#include <GdalFile/Override/FdoGrfpClassDefinition.h>
#include <GdalFile/Override/FdoGrfpPhysicalSchemaMapping.h>

FdoGrfpPhysicalSchemaMapping* FdoGrfpPhysicalSchemaMapping::Create()
{
	return new FdoGrfpPhysicalSchemaMapping();
}


FdoGrfpPhysicalSchemaMapping::FdoGrfpPhysicalSchemaMapping(void)
{
	m_ClassMappings = FdoGrfpClassCollection::Create(this);
}

FdoGrfpPhysicalSchemaMapping::~FdoGrfpPhysicalSchemaMapping(void)
{
}

void FdoGrfpPhysicalSchemaMapping::Dispose()
{
	delete this;
}

FdoGrfpClassCollection* FdoGrfpPhysicalSchemaMapping::GetClasses()
{
	return FDO_SAFE_ADDREF(m_ClassMappings.p);
}

void FdoGrfpPhysicalSchemaMapping::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
	try
	{
        if (attrs == NULL || pContext == NULL) 
            throw FdoCommandException::Create(NlsMsgGet(GRFP_3_NULL_ARGUMENT, "A required argument was set to NULL"));
		
		BaseType::InitFromXml(pContext, attrs);
	}
	catch (FdoException* ex)
	{
		pContext->AddError(ex);
		ex->Release();
	}
}

FdoString* FdoGrfpPhysicalSchemaMapping::GetProvider()
{
	return FdoGrfpGlobals::GRFPProviderName;
}

FdoXmlSaxHandler* FdoGrfpPhysicalSchemaMapping::XmlStartElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname, 
    FdoXmlAttributeCollection* atts
)
{
    FdoXmlSaxHandler* pRet = NULL;
    try 
	{
        if (context == NULL || name == NULL)
            throw FdoCommandException::Create(NlsMsgGet(GRFP_3_NULL_ARGUMENT, "A required argument was set to NULL"));
		
		pRet = BaseType::XmlStartElement(context, uri, name, qname, atts);
		if (pRet == NULL)
		{
			if (STREQUAL(name, FdoGrfpXmlGlobals::ClassDefinitionElement))
			{
				FdoGrfpClassDefinitionP newClass = FdoGrfpClassDefinition::Create();
				newClass->InitFromXml(context, atts);
				m_ClassMappings->Add(newClass);
				pRet = newClass;
			}
		}
	}
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }

    return pRet;
}

FdoBoolean FdoGrfpPhysicalSchemaMapping::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
	return BaseType::XmlEndElement(context, uri, name, qname);
}

void FdoGrfpPhysicalSchemaMapping::_writeXml( FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags )
{
    if (xmlWriter == NULL || flags == NULL)
        throw FdoCommandException::Create(NlsMsgGet(GRFP_3_NULL_ARGUMENT, "A required argument was set to NULL"));
	
	xmlWriter->WriteStartElement(FdoGrfpXmlGlobals::SchemaMapping);	//start SchemaMapping

	BaseType::_writeXml(xmlWriter, flags);	//Call base _writeXml to write name and provider attribute
    xmlWriter->WriteAttribute(FdoXml::mXmlnsPref, FdoGrfpXmlGlobals::XMLNS_Value);  //write RFP XMLNS

	//write all classes
	FdoInt32 count = m_ClassMappings->GetCount();
	for (int i = 0; i < count; i++)
	{
		FdoGrfpClassDefinitionP classDef = m_ClassMappings->GetItem(i);
		classDef->_writeXml(xmlWriter, flags);
	}

	xmlWriter->WriteEndElement(); //end of SchemaMapping
}
