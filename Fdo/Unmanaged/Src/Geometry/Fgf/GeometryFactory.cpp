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

// This is the implementation for FgfGeometryFactory AND FgfGeometryFactory2.
// Conversion to smart pointers was done while the public API was frozen,
// so FgfGeometryFactory2 was created as an unpublished extension of 
// FgfGeometryFactory in order to add smart-pointer members and use them.

#include <float.h>      // For _isnan()
#include <Geometry/Fgf/Factory.h>
#include "GeometryFactory2.h"
#include "GeometryThreadData.h"
#include <Parse/ParseFgft.h>
#include <math.h>
#include "Util.h"

#define WKB_LITTLE_ENDIAN    (1)

FdoFgfGeometryFactory* FdoFgfGeometryFactory::GetInstance()
{
    FdoGeometryThreadData * threadData = FdoGeometryThreadData::GetValue();
    if (threadData->geometryFactory == NULL)
    {
        // Create a new geometry factory, which will be the only actual instance
        // for this thread.  It will be released automatically when the thread
        // releases the thread data upon exit.  That happens when
        // code in FdoGeometryDll.cpp calls FdoGeometryThreadData::ReleaseValue().

        try
        {
            threadData->geometryFactory = new FdoFgfGeometryFactory();
        }
        catch(...)
        {
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
        }
    }

	return FDO_SAFE_ADDREF((threadData->geometryFactory).p);
}

FdoFgfGeometryFactory * FdoFgfGeometryFactory::GetPrivateInstance(
    FdoInt32 numGeometries,
    FdoInt32 numPositions,
    FdoInt32 numEnvelopes,
    FdoInt32 numCurveSegments,
    FdoInt32 numRings)
{
    // Only a limited implementation is available, which uses defaults for
    // the pool sizes.  The returned object is, however, a private one,
    // i.e. not in thread-local storage.
    FdoFgfGeometryFactory * gf = new FdoFgfGeometryFactory(
        numGeometries,
        numPositions,
        numEnvelopes,
        numCurveSegments,
        numRings);

    return gf;
}


// Constructor
FdoFgfGeometryFactory::FdoFgfGeometryFactory()
:
    m_private(NULL)
{
    try
    {
        m_private = new FdoFgfGeometryFactory2(true);
    }
    catch(...)
    {
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
    }
#ifdef EXTRA_DEBUG
    char fileName[1000];
    sprintf(fileName, "C:\\FdoDebugFile0x%lx.log", (long)(this));
    m_private->m_geometryPools->m_fdoDebugFile = fopen(fileName, "w+");
#endif
}


// Constructor
FdoFgfGeometryFactory::FdoFgfGeometryFactory(
    FdoInt32 numGeometries,
    FdoInt32 numPositions,
    FdoInt32 numEnvelopes,
    FdoInt32 numCurveSegments,
    FdoInt32 numRings)
:
    m_private(NULL)
{
    try
    {
        m_private = new FdoFgfGeometryFactory2(false);
    }
    catch(...)
    {
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
    }
#ifdef EXTRA_DEBUG
    char fileName[1000];
    sprintf(fileName, "C:\\FdoDebugFile0x%lx.log", (long)(this));
    m_private->m_geometryPools->m_fdoDebugFile = fopen(fileName, "w+");
#endif
}


