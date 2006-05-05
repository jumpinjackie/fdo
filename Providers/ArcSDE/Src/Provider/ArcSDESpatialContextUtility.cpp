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
#include "ArcSDESpatialContextUtility.h"


// Convert spatial context name to ArcSDE SRID;
// throw an exception if the name is not valid:
LONG ArcSDESpatialContextUtility::SpatialContextNameToSRID(ArcSDEConnection* connection, FdoString *spatialContextName)
{
    LONG lSRID = -1;

    // validate input:
    if ((NULL == spatialContextName) || (NULL == connection))
        throw FdoException::Create(NlsMsgGet(ARCSDE_NULL_ARGUMENT, "A required argument was set to NULL."));

    FdoPtr<ArcSDEGetSpatialContexts> getSpatialContextsCmd = (ArcSDEGetSpatialContexts*)connection->CreateCommand(FdoCommandType_GetSpatialContexts);
    getSpatialContextsCmd->SetActiveOnly(false);
    FdoPtr<ArcSDESpatialContextReader> reader = (ArcSDESpatialContextReader*)getSpatialContextsCmd->Execute();
    while (reader->ReadNext())
    {
        if (0==wcscmp(reader->GetName(), spatialContextName))
        {
            lSRID = reader->GetSRID();
            break;
        }
    }

    // Validate that we found the requested spatial context:
    if (-1==lSRID)
        throw FdoException::Create(NlsMsgGet1(ARCSDE_SPATIALCONTEXT_NAME_NOT_FOUND, "A spatial context of name '%1$ls' was not found.", (const wchar_t*)spatialContextName));

    return lSRID;
}



// Get the spatial context name from an ArcSDE SRID:
FdoStringP ArcSDESpatialContextUtility::SRIDToSpatialContextName(ArcSDEConnection* connection, LONG srid)
{
    SE_SPATIALREFINFO spatialRefInfo;
    LONG result = SE_spatialrefinfo_create(&spatialRefInfo);
    handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_RETRIEVE_COORDSYS_FAILED, "Error while retrieving active coordinate reference system.");
    result = SE_spatialref_get_info(connection->GetConnection(), srid, spatialRefInfo);
    handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_RETRIEVE_COORDSYS_FAILED, "Error while retrieving active coordinate reference system.");

    FdoStringP ret = GetSpatialContextName(spatialRefInfo);

    SE_spatialrefinfo_free (spatialRefInfo);

    return ret;
}

FdoStringP ArcSDESpatialContextUtility::GetSpatialContextName(SE_SPATIALREFINFO spatialRefInfo)
{
    FdoStringP spatialContextName;
    LONG lResult = -1;

    // Try to get name from AUTH_NAME field:
    CHAR mbAuthName[SE_MAX_SPATIALREF_AUTHNAME_LEN];
    lResult = SE_spatialrefinfo_get_auth_name(spatialRefInfo, mbAuthName);
    handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_TO_READ_SRS, "Failed to get or set information for this ArcSDE Spatial Reference System.");

    wchar_t *wAuthName = NULL;
    multibyte_to_wide(wAuthName, mbAuthName);
    if (0==wcsncmp(wAuthName, SPATIALCONTEXT_AUTHNAME_PREFIX, wcslen(SPATIALCONTEXT_AUTHNAME_PREFIX)))
    {
        spatialContextName = (const wchar_t*)(wAuthName + wcslen(SPATIALCONTEXT_AUTHNAME_PREFIX));

        // NOTE: for some unknown scary reason, we get unexpected garbage at the end of the AuthName,
        //    even though the authname appears valid in the RDBMS, so we need to trim it:
        spatialContextName = spatialContextName.Left(SPATIALCONTEXT_AUTHNAME_SUFFIX);
    }
    else  // use SRID as name
    {
        LONG lSRID = -1;
        lResult = SE_spatialrefinfo_get_srid(spatialRefInfo, &lSRID);
        handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_TO_READ_SRS, "Failed to get or set information for this ArcSDE Spatial Reference System.");

        wchar_t wBuffer[50];
        spatialContextName = FdoCommonOSUtil::ltow(lSRID, wBuffer, 50);
    }

    return spatialContextName;
}

