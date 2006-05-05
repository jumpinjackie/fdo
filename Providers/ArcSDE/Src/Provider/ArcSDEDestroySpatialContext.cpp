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
#include "ArcSDEDestroySpatialContext.h"


ArcSDEDestroySpatialContext::ArcSDEDestroySpatialContext(ArcSDEConnection *connection) :
    ArcSDECommand<FdoIDestroySpatialContext>(connection)
{
}

ArcSDEDestroySpatialContext::~ArcSDEDestroySpatialContext(void)
{
}

FdoString* ArcSDEDestroySpatialContext::GetName()
{
    return m_wcsName;
}

void ArcSDEDestroySpatialContext::SetName(FdoString* value)
{
    m_wcsName = value;
}

void ArcSDEDestroySpatialContext::Execute()
{
    // Validate name:
    if (m_wcsName==L"")
        throw FdoCommandException::Create(NlsMsgGet(ARCSDE_SPATIALCONTEXT_NAME_NOT_SPECIFIED, "Spatial context name not specified."));

    // Convert name to SRID:
    LONG lSRID = ArcSDESpatialContextUtility::SpatialContextNameToSRID(mConnection, m_wcsName);

    // Perform the delete:
    LONG lResult = SE_spatialref_delete(mConnection->GetConnection(), lSRID);
    handle_sde_err<FdoCommandException>(mConnection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_SPATIALCONTEXT_DELETE_FAILED, "Failed to delete spatial context '%1$ls'.", (const wchar_t*)m_wcsName);

    // If this was the active spatial context, reset the active spatial context:
    if ((mConnection->GetActiveSpatialContext() != NULL) && (0==wcscmp(m_wcsName, mConnection->GetActiveSpatialContext())))
        mConnection->SetActiveSpatialContextToDefault();
}