// Destructor
FdoFgfGeometryFactory::~FdoFgfGeometryFactory()
{
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "FdoFgfGeometryFactory::~FdoFgfGeometryFactory()\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
        m_private->m_geometryPools->PrintStats(m_private->m_geometryPools->m_fdoDebugFile);
        fclose(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif

    delete m_private;
}

FdoFgfGeometryFactory2::FdoFgfGeometryFactory2(bool useThreadLocal)
    : m_useThreadLocal(useThreadLocal)
{
    try
    {
        if (useThreadLocal)
        {
            FdoGeometryThreadData * threadData = FdoGeometryThreadData::GetValue();
            if (NULL == threadData->geometryPools)
                threadData->geometryPools = new FdoFgfGeometryPools();
            m_geometryPools = threadData->geometryPools;
        }
        else
        {
            m_geometryPools = new FdoFgfGeometryPools();
        }
    }
    catch(...)
    {
        if (m_geometryPools == NULL)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
    }
}

FdoFgfGeometryFactory2::~FdoFgfGeometryFactory2()
{
    if (!m_useThreadLocal)
        FDO_SAFE_RELEASE(m_geometryPools.p);
}

#ifdef EXTRA_DEBUG
void FdoFgfGeometryPools::PrintStats(FILE * fileHandle)
{
    if (NULL != fileHandle)
    {
        FdoInt32 ehits=0, rhits=0, misses=0;
        fprintf(fileHandle, "  FdoFgfGeometryFactory::PrintStats():\n");
        int debugFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
        fprintf(fileHandle, "    memory debug flag = 0x%lx.\n", debugFlag);
        fprintf(fileHandle, "    %d ByteArrays created.\n", m_numByteArraysCreated);
        if (m_PoolByteArray != NULL)
        {
            m_PoolByteArray->GetStats(&ehits, &rhits, &misses);
            fprintf(fileHandle,
                "      ByteArray Pool stats: %d empty hits, %d reusable hits, %d misses\n",
                ehits, rhits, misses);
        }
        if (this->m_PoolLineString != NULL)
        {
            m_PoolLineString->GetStats(&ehits, &rhits, &misses);
            fprintf(fileHandle,
                "      LineString Pool stats: %d empty hits, %d reusable hits, %d misses\n",
                ehits, rhits, misses);
        }
        if (m_PoolLinearRing != NULL)
        {
            m_PoolLinearRing->GetStats(&ehits, &rhits, &misses);
            fprintf(fileHandle,
                "      LinearRing Pool stats: %d empty hits, %d reusable hits, %d misses\n",
                ehits, rhits, misses);
        }
        fflush(fileHandle);
    }
}
#endif

FdoILineString* FdoFgfGeometryFactory::CreateLineString(FdoDirectPositionCollection * positions)
{
    FDOPOOL_CREATE_OBJECT(
        m_private->m_geometryPools->m_PoolLineString, FdoPoolFgfLineString, 4,
        FdoFgfLineString,
        FdoFgfLineString(this, m_private->GetPoolsForGeomCtor(), positions),
        Reset(positions) );
}

FdoILineString* FdoFgfGeometryFactory::CreateLineString(FdoInt32 dimensionType, FdoInt32 numOrdinates, double* ordinates)
{
    FDOPOOL_CREATE_OBJECT(
        m_private->m_geometryPools->m_PoolLineString, FdoPoolFgfLineString, 4,
        FdoFgfLineString,
        FdoFgfLineString(this, m_private->GetPoolsForGeomCtor(), dimensionType, numOrdinates, ordinates),
        Reset(dimensionType, numOrdinates, ordinates) );
}

// Pseudo copy constructor; this method can convert between implementations.
FdoIGeometry * FdoFgfGeometryFactory::CreateGeometry(FdoIGeometry * geometry)
{
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateGeometry(geometry=0x%lx) type=%d IN\n",
            (long)geometry, (int)(geometry->GetDerivedType()));
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
        try
        {
            //if (geometry->GetDerivedType() == FdoGeometryType_LineString)
            {
                FdoString * fgftString = geometry->GetText();
                fprintf(m_private->m_geometryPools->m_fdoDebugFile, "  fgft='%S'\n", fgftString);
                fflush(m_private->m_geometryPools->m_fdoDebugFile);
            }
        }
        catch ( FdoException* e )
        {
            fprintf(m_private->m_geometryPools->m_fdoDebugFile, "  Caught FdoException from GetText(): <<%S>>\n", e->GetExceptionMessage());
            fflush(m_private->m_geometryPools->m_fdoDebugFile);
        }
        catch (...)
        {
            fprintf(m_private->m_geometryPools->m_fdoDebugFile, "  Caught unknown exception from GetText().\n");
            fflush(m_private->m_geometryPools->m_fdoDebugFile);
        }

    }
#endif

    FdoByteArray * newByteArray = this->GetByteArray();

    FgfUtil::WriteGeometry(geometry, &newByteArray);

    FdoPtr<FdoIGeometry> newGeometry = this->CreateGeometryFromFgf(newByteArray, NULL, 0);

    FDO_SAFE_RELEASE(newByteArray);
    newByteArray = NULL;

#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateGeometry() OUT\n",
            (long)geometry);
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	return FDO_SAFE_ADDREF(newGeometry.p);
}


/************************************************************************/
/* CreateGeometryFromFgf()                                                                     */
/************************************************************************/
FdoIGeometry * FdoFgfGeometryFactory::CreateGeometryFromFgf(
    FdoByteArray * byteArray,
    const FdoByte * byteArrayData,
    FdoInt32 count)
{
	if ( (NULL == byteArray) &&
		 (NULL == byteArrayData || count < sizeof(FdoInt32)) )   // Must hold Int32 for geometry type.
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoIGeometry",
                                                               L"byteArray/byteArrayData/count"));

#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateGeometryFromFgf(0x%lx, 0x%lx, count=%d) IN\n",
            (long)byteArray, (long)byteArrayData, count);
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	FdoPtr<FdoIGeometry> newGeometry;

	const FdoByte *         streamPtr = NULL;
	const FdoByte *         streamEnd = NULL;
    if (NULL != byteArray)
    {
        streamPtr = byteArray->GetData();
        streamEnd = streamPtr + byteArray->GetCount();
    }
    else
    {
        streamPtr = byteArrayData;
        streamEnd = streamPtr + count;
    }

	FdoGeometryType geometryType = (FdoGeometryType) FgfUtil::ReadInt32(&streamPtr, streamEnd);
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "FdoFgfGeometryFactory::CreateGeometryFromFgf GeometryType=%d\n", (int) geometryType);
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif

#define CASE_CREATE_POOLED_GEOMETRY(type) \
    case FdoGeometryType_##type: \
        newGeometry = m_private->m_geometryPools->Create##type(this, m_private->GetPoolsForGeomCtor(), byteArray, byteArrayData, count); \
        break;

    // Acquire (from a pool or 'new') a geometry of the needed
    // type, making it point to the given byte array.
    switch ( geometryType )
    {
    CASE_CREATE_POOLED_GEOMETRY(LineString);
    CASE_CREATE_POOLED_GEOMETRY(Point);
    CASE_CREATE_POOLED_GEOMETRY(Polygon);
    CASE_CREATE_POOLED_GEOMETRY(MultiPoint);
    CASE_CREATE_POOLED_GEOMETRY(MultiGeometry);
    CASE_CREATE_POOLED_GEOMETRY(MultiLineString);
    CASE_CREATE_POOLED_GEOMETRY(MultiPolygon);
    CASE_CREATE_POOLED_GEOMETRY(CurveString);
    CASE_CREATE_POOLED_GEOMETRY(MultiCurveString);
    CASE_CREATE_POOLED_GEOMETRY(CurvePolygon);
    CASE_CREATE_POOLED_GEOMETRY(MultiCurvePolygon);
    default:
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_UNKNOWN_GEOMETRY_TYPE),
                                                               L"FdoFgfGeometryFactory::CreateGeometryFromFgf",
                                                               geometryType));
    }

