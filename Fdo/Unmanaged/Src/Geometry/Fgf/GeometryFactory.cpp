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

#ifdef EXTRA_DEBUG
    FILE * FdoDebugFile = NULL;
#endif


FdoFgfGeometryFactory* FdoFgfGeometryFactory::GetInstance()
{
    FdoGeometryThreadData * threadData = FdoGeometryThreadData::GetValue();
    if (threadData->geometryFactory == NULL)
    {
        // Create a new geometry factory, which will be the only actual instance
        // for this thread.  It will be released automatically when the thread
        // releases the thread data upon exit.  That happens when
        // code in FdoGeometryDll.cpp calls FdoGeometryThreadData::ReleaseValue().

        threadData->geometryFactory = new FdoFgfGeometryFactory();
        if (NULL == threadData->geometryFactory)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
    }

#ifdef EXTRA_DEBUG
    if (NULL == FdoDebugFile)
    {
        FdoDebugFile = fopen("D:\\temp\\FdoDebugFile.log", "w+");
    }
#endif
	return FDO_SAFE_ADDREF((threadData->geometryFactory).p);
}

FdoFgfGeometryFactory * FdoFgfGeometryFactory::GetPrivateInstance(
    FdoInt32 numGeometries,
    FdoInt32 numPositions,
    FdoInt32 numEnvelopes,
    FdoInt32 numCurveSegments,
    FdoInt32 numRings)
{
	throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
}


// Constructor
FdoFgfGeometryFactory::FdoFgfGeometryFactory()
{
    m_private = new FdoFgfGeometryFactory2();
    if (NULL == m_private)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
}


// Destructor
FdoFgfGeometryFactory::~FdoFgfGeometryFactory()
{
    delete m_private;

#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "FdoFgfGeometryFactory::~FdoFgfGeometryFactory()\n");
        fflush(FdoDebugFile);
        PrintStats();
    }
#endif
}

#ifdef EXTRA_DEBUG
void FdoFgfGeometryFactory2::PrintStats()
{
    if (NULL != FdoDebugFile)
    {
        FdoInt32 ehits=0, rhits=0, misses=0;
        fprintf(FdoDebugFile, "  FdoFgfGeometryFactory::PrintStats():\n");
        fprintf(FdoDebugFile, "    %d LinearRings created.\n", m_private->m_numLinearRingsCreated);
        if (m_private->m_linearRingPool != NULL)
        {
            this->m_private->m_linearRingPool->GetStats(&ehits, &rhits, &misses);
            fprintf(FdoDebugFile,
                "      Pool stats: %d empty hits, %d reusable hits, %d misses\n",
                ehits, rhits, misses);
        }
        fflush(FdoDebugFile);
    }
}
#endif

FdoILineString* FdoFgfGeometryFactory::CreateLineString(FdoDirectPositionCollection * positions)
{
#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateLineString() IN\n");
        fflush(FdoDebugFile);
    }
#endif
	if ( (NULL == positions) ||
        ( 0 == positions->GetCount()) )
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	FdoPtr<FdoILineString> lineString = new FdoFgfLineString(this, positions);

    if (lineString == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateLineString() OUT\n");
        fflush(FdoDebugFile);
    }
#endif
	return FDO_SAFE_ADDREF(lineString.p);
}

FdoILineString* FdoFgfGeometryFactory::CreateLineString(FdoInt32 dimensionType, FdoInt32 numOrdinates, double* ordinates)
{
#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateLineString() IN\n");
        fflush(FdoDebugFile);
    }
#endif
	if ( (numOrdinates <= 0) ||
		 (NULL == ordinates) )
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	FdoPtr<FdoILineString> geometry = new FdoFgfLineString(this, dimensionType, numOrdinates, ordinates);

    if (geometry == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateLineString() OUT\n");
        fflush(FdoDebugFile);
    }
#endif
	return FDO_SAFE_ADDREF(geometry.p);
}

// Pseudo copy constructor; this method can convert between implementations.
FdoIGeometry * FdoFgfGeometryFactory::CreateGeometry(FdoIGeometry * geometry)
{
#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateGeometry(geometry=0x%lx) type=%d IN\n",
            (long)geometry, (int)(geometry->GetDerivedType()));
        fflush(FdoDebugFile);
        try
        {
            //if (geometry->GetDerivedType() == FdoGeometryType_LineString)
            {
                FdoString * fgftString = geometry->GetText();
                fprintf(FdoDebugFile, "  fgft='%S'\n", fgftString);
                fflush(FdoDebugFile);
            }
        }
        catch ( FdoException* e )
        {
            fprintf(FdoDebugFile, "  Caught FdoException from GetText(): <<%S>>\n", e->GetExceptionMessage());
            fflush(FdoDebugFile);
        }
        catch (...)
        {
            fprintf(FdoDebugFile, "  Caught unknown exception from GetText().\n");
            fflush(FdoDebugFile);
        }

    }
