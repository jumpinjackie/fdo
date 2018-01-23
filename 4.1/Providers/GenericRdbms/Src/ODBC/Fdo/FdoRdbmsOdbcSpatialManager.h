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


#ifndef FdoRdbmsOdbcSpatialManager_h
#define FdoRdbmsOdbcSpatialManager_h

#ifdef _WIN32
#pragma once
#endif

#include <SpatialManager/FdoRdbmsSpatialManager.h>


/// <summary>This is an implementation class for spatial support in the ODBC 
/// FDO provider.
/// </summary>
class FdoRdbmsOdbcSpatialManager : public FdoRdbmsSpatialManager
{
public:

    static FdoRdbmsOdbcSpatialManager * Create();

    virtual FdoRdbmsSpatialSqlFilter * GetSqlFilter(
        const FdoSmLpGeometricPropertyDefinition * geometricProperty,
        const FdoGeometricCondition * geometricCondition);

    virtual FdoRdbmsSpatialSecondaryFilter * GetSecondaryFilter(
        const FdoSmLpGeometricPropertyDefinition * geometricProperty,
        const FdoGeometricCondition * geometricCondition);

    virtual void UpdateGeometriesInLine(
        const FdoSmLpClassDefinition *classDefinition, 
        FdoPropertyValueCollection * values);

    virtual void UpdateGeometriesOutOfLine(
        const FdoSmLpClassDefinition *classDefinition, 
        FdoPropertyValueCollection * values);

    virtual void InsertGeometryInLine(
        const FdoSmLpGeometricPropertyDefinition * geomPropDef, 
        FdoGeometryValue * value,
        FdoStringsP& siKeys);

    virtual void InsertGeometriesOutOfLine(
        const FdoSmLpClassDefinition *classDefinition, 
        FdoPropertyValueCollection * values);

    virtual void DeleteGeometriesOutOfLine(
        const FdoSmLpClassDefinition *classDefinition, 
        const FdoIdentifierCollection * identifiers);

    virtual void CreateSpatialIndex(
        const FdoSmLpGeometricPropertyDefinition * geometricProperty);

    virtual void DropSpatialIndex(
        const FdoSmLpGeometricPropertyDefinition * geometricProperty);

protected:

    FdoRdbmsOdbcSpatialManager          ();
    virtual ~FdoRdbmsOdbcSpatialManager ();

};  //  class FdoRdbmsOdbcSpatialManager

#endif

