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

#include "stdafx.h"
#include "FdoRdbmsOdbcSpatialSecondaryFilterLocal.h"
#include <FdoSpatial.h>     // For FdoSpatialUtility::Evaluate

FdoRdbmsOdbcSpatialSecondaryFilterLocal *
FdoRdbmsOdbcSpatialSecondaryFilterLocal::Create(
    const FdoGrdSchemaManager *schemaManager,
    const FdoSmLpGeometricPropertyDefinition *geometricProperty,
    const FdoGeometricCondition *geometricCondition)
{
    FdoRdbmsOdbcSpatialSecondaryFilterLocal * newFilter =
        new FdoRdbmsOdbcSpatialSecondaryFilterLocal(
        schemaManager,
        geometricProperty,
        geometricCondition);
    if (NULL == newFilter)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

    return newFilter;
}


bool
FdoRdbmsOdbcSpatialSecondaryFilterLocal::MeetsCondition(
    FdoIGeometry *geometry)
{
    bool meetsCondition=FdoSpatialUtility::Evaluate (geometry, mOperation, mGeometryOfCondition);

    return meetsCondition;
}


FdoString * FdoRdbmsOdbcSpatialSecondaryFilterLocal::GetPropertyName()
{
    return (FdoString *) mGeometryToTestPropertyName;
}

FdoSpatialOperations FdoRdbmsOdbcSpatialSecondaryFilterLocal::GetOperation()
{
    return mOperation;
}

FdoRdbmsOdbcSpatialSecondaryFilterLocal::FdoRdbmsOdbcSpatialSecondaryFilterLocal()
{
    // This will never be implemented (should be protected from external caller).
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
}


FdoRdbmsOdbcSpatialSecondaryFilterLocal::FdoRdbmsOdbcSpatialSecondaryFilterLocal(
    const FdoGrdSchemaManager *schemaManager,
    const FdoSmLpGeometricPropertyDefinition *geometricProperty,
    const FdoGeometricCondition *geometricCondition)
{
    // Note: needed to cast away constness.
    FdoPtr<FdoIdentifier> propertyNameId = ((FdoGeometricCondition *)geometricCondition)->GetPropertyName();
    mGeometryToTestPropertyName = propertyNameId->GetName();
    if( NULL != dynamic_cast<const FdoDistanceCondition*>( geometricCondition ) )
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_85_DISTANCE_SPATIAL_CONDITION_NOT_SUPPORTED)));

    const FdoSpatialCondition * spatialCondition = dynamic_cast<const FdoSpatialCondition*>( geometricCondition );

    if ( NULL == spatialCondition )
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoRdbmsSpatialSecondaryFilterLocal",
                                                               L"geometricCondition"));

    // Note: needed to cast away constness.
    FdoPtr<FdoExpression> geometryExpression = ((FdoSpatialCondition *)spatialCondition)->GetGeometry();
    FdoGeometryValue * geometryValue = dynamic_cast<FdoGeometryValue*>(geometryExpression.p);
    if (NULL == geometryValue)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_84_SPATIAL_CONDITION_NOT_LITERAL_GEOMETRY)));

    FdoPtr<FdoByteArray> geometryFgf = geometryValue->GetGeometry();
    if (geometryFgf == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_60_NULL_POINTER)));

    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    mGeometryOfCondition = gf->CreateGeometryFromFgf(geometryFgf);

    // Note: needed to cast away constness.
    mOperation = ((FdoSpatialCondition *)spatialCondition)->GetOperation();
}


FdoRdbmsOdbcSpatialSecondaryFilterLocal::~FdoRdbmsOdbcSpatialSecondaryFilterLocal()
{
}