#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateGeometryFromFgf() OUT\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
    return FDO_SAFE_ADDREF(newGeometry.p);
}

FdoIGeometry * FdoFgfGeometryFactory::CreateGeometryFromFgf(FdoByteArray * byteArray)
{
    return CreateGeometryFromFgf(byteArray, NULL, 0);
}

FdoIGeometry * FdoFgfGeometryFactory::CreateGeometryFromFgf(
        const FdoByte * byteArray, FdoInt32 count)
{
    return CreateGeometryFromFgf(NULL, byteArray, count);
}


/************************************************************************/
/* GetFgf()                                                                     */
/************************************************************************/
FdoByteArray * FdoFgfGeometryFactory::GetFgf(FdoIGeometry * geometry)
{
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "FdoFgfGeometryFactory::GetFgf() IN\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	FdoPtr<FdoByteArray> array = 0;

	/*
	 * This works only when the geometry's implementation
	 * is actually FGF-based.
	 */

	switch ( geometry->GetDerivedType() )
	{
	case FdoGeometryType_LineString:
		array = ((FdoFgfLineString *)geometry)->GetFgf();
		break;

	case FdoGeometryType_Point:
		array = ((FdoFgfPoint*)geometry)->GetFgf();
		break;

	case FdoGeometryType_Polygon:
		array = ((FdoFgfPolygon*)geometry)->GetFgf();
		break;

	case FdoGeometryType_MultiPoint:
		array = ((FdoFgfMultiPoint*)geometry)->GetFgf();
		break;

	case FdoGeometryType_MultiGeometry:
		array = ((FdoFgfMultiGeometry*)geometry)->GetFgf();
		break;

	case FdoGeometryType_MultiLineString:
		array = ((FdoFgfMultiLineString*)geometry)->GetFgf();
		break;

	case FdoGeometryType_MultiPolygon:
		array = ((FdoFgfMultiPolygon*)geometry)->GetFgf();
		break;

	case FdoGeometryType_CurveString:
		array = ((FdoFgfCurveString*)geometry)->GetFgf();
		break;

	case FdoGeometryType_MultiCurveString:
		array = ((FdoFgfMultiCurveString*)geometry)->GetFgf();
		break;

	case FdoGeometryType_CurvePolygon:
		array = ((FdoFgfCurvePolygon*)geometry)->GetFgf();
		break;

	case FdoGeometryType_MultiCurvePolygon:
		array = ((FdoFgfMultiCurvePolygon*)geometry)->GetFgf();
		break;

	default:
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_UNKNOWN_GEOMETRY_TYPE),
                                                               L"FdoFgfGeometryFactory::GetFgf",
                                                               geometry->GetDerivedType()));
	}

#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "FdoFgfGeometryFactory::GetFgf() OUT\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	return FDO_SAFE_ADDREF(array.p);
}

