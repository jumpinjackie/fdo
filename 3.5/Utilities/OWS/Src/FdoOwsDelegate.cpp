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
#include <FdoCommonStringUtil.h>
#include <OWS/FdoOwsGlobals.h>
#include <OWS/FdoOwsResponse.h>
#include <OWS/FdoOwsRequest.h>
#include <OWS/FdoOwsRequestMetadata.h>
#include <OWS/FdoOwsDelegate.h>
#include "FdoOwsUrlResolver.h"
#include "FdoOwsHttpHandler.h"
#include "FdoOwsExceptionReport.h"

FdoOwsDelegate::FdoOwsDelegate()
{
}

FdoOwsDelegate::FdoOwsDelegate(FdoString* defaultUrl, FdoString* userName, FdoString* passwd) : m_defaultUrl(defaultUrl),
                                    m_userName(userName), m_passwd(passwd)
{
}

FdoOwsDelegate::~FdoOwsDelegate()
{
}

void FdoOwsDelegate::SetRequestMetadatas(FdoOwsRequestMetadataCollection* requestMetadatas)
{
    m_requestMetadatas = FDO_SAFE_ADDREF(requestMetadatas);
}

FdoOwsResponse* FdoOwsDelegate::Invoke(FdoOwsRequest* request)
{
    bool bGet = true;
    
    // We wont modify the URL from the GetCapabilities response directly
    FdoStringP url = m_defaultUrl;

    // If possible, try and resolve the URL address
    if (m_requestMetadatas != NULL)
    {
        if (m_urlResolver == NULL)
            m_urlResolver = FdoOwsUrlResolver::Create(m_requestMetadatas);
        FdoStringP rv = m_urlResolver->GetUrl(bGet, request->GetRequest ());
        if (rv != NULL)
            url = rv;
    }

    // Only URL encode the parameters for non-GetCapability responses
    if (NULL == dynamic_cast<FdoOwsGetCapabilities*>(request))
    {
        // Extract the URL and additional parameters
        FdoStringP parameters = url.Right(L"?");

        // URL encode the additiional parameters
        if (parameters != L"")
        {
            // Encoded parameters
            FdoStringP encodedParams;

            // Extract the url base string
            FdoStringP urlbase = url.Left(L"?");

            // Extract all the additional parameter components into their respective string values
            FdoStringsP parameterItems = FdoStringCollection::Create(parameters, FdoOwsGlobals::And);
            for (FdoInt32 i=0; i<parameterItems->GetCount(); i++)
            {
                // Extract eact item
                FdoStringElementP parameterItem = parameterItems->GetItem(i);

                // Extract the parameter name/value
                FdoStringP parameterString = parameterItem->GetString();
                FdoStringP parameterName = parameterString.Left(FdoOwsGlobals::Equal);
                FdoStringP parameterValue = parameterString.Right(FdoOwsGlobals::Equal);

                // URL encode the additiional parameters
                if (parameterValue != L"" && 
                    FdoCommonStringUtil::StringCompareNoCase(parameterName, FdoOwsGlobals::version) != 0 &&
                    FdoCommonStringUtil::StringCompareNoCase(parameterName, FdoOwsGlobals::SRS) != 0 &&
                    FdoCommonStringUtil::StringCompareNoCase(parameterName, FdoOwsGlobals::CRS) != 0)
                {
                    // Get curl to escape the additional params
                    char* temp = curl_escape(parameterValue, 0);
                    if (temp != NULL)
                    {
                        parameterValue = temp;
                        curl_free(temp);
                    }

                    // Rebuild the parameter item
                    encodedParams += FdoStringP::Format(L"%ls%ls%ls", (FdoString*)parameterName, FdoOwsGlobals::Equal, (FdoString*)parameterValue);
                }
                else
                {
                    // Rebuild the parameter item
                    encodedParams += parameterString;
                }

                // If we have more parameters to process, add the delimeter
                if (i != (parameterItems->GetCount()-1))
                    encodedParams += FdoOwsGlobals::And;
            }

            // Rebuild the URL
            url = FdoStringP::Format(L"%ls?%ls", (FdoString*)urlbase, (FdoString*)encodedParams);
        }
    }

    // encode request
    FdoStringP requestString;
    if (bGet)
        requestString = request->EncodeKVP();
    else
        requestString = request->EncodeXml();

    // create http handler, which is also a IO stream
    // Addresses may be input as UTF-8, wide characters depending of local of the sender!
    const char* mbUrl = url;
    const char* mbRequestString = requestString;
    const char* mbUserName = m_userName;
    const char* mbPasswd = m_passwd;

    FdoPtr<FdoOwsHttpHandler> httpHandler = FdoOwsHttpHandler::Create(mbUrl, bGet, mbRequestString, mbUserName, mbPasswd);
        
    // Here we use 2 mins as the default value for "connection" timeout.
    httpHandler->SetConnectionTimeout (60 * 2);

    // The Perform call won't return util beginning receiving http content.
    // If there are any connection related problems, exceptions
    // thrown out here
    httpHandler->Perform(); 

    // Check whether there is an exception reported from server
    FdoOwsMIMEType mimeType = httpHandler->GetMIMEType();
    FdoIoStream* stream = static_cast<FdoIoStream*>(httpHandler.p);
    if (mimeType == FdoOwsMIMEType_text_xml || mimeType == FdoOwsMIMEType_unknown) // there might be exception, we must try
    {
        bool bException = false;
        FdoPtr<FdoOwsExceptionReport> exceptionReport = FdoOwsExceptionReport::Create();
        try {
            exceptionReport->ReadXml(stream);
            bException = true; // no exception means this is an exception report
        } catch (FdoException* e) {
            FDO_SAFE_RELEASE(e);
            // this is not exception report so we must reset the stream
            // so that further parsing could be performed properly
            stream->Reset();
        }
        if (bException)
            throw FdoException::Create(exceptionReport->GetExceptionMessage());
    }

    // return Ows response
    return FdoOwsResponse::Create(mimeType, stream); 
}






