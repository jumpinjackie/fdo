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
#include <OWS/FdoOwsServiceMetadata.h>
#include <OWS/FdoOwsCapabilities.h>
#include <OWS/FdoOwsServiceIdentification.h>
#include <OWS/FdoOwsXmlSaxContext.h>
#include <OWS/FdoOwsGlobals.h>

FdoOwsServiceMetadata::FdoOwsServiceMetadata()
{
}

FdoOwsServiceMetadata::~FdoOwsServiceMetadata()
{
}

FdoOwsServiceMetadata* FdoOwsServiceMetadata::Create()
{
    return new FdoOwsServiceMetadata();
}

FdoOwsServiceIdentification* FdoOwsServiceMetadata::OnCreateServiceIdentification()
{
    return FdoOwsServiceIdentification::Create();
}

FdoOwsCapabilities* FdoOwsServiceMetadata::OnCreateCapabilities()
{
    return FdoOwsCapabilities::Create();
}


FdoString* FdoOwsServiceMetadata::GetVersion() const
{
    return (FdoString*)m_version;
}

FdoString* FdoOwsServiceMetadata::GetUpdateSequence() const
{
    return (FdoString*)m_updateSequence;
}

FdoOwsServiceIdentification* FdoOwsServiceMetadata::GetServiceIdentification() const
{
    return FDO_SAFE_ADDREF(m_serviceId.p);
}
FdoOwsCapabilities* FdoOwsServiceMetadata::GetCapabilities() const
{
    return FDO_SAFE_ADDREF(m_capabilities.p);
}

FdoXmlSaxHandler* FdoOwsServiceMetadata::XmlStartElement(
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
        switch (myContext->StateServiceMetadata())
        {
        // state 0 represents that we are now in document root
        // possible subelements include <WFS_Capabilities/>, <WMS_Capabilities/> and <WMT_MS_Capabilities/>
        case 0:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::WFS_Capabilities) == 0 ||
                    FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::WMS_Capabilities) == 0 ||
                    FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::WMT_MS_Capabilities) == 0)
                {
                    FdoPtr<FdoXmlAttribute> attr = atts->FindItem(FdoOwsGlobals::version);
                    if (attr != NULL)
                        m_version = attr->GetValue();
                    attr = atts->FindItem(FdoOwsGlobals::updateSequence);
                    if (attr != NULL)
                        m_updateSequence = attr->GetValue();

                    myContext->SetStateServiceMetadata(1);
                }
                else
                    throw FdoException::Create(FdoException::NLSGetMessage(
                                            FDO_NLSID(FDO_52_BADSUBELEMENT), 
                                            "Error reading from XML, unexpected element %1$ls inside '%2$ls'.", 
                                            name, FdoOwsGlobals::OWS_Capabilities));
                break;
            }
        // state 1 means that we are in nested level 1, possible subelements inlucde 
        // <Service/> and <Capability/>
        case 1:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Service) == 0)
                {
                    m_serviceId = OnCreateServiceIdentification();
                    m_serviceId->InitFromXml(context, atts);
                    pRet = m_serviceId.p;
                }
                else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Capability) == 0)
                {
                    m_capabilities = OnCreateCapabilities();
                    m_capabilities->InitFromXml(context, atts);
                    pRet = m_capabilities.p;
                }
                else
                    pRet = BaseType::XmlStartElement(context, uri, name, qname, atts);

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
FdoBoolean FdoOwsServiceMetadata::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    FdoBoolean ret = false;
    try 
	{
        if (context == NULL || name == NULL)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_61_NULL_ARGUMENT), "A required argument was set to NULL."));
		
        FdoOwsXmlSaxContext* myContext = static_cast<FdoOwsXmlSaxContext*>(context);
        switch (myContext->StateServiceMetadata())
        {
        case 0:
            {
                ret = BaseType::XmlEndElement(context, uri, name, qname);
                break;
            }
        case 1:
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::WFS_Capabilities) == 0 ||
                    FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::WMS_Capabilities) == 0  ||
                    FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::WMT_MS_Capabilities) == 0)
                    myContext->SetStateServiceMetadata(0);
                else
                    ret = BaseType::XmlEndElement(context, uri, name, qname);
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

FdoXmlSaxContext* FdoOwsServiceMetadata::GetSaxContext()
{
    return new FdoOwsXmlSaxContext(FdoPtr<FdoXmlReader>(GetXmlReader()));
}