#endif

    FdoByteArray * newByteArray = this->GetByteArray();

    FgfUtil::WriteGeometry(geometry, &newByteArray);

    FdoPtr<FdoIGeometry> newGeometry = this->CreateGeometryFromFgf(newByteArray, NULL, 0);

    FDO_SAFE_RELEASE(newByteArray);
    newByteArray = NULL;

#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateGeometry() OUT\n",
            (long)geometry);
        fflush(FdoDebugFile);
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
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateGeometryFromFgf(0x%lx, 0x%lx, count=%d) IN\n",
            (long)byteArray, (long)byteArrayData, count);
        fflush(FdoDebugFile);
    }
#endif
	FdoPtr<FdoIGeometry> newGeometry;

    if (m_private->m_geometryPool2 == NULL)
    {
        m_private->m_geometryPool2 = FdoGeometryCollection::Create();
    }

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

    FdoInt32        pooledGeometryToReplaceIndex = -1;
    FdoInt32        pooledGeometryToReassignIndex = -1;
    bool            done = false;

    for (FdoInt32 i = 0;  !done && i < m_private->m_geometryPool2->GetCount(); i++)
    {
        FdoPtr<FdoIGeometry>  pooledGeometry = m_private->m_geometryPool2->GetItem(i);

        if (pooledGeometry != NULL)
        {
            FdoGeometryType pooledGeometryType = pooledGeometry->GetDerivedType();
            if (pooledGeometryType == geometryType)
            {
                // New FGF is the same type as the pooled geometry.
                FdoInt32 refCount = GET_REFCOUNT(pooledGeometry.p);
                if (2 == refCount)  // 2 == local variable pointer + pool pointer
                {
                    // This factory is the exclusive owner of this pooled geometry.  Just re-assign its FGF.
                    pooledGeometryToReassignIndex = i;
                }
                else
                {
                    // We found a geometry of the correct type, but we aren't the excusive owner.
                    pooledGeometryToReplaceIndex = i;
                }
                done = true;    // Done, whether we re-used the geometry or not (only pool 1 of each type)
            }
        }
    }

#define CASE_REASSIGN_FGF(type) \
    case FdoGeometryType_##type: \
        { \
            newGeometry = m_private->m_geometryPool2->GetItem(pooledGeometryToReassignIndex); \
            FdoFgf##type * derivedGeom = (FdoFgf##type *)(newGeometry.p); \
            derivedGeom->SetFgf(byteArray, byteArrayData, count); \
        } \
        break;

#define CASE_CREATE_GEOMETRY(type) \
    case FdoGeometryType_##type: \
        newGeometry = new FdoFgf##type(this, byteArray, byteArrayData, count); \
        break;

    if (pooledGeometryToReassignIndex >= 0)
    {
        // Call SetFgf() on the appropriate type.  Unfortunately, we cannot call it
        // directly on the base FdoFgfGeometryImpl type because it's a template on an 
        // abstract type (FdoIGeometry).
	    switch ( geometryType )
	    {
        CASE_REASSIGN_FGF(LineString);
        CASE_REASSIGN_FGF(Point);
        CASE_REASSIGN_FGF(Polygon);
        CASE_REASSIGN_FGF(MultiPoint);
        CASE_REASSIGN_FGF(MultiGeometry);
        CASE_REASSIGN_FGF(MultiLineString);
        CASE_REASSIGN_FGF(MultiPolygon);
        CASE_REASSIGN_FGF(CurveString);
        CASE_REASSIGN_FGF(MultiCurveString);
        CASE_REASSIGN_FGF(CurvePolygon);
        CASE_REASSIGN_FGF(MultiCurvePolygon);
	    default:
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));
        }
    }
    else
    {
        // We did not get anything from the pool.  Really create a new geometry.

	    switch ( geometryType )
	    {
	    CASE_CREATE_GEOMETRY(LineString);
	    CASE_CREATE_GEOMETRY(Point);
	    CASE_CREATE_GEOMETRY(Polygon);
	    CASE_CREATE_GEOMETRY(MultiPoint);
	    CASE_CREATE_GEOMETRY(MultiGeometry);
	    CASE_CREATE_GEOMETRY(MultiLineString);
	    CASE_CREATE_GEOMETRY(MultiPolygon);
	    CASE_CREATE_GEOMETRY(CurveString);
	    CASE_CREATE_GEOMETRY(MultiCurveString);
	    CASE_CREATE_GEOMETRY(CurvePolygon);
	    CASE_CREATE_GEOMETRY(MultiCurvePolygon);
	    default:
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));
	    }

        // Update the pool.
        if (pooledGeometryToReplaceIndex >= 0)
        {
            m_private->m_geometryPool2->SetItem(pooledGeometryToReplaceIndex, newGeometry);
        }
        else
        {
            (void) m_private->m_geometryPool2->Add(newGeometry);
        }
    } // end creation of new geometry

    if (newGeometry == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateGeometryFromFgf() OUT\n",
            (long)byteArray, (long)byteArrayData, count);
        fflush(FdoDebugFile);
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
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "FdoFgfGeometryFactory::GetFgf() IN\n");
        fflush(FdoDebugFile);
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
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));
	}

