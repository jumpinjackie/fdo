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

FdoWfsServiceMetadata* FdoWfsDelegate::GetCapabilities()
{
    FdoPtr<FdoOwsGetCapabilities> request = FdoOwsGetCapabilities::Create(FdoWfsGlobals::WFS);
    request->SetVersion(FdoWfsGlobals::WfsVersion);

    FdoPtr<FdoOwsResponse> response = Invoke(request);
    FdoPtr<FdoIoStream> stream = response->GetStream();
    FdoPtr<FdoWfsServiceMetadata> rv = FdoWfsServiceMetadata::Create();
    rv->ReadXml(stream);

    return FDO_SAFE_ADDREF(rv.p);
}

FdoFeatureSchemaCollection* FdoWfsDelegate::DescribeFeatureType(FdoStringCollection* typeNames)
{
    FdoPtr<FdoWfsDescribeFeatureType> request = FdoWfsDescribeFeatureType::Create(typeNames);
    request->SetVersion(FdoWfsGlobals::WfsVersion);

    FdoPtr<FdoOwsResponse> response = Invoke(request);
    FdoPtr<FdoIoStream> stream = response->GetStream();

    // now we should merge stream and all its referenced schema
    FdoWfsSchemaMerger merger;
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
                                              FdoFilter* where)
{
    FdoPtr<FdoWfsGetFeature> request = FdoWfsGetFeature::Create(targetNamespace, 
                                                                srsName, 
                                                                propertiesToSelect, 
                                                                from, 
                                                                where);
    FdoPtr<FdoOwsResponse> response;
    try
    {
        response = Invoke(request);
    }
    catch(FdoException* exc) // some servers request to have the class name in the front of properties, so we will try to place them
    {
        exc->Release();
        request->EncodeWithClassName(true);
        response = Invoke(request); // if second time we will get an exception then is something wrong.
    }
    FdoPtr<FdoIoStream> stream = response->GetStream();

    FdoPtr<FdoXmlReader> xmlReader = FdoXmlReader::Create(stream);
	FdoPtr<FdoXmlFeatureFlags> flags = FdoXmlFeatureFlags::Create(FdoWfsGlobals::fdo_customer, FdoXmlFlags::ErrorLevel_VeryLow);
	flags->SetSchemaMappings(schemaMappings);
    FdoPtr<FdoXmlFeatureReader> xmlFeatureReader = FdoXmlFeatureReader::Create(xmlReader, flags);
    xmlFeatureReader->SetFeatureSchemas(schemas);
	FdoPtr<FdoWfsFeatureReader> featureReader = new FdoWfsFeatureReader;	
	featureReader->SetXmlFeatureReader(FDO_SAFE_ADDREF(xmlFeatureReader.p));

    return FDO_SAFE_ADDREF(featureReader.p);
}

