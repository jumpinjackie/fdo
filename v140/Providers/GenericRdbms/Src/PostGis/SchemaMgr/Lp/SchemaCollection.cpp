//
// Copyright (C) 2006 Refractions Research, Inc.
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#include "stdafx.h"
#include "SchemaCollection.h"

FdoSmLpPostGisSchemaCollection::FdoSmLpPostGisSchemaCollection(FdoSmPhMgrP mgr,
    FdoSmLpSpatialContextMgrP scMgr)
        : FdoSmLpSchemaCollection(mgr, scMgr)
{
    // idle
}

FdoSmLpPostGisSchemaCollection::~FdoSmLpPostGisSchemaCollection()
{
    // idle
}

FdoSmLpSchemaP FdoSmLpPostGisSchemaCollection::NewSchema(
    FdoSmPhSchemaReaderP reader)
{
    FdoSmLpPostGisSchema* pgSchema = NULL;
    pgSchema = new FdoSmLpPostGisSchema(reader,
        GetPhysicalSchema(), this);

    return pgSchema;
}

FdoSmLpSchemaP FdoSmLpPostGisSchemaCollection::NewSchema(
    FdoFeatureSchema* featSchema,
    bool ignoreStates)
{
    FdoSmLpPostGisSchema* pgSchema = NULL;
    pgSchema = new FdoSmLpPostGisSchema(featSchema, ignoreStates,
        GetPhysicalSchema(), this);

    return pgSchema;
}

bool FdoSmLpPostGisSchemaCollection::CanApplySchemaWithoutMetaSchema() const
{
    return true;
}

