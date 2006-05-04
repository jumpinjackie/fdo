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



#include "GeometryUtility.h"
#include "StringUtility.h"

FdoString* GeometryUtility::DimensionalityToFgftString(FdoInt32 dimensionality)
{
    FdoString* value = 0;
    static FdoString* xy = L"";          // Don't specify XY (compatible with OpenGIS WKT)
    static FdoString* xyz = L"XYZ ";
    static FdoString* xym = L"XYM ";
    static FdoString* xyzm = L"XYZM ";

    if ( dimensionality == FdoDimensionality_XY )
        value = xy;
    else if ( dimensionality == ( FdoDimensionality_XY | FdoDimensionality_Z ) )
        value = xyz;
    else if ( dimensionality == ( FdoDimensionality_XY | FdoDimensionality_M ) )
        value = xym;
    else if ( dimensionality == ( FdoDimensionality_XY | FdoDimensionality_Z | FdoDimensionality_M ) )
        value = xyzm;
    else
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

    return value;
}

FdoInt32 GeometryUtility::DimensionalityToNumOrdinates(FdoInt32 dimensionality)
{
    FdoInt32 value = 0;

    if ( dimensionality == FdoDimensionality_XY )
        value = 2;
    else if ( dimensionality == ( FdoDimensionality_XY | FdoDimensionality_Z ) )
        value = 3;
    else if ( dimensionality == ( FdoDimensionality_XY | FdoDimensionality_M ) )
        value = 3;
    else if ( dimensionality == ( FdoDimensionality_XY | FdoDimensionality_Z | FdoDimensionality_M ) )
        value = 4;
    else
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

    return value;
}


wchar_t * GeometryUtility::AllocateStringForPositions(FdoInt32 dimensionality, FdoInt32 numPositions)
{
    FdoInt32 numOrdinates = DimensionalityToNumOrdinates(dimensionality) * numPositions;
    FdoInt32 numChars = numOrdinates * GeometryUtility::GenerousCharsPerOrdinate;
    wchar_t * value = new wchar_t[numChars];
    if (0 == value)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
    value[0] = L'\0';

    return value;
}


void GeometryUtility::AppendPositionToString(wchar_t * string, const FdoIDirectPosition * position)
{
    static FdoString* ordinateSeparator = L" ";
    wchar_t ordinateBuffer[GeometryUtility::GenerousCharsPerOrdinate];

    // Convert X ordinate to string and append to string buffer, which had better be
    // big enough -- see AllocateStringForPositions().
    FdoStringUtility::FormatDouble(position->GetX(), ordinateBuffer, GeometryUtility::GenerousCharsPerOrdinate);
    FdoStringUtility::StringConcatenate(string, ordinateBuffer);

    // Append Y ordinate.
    FdoStringUtility::StringConcatenate(string, ordinateSeparator);
    FdoStringUtility::FormatDouble(position->GetY(), ordinateBuffer, GeometryUtility::GenerousCharsPerOrdinate);
    FdoStringUtility::StringConcatenate(string, ordinateBuffer);

    if ( (position->GetDimensionality() & FdoDimensionality_Z ) != 0 )
    {
        // Append Z ordinate.
        FdoStringUtility::StringConcatenate(string, ordinateSeparator);
        FdoStringUtility::FormatDouble(position->GetZ(), ordinateBuffer, GeometryUtility::GenerousCharsPerOrdinate);
        FdoStringUtility::StringConcatenate(string, ordinateBuffer);
    }

    if ( (position->GetDimensionality() & FdoDimensionality_M ) != 0 )
    {
        // Append M ordinate.
        FdoStringUtility::StringConcatenate(string, ordinateSeparator);
        FdoStringUtility::FormatDouble(position->GetM(), ordinateBuffer, GeometryUtility::GenerousCharsPerOrdinate);
        FdoStringUtility::StringConcatenate(string, ordinateBuffer);
    }
}

