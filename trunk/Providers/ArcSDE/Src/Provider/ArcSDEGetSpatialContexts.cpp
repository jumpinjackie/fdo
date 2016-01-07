/*
 * 
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
#include "ArcSDEGetSpatialContexts.h"
#include "ArcSDESpatialContextReader.h"
#include "ArcSDESpatialContextSQLReader.h"


ArcSDEGetSpatialContexts::ArcSDEGetSpatialContexts() :
    ArcSDECommand<FdoIGetSpatialContexts>(NULL),
    m_bActiveOnly(false),
    m_lSridOnly(-1)
{
}

ArcSDEGetSpatialContexts::ArcSDEGetSpatialContexts(FdoIConnection *connection, long srid) :
    ArcSDECommand<FdoIGetSpatialContexts>(connection),
    m_bActiveOnly(false),
    m_lSridOnly(srid)
{
}

ArcSDEGetSpatialContexts::~ArcSDEGetSpatialContexts(void)
{
}

const bool ArcSDEGetSpatialContexts::GetActiveOnly()
{
    return m_bActiveOnly;
}

void ArcSDEGetSpatialContexts::SetActiveOnly(const bool value)
{
    m_bActiveOnly = value;
}

FdoISpatialContextReader* ArcSDEGetSpatialContexts::Execute()
{
    // Create & return the Spatial Context reader:
    if (m_lSridOnly == -1)
        return new ArcSDESpatialContextReader(mConnection, m_bActiveOnly);
    else
        return new ArcSDESpatialContextReader(mConnection, m_lSridOnly);
}
