/*
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
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpStreamReaderRGB24.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#include "FDORFP.h"
#include "FdoRfpStreamReaderRGB24.h"

FdoRfpStreamReaderRGB24::FdoRfpStreamReaderRGB24(FdoIStreamReader* streamReader)
{
	m_baseRGB32Reader = static_cast<FdoBLOBStreamReader*>(FDO_SAFE_ADDREF(streamReader));
	m_cachedCount = 0;
}

FdoRfpStreamReaderRGB24::~FdoRfpStreamReaderRGB24()
{
}

/// <summary>Get the size of a stream.</summary>
/// <returns>Returns the size of the data source in number of items </returns> 
FdoInt64   FdoRfpStreamReaderRGB24::GetLength()
{
	return m_baseRGB32Reader->GetLength() / 4 * 3;
}

/// <summary> Skips a number of items </summary>
/// <param name="offset">Number of items to skip. Must be a positive number </param> 
/// <returns>Returns nothing </returns> 
void       FdoRfpStreamReaderRGB24::Skip(const FdoInt32 offset)
{
	FdoInt32 offset1 = offset;
	offset1 -= m_cachedCount;

	if (offset1 <= 0)
		m_cachedCount -= offset;
	else
	{
		m_cachedCount = 0;
		m_baseRGB32Reader->Skip(offset1 / 3 * 4);
		FdoInt32 left = offset1 % 3;
		if (left != 0)
		{
			FdoByte buffer[4];
			FdoInt32 read = m_baseRGB32Reader->ReadNext(buffer, 0, 4);
			if (read != 0)
			{
				m_cached[m_cachedCount++] = buffer[2];
				if (left == 1)
					m_cached[m_cachedCount++] = buffer[1];
			}
		}
	}
}
    
/// <summary> Get the current index in the stream </summary>
/// <returns>Returns the current index in the stream in number of items </returns> 
FdoInt64   FdoRfpStreamReaderRGB24::GetIndex()
{
	return m_baseRGB32Reader->GetIndex() / 4 * 3 - m_cachedCount;
}

/// <summary> Reset the current index in the stream. Allows re-reading </summary>
/// <returns>Returns nothing </returns> 
void       FdoRfpStreamReaderRGB24::Reset()
{
	m_cachedCount = 0;
	m_baseRGB32Reader->Reset();
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

FdoInt32   FdoRfpStreamReaderRGB24::ReadNext( FdoByte* buffer, 
                                    const FdoInt32 offset,
                                    const FdoInt32 count)
{
	FdoInt32 countToRead = (FdoInt32)(count == -1 ? GetLength() - GetIndex() : count);
	if (countToRead == 0)
		return 0;
	FdoInt32 countRead = 0;
	while (m_cachedCount > 0 && countToRead > 0)
	{
		*(buffer + offset + countRead) = m_cached[m_cachedCount - 1];
		countRead++;
		m_cachedCount--;
		countToRead--;
	}
	if (countToRead == 0)
		return countRead;

	FdoInt32 countToReadInBase = (countToRead + 2) / 3 * 4;
	FdoByte* buffer1 = new FdoByte[countToReadInBase];
	FdoInt32 countReadInBase = m_baseRGB32Reader->ReadNext(buffer1, 0, countToReadInBase);
	FdoInt32 pixelsReadInBase = countReadInBase / 4;
	//FdoInt32 left = countToRead % 3;
	FdoInt32 currentPt = 0;
	while (pixelsReadInBase > 0 && countToRead > 2)
	{
		*(buffer + offset + countRead++) = *(buffer1 + currentPt++);
		*(buffer + offset + countRead++) = *(buffer1 + currentPt++);
		*(buffer + offset + countRead++) = *(buffer1 + currentPt++);
		currentPt++;	//trim the alpha byte
		pixelsReadInBase--;
		countToRead -= 3;
	}

	if (countToRead != 0 && pixelsReadInBase == 1)
	{
		*(buffer + offset + countRead++) = *(buffer1 + currentPt++);
		countToRead--;
		m_cached[m_cachedCount++] = *(buffer1 + currentPt + 1);
		if (countToRead != 0)
			*(buffer + offset + countRead++) = *(buffer1 + currentPt);
		else
			m_cached[m_cachedCount++] = *(buffer1 + currentPt);
	}

	delete buffer1;
	return countRead;

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
FdoInt32    FdoRfpStreamReaderRGB24::ReadNext(  FdoArray<FdoByte> * &buffer, 
                                        const FdoInt32 offset,
                                            const FdoInt32 count)
{
	FdoInt32 countToRead = (FdoInt32)(count == -1 ? GetLength() - GetIndex() : count);
	buffer = FdoArray<FdoByte>::Create(offset + countToRead);

	return ReadNext(buffer->GetData(), offset, count);

}