static FdoByteArray * CreateFgfFromWkb(FdoInt32 geometryType, FdoByte * byteArray, FdoInt32 byteArraySize, FdoInt32 * numBytesRead)
{
    // Assume that byteArray points to WKB data immediately after the geometry type.
    // We'll use the available WKB data to create FGF data and then create a geometry object.

    if (NULL != numBytesRead)
        *numBytesRead = 0;
    FdoByte * bytes = byteArray;
    FdoInt32  numBytes = byteArraySize;
    FdoInt32 numBytesForOrdinates = 0;
    FdoInt32 numPositions = 0;
    FdoInt32 numRings;

    FdoInt32 dimensionality = FdoDimensionality_XY; // XY is the only one supported by WKB.

    FdoInt32 fgfNumBytes = numBytes + sizeof(geometryType) + sizeof(dimensionality);

	FdoByteArray * fgfByteArray = FdoByteArray::Create(fgfNumBytes); // Don't use smart pointer for updateable FdoArray.
    fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(geometryType), (FdoByte *) &geometryType);

	// For singleton types, just copy the array.
    // For multi types, build up FGF by looking at individual sub-geometries (recursively).
    switch(geometryType)
    {
	case FdoGeometryType_Point:
        fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(dimensionality), (FdoByte *) &dimensionality);
        numBytesForOrdinates = 2 * sizeof(double); /*X,Y*/
        fgfByteArray = FdoByteArray::Append(fgfByteArray, numBytesForOrdinates, bytes);
        bytes += numBytesForOrdinates;
        numBytes -= numBytesForOrdinates;
		break;
	case FdoGeometryType_LineString:
        fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(dimensionality), (FdoByte *) &dimensionality);
        numPositions = *((FdoInt32 *) bytes);
        bytes += sizeof(numPositions);
        numBytes -= sizeof(numPositions);
        numBytesForOrdinates = 2 * sizeof(double) * numPositions;
        fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(numPositions), (FdoByte *) &numPositions);
        fgfByteArray = FdoByteArray::Append(fgfByteArray, numBytesForOrdinates, bytes);
        bytes += numBytesForOrdinates;
        numBytes -= numBytesForOrdinates;
		break;
	case FdoGeometryType_Polygon:
        fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(dimensionality), (FdoByte *) &dimensionality);
        numRings = *((FdoInt32 *) bytes);
        bytes += sizeof(numRings);
        numBytes -= sizeof(numRings);
        fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(numRings), (FdoByte *) &numRings);

        for (FdoInt32 i = 0;  i < numRings && numBytes > 0;  i++)
        {
            numPositions = *((FdoInt32 *) bytes);
            bytes += sizeof(numPositions);
            numBytes -= sizeof(numPositions);
            numBytesForOrdinates = 2 * sizeof(double) * numPositions;
            fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(numPositions), (FdoByte *) &numPositions);
            fgfByteArray = FdoByteArray::Append(fgfByteArray, numBytesForOrdinates, bytes);
            bytes += numBytesForOrdinates;
            numBytes -= numBytesForOrdinates;
        }
		break;

	case FdoGeometryType_MultiPoint:
	case FdoGeometryType_MultiGeometry:
	case FdoGeometryType_MultiLineString:
	case FdoGeometryType_MultiPolygon:
        {
            FdoInt32 numSubGeometries = *((FdoInt32 *) bytes);
            bytes += sizeof(numSubGeometries);
            numBytes -= sizeof(numSubGeometries);
            fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(numSubGeometries), (FdoByte *) &numSubGeometries);

            for (FdoInt32 i = 0;  i < numSubGeometries && numBytes > 0;  i++)
            {
                // Skip Endian byte.
                FdoByte endian = *bytes;
	            bytes++;
	            numBytes--;
	            if (endian != WKB_LITTLE_ENDIAN)
                    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
                FdoInt32 geometryTypeSubGeom = *((FdoInt32 *) bytes);
                bytes += sizeof(geometryTypeSubGeom);
                numBytes -= sizeof(geometryTypeSubGeom);
                FdoInt32 numBytesReadSubGeom = 0;

                // Recurse on sub-geometry.
                FdoPtr<FdoByteArray> fgfByteArraySubGeom = CreateFgfFromWkb(geometryTypeSubGeom, bytes, numBytes, &numBytesReadSubGeom);
                bytes += numBytesReadSubGeom;
                numBytes -= numBytesReadSubGeom;
                fgfByteArray = FdoByteArray::Append(fgfByteArray, fgfByteArraySubGeom->GetCount(), fgfByteArraySubGeom->GetData());
            }
        }
        break;
	default:
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
    }

    if (NULL != numBytesRead)
        *numBytesRead = byteArraySize - numBytes;

	return fgfByteArray;
}

FdoIGeometry * FdoFgfGeometryFactory::CreateGeometryFromWkb(FdoByteArray * byteArray)
{
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateGeometryFromWkb() IN\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
#define MIN_WKB_SIZE (sizeof(FdoByte)+sizeof(FdoInt32)+sizeof(FdoInt32))    // endian byte, geometry type, at least an integer of data
	if ( (NULL == byteArray) || (byteArray->GetCount() < MIN_WKB_SIZE) )
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoIGeometry",
                                                               L"byteArray"));

    // Need to copy the array, skipping the initial (endian) byte and 
    // adding the dimensionality (32-bit integer after geometry type).

	FdoInt32 numBytes = byteArray->GetCount();
	FdoByte * bytes = byteArray->GetData();

    // Skip Endian byte.
    FdoByte endian = *bytes;
	bytes++;
	numBytes--;
	if (endian != WKB_LITTLE_ENDIAN)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
    // Get Geometry type.
    FdoInt32 geometryType = *((FdoInt32 *) bytes);
    bytes += sizeof(geometryType);
    numBytes -= sizeof(geometryType);

    FdoPtr<FdoByteArray> fgfByteArray = CreateFgfFromWkb(geometryType, bytes, numBytes, NULL);

    FdoPtr<FdoIGeometry> geometry = this->CreateGeometryFromFgf(fgfByteArray);

#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateGeometryFromWkb() OUT\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
    return FDO_SAFE_ADDREF(geometry.p);
}


