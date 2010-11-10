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
#include "FdoWfsDelegate.h"
#include "FdoWfsServiceMetadata.h"
#include <OWS/FdoOwsGetCapabilities.h>
#include <OWS/FdoOwsResponse.h>
#include "FdoWfsDescribeFeatureType.h"
#include "FdoWfsSchemaMerger.h"
#include "FdoWfsGetFeature.h"
#include <Fdo/Xml/FeatureReader.h>
#include <Fdo/Xml/FeatureFlags.h>


FdoWfsDelegate::FdoWfsDelegate(FdoString* defaultUrl, FdoString* userName, FdoString* passwd) :
     FdoOwsDelegate(defaultUrl, userName, passwd)
{
}

FdoWfsDelegate::~FdoWfsDelegate()
{
}

FdoWfsDelegate* FdoWfsDelegate::Create(FdoString* defaultUrl, FdoString* userName, FdoString* passwd)
{
    return new FdoWfsDelegate(defaultUrl, userName, passwd);
}

FdoWfsServiceMetadata* FdoWfsDelegate::GetCapabilities(FdoString* version)
{
    FdoPtr<FdoOwsGetCapabilities> request = FdoOwsGetCapabilities::Create(FdoWfsGlobals::WFS);
    if (version == NULL || wcslen(version) == 0)
        request->SetVersion(FdoWfsGlobals::WfsVersion);
    else
        request->SetVersion(version, false); // version already in URL.

    FdoPtr<FdoOwsResponse> response = Invoke(request);
    FdoPtr<FdoIoStream> stream = response->GetStream();
    FdoPtr<FdoWfsServiceMetadata> rv = FdoWfsServiceMetadata::Create();
    rv->ReadXml(stream);

    return FDO_SAFE_ADDREF(rv.p);
}

FdoFeatureSchemaCollection* FdoWfsDelegate::DescribeFeatureType(FdoStringCollection* typeNames, FdoString* version)
{
    FdoPtr<FdoWfsDescribeFeatureType> request = FdoWfsDescribeFeatureType::Create(typeNames,version);

    FdoPtr<FdoOwsResponse> response = Invoke(request);
    FdoPtr<FdoIoStream> stream = response->GetStream();

    // now we should merge stream and all its referenced schema
	FdoWfsSchemaMerger merger(version);
    stream = merger.MergeSchema(stream, GetUrl(), L"");
    FdoPtr<FdoFeatureSchemaCollection> schemas = FdoFeatureSchemaCollection::Create(NULL);
    FdoPtr<FdoXmlFlags> flags = FdoXmlFlags::Create(FdoWfsGlobals::fdo_customer, FdoXmlFlags::ErrorLevel_VeryLow);
    flags->SetSchemaNameAsPrefix(true);
    flags->SetElementDefaultNullability(true);
    schemas->ReadXml(stream, flags);

    return FDO_SAFE_ADDREF(schemas.p);
}

FdoIFeatureReader* FdoWfsDelegate::GetFeature(FdoFeatureSchemaCollection* schemas, 
                                              FdoPhysicalSchemaMappingCollection* schemaMappings, 
                                              FdoString* targetNamespace,
                                              FdoString* srsName,
                                              FdoStringCollection* propertiesToSelect,
                                              FdoString* from,
                                              FdoFilter* where,
                                              FdoString* schemaName,
                                              FdoString* version,
                                              FdoWfsCancelExecutationHander handler,
                                              void* handleData)
{
    FdoPtr<FdoWfsGetFeature> request = FdoWfsGetFeature::Create(targetNamespace, 
                                                                srsName, 
                                                                propertiesToSelect, 
                                                                from, 
                                                                where,
                                                                schemaName,
																version);
    FdoPtr<FdoOwsResponse> response;
    FdoException* exc1 = NULL;
    try
    {
        response = Invoke(request);
    }
    catch(FdoException* exc) // some servers request to have the class name in the front of properties, so we will try to place them
    {
        exc1 = exc;
        request->EncodeWithClassName(true);
        try
        {
            response = Invoke(request);
        }
        catch(FdoException* exc2) // rare cases
        {
            exc2->Release();
            request->SetSchemaName(L""); // remove schema name
            request->EncodeWithClassName(false);
            try
            {
                response = Invoke(request);
            }
            catch(FdoException* exc3)
            {
                exc3->Release();
                request->EncodeWithClassName(true);
                try
                {
                    response = Invoke(request);
                }
                catch(FdoException* exc4)
                {
                    exc4->Release();
                    throw exc1;
                }
            }
        }
        exc1->Release();
    }
    FdoPtr<FdoIoStream> stream = response->GetStream();
	FdoPtr<FdoIoStream> tempStream = preProcessStream(stream, handler, handleData);
    FdoPtr<FdoXmlReader> xmlReader = FdoXmlReader::Create(tempStream);
	FdoPtr<FdoXmlFeatureFlags> flags = FdoXmlFeatureFlags::Create(FdoWfsGlobals::fdo_customer, FdoXmlFlags::ErrorLevel_VeryLow);
	flags->SetSchemaMappings(schemaMappings);
    FdoPtr<FdoXmlFeatureReader> xmlFeatureReader = FdoXmlFeatureReader::Create(xmlReader, flags);
    xmlFeatureReader->SetFeatureSchemas(schemas);
	FdoPtr<FdoWfsFeatureReader> featureReader = new FdoWfsFeatureReader;	
	featureReader->SetXmlFeatureReader(FDO_SAFE_ADDREF(xmlFeatureReader.p));

    return FDO_SAFE_ADDREF(featureReader.p);
}

FdoIoStream* FdoWfsDelegate::preProcessStream(FdoIoStream *stream, FdoWfsCancelExecutationHander handler, void* handleData)
{
	FdoIoFileStreamP tempStream= FdoIoFileStream::Create( L"temp_stream.xml", L"w+" );
	
	FdoByte buffer[4096];
	bool bCanceled = false;
	const int readSize = sizeof(buffer)/sizeof(FdoByte);
	do
	{
		FdoSize cntRead = stream->Read(buffer,readSize);
		if (cntRead == 0)
			break;
		for (FdoSize i =0;i<cntRead;i++)
		{
			int num = (int)buffer[i];
			if ((num >= 1 && num <= 7) || 
				(num >= 11 && num <=19))
				buffer[i] = ' ';
		}
		tempStream->Write(buffer,cntRead);
	}
	while (!(bCanceled = handler(handleData)));

	if (bCanceled)
		tempStream->SetLength(0);

	tempStream->Reset();
	return FDO_SAFE_ADDREF(tempStream.p);

}


