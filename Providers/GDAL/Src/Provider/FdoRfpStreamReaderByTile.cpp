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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpStreamReaderByTile.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#include "FDORFP.h"
#include "FdoRfpStreamReaderByTile.h"
#include "FdoRfpImage.h"
#include <cpl_conv.h>


FdoRfpStreamReaderGdalByTile::FdoRfpStreamReaderGdalByTile(
    const FdoPtr<FdoRfpImage>& image,                                 
    int blockXSize, int blockYSize,
    int winXOff, int winYOff, int winXSize, int winYSize,
    int viewXSize, int viewYSize )
        : m_image(image), m_row(0), m_col(0), m_offset(0), 
          m_blockXSize(blockXSize), m_blockYSize(blockYSize),
          m_winXOff(winXOff), m_winYOff(winYOff), m_winXSize(winXSize), m_winYSize(winYSize),
          m_viewXSize(viewXSize), m_viewYSize(viewYSize)
{

    // Eventually do we need to override this? 
    m_gdalDataType = m_image->m_gdalDataType;

    // calculate out the number of tile row and col
    m_numTileCols = (viewXSize - 1) / m_blockXSize + 1;
    m_numTileRows = (viewYSize - 1) / m_blockYSize + 1;

    // figure out the # of bytes in one tile
    m_numTileBytes = m_blockXSize * m_blockYSize * m_image->m_bytesPerPixel;

    // calculate out the length of the stream
    m_length = (FdoInt64)m_numTileRows * m_numTileCols * m_numTileBytes;

    // Allocate tile buffer.
    m_tileData = (GByte *) CPLMalloc(m_numTileBytes);

    // check out the tile at (0, 0)
    _getTile();
}

FdoRfpStreamReaderGdalByTile::~FdoRfpStreamReaderGdalByTile()
{
    CPLFree( m_tileData );
}

/// <summary>Get the size of a stream.</summary>
/// <returns>Returns the size of the data source in number of items </returns> 
FdoInt64 FdoRfpStreamReaderGdalByTile::GetLength()
{
    return m_length;
}

/// <summary> Skips a number of items </summary>
/// <param name="offset">Number of items to skip. Must be a positive number </param> 
/// <returns>Returns nothing </returns> 

void FdoRfpStreamReaderGdalByTile::Skip(const FdoInt32 offset)
{
    if (offset < 0)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));

    FdoInt64 length = GetLength();
    FdoInt32 numRows = m_numTileRows;
    FdoInt32 numCols = m_numTileCols;

    // calculate the target index
    FdoInt64 indexTarget = GetIndex() + offset;
    if (indexTarget < 0)
        indexTarget = 0;
    else if (indexTarget > length)
        indexTarget = length;

    // calculate the row, col and offset of the target index
    FdoInt32 row, col, offsetNew;
    if (indexTarget == length)
    {
        row = numRows;
        col = 0;
        offsetNew = 0;
    }
    else
    {
        FdoInt64 numOneRow = m_numTileCols * m_numTileBytes;
        // calculate row
        row = (FdoInt32) (indexTarget / numOneRow);
        // calculate col
        FdoInt32 offsetInRow = (FdoInt32)(indexTarget - row * numOneRow);
        col = offsetInRow / m_numTileBytes;
        // calculate offset in tile
        offsetNew = offsetInRow - col * m_numTileBytes;
    }

    // skip
    _moveTo(row, col, offsetNew);
}

// move the cursor to the position
void FdoRfpStreamReaderGdalByTile::_moveTo(int row, int col, int offset)
{
    if (row == m_row && col == m_col)	// remain in the same tile
        m_offset = offset;
    else
    {
        m_row = row;
        m_col = col;
        m_offset = offset;

        // get the new tile if it is valid
        if (m_row < m_numTileRows)
            _getTile();
    }

}

    
/// <summary> Get the current index in the stream </summary>
/// <returns>Returns the current index in the stream in number of items </returns> 
FdoInt64 FdoRfpStreamReaderGdalByTile::GetIndex()
{
    if (m_row == m_numTileRows)
        return GetLength();
    return (m_numTileCols *  m_row + m_col) * (FdoInt64)m_numTileBytes + m_offset;
}