FdoByteArray * FdoFgfGeometryFactory::GetWkb(FdoIGeometry * geometry)
{
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "FdoFgfGeometryFactory::GetWkb() IN\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
    if (NULL == geometry)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                               L"FdoFgfGeometryFactory::GetWkb",
                                                               L"geometry"));

    // WKB only supports XY, no Z or M ordinates.
    FdoInt32 dimensionality = geometry->GetDimensionality();
    if (dimensionality != FdoDimensionality_XY)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));

	FdoByteArray * wkb = NULL;   // Don't use smart pointer for updateable FdoArray.

	// For singleton types, copy the array, prepending  the initial (endian) byte
    // and stripping the dimensionality.
    // For multi types, build up WKB by looking at individual sub-geometries (recursively).
	switch ( geometry->GetDerivedType() )
	{
	case FdoGeometryType_LineString:
	case FdoGeometryType_Point:
	case FdoGeometryType_Polygon:
		{
			FdoByte endianFlag = WKB_LITTLE_ENDIAN;
			FdoPtr<FdoByteArray> fgf = this->GetFgf(geometry);
	        FdoInt32 numBytes = fgf->GetCount();
	        FdoByte * bytes = fgf->GetData();
			wkb = FdoByteArray::Create(numBytes+sizeof(endianFlag)-sizeof(dimensionality));
            FdoInt32 geometryType = *((FdoInt32 *) bytes);
            bytes += sizeof(geometryType)+ sizeof(dimensionality);
            numBytes -= sizeof(geometryType) + sizeof(dimensionality);
			wkb = FdoByteArray::Append(wkb, endianFlag);
            wkb = FdoByteArray::Append(wkb, sizeof(geometryType), (FdoByte *) &geometryType);
			wkb = FdoByteArray::Append(wkb, numBytes, bytes);
		}
		break;

	case FdoGeometryType_MultiPoint:
	case FdoGeometryType_MultiGeometry:
	case FdoGeometryType_MultiLineString:
	case FdoGeometryType_MultiPolygon:
        {
            const FdoIGeometricAggregateAbstract * multiGeom =
                static_cast<const FdoIGeometricAggregateAbstract *>(geometry);
			FdoByte endianFlag = WKB_LITTLE_ENDIAN;
            FdoInt32 geometryType = geometry->GetDerivedType();
            FdoInt32 numSubGeometries = multiGeom->GetCount();

		    wkb = FdoByteArray::Create();
			wkb = FdoByteArray::Append(wkb, endianFlag);
            wkb = FdoByteArray::Append(wkb, sizeof(geometryType), (FdoByte *) &geometryType);
            wkb = FdoByteArray::Append(wkb, sizeof(numSubGeometries), (FdoByte *) &numSubGeometries);

            for (FdoInt32 i = 0;  i < multiGeom->GetCount();  i++)
            {
                FdoPtr<FdoIGeometry> subGeom;
                switch (geometryType)
                {
                case FdoGeometryType_MultiPoint:      subGeom = static_cast<const FdoIMultiPoint *>(multiGeom)->GetItem(i);  break;
	            case FdoGeometryType_MultiGeometry:   subGeom = static_cast<const FdoIMultiGeometry *>(multiGeom)->GetItem(i);  break;
	            case FdoGeometryType_MultiLineString: subGeom = static_cast<const FdoIMultiLineString *>(multiGeom)->GetItem(i);  break;
	            case FdoGeometryType_MultiPolygon:    subGeom = static_cast<const FdoIMultiPolygon *>(multiGeom)->GetItem(i);  break;
	            default: throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
                }
                FdoPtr<FdoByteArray> subGeomWkb = GetWkb(subGeom);  // Recurse for sub-geometries.

                wkb = FdoByteArray::Append(wkb, subGeomWkb->GetCount(), subGeomWkb->GetData());
            }
        }
        break;
	default:
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
	}
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "FdoFgfGeometryFactory::GetWkb() OUT\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	return wkb;
}


// Points
/************************************************************************/
/* Create point                                                                     */
/************************************************************************/
FdoIPoint* FdoFgfGeometryFactory::CreatePoint(FdoIDirectPosition* position)
{
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreatePoint() IN\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	if (NULL == position)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoIPoint",
                                                               L"position"));

	FdoPtr<FdoIPoint> point = new FdoFgfPoint(this, m_private->GetPoolsForGeomCtor(), position);
    if (point == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreatePoint() OUT\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	return FDO_SAFE_ADDREF(point.p);
}


/************************************************************************/
/* Create point                                                                     */
/************************************************************************/
FdoIPoint* FdoFgfGeometryFactory::CreatePoint(FdoInt32 dimensionality, double* ordinates)
{
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreatePoint() IN\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	if (NULL == ordinates)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"ordinates"));

	FdoPtr<FdoIPoint> point = new FdoFgfPoint(this, m_private->GetPoolsForGeomCtor(), dimensionality, ordinates);
    if (point == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreatePoint() OUT\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	return FDO_SAFE_ADDREF(point.p);
}

// LinearRing
/************************************************************************/
/* Create LinearRing                                                                     */
/************************************************************************/
FdoILinearRing* FdoFgfGeometryFactory::CreateLinearRing(FdoDirectPositionCollection * positions)
{
    FDOPOOL_CREATE_OBJECT(
        m_private->m_geometryPools->m_PoolLinearRing, FdoPoolFgfLinearRing, 4,
        FdoFgfLinearRing,
        FdoFgfLinearRing(this, m_private->GetPoolsForGeomCtor(), positions),
        Reset(positions) );
}


/************************************************************************/
/* Crete LinearRing                                                                     */
/************************************************************************/
FdoILinearRing* FdoFgfGeometryFactory::CreateLinearRing(FdoInt32 dimtype, FdoInt32 numOrdinates, double* ordinates)
{
    FDOPOOL_CREATE_OBJECT(
        m_private->m_geometryPools->m_PoolLinearRing, FdoPoolFgfLinearRing, 4,
        FdoFgfLinearRing,
        FdoFgfLinearRing(this, m_private->GetPoolsForGeomCtor(), dimtype, numOrdinates, ordinates),
        Reset(dimtype, numOrdinates, ordinates) );
}

//LineStringSegment
/************************************************************************/
/* Create LineStringSegment                                                                     */
/************************************************************************/

