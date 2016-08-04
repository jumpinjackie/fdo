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
#include <Rdbms/Override/PostGis/PostGisOvGeometricPropertyDefinition.h>

FdoPostGISOvGeometricPropertyDefinition* FdoPostGISOvGeometricPropertyDefinition::Create()
{
    return (new FdoPostGISOvGeometricPropertyDefinition());
}

FdoPostGISOvGeometricPropertyDefinition* FdoPostGISOvGeometricPropertyDefinition::Create(
    FdoString* name)
{
    return (new FdoPostGISOvGeometricPropertyDefinition(name));
}

FdoPostGISOvGeometricPropertyDefinition::FdoPostGISOvGeometricPropertyDefinition()
{
    // idle
}

FdoPostGISOvGeometricPropertyDefinition::FdoPostGISOvGeometricPropertyDefinition(
    FdoString* name) : FdoRdbmsOvPropertyDefinition(name),
    FdoRdbmsOvGeometricPropertyDefinition(name),
    FdoPostGISOvPropertyDefinition(name)
{
    // idle
}

FdoPostGISOvGeometricPropertyDefinition::~FdoPostGISOvGeometricPropertyDefinition()
{
    // idle
}

FdoPostGISOvGeometricColumn* FdoPostGISOvGeometricPropertyDefinition::GetColumn()
{
    FdoPostGISOvGeometricColumn* column = NULL;
    column =  static_cast<FdoPostGISOvGeometricColumn*>(
        FdoRdbmsOvGeometricPropertyDefinition::GetColumn());

    return column; 
}

void FdoPostGISOvGeometricPropertyDefinition::SetColumn(
    FdoPostGISOvGeometricColumn* column)
{
    FdoRdbmsOvGeometricPropertyDefinition::SetColumn(column);
}

void FdoPostGISOvGeometricPropertyDefinition::Dispose()
{
    delete this;
}
