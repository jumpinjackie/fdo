/******************************************************************************
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
  *
 ******************************************************************************/


#ifndef FdoRdbmsOdbcSpatialSecondaryFilterLocal_h
#define FdoRdbmsOdbcSpatialSecondaryFilterLocal_h

#ifdef _WIN32
#pragma once
#endif

#include "../Src/SchemaMgr/SchemaManager.h"
#include "../SchemaMgr/Lp/GeometricPropertyDefinition.h"
#include <SpatialManager/FdoRdbmsSpatialSecondaryFilter.h>


/// <summary>This class implements local secondary spatial filtering.
/// </summary>
class FdoRdbmsOdbcSpatialSecondaryFilterLocal : public FdoRdbmsSpatialSecondaryFilter
{
public:

    static FdoRdbmsOdbcSpatialSecondaryFilterLocal * Create(
        const FdoGrdSchemaManager * schemaManager,
        const FdoSmLpGeometricPropertyDefinition * geometricProperty,
        const FdoGeometricCondition * geometricCondition);

    virtual bool MeetsCondition(
        FdoIGeometry * geometry);

    virtual FdoString *  GetPropertyName();

    virtual FdoSpatialOperations GetOperation();

protected:

    FdoRdbmsOdbcSpatialSecondaryFilterLocal          ();
    FdoRdbmsOdbcSpatialSecondaryFilterLocal(
        const FdoGrdSchemaManager * schemaManager,
        const FdoSmLpGeometricPropertyDefinition * geometricProperty,
        const FdoGeometricCondition * geometricCondition);
    virtual ~FdoRdbmsOdbcSpatialSecondaryFilterLocal ();

    FdoPtr<FdoIGeometry>    mGeometryOfCondition;
    FdoSpatialOperations    mOperation;
    FdoStringP              mGeometryToTestPropertyName;

};  //  class FdoRdbmsSpatialSecondaryFilterLocal

#endif