FdoILineStringSegment* FdoFgfGeometryFactory::CreateLineStringSegment(FdoDirectPositionCollection * positions)
{
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateLineStringSegment() IN\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	if ( (NULL == positions) ||
        (0 == positions->GetCount()) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoILineStringSegment",
                                                               L"positions"));

	FdoPtr<FdoFgfLineStringSegment> lineSegment = new FdoFgfLineStringSegment(this, positions);

    if (lineSegment == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateLineStringSegment() OUT\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
    return FDO_SAFE_ADDREF(lineSegment.p);

}


/************************************************************************/
/* CreateLineStringSegment                                                                     */
/************************************************************************/
FdoILineStringSegment* FdoFgfGeometryFactory::CreateLineStringSegment(FdoInt32 dimensionality, FdoInt32 numOrdinates, double* ordinates)
{
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateLineStringSegment() IN\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	if ( (NULL == ordinates) ||
		 (numOrdinates <= 0) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoILineStringSegment",
                                                               L"ordinates/numOrdinates"));

	FdoPtr<FdoFgfLineStringSegment> lineSegment = new FdoFgfLineStringSegment(this, dimensionality, numOrdinates, ordinates);

    if (lineSegment == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateLineStringSegment() OUT\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
    return FDO_SAFE_ADDREF(lineSegment.p);
}

// Polygon
/************************************************************************/
/* CreatePolygon                                                                     */
/************************************************************************/
FdoIPolygon* FdoFgfGeometryFactory::CreatePolygon(FdoILinearRing* exteriorRing, FdoLinearRingCollection* interiorRings)
{
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreatePolygon() IN\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	if (NULL == exteriorRing)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoIPolygon",
                                                               L"exteriorRing"));

	FdoPtr<FdoFgfPolygon> polygon = new FdoFgfPolygon(this, m_private->GetPoolsForGeomCtor(), exteriorRing, interiorRings);

    if (polygon == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreatePolygon() OUT\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	return FDO_SAFE_ADDREF(polygon.p);
}


// MultiPoint
/************************************************************************/
/* CreateMultiPoint                                                                     */
/************************************************************************/
FdoIMultiPoint* FdoFgfGeometryFactory::CreateMultiPoint(FdoPointCollection* points)
{
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateMultiPoint() IN\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	if ( (NULL == points) ||
        (0 == points->GetCount()) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoIMultiPoint",
                                                               L"points"));

	FdoPtr<FdoFgfMultiPoint> multiPoint =  new FdoFgfMultiPoint(this, m_private->GetPoolsForGeomCtor(), points);

    if (multiPoint == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateMultiPoint() OUT\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	return FDO_SAFE_ADDREF(multiPoint.p);

}


/************************************************************************/
/* CreateMultiPoint                                                                     */
/************************************************************************/
FdoIMultiPoint* FdoFgfGeometryFactory::CreateMultiPoint(FdoInt32 dimensionality, FdoInt32 numOrdinates, double* ordinates)
{
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateMultiPoint() IN\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	if ( (NULL == ordinates) ||
		(numOrdinates <= 0) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoIMultiPoint",
                                                               L"ordinates/numOrdinates"));

	FdoPtr<FdoFgfMultiPoint> multiPoint =  new FdoFgfMultiPoint(this, m_private->GetPoolsForGeomCtor(), dimensionality, numOrdinates, ordinates);

    if (multiPoint == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateMultiPoint() OUT\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	return FDO_SAFE_ADDREF(multiPoint.p);
}


//MultiGeometry
/************************************************************************/
/* Create MultiGeometry                                                                 */
/************************************************************************/
FdoIMultiGeometry* FdoFgfGeometryFactory::CreateMultiGeometry(FdoGeometryCollection* geometries)
{
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateMultiGeometry() IN\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	if ( (NULL == geometries) ||
        (0 == geometries->GetCount()) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoIMultiGeometry",
                                                               L"geometries"));

	FdoPtr<FdoFgfMultiGeometry> multiGeometry =  new FdoFgfMultiGeometry(this, m_private->GetPoolsForGeomCtor(), geometries);

    if (multiGeometry == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateMultiGeometry() OUT\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	return FDO_SAFE_ADDREF(multiGeometry.p);
}


//MultiLineString
/************************************************************************/
/* Create MultiLineString                                                                     */
/************************************************************************/
FdoIMultiLineString* FdoFgfGeometryFactory::CreateMultiLineString(FdoLineStringCollection* lineStrings)
{
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateMultiLineString() IN\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	if ( (NULL == lineStrings) ||
        (0 == lineStrings->GetCount()) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoIMultiLineString",
                                                               L"lineStrings"));

	FdoPtr<FdoFgfMultiLineString> multiLineString =  new FdoFgfMultiLineString(this, m_private->GetPoolsForGeomCtor(), lineStrings);

    if (multiLineString == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateMultiLineString() OUT\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	return FDO_SAFE_ADDREF(multiLineString.p);
}



//MultiPolygon
/************************************************************************/
/* Create MultiPolygon                                                                     */
/************************************************************************/
FdoIMultiPolygon* FdoFgfGeometryFactory::CreateMultiPolygon(FdoPolygonCollection* polygons)
{
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateMultiPolygon() IN\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	if ( (NULL == polygons) ||
        (0 == polygons->GetCount()) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoIMultiPolygon",
                                                               L"polygons"));

	FdoPtr<FdoFgfMultiPolygon> multiPolygon =  new FdoFgfMultiPolygon(this, m_private->GetPoolsForGeomCtor(), polygons);

    if (multiPolygon == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateMultiPolygon() OUT\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	return FDO_SAFE_ADDREF(multiPolygon.p);
}

