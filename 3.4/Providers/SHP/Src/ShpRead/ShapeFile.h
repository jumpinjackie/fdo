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
 */

#ifndef SHAPEFILE_H
#define SHAPEFILE_H

#include "Shape.h"
#include "NullShape.h"
#include "PointShape.h"
#include "PolylineShape.h"
#include "PolygonShape.h"
#include "MultiPointShape.h"
#include "PointZShape.h"
#include "PolylineZShape.h"
#include "PolygonZShape.h"
#include "MultiPointZShape.h"
#include "PointMShape.h"
#include "PolylineMShape.h"
#include "PolygonMShape.h"
#include "MultiPointMShape.h"
#include "MultiPatchShape.h"
#include "ShapeFileBase.h"

class ShapeFile : public ShapeFileBase
{

typedef struct {
    ULONG   nOffset; 
	bool	bOffsetValid;
    int     nRecordNumber;
    int     nContentLength;
} SHPRecordInfo;

public:
    ShapeFile(const WCHAR* wszFilename);
    ShapeFile(const WCHAR* wszFilename, eShapeTypes shape_type, bool has_m);
    virtual ~ShapeFile();

    static Shape* ShapeFromPoint (FdoIPoint* point, BoundingBoxEx* box, int record);
    static Shape* ShapeFromMultiPoint (FdoIMultiPoint* point, BoundingBoxEx* box, int record);
    static Shape* ShapeFromLine (FdoILineString* line, BoundingBoxEx* box, int record);
    static Shape* ShapeFromMultiLine (FdoIMultiLineString* line, BoundingBoxEx* box, int record);
    static Shape* ShapeFromPolygon (FdoIPolygon* polygon, BoundingBoxEx* box, int record);
    /* //ECO 10400 indicates we should never support FdoIMultiPolygon as input
    static Shape* ShapeFromMultiPolygon (FdoIMultiPolygon* polygons, BoundingBoxEx* box, int record);
    */
    static Shape* ShapeFromGeometry (FdoByteArray* bytes, int nRecordNumber);

    // Data access methods
    Shape* GetObjectAt(ULONG nOffset, eShapeTypes& nShapeTypes);
    int ReadRecordHeader(ULONG ulOffset, ULONG& ulNextObjectOffset, int& nRecordNumber);
    void SetObjectAt (Shape* shape, bool batch = false);
    void WriteRecordHeader (int number, int length);
    void PutShape (Shape* shape);
private:
    // Cache Info
    BYTE*               m_szRowBuffer;
    size_t              m_nRowBufferSize;
    SHPRecordInfo       m_ReadRecordsBuffer[SHP_FILE_READ_CACHE_SIZE];

    // Statistics
    int                 m_nCacheHits;
    int                 m_nCacheMisses;

private:
    void    ReadRecordInfo(SHPRecordInfo *pRecordInfo);
    void    ReadRawDataBlock(ULONG ulStartOffset );
    BYTE*   GetRowShapeFromCache(ULONG ulOffset, int& nRecordNumber);
    void    ClearRowShapeCache();

};

#endif // SHAPEFILE_H

