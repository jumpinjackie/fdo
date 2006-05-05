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
#include <WMS/Override/FdoWmsOvClassDefinition.h>
#include <WMS/Override/FdoWmsOvRasterDefinition.h>

FdoWmsOvClassDefinition::FdoWmsOvClassDefinition(void)
{
}

FdoWmsOvClassDefinition::~FdoWmsOvClassDefinition(void)
{
}

FdoWmsOvClassDefinition* FdoWmsOvClassDefinition::Create()
{
    return new FdoWmsOvClassDefinition();
}

FdoWmsOvRasterDefinition* FdoWmsOvClassDefinition::GetRasterDefinition()
{
	return FDO_SAFE_ADDREF(m_rasterDefinition.p);
}

void FdoWmsOvClassDefinition::SetRasterDefinition(FdoWmsOvRasterDefinition* rasterDef)
{
	m_rasterDefinition = FDO_SAFE_ADDREF(rasterDef);
	m_rasterDefinition->SetParent(this);
}

FdoStringP FdoWmsOvClassDefinition::GetQualifiedName()
{
    FdoStringP ret = this->GetName ();
    FdoPtr<FdoPhysicalElementMapping> parent = this->GetParent ();
    if (parent != NULL)
    {
        FdoStringP parentName = parent->GetName ();
        if (parentName.GetLength() > 0)
            ret = parentName + L":" + ret;
    }

    return ret;
}

void FdoWmsOvClassDefinition::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    return BaseType::InitFromXml(pContext, attrs);
}

FdoXmlSaxHandler* FdoWmsOvClassDefinition::XmlStartElement(
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
			if (FdoCommonStringUtil::StringCompareNoCase(name, FdoWmsXmlGlobals::g_WmsRasterDefinition) == 0)	{
				FdoWmsOvRasterDefinitionP rasterDefinition = FdoWmsOvRasterDefinition::Create();
				rasterDefinition->InitFromXml(context, atts);
				SetRasterDefinition(rasterDefinition);
				pRet = rasterDefinition;
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

FdoBoolean FdoWmsOvClassDefinition::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    return BaseType::XmlEndElement(context, uri, name, qname);
}

void FdoWmsOvClassDefinition::_writeXml( FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags )
{
    VALIDATE_ARGUMENT(xmlWriter);
    VALIDATE_ARGUMENT(flags);
    
	xmlWriter->WriteStartElement(FdoWmsXmlGlobals::g_WmsClassDefinitionElement);
	BaseType::_writeXml(xmlWriter, flags);
	m_rasterDefinition->_writeXml(xmlWriter, flags);
	xmlWriter->WriteEndElement();
}

void FdoWmsOvClassDefinition::Dispose(void)
{
    delete this;
}
