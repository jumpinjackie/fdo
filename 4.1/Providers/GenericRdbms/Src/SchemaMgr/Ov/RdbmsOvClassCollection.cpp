/*
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
 */

#include "stdafx.h"

#include <Rdbms/Override/RdbmsOvClassCollection.h>

FdoRdbmsOvClassCollection* FdoRdbmsOvClassCollection::Create()
{
    return new FdoRdbmsOvClassCollection();
}

FdoRdbmsOvClassCollection* FdoRdbmsOvClassCollection::Create(FdoPhysicalElementMapping* parent)
{
    return new FdoRdbmsOvClassCollection(parent);
}

FdoRdbmsOvClassCollection::FdoRdbmsOvClassCollection()
{
}

FdoRdbmsOvClassCollection::FdoRdbmsOvClassCollection(FdoPhysicalElementMapping* parent) :
    FdoPhysicalElementMappingCollection<FdoRdbmsOvClassDefinition>(parent)
{
}

FdoRdbmsOvClassCollection::~FdoRdbmsOvClassCollection()
{
}

void FdoRdbmsOvClassCollection::Dispose()
{
    delete this;
}

FdoRdbmsOvReadOnlyClassCollection* FdoRdbmsOvReadOnlyClassCollection::Create()
{
    return new FdoRdbmsOvReadOnlyClassCollection();
}

FdoRdbmsOvReadOnlyClassCollection* FdoRdbmsOvReadOnlyClassCollection::Create(FdoRdbmsOvClassCollection* baseCollection)
{
    return new FdoRdbmsOvReadOnlyClassCollection(baseCollection);
}

FdoRdbmsOvReadOnlyClassCollection::FdoRdbmsOvReadOnlyClassCollection()
{
}

FdoRdbmsOvReadOnlyClassCollection::FdoRdbmsOvReadOnlyClassCollection(FdoRdbmsOvClassCollection* baseCollection) :
    FdoReadOnlyNamedCollection<FdoRdbmsOvClassDefinition,FdoRdbmsOvClassCollection >(
        baseCollection
    )
{
}

FdoRdbmsOvReadOnlyClassCollection::~FdoRdbmsOvReadOnlyClassCollection()
{
}

void FdoRdbmsOvReadOnlyClassCollection::Dispose()
{
    delete this;
}