// CircularArcSegment
/************************************************************************/
/* Create CircularArcSegment                                                                     */
/************************************************************************/
FdoICircularArcSegment* FdoFgfGeometryFactory::CreateCircularArcSegment(FdoIDirectPosition* startPoint, FdoIDirectPosition* midPoint, FdoIDirectPosition* endPoint)
{
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateCircularArcSegment() IN\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	if ( (NULL == startPoint) ||
		 (NULL == midPoint) ||
		 (NULL == endPoint) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoICircularArcSegment",
                                                               L"startPoint/midPoint/endPoint"));

	FdoPtr<FdoFgfCircularArcSegment> circArcSegment = new FdoFgfCircularArcSegment(
        this, startPoint, midPoint, endPoint);
    if (circArcSegment == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateCircularArcSegment() OUT\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	return FDO_SAFE_ADDREF(circArcSegment.p);
	
}

// CurveString
/************************************************************************/
/* CreateCurveString                                                                     */
/************************************************************************/
FdoICurveString* FdoFgfGeometryFactory::CreateCurveString(FdoCurveSegmentCollection* curveSegments)
{
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateCurveString() IN\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	if ( (NULL == curveSegments) ||
        (0 == curveSegments->GetCount()) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoICurveString",
                                                               L"curveSegments"));

	FdoPtr<FdoFgfCurveString> curveString = new FdoFgfCurveString(this, m_private->GetPoolsForGeomCtor(), curveSegments);

    if (curveString == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateCurveString() OUT\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	return FDO_SAFE_ADDREF(curveString.p);
}

// MultiCurveString
/************************************************************************/
/* CreateMultiCurveString                                                                     */
/************************************************************************/
// MultiCurveString
FdoIMultiCurveString* FdoFgfGeometryFactory::CreateMultiCurveString(FdoCurveStringCollection* curveStrings)
{
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateMultiCurveString() IN\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
    if ( (NULL == curveStrings) || (0 == curveStrings->GetCount()) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoIMultiCurveString",
                                                               L"curveStrings"));

	FdoPtr<FdoFgfMultiCurveString> multiCurveString = new FdoFgfMultiCurveString(this, m_private->GetPoolsForGeomCtor(), curveStrings);

    if (multiCurveString == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateMultiCurveString() OUT\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	return FDO_SAFE_ADDREF(multiCurveString.p);
}


// Ring
/************************************************************************/
/* CreateRing                                                                     */
/************************************************************************/
FdoIRing* FdoFgfGeometryFactory::CreateRing(FdoCurveSegmentCollection* curveSegments)
{
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateRing() IN\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	if ( (NULL == curveSegments) ||
        (0 == curveSegments->GetCount()) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoIRing",
                                                               L"curveSegments"));

	FdoPtr<FdoFgfRing> ring = new FdoFgfRing(this, curveSegments);

    if (ring == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateRing() OUT\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	return FDO_SAFE_ADDREF(ring.p);
}



// CurvePolygon
/************************************************************************/
/* CreateCurvePolygon()                                                                     */
/************************************************************************/
FdoICurvePolygon* FdoFgfGeometryFactory::CreateCurvePolygon(FdoIRing* exteriorRing, FdoRingCollection* interiorRings)
{
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateCurvePolygon() IN\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	if (NULL == exteriorRing)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoICurvePolygon",
                                                               L"exteriorRing"));

	FdoPtr<FdoFgfCurvePolygon> curvePolygon = new FdoFgfCurvePolygon(this, m_private->GetPoolsForGeomCtor(), exteriorRing, interiorRings);

    if (curvePolygon == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateCurvePolygon() OUT\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	return FDO_SAFE_ADDREF(curvePolygon.p);

}


// MultiCurvePolygon
/************************************************************************/
/* CreateMultiCurvePolygon                                                                     */
/************************************************************************/
FdoIMultiCurvePolygon* FdoFgfGeometryFactory::CreateMultiCurvePolygon(FdoCurvePolygonCollection* curvePolygons)
{
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateMultiCurvePolygon() IN\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
    if ( (NULL == curvePolygons) || 0 == curvePolygons->GetCount() )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoIMultiCurvePolygon",
                                                               L"curvePolygons"));

	FdoPtr<FdoFgfMultiCurvePolygon> multiCurvePolygon = new FdoFgfMultiCurvePolygon(this, m_private->GetPoolsForGeomCtor(), curvePolygons);

    if (multiCurvePolygon == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateMultiCurvePolygon() OUT\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	return FDO_SAFE_ADDREF(multiCurvePolygon.p);

}


