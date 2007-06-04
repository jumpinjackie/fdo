//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#include "stdafx.h"

#include "PostGisProvider.h"
#include "SelectAggregatesCommand.h"
#include "FeatureReader.h"
#include "FilterProcessor.h"
#include "Connection.h"
#include "PgCursor.h"
#include "PgUtility.h"
#include "PostGIS/FdoPostGisOverrides.h"
// std
#include <cassert>
#include <string>

namespace fdo { namespace postgis {

SelectAggregatesCommand::SelectAggregatesCommand(Connection* conn) :
    Base(conn),
    mProperties(NULL),
    mOrderingProperties(NULL),
    mOrderingOption(FdoOrderingOption_Ascending),
    mGroupingProperties(NULL),
    mGroupingFilter(NULL),
    mDistinct(false)
{
    // idle
}

SelectAggregatesCommand::~SelectAggregatesCommand()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void SelectAggregatesCommand::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoIBaseSelect interface
///////////////////////////////////////////////////////////////////////////////

FdoIdentifierCollection* SelectAggregatesCommand::GetPropertyNames()
{
    if (NULL == mProperties)
    {
        mProperties = FdoIdentifierCollection::Create();
    }

    FDO_SAFE_ADDREF(mProperties.p);
    return mProperties.p;
}
 	
FdoIdentifierCollection* SelectAggregatesCommand::GetOrdering()
{
    if (NULL == mOrderingProperties)
    {
        mOrderingProperties = FdoIdentifierCollection::Create();
    }

    FDO_SAFE_ADDREF(mOrderingProperties.p);
    return mOrderingProperties.p;
}

void SelectAggregatesCommand::SetOrderingOption(FdoOrderingOption option)
{
    FDOLOG_MARKER("SelectAggregatesCommand::+SetOrderingOption");
    FDOLOG_WRITE("Ordering: %d", option);

    mOrderingOption = option;
}

FdoOrderingOption SelectAggregatesCommand::GetOrderingOption()
{
    return mOrderingOption;
}

///////////////////////////////////////////////////////////////////////////////
// FdoISelectAggregates interface
///////////////////////////////////////////////////////////////////////////////

void SelectAggregatesCommand::SetDistinct(bool value)
{
    mDistinct = value;
}

bool SelectAggregatesCommand::GetDistinct()
{
    return mDistinct    ;
}

FdoIdentifierCollection* SelectAggregatesCommand::GetGrouping()
{
    if (NULL == mGroupingProperties)
    {
        mGroupingProperties = FdoIdentifierCollection::Create();
    }

    FDO_SAFE_ADDREF(mGroupingProperties.p);
    return mGroupingProperties.p;
}

void SelectAggregatesCommand::SetGroupingFilter(FdoFilter* filter)
{
    mGroupingFilter = filter;
    FDO_SAFE_ADDREF(mGroupingFilter.p);
}

FdoFilter* SelectAggregatesCommand::GetGroupingFilter()
{
    FDO_SAFE_ADDREF(mGroupingFilter.p);
    return mGroupingFilter.p;
}

FdoIDataReader* SelectAggregatesCommand::Execute()
{
    FDOLOG_MARKER("SelectAggregatesCommand::+Execute");

    return NULL;
}

}} // namespace fdo::postgis
