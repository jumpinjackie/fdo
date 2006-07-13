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
    m_bActiveOnly(false)
{
}

ArcSDEGetSpatialContexts::ArcSDEGetSpatialContexts(FdoIConnection *connection) :
    ArcSDECommand<FdoIGetSpatialContexts>(connection),
    m_bActiveOnly(false)
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
    LONG lResult = SE_SUCCESS;
    SE_SPATIALREFINFO *arrSpatialRefs = NULL;
    LONG lSpatialRefCount = 0L;
    FdoPtr<FdoISQLDataReader> sqlReader;

    // Either return all spatial contexts, or only the active one:
    if (m_bActiveOnly)
    {
        FdoString* spatialContextName = mConnection->GetActiveSpatialContext();
        if (NULL != spatialContextName)
        {
            // map spatial context name to spatial reference system SRID:
            LONG lSRID = -1L;
            lSRID = ArcSDESpatialContextUtility::SpatialContextNameToSRID(mConnection, spatialContextName);

            // fetch the request ArcSDE Spatial Reference System (equivalent to one FDO Spatial Context):
            arrSpatialRefs = new SE_SPATIALREFINFO[1];
            lSpatialRefCount = 1;
            lResult = SE_spatialrefinfo_create(&(arrSpatialRefs[0]));
            handle_sde_err<FdoCommandException>(mConnection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_FAILED_TO_RETRIEVE_SRS, "Failed to retrieve requested ArcSDE Spatial Reference Systems.");
            lResult = SE_spatialref_get_info(mConnection->GetConnection(), lSRID, arrSpatialRefs[0]);
            if (lResult != SE_SUCCESS)
            {
                delete[] arrSpatialRefs;

                FdoPtr<FdoISQLCommand> sql = (FdoISQLCommand*)mConnection->CreateCommand(FdoCommandType_SQLCommand);
                sql->SetSQLStatement(FdoStringP::Format(L"select * from %lsSPATIAL_REFERENCES WHERE SRID=%d", mConnection->RdbmsSystemTablePrefix(), lSRID));
                sqlReader = sql->ExecuteReader();
            }
        }
    }
    else
    {
        // fetch all ArcSDE Spatial Reference Systems (equivalent to FDO Spatial Contexts):
        lResult = SE_spatialref_get_info_list(mConnection->GetConnection(), &arrSpatialRefs, &lSpatialRefCount);
        if (lResult != SE_SUCCESS)
        {
            FdoPtr<FdoISQLCommand> sql = (FdoISQLCommand*)mConnection->CreateCommand(FdoCommandType_SQLCommand);
            sql->SetSQLStatement(FdoStringP::Format(L"select * from %lsSPATIAL_REFERENCES", mConnection->RdbmsSystemTablePrefix()));
            sqlReader = sql->ExecuteReader();
        }
    }

    // Create & return the Spatial Context reader:
    if (sqlReader)
        return new ArcSDESpatialContextSQLReader(mConnection, sqlReader);
    else
        return new ArcSDESpatialContextReader(mConnection, arrSpatialRefs, lSpatialRefCount, m_bActiveOnly);
}

