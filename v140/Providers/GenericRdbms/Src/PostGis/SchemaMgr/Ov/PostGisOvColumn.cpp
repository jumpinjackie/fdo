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
#include <Rdbms/Override/PostGis/PostGisOvColumn.h>

FdoPostGISOvColumn* FdoPostGISOvColumn::Create()
{
    return (new FdoPostGISOvColumn());
}

FdoPostGISOvColumn* FdoPostGISOvColumn::Create(FdoString *name)
{
    return (new FdoPostGISOvColumn(name));
}

FdoPostGISOvColumn::FdoPostGISOvColumn()
{
    // idle
}

FdoPostGISOvColumn::FdoPostGISOvColumn(FdoString* name)
    : FdoRdbmsOvColumn(name)
{
    // idle
}

FdoPostGISOvColumn::~FdoPostGISOvColumn()
{
    // idle
}

void FdoPostGISOvColumn::Dispose()
{
    delete this;
}

void FdoPostGISOvColumn::InitFromXml(FdoXmlSaxContext* saxContext,
                                     FdoXmlAttributeCollection* attrs)
{
    FdoRdbmsOvColumn::InitFromXml(saxContext, attrs);
}

void FdoPostGISOvColumn::_writeXmlContents(FdoXmlWriter* xmlWriter,
                                           const FdoXmlFlags* flags)
{
    // idle
}
