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
#include "FdoRdbmsSimpleBLOBStreamReader.h"

FdoRdbmsSimpleBLOBStreamReader::FdoRdbmsSimpleBLOBStreamReader(FdoLOBValue* val)
{
    m_BlobIndex = 0L;
    m_Blob = (val) ? val->GetData() : NULL;
}

FdoRdbmsSimpleBLOBStreamReader::~FdoRdbmsSimpleBLOBStreamReader(void)
{
    FDO_SAFE_RELEASE(m_Blob);
}

FdoInt64 FdoRdbmsSimpleBLOBStreamReader::GetLength()
{
    return (m_Blob) ? m_Blob->GetCount() : 0;
}

void FdoRdbmsSimpleBLOBStreamReader::Skip(const FdoInt32 offset)
{
    if (offset < 0)
        throw FdoCommandException::Create(NlsMsgGet1(FDORDBMS_547, "The given stream offset '%1$d' is negative.", offset));

    FdoInt64 len = GetLength();
    if ((m_BlobIndex + offset) >= len)
        throw FdoCommandException::Create(NlsMsgGet1(FDORDBMS_548, "The given stream offset '%1$d' is too large.", offset));

    m_BlobIndex += offset;
}

FdoInt64 FdoRdbmsSimpleBLOBStreamReader::GetIndex()
{
    return m_BlobIndex;
}

void FdoRdbmsSimpleBLOBStreamReader::Reset()
{
    m_BlobIndex = 0L;
}

FdoInt32 FdoRdbmsSimpleBLOBStreamReader::ReadNext( FdoByte* buffer, 
                                    const FdoInt32 offset,
                                    const FdoInt32 count)
{
    FdoInt32 count2 = count;  // Bypass unnecessary "const"

    // Validate count2:
    if ((count2 < -1) || (count2==0))
        throw FdoCommandException::Create(NlsMsgGet1(FDORDBMS_549, "The count '%1$d' is invalid. It must be either positive or -1 to read the entire stream.", count));

    // Validate offset:
    if (offset < 0)
        throw FdoCommandException::Create(NlsMsgGet1(FDORDBMS_547, "The given stream offset '%1$d' is negative.", offset));

    // Validate buffer:
    if (NULL==buffer)
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_550, "Buffer required argument was set to NULL."));

    FdoInt32 len = (FdoInt32)GetLength();
    // Update count2:
    if ((-1 == count2) || (count2 > (len - m_BlobIndex)))
        count2 = (len - m_BlobIndex);

    // Transfer requested number of bytes to the buffer:
    // (NOTE: I have to make the assumption that buffer is big enough to hold all the data)
    memcpy(&buffer[offset], m_Blob->GetData()+m_BlobIndex, count2);

    // Update blob index:
    m_BlobIndex += count2;

    // Return number of bytes read:
    return count2;
}

FdoInt32 FdoRdbmsSimpleBLOBStreamReader::ReadNext(  FdoArray<FdoByte> * &buffer, 
                                        const FdoInt32 offset,
                                        const FdoInt32 count)
{
    FdoInt32 count2 = count;  // Bypass unnecessary "const"

    // Validate count:
    if ((count2 < -1) || (count2==0))
        throw FdoCommandException::Create(NlsMsgGet1(FDORDBMS_549, "The count '%1$d' is invalid. It must be either positive or -1 to read the entire stream.", count));

    // Validate offset:
    if (offset < 0)
        throw FdoCommandException::Create(NlsMsgGet1(FDORDBMS_547, "The given stream offset '%1$d' is negative.", offset));

    // Validate buffer:
    if (NULL==buffer)
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_550, "Buffer required argument was set to NULL."));

    if (offset > buffer->GetCount())
        throw FdoCommandException::Create(NlsMsgGet1(FDORDBMS_548, "The given stream offset '%1$d' is too large.", offset));

    FdoInt32 len = (FdoInt32)GetLength();

    // Update count:
    if ((-1 == count2) || (count2 > (len - m_BlobIndex)))
        count2 = (len - m_BlobIndex);

    // Increase array size, if necessary:
    FdoInt32 newMinimumSize = offset + count2;
    if (newMinimumSize > buffer->GetCount())
        buffer = FdoByteArray::SetSize( buffer, newMinimumSize );

    // Perform the read on the FdoArray's internal buffer:
    return ReadNext(buffer->GetData(), offset, count2);
}
