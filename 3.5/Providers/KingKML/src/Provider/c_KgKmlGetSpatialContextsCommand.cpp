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

#include "c_KgKmlGetSpatialContextsCommand.h"
#include "c_KgKmlSpatialContextReader.h"

c_KgKmlGetSpatialContextsCommand::c_KgKmlGetSpatialContextsCommand() :
    c_KgKmlFdoCommand<FdoIGetSpatialContexts>(NULL),
    m_ActiveOnly(false)
{
}

c_KgKmlGetSpatialContextsCommand::c_KgKmlGetSpatialContextsCommand(c_KgKmlConnection* Connection) :
    c_KgKmlFdoCommand<FdoIGetSpatialContexts>(Connection),
    m_ActiveOnly(false)
{
}

c_KgKmlGetSpatialContextsCommand::~c_KgKmlGetSpatialContextsCommand(void)
{
}

const bool c_KgKmlGetSpatialContextsCommand::GetActiveOnly()
{
    return (m_ActiveOnly);
}

void c_KgKmlGetSpatialContextsCommand::SetActiveOnly(const bool value)
{	
    m_ActiveOnly = value;
}

FdoISpatialContextReader* c_KgKmlGetSpatialContextsCommand::Execute()
{

    FdoPtr<c_KgKmlSpatialContextCollection> sc_col = m_Connection->GetSpatialContexts (true);
    return new c_KgKmlSpatialContextReader (sc_col);
}