FdoString* GeometryUtility::CreateGeometryText(FdoIGeometry * geometry)
{
    // This code really should be distributed among the various geometry types,
    // but having it here allows the deferral of the creation of a lot of 
    // rework to have partial implementation class files for the geometry types.

    FdoString* value = 0;
    wchar_t * buffer = 0;
    wchar_t ** buffers = 0;
    FdoInt32 numBuffers = 0;
    FdoInt32 dim = geometry->GetDimensionality();
    FdoGeometryType type = geometry->GetDerivedType();

    switch (type)
    {
    case FdoGeometryType_Point:
        {
            const FdoIPoint * dg = static_cast<const FdoIPoint *>(geometry);   // derived geometry
            buffer = AllocateStringForPositions(dim, 1);
            FdoPtr<FdoIDirectPosition> pos = dg->GetPosition();
            AppendPositionToString(buffer, pos);
            wchar_t * tmp = FdoStringUtility::MakeString(L"POINT ", DimensionalityToFgftString(dim), L"(", buffer, L")");
            value = (FdoString*) tmp;
        }
        break;
    case FdoGeometryType_LineString:
        {
            FdoILineString* dg = static_cast<FdoILineString *>(geometry);   // derived geometry
            FdoPtr<FdoDirectPositionCollection> positions = dg->GetPositions();
			buffer = CreateFgftContentForPositions(dim, positions);
            wchar_t * tmp = FdoStringUtility::MakeString(L"LINESTRING ", DimensionalityToFgftString(dim), buffer);
            value = (FdoString*) tmp;
        }
        break;
    case FdoGeometryType_Polygon:
        {
            const FdoIPolygon * dg = static_cast<const FdoIPolygon *>(geometry);   // derived geometry
            buffer = CreateFgftContentForPolygon(dg);
            wchar_t * tmp = FdoStringUtility::MakeString(L"POLYGON ", DimensionalityToFgftString(dim), buffer);
            value = (FdoString*) tmp;
        }
        break;
    case FdoGeometryType_MultiPoint:
        {
            const FdoIMultiPoint * dg = static_cast<const FdoIMultiPoint *>(geometry);   // derived geometry
            FdoPtr<FdoDirectPositionCollection> positions = FdoDirectPositionCollection::Create();
            for (FdoInt32 i = 0;  i < dg->GetCount();  i++ )
            {
                FdoPtr<FdoIPoint> point = dg->GetItem(i);
                FdoPtr<FdoIDirectPosition> pos = point->GetPosition();
                positions->Add(pos);
            }
            buffer = CreateFgftContentForPositions(dim, positions);
            wchar_t * tmp = FdoStringUtility::MakeString(L"MULTIPOINT ", DimensionalityToFgftString(dim), buffer);
            value = (FdoString*) tmp;
        }
        break;
    case FdoGeometryType_MultiLineString:
        {
            const FdoIMultiLineString * dg = static_cast<const FdoIMultiLineString *>(geometry);   // derived geometry
            numBuffers = dg->GetCount();
            buffers = new wchar_t*[numBuffers];
            if (0 == buffers)
                throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
            for (FdoInt32 i = 0;  i < numBuffers;  i++ )
            {
                FdoPtr<FdoILineString> line = dg->GetItem(i);
                FdoPtr<FdoDirectPositionCollection> positions = line->GetPositions();
                buffers[i] = CreateFgftContentForPositions(dim, positions);
            }
            buffer = FdoStringUtility::MakeString(numBuffers, (FdoString**)buffers, L", ");
            wchar_t * tmp = FdoStringUtility::MakeString(L"MULTILINESTRING ", DimensionalityToFgftString(dim), L"(", buffer, L")");
            value = (FdoString*) tmp;
        }
        break;
    case FdoGeometryType_MultiPolygon:
        {
            const FdoIMultiPolygon * dg = static_cast<const FdoIMultiPolygon *>(geometry);   // derived geometry
            numBuffers = dg->GetCount();
            buffers = new wchar_t*[numBuffers];
            if (0 == buffers)
                throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
            for (FdoInt32 i = 0;  i < numBuffers;  i++ )
            {
                FdoPtr<FdoIPolygon> polygon = dg->GetItem(i);
                buffers[i] = CreateFgftContentForPolygon(polygon);
            }
            buffer = FdoStringUtility::MakeString(numBuffers, (FdoString**)buffers, L", ");
            wchar_t * tmp = FdoStringUtility::MakeString(L"MULTIPOLYGON ", DimensionalityToFgftString(dim), L"(", buffer, L")");
            value = (FdoString*) tmp;
        }
        break;
    case FdoGeometryType_CurveString:
        {
            const FdoICurveString * dg = static_cast<const FdoICurveString *>(geometry);   // derived geometry
            FdoPtr<FdoCurveSegmentCollection> segments;
            segments = dg->GetCurveSegments();
            buffer = CreateFgftContentForCurveSegments(segments);
            wchar_t * tmp = FdoStringUtility::MakeString(L"CURVESTRING ", DimensionalityToFgftString(dim), buffer);
            value = (FdoString*) tmp;
        }
        break;
    case FdoGeometryType_CurvePolygon:
        {
            const FdoICurvePolygon * dg = static_cast<const FdoICurvePolygon *>(geometry);   // derived geometry
            buffer = CreateFgftContentForCurvePolygon(dg);
            wchar_t * tmp = FdoStringUtility::MakeString(L"CURVEPOLYGON ", DimensionalityToFgftString(dim), buffer);
            value = (FdoString*) tmp;
        }
        break;
    case FdoGeometryType_MultiCurvePolygon:
        {
            const FdoIMultiCurvePolygon * dg = static_cast<const FdoIMultiCurvePolygon *>(geometry);   // derived geometry
            numBuffers = dg->GetCount();
            buffers = new wchar_t*[numBuffers];
            if (0 == buffers)
                throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

            for (FdoInt32 i = 0;  i < numBuffers;  i++ )
            {
                FdoPtr<FdoICurvePolygon> polygon = dg->GetItem(i);
                buffers[i] = CreateFgftContentForCurvePolygon(polygon);
            }
            buffer = FdoStringUtility::MakeString(numBuffers, (FdoString**)buffers, L", ");
            wchar_t * tmp = FdoStringUtility::MakeString(L"MULTICURVEPOLYGON ", DimensionalityToFgftString(dim), L"(", buffer, L")");
            value = (FdoString*) tmp;
        }
        break;
    case FdoGeometryType_MultiCurveString:
        {
            const FdoIMultiCurveString * dg = static_cast<const FdoIMultiCurveString *>(geometry);   // derived geometry
            numBuffers = dg->GetCount();
            buffers = new wchar_t*[numBuffers];
            if (0 == buffers)
                throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

            for (FdoInt32 i = 0;  i < numBuffers;  i++ )
            {
                FdoPtr<FdoICurveString> curveString = dg->GetItem(i);
                FdoPtr<FdoCurveSegmentCollection> segments = curveString->GetCurveSegments();
                buffers[i] = CreateFgftContentForCurveSegments(segments);
            }
            buffer = FdoStringUtility::MakeString(numBuffers, (FdoString**)buffers, L", ");
            wchar_t * tmp = FdoStringUtility::MakeString(L"MULTICURVESTRING ", DimensionalityToFgftString(dim), L"(", buffer, L")");
            value = (FdoString*) tmp;
        }
        break;
    case FdoGeometryType_MultiGeometry:
        {
            const FdoIMultiGeometry * dg = static_cast<const FdoIMultiGeometry *>(geometry);   // derived geometry
            numBuffers = dg->GetCount();
            buffers = new wchar_t*[numBuffers];
            if (0 == buffers)
                throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

            for (FdoInt32 i = 0;  i < numBuffers;  i++ )
            {
                FdoPtr<FdoIGeometry> geom = dg->GetItem(i);
                buffers[i] = (wchar_t *) CreateGeometryText(geom);
            }
            buffer = FdoStringUtility::MakeString(numBuffers, (FdoString**)buffers, L", ");
            wchar_t * tmp = FdoStringUtility::MakeString(L"GEOMETRYCOLLECTION (", buffer, L")");
            value = (FdoString*) tmp;
        }
        break;
    default:
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));
        break;
    }

    if (0 != buffer)
    {
        delete [] buffer;
        buffer = 0;
    }
    if (0 != buffers)
    {
        for (FdoInt32 i=0;  i < numBuffers;  i++)
            delete [] buffers[i];
        delete [] buffers;
        buffers = 0;
    }
    return value;
}

