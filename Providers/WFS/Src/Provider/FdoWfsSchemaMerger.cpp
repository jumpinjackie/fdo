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
#include "FdoWfsSchemaMerger.h"
#include "FdoWfsSchemaRefHandler.h"
#include <OWS/FdoOwsIHttpHandler.h>
#include <FdoCommonStringUtil.h>
#include "FdoWfsSchemaCopyHandler.h"
#include "FdoWfsNamedIoStream.h"
#include "feature.h"
#include "geometry.h"
#include "xlinks.h"
#include "Gml311/feature3.h"
#include "Gml311/basicTypes3.h"
#include "Gml311/coordinateOperations3.h"
#include "Gml311/coordinateReferenceSystems3.h"
#include "Gml311/coordinateSystems3.h"
#include "Gml311/dataQuality3.h"
#include "Gml311/dictionary3.h"
#include "Gml311/direction3.h"
#include "Gml311/geometryAggregates3.h"
#include "Gml311/geometryBasic0d1d3.h"
#include "Gml311/geometryBasic2d3.h"
#include "Gml311/geometryComplexes3.h"
#include "Gml311/geometryPrimitives3.h"
#include "Gml311/gml3.h"
#include "Gml311/gmlBase3.h"
#include "Gml311/grids3.h"
#include "Gml311/measures3.h"
#include "Gml311/referenceSystems3.h"
#include "Gml311/temporal3.h"
#include "Gml311/temporalReferenceSystems3.h"
#include "Gml311/units3.h"
#include "Gml311/valueObjects3.h"

FdoWfsSchemaMerger::FdoWfsSchemaMerger()
{
    m_FeatureLoaded = false;
    m_GeometryLoaded = false;
    m_XlinksLoaded = false;
    m_mergedList = NULL;
    m_loadedStreams = NULL;
    m_copier = NULL;
}

FdoWfsSchemaMerger::FdoWfsSchemaMerger(FdoString* version)
{
    m_FeatureLoaded = false;
    m_GeometryLoaded = false;
    m_XlinksLoaded = false;
    m_mergedList = NULL;
    m_loadedStreams = NULL;
    m_copier = NULL;
	m_version = version;
	
	// add the GML3 basic schemas
	if (wcscmp(m_version,FdoWfsGlobals::WfsVersion110) == 0)
	{
		m_Gml311[L"feature.xsd"] = (char**)feature3;
		m_Gml311[L"basicTypes.xsd"] = (char**)basicTypes3;
		m_Gml311[L"coordinateOperations.xsd"] = (char**)coordinateOperations3;
		m_Gml311[L"coordinateReferenceSystems.xsd"] = (char**)coordinateReferenceSystems3;
		m_Gml311[L"coordinateSystems.xsd"] = (char**)coordinateSystems3;
		m_Gml311[L"dataQuality.xsd"] = (char**)dataQuality3;
		m_Gml311[L"dictionary.xsd"] = (char**)dictionary3;
		m_Gml311[L"direction.xsd"] = (char**)direction3;
		m_Gml311[L"geometryAggregates.xsd"] = (char**)geometryAggregates3;
		m_Gml311[L"geometryBasic0d1d.xsd"] = (char**)geometryBasic0d1d3;
		m_Gml311[L"geometryBasic2d.xsd"] = (char**)geometryBasic2d3;
		m_Gml311[L"geometryComplexes.xsd"] = (char**)geometryComplexes3;
		m_Gml311[L"geometryPrimitives.xsd"] = (char**)geometryPrimitives3;
		m_Gml311[L"gml.xsd"] = (char**)gml3;
		m_Gml311[L"gmlBase.xsd"] = (char**)gmlBase3;
		m_Gml311[L"grids.xsd"] = (char**)grids3;
		m_Gml311[L"measures.xsd"] = (char**)measures3;
		m_Gml311[L"referenceSystems.xsd"] = (char**)referenceSystems3;
		m_Gml311[L"temporal.xsd"] = (char**)temporal3;
		m_Gml311[L"temporalReferenceSystems.xsd"] = (char**)temporalReferenceSystems3;
		m_Gml311[L"units.xsd"] = (char**)units3;
		m_Gml311[L"valueObjects.xsd"] = (char**)valueObjects3;
	}
}


