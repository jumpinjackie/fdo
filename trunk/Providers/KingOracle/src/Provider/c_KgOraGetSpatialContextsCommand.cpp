/*
* Copyright (C) 2006  SL-King d.o.o
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

#include "c_KgOraGetSpatialContextsCommand.h"
#include "c_KgOraSpatialContextReader.h"

c_KgOraGetSpatialContextsCommand::c_KgOraGetSpatialContextsCommand() :
    c_KgOraFdoCommand<FdoIGetSpatialContexts>(NULL),
    m_ActiveOnly(false)
{
}

c_KgOraGetSpatialContextsCommand::c_KgOraGetSpatialContextsCommand(c_KgOraConnection* Connection) :
    c_KgOraFdoCommand<FdoIGetSpatialContexts>(Connection),
    m_ActiveOnly(false)
{
}

c_KgOraGetSpatialContextsCommand::~c_KgOraGetSpatialContextsCommand(void)
{
}

const bool c_KgOraGetSpatialContextsCommand::GetActiveOnly()
{
    return (m_ActiveOnly);
}

void c_KgOraGetSpatialContextsCommand::SetActiveOnly(const bool value)
{	
    m_ActiveOnly = value;
}

FdoISpatialContextReader* c_KgOraGetSpatialContextsCommand::Execute()
{

    FdoPtr<c_KgOraSpatialContextCollection> sc_col = m_Connection->GetSpatialContexts (true);
    return new c_KgOraSpatialContextReader (sc_col);
}
