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
#include "ArcSDECreateSpatialContext.h"
#include "ArcSDEUtils.h"


ArcSDECreateSpatialContext::ArcSDECreateSpatialContext(ArcSDEConnection *connection) :
    ArcSDECommand<FdoICreateSpatialContext>(connection),
    m_bUpdateExisting(false),
    m_bDescriptionSet(false),
    m_bCoordinateSystemSet(false),
    m_bCoordinateSystemWktSet(false),
    m_bExtentTypeSet(false),
    m_eExtentType(FdoSpatialContextExtentType_Static),
    m_bExtentSet(false),
    m_bXYToleranceSet(false),
    m_bZToleranceSet(false),
    m_dZTolerance(0.001)
{
}

ArcSDECreateSpatialContext::~ArcSDECreateSpatialContext(void)
{
}

FdoString* ArcSDECreateSpatialContext::GetName()
{
    return m_wcsName;
}

void ArcSDECreateSpatialContext::SetName(FdoString* value)
{
    m_wcsName = value;
}

FdoString* ArcSDECreateSpatialContext::GetDescription()
{
    return m_wcsDescription;
}

void ArcSDECreateSpatialContext::SetDescription(FdoString* value)
{
    m_wcsDescription = value;
    m_bDescriptionSet = true;
}

FdoString* ArcSDECreateSpatialContext::GetCoordinateSystem()
{
    return m_wcsCoordSys;
}

void ArcSDECreateSpatialContext::SetCoordinateSystem(FdoString* value)
{
    m_wcsCoordSys = value;
    m_bCoordinateSystemSet = true;
}

FdoString* ArcSDECreateSpatialContext::GetCoordinateSystemWkt()
{
    return m_wcsCoordSysWkt;
}

void ArcSDECreateSpatialContext::SetCoordinateSystemWkt(FdoString* value)
{
    m_wcsCoordSysWkt = value;
    m_bCoordinateSystemWktSet = true;
}

FdoSpatialContextExtentType ArcSDECreateSpatialContext::GetExtentType()
{
    return m_eExtentType;
}

void ArcSDECreateSpatialContext::SetExtentType(FdoSpatialContextExtentType value)
{
    m_eExtentType = value;
    m_bExtentTypeSet = true;
}

FdoByteArray* ArcSDECreateSpatialContext::GetExtent()
{
    return FDO_SAFE_ADDREF(m_Extent.p);
}

void ArcSDECreateSpatialContext::SetExtent(FdoByteArray* value)
{
    m_Extent = FDO_SAFE_ADDREF(value);
    m_bExtentSet = true;
}

const double ArcSDECreateSpatialContext::GetXYTolerance()
{
    return m_dXYTolerance;
}

void ArcSDECreateSpatialContext::SetXYTolerance(const double value)
{
    m_dXYTolerance = value;
    m_bXYToleranceSet = true;
}

const double ArcSDECreateSpatialContext::GetZTolerance()
{
    return m_dZTolerance;
}

void ArcSDECreateSpatialContext::SetZTolerance(const double value)
{
    m_dZTolerance = value;
    m_bZToleranceSet = true;
}

const bool ArcSDECreateSpatialContext::GetUpdateExisting()
{
    return m_bUpdateExisting;
}

void ArcSDECreateSpatialContext::SetUpdateExisting(const bool value)
{
    m_bUpdateExisting = value;
}