FdoIGeometry * FdoFgfGeometryFactory::CreateGeometry(FdoIEnvelope * envelope)
{
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateGeometry(envelope) IN\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
    int dimensionality = FdoDimensionality_XY;
    FdoPtr<FdoILinearRing> ring;

#ifdef _WIN32
    if (_isnan(envelope->GetMinZ()) || _isnan(envelope->GetMaxZ()))
#else
    if (isnan(envelope->GetMinZ()) || isnan(envelope->GetMaxZ()))
#endif
    {
        double ordinates [10] = 
        {
            envelope->GetMinX(), envelope->GetMinY(),
            envelope->GetMaxX(), envelope->GetMinY(),
            envelope->GetMaxX(), envelope->GetMaxY(),
            envelope->GetMinX(), envelope->GetMaxY(),
            envelope->GetMinX(), envelope->GetMinY()
        };
        ring = CreateLinearRing(dimensionality, 10, ordinates);
    }
    else
    {
        dimensionality |= FdoDimensionality_Z;

        double ordinates [15] = 
        {
            // Use minimum Z as a tentative value.  In the target use
            // case, spatial query, it will be ignored anyway.
            envelope->GetMinX(), envelope->GetMinY(), envelope->GetMinZ(),
            envelope->GetMaxX(), envelope->GetMinY(), envelope->GetMinZ(),
            envelope->GetMaxX(), envelope->GetMaxY(), envelope->GetMinZ(),
            envelope->GetMinX(), envelope->GetMaxY(), envelope->GetMinZ(),
            envelope->GetMinX(), envelope->GetMinY(), envelope->GetMinZ()
        };
        ring = CreateLinearRing(dimensionality, 15, ordinates);
    }


    FdoPtr<FdoIPolygon> geometry = CreatePolygon(ring, NULL);

#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateGeometry(envelope) OUT\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
    return FDO_SAFE_ADDREF(geometry.p);
}

FdoIGeometry* FdoFgfGeometryFactory::CreateGeometry(FdoString* fgft)
{
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateGeometry(fgft) IN\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	FdoPtr<FdoIGeometry>	geometry;
	FdoParseFgft*	parse = new FdoParseFgft();
	if (parse != NULL)
	{
        try
        {
    		geometry = parse->ParseFgft(fgft);
        }
        catch (FdoException* exception)
        {
            delete parse;
            throw exception;
        }
		delete parse;
	}
#ifdef EXTRA_DEBUG
    if (NULL != m_private->m_geometryPools->m_fdoDebugFile)
    {
        fprintf(m_private->m_geometryPools->m_fdoDebugFile, "CreateGeometry(fgft) OUT\n");
        fflush(m_private->m_geometryPools->m_fdoDebugFile);
    }
#endif
	return FDO_SAFE_ADDREF(geometry.p);
}


FdoByteArray * FdoFgfGeometryPools::GetByteArray()
{
    if (m_PoolByteArray == NULL)
        m_PoolByteArray = FdoPoolFgfByteArray::Create(10);

    // We avoid smart pointers because updateable FdoArray types cannot have
    // shared ownership, since they can move around in memory.

    FdoByteArray * byteArray = NULL;
    byteArray = m_PoolByteArray->FindReusableItem();

    if (byteArray == NULL)
    {
	    byteArray = FdoByteArray::Create(172); // A guess: endian+type+dim+20ordinates
#ifdef EXTRA_DEBUG
        m_numByteArraysCreated++;
#endif
    }
    else
    {
        byteArray = byteArray->SetSize(byteArray, 0);
    }

    return byteArray;
}

FdoByteArray * FdoFgfGeometryFactory::GetByteArray()
{
    return this->m_private->m_geometryPools->GetByteArray();
}

void FdoFgfGeometryPools::TakeReleasedByteArray(FdoByteArray * byteArray)
{
	if (0 == byteArray)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                               L"FdoFgfGeometryFactory::TakeReleasedByteArray",
                                                               L"byteArray"));

    if (m_PoolByteArray == NULL)
        m_PoolByteArray = FdoPoolFgfByteArray::Create(10);

    m_PoolByteArray->AddItem((FdoByteArray *)byteArray);
}

void FdoFgfGeometryFactory::TakeReleasedByteArray(FdoByteArray * byteArray)
{
    this->m_private->m_geometryPools->TakeReleasedByteArray(byteArray);
}


#define DEFINE_CREATE_POOLED_GEOMETRY(type) \
    FdoI##type * FdoFgfGeometryPools::Create##type( \
        FdoFgfGeometryFactory * factory, \
        FdoFgfGeometryPools * pools, \
        FdoByteArray * byteArray, \
        const FdoByte * data, \
        FdoInt32 count \
        ) \
    { \
        FDOPOOL_CREATE_OBJECT( \
            m_Pool##type, FdoPoolFgf##type, 4, \
            FdoFgf##type, \
            FdoFgf##type(factory, pools, byteArray, data, count), \
            Reset(byteArray, data, count) ); \
    }

DEFINE_CREATE_POOLED_GEOMETRY(LineString);
DEFINE_CREATE_POOLED_GEOMETRY(Point);
DEFINE_CREATE_POOLED_GEOMETRY(Polygon);
DEFINE_CREATE_POOLED_GEOMETRY(MultiPoint);
DEFINE_CREATE_POOLED_GEOMETRY(MultiGeometry);
DEFINE_CREATE_POOLED_GEOMETRY(MultiLineString);
DEFINE_CREATE_POOLED_GEOMETRY(MultiPolygon);
DEFINE_CREATE_POOLED_GEOMETRY(CurveString);
DEFINE_CREATE_POOLED_GEOMETRY(MultiCurveString);
DEFINE_CREATE_POOLED_GEOMETRY(CurvePolygon);
DEFINE_CREATE_POOLED_GEOMETRY(MultiCurvePolygon);


/************************************************************************/
/* Dispose()                                                                     */
/************************************************************************/
void FdoFgfGeometryFactory::Dispose()
{
    delete this;
}
