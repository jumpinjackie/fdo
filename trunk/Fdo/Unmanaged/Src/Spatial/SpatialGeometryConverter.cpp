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

#include <Spatial/SpatialStd.h>
#include <FdoGeometry.h>
#include <Spatial/SpatialGeometryConverter.h>

FdoIGeometry * FdoSpatialGeometryConverter::ConvertOrdinates(
    FdoIGeometry * geometry,
    bool applyTargetDimensionality,
    FdoInt32 targetDimensionality,
    double padValueZ,
    double padValueM)
{
    FdoIGeometry * newGeometry = NULL;

    if ( geometry == NULL )
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    const double * ordinates = NULL;
    double * newOrdinates = NULL;
    FdoInt32 numOrdinates;
    FdoInt32 numPositions;
    FdoInt32 dimensionality;
    FdoInt32 outputDim;
    FdoInt32 i;

    FdoGeometryType geomType = geometry->GetDerivedType();

    switch (geomType)
    {
    case FdoGeometryType_LineString:
        {
            FdoILineString * derivedGeom = static_cast<FdoILineString *>(geometry);
            dimensionality = derivedGeom->GetDimensionality();
            numPositions = derivedGeom->GetCount();
            outputDim = applyTargetDimensionality ? targetDimensionality : dimensionality;
            numOrdinates = numPositions * this->DimensionalityToNumOrdinates(outputDim);
            newOrdinates = AllocateOrdinates(numOrdinates);
            ordinates = derivedGeom->GetOrdinates();
            ConvertOrdinates(dimensionality, numPositions, ordinates, padValueZ, padValueM, outputDim, newOrdinates);
            newGeometry = gf->CreateLineString(outputDim, numOrdinates, newOrdinates);
            delete [] newOrdinates;
        }
        break;
    case FdoGeometryType_Point:
        {
            FdoIPoint * derivedGeom = static_cast<FdoIPoint *>(geometry);
            dimensionality = derivedGeom->GetDimensionality();
            numPositions = 1;
            outputDim = applyTargetDimensionality ? targetDimensionality : dimensionality;
            numOrdinates = numPositions * this->DimensionalityToNumOrdinates(outputDim);
            newOrdinates = AllocateOrdinates(numOrdinates);
            ordinates = derivedGeom->GetOrdinates();
            ConvertOrdinates(dimensionality, numPositions, ordinates, padValueZ, padValueM, outputDim, newOrdinates);
            newGeometry = gf->CreatePoint(outputDim, newOrdinates);
            delete [] newOrdinates;
        }
        break;
    case FdoGeometryType_Polygon:
        {
            FdoIPolygon * derivedGeom = static_cast<FdoIPolygon *>(geometry);
            outputDim = applyTargetDimensionality ? targetDimensionality : derivedGeom->GetDimensionality();
            FdoPtr<FdoLinearRingCollection> newInteriorRings = FdoLinearRingCollection::Create();

            FdoPtr<FdoILinearRing> ring = derivedGeom->GetExteriorRing();
            FdoPtr<FdoILinearRing> newExteriorRing = ConvertOrdinates(ring, outputDim, padValueZ, padValueM);

            for (i = 0;  i < derivedGeom->GetInteriorRingCount();  i++)
            {
                ring = derivedGeom->GetInteriorRing(i);
                FdoPtr<FdoILinearRing> newInteriorRing = ConvertOrdinates(ring, outputDim, padValueZ, padValueM);
                newInteriorRings->Add(newInteriorRing);
            }

            newGeometry = gf->CreatePolygon(newExteriorRing, newInteriorRings);   
        }
        break;
    case FdoGeometryType_CurveString:
        {
            FdoICurveString * derivedGeom = static_cast<FdoICurveString *>(geometry);
            outputDim = applyTargetDimensionality ? targetDimensionality : derivedGeom->GetDimensionality();
            FdoPtr<FdoCurveSegmentCollection> csc = derivedGeom->GetCurveSegments();
            FdoPtr<FdoCurveSegmentCollection> newCsc = ConvertOrdinates(csc, outputDim, padValueZ, padValueM);

            newGeometry = gf->CreateCurveString(newCsc);
        }
        break;

    case FdoGeometryType_CurvePolygon:
        {
            FdoICurvePolygon * derivedGeom = static_cast<FdoICurvePolygon *>(geometry);
            outputDim = applyTargetDimensionality ? targetDimensionality : derivedGeom->GetDimensionality();
            FdoPtr<FdoRingCollection> newInteriorRings = FdoRingCollection::Create();

            FdoPtr<FdoIRing> ring = derivedGeom->GetExteriorRing();
            FdoPtr<FdoCurveSegmentCollection> csc = ring->GetCurveSegments();
            FdoPtr<FdoCurveSegmentCollection> newCsc = ConvertOrdinates(csc, outputDim, padValueZ, padValueM);
            FdoPtr<FdoIRing> newExteriorRing = gf->CreateRing(newCsc);

            for (FdoInt32 i = 0;  i < derivedGeom->GetInteriorRingCount();  i++)
            {
                ring = derivedGeom->GetInteriorRing(i);
                csc = ring->GetCurveSegments();
                newCsc = ConvertOrdinates(csc, outputDim, padValueZ, padValueM);
                FdoPtr<FdoIRing> newInteriorRing = gf->CreateRing(newCsc);
                newInteriorRings->Add(newInteriorRing);
            }

            newGeometry = gf->CreateCurvePolygon(newExteriorRing, newInteriorRings);   
        }
        break;

    case FdoGeometryType_MultiPoint:
        {
            FdoIMultiPoint * derivedGeom = static_cast<FdoIMultiPoint *>(geometry);
            dimensionality = derivedGeom->GetDimensionality();
            numPositions = derivedGeom->GetCount();
            outputDim = applyTargetDimensionality ? targetDimensionality : dimensionality;
            numOrdinates = numPositions * this->DimensionalityToNumOrdinates(outputDim);
            newOrdinates = AllocateOrdinates(numOrdinates);
            ordinates = derivedGeom->GetOrdinates();
            ConvertOrdinates(dimensionality, numPositions, ordinates, padValueZ, padValueM, outputDim, newOrdinates);
            newGeometry = gf->CreateMultiPoint(outputDim, numOrdinates, newOrdinates);
        }
        break;
    case FdoGeometryType_MultiLineString:
        {
            FdoIMultiLineString * derivedGeom = static_cast<FdoIMultiLineString *>(geometry);
            FdoPtr<FdoLineStringCollection> newSubGeometries = FdoLineStringCollection::Create();

            FdoInt32 numSubGeometries = derivedGeom->GetCount();

            for (FdoInt32 i = 0;  i < numSubGeometries;  i++)
            {
                FdoPtr<FdoILineString> subGeom = derivedGeom->GetItem(i);
                FdoPtr<FdoIGeometry> newSubGeometry = ConvertOrdinates(subGeom, applyTargetDimensionality, targetDimensionality);
                newSubGeometries->Add(static_cast<FdoILineString *>(newSubGeometry.p));
            }
            newGeometry = gf->CreateMultiLineString(newSubGeometries);
        }
        break;
    case FdoGeometryType_MultiPolygon:
        {
            FdoIMultiPolygon * derivedGeom = static_cast<FdoIMultiPolygon *>(geometry);
            FdoPtr<FdoPolygonCollection> newSubGeometries = FdoPolygonCollection::Create();

            FdoInt32 numSubGeometries = derivedGeom->GetCount();

            for (FdoInt32 i = 0;  i < numSubGeometries;  i++)
            {
                FdoPtr<FdoIPolygon> subGeom = derivedGeom->GetItem(i);
                FdoPtr<FdoIGeometry> newSubGeometry = ConvertOrdinates(subGeom, applyTargetDimensionality, targetDimensionality);
                newSubGeometries->Add(static_cast<FdoIPolygon *>(newSubGeometry.p));
            }
            newGeometry = gf->CreateMultiPolygon(newSubGeometries);
        }
        break;
    case FdoGeometryType_MultiCurveString:
        {
            FdoIMultiCurveString * derivedGeom = static_cast<FdoIMultiCurveString *>(geometry);
            FdoPtr<FdoCurveStringCollection> newSubGeometries = FdoCurveStringCollection::Create();

            FdoInt32 numSubGeometries = derivedGeom->GetCount();

            for (FdoInt32 i = 0;  i < numSubGeometries;  i++)
            {
                FdoPtr<FdoICurveString> subGeom = derivedGeom->GetItem(i);
                FdoPtr<FdoIGeometry> newSubGeometry = ConvertOrdinates(subGeom, applyTargetDimensionality, targetDimensionality);
                newSubGeometries->Add(static_cast<FdoICurveString *>(newSubGeometry.p));
            }
            newGeometry = gf->CreateMultiCurveString(newSubGeometries);
        }
        break;
    case FdoGeometryType_MultiCurvePolygon:
        {
            FdoIMultiCurvePolygon * derivedGeom = static_cast<FdoIMultiCurvePolygon *>(geometry);
            FdoPtr<FdoCurvePolygonCollection> newSubGeometries = FdoCurvePolygonCollection::Create();

            FdoInt32 numSubGeometries = derivedGeom->GetCount();

            for (FdoInt32 i = 0;  i < numSubGeometries;  i++)
            {
                FdoPtr<FdoICurvePolygon> subGeom = derivedGeom->GetItem(i);
                FdoPtr<FdoIGeometry> newSubGeometry = ConvertOrdinates(subGeom, applyTargetDimensionality, targetDimensionality);
                newSubGeometries->Add(static_cast<FdoICurvePolygon *>(newSubGeometry.p));
            }
            newGeometry = gf->CreateMultiCurvePolygon(newSubGeometries);
        }
        break;
    case FdoGeometryType_MultiGeometry:
        {
            FdoIMultiGeometry * derivedGeom = static_cast<FdoIMultiGeometry *>(geometry);
            FdoPtr<FdoGeometryCollection> newSubGeometries = FdoGeometryCollection::Create();

            FdoInt32 numSubGeometries = derivedGeom->GetCount();

            for (FdoInt32 i = 0;  i < numSubGeometries;  i++)
            {
                FdoPtr<FdoIGeometry> subGeom = derivedGeom->GetItem(i);
                FdoPtr<FdoIGeometry> newSubGeometry = ConvertOrdinates(subGeom, applyTargetDimensionality, targetDimensionality);
                newSubGeometries->Add(static_cast<FdoIGeometry *>(newSubGeometry.p));
            }
            newGeometry = gf->CreateMultiGeometry(newSubGeometries);
        }
        break;
    default:
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
    }

    return newGeometry;
}


