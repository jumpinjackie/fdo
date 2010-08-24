/*
 * Copyright (C) 2004-2010  Autodesk, Inc.
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
#include <OWS/FdoOwsOperationsMetadata.h>
#include <OWS/FdoOwsGlobals.h>


void FdoOwsXLink::InitFromXml(
    FdoXmlSaxContext* context, 
    FdoXmlAttributeCollection* attrs)
{
    VALIDATE_ARGUMENT(attrs);
    VALIDATE_ARGUMENT(context);

    try 
    {
		FdoXmlAttributeP att = attrs->FindItem(FdoOwsGlobals::type);
        if (att != NULL) {
            m_type = att->GetValue();
        }

        att = attrs->FindItem(FdoOwsGlobals::href);
        if (att != NULL) {
            m_href = att->GetValue();
        }
    }
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        FDO_SAFE_RELEASE(ex);
    }

    return;
}

FdoXmlSaxHandler* FdoOwsXLink::XmlStartElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname, 
    FdoXmlAttributeCollection* atts)
{
    return BaseType::XmlStartElement(context, uri, name, qname, atts);
}

FdoBoolean FdoOwsXLink::XmlEndElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname)
{
    return BaseType::XmlEndElement(context, uri, name, qname);
}

void FdoOwsHttp::InitFromXml(
    FdoXmlSaxContext* context, 
    FdoXmlAttributeCollection* attrs)
{
    return;
}

FdoXmlSaxHandler* FdoOwsHttp::XmlStartElement(
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
            if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Get) == 0) {
                m_httpGetXlink = FdoOwsXLink::Create();
                m_httpGetXlink->InitFromXml(context, atts);
                pRet = m_httpGetXlink;
            }
			else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Post) == 0)
			{
                m_httpPostXlink = FdoOwsXLink::Create();
                m_httpPostXlink->InitFromXml(context, atts);
                pRet = m_httpPostXlink;
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

FdoBoolean FdoOwsHttp::XmlEndElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname)
{
    return BaseType::XmlEndElement(context, uri, name, qname);
}



void FdoOwsDcp::InitFromXml(
    FdoXmlSaxContext* context, 
    FdoXmlAttributeCollection* attrs)
{
    return;
}

FdoXmlSaxHandler* FdoOwsDcp::XmlStartElement(
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
            if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::HTTP) == 0) {
                m_http = FdoOwsHttp::Create();
                m_http->InitFromXml(context, atts);
                pRet = m_http;
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

FdoBoolean FdoOwsDcp::XmlEndElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname)
{
    return BaseType::XmlEndElement(context, uri, name, qname);
}




void FdoOwsParameter::InitFromXml(
    FdoXmlSaxContext* context, 
    FdoXmlAttributeCollection* attrs)
{
    VALIDATE_ARGUMENT(attrs);
    VALIDATE_ARGUMENT(context);

    try 
    {
        FdoXmlAttributeP att = attrs->FindItem(FdoOwsGlobals::name);
        if (att != NULL) 
            m_name = att->GetValue();
    }
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }

    return;
}

FdoXmlSaxHandler* FdoOwsParameter::XmlStartElement(
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
            if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Value) == 0) {
                m_xmlContentHandler = FdoXmlCharDataHandler::Create();
                pRet = m_xmlContentHandler;
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

FdoBoolean FdoOwsParameter::XmlEndElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname)
{
    VALIDATE_ARGUMENT(name);
    VALIDATE_ARGUMENT(context);

    try 
    {
        if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Value) == 0)
            m_values->Add(m_xmlContentHandler->GetString());

        FDO_SAFE_RELEASE(m_xmlContentHandler.p);
    }
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }

    return BaseType::XmlEndElement(context, uri, name, qname);
}




void FdoOwsOperation::InitFromXml(
    FdoXmlSaxContext* context, 
    FdoXmlAttributeCollection* attrs)
{
    VALIDATE_ARGUMENT(attrs);
    VALIDATE_ARGUMENT(context);

    try 
    {
        FdoXmlAttributeP att = attrs->FindItem(FdoOwsGlobals::name);
        if (att != NULL) 
            m_name = att->GetValue();
    }
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }

    return;
}

FdoXmlSaxHandler* FdoOwsOperation::XmlStartElement(
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
            if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Parameter) == 0) {
                FdoOwsParameterP parameter = FdoOwsParameter::Create();
                parameter->InitFromXml(context, atts);
                m_parameters->Add(parameter);
                pRet = parameter;
            }
			else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::DCP) == 0) {
                m_dcp = FdoOwsDcp::Create();
                m_dcp->InitFromXml(context, atts);
                pRet = m_dcp;
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

FdoBoolean FdoOwsOperation::XmlEndElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname)
{
    return BaseType::XmlEndElement(context, uri, name, qname);
}




void FdoOwsOperationsMetadata::InitFromXml(
    FdoXmlSaxContext* context, 
    FdoXmlAttributeCollection* attrs)
{
    return;
}

FdoXmlSaxHandler* FdoOwsOperationsMetadata::XmlStartElement(
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
			if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::Operation) == 0) {
                FdoOwsOperationP operation = FdoOwsOperation::Create();
                operation->InitFromXml(context, atts);
                m_operations->Add(operation);
                pRet = operation;
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

FdoBoolean FdoOwsOperationsMetadata::XmlEndElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname)
{
    return BaseType::XmlEndElement(context, uri, name, qname);
}


