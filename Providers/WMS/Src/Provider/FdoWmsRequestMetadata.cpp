//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include <stdafx.h>
#include <FdoWmsRequestMetadata.h>
#include "FdoWmsXmlGlobals.h"

FdoWmsRequestMetadata::FdoWmsRequestMetadata(FdoString* name) : FdoOwsRequestMetadata(name)
{
    mFormats = FdoStringCollection::Create();
}

FdoWmsRequestMetadata::~FdoWmsRequestMetadata()
{
}

FdoOwsRequestMetadata* FdoWmsRequestMetadata::Create(FdoString* name)
{
    return new FdoWmsRequestMetadata (name);
}

FdoXmlSaxHandler* FdoWmsRequestMetadata::XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts
    )
{
    FdoXmlSaxHandler* pRet = NULL;
    pRet = BaseType::XmlStartElement(context, uri, name, qname, atts);
    if (pRet == NULL)
    {
        if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesFormat) == 0)
        {
            mXmlContentHandler = FdoXmlCharDataHandler::Create();
            pRet = mXmlContentHandler;
        }
    }
    
    return pRet;
}

FdoBoolean FdoWmsRequestMetadata::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    FdoBoolean bRet = BaseType::XmlEndElement(context, uri, name, qname);

    if (FdoCommonOSUtil::wcsicmp(name, FdoWmsXmlGlobals::WmsCapabilitiesFormat) == 0)
    {
        mFormats->Add (mXmlContentHandler->GetString ());        
    }
    
    return bRet;
}

FdoStringCollection* FdoWmsRequestMetadata::GetFormats ()
{
    return FDO_SAFE_ADDREF (mFormats.p);
}