#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "FdoFgfGeometryFactory::GetFgf() OUT\n");
        fflush(FdoDebugFile);
    }
#endif
	return FDO_SAFE_ADDREF(array.p);
}

static FdoByteArray * CreateFgfFromWkb(FdoInt32 geometryType, FdoByte * byteArray, FdoInt32 byteArraySize, FdoInt32 * numBytesRead)
{
#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateFgfFromWkb() IN\n");
        fflush(FdoDebugFile);
    }
#endif
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

#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateFgfFromWkb() OUT\n");
        fflush(FdoDebugFile);
    }
#endif
	return fgfByteArray;
}

FdoIGeometry * FdoFgfGeometryFactory::CreateGeometryFromWkb(FdoByteArray * byteArray)
{
#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateGeometryFromWkb() IN\n");
        fflush(FdoDebugFile);
    }
#endif
#define MIN_WKB_SIZE (sizeof(FdoByte)+sizeof(FdoInt32)+sizeof(FdoInt32))    // endian byte, geometry type, at least an integer of data
	if ( (NULL == byteArray) || (byteArray->GetCount() < MIN_WKB_SIZE) )
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

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
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateGeometryFromWkb() OUT\n");
        fflush(FdoDebugFile);
    }
#endif
    return FDO_SAFE_ADDREF(geometry.p);
}


FdoByteArray * FdoFgfGeometryFactory::GetWkb(FdoIGeometry * geometry)
{
#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "FdoFgfGeometryFactory::GetWkb() IN\n");
        fflush(FdoDebugFile);
    }
#endif
    if (NULL == geometry)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

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
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "FdoFgfGeometryFactory::GetWkb() OUT\n");
        fflush(FdoDebugFile);
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
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreatePoint() IN\n");
        fflush(FdoDebugFile);
    }
#endif
	if (NULL == position)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	FdoPtr<FdoIPoint> point = new FdoFgfPoint(this, position);
    if (point == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreatePoint() OUT\n");
        fflush(FdoDebugFile);
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
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreatePoint() IN\n");
        fflush(FdoDebugFile);
    }
#endif
	if (NULL == ordinates)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	FdoPtr<FdoIPoint> point = new FdoFgfPoint(this, dimensionality, ordinates);
    if (point == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreatePoint() OUT\n");
        fflush(FdoDebugFile);
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
#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateLinearRing() IN\n");
        fflush(FdoDebugFile);
    }
#endif

    if (m_private->m_linearRingPool == NULL)
        m_private->m_linearRingPool = FdoFgfLinearRingCache::Create(4);

    FdoPtr<FdoFgfLinearRing> lineRing = m_private->m_linearRingPool->FindReusableItem();

    if (lineRing == NULL)
    {
	    lineRing = new FdoFgfLinearRing(this, positions);
        if (lineRing == NULL)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
    }
    else
    {
        lineRing->Reset(positions);
    }

    m_private->m_linearRingPool->AddItem(lineRing);

#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateLinearRing() OUT\n");
        fflush(FdoDebugFile);
        PrintStats();
    }
#endif
	return FDO_SAFE_ADDREF(lineRing.p);
}


/************************************************************************/
/* Crete LinearRing                                                                     */
/************************************************************************/
FdoILinearRing* FdoFgfGeometryFactory::CreateLinearRing(FdoInt32 dimtype, FdoInt32 numOrdinates, double* ordinates)
{
#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateLinearRing() IN\n");
        fflush(FdoDebugFile);
    }
