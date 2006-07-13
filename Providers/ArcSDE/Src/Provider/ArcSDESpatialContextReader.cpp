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
#include "ArcSDESpatialContextReader.h"


ArcSDESpatialContextReader::ArcSDESpatialContextReader(ArcSDEConnection* connection, SE_SPATIALREFINFO *arrSpatialRefs, LONG lSpatialRefCount, bool bActiveOnly)
{
    mConnection = FDO_SAFE_ADDREF(connection);
    mArrSpatialRefs = arrSpatialRefs;
    mlSpatialRefCount = lSpatialRefCount;
    mlCurrentSpatialRef = -1L;
    mbActiveOnly = bActiveOnly;
}

ArcSDESpatialContextReader::~ArcSDESpatialContextReader(void)
{
    // clean up:

    // NOTE: the way we allocate the SE_SPATIANREFINFO array is different than the way SE_spatialref_get_info_list()
    //       allocates it, so we need to deallocate them differently:
    if (NULL != mArrSpatialRefs)
    {
        if (mbActiveOnly)
        {
            SE_spatialrefinfo_free(mArrSpatialRefs[0]);
            delete[] mArrSpatialRefs;
        }
        else
        {
            SE_spatialref_free_info_list(mlSpatialRefCount, mArrSpatialRefs);
        }
    }
}

void ArcSDESpatialContextReader::Validate(void)
{
    if (mlCurrentSpatialRef < 0)
        throw FdoException::Create (NlsMsgGet1(ARCSDE_READER_NOT_READY, "Must %1$ls prior to accessing reader.", L"ReadNext"));

    if (mlCurrentSpatialRef >= mlSpatialRefCount)
        throw FdoException::Create (NlsMsgGet (ARCSDE_READER_EXHAUSTED, "Reader is exhausted."));
}

FdoString* ArcSDESpatialContextReader::GetName()
{
    Validate();

    LONG lSRID = -1L;
    LONG lResult = SE_SUCCESS;

    if (mSpatialContextName == L"")
    {
        mSpatialContextName = ArcSDESpatialContextUtility::GetSpatialContextName(mArrSpatialRefs[mlCurrentSpatialRef]);
    }

    return mSpatialContextName;
}

FdoString* ArcSDESpatialContextReader::GetDescription()
{
    Validate();

    LONG lResult = SE_SUCCESS;
    CHAR desc[SE_MAX_DESCRIPTION_LEN];

    if (mDescription == L"")
    {
        lResult = SE_spatialrefinfo_get_description(mArrSpatialRefs[mlCurrentSpatialRef], desc);
        handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_TO_READ_SRS, "Failed to get or set information for this ArcSDE Spatial Reference System.");

        wchar_t *wDesc = NULL;
        multibyte_to_wide(wDesc, desc);
        mDescription = wDesc;

        // NOTE: for some unknown scary reason, we get unexpected garbage at the end of the description,
        //    even though the description appears valid in the RDBMS, so we need to trim it:
        //TODO: see if we can fix this issue!!
        // NOTE: if FdoStringP::Left() doesnt find the given suffix (e.g. in the case of a natively-created spatial reference system),
        //    it returns the entire string unchanged.
        mDescription = mDescription.Left(SPATIALCONTEXT_DESC_SUFFIX);
    }

    return mDescription;
}

FdoString* ArcSDESpatialContextReader::GetCoordinateSystem()
{
    Validate();

    if (mCoordSysName == L"")
    {
        LONG id = -1;
        LONG         lResult = SE_SUCCESS;
        SE_COORDREF  coordref;
        CHAR         coordSysWkt[PE_BUFFER_MAX];

        // Get coordref:
        lResult = SE_coordref_create (&coordref);
        handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_UNEXPECTED_ERROR, "Unexpected error encountered in ArcSDE Provider.");
        lResult = SE_spatialrefinfo_get_coordref(mArrSpatialRefs[mlCurrentSpatialRef], coordref);
        handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_TO_READ_SRS, "Failed to get or set information for this ArcSDE Spatial Reference System.");

        // Get coordref's serialized 'description' (its datum, spheroid, projection, etc):
        lResult = SE_coordref_get_description(coordref, coordSysWkt);
        handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_TO_READ_SRS, "Failed to get or set information for this ArcSDE Spatial Reference System.");

        // Get coordref's id, then store it in mCoordSysName:
        // ECO 10353b "Specify consistent handling of unknown coordinate systems":
        if (0==strcmp(coordSysWkt, SPATIALCONTEXT_COORDYS_WKT_UNKNOWN_SDE))
        {
            mCoordSysName = SPATIALCONTEXT_CORDSYS_NAME_UNKNOWN_FDO;
        }
        else
        {
            lResult = SE_coordref_get_id(coordref, &id);
            handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_TO_READ_SRS, "Failed to get or set information for this ArcSDE Spatial Reference System.");
            wchar_t wBuffer[20];
            mCoordSysName = FdoCommonOSUtil::ltow(id, wBuffer, ELEMENTS(wBuffer));
        }

        // Cleanup:
        SE_coordref_free(coordref);
    }

    return mCoordSysName;
}


