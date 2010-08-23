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

#ifndef FDOWFSDELEGATE_H
#define FDOWFSDELEGATE_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <OWS/FdoOwsDelegate.h>

class FdoWfsServiceMetadata;

class FdoWfsDelegate : public FdoOwsDelegate
{
protected:
	FdoWfsDelegate() {}
    FdoWfsDelegate(FdoString* defaultUrl, FdoString* userName, FdoString* passwd);
    virtual ~FdoWfsDelegate();
    virtual void Dispose() { delete this; }

public:
    static FdoWfsDelegate* Create(FdoString* defaultUrl, FdoString* userName, FdoString* passwd);

    FdoWfsServiceMetadata* GetCapabilities(FdoString* version);
    FdoFeatureSchemaCollection* DescribeFeatureType(FdoStringCollection* typeNames,FdoString* version);
    FdoIFeatureReader* GetFeature(FdoFeatureSchemaCollection* schemas, 
                                    FdoPhysicalSchemaMappingCollection* schemaMappings, 
                                    FdoString* targetNamespace, FdoString* srsName,
                                    FdoStringCollection* propertiesToSelect,
                                    FdoString* from,
                                    FdoFilter* where,
                                    FdoString* schemaName,
									FdoString* version);

};

typedef FdoPtr<FdoWfsDelegate> FdoWfsDelegateP;

#endif

