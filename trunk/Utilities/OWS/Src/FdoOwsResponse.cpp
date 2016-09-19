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
#include <OWS/FdoOwsResponse.h>

FdoOwsResponse::FdoOwsResponse() :
    m_mimeType(FdoOwsMIMEType_unknown)
{
}

FdoOwsResponse::FdoOwsResponse(FdoOwsMIMEType mimeType, FdoIoStream* stream) :
                                m_mimeType(mimeType), m_stream(stream)
{
    FDO_SAFE_ADDREF(stream);
}

FdoOwsResponse::~FdoOwsResponse()
{
}

FdoOwsResponse* FdoOwsResponse::Create(FdoOwsMIMEType mimeType, FdoIoStream* stream)
{
    return new FdoOwsResponse(mimeType, stream);
}

FdoOwsMIMEType FdoOwsResponse::GetMIMEType()
{
    return m_mimeType;
}

FdoIoStream* FdoOwsResponse::GetStream()
{
    return FDO_SAFE_ADDREF(m_stream.p);
}








