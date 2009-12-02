#ifndef _FGFUTIL_H_
#define _FGFUTIL_H_
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

#ifdef _WIN32
#pragma once
#endif

#include <Geometry/GeometryStd.h>
#include "GeometryUtility.h"
#include <Geometry/Fgf/Factory.h>
class FdoFgfGeometryPools;   // Forward declaration

// Use lots of macros here, rather than inline functions, to force inline expansion
// of code for performance.

#define FGFUTIL_STREAM_CHECK( \
    /* const FdoByte ** */ streamPtr, \
    /* const FdoByte * */  streamEnd, \
    /* FdoInt32 */         count ) \
{ \
	if ((*(streamPtr)) + count > streamEnd) \
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_5_INDEXOUTOFBOUNDS))); \
}

#define FGFUTIL_READ_DOUBLE( \
    /* const FdoByte ** */ streamPtr, \
    /* const double * */   valuePtr ) \
{ \
    /* This is used mainly for positions, so leave array bounds check for caller. */ \
    *(valuePtr) = *((const double *)(*(streamPtr))); \
    *(streamPtr) += sizeof(double); \
}

#define FGFUTIL_READ_DIRECTPOSITION_BY_MEMBERS( \
    /* const FdoByte ** */         streamPtr, \
    /* const FdoByte * */          streamEnd, \
    /* FdoInt32 */                 dimensionality, \
    /* double * */                 valuePtrX, \
    /* double * */                 valuePtrY, \
    /* double * */                 valuePtrZ, \
    /* double * */                 valuePtrM ) \
{ \
	FGFUTIL_STREAM_CHECK((streamPtr), (streamEnd), 2 * sizeof(double)); \
    FGFUTIL_READ_DOUBLE((streamPtr), (valuePtrX)); \
    FGFUTIL_READ_DOUBLE((streamPtr), (valuePtrY)); \
    if (((dimensionality) &  FdoDimensionality_Z) != 0) \
    { \
    	FGFUTIL_STREAM_CHECK((streamPtr), (streamEnd), sizeof(double)); \
        if (NULL != (valuePtrZ)) \
        { \
            FGFUTIL_READ_DOUBLE((streamPtr), (valuePtrZ)); \
        } \
        else \
        { \
            FGFUTIL_SKIP_DOUBLES((streamPtr), (streamEnd), 1); \
        } \
    } \
    if (((dimensionality) &  FdoDimensionality_M) != 0) \
    { \
    	FGFUTIL_STREAM_CHECK((streamPtr), (streamEnd), sizeof(double)); \
        if (NULL != (valuePtrM)) \
        { \
            FGFUTIL_READ_DOUBLE((streamPtr), (valuePtrM)); \
        } \
        else \
        { \
            FGFUTIL_SKIP_DOUBLES((streamPtr), (streamEnd), 1); \
        } \
    } \
}

#define FGFUTIL_SKIP_INT32S( \
    /* const FdoByte ** */ streamPtr, \
    /* const FdoByte * */  streamEnd, \
    /* FdoInt32 */         count ) \
{ \
    FdoInt32 numBytes = (count) * sizeof(FdoInt32); \
	FGFUTIL_STREAM_CHECK((streamPtr), (streamEnd), numBytes); \
    *(streamPtr) += numBytes; \
}

#define FGFUTIL_SKIP_DOUBLES( \
    /* const FdoByte ** */ streamPtr, \
    /* const FdoByte * */  streamEnd, \
    /* FdoInt32 */         count ) \
{ \
    FdoInt32 numBytes = (count) * sizeof(double); \
	FGFUTIL_STREAM_CHECK((streamPtr), (streamEnd), numBytes); \
    *(streamPtr) += numBytes; \
}

#define FGFUTIL_SKIP_DIRECTPOSITIONS( \
    /* const FdoByte ** */ streamPtr, \
    /* const FdoByte * */  streamEnd, \
    /* FdoInt32 */         ordinatesPerPosition, \
    /* FdoInt32 */         count ) \
{ \
    FdoInt32 numBytes = (ordinatesPerPosition) * (count) * sizeof(double); \
	FGFUTIL_STREAM_CHECK((streamPtr), (streamEnd), numBytes) \
    *(streamPtr) += numBytes; \
}


#define FGFUTIL_WRITE_INT32( \
    /* FdoByteArray ** */    streamPtr, \
    /* FdoInt32 */           value ) \
{ \
    FdoInt32 localValue = (value); \
    *(streamPtr) = FdoByteArray::Append(*(streamPtr), sizeof(FdoInt32), (FdoByte*) &localValue); \
}