wchar_t * GeometryUtility::CreateFgftContentForPositions(FdoInt32 dimensionality, FdoDirectPositionCollection * positions)
{
    wchar_t * value = AllocateStringForPositions(dimensionality, positions->GetCount());
    static FdoString* pointSeparator = L", ";

    FdoStringUtility::StringConcatenate(value, L"(");
    for (FdoInt32 i = 0;  i < positions->GetCount();  i++ )
    {
        if ( i > 0 )
            FdoStringUtility::StringConcatenate(value, pointSeparator);
		FdoPtr<FdoIDirectPosition> pos = positions->GetItem(i);
        AppendPositionToString(value, pos);
    }
    FdoStringUtility::StringConcatenate(value, L")");

    return value;
}


wchar_t * GeometryUtility::CreateFgftContentForPolygon(const FdoIPolygon * polygon)
{
    wchar_t * value = 0;

    wchar_t * buffer = 0;
    wchar_t ** buffers = 0;
    FdoInt32 numBuffers = polygon->GetInteriorRingCount() + 1;   // +1 for the exterior loop

    FdoInt32 dim = polygon->GetDimensionality();

    buffers = new wchar_t*[numBuffers];
    if (0 == buffers)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
    FdoPtr<FdoILinearRing> ring = polygon->GetExteriorRing();
    FdoPtr<FdoDirectPositionCollection> positions = ring->GetPositions();
    buffers[0] = CreateFgftContentForPositions(dim, positions);

    FdoInt32 numInteriorRings = polygon->GetInteriorRingCount();
    for (FdoInt32 i = 1;  i < numBuffers;  i++ )
    {
        ring = polygon->GetInteriorRing(i-1);
        positions = ring->GetPositions();
        buffers[i] = CreateFgftContentForPositions(dim, positions);
    }

    buffer = FdoStringUtility::MakeString(numBuffers, (FdoString**)buffers, L", ");
    value = FdoStringUtility::MakeString(L"(", buffer, L")");

    if (0 != buffer)
    {
        delete [] buffer;
        buffer = 0;
    }
    if (0 != buffers)
    {
        for (FdoInt32 i=0;  i < numBuffers;  i++)
            delete [] buffers[i];
        delete [] buffers;
        buffers = 0;
    }

    return value;
}

