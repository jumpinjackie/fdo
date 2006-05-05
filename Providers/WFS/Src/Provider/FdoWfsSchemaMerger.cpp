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

FdoWfsSchemaMerger::FdoWfsSchemaMerger()
{
}

FdoWfsSchemaMerger::~FdoWfsSchemaMerger()
{
}

FdoIoStream* FdoWfsSchemaMerger::MergeSchema(FdoIoStream* schema, FdoString* schemaLocation, FdoString* uri)
{
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
            refFullLocations->Add(fullLocation);
            refUris->Add(ref->GetValue());
            // check whether it is already loaded
            FdoPtr<FdoWfsNamedIoStream> loadedStream = m_loadedStreams->FindItem((FdoString*)fullLocation);
            if (loadedStream == NULL)
            {
				FdoPtr<FdoIoStream> stream;
				char** xmlSchema = NULL;
				// if it is a well known GML schema, then do not get it from outside
				if (fullLocation.Contains(L"feature.xsd"))
					xmlSchema = (char**)feature;
				else if (fullLocation.Contains(L"geometry.xsd"))
					xmlSchema = (char**)geometry;
				else if (fullLocation.Contains(L"xlinks.xsd"))
					xmlSchema = (char**)xlinks;

				if (xmlSchema != NULL) {

					stream = FdoIoMemoryStream::Create();
					int i = 0;
                    while (xmlSchema[i] != NULL){
						stream->Write((FdoByte*)xmlSchema[i], strlen(xmlSchema[i]));
                        i++;
                    }
					stream->Reset();
				} else {
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