#define FGFUTIL_WRITE_DOUBLES( \
    /* FdoByteArray ** */    streamPtr, \
    /* FdoInt32 */           numValues, \
    /* double * */           values ) \
{ \
    (*streamPtr) = FdoByteArray::Append(*(streamPtr), (numValues) * sizeof(double), (FdoByte*) values); \
}

#define FGFUTIL_DISPOSE_TO_POOL_OR_HEAP(type) \
{ \
    FdoFgfGeometryPools * pools = FgfUtil::GetPoolsNoRef(m_pools); \
    if (NULL == pools || \
        pools->m_Pool##type == NULL || \
        !pools->m_Pool##type->AddItem(this)) \
    { \
        delete this; \
    } \
}

class FgfUtil
{
public:

    // Use only public methods to write geometry to stream (supports format converstion).
	static void WriteGeometry(FdoIGeometry* geometry, FdoByteArray ** outputStream);

	static void WriteLinearRing(FdoILinearRing* lineRing, FdoByteArray ** outputStream);
	static void WriteRing(FdoIRing* ring, FdoByteArray ** outputStream);
	static void WriteCurveSegment(FdoICurveSegmentAbstract* curveSeg, FdoByteArray ** outputStream);

	static FdoIGeometry* ReadGeometry(FdoFgfGeometryFactory * factory, FdoGeometryType expectedGeometryType, const FdoByte ** inputStream, const FdoByte * streamEnd);
	static FdoIGeometry* ReadGeometryFromAggregate(FdoFgfGeometryFactory * factory, FdoInt32 Index, FdoGeometryType expectedGeometryType, const FdoByte ** inputStream, const FdoByte * streamEnd);

	static void SkipGeometry(const FdoByte ** inputStream, const FdoByte * streamEnd);

	static FdoICurveSegmentAbstract* ReadCurveSegment(FdoFgfGeometryFactory * factory, FdoInt32 dimensionality, FdoIDirectPosition* startPos, const FdoByte ** inputStream, const FdoByte * streamEnd);

    static void SkipRings(FdoInt32 numCurveRings, FdoInt32 dimensionality, const FdoByte ** inputStream, const FdoByte * streamEnd);
	static void SkipCurveSegments(FdoInt32 numSegs, FdoInt32 dimensionality, const FdoByte ** inputStream, const FdoByte * streamEnd);

    inline static FdoInt32 ReadInt32(const FdoByte ** inputStream, const FdoByte * streamEnd)
    {
	    FGFUTIL_STREAM_CHECK(inputStream, streamEnd, sizeof(FdoInt32));
        FdoInt32 value = *((const FdoInt32 *)(*inputStream));
        *inputStream += sizeof(FdoInt32);
        return value;
    }

    inline static FdoIDirectPosition* ReadDirectPosition(FdoFgfGeometryFactory * factory, FdoInt32 dimensionality, const FdoByte ** inputStream, const FdoByte * streamEnd)
    {
        FdoInt32 numBytes = sizeof(double) * GeometryUtility::DimensionalityToNumOrdinates(dimensionality);
	    FGFUTIL_STREAM_CHECK(inputStream, streamEnd, numBytes);
        FdoIDirectPosition* pos = factory->CreatePosition(dimensionality, (double *)(*inputStream));
        *inputStream += numBytes;
        return pos;
    }

    inline static void WriteDirectPosition(
        FdoByteArray **         outputStream,
        FdoIDirectPosition *    value ) \
    {
	    FdoInt32 dimension = value->GetDimensionality();

        FdoInt32 numDimensions = 2;
        double ordinates[4];
 
	    ordinates[0] = value->GetX();
	    ordinates[1] = value->GetY();

        if ( (dimension &  FdoDimensionality_Z) != 0 )
		    ordinates[numDimensions++] = value->GetZ();
	    if ( (dimension &  FdoDimensionality_M) != 0 )
		    ordinates[numDimensions++] = value->GetM();

        FGFUTIL_WRITE_DOUBLES(outputStream, numDimensions, ordinates);
    }

    // Get object pools (no ref counting, for performance).
    static FdoFgfGeometryPools * GetPoolsNoRef(
          FdoFgfGeometryPools *   privatePools);

    // Wrapper around pools' byte array.  Some compilers (e.g. GNU 3.4.x)
    // won't allow directly calling from a forward declaration in 
    // FdoFgfGeometryImpl, so we make the call available here too.
    static void TakeReleasedByteArray(FdoFgfGeometryPools * pools, FdoByteArray * byteArray);
};

#endif

