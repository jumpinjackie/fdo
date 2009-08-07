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
#include "DoublePoint.h"
#include "BoundingBox.h"
#include "Constants.h"
#include "ShapeFileBase.h"
#include <FdoCommonOSUtil.h>

#include <limits>
#ifndef _WIN32
#include <math.h>
#endif

using namespace std;

ShapeFileBase::ShapeFileBase ()
{
    // Initialize
    m_nFileCode = 0;
    m_nFileLength = 0;
    m_nFileVersion = 0;
    m_nFileShapeType = eNullShape;
    m_bMDataPresent = false;

    m_dXMin = 0.0;
    m_dXMax = 0.0;
    m_dYMin = 0.0;
    m_dYMax = 0.0;
    m_dZMin = 0.0;
    m_dZMax = 0.0;
    m_dMMin = 0.0;
    m_dMMax = 0.0;
}

ShapeFileBase::~ShapeFileBase (void)
{
}

void ShapeFileBase::OpenRead (const wchar_t* name)  
{
    ErrorCode code;

    if (OpenFile (name, IDF_OPEN_READ, code)) { 
        GetFileHeaderDetails ();
	}
    else
        throw FdoCommonFile::ErrorCodeToException (code, name, IDF_OPEN_READ);
}

void ShapeFileBase::OpenWrite (const wchar_t* name, eShapeTypes shape_type, bool has_m)
{
    ErrorCode code;

    _FDORPT0(0, "Creating new ShapeFile object\n");

    // Initialize
    m_nFileCode = nSHP_FILE_CODE;
    SetHeaderDirty (true);
    m_nFileLength = SHPHeaderSize / 2; // size in 16 bit words
    m_nFileVersion = nSHP_FILE_VERSION;
    m_nFileShapeType = shape_type;
    m_bMDataPresent = has_m;
    m_dXMin = fNO_DATA;
    m_dXMax = fNO_DATA;
    m_dYMin = fNO_DATA;
    m_dYMax = fNO_DATA;
    m_dZMin = fNO_DATA;
    m_dZMax = fNO_DATA;
    m_dMMin = fNO_DATA;
    m_dMMax = fNO_DATA;
	OpenFlags flags = IDF_OPEN_UPDATE;

	if ( !FileExists (name) )
		flags = (OpenFlags) (flags | IDF_CREATE_NEW);

	if (OpenFile (name, flags, code)) {
        PutFileHeaderDetails ();
	}
    else
        throw FdoCommonFile::ErrorCodeToException (code, name, (OpenFlags)(IDF_OPEN_UPDATE));
}

void ShapeFileBase::Reopen(OpenFlags openFlags)
{
	ErrorCode code;

    _FDORPT0(0, "Reopening a SHP file without recreating the object\n");

	if (openFlags & IDF_OPEN_READ)
	{
		if ( !OpenFile (FileName(), IDF_OPEN_READ, code) )
			throw FdoCommonFile::ErrorCodeToException (code, FileName(), IDF_OPEN_READ);

	}
	else if ( IsReadOnly() && (openFlags & IDF_OPEN_UPDATE) )
	{
		// Check is temporary, othewise dissallow 2 writers
		if (!OpenFile (FileName(), IDF_OPEN_UPDATE, code))
			throw FdoCommonFile::ErrorCodeToException (code, FileName(), IDF_OPEN_UPDATE);

	}
}

/*****************************************************************************
 * Name         : GetFileHeaderDetails
 *
 * Description  : This method gets the file header details.
 *
 * Parameters   :
 *
 * Return       : 
 *
 * Notes        : N/A
 *
 *****************************************************************************/