/// <summary> Reset the current index in the stream. Allows re-reading </summary>
/// <returns>Returns nothing </returns> 
void FdoRfpStreamReaderGdalByTile::Reset()
{
    _moveTo(0, 0, 0);
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

FdoInt32 FdoRfpStreamReaderGdalByTile::ReadNext( FdoByte* buffer, 
                                    const FdoInt32 offset,
                                    const FdoInt32 count)
{
    if (count < -1 || offset < 0 || buffer == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));

    FdoInt32 countToRead = (FdoInt32)(count == -1 ? GetLength() - GetIndex() : count);

    FdoInt32 numRows = m_numTileRows;
    FdoInt32 countRead = 0;
    while (countToRead > 0 && m_row < m_numTileRows)
    {
        FdoInt32 numLeftInTile = m_numTileBytes - m_offset;
        FdoInt32 copyCount = countToRead < numLeftInTile ? countToRead : numLeftInTile;
        // copy data
        memcpy((void*)(buffer + offset + countRead), (const void*)(m_tileData + m_offset), copyCount);
        countRead += copyCount;
        countToRead -= copyCount;
        numLeftInTile -= copyCount;
        m_offset += copyCount;
        
        // if we reach the end of the tile, move to the next tile
        if (numLeftInTile == 0)
        {
            // calculate new tile
            m_offset = 0;
            m_col++;
            if (m_col == m_numTileCols)
            {
                m_col = 0;
                m_row++;
            }

            // check out new tile
            if (m_row < m_numTileRows)
                _getTile();
        }
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
FdoInt32 FdoRfpStreamReaderGdalByTile::ReadNext(  FdoArray<FdoByte> * &buffer, 
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

// get current tile
void FdoRfpStreamReaderGdalByTile::_getTile()
{
    CPLErr eErr;

    //TODO: This does not address partial tiles at right and bottom!

    // Compute the ratio of a view pixel to a file pixel.
    double ratioX, ratioY;

    ratioX = m_winXSize / (double) m_viewXSize;
    ratioY = m_winYSize / (double) m_viewYSize;

    // Compute the window on the file we need to read to satisfy the request.
    double fileWinULX, fileWinULY, fileWinLRX, fileWinLRY;
    int    fileWinXOff, fileWinYOff, fileWinXSize, fileWinYSize;

    fileWinULX = m_winXOff + (m_col * m_blockXSize) * ratioX;
    fileWinLRX = fileWinULX + m_blockXSize * ratioX;

    fileWinULY = m_winYOff + (m_row * m_blockYSize) * ratioY;
    fileWinLRY = fileWinULY + m_blockYSize * ratioY;

    fileWinXOff = (int) floor( fileWinULX + 0.5 );
    fileWinYOff = (int) floor( fileWinULY + 0.5 );
    fileWinXSize = (int) floor( fileWinLRX + 0.5 ) - fileWinXOff;
    fileWinYSize = (int) floor( fileWinLRY + 0.5 ) - fileWinYOff;
    
    // Read into pixel interleaved buffer.
    eErr = GDALDatasetRasterIO( m_image->m_ds, GF_Read, 
                                fileWinXOff, fileWinYOff, fileWinXSize, fileWinYSize,
                                m_tileData, m_blockXSize, m_blockYSize, m_gdalDataType, 
                                m_image->m_components, m_image->m_bandList, 
                                m_image->m_bytesPerPixel, 
                                m_image->m_bytesPerPixel * m_blockXSize,
                                m_image->m_bytesPerPixel / m_image->m_components );

    if( eErr != CE_None )
    {
        // Throw exception on failiure!
    }
}

inline FdoInt32 FdoRfpStreamReaderGdalByTile::_bytesPerRow(int width)
{
    return m_image->m_bytesPerPixel * m_image->m_blockXSize;
}
