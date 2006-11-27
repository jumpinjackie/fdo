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
#include <OWS/FdoOwsGetCapabilities.h>
#include <OWS/FdoOwsGlobals.h>


FdoOwsGetCapabilities::FdoOwsGetCapabilities()
{
}

FdoOwsGetCapabilities::FdoOwsGetCapabilities(FdoString* service) :
    FdoOwsRequest(service, FdoOwsGlobals::GetCapabilities)
{
    m_acceptVersions = FdoStringCollection::Create();
    m_sections = FdoStringCollection::Create();
    m_acceptFormats = FdoStringCollection::Create();
}

FdoOwsGetCapabilities::~FdoOwsGetCapabilities()
{
}

FdoOwsGetCapabilities* FdoOwsGetCapabilities::Create(FdoString* service)
{
    return new FdoOwsGetCapabilities(service);
}


void FdoOwsGetCapabilities::AddAcceptVersion(FdoString* version)
{
    m_acceptVersions->Add(version);
}

void FdoOwsGetCapabilities::AddSection(FdoString* section)
{
    m_sections->Add(section);
}

void FdoOwsGetCapabilities::AddAcceptFormat(FdoString* acceptFormat)
{
    m_acceptFormats->Add(acceptFormat);
}

void FdoOwsGetCapabilities::SetUpdateSequence(FdoString* updateSequence)
{
    m_updateSequence = updateSequence;
}

// IEncodable implementation
FdoStringP FdoOwsGetCapabilities::EncodeKVP()
{
    // For common request, version and service
    FdoStringP ret = FdoOwsRequest::EncodeKVP();

    // TODO: Add GetCapabilities specific encoding here...
    //
    //

    return ret;
}

FdoStringP FdoOwsGetCapabilities::EncodeXml()
{
    return L"";
}