void ShapeFileBase::GetFileHeaderDetails ()
{
    SHPHeader shpHeader;

    _FDORPT0(0, "GetFileHeaderDetails()\n");

    // Read the File Header details
    if (!ReadFile (&shpHeader, SHPHeaderSize))
        throw FdoCommonFile::LastErrorToException (L"ShapeFileBase::GetFileHeaderDetails");

    // We need to swap the BigEndian values
    m_nFileCode = shpHeader.nFileCode = SWAPLONG(shpHeader.nFileCode);

	// Check file code
	if ( m_nFileCode != nSHP_FILE_CODE )
		throw FdoException::Create (NlsMsgGet(SHP_CONNECTION_FILE_INVALID_FILECODE, "The file '%1$ls' has an invalid File Code number (%2$d).", FileName (), m_nFileCode));

	// Check version
    m_nFileVersion = shpHeader.nFileVersion;

    if(m_nFileVersion == nSHP_FILE_VERSION)
    {
        m_nFileLength = shpHeader.nFileLength = SWAPLONG(shpHeader.nFileLength);
        m_nFileShapeType = (eShapeTypes)shpHeader.nFileShapeType;

        m_dXMin = shpHeader.cXMin;
        m_dYMin = shpHeader.cYMin;
        if (m_nFileLength * WORD_SIZE_IN_BYTES > SHPHeaderSize) // only check if there are shapes
            CheckBoundingBox(m_dXMin, m_dYMin, eMinXMinY);
        m_dXMax = shpHeader.cXMax;
        m_dYMax = shpHeader.cYMax;
        if (m_nFileLength * WORD_SIZE_IN_BYTES > SHPHeaderSize) // only check if there are shapes
            CheckBoundingBox(m_dXMax, m_dYMax, eMaxXMaxY);
        // Initialize
        m_dZMin = shpHeader.cZMin;
        m_dZMax = shpHeader.cZMax;
        m_dMMin = shpHeader.cMMin;
        m_dMMax = shpHeader.cMMax;

        if ((m_nFileShapeType == ePointZShape) ||
            (m_nFileShapeType == ePolylineZShape) ||
            (m_nFileShapeType == ePolygonZShape) ||
            (m_nFileShapeType == eMultiPointZShape))
        {

#if 0
            // Some SHP file use different values for 'no-data' indicator rather than fNO_DATA.
            // Skip the validation test since a) such value is not standard and b) the Z bounding box 
            // values are not used.
            if (m_nFileLength * WORD_SIZE_IN_BYTES > SHPHeaderSize) // only check if there are shapes
                CheckBoundingBox(m_dZMin, m_dZMax, eMinZMaxZ);
#endif
            // In the SHP specification there is a note about the M bounding box: if M values are not used
            // then Mmin and Mmax are 0.0. In such situation, m_bMdataPresent should not be set to true. 
            if ((m_dMMin > fNO_DATA) && (m_dMMax > fNO_DATA) && (m_dMMin != 0.0) && (m_dMMax != 0.0))
            {
                m_bMDataPresent = true;
                if (m_nFileLength * WORD_SIZE_IN_BYTES > SHPHeaderSize) // only check if there are shapes
                    CheckBoundingBox(m_dMMin, m_dMMax, eMinMMaxM);
            }
            else
                m_bMDataPresent = false;
        }
        if ((m_nFileShapeType == ePointMShape) ||
            (m_nFileShapeType == ePolylineMShape) ||
            (m_nFileShapeType == ePolygonMShape) ||
            (m_nFileShapeType == eMultiPointMShape))
        {
#if 0
            // Same issue as with Z values validation. See comment above.
            if (m_nFileLength * WORD_SIZE_IN_BYTES > SHPHeaderSize) // only check if there are shapes
                CheckBoundingBox(m_dMMin, m_dMMax, eMinMMaxM);
#endif
            m_bMDataPresent = true; // redundant
        }
    }
    else
        throw FdoException::Create (NlsMsgGet(SHP_CONNECTION_FILE_INVALID_VERSION, "The file '%1$ls' has an invalid version number (%2$d).", FileName (), m_nFileVersion));
}

/*****************************************************************************
 * Name         : PutFileHeaderDetails
 *
 * Description  : This method writes the file header details.
 *                It is up to the caller to ensure that member variables are
 *                correctly set. In particular file length (in 16 bit words),
 *                and bounding box values (as "no data" values if appropriate
 *                for an M-less or Z-less shape type.
 *
 * Parameters   :
 *
 * Return       : int
 *
 * Notes        : N/A
 *
 *****************************************************************************/
void ShapeFileBase::PutFileHeaderDetails ()
{
    SHPHeader header;

    _FDORPT0(0, "PutFileHeaderDetails()\n");

    memset(&header, 0, sizeof(SHPHeader));
    header.nFileCode = SWAPLONG(m_nFileCode);
    header.nFileLength = SWAPLONG(m_nFileLength);
    header.nFileVersion = nSHP_FILE_VERSION;
    header.nFileShapeType = m_nFileShapeType;
    header.cXMin = m_dXMin;
    header.cYMin = m_dYMin;
    header.cXMax = m_dXMax;
    header.cYMax = m_dYMax;
    header.cZMin = m_dZMin;
    header.cZMax = m_dZMax;
    header.cMMin = m_dMMin;
    header.cMMax = m_dMMax;
    if (SetFilePointer64 ((FdoInt64)0))
        if (WriteFile (&header, SHPHeaderSize))
            SetHeaderDirty (false);
        else
            throw FdoCommonFile::LastErrorToException (L"ShapeFileBase::PutFileHeaderDetails(WriteHeader)");
    else
        throw FdoCommonFile::LastErrorToException (L"ShapeFileBase::PutFileHeaderDetails(SetFilePointer64)");
}

/*****************************************************************************
 * Name         : CheckBoundingBox
 *
 * Description  : This method checks the bounding box values for validity.
 *
 * Parameters   : double dMinValue
 *                double dMaxValue
 *                eMinMaxTypes eMinMaxType
 *
 * Return       : int
 *
 * Notes        : N/A
 *
 *****************************************************************************/
