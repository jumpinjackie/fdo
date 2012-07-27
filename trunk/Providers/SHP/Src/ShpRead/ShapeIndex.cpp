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
#include "ShapeIndex.h"
#include "Constants.h"
#include <limits>

using namespace std;

/*****************************************************************************
 * Name         : ShapeIndex
 *
 * Description  : Constructor
 *
 * Parameters   : const WCHAR* wszFilename
 *                int&         nStatus
 *
 * Return       : N/A
 *
 * Notes        : N/A
 *
 *****************************************************************************/
ShapeIndex::ShapeIndex(const WCHAR* wszFilename)
{
    _FDORPT0(0, "Creating ShapeIndex object\n");

    ClearRowIndexCache();

    OpenRead (wszFilename);
}

ShapeIndex::ShapeIndex (const WCHAR* wszFilename, ShapeFile* shp, FdoString *tempDir)
{
    int count;
    ULONG offset;
    ULONG next;
    int record;
    SHPIndexRecordHeader header;

    _FDORPT0(0, "Creating new ShapeIndex object\n");

    ClearRowIndexCache();

    try
    {
        OpenWrite (wszFilename, shp->GetFileShapeType (), shp->HasMData ());
    }
    catch (FdoException *e)
    {
        e->Release();

        // Try again in temp folder:
        wchar_t* temp;
        FdoCommonFile::GetTempFile(&temp, tempDir);
        OpenWrite (temp, shp->GetFileShapeType (), shp->HasMData ());
        delete[] temp;
        SetTemporaryFile(true);
    }
    SetHeaderDirty (true);
	SetBoundingBoxMinX (shp->GetBoundingBoxMinX ());
	SetBoundingBoxMaxX (shp->GetBoundingBoxMaxX ());
	SetBoundingBoxMinY (shp->GetBoundingBoxMinY ());
	SetBoundingBoxMaxY (shp->GetBoundingBoxMaxY ());
	SetBoundingBoxMinZ (shp->GetBoundingBoxMinZ ());
	SetBoundingBoxMaxZ (shp->GetBoundingBoxMaxZ ());
	SetBoundingBoxMinM (shp->GetBoundingBoxMinM ());
	SetBoundingBoxMaxM (shp->GetBoundingBoxMaxM ());

    count = 0;
    offset = SHPHeaderSize;
    while (true)
    {
        if (SHP_OK == shp->ReadRecordHeader (offset, next, record))
        {
            // These are stored in BigEndian so they must be swapped
            header.nOffset = SWAPLONG(offset / WORD_SIZE_IN_BYTES);
            header.nContentLength = SWAPLONG((next - sizeof(SHPRecordHeader)) / WORD_SIZE_IN_BYTES);
            if (!WriteFile (&header, sizeof(SHPIndexRecordHeader)))
                throw FdoCommonFile::LastErrorToException (L"ShapeIndex::ShapeIndex(WriteHeader)");
            offset += next;
            count++;
        }
        else
            break;
    }
    m_nFileLength += count * 4; // 8 bytes per entry
    PutFileHeaderDetails ();

	// Close and reopen for Read only
	Reopen(FdoCommonFile::IDF_OPEN_READ );
}

/*****************************************************************************
 * Name         : ~ShapeIndex
 *
 * Description  : Destructor
 *
 * Parameters   :
 *
 * Return       : N/A
 *
 * Notes        : N/A
 *
 *****************************************************************************/
ShapeIndex::~ShapeIndex()
{
    _FDORPT0(0, "Deleting ShapeIndex object\n");
}

/*****************************************************************************
 * Name         : ReadRecordHeader
 *
 * Description  : This method reads the record header.
 *
 * Parameters   : ULONG ulIndexOffset
 *                int&  nOffset
 *                int&  nContentLength
 *
 * Return       : int
 *
 * Notes        : N/A
 *
 *****************************************************************************/
void ShapeIndex::ReadRecordHeader (ULONG ulIndexOffset, ULONG& nOffset, int& nContentLength)
{
    _FDORPT0(0, "ReadRecordHeader()\n");
    SHPIndexRecordHeader shpIndexRecordHeader;

    if (!SetFilePointer64((FdoInt64)ulIndexOffset))
        throw FdoCommonFile::LastErrorToException (L"ShapeIndex::ReadRecordHeader(SetFilePointer64)");
    if (!ReadFile (&shpIndexRecordHeader, sizeof(SHPIndexRecordHeader)))
        throw FdoCommonFile::LastErrorToException (L"ShapeIndex::ReadRecordHeader(ReadHeader)");
    // These are stored in BigEndian so they must be swapped
    nOffset = SWAPLONG(shpIndexRecordHeader.nOffset);
    nContentLength = SWAPLONG(shpIndexRecordHeader.nContentLength);
}

