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


#ifndef FDOOWSGETCAPABILITIES_H
#define FDOOWSGETCAPABILITIES_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <OWS/FdoOwsRequest.h>

class FdoOwsGetCapabilities : public FdoOwsRequest
{
private:
    FdoPtr<FdoStringCollection> m_acceptVersions;
    FdoPtr<FdoStringCollection> m_sections;
    FdoStringP                  m_updateSequence;
    FdoPtr<FdoStringCollection> m_acceptFormats;

protected:
    FdoOwsGetCapabilities();
    FdoOwsGetCapabilities(FdoString* service);
    virtual ~FdoOwsGetCapabilities();
    virtual void Dispose() { delete this; }

public:
    FDOOWS_API static FdoOwsGetCapabilities* Create(FdoString* service);

    FDOOWS_API void AddAcceptVersion(FdoString* version);
    FDOOWS_API void AddSection(FdoString* section);
    FDOOWS_API void AddAcceptFormat(FdoString* acceptFormat);
    FDOOWS_API void SetUpdateSequence(FdoString* updateSequence);

    /// IEncodable implementation
    FDOOWS_API virtual FdoStringP EncodeKVP();
    FDOOWS_API virtual FdoStringP EncodeXml();
};

typedef FdoPtr<FdoOwsGetCapabilities> FdoOwsGetCapabilitiesP;

#endif


