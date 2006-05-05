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
#include <OWS/FdoOwsCapabilities.h>
#include <OWS/FdoOwsRequestMetadata.h>
#include <OWS/FdoOwsXmlSaxContext.h>
#include <OWS/FdoOwsGlobals.h>

FdoOwsCapabilities::FdoOwsCapabilities()
{
    m_requests = FdoOwsRequestMetadataCollection::Create();
}

FdoOwsCapabilities::~FdoOwsCapabilities()
{
}

FdoOwsCapabilities* FdoOwsCapabilities::Create()
{
    return new FdoOwsCapabilities();
}

FdoOwsRequestMetadata* FdoOwsCapabilities::OnCreateRequestMetadata(FdoString* name)
{
    return FdoOwsRequestMetadata::Create(name);
}

void FdoOwsCapabilities::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
}

FdoXmlSaxHandler* FdoOwsCapabilities::XmlStartElement(
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
        if (myContext->StateCapability() == 0)
        {
            if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Request) == 0)			
                myContext->SetStateCapability(1);
            else
                pRet = BaseType::XmlStartElement(context, uri, name, qname, atts);
        }
        else
        {
            FdoPtr<FdoOwsRequestMetadata> requestMetadata = OnCreateRequestMetadata(name);
            requestMetadata->InitFromXml(context, atts);
            m_requests->Add(requestMetadata);
            pRet = requestMetadata;

        }
	}
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }

    return pRet;
}

FdoBoolean FdoOwsCapabilities::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    FdoBoolean ret = false;
    try 
	{
        if (context == NULL || name == NULL)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_61_NULL_ARGUMENT), "A required argument was set to NULL."));
		
        FdoOwsXmlSaxContext* myContext = static_cast<FdoOwsXmlSaxContext*>(context);
        if (myContext->StateCapability() == 0)
        {
            ret = BaseType::XmlEndElement(context, uri, name, qname);
        }
        else
        {
            //if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Request) == 0)
			if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::GetCapabilities) == 0)
                myContext->SetStateCapability(0);
        }
	}
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }

    return ret;
}

FdoOwsRequestMetadataCollection* FdoOwsCapabilities::GetRequestMetadatas() const
{
    return FDO_SAFE_ADDREF(m_requests.p);
}








