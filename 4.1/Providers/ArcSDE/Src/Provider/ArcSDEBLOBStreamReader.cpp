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
#include "ArcSDEBLOBStreamReader.h"

ArcSDEBLOBStreamReader::ArcSDEBLOBStreamReader(SE_BLOB_INFO *blobInfo) :
    m_BlobInfo(blobInfo),
    m_BlobIndex(0L)
{
}

ArcSDEBLOBStreamReader::~ArcSDEBLOBStreamReader(void)
{
    // Clean up:
    SE_blob_free(m_BlobInfo);
    delete m_BlobInfo;
    m_BlobInfo = NULL;
}


/// <summary>Get the size of a stream.</summary>
/// <returns>Returns the size of the data source in number of items </returns> 
FdoInt64   ArcSDEBLOBStreamReader::GetLength()
{
    return m_BlobInfo->blob_length;
}

/// <summary> Skips a number of items </summary>
/// <param name="offset">Number of items to skip. Must be a positive number </param>
/// <returns>Returns nothing </returns>
void       ArcSDEBLOBStreamReader::Skip(const FdoInt32 offset)
{
    // Validate offset:
    if (offset < 0)
        throw FdoCommandException::Create(NlsMsgGet1(ARCSDE_OFFSET_NEGATIVE, "The given stream offset '%1$d' is negative.", offset));
    if ((m_BlobIndex + offset) >= m_BlobInfo->blob_length)
        throw FdoCommandException::Create(NlsMsgGet1(ARCSDE_OFFSET_TOO_LARGE, "The given stream offset '%1$d' is too large.", offset));

    // Skip:
    m_BlobIndex += offset;
}

/// <summary> Get the current index in the stream </summary>
/// <returns>Returns the current index in the stream in number of items </returns> 
FdoInt64   ArcSDEBLOBStreamReader::GetIndex()
{
    return m_BlobIndex;
}

/// <summary> Reset the current index in the stream. Allows re-reading </summary>
/// <returns>Returns nothing </returns> 
void       ArcSDEBLOBStreamReader::Reset()
{
    m_BlobIndex = 0L;
}
    
/// <summary> Reads in the next block of items. Use ReadNext( buffer) to read in 
/// the entire stream. </summary>
/// <param name="buffer"> Buffer holding data read in </param> 
/// <param name="offset"> Index in the buffer indicating the beginning of the output buffer. 
/// The caller is responsable for allocating a buffer of a proper size. </param> 
/// <param name="count">  Number of items to be read in. If -1 read the entire stream.
/// Throws "invalid parameter"  exception if not a positive value or -1.</param> 
/// <returns> Returns the number of items actually read in. When 0 (zero) then the 
/// end-of-stream was reached. </returns>

FdoInt32   ArcSDEBLOBStreamReader::ReadNext( FdoByte* buffer, 
                                    const FdoInt32 offset,
                                    const FdoInt32 count)
{
    FdoInt32 count2 = count;  // Bypass unnecessary "const"

    // Validate count2:
    if ((count2 < -1) || (count2==0))
        throw FdoCommandException::Create(NlsMsgGet1(ARCSDE_INVALID_COUNT, "The count '%1$d' is invalid.  It must be either positive or -1 to read the entire stream.", count));

    // Validate offset:
    if (offset < 0)
        throw FdoCommandException::Create(NlsMsgGet1(ARCSDE_OFFSET_NEGATIVE, "The given offset '%1$d' is negative.", offset));

    // Validate buffer:
    if (NULL==buffer)
        throw FdoCommandException::Create(NlsMsgGet(ARCSDE_NULL_ARGUMENT, "A required argument was set to NULL."));

    // Update count2:
    if ((-1 == count2) || (count2 > (m_BlobInfo->blob_length - m_BlobIndex)))
        count2 = (m_BlobInfo->blob_length - m_BlobIndex);

    // Transfer requested number of bytes to the buffer:
    // (NOTE: I have to make the assumption that buffer is big enough to hold all the data)
    for (FdoInt32 i=0; i<count2; i++)
        buffer[offset+i] = m_BlobInfo->blob_buffer[m_BlobIndex+i];
    // TODO: can we use memcpy() here for faster performance?

    // Update blob index:
    m_BlobIndex += count2;

    // Return number of bytes read:
    return count2;
}

/// </summary> Reads in the next block of items. Use ReadNext( buffer) to read in 
/// the entire stream. The caller is responsible to create/dispose the FdoArray. </summary>
/// <param name="buffer"> FdoArray object holding data read in. Cannot be NULL </param> 
/// <param name="offset"> Index in the array indicating the beginning of the output buffer. 
/// If the end of the buffer, then read in appending mode (store at the end). 
/// Otherwise, overwrite the buffer. Throws "invalid parameter" exception if not in this range.</param> 
/// <param name="count">  Number of items to be read in. If -1 read the entire stream.
///  Throws "invalid parameter"  exception if not a positive value or -1.</param> 
/// <returns> Returns number of items read.</returns> 
FdoInt32    ArcSDEBLOBStreamReader::ReadNext(  FdoArray<FdoByte> * &buffer, 
                                        const FdoInt32 offset,
                                        const FdoInt32 count)
{
    FdoInt32 count2 = count;  // Bypass unnecessary "const"

    // Validate count:
    if ((count2 < -1) || (count2==0))
        throw FdoCommandException::Create(NlsMsgGet1(ARCSDE_INVALID_COUNT, "The count '%1$d' is invalid.  It must be either positive or -1 to read the entire stream.", count2));

    // Validate offset:
    if (offset < 0)
        throw FdoCommandException::Create(NlsMsgGet1(ARCSDE_OFFSET_NEGATIVE, "The given offset '%1$d' is negative.", offset));
    if (offset > buffer->GetCount())
        throw FdoCommandException::Create(NlsMsgGet1(ARCSDE_OFFSET_TOO_LARGE, "The given stream offset '%1$d' is too large.", offset));

    // Validate buffer:
    if (NULL==buffer)
        throw FdoCommandException::Create(NlsMsgGet(ARCSDE_NULL_ARGUMENT, "A required argument was set to NULL."));

    // Update count:
    if ((-1 == count2) || (count2 > (m_BlobInfo->blob_length - m_BlobIndex)))
        count2 = (m_BlobInfo->blob_length - m_BlobIndex);

    // Increase array size, if necessary:
    FdoInt32 newMinimumSize = offset + count2;
    if (newMinimumSize > buffer->GetCount())
        buffer = FdoByteArray::SetSize( buffer, newMinimumSize );

    // Perform the read on the FdoArray's internal buffer:
    return ReadNext(buffer->GetData(), offset, count2);
}

