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
#include <OWS/FdoOwsRequestMetadata.h>
#include "FdoOwsUrlResolver.h"

FdoOwsUrlResolver::FdoOwsUrlResolver()
{
}

FdoOwsUrlResolver::FdoOwsUrlResolver(FdoOwsRequestMetadataCollection* requestMetadatas) :
                                m_requestMetadatas(requestMetadatas)
{
    FDO_SAFE_ADDREF(requestMetadatas);
}

FdoOwsUrlResolver::~FdoOwsUrlResolver()
{
}

FdoOwsUrlResolver* FdoOwsUrlResolver::Create(FdoOwsRequestMetadataCollection* requestMetadatas)
{
    return new FdoOwsUrlResolver(requestMetadatas);
}

FdoStringP FdoOwsUrlResolver::GetUrl(bool& bGet, FdoString* requestName)
{
    FdoStringP ret;
    int cnt = m_requestMetadatas->GetCount();
    for (int i=0; i<cnt; i++)
    {
        FdoPtr<FdoOwsRequestMetadata> metadata = m_requestMetadatas->GetItem(i);
        if (FdoCommonOSUtil::wcsicmp(metadata->GetName (), requestName) == 0)
        {
            FdoStringsP urls;
            if (bGet)
                urls = metadata->GetHttpGetUrls();
            else
                urls = metadata->GetHttpPostUrls();

            // If there are more than one URL, use the first one as default.
            if (urls->GetCount() > 0)
                ret = urls->GetString (0);
            break;
        }
    }

    return ret;
}
