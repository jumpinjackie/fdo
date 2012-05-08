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
#include <Rdbms/Override/PostGis/PostGisOvTable.h>

FdoPostGISOvTable* FdoPostGISOvTable::Create()
{
    return (new FdoPostGISOvTable());
}

FdoPostGISOvTable* FdoPostGISOvTable::Create(FdoString* name)
{
    return (new FdoPostGISOvTable(name));
}

FdoPostGISOvTable::FdoPostGISOvTable()
{
    Init();
}

FdoPostGISOvTable::FdoPostGISOvTable(FdoString* name)
    : FdoRdbmsOvTable(name)
{
    Init();
}

FdoPostGISOvTable::~FdoPostGISOvTable()
{
    // idle
}

FdoString* FdoPostGISOvTable::GetOwner()
{
    return mOwner;
}

void FdoPostGISOvTable::SetOwner(FdoString* owner)
{
    mOwner = owner;
}

void FdoPostGISOvTable::SetParent(FdoPhysicalElementMapping* value)
{
    FdoPhysicalElementMapping::SetParent(value);
}

void FdoPostGISOvTable::InitFromXml(FdoXmlSaxContext* saxContext,
                                    FdoXmlAttributeCollection* attrs)
{
    FdoRdbmsOvTable::InitFromXml(saxContext, attrs);

    FdoXmlAttributeP xmlAttr = attrs->FindItem(L"owner");
    if (xmlAttr)
    {
        mOwner = xmlAttr->GetValue();
    }
}

void FdoPostGISOvTable::_writeXmlContents(FdoXmlWriter* xmlWriter,
                                          const FdoXmlFlags* flags)
{
    if (mOwner.GetLength() > 0)
        xmlWriter->WriteAttribute(L"owner", mOwner);

}

void FdoPostGISOvTable::Dispose()
{
    delete this;
}

void FdoPostGISOvTable::Init()
{
}