FdoWfsSchemaMerger::~FdoWfsSchemaMerger()
{
	if (!m_Gml311.empty())
		m_Gml311.clear();
}

FdoIoStream* FdoWfsSchemaMerger::MergeSchema(FdoIoStream* schema, FdoString* schemaLocation, FdoString* uri)
{
    m_FeatureLoaded = false;
    m_GeometryLoaded = false;
    m_XlinksLoaded = false;
    m_mergedList = FdoDictionary::Create();
    m_loadedStreams = FdoWfsNamedIoStreamCollection::Create();
    m_mergedSchema = FdoIoMemoryStream::Create();
    FdoXmlWriterP xmlWriter = FdoXmlWriter::Create( m_mergedSchema );
    m_copier = FdoWfsSchemaCopyHandler::Create(xmlWriter);
    xmlWriter = NULL;


    _mergeSchema(schema, schemaLocation, uri);

    m_mergedList = NULL;
    m_loadedStreams = NULL;
    m_copier = NULL;
    m_mergedSchema->Reset();

    return FDO_SAFE_ADDREF(m_mergedSchema.p);
}


void FdoWfsSchemaMerger::_mergeSchema(FdoIoStream* schema, FdoString* schemaLocation, FdoString* uri)
{
    // first we must check whether it is already merged
    // similar as "#ifndef _XXX_H"
    FdoPtr<FdoDictionaryElement> item = m_mergedList->FindItem(schemaLocation);
    if (item != NULL)
        return;

    // if not merged, mark itself as merged
    // similar as "#define _XXX_H"
    item = FdoDictionaryElement::Create( schemaLocation, uri );
    m_mergedList->Add(item);
    
    // get all refs
    FdoXmlReaderP srcReader = FdoXmlReader::Create(schema);
    
    FdoPtr<FdoWfsSchemaRefHandler> refHandler = FdoWfsSchemaRefHandler::Create();

    srcReader->Parse(refHandler.p);
    FdoPtr<FdoDictionary> refs = refHandler->GetRefs();
    FdoPtr<FdoStringCollection> refFullLocations = FdoStringCollection::Create();
    FdoPtr<FdoStringCollection> refUris = FdoStringCollection::Create();

    // try to retrieve all the refs
    FdoInt32 count = refs->GetCount();
    int i;
    for (i = 0; i < count; i++)
    {
        FdoPtr<FdoDictionaryElement> ref = refs->GetItem(i);
        FdoString* location = ref->GetName();
        FdoStringP fullLocation = _getFullLocation(location, schemaLocation);
        // check whether it is merged
        FdoPtr<FdoDictionaryElement> item = m_mergedList->FindItem((FdoString*)fullLocation);
        if (item == NULL)
        {
            // check whether it is already loaded
            FdoPtr<FdoWfsNamedIoStream> loadedStream = m_loadedStreams->FindItem((FdoString*)fullLocation);
            if (loadedStream == NULL)
            {
                FdoPtr<FdoIoStream> stream;
                char** xmlSchema = NULL;
				// if it is a well known GML schema, then do not get it from outside
				if (wcscmp(m_version,FdoWfsGlobals::WfsVersion) == 0) //1.0.0 version, use GML 2.1.2
				{
					if (fullLocation.Contains(L"feature.xsd"))
					{
						if (m_FeatureLoaded)
							continue;
						m_FeatureLoaded = true;
						xmlSchema = (char**)feature;
					}
					else if (fullLocation.Contains(L"geometry.xsd"))
					{
						if (m_GeometryLoaded)
							continue;
						m_GeometryLoaded = true;
						xmlSchema = (char**)geometry;
					}
				}
				else if (wcscmp(m_version,FdoWfsGlobals::WfsVersion110) == 0) //1.1.0 version, use GML 3.1.1
				{
					// don't need to merge the unneeded parts
					if (fullLocation.Contains(L"dynamicFeature.xsd")|| fullLocation.Contains(L"defaultStyle.xsd") || fullLocation.Contains(L"datums.xsd") ||
						fullLocation.Contains(L"smil20-language.xsd") ||  fullLocation.Contains(L"xml-mod.xsd") || fullLocation.Contains(L"smil20.xsd") ||
						fullLocation.Contains(L"temporalTopology.xsd") || fullLocation.Contains(L"topology.xsd") ||
						fullLocation.Contains(L"coverage.xsd") || fullLocation.Contains(L"observation.xsd") )
						continue;
					else // found in the m_Gml313 map
					{
						//get the xsd name
						FdoStringP name = this->_getXSDName(fullLocation);
						std::map<FdoStringP,char** >::iterator iter = m_Gml311.find(name);
						if(iter != m_Gml311.end())
							xmlSchema = iter->second;
					}
				}
                
				if (fullLocation.Contains(L"xlinks.xsd")) //xlinks schema is same between GML 2 and GML 3
                {
                    if (m_XlinksLoaded)
                        continue;
                    m_XlinksLoaded = true;
                    xmlSchema = (char**)xlinks;
                }
                refFullLocations->Add(fullLocation);
                refUris->Add(ref->GetValue());
                if (xmlSchema != NULL)
                {
                    stream = FdoIoMemoryStream::Create();
                    int i = 0;
                    while (xmlSchema[i] != NULL)
                    {
                        stream->Write((FdoByte*)xmlSchema[i], strlen(xmlSchema[i]));
                        i++;
                    }
                    stream->Reset();
                }
                else
                {
                    char* mbUrl;
                    wide_to_multibyte(mbUrl, (FdoString*)fullLocation);
                    FdoPtr<FdoOwsIHttpHandler> httpHandler = FdoOwsIHttpHandler::Create(mbUrl, true, "", "", "");
                    httpHandler->Perform();
                    stream = static_cast<FdoIoStream*>(FDO_SAFE_ADDREF(httpHandler.p));
                }
                FdoPtr<FdoWfsNamedIoStream> item = FdoWfsNamedIoStream::Create(fullLocation, stream);
                m_loadedStreams->Add(item);
            }
        }
    }
    count = refFullLocations->GetCount();
    // recursively merge all the unmerged refs first
    for (i = 0; i < count; i++)
    {
        FdoString* fullLocation = refFullLocations->GetString(i);
        FdoPtr<FdoWfsNamedIoStream> namedStream = m_loadedStreams->GetItem(fullLocation);
        FdoPtr<FdoIoStream> stream = namedStream->GetStream();
        FdoString* uri = refUris->GetString(i);
        // call recursively
        _mergeSchema(stream, fullLocation, uri);
    }

    // finally merge itself
    schema->Reset();
    srcReader = FdoXmlReader::Create(schema);
    srcReader->Parse(m_copier);

}

FdoStringP FdoWfsSchemaMerger::_getXSDName(FdoStringP location)
{
	FdoStringP rv;

	size_t len = location.GetLength();
	size_t i;
	for (i = len - 1;i>=0;i--)
	{
         if (*((FdoString*)location + i) == FdoWfsGlobals::SLASH)
              break;
	}

	if ( i == -1)
		rv = L"";
	else
		rv = location.Mid(i + 1,len - i);
	return rv;

}

FdoStringP FdoWfsSchemaMerger::_getFullLocation(FdoString* relativeLocation, FdoString* baseLocation)
{
    FdoStringP rv = relativeLocation;
    if (!rv.Contains(FdoWfsGlobals::HTTPProtocal))
    {
        rv = baseLocation;
        // find the last "/"
        size_t len = rv.GetLength();
        size_t i;
        for (i = len - 1; i >= 0; i--)
        {
            if (*((FdoString*)rv + i) == FdoWfsGlobals::SLASH)
                break;
        }
        if (i == -1) // not found
            rv = relativeLocation;
        else
            rv = rv.Mid(0, i + 1) + relativeLocation;
    }

    return rv;
}






