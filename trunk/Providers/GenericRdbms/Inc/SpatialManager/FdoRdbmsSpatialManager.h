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
 ******************************************************************************/


#ifndef FdoRdbmsSpatialManager_h
#define FdoRdbmsSpatialManager_h

#ifdef _WIN32
#pragma once
#endif

#include "../Src/SchemaMgr/SchemaManager.h"
#include "../SchemaMgr/Lp/GeometricPropertyDefinition.h"
#include <Sm/Lp/ClassDefinition.h>
#include <SpatialManager/FdoRdbmsSpatialSqlFilter.h>
#include <SpatialManager/FdoRdbmsSpatialSecondaryFilter.h>


/// <summary>This is an abstract class for spatial support in GenericRdbms-based
/// providers.  Providers may implement it in order to have their own behaviour.
/// Note that the predefined implementations, FdoRdbmsSpatialManagerRsi
/// and FdoRdbmsSpatialManagerStub.
class FdoRdbmsSpatialManager : public FdoDisposable
{
public:

    virtual FdoRdbmsSpatialSqlFilter * GetSqlFilter(
        const FdoSmLpGeometricPropertyDefinition * geometricProperty,
        const FdoGeometricCondition * geometricCondition) = 0;

    virtual FdoRdbmsSpatialSecondaryFilter * GetSecondaryFilter(
        const FdoSmLpGeometricPropertyDefinition * geometricProperty,
        const FdoGeometricCondition * geometricCondition) = 0;

    virtual void UpdateGeometriesInLine(
        const FdoSmLpClassDefinition *classDefinition, 
        FdoPropertyValueCollection * values) = 0;

    virtual void UpdateGeometriesOutOfLine(
        const FdoSmLpClassDefinition *classDefinition, 
        FdoPropertyValueCollection * values) = 0;

    virtual void InsertGeometryInLine(
        const FdoSmLpGeometricPropertyDefinition * geomPropDef, 
        FdoGeometryValue * value,
        FdoStringsP& siKeys) = 0;

    virtual void InsertGeometriesOutOfLine(
        const FdoSmLpClassDefinition *classDefinition, 
        FdoPropertyValueCollection * values) = 0;

    virtual void DeleteGeometriesOutOfLine(
        const FdoSmLpClassDefinition *classDefinition, 
        const FdoIdentifierCollection * identifiers) = 0;

    virtual void CreateSpatialIndex(
        const FdoSmLpGeometricPropertyDefinition * geometricProperty) = 0;

    virtual void DropSpatialIndex(
        const FdoSmLpGeometricPropertyDefinition * geometricProperty) = 0;

protected:

    FdoRdbmsSpatialManager          () {};
    virtual ~FdoRdbmsSpatialManager () {};
};  //  class FdoRdbmsSpatialManager

typedef FdoPtr<FdoRdbmsSpatialManager> FdoRdbmsSpatialManagerP;

#endif


