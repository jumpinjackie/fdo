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
#include <SHP/Override/FdoShpClassDefinition.h>

FdoShpOvClassDefinition* FdoShpOvClassDefinition::Create()
{
    FdoShpOvClassDefinition* pObject = new FdoShpOvClassDefinition();
    VALIDATE_ALLOCATION(pObject);
    return pObject;
}

FdoShpOvClassDefinition::FdoShpOvClassDefinition(void)
{
	m_pathUpdated = false;
    m_Properties = FdoShpOvPropertyDefinitionCollection::Create(this);
    VALIDATE_ALLOCATION(m_Properties);
}

FdoShpOvClassDefinition::~FdoShpOvClassDefinition(void)
{
}

void FdoShpOvClassDefinition::Dispose(void)
{
    delete this;
}

FdoShpOvPropertyDefinitionCollection* FdoShpOvClassDefinition::GetProperties() const
{
    return FDO_SAFE_ADDREF(m_Properties.p);
}

FdoString* FdoShpOvClassDefinition::GetShapeFile() const
{
    return m_ShapeFileLocation;
}

void FdoShpOvClassDefinition::SetShapeFile(FdoString * location)
{
    m_ShapeFileLocation = location;
}

void FdoShpOvClassDefinition::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
	try
	{
        VALIDATE_ARGUMENT(attrs);
        VALIDATE_ARGUMENT(pContext);

        BaseType::InitFromXml(pContext, attrs);

        FdoXmlAttributeP att = attrs->FindItem(FdoShpOvXmlGlobals::g_SHPShapeFileAttribute);
        if (att != NULL) {
            SetShapeFile(att->GetValue());
        }
    }
	catch (FdoException* e)
	{
		pContext->AddError(e);
		e->Release();
	}
}

FdoXmlSaxHandler* FdoShpOvClassDefinition::XmlStartElement(FdoXmlSaxContext* context,FdoString* uri, FdoString* name, FdoString* qname, FdoXmlAttributeCollection* atts)
{
	FdoXmlSaxHandler* pRet = NULL;

	try
	{
        VALIDATE_ARGUMENT(name);
        VALIDATE_ARGUMENT(context);

        pRet = BaseType::XmlStartElement(context, uri, name, qname, atts);
		if (pRet == NULL) {
			if (FdoCommonOSUtil::wcsicmp(name, FdoShpOvXmlGlobals::g_SHPPropertyElement) == 0) {
				FdoShpOvPropertyDefinitionP newProperty = FdoShpOvPropertyDefinition::Create();
				newProperty->InitFromXml(context, atts);
				m_Properties->Add(newProperty);
				pRet = newProperty;
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

FdoBoolean FdoShpOvClassDefinition::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
	return BaseType::XmlEndElement(context, uri, name, qname);
}

void FdoShpOvClassDefinition::_writeXml(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
    VALIDATE_ARGUMENT(xmlWriter);
    VALIDATE_ARGUMENT(flags);
	
	xmlWriter->WriteStartElement(FdoShpOvXmlGlobals::g_SHPClassDefinitionElement);

	BaseType::_writeXml(xmlWriter, flags);	

    xmlWriter->WriteAttribute(FdoShpOvXmlGlobals::g_SHPShapeFileAttribute, GetShapeFile());

    for (FdoInt32 i = 0; i <  m_Properties->GetCount(); i++)	{
		FdoShpOvPropertyDefinitionP propertyDef = m_Properties->GetItem(i);
		propertyDef->_writeXml(xmlWriter, flags);
	}

	xmlWriter->WriteEndElement();
}



// Helper method to find a property mapping by its column name:
FdoShpOvPropertyDefinition* FdoShpOvClassDefinition::FindByColumnName(FdoString* columnName)
{
    FdoPtr<FdoShpOvPropertyDefinition> shpPropertyDef;

    // Find the property override that points to the given column name:
    for (FdoInt32 i=0; i<m_Properties->GetCount(); i++)
    {
        FdoPtr<FdoShpOvPropertyDefinition> tempShpPropertyDef = m_Properties->GetItem(i);
        FdoPtr<FdoShpOvColumnDefinition> tempColumnDef = tempShpPropertyDef->GetColumn();
        if (0==wcscmp(tempColumnDef->GetName(), columnName))
        {
            shpPropertyDef = tempShpPropertyDef;  // automatic addref here
            break;
        }
    }

    // Return what we found (or NULL if not found):
    return FDO_SAFE_ADDREF(shpPropertyDef.p);
}

