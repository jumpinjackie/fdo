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
#include "ArcSDEActivateSpatialContext.h"

ArcSDEActivateSpatialContext::ArcSDEActivateSpatialContext(ArcSDEConnection *connection) :
    ArcSDECommand<FdoIActivateSpatialContext>(connection)
{
    m_wcsName = L"";
}

ArcSDEActivateSpatialContext::~ArcSDEActivateSpatialContext(void)
{
}


FdoString* ArcSDEActivateSpatialContext::GetName()
{
    return m_wcsName;
}

void ArcSDEActivateSpatialContext::SetName(FdoString* value)
{
    m_wcsName = value;
}

void ArcSDEActivateSpatialContext::Execute()
{
    // validate the spatial context name:
    (void)ArcSDESpatialContextUtility::SpatialContextNameToSRID(mConnection, m_wcsName);

    // set the active spatial context:
    mConnection->SetActiveSpatialContext(m_wcsName);
}


