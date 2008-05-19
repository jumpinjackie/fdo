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
#include <OWS/FdoOwsXmlSaxContext.h>
#include <OWS/FdoOwsXmlSaxContext.h>
#include <OWS/FdoOwsGlobals.h>
#include "FdoOwsExceptionReport.h"

FdoOwsExceptionReport::FdoOwsExceptionReport()
{
}

FdoOwsExceptionReport::~FdoOwsExceptionReport()
{
}

FdoOwsExceptionReport* FdoOwsExceptionReport::Create()
{
    return new FdoOwsExceptionReport();
}

FdoString* FdoOwsExceptionReport::GetExceptionMessage()
{
    return m_exceptionMessage;
}

FdoXmlSaxHandler* FdoOwsExceptionReport::XmlStartElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname, 
    FdoXmlAttributeCollection* atts
)
{
    FdoXmlSaxHandler* pRet = NULL;
    if (context == NULL || name == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_61_NULL_ARGUMENT), "A required argument was set to NULL."));
	
    FdoOwsXmlSaxContext* myContext = static_cast<FdoOwsXmlSaxContext*>(context);
    switch (myContext->StateExceptionReport())
    {
    // we are now in document root, possible subelements include only <ServiceExceptionReport/>
    case 0:
        {
            if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::ServiceExceptionReport) == 0)
                myContext->SetStateExceptionReport(1);
            else
                throw FdoException::Create(FdoException::NLSGetMessage(
                                        FDO_NLSID(FDO_52_BADSUBELEMENT), 
                                        "Error reading from XML, unexpected element %1$ls inside '%2$ls'.", 
                                        name, FdoOwsGlobals::ServiceExceptionReport));
            break;
        }
    // we are now in nested level 1, possible subelements include only <ServiceException/>
    case 1:
        {
            if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::ServiceException) == 0)
            {
                FdoPtr<FdoXmlAttribute> attr = atts->FindItem(FdoOwsGlobals::code);
                if (attr != NULL)
                    m_exceptionMessage = attr->GetValue();
                FdoPtr<FdoXmlCharDataHandler> handler = FdoXmlCharDataHandler::Create();
                myContext->SetExceptionReportCharDataHandler(handler);
                pRet = handler.p;
            }
            else
                throw FdoException::Create(FdoException::NLSGetMessage(
                                        FDO_NLSID(FDO_52_BADSUBELEMENT), 
                                        "Error reading from XML, unexpected element %1$ls inside '%2$ls'.", 
                                        name, FdoOwsGlobals::ServiceExceptionReport));
            break;

        }
    default:
        throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_57_UNEXPECTEDERROR), "Unexpected error encountered."));

    }

    return pRet;
}

FdoBoolean FdoOwsExceptionReport::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
    FdoBoolean ret = false;
    if (context == NULL || name == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_61_NULL_ARGUMENT), "A required argument was set to NULL."));
	
    FdoOwsXmlSaxContext* myContext = static_cast<FdoOwsXmlSaxContext*>(context);
    switch (myContext->StateExceptionReport())
    {
    case 0:
        {
            break;
        }
    case 1:
        {
            if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::ServiceException) == 0)
            {
                FdoPtr<FdoXmlCharDataHandler> handler = myContext->ExceptionReportCharDataHandler();
                if (handler != NULL) // exception message provided in the content
                {
                    FdoStringP exStr = handler->GetString();
                    if (exStr.GetLength() != 0) {
                        if (m_exceptionMessage.GetLength() != 0) 
                            m_exceptionMessage += L": ";
                        m_exceptionMessage += exStr;
                    }   
                    myContext->SetExceptionReportCharDataHandler(NULL);
                }
            }
            else if (FdoCommonOSUtil::wcsicmp(name, FdoOwsGlobals::ServiceExceptionReport) == 0)
                myContext->SetStateExceptionReport(0);
            break;
        }
    default:
        throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_57_UNEXPECTEDERROR), "Unexpected error encountered."));
    }

    return ret;
}

void FdoOwsExceptionReport::XmlCharacters(FdoXmlSaxContext*, FdoString*)
{
}

FdoXmlSaxContext* FdoOwsExceptionReport::GetSaxContext()
{
    return new FdoOwsXmlSaxContext(FdoPtr<FdoXmlReader>(GetXmlReader()));
}





