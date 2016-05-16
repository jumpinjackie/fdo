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
#include <WMS/Override/FdoWmsOvLayerDefinition.h>

FdoWmsOvLayerDefinition::FdoWmsOvLayerDefinition(void)
{
}

FdoWmsOvLayerDefinition::~FdoWmsOvLayerDefinition(void)
{
}

FdoWmsOvLayerDefinition* FdoWmsOvLayerDefinition::Create()
{
    return new FdoWmsOvLayerDefinition;
}

FdoWmsOvStyleDefinition* FdoWmsOvLayerDefinition::GetStyle(void) const
{
    return FDO_SAFE_ADDREF(m_styleDefinition.p);
}

void FdoWmsOvLayerDefinition::SetStyle(FdoWmsOvStyleDefinition* style)
{
    m_styleDefinition = FDO_SAFE_ADDREF(style);
}

void FdoWmsOvLayerDefinition::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    return BaseType::InitFromXml(pContext, attrs);
}

FdoXmlSaxHandler* FdoWmsOvLayerDefinition::XmlStartElement(
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
            if (FdoCommonStringUtil::StringCompareNoCase(name, FdoWmsXmlGlobals::g_WmsStyleDefinition) == 0) {
				FdoWmsOvStyleDefinitionP pNewObj = FdoWmsOvStyleDefinition::Create();
				pNewObj->InitFromXml(context, atts);
				SetStyle(pNewObj);
				pRet = pNewObj;
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

FdoBoolean FdoWmsOvLayerDefinition::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    return BaseType::XmlEndElement(context, uri, name, qname);
}

void FdoWmsOvLayerDefinition::_writeXml( FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags )
{
    VALIDATE_ARGUMENT(xmlWriter);
    VALIDATE_ARGUMENT(flags);
	
	xmlWriter->WriteStartElement(FdoWmsXmlGlobals::g_WmsLayerDefinition);
	BaseType::_writeXml(xmlWriter, flags);

	FdoWmsOvStyleDefinitionP style = GetStyle();
    if (style)
		style->_writeXml(xmlWriter, flags);

	xmlWriter->WriteEndElement();
}

void FdoWmsOvLayerDefinition::Dispose(void)
{
    delete this;
}

