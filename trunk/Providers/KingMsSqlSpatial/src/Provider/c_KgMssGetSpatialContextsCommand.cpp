/*
* Copyright (C) 2007  Haris Kurtagic
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
*/


#include "stdafx.h"

#include "c_KgMssGetSpatialContextsCommand.h"
#include "c_KgMssSpatialContextReader.h"

c_KgMssGetSpatialContextsCommand::c_KgMssGetSpatialContextsCommand() :
    c_KgMssFdoCommand<FdoIGetSpatialContexts>(NULL),
    m_ActiveOnly(false)
{
}

c_KgMssGetSpatialContextsCommand::c_KgMssGetSpatialContextsCommand(c_KgMssConnection* Connection) :
    c_KgMssFdoCommand<FdoIGetSpatialContexts>(Connection),
    m_ActiveOnly(false)
{
}

c_KgMssGetSpatialContextsCommand::~c_KgMssGetSpatialContextsCommand(void)
{
}

const bool c_KgMssGetSpatialContextsCommand::GetActiveOnly()
{
    return (m_ActiveOnly);
}

void c_KgMssGetSpatialContextsCommand::SetActiveOnly(const bool value)
{	
    m_ActiveOnly = value;
}

FdoISpatialContextReader* c_KgMssGetSpatialContextsCommand::Execute()
{

    FdoPtr<c_KgMssSpatialContextCollection> sc_col = m_Connection->GetSpatialContexts (true);
    return new c_KgMssSpatialContextReader (sc_col);
}
