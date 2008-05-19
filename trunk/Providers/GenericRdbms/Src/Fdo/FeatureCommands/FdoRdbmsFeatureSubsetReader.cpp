/*
 * 
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
#include "FdoRdbmsSchemaUtil.h"

#include "FdoCommon.h"

#include "FdoRdbmsFeatureSubsetReader.h"
#include "FdoRdbmsDescribeSchemaCommand.h"
#include "FdoRdbmsUtil.h"


FdoRdbmsFeatureSubsetReader::FdoRdbmsFeatureSubsetReader( FdoIConnection *connection, GdbiQueryResult *queryResult, bool isFeatureQuery, const FdoSmLpClassDefinition *classDef, FdoFeatureSchemaCollection *schmCol, FdoIdentifierCollection *properties, FdoRdbmsSecondarySpatialFilterCollection * secondarySpatialFilters, vector<int> *logicalOps):
FdoRdbmsFeatureReader(connection, queryResult, isFeatureQuery, classDef, schmCol, properties, 0, secondarySpatialFilters, logicalOps)
{
}

FdoRdbmsFeatureSubsetReader::~FdoRdbmsFeatureSubsetReader()
{

}

bool FdoRdbmsFeatureSubsetReader::ReadNext( )
{
    if( mQueryResult == NULL )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_19, "Query ended"));

    if (mGeometryCache)
    {
        mGeometryCache->Release();
        mGeometryCache = NULL;
    }

    mHasMoreFeatures = false;
    if ( ! ReadNextWithLocalFilter() )
    {
        mQueryResult->Close();
        delete mQueryResult;
        mQueryResult = NULL;
        return false;
    }

    // A new property row is available. This get overwritten below when we are handling a feature query.
    // For feature query, we need to execute a secondary query against the class table to get the properties
    mPropertiesFetched = true;

    //
    // Use the feature query id to fetch the class properties. Mark the cache as if the query is already cached.
    mAttrsQidIdx = 0;
    mAttrQueryCache[mAttrsQidIdx].query = mQueryResult;


    return (mHasMoreFeatures = true);
}



