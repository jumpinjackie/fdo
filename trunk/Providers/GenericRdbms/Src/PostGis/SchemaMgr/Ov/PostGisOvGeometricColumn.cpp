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
#include <Rdbms/Override/PostGis/PostGisOvGeometricColumn.h>

FdoPostGISOvGeometricColumn* FdoPostGISOvGeometricColumn::Create()
{
    return (new FdoPostGISOvGeometricColumn());
}

FdoPostGISOvGeometricColumn* FdoPostGISOvGeometricColumn::Create(
    FdoString* name)
{
    return (new FdoPostGISOvGeometricColumn(name));
}

FdoPostGISOvGeometricColumn::FdoPostGISOvGeometricColumn()
{
}

FdoPostGISOvGeometricColumn::FdoPostGISOvGeometricColumn(FdoString* name)
    : FdoRdbmsOvGeometricColumn(name)
{
    // idle
}

FdoPostGISOvGeometricColumn::~FdoPostGISOvGeometricColumn()
{
    // idle
}

void FdoPostGISOvGeometricColumn::Dispose()
{
    delete this;
}

void FdoPostGISOvGeometricColumn::InitFromXml(FdoXmlSaxContext* saxContext,
                                              FdoXmlAttributeCollection* attrs)
{
    FdoRdbmsOvGeometricColumn::InitFromXml(saxContext, attrs);
}

void FdoPostGISOvGeometricColumn::_writeXmlContents(FdoXmlWriter* xmlWriter,
                                                    const FdoXmlFlags* flags)
{
    // idle
}
