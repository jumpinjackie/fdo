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

#include <stdafx.h>
#include <FdoCommonOSUtil.h>
#include <OWS/FdoOwsRequestMetadata.h>
#include <OWS/FdoOwsXmlSaxContext.h>
#include <OWS/FdoOwsGlobals.h>

FdoOwsRequestMetadata::FdoOwsRequestMetadata(FdoString* name) : m_name(name)
{
    m_httpGetURLs = FdoStringCollection::Create();
    m_httpPostURLs = FdoStringCollection::Create();
}

FdoOwsRequestMetadata::~FdoOwsRequestMetadata()
{
}

FdoOwsRequestMetadata* FdoOwsRequestMetadata::Create(FdoString* name)
{
    return new FdoOwsRequestMetadata(name);
}

FdoString* FdoOwsRequestMetadata::GetName()
{
    return (FdoString*)m_name;
}

FdoBoolean FdoOwsRequestMetadata::CanSetName()
{
    return false;
}


FdoStringCollection* FdoOwsRequestMetadata::GetHttpGetUrls()
{
    return FDO_SAFE_ADDREF(m_httpGetURLs.p);
}

FdoStringCollection* FdoOwsRequestMetadata::GetHttpPostUrls()
{
    return FDO_SAFE_ADDREF(m_httpPostURLs.p);
}


void FdoOwsRequestMetadata::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
}

FdoXmlSaxHandler* FdoOwsRequestMetadata::XmlStartElement(
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
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_61_NULL_ARGUMENT), "A required argument was set to NULL."));
		
        FdoOwsXmlSaxContext* myContext = static_cast<FdoOwsXmlSaxContext*>(context);
        switch (myContext->StateRequestMetadata())
        {
        // we are now in nested level 0, possible subelements inlcude only <DCPType/>
        case 0:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::DCPType) == 0)
                    myContext->SetStateRequestMetadata(1);
                else
                    pRet = BaseType::XmlStartElement(context, uri, name, qname, atts);
                break;
            }
        // we are now in nested level 1, possible subelements inlcude only <HTTP/>
        case 1:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::HTTP) == 0)
                    myContext->SetStateRequestMetadata(2);
                else
                    throw FdoException::Create(FdoException::NLSGetMessage(
                                            FDO_NLSID(FDO_52_BADSUBELEMENT), 
                                            "Error reading from XML, unexpected element %1$ls inside '%2$ls'.", 
                                            name, FdoOwsGlobals::DCPType));

                break;
            }
        // we are now in nested level 2, possible subelements include <Get/>, <Post/> and <OnlineResource/>
        case 2:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Get) == 0)
                {
                    FdoPtr<FdoXmlAttribute> urlAttr = atts->FindItem(FdoOwsGlobals::onlineResource);
                    if (urlAttr != NULL) // the link exists in attribute
                        m_httpGetURLs->Add(urlAttr->GetValue());
                    else // the link exists in subelement
                        m_httpURLs = m_httpGetURLs;
                }
                else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Post) == 0)
                {
                    FdoPtr<FdoXmlAttribute> urlAttr = atts->FindItem(FdoOwsGlobals::onlineResource);
                    if (urlAttr != NULL) // the link exists in attribute
                        m_httpPostURLs->Add(urlAttr->GetValue());
                    else // the link exists in subelement
                        m_httpURLs = m_httpPostURLs;
                }
                else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::OnlineResource) == 0)
                {
                    if (m_httpURLs != NULL)
                    {
                        FdoPtr<FdoXmlAttribute> urlAttr = atts->FindItem(FdoOwsGlobals::href);
                        if (urlAttr != NULL)
                            m_httpURLs->Add(urlAttr->GetValue());
                        m_httpURLs = NULL;
                    }
                }
                else
                    throw FdoException::Create(FdoException::NLSGetMessage(
                                            FDO_NLSID(FDO_52_BADSUBELEMENT), 
                                            "Error reading from XML, unexpected element %1$ls inside '%2$ls'.", 
                                            name, FdoOwsGlobals::HTTP));
                break;
            }
        default:
            throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_57_UNEXPECTEDERROR), "Unexpected error encountered."));

        }

	}
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }

    return pRet;

}

FdoBoolean FdoOwsRequestMetadata::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    FdoBoolean ret = false;
    try 
	{
        if (context == NULL || name == NULL)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_61_NULL_ARGUMENT), "A required argument was set to NULL."));
		
        FdoOwsXmlSaxContext* myContext = static_cast<FdoOwsXmlSaxContext*>(context);
        switch (myContext->StateRequestMetadata())
        {
        case 0:
            {
                ret = BaseType::XmlEndElement(context, uri, name, qname);
                break;
            }
        case 1:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::DCPType) == 0)
                    myContext->SetStateRequestMetadata(0);
                break;
            }
        case 2:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::HTTP) == 0)
                    myContext->SetStateRequestMetadata(1);

                break;
            }
        default:
            throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_57_UNEXPECTEDERROR), "Unexpected error encountered."));
        }
	}
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }

    return ret;
}

FdoOwsRequestMetadataCollection::FdoOwsRequestMetadataCollection()
{
}

FdoOwsRequestMetadataCollection::~FdoOwsRequestMetadataCollection()
{
}

FdoOwsRequestMetadataCollection* FdoOwsRequestMetadataCollection::Create()
{
    return new FdoOwsRequestMetadataCollection();
}











