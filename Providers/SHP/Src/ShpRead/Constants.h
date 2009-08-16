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

#ifndef CONSTANTS_H
#define CONSTANTS_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "DoublePoint.h"
#include "BoundingBox.h"

//*******************************************************************
// D E F I N I T I O N S
//*******************************************************************
// The size of the cache used for buffering when reading the SHP, SHX and DBF files.
#define SHP_FILE_READ_CACHE_SIZE 50

//*******************************************************************
// E N U M E R A T I O N S
//*******************************************************************

enum eMinMaxTypes {
    eMinXMinY = 0,
    eMaxXMaxY,
    eMinZMaxZ,
    eMinMMaxM
};

// Shape Types
enum eShapeTypes {
    eNullShape = 0,             // 0
    ePointShape = 1,            // 1
    ePolylineShape = 3,         // 3
    ePolygonShape = 5,          // 5
    eMultiPointShape = 8,       // 8
    ePointZShape = 11,          // 11
    ePolylineZShape = 13,       // 13
    ePolygonZShape = 15,        // 15
    eMultiPointZShape = 18,     // 18
    ePointMShape = 21,          // 21
    ePolylineMShape = 23,       // 23
    ePolygonMShape = 25,        // 25
    eMultiPointMShape = 28,     // 28
    eMultiPatchShape = 31       // 31
};

// Part Types
enum ePartTypes {
    eTriangleStripPartType = 0,     // 0
    eTriangleFanPartType,           // 1
    eOuterRingPartType,             // 2
    eInnerRingPartType,             // 3
    eFirstRingPartType,             // 4
    eRingPartType                   // 5
};

//*******************************************************************
// T Y P E D E F S
//*******************************************************************

#pragma pack(push, 1)

// ESRI Shape Structures
typedef struct {
    int type;                       // 4 bytes
    BoundingBox boundingBox;        // 32 Bytes
    int nNumPoints;                 // 4 Bytes
} ESRIMultiPointRecord;

typedef struct {
    int type;                       // 4 bytes
    BoundingBox boundingBox;        // 32 Bytes
    int nNumParts;                  // 4 Bytes
    int nNumPoints;                 // 4 Bytes
} ESRIPolylineRecord;

typedef struct {
    int type;                       // 4 bytes
    BoundingBox boundingBox;        // 32 Bytes
    int nNumParts;                  // 4 Bytes
    int nNumPoints;                 // 4 Bytes
} ESRIPolygonRecord;

typedef struct {
    unsigned int nOffset;
    int nContentLength;
} SHPIndexRecordHeader;

typedef struct {
    int nRecordNumber;
    int nContentLength;
} SHPRecordHeader;

typedef struct {
    int nFileCode;
    int nUnused1;
    int nUnused2;
    int nUnused3;
    int nUnused4;
    int nUnused5;
    int nFileLength;
    int nFileVersion;
    int nFileShapeType;
    double cXMin;
    double cYMin;
    double cXMax;
    double cYMax;
    double cZMin;
    double cZMax;
    double cMMin;
    double cMMax;
} SHPHeader;
const int SHPHeaderSize = sizeof (SHPHeader);

#pragma pack(pop)

//*******************************************************************
// D E F I N E S
//*******************************************************************
// Macros to swap from Big Endian to Little Endian
#define SWAPWORD(x) MAKEWORD( HIBYTE(x), LOBYTE(x) )
#define SWAPLONG(x) MAKELONG( SWAPWORD(HIWORD(x)), SWAPWORD(LOWORD(x)) )

// Status codes, errors are negative
#define SHP_OK                                          1
#define SHP_SI_SEARCH_COMPLETE                          3
#define SHP_SI_SEARCH_ALL_TYPES                         4                       
#define SHP_SI_TRAVERSAL_COMPLETE  SHP_SI_SEARCH_COMPLETE
#define SHP_SI_TRAVERSAL_CANCELLED                      5
#define SHP_SI_SEARCH_OBJECT_NOT_FOUND                  6

#define SHP_EOF                                         -1

// DBF Defines
#define DEFAULT_INT_COL_LENGTH          11		// 10 digits + 1 (the minus sign): (-2147483648, 2147483647)

// Record information
#define RECORD_HEADER_SIZE              8
#define RECORD_SHAPETYPE_HEADER_SIZE    12
#define WORD_SIZE_IN_BYTES              2

