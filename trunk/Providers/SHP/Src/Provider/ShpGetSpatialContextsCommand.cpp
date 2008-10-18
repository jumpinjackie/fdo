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

#include "ShpGetSpatialContextsCommand.h"
#include "ShpSpatialContextReader.h"

ShpGetSpatialContextsCommand::ShpGetSpatialContextsCommand() :
    FdoCommonCommand<FdoIGetSpatialContexts, ShpConnection>(NULL),
    mActiveOnly(false)
{
}

ShpGetSpatialContextsCommand::ShpGetSpatialContextsCommand(FdoIConnection *connection) :
    FdoCommonCommand<FdoIGetSpatialContexts, ShpConnection>(connection),
    mActiveOnly(false)
{
}

ShpGetSpatialContextsCommand::~ShpGetSpatialContextsCommand(void)
{
}

const bool ShpGetSpatialContextsCommand::GetActiveOnly()
{
    return (mActiveOnly);
}

void ShpGetSpatialContextsCommand::SetActiveOnly(const bool value)
{
	// Since SHP provider doesn't implement Activate Spatial Context command,
	// disallow this option.
	if ( value )
//		throw FdoException::Create(NlsMsgGet(SHP_SPATIALCONTEXT_INVALIDOPTION, "Unsupported option." ));
    mActiveOnly = value;
}

FdoISpatialContextReader* ShpGetSpatialContextsCommand::Execute()
{
	// Get the spatial context list. The extents will be recomputed with the actual
	// extents of the SHP files grouped by the same Coordinate System.

    ShpSpatialContextCollectionP spatialContextColl = mConnection->GetSpatialContexts (true);
    return (new ShpSpatialContextReader (spatialContextColl));
}