wchar_t * GeometryUtility::CreateFgftContentForCurvePolygon(const FdoICurvePolygon * polygon)
{
    wchar_t * value = 0;

    wchar_t * buffer = 0;
    wchar_t ** buffers = 0;
    FdoInt32 numBuffers = polygon->GetInteriorRingCount() + 1;   // +1 for the exterior loop

    FdoInt32 dim = polygon->GetDimensionality();

    buffers = new wchar_t*[numBuffers];
    if (0 == buffers)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
    FdoPtr<FdoIRing> ring = polygon->GetExteriorRing();
    FdoPtr<FdoCurveSegmentCollection> segments = ring->GetCurveSegments();
    buffers[0] = CreateFgftContentForCurveSegments(segments);

    FdoInt32 numInteriorRings = polygon->GetInteriorRingCount();
    for (FdoInt32 i = 1;  i < numBuffers;  i++ )
    {
        ring = polygon->GetInteriorRing(i-1);
        segments = ring->GetCurveSegments();
        buffers[i] = CreateFgftContentForCurveSegments(segments);
    }

    buffer = FdoStringUtility::MakeString(numBuffers, (FdoString**)buffers, L", ");
    value = FdoStringUtility::MakeString(L"(", buffer, L")");

    if (0 != buffer)
    {
        delete [] buffer;
        buffer = 0;
    }
    if (0 != buffers)
    {
        for (FdoInt32 i=0;  i < numBuffers;  i++)
            delete [] buffers[i];
        delete [] buffers;
        buffers = 0;
    }

    return value;
}

