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
 */
#include "stdafx.h"

#include <FdoWmsStreamReaderGdalByRow.h>
#include <gdal.h>
#include <gdal_priv.h>
#include <FdoWmsImage.h>

FdoWmsStreamReaderGdalByRow::FdoWmsStreamReaderGdalByRow(FdoPtr<FdoWmsImage>& image) : m_image(image), m_index(0)
{
	m_length = m_image->GetLength();

	// get the buffer data for the raster image
	m_buffer = m_image->GetBuffer();
}

FdoWmsStreamReaderGdalByRow::~FdoWmsStreamReaderGdalByRow()
{
	// it would be released by m_image
	m_buffer = NULL;
}

/// <summary>Get the size of a stream.</summary>
/// <returns>Returns the size of the data source in number of items </returns> 
FdoInt64 FdoWmsStreamReaderGdalByRow::GetLength()
{	
	return m_length;
}

/// <summary> Skips a number of items </summary>
/// <param name="offset">Number of items to skip. Must be a positive number </param> 
/// <returns>Returns nothing </returns> 
void FdoWmsStreamReaderGdalByRow::Skip(const FdoInt32 offset)
{
    if (offset < 0)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));

	m_index += offset;
}
    
/// <summary> Get the current index in the stream </summary>
/// <returns>Returns the current index in the stream in number of items </returns> 
FdoInt64 FdoWmsStreamReaderGdalByRow::GetIndex()
{
	return m_index;
}

/// <summary> Reset the current index in the stream. Allows re-reading </summary>
/// <returns>Returns nothing </returns> 
void FdoWmsStreamReaderGdalByRow::Reset()
{
 	m_index = 0;
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
FdoInt32 FdoWmsStreamReaderGdalByRow::ReadNext( FdoByte* buffer, 
                                    const FdoInt32 offset,
                                    const FdoInt32 count)
{
    if (count < -1 || offset < 0 || buffer == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));

	FdoInt32 countRead = 0;
	FdoInt32 remains = GetLength() - GetIndex();
	if (remains > 0)
	{
		countRead = count;
		if (count == -1 || count > remains)
			countRead = remains;

		memcpy(buffer+offset, m_buffer+m_index, countRead);
		m_index += countRead;
	}

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
FdoInt32 FdoWmsStreamReaderGdalByRow::ReadNext(  FdoArray<FdoByte> * &buffer, 
                                        const FdoInt32 offset,
                                        const FdoInt32 count)
{
    if (count < -1 || offset < 0)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));
	FdoInt32 countToRead = (FdoInt32)(count == -1 ? GetLength() - GetIndex() : count);
	// resize the buffer if necessary
	buffer = FdoArray<FdoByte>::Create(offset + countToRead);

	return ReadNext(buffer->GetData(), offset, count);
}