FdoILinearRing * FdoSpatialGeometryConverter::ConvertOrdinates(
    FdoILinearRing * ring,
    FdoInt32 outputDim,
    double padValueZ,
    double padValueM)
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoILinearRing> newRing;

    const double * ordinates = NULL;
    double * newOrdinates = NULL;
    FdoInt32 numOrdinates;
    FdoInt32 numPositions;
    FdoInt32 dimensionality = ring->GetDimensionality();

    numPositions = ring->GetCount();
    numOrdinates = numPositions * this->DimensionalityToNumOrdinates(outputDim);
    newOrdinates = AllocateOrdinates(numOrdinates);
    ordinates = ring->GetOrdinates();
    ConvertOrdinates(dimensionality, numPositions, ordinates, padValueZ, padValueM, outputDim, newOrdinates);
    newRing = gf->CreateLinearRing(outputDim, numOrdinates, newOrdinates);
    delete [] newOrdinates;

    return FDO_SAFE_ADDREF(newRing.p);
}

FdoCurveSegmentCollection * FdoSpatialGeometryConverter::ConvertOrdinates(
    FdoCurveSegmentCollection * csc,
    FdoInt32 outputDim,
    double padValueZ,
    double padValueM)
{
    FdoPtr<FdoCurveSegmentCollection> newCsc = FdoCurveSegmentCollection::Create();

    FdoInt32 count = csc->GetCount();
    for (FdoInt32 i=0;  i < count;  i++)
    {
        FdoPtr<FdoICurveSegmentAbstract> cs = csc->GetItem(i);
        FdoPtr<FdoICurveSegmentAbstract> newCs = ConvertOrdinates(cs, outputDim, padValueZ, padValueM);
        newCsc->Add(newCs);
    }

    return FDO_SAFE_ADDREF(newCsc.p);
}