void ShapeFileBase::CheckBoundingBox(double dMinValue, double dMaxValue, eMinMaxTypes eMinMaxType)
{
    _FDORPT0(0, "CheckBoundingBox()\n");

    // NOTE: it is perfectly acceptable for a file to have a NULL bounding box *and* contain rows of data
    //       this can happen if (A) the file is a DBF file with no corresponding SHP file, or if (B) a SHP file
    //       has only NULL geometry values in it.  Therefore, we accept the values fNO_DATA as a bounding box
    //       even if the SHP/DBF file contain 1+ rows.

    if((dMinValue < fNO_DATA) ||
       (FdoCommonOSUtil::_isnan(dMinValue)) ||
#ifdef _WIN32
       (dMinValue == numeric_limits<double>::infinity()))
#else
       (dMinValue == INFINITY))
#endif
    {
        // Check to see if data values are "No Data"
        switch (eMinMaxType)
        {
            case eMinXMinY:
                throw FdoException::Create (NlsMsgGet(SHP_CONNECTION_FILE_INVALID_BOUNDING_BOX, "The file '%1$ls' has an invalid bounding box (%2$ls).", FileName (), L"minx"));
                break;
            case eMaxXMaxY:
                throw FdoException::Create (NlsMsgGet(SHP_CONNECTION_FILE_INVALID_BOUNDING_BOX, "The file '%1$ls' has an invalid bounding box (%2$ls).", FileName (), L"maxx"));
                break;
            case eMinZMaxZ:
                throw FdoException::Create (NlsMsgGet(SHP_CONNECTION_FILE_INVALID_BOUNDING_BOX, "The file '%1$ls' has an invalid bounding box (%2$ls).", FileName (), L"minz"));
                break;
            case eMinMMaxM:
                throw FdoException::Create (NlsMsgGet(SHP_CONNECTION_FILE_INVALID_BOUNDING_BOX, "The file '%1$ls' has an invalid bounding box (%2$ls).", FileName (), L"minm"));
                break;
            default:
                throw FdoException::Create (NlsMsgGet(SHP_UNEXPECTED_ERROR, "Unexpected error '%1$ls'.", L"invalid eMinMaxType"));
                break;
        }
    }
    else if((dMaxValue < fNO_DATA) ||
            (FdoCommonOSUtil::_isnan(dMaxValue)) ||
#ifdef _WIN32
            (dMaxValue == numeric_limits<double>::infinity()))
#else
            (dMaxValue == INFINITY))
#endif
    {
        // Check to see if data values are "No Data"
        switch(eMinMaxType)
        {
            case eMinXMinY:
                throw FdoException::Create (NlsMsgGet(SHP_CONNECTION_FILE_INVALID_BOUNDING_BOX, "The file '%1$ls' has an invalid bounding box (%2$ls).", FileName (), L"miny"));
                break;
            case eMaxXMaxY:
                throw FdoException::Create (NlsMsgGet(SHP_CONNECTION_FILE_INVALID_BOUNDING_BOX, "The file '%1$ls' has an invalid bounding box (%2$ls).", FileName (), L"maxy"));
                break;
            case eMinZMaxZ:
                throw FdoException::Create (NlsMsgGet(SHP_CONNECTION_FILE_INVALID_BOUNDING_BOX, "The file '%1$ls' has an invalid bounding box (%2$ls).", FileName (), L"maxz"));
                break;
            case eMinMMaxM:
                throw FdoException::Create (NlsMsgGet(SHP_CONNECTION_FILE_INVALID_BOUNDING_BOX, "The file '%1$ls' has an invalid bounding box (%2$ls).", FileName (), L"maxm"));
                break;
            default:
                throw FdoException::Create (NlsMsgGet(SHP_UNEXPECTED_ERROR, "Unexpected error '%1$ls'.", L"invalid eMinMaxType"));
                break;
        }
    }
}

const wchar_t* ShapeFileBase::ShapeTypeToString (eShapeTypes type)
{
    const wchar_t* ret;

    switch (type)
    {
        case eNullShape:
            ret = L"Null Shape";
            break;
        case ePointShape:
            ret = L"Point";
            break;
        case ePolylineShape:
            ret = L"PolyLine";
            break;
        case ePolygonShape:
            ret = L"Polygon";
            break;
        case eMultiPointShape:
            ret = L"MultiPoint";
            break;
        case ePointZShape:
            ret = L"PointZ";
            break;
        case ePolylineZShape:
            ret = L"PolyLineZ";
            break;
        case ePolygonZShape:
            ret = L"PolygonZ";
            break;
        case eMultiPointZShape:
            ret = L"MultiPointZ";
            break;
        case ePointMShape:
            ret = L"PointM";
            break;
        case ePolylineMShape:
            ret = L"PolyLineM";
            break;
        case ePolygonMShape:
            ret = L"PolygonM";
            break;
        case eMultiPointMShape:
            ret = L"MultiPointM";
            break;
        case eMultiPatchShape:
            ret = L"MultiPatch";
            break;
        default:
            ret = L"Unknown";
    }

    return (ret);
}

