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
#include "FdoRdbmsOdbcSpatialManager.h"
#include "FdoRdbmsOdbcSpatialSqlFilter.h"


FdoRdbmsOdbcSpatialManager *
FdoRdbmsOdbcSpatialManager::Create()
{
    FdoRdbmsOdbcSpatialManager * spm = new FdoRdbmsOdbcSpatialManager();

    if (NULL == spm)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

    return spm;
}


FdoRdbmsSpatialSqlFilter *
FdoRdbmsOdbcSpatialManager::GetSqlFilter(
    const FdoSmLpGeometricPropertyDefinition * geometricProperty,
    const FdoGeometricCondition * geometricCondition)
{
    FdoRdbmsSpatialSqlFilter * sqf =
        FdoRdbmsOdbcSpatialSqlFilter::Create(geometricProperty, geometricCondition);

    return sqf;
}


FdoRdbmsSpatialSecondaryFilter *
FdoRdbmsOdbcSpatialManager::GetSecondaryFilter(
    const FdoSmLpGeometricPropertyDefinition * geometricProperty,
    const FdoGeometricCondition * geometricCondition)
{
    return NULL;
}


void
FdoRdbmsOdbcSpatialManager::UpdateGeometriesInLine(
    const FdoSmLpClassDefinition *classDefinition, 
    FdoPropertyValueCollection * values)
{
}


void
FdoRdbmsOdbcSpatialManager::UpdateGeometriesOutOfLine(
    const FdoSmLpClassDefinition *classDefinition, 
    FdoPropertyValueCollection * values)
{
    // This will only be implemented if dedicated SI tables are supported.
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
}


void
FdoRdbmsOdbcSpatialManager::InsertGeometryInLine(
    const FdoSmLpGeometricPropertyDefinition * geomPropDef, 
    FdoGeometryValue * value,
    FdoStringsP& siKeys)
{
}


void
FdoRdbmsOdbcSpatialManager::InsertGeometriesOutOfLine(
    const FdoSmLpClassDefinition *classDefinition, 
    FdoPropertyValueCollection * values)
{
    // This will only be implemented if dedicated geometry tables are supported.
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
}


void
FdoRdbmsOdbcSpatialManager::DeleteGeometriesOutOfLine(
    const FdoSmLpClassDefinition *classDefinition, 
    const FdoIdentifierCollection * identifiers)
{
    // This will only be implemented if dedicated geometry tables are supported.
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
}


void
FdoRdbmsOdbcSpatialManager::CreateSpatialIndex(
    const FdoSmLpGeometricPropertyDefinition * geometricProperty)
{
}


void
FdoRdbmsOdbcSpatialManager::DropSpatialIndex(
    const FdoSmLpGeometricPropertyDefinition * geometricProperty)
{
}


FdoRdbmsOdbcSpatialManager::FdoRdbmsOdbcSpatialManager()
{
}


FdoRdbmsOdbcSpatialManager::~FdoRdbmsOdbcSpatialManager()
{
}
