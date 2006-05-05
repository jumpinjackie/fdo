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
#include "FdoWmsOverrides.h"
#include "FdoWmsXmlGlobals.h"
#include <WMS/Override/FdoWmsOvRasterDefinition.h>

FdoWmsOvRasterDefinition::FdoWmsOvRasterDefinition(void) :
    m_pXmlContentHandler(NULL),
    m_formmatType(FdoWmsOvFormatType_Png)
{
	m_layers = FdoWmsOvLayerCollection::Create(this);
}

FdoWmsOvRasterDefinition::~FdoWmsOvRasterDefinition(void)
{
    FDO_SAFE_RELEASE(m_pXmlContentHandler);
}

void FdoWmsOvRasterDefinition::Dispose()
{
	delete this;
}

FdoWmsOvRasterDefinition* FdoWmsOvRasterDefinition::Create()
{
	return new FdoWmsOvRasterDefinition();
}

FdoWmsOvFormatType FdoWmsOvRasterDefinition::GetFormatType(void) const
{
    return m_formmatType;
}

void FdoWmsOvRasterDefinition::SetFormatType(FdoWmsOvFormatType value)
{
    m_formmatType = value;
}

FdoBoolean FdoWmsOvRasterDefinition::GetTransparent(void) const
{
    return m_transparent;
}

void FdoWmsOvRasterDefinition::SetTransparent(FdoBoolean transparent)
{
    m_transparent = transparent;
}

FdoString* FdoWmsOvRasterDefinition::GetBackgroundColor(void) const
{
    return m_backgroundColor;
}

void FdoWmsOvRasterDefinition::SetBackgroundColor(FdoString* bgColor)
{
    m_backgroundColor = bgColor;
}

FdoString* FdoWmsOvRasterDefinition::GetTimeDimension(void) const
{
    return m_timeDimension;
}

void FdoWmsOvRasterDefinition::SetTimeDimension(FdoString* time)
{
    m_timeDimension = time;
}

FdoString* FdoWmsOvRasterDefinition::GetElevationDimension(void) const
{
    return m_elevationDimension;
}

void FdoWmsOvRasterDefinition::SetElevationDimension(FdoString* dimension)
{
    m_elevationDimension = dimension;
}

FdoString* FdoWmsOvRasterDefinition::GetSpatialContextName(void) const
{
    return m_spatialContext;
}

void FdoWmsOvRasterDefinition::SetSpatialContextName(FdoString* value)
{
    m_spatialContext = value;
}

FdoWmsOvLayerCollection* FdoWmsOvRasterDefinition::GetLayers(void) const
{
    return FDO_SAFE_ADDREF(m_layers.p);
}

void FdoWmsOvRasterDefinition::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    return BaseType::InitFromXml(pContext, attrs);
}

// Handle the start of a sub-element
FdoXmlSaxHandler* FdoWmsOvRasterDefinition::XmlStartElement(
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
			if (FdoCommonStringUtil::StringCompareNoCase(name, FdoWmsXmlGlobals::g_WmsLayerDefinition) == 0) {
				FdoWmsOvLayerDefinitionP pNewObj = FdoWmsOvLayerDefinition::Create();
				pNewObj->InitFromXml(context, atts);
				m_layers->Add(pNewObj);
				pRet = pNewObj;
			}
            else if (FdoCommonStringUtil::StringCompareNoCase(name, FdoWmsXmlGlobals::g_WmsImageFormat) == 0 ||
                     FdoCommonStringUtil::StringCompareNoCase(name, FdoWmsXmlGlobals::g_WmsTransparent) == 0 ||
                     FdoCommonStringUtil::StringCompareNoCase(name, FdoWmsXmlGlobals::g_WmsBackgroundColor) == 0 ||
                     FdoCommonStringUtil::StringCompareNoCase(name, FdoWmsXmlGlobals::g_WmsTimeDimension) == 0 ||
                     FdoCommonStringUtil::StringCompareNoCase(name, FdoWmsXmlGlobals::g_WmsElevationDimension) == 0 ||
                     FdoCommonStringUtil::StringCompareNoCase(name, FdoWmsXmlGlobals::g_WmsSpatialContext) == 0 ||
                     FdoCommonStringUtil::StringCompareNoCase(name, FdoWmsXmlGlobals::g_WmsTransparent) == 0) 
            {
                m_pXmlContentHandler = FdoXmlCharDataHandler::Create();
                pRet = m_pXmlContentHandler;
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

// Handle the end of a sub-element
FdoBoolean FdoWmsOvRasterDefinition::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    try {
        VALIDATE_ARGUMENT(name);
        VALIDATE_ARGUMENT(context);

        BaseType::XmlEndElement(context, uri, name, qname);

        if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::g_WmsImageFormat) == 0) {
            _SetFormatType(m_pXmlContentHandler->GetString());
        }
        else if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::g_WmsTransparent) == 0) {
            _SetTransparent(m_pXmlContentHandler->GetString());
        }
        else if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::g_WmsBackgroundColor) == 0) {
            SetBackgroundColor(m_pXmlContentHandler->GetString());
        }
        else if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::g_WmsTimeDimension) == 0) {
            SetTimeDimension(m_pXmlContentHandler->GetString());
        }
        else if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::g_WmsElevationDimension) == 0) {
            SetElevationDimension(m_pXmlContentHandler->GetString());
        }
        else if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::g_WmsSpatialContext) == 0) {
            SetSpatialContextName(m_pXmlContentHandler->GetString());
        }

        FDO_SAFE_RELEASE(m_pXmlContentHandler);
    }
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }

    return false;
}