FdoICurveSegmentAbstract * FdoSpatialGeometryConverter::ConvertOrdinates(
    FdoICurveSegmentAbstract * cs,
    FdoInt32 outputDim,
    double padValueZ,
    double padValueM)
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoICurveSegmentAbstract> newCs;

    const double * ordinates = NULL;
    double * newOrdinates = NULL;
    FdoInt32 numOrdinates;
    FdoInt32 numPositions;
    FdoInt32 dimensionality = cs->GetDimensionality();
    bool paddingZ = ( (outputDim&FdoDimensionality_Z) != 0 && (dimensionality&FdoDimensionality_Z) == 0 );
    bool paddingM = ( (outputDim&FdoDimensionality_M) != 0 && (dimensionality&FdoDimensionality_M) == 0 );

    FdoGeometryComponentType gct = cs->GetDerivedType();

    switch (gct)
    {
    case FdoGeometryComponentType_LineStringSegment:
        {
            FdoILineStringSegment * ls = static_cast<FdoILineStringSegment *> ((FdoICurveSegmentAbstract *)cs);
            numPositions = ls->GetCount();
            numOrdinates = numPositions * this->DimensionalityToNumOrdinates(outputDim);
            newOrdinates = AllocateOrdinates(numOrdinates);
            ordinates = ls->GetOrdinates();
            ConvertOrdinates(dimensionality, numPositions, ordinates, padValueZ, padValueM, outputDim, newOrdinates);
            newCs = gf->CreateLineStringSegment(outputDim, numOrdinates, newOrdinates);
            delete [] newOrdinates;
        }
        break;
    case FdoGeometryComponentType_CircularArcSegment:
        {
            FdoICircularArcSegment * arc = static_cast<FdoICircularArcSegment *> ((FdoICurveSegmentAbstract *)cs);
            FdoPtr<FdoIDirectPosition> start = arc->GetStartPosition();
            FdoPtr<FdoIDirectPosition> mid = arc->GetMidPoint();
            FdoPtr<FdoIDirectPosition> end = arc->GetEndPosition();
            numPositions = 1;
            numOrdinates = this->DimensionalityToNumOrdinates(outputDim);
            double ords[4];

            ordinates = start->GetOrdinates();
            ConvertOrdinates(dimensionality, numPositions, ordinates, padValueZ, padValueM, outputDim, ords);
            FdoPtr<FdoDirectPositionImpl> newStart = FdoDirectPositionImpl::Create(outputDim, ords);

            ordinates = mid->GetOrdinates();
            ConvertOrdinates(dimensionality, numPositions, ordinates, padValueZ, padValueM, outputDim, ords);
            FdoPtr<FdoDirectPositionImpl> newMid = FdoDirectPositionImpl::Create(outputDim, ords);

            ordinates = end->GetOrdinates();
            ConvertOrdinates(dimensionality, numPositions, ordinates, padValueZ, padValueM, outputDim, ords);
            FdoPtr<FdoDirectPositionImpl> newEnd = FdoDirectPositionImpl::Create(outputDim, ords);

            newCs = gf->CreateCircularArcSegment(newStart, newMid, newEnd);
        }
        break;
    default:
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
    }

    return FDO_SAFE_ADDREF(newCs.p);
}


