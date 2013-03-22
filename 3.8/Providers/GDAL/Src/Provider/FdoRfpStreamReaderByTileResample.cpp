/*
 * Copyright (C) 2012  Autodesk, Inc.
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
 *
 */

#include "FDORFP.h"
#include "FdoRfpStreamReaderByTile.h"
#include "FdoRfpStreamReaderByTileResample.h"
#include "FdoRfpImage.h"
#include "FdoRfpDatasetCache.h"
#include "FdoCommonStringUtil.h"
#include "FdoRfpGlobals.h"
#include <cpl_conv.h>
#include <gdalwarper.h>


FdoRfpStreamReaderGdalByTileResample::FdoRfpStreamReaderGdalByTileResample(
    const FdoPtr<FdoRfpImage>& image,                                 
    FdoRasterDataModel *model_in, FdoString* resamplingMethod,
    int winXOff, int winYOff, int winXSize, int winYSize,
    int viewXSize, int viewYSize )
        : m_resampleRatio(2),
        m_tileBuffer(NULL),
        m_resamplingMethod(resamplingMethod),
        FdoRfpStreamReaderGdalByTile(image, model_in, winXOff, winYOff, winXSize, winYSize, viewXSize, viewYSize)
{
    // Make a copy of the original data model and multiply tile size by resampling ratio
    FdoPtr<FdoRasterDataModel> subSampleModel = FdoRasterDataModel::Create();
    *subSampleModel = *model_in;

    subSampleModel->SetTileSizeX(model_in->GetTileSizeX() * m_resampleRatio );
    subSampleModel->SetTileSizeY(model_in->GetTileSizeY() * m_resampleRatio );

    // Create reader at m_resampleRatio times the resolution of this wrapper reader
    m_resampledReader = new FdoRfpStreamReaderGdalByTile(image, subSampleModel, winXOff, winYOff, winXSize, winYSize,
                                                            m_resampleRatio*viewXSize, m_resampleRatio*viewYSize);
    m_tileBuffer = (FdoByte*) malloc(m_resampledReader->m_numTileBytes);

    // Get first RGBA tile from resampling reader
    m_resampledReader->_getTile();

}

FdoRfpStreamReaderGdalByTileResample::~FdoRfpStreamReaderGdalByTileResample()
{
    if (m_tileBuffer != NULL)
    {
        free(m_tileBuffer);
        m_tileBuffer = NULL;
    }
}

struct TransformScaling {
    double x;
    double y;
};

int ImageToTileTransform(void* args, int bDstToSrc, int nPointCount, double *x, double *y, double *z, int* panSuccess)
{
	TransformScaling* scale = (TransformScaling*) args;

	if (bDstToSrc == TRUE)
	{
		for (int i=0; i<nPointCount; i++)
		{
			double x2 = x[i]*scale->x;
			double y2 = y[i]*scale->y;
            x[i] = x2;
            y[i] = y2;
			panSuccess[i] = TRUE;
		}
	}
	else
	{
		for (int i=0; i<nPointCount; i++)
		{
			double x2 = x[i]/scale->x;
			double y2 = y[i]/scale->y;
            x[i] = x2;
            y[i] = y2;
			panSuccess[i] = TRUE;
		}
	}
	return TRUE;
}