void ArcSDECreateSpatialContext::Execute()
{
    FdoPtr<ArcSDEConnection> connection;
    LONG lResult = SE_SUCCESS;
    SE_SPATIALREFINFO  seSpatialRefInfo;
    SE_COORDREF seCoordRef;
    long lCoordsysId = -1;
    wchar_t wAuthName[SE_MAX_SPATIALREF_AUTHNAME_LEN];

    // verify the connection
    connection = static_cast<ArcSDEConnection*>(GetConnection ());
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established."));

    // Validate name:
    if (wcslen(m_wcsName)==0)
        throw FdoCommandException::Create(NlsMsgGet(ARCSDE_SPATIALCONTEXT_NAME_NOT_SPECIFIED, "Spatial context name not specified."));

    // Validate that no spatial context of the same name already exists:
    if (!m_bUpdateExisting)
    {
        FdoPtr<FdoIGetSpatialContexts> getSCcmd = (FdoIGetSpatialContexts*)connection->CreateCommand(FdoCommandType_GetSpatialContexts);
        FdoPtr<FdoISpatialContextReader> reader = getSCcmd->Execute();
        while (reader->ReadNext())
            if (0==wcscmp(reader->GetName(), m_wcsName))
                throw FdoCommandException::Create(NlsMsgGet1(ARCSDE_SPATIALCONTEXT_NAME_ALREADY_EXISTS, "A spatial context of name '%1$ls' already exists.", (const wchar_t*)m_wcsName));
    }

    SE_CONNECTION connectionArcSDE = connection->GetConnection();

    // Create SE_SPATIALREFINFO and SE_COORDREF objects:
    lResult = SE_spatialrefinfo_create(&seSpatialRefInfo);
    handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_CREATE_OBJECT_FAILED, "Failed to create ArcSDE object '%1$ls'.", L"SE_SPATIALREFINFO");
    lResult = SE_coordref_create(&seCoordRef);
    handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_CREATE_OBJECT_FAILED, "Failed to create ArcSDE object '%1$ls'.", L"SE_COORDREF");

    if (m_bUpdateExisting)
    {
        // Retrieve the requested spatial context, by name:
        LONG lSRID = ArcSDESpatialContextUtility::SpatialContextNameToSRID(connection, m_wcsName);
        lResult = SE_spatialref_get_info(connection->GetConnection(), lSRID, seSpatialRefInfo);
        handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_GET_OBJECT_FAILED, "Failed to retrieve ArcSDE object '%1$ls'.", L"SE_SPATIALREFINFO");

        // Retrieve associated SE_COORDREF object:
        lResult = SE_spatialrefinfo_get_coordref(seSpatialRefInfo, seCoordRef);
        handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_GET_OBJECT_FAILED, "Failed to retrieve ArcSDE object '%1$ls'.", L"SE_COORDREF");
    }

    // Set the coordinate system:
    if (m_bCoordinateSystemSet || !m_bUpdateExisting)
    {
        if (!m_wcsCoordSys.IsNumber())
            throw FdoException::Create(NlsMsgGet1(ARCSDE_COORDSYS_ID_NOT_A_NUMBER, "The coordinate system '%1$ls' is not an integer that corresponds to an entry in ArSDE's Projection Engine.", (const wchar_t*)m_wcsCoordSys));

        lCoordsysId = m_wcsCoordSys.ToLong();
        lResult = SE_coordref_set_by_id (seCoordRef, lCoordsysId);
        handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_COORDSYS_ID_NOT_A_NUMBER, "The coordinate system '%1$ls' is not an integer that corresponds to an entry in ArSDE's Projection Engine.", (const wchar_t*)m_wcsCoordSys);
    }

    // Set extent type:
    if (m_bExtentTypeSet || !m_bUpdateExisting)
    {
        // Validate extent type:
        if (m_eExtentType != FdoSpatialContextExtentType_Static)
            throw FdoCommandException::Create(NlsMsgGet(ARCSDE_SPATIALCONTEXT_UNSUPPORTED_EXTENTTYPE, "The given spatial context extent type is not supported."));

        // NOTE: ArcSDE only supports static extents, so nothing to actually set; validation is all we need to do.
    }

    // NOTE: we ignore the given FDO XY tolerance, for 2 reasons:
    //      - ArcSDE forces specifying ONE OF the tolerance+origin OR extents, not both;
    //        whichever one isn't specified is calculated to be the maximum allowed by ArcSDE.
    //      - Extents is more important than tolerance; this is because a user attempting to insert data
    //        within extents that are calculated smaller than expected may get a runtime exception,
    //        while a user attempting to insert data with tolerance that is calculated to be smaller than expected
    //        should still succeed (no runtime exceptions).  To minimize runtime exceptions, we give preference
    //        to specifying accurate extents rather than accurate tolerances.

    // Set extent:
    bool bZToleranceSetViaExtents = false;
    FdoPtr<FdoIEnvelope> extentsEnvelope;
    if (m_bExtentSet || !m_bUpdateExisting)
    {
        // Validate the extent:
        if (m_Extent == NULL)
            throw FdoException::Create (NlsMsgGet (ARCSDE_SPATIALCONTEXT_NEED_EXTENT_TO_CREATE, "Cannot create a spatial context without specifying an extent."));

        // Convert extent FDO ByteArray to ArcSDE SE_ENVELOPE:
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoIPolygon>   geom = (FdoIPolygon *)gf->CreateGeometryFromFgf( m_Extent );
        FdoPtr<FdoILinearRing> extPoly = geom->GetExteriorRing();
        extentsEnvelope = extPoly->GetEnvelope();
        SE_ENVELOPE sdeEnvelope;
        sdeEnvelope.minx = extentsEnvelope->GetMinX();
        sdeEnvelope.miny = extentsEnvelope->GetMinY();
        sdeEnvelope.maxx = extentsEnvelope->GetMaxX();
        sdeEnvelope.maxy = extentsEnvelope->GetMaxY();
        if ((extentsEnvelope->GetMinZ() != extentsEnvelope->GetMaxZ()) && !FdoCommonOSUtil::_isnan(extentsEnvelope->GetMinZ()) && !FdoCommonOSUtil::_isnan(extentsEnvelope->GetMaxZ()))
            bZToleranceSetViaExtents = true;

        // Set xy/z/m offsets:
        lResult = SE_coordref_set_xy_by_envelope(seCoordRef, &sdeEnvelope);
        handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_COORDREF_ERROR, "Unexpected error encountered while manipulating an ArcSDE coordinate reference.");
    }

    // Set Z tolerance:
    if (bZToleranceSetViaExtents)
    {
        lResult =  SE_coordref_set_z_by_range(seCoordRef, extentsEnvelope->GetMinZ(), extentsEnvelope->GetMaxZ());
        handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_COORDREF_ERROR, "Unexpected error encountered while manipulating an ArcSDE coordinate reference.");
    }
    else if (m_bZToleranceSet || !m_bUpdateExisting)
    {
        // Validate Z tolerance:
        if (m_dZTolerance == 0.0)
            throw FdoCommandException::Create(NlsMsgGet(ARCSDE_SPATIALCONTEXT_ZTOLERANCE_ZERO, "A spatial context's Z tolerance can't be zero."));

        lResult =  SE_coordref_set_z(seCoordRef, 0.0, 1.0/m_dZTolerance);
        handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_COORDREF_ERROR, "Unexpected error encountered while manipulating an ArcSDE coordinate reference.");
    }

    // Set M tolerance (no way to retrieve this from FDO API, so we always set it):
    if (!m_bUpdateExisting)
    {
        lResult =  SE_coordref_set_m(seCoordRef, 0.0, 1.0/DEFAULT_M_TOLERANCE);
        handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_COORDREF_ERROR, "Unexpected error encountered while manipulating an ArcSDE coordinate reference.");
    }

    // Put coordinate ref object in spatialrefinfo object:
    lResult = SE_spatialrefinfo_set_coordref(seSpatialRefInfo, seCoordRef);
    handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_SPATIALREF_ERROR, "Unexpected error encountered while manipulating an ArcSDE spatial reference system.");

    // Set description (quietly truncate if too long for ArcSDE):
    // DEFECT 639233: arcsde doesn't like descriptions longer than 23 characters, it triggers problems with retrieving spatial contexts later on.
    if (m_bDescriptionSet || !m_bUpdateExisting)
    {
        // truncate to appropriate length if necessary, leaving room for suffix:
        CHAR* csDescription = NULL;
        wide_to_multibyte(csDescription, (const wchar_t*)m_wcsDescription);
        if (strlen(csDescription) >= (SPATIALCONTEXT_MAX_DESC_LENGTH-wcslen(SPATIALCONTEXT_DESC_SUFFIX)))
        {
            long lPos = SPATIALCONTEXT_MAX_DESC_LENGTH-(long)wcslen(SPATIALCONTEXT_DESC_SUFFIX);
            while (FdoCommonOSUtil::ismbstrail((const unsigned char*)csDescription, (const unsigned char*)csDescription + lPos))
                lPos--;
            csDescription[lPos] = '\0';
        }

        // append suffix:
        CHAR* csSpatialContextDescSuffix = NULL;
        wide_to_multibyte(csSpatialContextDescSuffix, SPATIALCONTEXT_DESC_SUFFIX);
        CHAR csSpatialcontextDescWithSuffix[SE_MAX_DESCRIPTION_LEN+1];
        sprintf(csSpatialcontextDescWithSuffix, "%s%s",csDescription , csSpatialContextDescSuffix);

        // finally set the description:        
        lResult = SE_spatialrefinfo_set_description(seSpatialRefInfo, csSpatialcontextDescWithSuffix);
        handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_SPATIALREF_ERROR, "Unexpected error encountered while manipulating an ArcSDE spatial reference system.");
    }

    // Set name (in user-managed AUTH_NAME field):
    if (!m_bUpdateExisting)
    {
        CHAR *mbAuthName = NULL;
        FdoCommonOSUtil::swprintf(wAuthName, SE_MAX_SPATIALREF_AUTHNAME_LEN, L"%ls%ls%ls", SPATIALCONTEXT_AUTHNAME_PREFIX, (const wchar_t*)m_wcsName, SPATIALCONTEXT_AUTHNAME_SUFFIX);
        wide_to_multibyte(mbAuthName, wAuthName);
        lResult = SE_spatialrefinfo_set_auth_name(seSpatialRefInfo, mbAuthName);
        handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_SPATIALREF_ERROR, "Unexpected error encountered while manipulating an ArcSDE spatial reference system.");
    }

    // Create (or update) spatial reference:
    if (m_bUpdateExisting)
    {
        lResult = SE_spatialref_alter(connectionArcSDE, seSpatialRefInfo);
        handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_SPATIALREF_STORE_ERROR, "Failed to create or update an ArcSDE spatial reference system in the datastore.");

	SE_spatialrefinfo_free(seSpatialRefInfo);
        SE_coordref_free(seCoordRef);
    }
    else
    {
        lResult = SE_spatialref_create(connectionArcSDE, seSpatialRefInfo);
        handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_SPATIALREF_STORE_ERROR, "Failed to create or update an ArcSDE spatial reference system in the datastore.");

        // IMPORTANT: ArcSDE may not have created a spatial context is there is already one that matches the
        //            given coordinate system id + extents + resolution.  So we have to check that this has happened
        //            (by verifying the AUTH_NAME of the given SRID matches our AUTH_NAME), and if so,
        //            complain to the user:

        SE_SPATIALREFINFO  seNewSpatialRefInfo;
        LONG lNewSRID = -1;
        char newAuthName[SE_MAX_SPATIALREF_AUTHNAME_LEN];
        lResult = SE_spatialrefinfo_get_srid(seSpatialRefInfo, &lNewSRID);
        handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_FAILED_TO_READ_SRS, "Failed to get or set information for this ArcSDE Spatial Reference System.");

        SE_spatialrefinfo_free(seSpatialRefInfo);
        SE_coordref_free(seCoordRef);

        lResult = SE_spatialrefinfo_create(&seNewSpatialRefInfo);
        handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_CREATE_OBJECT_FAILED, "Failed to create ArcSDE object '%1$ls'.", L"SE_SPATIALREFINFO");
        lResult = SE_spatialref_get_info(connection->GetConnection(), lNewSRID, seNewSpatialRefInfo);
        handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_GET_OBJECT_FAILED, "Failed to retrieve ArcSDE object '%1$ls'.", L"SE_SPATIALREFINFO");
        lResult = SE_spatialrefinfo_get_auth_name(seNewSpatialRefInfo, newAuthName);
        handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_FAILED_TO_READ_SRS, "Failed to get or set information for this ArcSDE Spatial Reference System.");
        FdoStringP newSpatialContextName = ArcSDESpatialContextUtility::GetSpatialContextName(seNewSpatialRefInfo);
        SE_spatialrefinfo_free(seNewSpatialRefInfo);
        // Compare the new auth_name to the given auth_name:
        wchar_t *wNewAuthName = NULL;
        multibyte_to_wide(wNewAuthName, newAuthName);
        if (0 != wcsncmp(wAuthName, wNewAuthName, wcslen(wAuthName)))
            throw FdoException::Create(NlsMsgGet2(ARCSDE_SIMILAR_SPATIAL_CONTEXT_EXISTS, "The spatial context '%1$ls' being created matches the coordinate system, extents, and resolution of existing spatial context '%2$ls'; ArcSDE does not allow this.", (const wchar_t*)m_wcsName, (const wchar_t*)newSpatialContextName));
    }
}

