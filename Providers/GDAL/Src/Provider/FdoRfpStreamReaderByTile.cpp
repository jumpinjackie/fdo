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
#include "FdoCommonStringUtil.h"
#include <cpl_conv.h>


FdoRfpStreamReaderGdalByTile::FdoRfpStreamReaderGdalByTile(
    const FdoPtr<FdoRfpImage>& image,                                 
    FdoRasterDataModel *model_in,
    int winXOff, int winYOff, int winXSize, int winYSize,
    int viewXSize, int viewYSize )
        : m_image(image), m_row(0), m_col(0), m_offset(0), 
          m_winXOff(winXOff), m_winYOff(winYOff), 
          m_winXSize(winXSize), m_winYSize(winYSize),
          m_viewXSize(viewXSize), m_viewYSize(viewYSize)
{

    // Make a local copy of the data model.  
    m_dataModel = FdoRasterDataModel::Create();
    *m_dataModel = *model_in;

    m_blockXSize = m_dataModel->GetTileSizeX();
    m_blockYSize = m_dataModel->GetTileSizeY();

    // Work out the components based on the datamodeltype.

    switch( m_dataModel->GetDataModelType() )
    {
      case FdoRasterDataModelType_Unknown:
      case FdoRasterDataModelType_Data:
      case FdoRasterDataModelType_Gray:
      case FdoRasterDataModelType_Palette:
        m_components = 1;
        m_bandList[0] = m_image->m_bandList[0];
        break;

      case FdoRasterDataModelType_RGB:
        m_components = 3;
        if( m_image->m_components == 1 )
            m_bandList[0] = m_bandList[1] = m_bandList[2] 
                = m_image->m_bandList[0];
        else
        {
            m_bandList[0] = m_image->m_bandList[0];
            m_bandList[1] = m_image->m_bandList[1];
            m_bandList[2] = m_image->m_bandList[2];
        }
        break;

      case FdoRasterDataModelType_RGBA:
        m_components = 4;
        if( m_image->m_components == 1 )
        {
            m_bandList[0] = m_bandList[1] = m_bandList[2] 
                = m_image->m_bandList[0];
            m_bandList[3] = 0;
        }
        else if( m_image->m_components == 3 )
        {
            m_bandList[0] = m_image->m_bandList[0];
            m_bandList[1] = m_image->m_bandList[1];
            m_bandList[2] = m_image->m_bandList[2];
            m_bandList[3] = 0;
        }
        else 
        {
            m_bandList[0] = m_image->m_bandList[0];
            m_bandList[1] = m_image->m_bandList[1];
            m_bandList[2] = m_image->m_bandList[2];
            m_bandList[3] = m_image->m_bandList[3];
        }
        break;
    }

    // Work out the band map to use. 
    m_bytesPerSample = m_dataModel->GetBitsPerPixel() / (8 * m_components);
    
    // Work out the data type to return expressed in GDAL terms.
    m_gdalDataType = GDT_Unknown;

    if( m_dataModel->GetDataType() == FdoRasterDataType_Float )
    { 
        if( m_bytesPerSample == 4 )
            m_gdalDataType = GDT_Float32;
        else if( m_bytesPerSample == 8 )
            m_gdalDataType == GDT_Float64;
    }
    else if( m_dataModel->GetDataType() == FdoRasterDataType_Integer )
    {
        if( m_bytesPerSample == 2 )
            m_gdalDataType = GDT_Int16;
        else if( m_bytesPerSample == 4 )
            m_gdalDataType == GDT_Int32;
    }
    else if( m_dataModel->GetDataType() == FdoRasterDataType_UnsignedInteger )
    {
        if( m_bytesPerSample == 1 )
            m_gdalDataType = GDT_Byte;
        else if( m_bytesPerSample == 2 )
            m_gdalDataType = GDT_UInt16;
        else if( m_bytesPerSample == 4 )
            m_gdalDataType == GDT_UInt32;
    }

    // If we have unknown then we are in trouble!  But for now we will 
    // just treat as if it is byte.

    if( m_gdalDataType == GDT_Unknown )
        m_gdalDataType = GDT_Byte;

    // calculate out the number of tile row and col
    m_numTileCols = (viewXSize - 1) / m_blockXSize + 1;
    m_numTileRows = (viewYSize - 1) / m_blockYSize + 1;

    // figure out the # of bytes in one tile
    m_numTileBytes = m_blockXSize * m_blockYSize 
        * m_components * m_bytesPerSample;

    // calculate out the length of the stream
    m_length = (FdoInt64)m_numTileRows * m_numTileCols * m_numTileBytes;

    // Allocate tile buffer - TODO: We should really use VSIMalloc() and
    // throw some sort of an exception if it fails.
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

    // Preinit the output buffer to 255.  This is mostly important
    // when we have tiles falling over the right or bottom edge.
    // But this setting also ensures that "added" alpha values are
    // 255.  

    // TODO: Eventually we will probably want to fill missing alpha
    // values with 255 and other missing data with 0. 
    
    memset( m_tileData, 255, m_numTileBytes );

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

    // If this would go off the right or bottom of the image, we may 
    // need to trip the read window.
    int fileXSize = GDALGetRasterXSize( m_image->m_ds );
    int fileYSize = GDALGetRasterYSize( m_image->m_ds );
    int wrkBlockXSize = m_blockXSize;
    int wrkBlockYSize = m_blockYSize;

    if( fileWinXOff + fileWinXSize > fileXSize )
    {
        double xRatio = (fileXSize - fileWinXOff) 
            / (double) fileWinXSize;

        fileWinXSize = fileXSize - fileWinXOff;
        wrkBlockXSize = (int) (wrkBlockXSize * xRatio + 0.5);
    }
    
    if( fileWinYOff + fileWinYSize > fileYSize )
    {
        double yRatio = (fileYSize - fileWinYOff) 
            / (double) fileWinYSize;

        fileWinYSize = fileYSize - fileWinYOff;
        wrkBlockYSize = (int) (wrkBlockYSize * yRatio + 0.5);
    }

    // Figure out the interleaving values to use based on the data
    // model and the selected interleaving.
    
    int pixelStep, lineStep, bandStep;
    
    switch( m_dataModel->GetOrganization() )
    {
      case FdoRasterDataOrganization_Pixel:
        pixelStep = m_bytesPerSample * m_components;
        lineStep = pixelStep * m_blockXSize;
        bandStep = m_bytesPerSample;
        break;

      case FdoRasterDataOrganization_Row:
        pixelStep = m_bytesPerSample;
        lineStep = m_bytesPerSample * m_blockXSize * m_components;
        bandStep = m_bytesPerSample * m_blockXSize;
        break;

      case FdoRasterDataOrganization_Image:
        pixelStep = m_bytesPerSample;
        lineStep = m_bytesPerSample * m_blockXSize;
        bandStep = lineStep * m_blockYSize;
        break;
    }

    // If we have a dummy band # for Alpha, then that means we should
    // effectively not read it. 
    int wrkComponents = m_components;
    if( m_components == 4 && m_bandList[3] == 0 )
        wrkComponents = 3;

    // Read into interleaved buffer.
    eErr = GDALDatasetRasterIO( m_image->m_ds, GF_Read, 
                                fileWinXOff, fileWinYOff, fileWinXSize, fileWinYSize,
                                m_tileData, wrkBlockXSize, wrkBlockYSize, 
                                m_gdalDataType, 
                                wrkComponents, m_bandList, 
                                pixelStep, lineStep, bandStep );

    if( eErr != CE_None )
    {
        wchar_t *msg = NULL;
        multibyte_to_wide( msg, CPLGetLastErrorMsg() );
        throw FdoException::Create( msg );
    }
}
