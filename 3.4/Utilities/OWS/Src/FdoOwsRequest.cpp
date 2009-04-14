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
#include <OWS/FdoOwsRequest.h>
#include <OWS/FdoOwsGlobals.h>
#include <curl/curl.h>

FdoOwsRequest::FdoOwsRequest()
{
}

FdoOwsRequest::FdoOwsRequest(FdoString* service, FdoString* request) :
                            m_service(service), m_request(request)
{
}

FdoOwsRequest::~FdoOwsRequest()
{
}

void FdoOwsRequest::SetVersion(FdoString* version)
{
    m_version = version;
}

FdoString* FdoOwsRequest::GetVersion ()
{
	return (FdoString*)m_version;
}

void FdoOwsRequest::SetRequest (FdoString* request)
{
    m_request = request;
}

FdoString* FdoOwsRequest::GetRequest ()
{
    return (FdoString*)m_request;
}

// IEncodable implementation
FdoStringP FdoOwsRequest::EncodeKVP()
{
    FdoStringP ret;
    if (m_version.GetLength() != 0)
    {
        ret += FdoOwsGlobals::version;
        ret += FdoOwsGlobals::Equal;
        ret += m_version;
        ret += FdoOwsGlobals::And;
    }
    ret += FdoOwsGlobals::service;
    ret += FdoOwsGlobals::Equal;
    ret += m_service;

    ret += FdoOwsGlobals::And;
    ret += FdoOwsGlobals::REQUEST;
    ret += FdoOwsGlobals::Equal;
    ret += m_request;

    return ret;
}

FdoStringP FdoOwsRequest::UrlEscape(FdoString * urlPart)
{
    FdoStringP result(urlPart);
    char * temp = curl_escape(result, 0);
    if (temp != NULL)
    {
        result = temp;
        curl_free(temp);
    }
    return result;
}

