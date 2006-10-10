

#include <Fdo.h>
#include <wchar.h>

//FDO 3.2 to 3.1 renaming -- remove these when FDO 3.2 is out.
//#define FDO_31
#ifdef FDO_31
#define FdoGeometryType GisGeometryType
#define FdoGeometryComponentType GisGeometryComponentType
#define FdoIReader GisIReader
#define FdoByte GisByte
#define FdoByteArray GisByteArray
#define FdoString GisString
#define FdoInt16 GisInt16
#define FdoInt32 GisInt32
#define FdoInt64 GisInt64
#define FdoIoStream GisIoStream
#define FdoDateTime GisDateTime
#define FdoIStreamReader GisIStreamReader
#define FDO_SAFE_RELEASE GIS_SAFE_RELEASE
#define FDO_SAFE_ADDREF GIS_SAFE_ADDREF
#define FdoIPolygon GisIPolygon
#define FdoIGeometry GisIGeometry
#define FdoPtr GisPtr
#define FdoFgfGeometryFactory GisAgfGeometryFactory
#define CreateGeometryFromFgf CreateGeometryFromAgf
#define FdoIEnvelope GisIEnvelope
#define GetFgf GetAgf
#define FdoILinearRing GisILinearRing
#define FdoGeometryType_Point GisGeometryType_Point
#define FdoGeometryType_LineString GisGeometryType_LineString
#define FdoGeometryType_Polygon GisGeometryType_Polygon
#define FdoGeometryType_MultiPoint GisGeometryType_MultiPoint
#define FdoGeometryType_MultiLineString GisGeometryType_MultiLineString
#define FdoGeometryType_MultiPolygon GisGeometryType_MultiPolygon
#define FdoGeometryType_MultiGeometry GisGeometryType_MultiGeometry
#define FdoGeometryComponentType_LinearRing GisGeometryComponentType_LinearRing
#define FdoGeometryComponentType_LineStringSegment GisGeometryComponentType_LineStringSegment
#define FdoGeometryComponentType_Ring GisGeometryComponentType_Ring
#define FdoDimensionality_XY GisDimensionality_XY
#define FdoDimensionality_Z GisDimensionality_Z
#define FdoDimensionality_M GisDimensionality_M
#endif

#ifdef _WIN32
#define OGR_API __declspec(dllexport)
#else
#define OGR_API
#endif

#ifndef _WIN32
#define _wcsnicmp wcsncasecmp
#endif

#include <ogrsf_frmts.h>


//TODO: usage of wcstombs/mbstowcs is incorrect in most cases
#define A2W(x) \
    size_t len##x = strlen(x)*4+4; \
    wchar_t* w##x = (wchar_t*)alloca(len##x); \
    mbstowcs(w##x, x, len##x);
    
#define W2A(x) \
    size_t len##x = wcslen(x)*4+1; \
    char* mb##x = (char*)alloca(len##x); \
    wcstombs(mb##x, x, len##x);



