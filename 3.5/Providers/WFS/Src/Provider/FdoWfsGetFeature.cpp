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
#include "FdoWfsGetFeature.h"
#include <OWS/FdoOwsOgcFilterSerializer.h>
#include "FdoWfsGlobals.h"
// uncomment this line to get a limited number of records
// this line have been added for debug purposes
//#define DEBUG_LIMIT_FEATURES

FdoWfsGetFeature::FdoWfsGetFeature(FdoString* targetNamespace, FdoString* srsName, 
                                    FdoStringCollection* propertiesToSelect,
                                    FdoString* from,
                                    FdoFilter* where,
                                    FdoString* schemaName) : FdoOwsRequest(FdoWfsGlobals::WFS, FdoWfsGlobals::GetFeature),
                                    m_targetNamespace(targetNamespace), m_srsName(srsName),
                                    m_propertiesToSelect(propertiesToSelect),
                                    m_from(from), m_where(where), m_schemaName(schemaName)
{
    m_encodeWithClassName = false;
    SetVersion(FdoWfsGlobals::WfsVersion);
    FDO_SAFE_ADDREF(propertiesToSelect);
    FDO_SAFE_ADDREF(where);
}

FdoWfsGetFeature::~FdoWfsGetFeature()
{
}

FdoWfsGetFeature* FdoWfsGetFeature::Create(FdoString* targetNamespace, FdoString* srsName, FdoStringCollection* propertiesToSelect,
                            FdoString* from, FdoFilter* where, FdoString* schemaName)
{
    return new FdoWfsGetFeature(targetNamespace, srsName, propertiesToSelect, from, where, schemaName);
}

FdoStringP FdoWfsGetFeature::EncodeKVP()
{
    // for common request, version and service
    FdoStringP ret = FdoOwsRequest::EncodeKVP();
#ifdef DEBUG_LIMIT_FEATURES
    ret += FdoWfsGlobals::And;
    ret += L"maxFeatures";
    ret += FdoWfsGlobals::Equal;
    ret += L"50";
#endif
    // TYPENAME, mandatory
    ret += FdoWfsGlobals::And;
    ret += FdoWfsGlobals::TYPENAME;
    ret += FdoWfsGlobals::Equal;
    if (m_schemaName.GetLength() != 0)
        ret += UrlEscape(m_schemaName + L":" + m_from);
    else
        ret += UrlEscape(m_from);
    // PROPERTYNAME, optional
    FdoInt32 numProps = 0;
    if (m_propertiesToSelect != NULL)
        numProps = m_propertiesToSelect->GetCount();
    if (numProps > 0)
    {
        ret += FdoWfsGlobals::And;
        ret += FdoWfsGlobals::PROPERTYNAME;
        ret += FdoWfsGlobals::Equal;
        FdoStringP prop = UrlEscape(m_propertiesToSelect->GetString(0));
        if (m_encodeWithClassName && !prop.Contains(L"/"))
        {
            ret += UrlEscape(m_from); // can we skip escaping this?
            ret += L"/";
        }
        ret += prop;
        for (int i = 1; i < numProps; i++)
        {
            prop = UrlEscape(m_propertiesToSelect->GetString(i));
            ret += FdoWfsGlobals::Comma;
            if (m_encodeWithClassName && !prop.Contains(L"/"))
            {
                ret += UrlEscape(m_from); // can we skip escaping this?
                ret += L"/";
            }
            ret += prop;
        }
    }
    // FILTER, optional
    if (m_where != NULL)
    {
        FdoPtr<FdoIoMemoryStream> stream = FdoIoMemoryStream::Create();
        FdoPtr<FdoXmlWriter> writer = FdoXmlWriter::Create(stream, false);
        writer->WriteStartElement(FdoWfsGlobals::Filter);

        // make "ogc" default namespace
        writer->WriteAttribute(FdoXml::mXmlnsPref, FdoWfsGlobals::OgcUri);
        // gml namespace
        FdoStringP ns = FdoXml::mXmlnsPref;
        ns += L":";
        ns += FdoXml::mGmlNs;
        writer->WriteAttribute(ns, FdoXml::mGmlUri);
        // application namespace
        //ns = FdoXml::mXmlnsPref;
        //ns += L":";
        //ns += FdoWfsGlobals::appns;
        //writer->WriteAttribute(ns, m_targetNamespace);

        FdoOwsOgcFilterSerializer::Serialize(m_where, writer, m_srsName, NULL);
        
        writer = NULL;
        stream->Reset();
        FdoInt64 length = stream->GetLength();
        char* buffer = new char[(size_t)length + 1];
        stream->Read((FdoByte*)buffer, (FdoSize)length);
        buffer[length] = '\0';
		// we must get rid of the preceding  <?xml version="1.0" encoding="UTF-8" ?> 
		char* buffer1 = buffer;
		while (*buffer1 != '>') buffer1++;
		buffer1++;
        FdoStringP filter = buffer1; 
		delete[] buffer;

        ret += FdoWfsGlobals::And;
        ret += FdoWfsGlobals::Filter;
        ret += FdoWfsGlobals::Equal;
        ret += UrlEscape(filter);

    }

    return ret;

}

FdoStringP FdoWfsGetFeature::EncodeXml()
{
    return L"";
}







