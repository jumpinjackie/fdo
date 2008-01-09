/*
 * Copyright (C) 2004-2007  Autodesk, Inc.
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
#include "../Provider/FdoWmsGlobals.h"

FdoWmsOvRasterDefinition::FdoWmsOvRasterDefinition(void) :
    m_pXmlContentHandler(NULL),
    m_formatType(FdoWmsXmlGlobals::g_WmsImageFormatPng),
    m_formatDesc(FdoWmsGlobals::RasterMIMEFormat_PNG),
    m_hasFormat(false),
    m_hasFormatType(false),
    m_transparent(false),
    m_useTileCache(true)
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

FdoString* FdoWmsOvRasterDefinition::GetImageFormat(void) 
{
    if (!m_hasFormatType && m_hasFormat) // value is from WMS 3.2.2 version configuration
    {
        // set <FormatType> interpreted from <Format>
        m_formatDesc = this->AbbrToMIMEFormatType(m_formatType);
    }

    // m_formatDesc is valid for WMS 3.3    
    return m_formatDesc;
}

// Input value would like: "image/png; PhotometricInterpretation=PaletteColor"
// or "image/png"
void FdoWmsOvRasterDefinition::SetImageFormat(FdoString* value)
{
    VALIDATE_ARGUMENT(value);

    // Set the WMS 3.3 tag: <FormatType>
    _SetFullFormatType(value);

    // Maintain the WMS 3.2.2 tag: <Format>PNG</Format> only allows 4 types: PNG, TIF, GIF and JPG.
    FdoStringP format = this->MIMEFormatTypeToAbbr(value);
    _SetFormatAbbr(format);
}

FdoBoolean FdoWmsOvRasterDefinition::GetTransparent(void) const
{
    return m_transparent;
}

void FdoWmsOvRasterDefinition::SetTransparent(FdoBoolean transparent)
{
    m_transparent = transparent;
}

FdoBoolean FdoWmsOvRasterDefinition::GetUseTileCache(void) const
{
    return m_useTileCache;
}

void FdoWmsOvRasterDefinition::SetUseTileCache(FdoBoolean value)
{
    m_useTileCache = value;
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
                     FdoCommonStringUtil::StringCompareNoCase(name, FdoWmsXmlGlobals::g_WmsImageFormatType) == 0 ||
                     FdoCommonStringUtil::StringCompareNoCase(name, FdoWmsXmlGlobals::g_WmsTransparent) == 0 ||
                     FdoCommonStringUtil::StringCompareNoCase(name, FdoWmsXmlGlobals::g_WmsBackgroundColor) == 0 ||
                     FdoCommonStringUtil::StringCompareNoCase(name, FdoWmsXmlGlobals::g_WmsTimeDimension) == 0 ||
                     FdoCommonStringUtil::StringCompareNoCase(name, FdoWmsXmlGlobals::g_WmsElevationDimension) == 0 ||
                     FdoCommonStringUtil::StringCompareNoCase(name, FdoWmsXmlGlobals::g_WmsSpatialContext) == 0 ||
                     FdoCommonStringUtil::StringCompareNoCase(name, FdoWmsXmlGlobals::g_WmsUseTileCache) == 0) 
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
            _SetFormatAbbr(m_pXmlContentHandler->GetString());
        }
        else if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::g_WmsImageFormatType) == 0) {
            _SetFullFormatType(m_pXmlContentHandler->GetString());
        }
        else if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::g_WmsTransparent) == 0) {
            _SetTransparent(m_pXmlContentHandler->GetString());
        }
        else if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::g_WmsUseTileCache) == 0) {
            _SetUseTileCache(m_pXmlContentHandler->GetString());
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
    xmlWriter->WriteCharacters(_GetFormatAbbr());
    xmlWriter->WriteEndElement();

    xmlWriter->WriteStartElement(FdoWmsXmlGlobals::g_WmsImageFormatType);
    xmlWriter->WriteCharacters(GetImageFormat());
    xmlWriter->WriteEndElement();

    xmlWriter->WriteStartElement(FdoWmsXmlGlobals::g_WmsTransparent);
    xmlWriter->WriteCharacters(_GetTransparent());
    xmlWriter->WriteEndElement();

    xmlWriter->WriteStartElement(FdoWmsXmlGlobals::g_WmsUseTileCache);
    xmlWriter->WriteCharacters(_GetUseTileCache());
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

// This one is for WMS provider 3.3 tag: <FormatType>
// <FormatType>image/png;PhotometricInterpretation=PaletteColor</FormatType>
// or
// <FormatType>image/png</FormatType>
void FdoWmsOvRasterDefinition::_SetFullFormatType(FdoString *value)
{
    VALIDATE_ARGUMENT(value);

    bool bMatch = false;
    FdoStringP format(value);

    // Format contains extra parameter
    if (format.Contains(FdoWmsGlobals::RasterFormatSemicolon))
        format = format.Left(FdoWmsGlobals::RasterFormatSemicolon);

    for (int i = 0; FdoWmsXmlGlobals::g_MIMEImageFormats[i] != NULL; i++)
    {
        if (FdoCommonStringUtil::StringCompareNoCase((FdoString*)format, FdoWmsXmlGlobals::g_MIMEImageFormats[i]) == 0)
        {
            bMatch = true;
            m_formatDesc = value;
            break;
        }
    }

    if (!bMatch)
        throw FdoSchemaException::Create (NlsMsgGet (FDOWMS_INVALID_XMLSCHEMA_RASTERFORMATTYPE, "'%1$ls' is not a valid XML Raster Format Type.", value));

    // <FormatType> is from XML file
    m_hasFormatType = true;
}

// This one for the WMS 3.2.2 tag: 
// <Format>PNG</Format> only allows 4 types: PNG, TIF, GIF and JPG.
void FdoWmsOvRasterDefinition::_SetFormatAbbr(FdoString* value)
{
    VALIDATE_ARGUMENT(value);

    if (FdoCommonStringUtil::StringCompareNoCase((FdoString*)value, FdoWmsXmlGlobals::g_WmsImageFormatPng) == 0) {
        m_formatType = FdoWmsXmlGlobals::g_WmsImageFormatPng;    
    }
    else if (FdoCommonStringUtil::StringCompareNoCase((FdoString*)value, FdoWmsXmlGlobals::g_WmsImageFormatTif) == 0) {
        m_formatType = FdoWmsXmlGlobals::g_WmsImageFormatTif;    
    }
    else if (FdoCommonStringUtil::StringCompareNoCase((FdoString*)value, FdoWmsXmlGlobals::g_WmsImageFormatJpg) == 0) {
        m_formatType = FdoWmsXmlGlobals::g_WmsImageFormatJpg;    
    }
    else if (FdoCommonStringUtil::StringCompareNoCase((FdoString*)value, FdoWmsXmlGlobals::g_WmsImageFormatGif) == 0) {
        m_formatType = FdoWmsXmlGlobals::g_WmsImageFormatGif;    
    }
    else {
        throw FdoSchemaException::Create (NlsMsgGet (FDOWMS_INVALID_XMLSCHEMA_RASTERFORMATTYPE, "'%1$ls' is not a valid XML Raster Format Type.", value));
    }
    
    // <Format> is from XML file
    m_hasFormat = true;
}

FdoStringP FdoWmsOvRasterDefinition::_GetFormatAbbr(void) const
{       
    return m_formatType;
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

void FdoWmsOvRasterDefinition::_SetUseTileCache(FdoString* value)
{
    VALIDATE_ARGUMENT(value);

    FdoBoolean useTileCache = true;
    if (FdoCommonStringUtil::StringCompareNoCase(value, FdoWmsXmlGlobals::g_WmsTrue) == 0) {
        useTileCache = true;
    }
    else if (FdoCommonStringUtil::StringCompareNoCase(value, FdoWmsXmlGlobals::g_WmsFalse) == 0) {
        useTileCache = false;
    }
    else {
        throw FdoSchemaException::Create (NlsMsgGet (FDOWMS_INVALID_XMLSCHEMA_USETILECACHE_VALUE, "'%1$ls' is not a valid XML use tile cache value.", value));
    }

    SetUseTileCache(useTileCache);
}

FdoStringP FdoWmsOvRasterDefinition::_GetUseTileCache(void) const
{
    FdoStringP strUseTileCache;
    strUseTileCache = (GetUseTileCache() == true ? L"true" : L"false");
    return strUseTileCache;
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

// Convert from WMS 3.3 format type to WMS 3.2.2 value
FdoStringP FdoWmsOvRasterDefinition::MIMEFormatTypeToAbbr(FdoString* fullFormat)
{
    VALIDATE_ARGUMENT(fullFormat);

    // Ignore the extra parameter
    FdoStringP format(fullFormat);
    if (format.Contains(FdoWmsGlobals::RasterFormatSemicolon))
        format = format.Left(FdoWmsGlobals::RasterFormatSemicolon);

    FdoStringP ret;
    if (FdoCommonStringUtil::StringCompareNoCase((FdoString*)format, FdoWmsGlobals::RasterMIMEFormat_PNG) == 0) {
        ret = FdoWmsXmlGlobals::g_WmsImageFormatPng;    // convert from "image/png" to "PNG"
    }
    else if (FdoCommonStringUtil::StringCompareNoCase((FdoString*)format, FdoWmsGlobals::RasterMIMEFormat_TIFF) == 0) {
        ret = FdoWmsXmlGlobals::g_WmsImageFormatTif;    // convert from "image/tiff" to "TIF"
    }
    else if (FdoCommonStringUtil::StringCompareNoCase((FdoString*)format, FdoWmsGlobals::RasterMIMEFormat_JPEG) == 0) {
        ret = FdoWmsXmlGlobals::g_WmsImageFormatJpg;    // convert from "image/jpeg" to "JPG"
    }
    else if (FdoCommonStringUtil::StringCompareNoCase((FdoString*)format, FdoWmsGlobals::RasterMIMEFormat_GIF) == 0) {
        ret = FdoWmsXmlGlobals::g_WmsImageFormatGif;    // convert from "image/gif" to "GIF"
    }
    else {
        throw FdoSchemaException::Create (NlsMsgGet (FDOWMS_INVALID_XMLSCHEMA_RASTERFORMATTYPE, "'%1$ls' is not a valid XML Raster Format Type.", fullFormat));
    }

    return ret;
}

// Convert from WMS 3.3 format type to WMS 3.2.2 value
FdoStringP FdoWmsOvRasterDefinition::AbbrToMIMEFormatType(FdoString* abbrFormat)
{
    VALIDATE_ARGUMENT(abbrFormat);

    FdoStringP ret;
    if (FdoCommonStringUtil::StringCompareNoCase((FdoString*)abbrFormat, FdoWmsXmlGlobals::g_WmsImageFormatPng) == 0) {
        ret = FdoWmsGlobals::RasterMIMEFormat_PNG;    // convert from "PNG" to "image/png"
    }
    else if (FdoCommonStringUtil::StringCompareNoCase((FdoString*)abbrFormat, FdoWmsXmlGlobals::g_WmsImageFormatTif) == 0) {
        ret = FdoWmsGlobals::RasterMIMEFormat_TIFF;    // convert from "TIF" to "image/tiff"
    }
    else if (FdoCommonStringUtil::StringCompareNoCase((FdoString*)abbrFormat,FdoWmsXmlGlobals::g_WmsImageFormatJpg) == 0) {
        ret = FdoWmsGlobals::RasterMIMEFormat_JPEG;    // convert from "JPG" to "image/jpeg"
    }
    else if (FdoCommonStringUtil::StringCompareNoCase((FdoString*)abbrFormat,FdoWmsXmlGlobals::g_WmsImageFormatGif) == 0) {
        ret = FdoWmsGlobals::RasterMIMEFormat_GIF;    // convert from "GIF" to "image/gif"
    }
    else {
        throw FdoSchemaException::Create (NlsMsgGet (FDOWMS_INVALID_XMLSCHEMA_RASTERFORMATTYPE, "'%1$ls' is not a valid XML Raster Format Type.", abbrFormat));
    }

    return ret;
}
