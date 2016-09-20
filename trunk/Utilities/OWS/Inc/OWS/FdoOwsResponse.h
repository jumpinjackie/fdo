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

#ifndef FDOOWSRESPONSE_H
#define FDOOWSRESPONSE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <OWS/FdoOwsMIMEType.h>

class FdoOwsResponse : public FdoIDisposable
{
private:
    FdoOwsMIMEType      m_mimeType;
    FdoPtr<FdoIoStream> m_stream;

protected:
    FdoOwsResponse();
    FdoOwsResponse(FdoOwsMIMEType mimeType, FdoIoStream* stream);
    virtual ~FdoOwsResponse();
    virtual void Dispose() { delete this; }

public:
    FDOOWS_API static FdoOwsResponse* Create(FdoOwsMIMEType mimeType, FdoIoStream* stream);
    FDOOWS_API FdoOwsMIMEType GetMIMEType();
    FDOOWS_API FdoIoStream* GetStream();
};

typedef FdoPtr<FdoOwsResponse> FdoOwsResponseP;


#endif


