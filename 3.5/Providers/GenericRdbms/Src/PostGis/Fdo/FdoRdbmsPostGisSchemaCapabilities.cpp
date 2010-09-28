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
#include "FdoRdbmsPostGisSchemaCapabilities.h"
#include "../../Src/Fdo/Capability/FdoRdbmsSchemaCapabilities.h"

FdoRdbmsPostGisSchemaCapabilities::FdoRdbmsPostGisSchemaCapabilities()
{
    // idle
}

bool FdoRdbmsPostGisSchemaCapabilities::SupportsInclusiveValueRangeConstraints()
{
    return true;
}

bool FdoRdbmsPostGisSchemaCapabilities::SupportsExclusiveValueRangeConstraints()
{
    return true;
}

bool FdoRdbmsPostGisSchemaCapabilities::SupportsValueConstraintsList()
{
    return true;
}

bool FdoRdbmsPostGisSchemaCapabilities::SupportsSchemaOverrides()
{
    return false;
}

bool FdoRdbmsPostGisSchemaCapabilities::SupportsObjectProperties()
{
    return false;
}

bool FdoRdbmsPostGisSchemaCapabilities::SupportsAssociationProperties()
{
    return false;
}

FdoDataType* FdoRdbmsPostGisSchemaCapabilities::GetSupportedAutoGeneratedTypes(FdoInt32& length)
{
    static FdoDataType autogen_types[] =
    {
        FdoDataType_Int32,
        FdoDataType_Int64
    };

    length = sizeof (autogen_types) / sizeof (FdoDataType);

    return (autogen_types);
}
