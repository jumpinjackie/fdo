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
#include <OWS/FdoOwsServiceIdentification.h>
#include <OWS/FdoOwsXmlSaxContext.h>
#include <OWS/FdoOwsGlobals.h>

FdoOwsServiceIdentification::FdoOwsServiceIdentification()
{
    m_keywords = FdoStringCollection::Create();
}

FdoOwsServiceIdentification::~FdoOwsServiceIdentification()
{
}

FdoOwsServiceIdentification* FdoOwsServiceIdentification::Create()
{
    return new FdoOwsServiceIdentification();
}

FdoString* FdoOwsServiceIdentification::GetName() const
{
    return (FdoString*)m_name;
}
FdoString* FdoOwsServiceIdentification::GetTitle() const
{
    return (FdoString*)m_title;
}
FdoString* FdoOwsServiceIdentification::GetAbstract() const
{
    return (FdoString*)m_abstract;
}
FdoStringCollection* FdoOwsServiceIdentification::GetKeywords() const
{
    return FDO_SAFE_ADDREF(m_keywords.p);
}
FdoString* FdoOwsServiceIdentification::GetUrl() const
{
    return (FdoString*)m_url;
}
FdoString* FdoOwsServiceIdentification::GetFees() const
{
    return (FdoString*)m_fees;
}
FdoString* FdoOwsServiceIdentification::GetAccessConstrains() const
{
    return (FdoString*)m_accessConstrains;
}
FdoString* FdoOwsServiceIdentification::GetServiceType() const
{
    return (FdoString*)m_serviceType;
}
FdoString* FdoOwsServiceIdentification::GetServiceTypeVersion() const
{
    return (FdoString*)m_serviceTypeVersion;
}

void FdoOwsServiceIdentification::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
}

FdoXmlSaxHandler* FdoOwsServiceIdentification::XmlStartElement(
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
        if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Name) == 0 ||
            FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Title) == 0 ||
            FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::OnlineResource) == 0 ||
			FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Abstract) == 0 ||
			FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::AcessConstraints) == 0 ||
		    FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::ServiceType) == 0 ||
            FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::ServiceTypeVersion) == 0 ||
            FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Fees) == 0||
			FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Keyword) == 0)
        {
            // find out whether the url exists in href attribute
            FdoPtr<FdoXmlAttribute> href;
            if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::OnlineResource) == 0 && 
                (href = atts->FindItem(FdoOwsGlobals::href)) != NULL)
                m_url = href->GetValue();
            else
            {
                FdoPtr<FdoXmlCharDataHandler> handler = FdoXmlCharDataHandler::Create();
                myContext->SetServiceIdCharDataHandler(handler);
                pRet = handler.p;
            }
        }
        else
            pRet = BaseType::XmlStartElement(context, uri, name, qname, atts);
	}
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }

    return pRet;
}

FdoBoolean FdoOwsServiceIdentification::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    FdoBoolean ret = false;
	try 
	{
        if (context == NULL || name == NULL)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_61_NULL_ARGUMENT), "A required argument was set to NULL."));
        FdoOwsXmlSaxContext* myContext = static_cast<FdoOwsXmlSaxContext*>(context);

        if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Name) == 0 ||
            FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Title) == 0 ||
            FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::OnlineResource) == 0 ||
			FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Abstract) == 0 ||
			FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::AcessConstraints) == 0 ||
		    FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::ServiceType) == 0 ||
            FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::ServiceTypeVersion) == 0 ||
            FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Fees) == 0 ||
			FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Keyword) == 0)
        {
            FdoPtr<FdoXmlCharDataHandler> handler = myContext->ServiceIdCharDataHandler();
            if (handler != NULL)
            {
			    FdoStringP value = handler->GetString();
                if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Name) == 0)
                    m_name = value;
                else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Title) == 0)
                    m_title = value;
                else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::OnlineResource) == 0)
                    m_url = value;
                else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Abstract) == 0)
                    m_abstract = value;
                else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::AcessConstraints) == 0)
                    m_accessConstrains = value;
				else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::ServiceType) == 0)
                    m_serviceType = value;
				else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::ServiceTypeVersion) == 0)
                    m_serviceTypeVersion = value;
				else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Fees) == 0)
                    m_fees = value;
				else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Keyword) == 0)
                    m_keywords->Add(value);

                myContext->SetServiceIdCharDataHandler(NULL);

            }
        }
        else
            ret = BaseType::XmlEndElement(context, uri, name, qname);
	}
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }

    return ret;

}







