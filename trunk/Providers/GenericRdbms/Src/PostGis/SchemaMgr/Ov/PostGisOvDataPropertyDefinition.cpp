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
#include <Rdbms/Override/PostGis/PostGisOvDataPropertyDefinition.h>

FdoPostGISOvDataPropertyDefinition* FdoPostGISOvDataPropertyDefinition::Create()
{
    return (new FdoPostGISOvDataPropertyDefinition());
}

FdoPostGISOvDataPropertyDefinition* FdoPostGISOvDataPropertyDefinition::Create(
    FdoString* name)
{
    return (new FdoPostGISOvDataPropertyDefinition(name));
}


FdoPostGISOvDataPropertyDefinition::FdoPostGISOvDataPropertyDefinition()
{
    // idle
}

FdoPostGISOvDataPropertyDefinition::FdoPostGISOvDataPropertyDefinition(
    FdoString* name) : FdoRdbmsOvPropertyDefinition(name),
      FdoRdbmsOvDataPropertyDefinition(name),
      FdoPostGISOvPropertyDefinition(name)
{
    // idle
}

FdoPostGISOvDataPropertyDefinition::~FdoPostGISOvDataPropertyDefinition()
{
    // idle
}

FdoPostGISOvColumn* FdoPostGISOvDataPropertyDefinition::GetColumn()
{
    FdoRdbmsOvColumn* rdbmsColumn = NULL;
    rdbmsColumn = FdoRdbmsOvDataPropertyDefinition::GetColumn();

    return (static_cast<FdoPostGISOvColumn*>(rdbmsColumn));
}

void FdoPostGISOvDataPropertyDefinition::SetColumn(FdoPostGISOvColumn *column)
{
    FdoRdbmsOvDataPropertyDefinition::SetColumn(column);
}

void FdoPostGISOvDataPropertyDefinition::Dispose()
{
    delete this;
}
