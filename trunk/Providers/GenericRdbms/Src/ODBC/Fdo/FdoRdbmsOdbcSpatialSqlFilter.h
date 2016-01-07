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


#ifndef FdoRdbmsOdbcSpatialSqlFilter_h
#define FdoRdbmsOdbcSpatialSqlFilter_h

#ifdef _WIN32
#pragma once
#endif

#include <SpatialManager/FdoRdbmsSpatialSqlFilter.h>


/// <summary>This implementation of FdoRdbmsSpatialSqlFilter supports the ODBC
/// FDO Provider.  There is no spatial index, only rectangular spatial conditions
// can be used, and geometries can only appear as Point type supports by two 
// or three ordinate columns.  Only the X and Y columns are supported by this filter.
class FdoRdbmsOdbcSpatialSqlFilter : public FdoRdbmsSpatialSqlFilter
{
public:

    static FdoRdbmsOdbcSpatialSqlFilter * Create(
        const FdoSmLpGeometricPropertyDefinition * geometricProperty,
        const FdoGeometricCondition * geometricCondition);

    virtual const char* FilterToSql();

protected:

    FdoRdbmsOdbcSpatialSqlFilter          ();
    FdoRdbmsOdbcSpatialSqlFilter(
        const FdoSmLpGeometricPropertyDefinition * geometricProperty,
        const FdoGeometricCondition * geometricCondition);
    virtual ~FdoRdbmsOdbcSpatialSqlFilter ();

    void ProcessOrdinateRange( FdoString * ordinateColumnName, double minimum, double maximum, FdoSpatialOperations spatialOp );

    FdoStringP                      m_sqlClause;

};  //  class FdoRdbmsOdbcSpatialSqlFilter

#endif