FdoString* ArcSDESpatialContextReader::GetCoordinateSystemWkt()
{
    Validate();

    LONG         lResult = SE_SUCCESS;
    SE_COORDREF  coordref;
    CHAR         coordSysWkt[PE_BUFFER_MAX];

    if (mCoordSysWkt == L"")
    {
        // Get coordref:
        lResult = SE_coordref_create (&coordref);
        handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_UNEXPECTED_ERROR, "Unexpected error encountered in ArcSDE Provider.");
        lResult = SE_spatialrefinfo_get_coordref(mArrSpatialRefs[mlCurrentSpatialRef], coordref);
        handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_TO_READ_SRS, "Failed to get or set information for this ArcSDE Spatial Reference System.");

        // Get coordref's serialized 'description' (its datum, spheroid, projection, etc):
        lResult = SE_coordref_get_description(coordref, coordSysWkt);
        handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_TO_READ_SRS, "Failed to get or set information for this ArcSDE Spatial Reference System.");

        // ECO 10353b "Specify consistent handling of unknown coordinate systems":
        if (0==strcmp(coordSysWkt, SPATIALCONTEXT_COORDYS_WKT_UNKNOWN_SDE))
        {
            mCoordSysWkt = SPATIALCONTEXT_CORDSYS_WKT_UNKNOWN_FDO;
        }
        else
        {
            wchar_t *wCoordSys = NULL;
            multibyte_to_wide(wCoordSys, coordSysWkt);
            mCoordSysWkt = wCoordSys;
        }

        // Cleanup:
        SE_coordref_free(coordref);
    }

    return mCoordSysWkt;
}

FdoSpatialContextExtentType ArcSDESpatialContextReader::GetExtentType()
{
    Validate();

    return FdoSpatialContextExtentType_Static;  //ok
}

FdoByteArray* ArcSDESpatialContextReader::GetExtent()
{
    Validate();

    LONG         lResult = SE_SUCCESS;
    SE_COORDREF  coordref;
    SE_ENVELOPE  envelope;

    // Get coordref:
    lResult = SE_coordref_create (&coordref);
    handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_UNEXPECTED_ERROR, "Unexpected error encountered in ArcSDE Provider.");
    lResult = SE_spatialrefinfo_get_coordref(mArrSpatialRefs[mlCurrentSpatialRef], coordref);
    handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_TO_READ_SRS, "Failed to get or set information for this ArcSDE Spatial Reference System.");

    // Get coordref's XY envelope:
    lResult = SE_coordref_get_xy_envelope(coordref, &envelope);
    handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_UNEXPECTED_ERROR, "Unexpected error encountered in ArcSDE Provider.");

    // Convert SE_ENVELOP to ordinate array:
    FdoByteArray* byteArray = ArcSDESpatialContextUtility::EnvelopeToFgf(envelope);

    // Cleanup:
    SE_coordref_free(coordref);

    // Return byte array:
    return byteArray;
}

const double ArcSDESpatialContextReader::GetXYTolerance()
{
    Validate();

    LONG         lResult = SE_SUCCESS;
    SE_COORDREF  coordref;

    // Get coordref:
    lResult = SE_coordref_create (&coordref);
    handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_UNEXPECTED_ERROR, "Unexpected error encountered in ArcSDE Provider.");
    lResult = SE_spatialrefinfo_get_coordref(mArrSpatialRefs[mlCurrentSpatialRef], coordref);
    handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_TO_READ_SRS, "Failed to get or set information for this ArcSDE Spatial Reference System.");

    // Get coordref's XY units:
    LFLOAT xOffset = 0.0;
    LFLOAT yOffset = 0.0;
    LFLOAT xyUnits = 0.0;
    lResult = SE_coordref_get_xy(coordref, &xOffset, &yOffset, &xyUnits);

    // Convert ArcSDE XY units to FDO xy tolerance:
    double fdoXYTolerance = 1.0 / xyUnits;

    // Cleanup:
    SE_coordref_free(coordref);

    return fdoXYTolerance;
}

const double ArcSDESpatialContextReader::GetZTolerance()
{
    Validate();

    LONG         lResult = SE_SUCCESS;
    SE_COORDREF  coordref;

    // Get coordref:
    lResult = SE_coordref_create (&coordref);
    handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_UNEXPECTED_ERROR, "Unexpected error encountered in ArcSDE Provider.");
    lResult = SE_spatialrefinfo_get_coordref(mArrSpatialRefs[mlCurrentSpatialRef], coordref);
    handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_TO_READ_SRS, "Failed to get or set information for this ArcSDE Spatial Reference System.");

    // Get coordref's Z units:
    LFLOAT zOffset = 0.0;
    LFLOAT zUnits = 0.0;
    lResult = SE_coordref_get_z(coordref, &zOffset, &zUnits);

    // Convert ArcSDE XY units to FDO xy tolerance:
    double fdoZTolerance = 1.0 / zUnits;

    // Cleanup:
    SE_coordref_free(coordref);

    return fdoZTolerance;
}

const bool ArcSDESpatialContextReader::IsActive()
{
    Validate();

    FdoString *currentName = GetName();
    FdoString *activeName = mConnection->GetActiveSpatialContext();

    return (activeName==NULL) ? false : (0==wcscmp(currentName, activeName));
}

bool ArcSDESpatialContextReader::ReadNext()
{
    mlCurrentSpatialRef++;

    // de-cache all cached data:
    mSpatialContextName = (FdoString*)NULL;
    mDescription = (FdoString*)NULL;
    mCoordSysWkt = (FdoString*)NULL;
    mCoordSysName = (FdoString*)NULL;

    return (mlCurrentSpatialRef < mlSpatialRefCount);
}

void ArcSDESpatialContextReader::Dispose()
{
    delete this;
}

long ArcSDESpatialContextReader::GetSRID()
{
    Validate();

    LONG lSRID = -1L;
    LONG lResult = SE_SUCCESS;

    lResult = SE_spatialrefinfo_get_srid(mArrSpatialRefs[mlCurrentSpatialRef], &lSRID);
    handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_TO_READ_SRS, "Failed to get or set information for this ArcSDE Spatial Reference System.");

    return lSRID;
}