wchar_t * GeometryUtility::CreateFgftContentForCurveSegments(FdoCurveSegmentCollection * segments)
{
    wchar_t * value = 0;
    wchar_t * buffer = 0;
    wchar_t ** buffers = 0;

    FdoInt32 numBuffers = segments->GetCount();
    buffers = new wchar_t*[numBuffers];
    if (0 == buffers)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

    for (FdoInt32 i = 0;  i < numBuffers;  i++ )
    {
        FdoPtr<FdoICurveSegmentAbstract> segment = segments->GetItem(i);
        buffers[i] = CreateFgftContentForCurveSegment(segment);
    }
    FdoPtr<FdoICurveSegmentAbstract> firstSegment = segments->GetItem(0);
    FdoInt32 dim = firstSegment->GetDimensionality();
    buffer = FdoStringUtility::MakeString(numBuffers, (FdoString**)buffers, L", ");
    wchar_t * startPositionBuffer = AllocateStringForPositions(dim, 1); // Start pos
    FdoStringUtility::StringConcatenate(startPositionBuffer, L"(");
    FdoPtr<FdoIDirectPosition> startPosition = firstSegment->GetStartPosition();
    AppendPositionToString(startPositionBuffer, startPosition);
    FdoStringUtility::StringConcatenate(startPositionBuffer, L" (");
    value = FdoStringUtility::MakeString(startPositionBuffer, buffer, L"))");
    delete [] startPositionBuffer;
    startPositionBuffer = 0;

    if (0 != buffer)
    {
        delete [] buffer;
        buffer = 0;
    }
    if (0 != buffers)
    {
        for (FdoInt32 i=0;  i < numBuffers;  i++)
            delete [] buffers[i];
        delete [] buffers;
        buffers = 0;
    }

    return value;
}

wchar_t * GeometryUtility::CreateFgftContentForCurveSegment(FdoICurveSegmentAbstract * segment)
{
    wchar_t * value = 0;

    wchar_t * buffer = 0;

    FdoInt32 dim = segment->GetDimensionality();

    FdoGeometryComponentType type = segment->GetDerivedType();

    switch(type)
    {
    case FdoGeometryComponentType_LineStringSegment:
        {
            FdoILineStringSegment * dg = static_cast<FdoILineStringSegment *>(segment);   // derived geometry component
            if (dg->GetCount() < 2)
        		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));
            FdoPtr<FdoDirectPositionCollection> positions = dg->GetPositions();
            positions->RemoveAt(0);   // Skip 1st point
            buffer = CreateFgftContentForPositions(dim, positions);
            value = FdoStringUtility::MakeString(L"LINESTRINGSEGMENT ", buffer);
        }
        break;
    case FdoGeometryComponentType_CircularArcSegment:
        {
            FdoICircularArcSegment * dg = static_cast<FdoICircularArcSegment *>(segment);   // derived geometry component
            value = AllocateStringForPositions(dim, 2);                // Skip 1st point

            FdoStringUtility::StringConcatenate(value, L"CIRCULARARCSEGMENT (");

            FdoPtr<FdoIDirectPosition> pos = dg->GetMidPoint();
            AppendPositionToString(value, pos);

            FdoStringUtility::StringConcatenate(value, L", ");

            pos = dg->GetEndPosition();
            AppendPositionToString(value, pos);
            FdoStringUtility::StringConcatenate(value, L")");
        }
        break;
    default:
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));
        break;
    }

    if (0 != buffer)
    {
        delete [] buffer;
        buffer = 0;
    }

    return value;
}