// get current tile
void FdoRfpStreamReaderGdalByTileResample::_getTile()
{
    CPLErr eErr;

    // Try move to
    m_resampledReader->_moveTo(m_row, m_col, 0);

    // pipe raw data into GDAL Warp
    GByte* resampledBytes = m_resampledReader->m_tileData;
    int resampledXSize = m_resampledReader->m_wrkBlockXSize;
    int resampledYSize = m_resampledReader->m_wrkBlockYSize;

    GDALDatasetH resampledDataSet = NULL;
    char dataPtrBuf[64];
    int nChars = CPLPrintPointer(dataPtrBuf, resampledBytes, 63);
    if (nChars > 0 && nChars < 63)
    {
        dataPtrBuf[nChars] = '\0';
        char buf[256];
        snprintf(buf, 255, "MEM:::DATAPOINTER=%s,PIXELS=%d,LINES=%d,BANDS=4,DATATYPE=Byte,PIXELOFFSET=4,LINEOFFSET=%d,BANDOFFSET=1",
            dataPtrBuf, resampledXSize, resampledYSize, resampledXSize*4);
        GDALDriverH memDriver = GDALGetDriverByName("MEM");
        resampledDataSet = GDALOpen(buf, GA_ReadOnly);
    }

    if( resampledDataSet == NULL )
    {
        wchar_t *msg = NULL;
        multibyte_to_wide( msg, CPLGetLastErrorMsg() );
        throw FdoException::Create( msg );
    }

    GDALResampleAlg algorithm = GRA_NearestNeighbour;
    FdoStringP upper = m_resamplingMethod.Upper();

    if (upper == FdoGrfpGlobals::ResamplingBilinear)  {
		algorithm = GRA_Bilinear;
    } else if (upper == FdoGrfpGlobals::ResamplingCubic) {
		algorithm = GRA_Cubic;
    } else if (upper == FdoGrfpGlobals::ResamplingCubicSpline) {
		algorithm = GRA_CubicSpline;
    } else if (upper == FdoGrfpGlobals::ResamplingLanczos) {
		algorithm = GRA_Lanczos;
	}


    int origXSize = resampledXSize/m_resampleRatio;
    int origYSize = resampledYSize/m_resampleRatio;

    // Clear output buffer to prepare for results of warp
    memset( m_tileData, 255, m_numTileBytes );

    GDALDatasetH tileDataSet = NULL;
    nChars = CPLPrintPointer(dataPtrBuf, m_tileData, 63);
    if (nChars > 0 && nChars < 63)
    {
        dataPtrBuf[nChars] = '\0';
        char buf[256];
        snprintf(buf, 255, "MEM:::DATAPOINTER=%s,PIXELS=%d,LINES=%d,BANDS=4,DATATYPE=Byte,PIXELOFFSET=4,LINEOFFSET=%d,BANDOFFSET=1",
        dataPtrBuf, origXSize, origYSize, origXSize*4);
	    tileDataSet = GDALOpen(buf, GA_Update);
    }

    if( tileDataSet == NULL )
    {
        wchar_t *msg = NULL;
        multibyte_to_wide( msg, CPLGetLastErrorMsg() );
        throw FdoException::Create( msg );
    }

    GDALWarpOptions *psOptions = GDALCreateWarpOptions();

    psOptions->nDstAlphaBand = 4;
    psOptions->nSrcAlphaBand = 4;
    psOptions->hSrcDS = resampledDataSet;
    psOptions->hDstDS = tileDataSet;
	psOptions->eResampleAlg = algorithm;
	psOptions->eWorkingDataType = GDT_Byte;
	psOptions->nBandCount = 4;
	psOptions->panSrcBands = (int *) CPLMalloc(sizeof(int) * 4 );
    psOptions->panDstBands = (int *) CPLMalloc(sizeof(int) * 4 );

    for( int i = 0; i < 4; i++ )
    {
        psOptions->panSrcBands[i] = i+1;
        psOptions->panDstBands[i] = i+1;
    }

    TransformScaling scaling;
    scaling.x = (double) m_resampleRatio;
    scaling.y = (double) m_resampleRatio;

    psOptions->pTransformerArg = (void*) &scaling;
    psOptions->pfnTransformer = ImageToTileTransform;

    GDALWarpOperation warpOp;
	eErr = warpOp.Initialize(psOptions);

    if ( eErr == CE_None )
    {
        eErr = warpOp.WarpRegion(0,0,origXSize,origYSize);
    }
   
    GDALClose(resampledDataSet);
    GDALClose(tileDataSet);

    if( eErr != CE_None )
    {
        wchar_t *msg = NULL;
        multibyte_to_wide( msg, CPLGetLastErrorMsg() );
        throw FdoException::Create( msg );
    }
}
