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
#include <SHP/Override/FdoShpPhysicalSchemaMapping.h>
#include "../Provider/ShpProvider.h"

FdoShpOvPhysicalSchemaMapping* FdoShpOvPhysicalSchemaMapping::Create()
{
    FdoShpOvPhysicalSchemaMapping* pObject = new FdoShpOvPhysicalSchemaMapping();
    VALIDATE_ALLOCATION(pObject);
    return pObject;
}

FdoShpOvPhysicalSchemaMapping::FdoShpOvPhysicalSchemaMapping(void)
{
    m_Classes = FdoShpOvClassCollection::Create(this);
    VALIDATE_ALLOCATION(m_Classes);
}

FdoShpOvPhysicalSchemaMapping::~FdoShpOvPhysicalSchemaMapping(void)
{
}

void FdoShpOvPhysicalSchemaMapping::Dispose(void)
{
    delete this;
}

FdoString* FdoShpOvPhysicalSchemaMapping::GetProvider()
{
    return SHP_PROVIDER_NAME;
}

FdoShpOvClassCollection* FdoShpOvPhysicalSchemaMapping::GetClasses() const
{
    return FDO_SAFE_ADDREF(m_Classes.p);
}

void FdoShpOvPhysicalSchemaMapping::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    BaseType::InitFromXml(pContext, attrs);
}

FdoXmlSaxHandler* FdoShpOvPhysicalSchemaMapping::XmlStartElement(FdoXmlSaxContext* context,FdoString* uri, FdoString* name, FdoString* qname, FdoXmlAttributeCollection* atts)
{
    FdoXmlSaxHandler* pRet = NULL;

    try 
    {
        VALIDATE_ARGUMENT(name);
        VALIDATE_ARGUMENT(context);
		
		pRet = BaseType::XmlStartElement(context, uri, name, qname, atts);
		if (pRet == NULL) {
            if (FdoCommonOSUtil::wcsicmp(name, FdoShpOvXmlGlobals::g_SHPClassDefinitionElement) == 0) {
				FdoShpOvClassDefinitionP newClass = FdoShpOvClassDefinition::Create();
				newClass->InitFromXml(context, atts);
				m_Classes->Add(newClass);
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

FdoBoolean FdoShpOvPhysicalSchemaMapping::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
	return BaseType::XmlEndElement(context, uri, name, qname);
}

void FdoShpOvPhysicalSchemaMapping::_writeXml( FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags )
{
    VALIDATE_ARGUMENT(xmlWriter);
    VALIDATE_ARGUMENT(flags);
	
	xmlWriter->WriteStartElement(FdoShpOvXmlGlobals::g_SHPSchemaMapping);

	BaseType::_writeXml(xmlWriter, flags);	

    xmlWriter->WriteAttribute(FdoXml::mXmlnsPref, FdoShpOvXmlGlobals::g_SHPXmlnsValue); 

	for (FdoInt32 i = 0; i <  m_Classes->GetCount(); i++)	{
		FdoShpOvClassDefinitionP classDef = m_Classes->GetItem(i);
		classDef->_writeXml(xmlWriter, flags);
	}

	xmlWriter->WriteEndElement();
}


FdoShpOvClassDefinition* FdoShpOvPhysicalSchemaMapping::FindByShapefile(FdoString *shapefilePath)
{
    FdoPtr<FdoShpOvClassDefinition> shpClassDef;

    // Find the class override that points to the given shapefileName:
    for (FdoInt32 i=0; i<m_Classes->GetCount(); i++)
    {
        FdoPtr<FdoShpOvClassDefinition> tempShpClassDef = m_Classes->GetItem(i);
        // TODO: do case-insensitive compare on WIN32 platform, case-sensitive on other platforms
        if (0==wcscmp(tempShpClassDef->GetShapeFile(), shapefilePath))
        {
            shpClassDef = tempShpClassDef;  // automatic addref here
            break;
        }
    }

    // Return what we found (or NULL if not found):
    return FDO_SAFE_ADDREF(shpClassDef.p);
}

FdoShpOvClassDefinition* FdoShpOvPhysicalSchemaMapping::FindByClassName(FdoString *class_name)
{
    FdoPtr<FdoShpOvClassDefinition> shpClassDef;

    // Find the class override that points to the given shapefileName:
    for (FdoInt32 i=0; i<m_Classes->GetCount(); i++)
    {
        FdoPtr<FdoShpOvClassDefinition> tempShpClassDef = m_Classes->GetItem(i);
        // TODO: do case-insensitive compare on WIN32 platform, case-sensitive on other platforms
        if (0==wcscmp(tempShpClassDef->GetName(), class_name))
        {
            shpClassDef = tempShpClassDef;  // automatic addref here
            break;
        }
    }

    // Return what we found (or NULL if not found):
    return FDO_SAFE_ADDREF(shpClassDef.p);
}

