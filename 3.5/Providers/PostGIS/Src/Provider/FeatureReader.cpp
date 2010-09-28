//
// Copyright (C) 2007 Refractions Research, Inc. 
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
// 22.Aug.2007 modified by Haris Kurtagic
//    1. Added mProps as member and changed constructor to accept input for it
//       It is used to set which properties are requested from Class - if empty reader is returning all properties
//    2. Changed FeatureReader::GetClassDefinition so it will return subset of properties of a Class (mClassDef).
//       Properties to return are set in a mProps; if mProps is empty all properties are return ( class is returned as It is)
//
#include "stdafx.h"

#include "PostGisProvider.h"
#include "FeatureReader.h"
#include "Connection.h"
#include "PgCursor.h"

#include <cassert>

namespace fdo { namespace postgis {

FeatureReader::FeatureReader(Connection* conn, PgCursor* cursor, FdoClassDefinition* classDef,FdoIdentifierCollection* PropDef)
    : Base(conn, cursor), mClassDef(classDef), mProps(PropDef)
{
    FDOLOG_MARKER("FeatureReader::FeatureReader");

    FDO_SAFE_ADDREF(mClassDef.p);
    
    FDO_SAFE_ADDREF(mProps.p);
}

FeatureReader::~FeatureReader()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void FeatureReader::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoIFeatureReader interface
///////////////////////////////////////////////////////////////////////////////

FdoClassDefinition* FeatureReader::GetClassDefinition()
{
    if (mProps && (mProps->GetCount() > 0))
    {
        FdoClassDefinition* newclass = 
            FdoCommonSchemaUtil::DeepCopyFdoClassDefinition(mClassDef);
        if (NULL != newclass)
        {
            FdoPtr<FdoPropertyDefinitionCollection> ids(newclass->GetProperties());
            long count = ids->GetCount();
            long ind = 0;

            while (ind < count)
            {
                FdoPtr<FdoPropertyDefinition> classprop(ids->GetItem(ind));
                bool found = false;

                for (long ind2 = 0; ind2 < mProps->GetCount(); ++ind2)
                {
                    FdoPtr<FdoIdentifier> prop2(mProps->GetItem(ind2));
                    if (0 == FdoCommonOSUtil::wcsicmp(classprop->GetName(), prop2->GetName()))
                    {
                        found = true;
                        break;
                    }
                }

                if (!found)
                {
                    ids->RemoveAt(ind);
                    count = ids->GetCount();
                }
                else
                {
                    ind++;
                }                          
            }
        }
        return newclass;
    }
    else
    {
        return FDO_SAFE_ADDREF(mClassDef.p);
    }
}

FdoInt32 FeatureReader::GetDepth()
{
    FDOLOG_MARKER("FeatureReader::GetDepth");
    
    // TODO: Verify this value.
    return 0;
}

const FdoByte* FeatureReader::GetGeometry(FdoString* propertyName, FdoInt32* count)
{
    return Base::GetGeometry(propertyName, count);
}

FdoByteArray* FeatureReader::GetGeometry(FdoString* propertyName)
{
    return Base::GetGeometry(propertyName);
}

FdoIFeatureReader* FeatureReader::GetFeatureObject(FdoString* propertyName)
{
    // TODO: An Association Property not supported yet by PostGIS provider.
    return NULL;
}

}} // namespace fdo::postgis