#endif
	if ( (NULL == ordinates) ||
		 (numOrdinates <= 0) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

    if (m_private->m_linearRingPool == NULL)
        m_private->m_linearRingPool = FdoFgfLinearRingCache::Create(4);

    FdoPtr<FdoFgfLinearRing> lineRing = m_private->m_linearRingPool->FindReusableItem();

    if (lineRing == NULL)
    {
	    lineRing = new FdoFgfLinearRing(this, dimtype, numOrdinates, ordinates);
        if (lineRing == NULL)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
    }
    else
    {
        lineRing->Reset(dimtype, numOrdinates, ordinates);
    }

    m_private->m_linearRingPool->AddItem(lineRing);

#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateLinearRing() OUT\n");
        fflush(FdoDebugFile);
        PrintStats();
    }
#endif
	return FDO_SAFE_ADDREF(lineRing.p);
}

//LineStringSegment
/************************************************************************/
/* Create LineStringSegment                                                                     */
/************************************************************************/

FdoILineStringSegment* FdoFgfGeometryFactory::CreateLineStringSegment(FdoDirectPositionCollection * positions)
{
#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateLineStringSegment() IN\n");
        fflush(FdoDebugFile);
    }
#endif
	if ( (NULL == positions) ||
        (0 == positions->GetCount()) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	FdoPtr<FdoFgfLineStringSegment> lineSegment = new FdoFgfLineStringSegment(this, positions);

    if (lineSegment == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateLineStringSegment() OUT\n");
        fflush(FdoDebugFile);
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
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateLineStringSegment() IN\n");
        fflush(FdoDebugFile);
    }
#endif
	if ( (NULL == ordinates) ||
		 (numOrdinates <= 0) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	FdoPtr<FdoFgfLineStringSegment> lineSegment = new FdoFgfLineStringSegment(this, dimensionality, numOrdinates, ordinates);

    if (lineSegment == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateLineStringSegment() OUT\n");
        fflush(FdoDebugFile);
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
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreatePolygon() IN\n");
        fflush(FdoDebugFile);
    }
#endif
	if (NULL == exteriorRing)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	FdoPtr<FdoFgfPolygon> polygon = new FdoFgfPolygon(this, exteriorRing, interiorRings);

    if (polygon == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreatePolygon() OUT\n");
        fflush(FdoDebugFile);
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
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateMultiPoint() IN\n");
        fflush(FdoDebugFile);
    }
#endif
	if ( (NULL == points) ||
        (0 == points->GetCount()) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	FdoPtr<FdoFgfMultiPoint> multiPoint =  new FdoFgfMultiPoint(this, points);

    if (multiPoint == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateMultiPoint() OUT\n");
        fflush(FdoDebugFile);
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
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateMultiPoint() IN\n");
        fflush(FdoDebugFile);
    }
#endif
	if ( (NULL == ordinates) ||
		(numOrdinates <= 0) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	FdoPtr<FdoFgfMultiPoint> multiPoint =  new FdoFgfMultiPoint(this, dimensionality, numOrdinates, ordinates);

    if (multiPoint == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateMultiPoint() OUT\n");
        fflush(FdoDebugFile);
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
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateMultiGeometry() IN\n");
        fflush(FdoDebugFile);
    }
#endif
	if ( (NULL == geometries) ||
        (0 == geometries->GetCount()) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	FdoPtr<FdoFgfMultiGeometry> multiGeometry =  new FdoFgfMultiGeometry(this, geometries);

    if (multiGeometry == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateMultiGeometry() OUT\n");
        fflush(FdoDebugFile);
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
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateMultiLineString() IN\n");
        fflush(FdoDebugFile);
    }
#endif
	if ( (NULL == lineStrings) ||
        (0 == lineStrings->GetCount()) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	FdoPtr<FdoFgfMultiLineString> multiLineString =  new FdoFgfMultiLineString(this, lineStrings);

    if (multiLineString == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateMultiLineString() OUT\n");
        fflush(FdoDebugFile);
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
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateMultiPolygon() IN\n");
        fflush(FdoDebugFile);
    }
#endif
	if ( (NULL == polygons) ||
        (0 == polygons->GetCount()) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	FdoPtr<FdoFgfMultiPolygon> multiPolygon =  new FdoFgfMultiPolygon(this, polygons);

    if (multiPolygon == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateMultiPolygon() OUT\n");
        fflush(FdoDebugFile);
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
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateCircularArcSegment() IN\n");
        fflush(FdoDebugFile);
    }
#endif
	if ( (NULL == startPoint) ||
		 (NULL == midPoint) ||
		 (NULL == endPoint) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	FdoPtr<FdoFgfCircularArcSegment> circArcSegment = new FdoFgfCircularArcSegment(
        this, startPoint, midPoint, endPoint);
    if (circArcSegment == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateCircularArcSegment() OUT\n");
        fflush(FdoDebugFile);
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
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateCurveString() IN\n");
        fflush(FdoDebugFile);
    }
#endif
	if ( (NULL == curveSegments) ||
        (0 == curveSegments->GetCount()) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	FdoPtr<FdoFgfCurveString> curveString = new FdoFgfCurveString(this, curveSegments);

    if (curveString == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateCurveString() OUT\n");
        fflush(FdoDebugFile);
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
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateMultiCurveString() IN\n");
        fflush(FdoDebugFile);
    }
#endif
    if ( (NULL == curveStrings) || (0 == curveStrings->GetCount()) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	FdoPtr<FdoFgfMultiCurveString> multiCurveString = new FdoFgfMultiCurveString(this, curveStrings);

    if (multiCurveString == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateMultiCurveString() OUT\n");
        fflush(FdoDebugFile);
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
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateRing() IN\n");
        fflush(FdoDebugFile);
    }
#endif
	if ( (NULL == curveSegments) ||
        (0 == curveSegments->GetCount()) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	FdoPtr<FdoFgfRing> ring = new FdoFgfRing(this, curveSegments);

    if (ring == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateRing() OUT\n");
        fflush(FdoDebugFile);
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
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateCurvePolygon() IN\n");
        fflush(FdoDebugFile);
    }
#endif
	if (NULL == exteriorRing)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	FdoPtr<FdoFgfCurvePolygon> curvePolygon = new FdoFgfCurvePolygon(this, exteriorRing, interiorRings);

    if (curvePolygon == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateCurvePolygon() OUT\n");
        fflush(FdoDebugFile);
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
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateMultiCurvePolygon() IN\n");
        fflush(FdoDebugFile);
    }
#endif
    if ( (NULL == curvePolygons) || 0 == curvePolygons->GetCount() )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	FdoPtr<FdoFgfMultiCurvePolygon> multiCurvePolygon = new FdoFgfMultiCurvePolygon(this, curvePolygons);

    if (multiCurvePolygon == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateMultiCurvePolygon() OUT\n");
        fflush(FdoDebugFile);
    }
#endif
	return FDO_SAFE_ADDREF(multiCurvePolygon.p);

}


FdoIGeometry * FdoFgfGeometryFactory::CreateGeometry(FdoIEnvelope * envelope)
{
#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateGeometry(envelope) IN\n");
        fflush(FdoDebugFile);
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
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateGeometry(envelope) OUT\n");
        fflush(FdoDebugFile);
    }
#endif
    return FDO_SAFE_ADDREF(geometry.p);
}

FdoIGeometry* FdoFgfGeometryFactory::CreateGeometry(FdoString* fgft)
{
#ifdef EXTRA_DEBUG
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateGeometry(fgft) IN\n");
        fflush(FdoDebugFile);
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
    if (NULL != FdoDebugFile)
    {
        fprintf(FdoDebugFile, "CreateGeometry(fgft) OUT\n");
        fflush(FdoDebugFile);
    }
#endif
	return FDO_SAFE_ADDREF(geometry.p);
}

FdoByteArray * FdoFgfGeometryFactory::GetByteArray()
{
    if (m_private->m_byteArrayPool == NULL)
        m_private->m_byteArrayPool = FdoByteArrayCache::Create(10);

    // Acquire or create a byte array, whose ownership we'll hand completely 
    // over to the writer.  Updateable FdoArray types cannot have shared ownership,
    // since they can move around in memory.  We also avoid smart pointers with
    // updateable FdoArray types for the same reason.

    FdoByteArray * byteArray = NULL;
#ifdef POOL_BYTEARRAYS
    byteArray = m_private->m_byteArrayPool->FindReusableItem();
#endif

    if (byteArray == NULL)
    {
	    byteArray = FdoByteArray::Create(172); // A guess: endian+type+dim+20ordinates
    }
    else
    {
        byteArray = FdoByteArray::SetSize(byteArray, 0);
    }

    return byteArray;
}


void FdoFgfGeometryFactory::TakeReleasedByteArray(FdoByteArray * byteArray)
{
	if (0 == byteArray)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

    if (m_private->m_byteArrayPool == NULL)
        m_private->m_byteArrayPool = FdoByteArrayCache::Create(10);

#ifdef POOL_BYTEARRAYS
    m_private->m_byteArrayPool->AddItem(byteArray);
#endif
}


/************************************************************************/
/* Dispose()                                                                     */
/************************************************************************/
void FdoFgfGeometryFactory::Dispose()
{
    delete this;
}