// Write this element to XML.
void FdoWmsOvRasterDefinition::_writeXml( FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags )
{
    VALIDATE_ARGUMENT(xmlWriter);
    VALIDATE_ARGUMENT(flags);
	
	xmlWriter->WriteStartElement(FdoWmsXmlGlobals::g_WmsRasterDefinition);
	BaseType::_writeXml(xmlWriter, flags);

    xmlWriter->WriteStartElement(FdoWmsXmlGlobals::g_WmsImageFormat);
    xmlWriter->WriteCharacters(_GetFormatType());
    xmlWriter->WriteEndElement();

    xmlWriter->WriteStartElement(FdoWmsXmlGlobals::g_WmsTransparent);
    xmlWriter->WriteCharacters(_GetTransparent());
    xmlWriter->WriteEndElement();

    xmlWriter->WriteStartElement(FdoWmsXmlGlobals::g_WmsBackgroundColor);
    xmlWriter->WriteCharacters(GetBackgroundColor());
    xmlWriter->WriteEndElement();

    xmlWriter->WriteStartElement(FdoWmsXmlGlobals::g_WmsTimeDimension);
    xmlWriter->WriteCharacters(GetTimeDimension());
    xmlWriter->WriteEndElement();

    xmlWriter->WriteStartElement(FdoWmsXmlGlobals::g_WmsElevationDimension);
    xmlWriter->WriteCharacters(GetElevationDimension());
    xmlWriter->WriteEndElement();

    xmlWriter->WriteStartElement(FdoWmsXmlGlobals::g_WmsSpatialContext);
    xmlWriter->WriteCharacters(GetSpatialContextName());
    xmlWriter->WriteEndElement();

    FdoInt32 count = m_layers->GetCount();
	for (int i = 0; i < count; i++)
	{
		FdoWmsOvLayerDefinitionP layer = m_layers->GetItem(i);
		layer->_writeXml(xmlWriter, flags);
	}

	xmlWriter->WriteEndElement();
}

FdoStringP FdoWmsOvRasterDefinition::_GetFormatType(void) const
{
    FdoStringP type;

    switch(GetFormatType()) 
    {
    case FdoWmsOvFormatType_Png:
        type = FdoWmsXmlGlobals::g_WmsImageFormatPng;
        break;
    case FdoWmsOvFormatType_Tif:
        type = FdoWmsXmlGlobals::g_WmsImageFormatTif;
        break;
    case FdoWmsOvFormatType_Jpg:
        type = FdoWmsXmlGlobals::g_WmsImageFormatJpg;
        break;
    case FdoWmsOvFormatType_Gif:
        type = FdoWmsXmlGlobals::g_WmsImageFormatGif;
        break;
    default:
        throw FdoException::Create (NlsMsgGet (FDOWMS_UNHANDLED_EXCEPTION, "Unhandled FDO WMS Provider Internal Exception in '%1$ls'", L"FdoWmsOvRasterDefinition::_GetFormatType"));
        break;
    };

    return type;
}

void FdoWmsOvRasterDefinition::_SetFormatType(FdoString* value)
{
    VALIDATE_ARGUMENT(value);

    FdoWmsOvFormatType type = FdoWmsOvFormatType_Png;
    if (FdoCommonStringUtil::StringCompareNoCase(value, FdoWmsXmlGlobals::g_WmsImageFormatPng) == 0) {
        type = FdoWmsOvFormatType_Png;
    }
    else if (FdoCommonStringUtil::StringCompareNoCase(value, FdoWmsXmlGlobals::g_WmsImageFormatTif) == 0) {
        type = FdoWmsOvFormatType_Tif;
    }
    else if (FdoCommonStringUtil::StringCompareNoCase(value, FdoWmsXmlGlobals::g_WmsImageFormatJpg) == 0) {
        type = FdoWmsOvFormatType_Jpg;
    }
    else if (FdoCommonStringUtil::StringCompareNoCase(value, FdoWmsXmlGlobals::g_WmsImageFormatGif) == 0) {
        type = FdoWmsOvFormatType_Gif;
    }
    else {
        throw FdoSchemaException::Create (NlsMsgGet (FDOWMS_INVALID_XMLSCHEMA_RASTERFORMATTYPE, "'%1$ls' is not a valid XML Raster Format Type.", value));
    }

    SetFormatType(type);
}

FdoStringP FdoWmsOvRasterDefinition::_GetTransparent(void) const
{
    FdoStringP strTransparent;
    strTransparent = (GetTransparent() == true ? L"true" : L"false");
    return strTransparent;
}

void FdoWmsOvRasterDefinition::_SetTransparent(FdoString* value)
{
    VALIDATE_ARGUMENT(value);

    FdoBoolean transparent = true;
    if (FdoCommonStringUtil::StringCompareNoCase(value, FdoWmsXmlGlobals::g_WmsTrue) == 0) {
        transparent = true;
    }
    else if (FdoCommonStringUtil::StringCompareNoCase(value, FdoWmsXmlGlobals::g_WmsFalse) == 0) {
        transparent = false;
    }
    else {
        throw FdoSchemaException::Create (NlsMsgGet (FDOWMS_INVALID_XMLSCHEMA_TRANSPARENCY_VALUE, "'%1$ls' is not a valid XML Transparency value.", value));
    }

    SetTransparent(transparent);
}

FdoStringP FdoWmsOvRasterDefinition::GetQualifiedName()
{
    FdoStringP ret = this->GetName ();
    FdoPtr<FdoPhysicalElementMapping> parent = this->GetParent ();
    if (parent != NULL)
    {
        FdoStringP parentQName = parent->GetQualifiedName ();
        if (parentQName.GetLength () > 0)
        {
            ret = parentQName + L"." + ret;
        }
    }

    return ret;
}
