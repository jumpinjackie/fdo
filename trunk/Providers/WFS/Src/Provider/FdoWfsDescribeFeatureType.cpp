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

#include "stdafx.h"
#include "FdoWfsDescribeFeatureType.h"
#include "FdoWfsGlobals.h"

FdoWfsDescribeFeatureType::FdoWfsDescribeFeatureType() :
    FdoOwsRequest(FdoWfsGlobals::WFS, FdoWfsGlobals::DescribeFeatureType)
{
    SetVersion(FdoWfsGlobals::WfsVersion);
}

FdoWfsDescribeFeatureType::FdoWfsDescribeFeatureType(FdoStringCollection* typeNames) :
    FdoOwsRequest(FdoWfsGlobals::WFS, FdoWfsGlobals::DescribeFeatureType),
        m_typeNames(typeNames)
{
    SetVersion(FdoWfsGlobals::WfsVersion);
    FDO_SAFE_ADDREF(m_typeNames.p);
}

FdoWfsDescribeFeatureType::~FdoWfsDescribeFeatureType()
{
}

FdoWfsDescribeFeatureType* FdoWfsDescribeFeatureType::Create(FdoStringCollection* typeNames)
{
    return new FdoWfsDescribeFeatureType(typeNames);
}

FdoStringP FdoWfsDescribeFeatureType::EncodeKVP()
{
    // for common request, version and service
    FdoStringP ret = FdoOwsRequest::EncodeKVP();

    FdoInt32 numTypes = 0;
    if (m_typeNames != NULL)
        numTypes = m_typeNames->GetCount();
    if (numTypes != 0)
    {
        ret += FdoWfsGlobals::And;
        ret += FdoWfsGlobals::TYPENAME;
        ret += FdoWfsGlobals::Equal;
        int it = 0;
        ret += UrlEscape(m_typeNames->GetString(it));
        for (it++; it < numTypes; it++)
        {
            ret += FdoWfsGlobals::Comma;
            ret += UrlEscape(m_typeNames->GetString(it));
        }
    }

    return ret;
}

FdoStringP FdoWfsDescribeFeatureType::EncodeXml()
{
    return L"";
}





