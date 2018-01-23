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
#include "FdoWmsServiceMetadata.h"
#include "FdoWmsXmlGlobals.h"
#include <OWS/FdoOwsGlobals.h>

FdoWmsServiceMetadata::FdoWmsServiceMetadata(void)
{
    m_capabilities = FdoWmsCapabilities::Create();
}

FdoWmsServiceMetadata::~FdoWmsServiceMetadata(void)
{
}

void FdoWmsServiceMetadata::Dispose() 
{
    delete this;
}

FdoWmsServiceMetadata* FdoWmsServiceMetadata::Create()
{
    return new FdoWmsServiceMetadata();
}

FdoXmlSaxHandler* FdoWmsServiceMetadata::XmlStartElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname, FdoXmlAttributeCollection* atts)
{
    FdoXmlSaxHandler* pRet = NULL;
    try 
	{
        if (context == NULL || name == NULL)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_61_NULL_ARGUMENT)));
		
        FdoOwsXmlSaxContext* myContext = static_cast<FdoOwsXmlSaxContext*>(context);
        if (myContext->StateServiceMetadata() == 0) 
        {
            if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::WMS_Capabilities) != 0 && FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::WMT_MS_Capabilities) != 0)
            {
                if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::WFS_Capabilities) == 0)
                    throw FdoException::Create(NlsMsgGet(FDOWMS_INVALID_ACCESSED_SERVER, "The HTTP request attempted to connect to a WFS server when expected a WMS server."));
                else
                    throw FdoException::Create(NlsMsgGet(FDOWMS_UNKNOWN_ACCESSED_SERVER, "The HTTP request attempted to connect to a non WMS server."));
            }
        }
        pRet = BaseType::XmlStartElement(context, uri, name, qname, atts);
	}
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
        
        try{context->ThrowErrors();}
        catch (FdoException* newEx)
        {
            FdoException* exc = FdoException::Create(NlsMsgGet(FDOWMS_UNACCEPTABLE_RESPONSE, "The HTTP request generated an unacceptable response by the server."));
            exc->SetCause(newEx);
            newEx->Release();
            throw exc;
        }
    }
    return pRet;
}

FdoBoolean FdoWmsServiceMetadata::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    FdoBoolean ret = false;
    try 
	{
        ret = BaseType::XmlEndElement(context, uri, name, qname);
	}
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
        
        try{context->ThrowErrors();}
        catch (FdoException* newEx)
        {
            FdoException* exc = FdoException::Create(NlsMsgGet(FDOWMS_UNACCEPTABLE_RESPONSE, "The HTTP request generated an unacceptable response by the server."));
            exc->SetCause(newEx);
            newEx->Release();
            throw exc;
        }
    }
    return ret;
}

FdoOwsCapabilities* FdoWmsServiceMetadata::OnCreateCapabilities()
{
    return FDO_SAFE_ADDREF(m_capabilities.p);
}

FdoOwsCapabilities* FdoWmsServiceMetadata::GetCapabilities() const
{
    return FDO_SAFE_ADDREF(m_capabilities.p);
}