void FdoSpatialGeometryConverter::ConvertOrdinates(
    FdoInt32 inputDim,
    FdoInt32 numPositions,
    const double * inputOrds,
    double padValueZ,
    double padValueM,
    FdoInt32 outputDim,
    double * outputOrds)
{
    FdoInt32 numInputOrds = numPositions * DimensionalityToNumOrdinates(inputDim);
    bool inputHasZ = (inputDim&FdoDimensionality_Z) != 0;
    bool inputHasM = (inputDim&FdoDimensionality_M) != 0;
    bool outputHasZ = (outputDim&FdoDimensionality_Z) != 0;
    bool outputHasM = (outputDim&FdoDimensionality_M) != 0;
    bool paddingZ = ( outputHasZ && !inputHasZ );
    bool paddingM = ( outputHasM && !inputHasM );

    double x, y;
    double z = padValueZ;
    double m = padValueM;

    // Assume that ordinate arrays are well-formed (no check for number of positions,
    // end of array at a position boundary, etc.).

    FdoInt32 ordIndex1 = 0;
    FdoInt32 ordIndex2 = 0;

    // Branch on input dimensionality so that we don't have to test for it for every position.
    // Performance could similarly be slightly improved by also branching
    // on output dimensionality (requires more cases).

    if (!inputHasZ && !inputHasM)
    {
        while (ordIndex1 < numInputOrds)
        {
            x = inputOrds[ordIndex1++];
            y = inputOrds[ordIndex1++];
            ConvertPosition(x, y);
            outputOrds[ordIndex2++] = x;
            outputOrds[ordIndex2++] = y;
            if (paddingZ)
                outputOrds[ordIndex2++] = padValueZ;
            if (paddingM)
                outputOrds[ordIndex2++] = padValueM;
        }
    }
    else if (inputHasZ && !inputHasM)
    {
        while (ordIndex1 < numInputOrds)
        {
            x = inputOrds[ordIndex1++];
            y = inputOrds[ordIndex1++];
            z = inputOrds[ordIndex1++];
            ConvertPosition(x, y, z);
            outputOrds[ordIndex2++] = x;
            outputOrds[ordIndex2++] = y;
            if (outputHasZ)
                outputOrds[ordIndex2++] = z;
            if (paddingM)
                outputOrds[ordIndex2++] = padValueM;
        }
    }
    else if (!inputHasZ && inputHasM)
    {
        while (ordIndex1 < numInputOrds)
        {
            x = inputOrds[ordIndex1++];
            y = inputOrds[ordIndex1++];
            m = inputOrds[ordIndex1++];
            ConvertPosition(x, y);
            outputOrds[ordIndex2++] = x;
            outputOrds[ordIndex2++] = y;
            if (paddingZ)
                outputOrds[ordIndex2++] = padValueZ;
            if (outputHasM)
                outputOrds[ordIndex2++] = m;
        }
    }
    else // inputHasZ && inputHasM
    {
        while (ordIndex1 < numInputOrds)
        {
            x = inputOrds[ordIndex1++];
            y = inputOrds[ordIndex1++];
            z = inputOrds[ordIndex1++];
            m = inputOrds[ordIndex1++];
            ConvertPosition(x, y, z);
            outputOrds[ordIndex2++] = x;
            outputOrds[ordIndex2++] = y;
            if (outputHasZ)
                outputOrds[ordIndex2++] = z;
            if (outputHasM)
                outputOrds[ordIndex2++] = m;
        }
    }
}

double * FdoSpatialGeometryConverter::AllocateOrdinates(FdoInt32 numOrdinates)
{
    double * ordinates = new double[numOrdinates];
    if (NULL == ordinates)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
    return ordinates;
}

FdoInt32 FdoSpatialGeometryConverter::DimensionalityToNumOrdinates(FdoInt32 dimensionality)
{
    static FdoInt32 ordinateCountTable[] = { 2, 3, 3, 4 };
    return ordinateCountTable[dimensionality];
}
