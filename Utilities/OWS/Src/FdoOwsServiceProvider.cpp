//
// Copyright (C) 2004-2010  Autodesk, Inc.
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
#include <OWS/FdoOwsServiceProvider.h>
#include <OWS/FdoOwsOperationsMetadata.h>
#include <OWS/FdoOwsGlobals.h>

void FdoOwsContactInfo::InitFromXml(
    FdoXmlSaxContext* context, 
    FdoXmlAttributeCollection* attrs)
{
    return;
}

FdoXmlSaxHandler* FdoOwsContactInfo::XmlStartElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname, 
    FdoXmlAttributeCollection* atts)
{
    VALIDATE_ARGUMENT(name);
    VALIDATE_ARGUMENT(context);

    FdoXmlSaxHandler* pRet = NULL;
    try 
    {
        pRet = BaseType::XmlStartElement(context, uri, name, qname, atts);
        if (pRet == NULL) {
            if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Voice) == 0 ||
                FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Facsimile) == 0 ||
                FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::DeliveryPoint) == 0 ||
                FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::City) == 0 ||
                FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::AdministrativeArea) == 0 ||
                FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::PostalCode) == 0 ||
                FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Country) == 0 ||
                FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::ElectronicMailAddress) == 0 ||
                FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::HoursOfService) == 0 ||
                FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::ContactInstructions) == 0) 
            {
                m_xmlContentHandler = FdoXmlCharDataHandler::Create();
                pRet = m_xmlContentHandler;
            }
            else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::OnlineResource) == 0) {
                m_onlineResource = FdoOwsXLink::Create();
                m_onlineResource->InitFromXml(context, atts);
                pRet = m_onlineResource;
            }
       }
    }
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        FDO_SAFE_RELEASE(ex);
    }

    return pRet;
}

FdoBoolean FdoOwsContactInfo::XmlEndElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname)
{
    VALIDATE_ARGUMENT(context);

    try 
    {
        if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Voice) == 0) {
            m_phoneNumber = m_xmlContentHandler->GetString();
        }
        else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Facsimile) == 0) {
            m_faxNumber = m_xmlContentHandler->GetString();
        }
        else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::DeliveryPoint) == 0) {
            m_deliveryPoint = m_xmlContentHandler->GetString();
        }
        else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::City) == 0) {
            m_city = m_xmlContentHandler->GetString();
        }
        else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::AdministrativeArea) == 0) {
            m_administrativeArea = m_xmlContentHandler->GetString();
        }
        else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::PostalCode) == 0) {
            m_postalCode = m_xmlContentHandler->GetString();
        }
        else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Country) == 0) {
            m_county = m_xmlContentHandler->GetString();
        }
        else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::ElectronicMailAddress) == 0) {
            m_electronicMailAddress = m_xmlContentHandler->GetString();
        }
        else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::HoursOfService) == 0) {
            m_hoursOfService = m_xmlContentHandler->GetString();
        }
        else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::ContactInstructions) == 0) {
            m_contactInstructions = m_xmlContentHandler->GetString();
        }

        FDO_SAFE_RELEASE(m_xmlContentHandler.p);
    }
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        FDO_SAFE_RELEASE(ex);
    }

    return BaseType::XmlEndElement(context, uri, name, qname);
}


void FdoOwsServiceContact::InitFromXml(
    FdoXmlSaxContext* context, 
    FdoXmlAttributeCollection* attrs)
{
    return;
}

FdoXmlSaxHandler* FdoOwsServiceContact::XmlStartElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname, 
    FdoXmlAttributeCollection* atts)
{
    VALIDATE_ARGUMENT(name);
    VALIDATE_ARGUMENT(context);

    FdoXmlSaxHandler* pRet = NULL;
    try 
    {
        pRet = BaseType::XmlStartElement(context, uri, name, qname, atts);
        if (pRet == NULL) {
            if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::IndividualName) == 0 ||
                FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::PositionName) == 0) 
            {
                m_xmlContentHandler = FdoXmlCharDataHandler::Create();
                pRet = m_xmlContentHandler;
            }
            else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::ContactInfo) == 0) {
                m_contactInfo = FdoOwsContactInfo::Create();
                m_contactInfo->InitFromXml(context, atts);
                pRet = m_contactInfo;
            }
        }
    }
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        FDO_SAFE_RELEASE(ex);
    }

    return pRet;
}

FdoBoolean FdoOwsServiceContact::XmlEndElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname)
{
    VALIDATE_ARGUMENT(context);

    try 
    {
        if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::IndividualName) == 0) {
            m_name = m_xmlContentHandler->GetString();
        }
        else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::PositionName) == 0) {
            m_position = m_xmlContentHandler->GetString();
        }

        FDO_SAFE_RELEASE(m_xmlContentHandler.p);
    }
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        FDO_SAFE_RELEASE(ex);
    }

    return BaseType::XmlEndElement(context, uri, name, qname);
}


void FdoOwsServiceProvider::InitFromXml(
    FdoXmlSaxContext* context, 
    FdoXmlAttributeCollection* attrs)
{
    return;
}

FdoXmlSaxHandler* FdoOwsServiceProvider::XmlStartElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname, 
    FdoXmlAttributeCollection* atts)
{
    VALIDATE_ARGUMENT(name);
    VALIDATE_ARGUMENT(context);

    FdoXmlSaxHandler* pRet = NULL;
    try 
    {
        pRet = BaseType::XmlStartElement(context, uri, name, qname, atts);
        if (pRet == NULL) {
            if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::ProviderName) == 0)  {
                m_xmlContentHandler = FdoXmlCharDataHandler::Create();
                pRet = m_xmlContentHandler;
            }
            else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::ServiceContact) == 0) {
                m_serviceContact = FdoOwsServiceContact::Create();
                m_serviceContact->InitFromXml(context, atts);
                pRet = m_serviceContact;
            }
            else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::ProviderSite) == 0) {
                m_site = FdoOwsXLink::Create();
                m_site->InitFromXml(context, atts);
                pRet = m_site;
            }
        }
    }
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        FDO_SAFE_RELEASE(ex);
    }

    return pRet;
}

FdoBoolean FdoOwsServiceProvider::XmlEndElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname)
{
    VALIDATE_ARGUMENT(context);

    try 
    {
        if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::ProviderName) == 0) {
            m_name = m_xmlContentHandler->GetString();
        }

        FDO_SAFE_RELEASE(m_xmlContentHandler.p);
    }
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        FDO_SAFE_RELEASE(ex);
    }

    return BaseType::XmlEndElement(context, uri, name, qname);
}