void ShapeIndex::WriteRecordHeader (ULONG ulIndexOffset, ULONG nOffset, int nContentLength)
{
    _FDORPT0(0, "WriteRecordHeader()\n");
    SHPIndexRecordHeader header;

    // Avoid the cache getting stale
    ClearRowIndexCache();

    // These are stored in BigEndian so they must be swapped
    header.nOffset = SWAPLONG(nOffset);
    header.nContentLength = SWAPLONG(nContentLength);
    if (!SetFilePointer64 ((FdoInt64)ulIndexOffset))
        throw FdoCommonFile::LastErrorToException (L"ShapeIndex::WriteRecordHeader(SetFilePointer64)");
    if (!WriteFile (&header, sizeof(SHPIndexRecordHeader)))
        throw FdoCommonFile::LastErrorToException (L"ShapeIndex::WriteRecordHeader(WriteHeader)");
}

/*****************************************************************************
 * Name         : GetObjectAt
 *
 * Description  : This method gets the object at the specified offset.
 *
 * Parameters   : int  nObjectNum
 *                int& nOffset
 *                int& nContentLength
 *
 * Return       : 
 *
 * Notes        : N/A
 *
 *****************************************************************************/
void ShapeIndex::GetObjectAt (int nObjectNum, ULONG& nOffset, int& nContentLength)
{
    _FDORPT0(0, ">>>>>>>>>> GetObjectAt() <<<<<<<<<<\n");
    long nRowsLengthRead;
    int nNumNewRows;

    if (!GetRowIndexFromCache (nObjectNum, nOffset, nContentLength))
    {
        ClearRowIndexCache();

        if ( SetFilePointer64( (FdoInt64)(RECORD_START_OFFSET + nObjectNum * RECORD_HEADER_SIZE) ))
        {
            if (!ReadFile (m_szRowBuffer, SHP_FILE_READ_CACHE_SIZE * RECORD_HEADER_SIZE, &nRowsLengthRead))
                throw FdoCommonFile::LastErrorToException (L"ShapeIndex::GetObjectAt(ReadHeaders)");
            nNumNewRows = nRowsLengthRead /sizeof(SHPIndexRecordHeader);

            // Refresh the cache
            m_nFirstCacheRecordId = nObjectNum;
            m_nLastCacheRecordId = nObjectNum + nNumNewRows - 1;

            // Check again the cache. 
            if ( !GetRowIndexFromCache( nObjectNum, nOffset, nContentLength ) )
                throw FdoException::Create (NlsMsgGet(SHP_END_OF_FILE_ERROR, "End of file occured reading shape at offset %1$ld for file '%2$ls'.", RECORD_START_OFFSET + nObjectNum * RECORD_HEADER_SIZE, FileName ()));
        }
    }
}


void ShapeIndex::SetObjectAt (int nObjectNum, ULONG nOffset, int nContentLength, bool batch)
{
    bool append;

    append = nObjectNum >= GetNumObjects ();
    WriteRecordHeader (
        append ?
            m_nFileLength * WORD_SIZE_IN_BYTES :
            RECORD_START_OFFSET + (nObjectNum * RECORD_HEADER_SIZE),
        nOffset / WORD_SIZE_IN_BYTES, 
        nContentLength / WORD_SIZE_IN_BYTES);
    // update the file length (the bounding box is updated with help from the R-Tree)
    if (append)
    {
        m_nFileLength += sizeof(SHPIndexRecordHeader) / WORD_SIZE_IN_BYTES;
        SetHeaderDirty (true);
        if (!batch)
            PutFileHeaderDetails ();
    }

    // Avoid the cache getting stale
    ClearRowIndexCache();
}

/*****************************************************************************
 * Name         : GetNumObjects
 *
 * Description  : This method gets the # of objects in the SHP file.
 *
 * Parameters   :
 *
 * Return       : int
 *
 * Notes        : N/A
 *
 *****************************************************************************/
int ShapeIndex::GetNumObjects()
{
    return ((m_nFileLength * WORD_SIZE_IN_BYTES) - RECORD_START_OFFSET) / RECORD_HEADER_SIZE;
}

/*****************************************************************************
 * Name         : GetRowIndexFromCache
 *
 * Description  : This method gets the specified record # data from the cache
 *                if it is there.
 *
 * Parameters   : int nRecord
 *
 * Return       : true if found, false otherwise
 *
 * Notes        : Records start with 0
 *
 *****************************************************************************/
bool ShapeIndex::GetRowIndexFromCache(int nRecord, ULONG& nOffset, int& nContentLength)
{
    bool    found = false;

    if ( nRecord >= m_nFirstCacheRecordId && nRecord <= m_nLastCacheRecordId )
    {
        long rawOffset = (nRecord - m_nFirstCacheRecordId);
        SHPIndexRecordHeader* rowIndex = &m_szRowBuffer[rawOffset]; 
                                
        nOffset = SWAPLONG(rowIndex->nOffset) * WORD_SIZE_IN_BYTES;
        nContentLength = SWAPLONG(rowIndex->nContentLength) * WORD_SIZE_IN_BYTES;

        found = true;
    }

    return found;
}

/*****************************************************************************/
void ShapeIndex::ClearRowIndexCache()
{
    m_nFirstCacheRecordId = 0;
    m_nLastCacheRecordId = -1;
}


