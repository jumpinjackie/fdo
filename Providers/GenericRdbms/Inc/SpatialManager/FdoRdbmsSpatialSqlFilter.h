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


#ifndef FdoRdbmsSpatialSqlFilter_h
#define FdoRdbmsSpatialSqlFilter_h

#ifdef _WIN32
#pragma once
#endif

#include "../Src/SchemaMgr/SchemaManager.h"
#include "../SchemaMgr/Lp/GeometricPropertyDefinition.h"

/// <summary>This abstract class defines translation from FDO geometric filtering
/// conditions to SQL.
/// It does not define more precise secondary filtering.</summary>
class FdoRdbmsSpatialSqlFilter : public FdoDisposable
{
public:

    virtual const char* FilterToSql() = 0;

protected:

    FdoRdbmsSpatialSqlFilter          () {};
    virtual ~FdoRdbmsSpatialSqlFilter () {};

};  //  class FdoRdbmsSpatialSqlFilter

typedef FdoPtr<FdoRdbmsSpatialSqlFilter> FdoRdbmsSpatialSqlFilterP;

#endif


