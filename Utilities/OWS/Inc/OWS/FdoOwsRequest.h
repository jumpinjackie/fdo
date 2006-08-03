//
// Copyright (C) 2004-2006  Autodesk, Inc.
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

#ifndef FDOOWSREQUEST_H
#define FDOOWSREQUEST_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <OWS/FdoOwsIEncodable.h>


class FdoOwsRequest : public FdoOwsIEncodable
{
protected:
    FdoStringP m_service;
    FdoStringP m_request;
    FdoStringP m_version;

protected:
    FDOOWS_API FdoOwsRequest();
    FDOOWS_API FdoOwsRequest(FdoString* service, FdoString* request);
    FDOOWS_API virtual ~FdoOwsRequest();

public:
    FDOOWS_API void SetVersion(FdoString* version);
    FDOOWS_API FdoString* GetVersion();
    FDOOWS_API virtual FdoStringP EncodeKVP();
    FDOOWS_API void SetRequest (FdoString* request);
    FDOOWS_API FdoString* GetRequest ();
};

typedef FdoPtr<FdoOwsRequest> FdoOwsRequestP;


#endif


