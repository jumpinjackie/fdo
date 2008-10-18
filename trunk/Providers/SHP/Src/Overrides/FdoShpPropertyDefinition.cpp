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
#include <SHP/Override/FdoShpPropertyDefinition.h>

FdoShpOvPropertyDefinition* FdoShpOvPropertyDefinition::Create()
{
    FdoShpOvPropertyDefinition* pObject = new FdoShpOvPropertyDefinition();
    VALIDATE_ALLOCATION(pObject);
    return pObject;
}

FdoShpOvPropertyDefinition::FdoShpOvPropertyDefinition(void)
{
}

FdoShpOvPropertyDefinition::~FdoShpOvPropertyDefinition(void)
{
}

void FdoShpOvPropertyDefinition::Dispose(void)
{
    delete this;
}

FdoShpOvColumnDefinition* FdoShpOvPropertyDefinition::GetColumn() const
{
    return FDO_SAFE_ADDREF(m_ColumnDefinition.p);
}

void FdoShpOvPropertyDefinition::SetColumn(FdoShpOvColumnDefinition * definition)
{
    m_ColumnDefinition = FDO_SAFE_ADDREF(definition);
}

void FdoShpOvPropertyDefinition::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    BaseType::InitFromXml(pContext, attrs);
}

FdoXmlSaxHandler* FdoShpOvPropertyDefinition::XmlStartElement(FdoXmlSaxContext* context,FdoString* uri, FdoString* name, FdoString* qname, FdoXmlAttributeCollection* atts)
{
	FdoXmlSaxHandler* pRet = NULL;

	try
	{
        VALIDATE_ARGUMENT(name);
        VALIDATE_ARGUMENT(context);

        pRet = BaseType::XmlStartElement(context, uri, name, qname, atts);
		if (pRet == NULL) {
			if (FdoCommonOSUtil::wcsicmp(name, FdoShpOvXmlGlobals::g_SHPColumnElement) == 0) {
				FdoShpOvColumnDefinitionP newColumn = FdoShpOvColumnDefinition::Create();
				newColumn->InitFromXml(context, atts);
				SetColumn(newColumn);
				pRet = newColumn;
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

FdoBoolean FdoShpOvPropertyDefinition::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
	return BaseType::XmlEndElement(context, uri, name, qname);
}

void FdoShpOvPropertyDefinition::_writeXml(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
    VALIDATE_ARGUMENT(xmlWriter);
    VALIDATE_ARGUMENT(flags);

    xmlWriter->WriteStartElement(FdoShpOvXmlGlobals::g_SHPPropertyElement);

    BaseType::_writeXml(xmlWriter, flags);

    if (m_ColumnDefinition != NULL) {
        m_ColumnDefinition->_writeXml(xmlWriter, flags);
    }

    xmlWriter->WriteEndElement();
}


