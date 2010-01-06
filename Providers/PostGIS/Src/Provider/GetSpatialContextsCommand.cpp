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
#include "GetSpatialContextsCommand.h"
#include "Connection.h"
#include "SpatialContext.h"
#include "SpatialContextReader.h"
#include "SpatialContextCollection.h"

#include <cassert>

namespace fdo { namespace postgis {

GetSpatialContextsCommand::GetSpatialContextsCommand(Connection* conn)
    : Base(conn), mActiveOnly(false)
{
    FDOLOG_WRITE("GetSpatialContextsCommand created");
}

GetSpatialContextsCommand::~GetSpatialContextsCommand()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIGetSpatialContexts interface
///////////////////////////////////////////////////////////////////////////////

const bool GetSpatialContextsCommand::GetActiveOnly()
{
    return mActiveOnly;
}

void GetSpatialContextsCommand::SetActiveOnly(bool const activeOnly)
{
    mActiveOnly = activeOnly;
}

FdoISpatialContextReader* GetSpatialContextsCommand::Execute()
{
    FDOLOG_MARKER("GetSpatialContextsCommand::+Execute");

    try
    {
        SpatialContextCollection::Ptr spContexts;
        spContexts = mConn->GetSpatialContexts();

        FDOLOG_WRITE("Number of contexts: %d", spContexts->GetCount());
        return (new SpatialContextReader(spContexts));
    }
    catch (FdoException* e)
    {
        FDOLOG_WRITE("The execution of GetSpatialContextsCommand command failed.");

        FdoCommandException* ne = NULL;
        ne = FdoCommandException::Create(L"The execution of GetSpatialContextsCommand command failed.", e);
        e->Release();
        throw ne;
    }

    return NULL;
}

}} // namespace fdo::postgis
