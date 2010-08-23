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


#ifndef FDOOWSDELEGATE_H
#define FDOOWSDELEGATE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

class FdoOwsRequestMetadataCollection;
class FdoOwsOperationCollection;
class FdoOwsRequest;
class FdoOwsResponse;
class FdoOwsUrlResolver;

class FdoOwsDelegate : public FdoIDisposable
{
private:
    FdoPtr<FdoOwsRequestMetadataCollection> m_requestMetadatas;
	FdoPtr<FdoOwsOperationCollection> m_operationMetadatas;
    FdoStringP m_defaultUrl;
    FdoStringP m_userName;
    FdoStringP m_passwd;
    FdoPtr<FdoOwsUrlResolver> m_urlResolver;

protected:
    FDOOWS_API FdoOwsDelegate();
	FDOOWS_API FdoOwsDelegate(FdoString* defaultUrl, FdoString* userName, FdoString* passwd);	
    FDOOWS_API virtual ~FdoOwsDelegate();
	FDOOWS_API virtual void Dispose() { delete this; }

public:
    FDOOWS_API void SetRequestMetadatas(FdoOwsRequestMetadataCollection* requestMetadatas);
    FDOOWS_API void SetOperationMetadatas(FdoOwsOperationCollection* operationMetadatas);

    FDOOWS_API FdoString* GetUrl() const { return m_defaultUrl; }
    FDOOWS_API FdoString* GetUserName() const { return m_userName; }
    FDOOWS_API FdoString* GetPassword() const { return m_passwd; }

    FDOOWS_API void SetUrl(FdoString* url) { m_defaultUrl = url; }
    FDOOWS_API void SetUserName(FdoString* userName) { m_userName = userName; }
    FDOOWS_API void SetPassword(FdoString* passwd) { m_passwd = passwd; }

    FDOOWS_API FdoOwsResponse* Invoke(FdoOwsRequest* request);
};



#endif


