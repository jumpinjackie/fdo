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
#include "slt.h"
#include "SltBLOBStreamReader.h"

SltBLOBStreamReader::SltBLOBStreamReader(FdoLOBValue* val)
{
    m_BlobIndex = 0L;
    m_Blob = (val) ? val->GetData() : NULL;
}

SltBLOBStreamReader::~SltBLOBStreamReader(void)
{
    FDO_SAFE_RELEASE(m_Blob);
}

FdoInt64 SltBLOBStreamReader::GetLength()
{
    return (m_Blob) ? m_Blob->GetCount() : 0;
}

void SltBLOBStreamReader::Skip(const FdoInt32 offset)
{
	wchar_t tmp[15];
    if (offset < 0)
    {
        swprintf(tmp, 15, L"%d", offset);
        throw FdoCommandException::Create((std::wstring(L"The given stream offset \'") + tmp + L"\' is negative.").c_str());
    }

    FdoInt64 len = GetLength();
    if ((m_BlobIndex + offset) >= len)
    {
        swprintf(tmp, 15, L"%d", offset);
        throw FdoCommandException::Create((std::wstring(L"The given stream offset \'") + tmp + L"\' is too large.").c_str());
    }

    m_BlobIndex += offset;
}

FdoInt64 SltBLOBStreamReader::GetIndex()
{
    return m_BlobIndex;
}

void SltBLOBStreamReader::Reset()
{
    m_BlobIndex = 0L;
}

FdoInt32 SltBLOBStreamReader::ReadNext( FdoByte* buffer, 
                                    const FdoInt32 offset,
                                    const FdoInt32 count)
{
    FdoInt32 count2 = count;  // Bypass unnecessary "const"
	wchar_t tmp[15];

    // Validate count2:
    if ((count2 < -1) || (count2==0))
    {
		swprintf(tmp, 15, L"%d", count);
        throw FdoCommandException::Create((std::wstring(L"The count \'") + tmp + L"\' is invalid. It must be either positive or -1 to read the entire stream.").c_str());
    }

    // Validate offset:
    if (offset < 0)
    {
		swprintf(tmp, 15, L"%d", offset);
        throw FdoCommandException::Create((std::wstring(L"The given offset \'") + tmp + L"\' is negative.").c_str());
    }

    // Validate buffer:
    if (NULL==buffer)
        throw FdoCommandException::Create(L"A required argument was set to NULL.");

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

FdoInt32 SltBLOBStreamReader::ReadNext(  FdoArray<FdoByte> * &buffer, 
                                        const FdoInt32 offset,
                                        const FdoInt32 count)
{
    FdoInt32 count2 = count;  // Bypass unnecessary "const"
	wchar_t tmp[15];

    // Validate count:
    if ((count2 < -1) || (count2==0))
    {
        swprintf(tmp, 15, L"%d", count);
        throw FdoCommandException::Create((std::wstring(L"The count \'") + tmp + L"\' is invalid. It must be either positive or -1 to read the entire stream.").c_str());
    }

    // Validate offset:
    if (offset < 0)
    {
		swprintf(tmp, 15, L"%d", offset);
        throw FdoCommandException::Create((std::wstring(L"The given offset \'") + tmp + L"\' is negative.").c_str());
    }

    if (offset > buffer->GetCount())
    {
        swprintf(tmp, 15, L"%d", offset);
        throw FdoCommandException::Create((std::wstring(L"The given stream offset \'") + tmp + L"\' is too large.").c_str());
    }

    // Validate buffer:
    if (NULL==buffer)
        throw FdoCommandException::Create(L"A required argument was set to NULL.");

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