// Record offsets
#define FILE_CODE_OFFSET            0
#define FILE_LENGTH_OFFSET          24
#define VERSION_OFFSET              28
#define SHAPE_TYPE_OFFSET           32
#define BOUNDING_BOX_XMIN_OFFSET    36
#define BOUNDING_BOX_YMIN_OFFSET    44
#define BOUNDING_BOX_XMAX_OFFSET    52
#define BOUNDING_BOX_YMAX_OFFSET    60
#define BOUNDING_BOX_ZMIN_OFFSET    68
#define BOUNDING_BOX_ZMAX_OFFSET    76
#define BOUNDING_BOX_MMIN_OFFSET    84
#define BOUNDING_BOX_MMAX_OFFSET    92
#define RECORD_START_OFFSET         100

#define RECORD_SHAPETYPE_OFFSET     8

// Bounding box offsets
#define RECORD_BOUNDINGBOX_XMIN_OFFSET          0
#define RECORD_BOUNDINGBOX_YMIN_OFFSET          8
#define RECORD_BOUNDINGBOX_XMAX_OFFSET          16
#define RECORD_BOUNDINGBOX_YMAX_OFFSET          24

#define RECORD_MULTIPOINT_NUMPOINTS_OFFSET      36
#define RECORD_MULTIPOINT_POINTS_OFFSET         40

#define RECORD_BOUNDINGBOX_OFFSET               4
#define RECORD_NUMPARTS_OFFSET                  36
#define RECORD_NUMPOINTS_OFFSET                 40
#define RECORD_PARTS_OFFSET                     44

// Point record offsets
#define RECORD_POINT_OFFSET                     4
#define RECORD_POINT_Z_OFFSET                   20
#define RECORD_POINT_M_NO_Z_OFFSET              20
#define RECORD_POINT_M_HAS_Z_OFFSET             28

// ***************************************************************************
// * C O N S T A N T S
// ***************************************************************************
const int nSHP_FILE_CODE = 9994;
const int nSHP_FILE_VERSION = 1000;
// Any floating point number smaller than -10^38 is considered by a shapefile
// reader to represent a "no data" value.
const double fNO_DATA = -1E38;
const int nFIELD_DESCRIPTOR_SIZE = 32;
const int nDBF_COLNAME_LENGTH = 11;
const int nFIELD_ADDRESS_SIZE = 4;
const int nFIELD_RESERVE_SIZE = 14;
const BYTE cVALID_RECORD_ID = 0x20; // ' '
const BYTE cDELETED_RECORD_ID = 0x2A; // '*'

// DBF Read errors
const int SHP_ERROR_UNSUPPORTED_DBF_FORMAT = -90;

// DBase types
const BYTE cDBASE_III_PLUS_NO_MEMO  = 0x03;
const BYTE cDBASE_III_PLUS_MEMO     = 0x83;

// DBF Offsets
const int DBF_RECORDS_IN_TABLE              = 4;
const int DBF_BYTES_IN_HEADER               = 8;
const int DBF_BYTES_IN_RECORDS              = 10;
const int DBF_FIELD_DESCRIPTOR_ARRAY_START  = 32;

// Characters
const WCHAR wszNULL = L'\0';
const CHAR szNULL = '\0';
const CHAR szSPACE = ' ';
const CHAR END_OF_HEADER = (CHAR)0x0d;
const CHAR END_OF_FILE = (CHAR)0x1a;

// from: "ESRI Shapefile Technical Description"
// <quote>The following are important notes about Polygon shapes.
// - The rings are closed (the first and last vertex of a ring MUST be the same).
// - The order of rings in the points array is not significant.
// - Polygons stored in a shapefile must be clean. A clean polygon is one that
// 1. Has no self-intersections. This means that a segment belonging to one ring may
// not intersect a segment belonging to another ring. The rings of a polygon can
// touch each other at vertices but not along segments. Colinear segments are
// considered intersecting.
// 2. Has the inside of the polygon on the "correct" side of the line that defines it. The
// neighborhood to the right of an observer walking along the ring in vertex order is
// the inside of the polygon. Vertices for a single, ringed polygon are, therefore,
// always in clockwise order. Rings defining holes in these polygons have a
// counterclockwise orientation. "Dirty" polygons occur when the rings that define
// holes in the polygon also go clockwise, which causes overlapping interiors.</quote>

// RELATE_RINGS flag controls the behaviour: 
//     - FALSE we assume that if there is more than one ring, that the first one is
//     the outer ring and all remaining ones are interior rings.
//     - TRUE then the rings will be associated to each other resulting in well formed
//     polygons (exterior rings and interior rings). A MultiPolygon may result as
//     well, depending on the number of exterior rings detected).

#define RELATE_RINGS    false

#endif  // CONSTANTS_H
