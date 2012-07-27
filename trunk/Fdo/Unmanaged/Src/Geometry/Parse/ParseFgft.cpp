// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include "ParseFgft.h"

// Note, if you enable this define here or LexFgft.h you'll see output
// of the temporary data structures used to save the double values and 
// context entries for each component of the geometry
// #define DEBUG_TRACEACTIONS	1

FdoParseFgft::FdoParseFgft()
{
	m_dim = kXY;
	m_gf = FdoFgfGeometryFactory::GetInstance();
	m_geometry = NULL;
	m_values = FdoDoubleArray::Create();
	m_types = FdoIntArray::Create();
	m_dims = FdoIntArray::Create();
	m_starts = FdoIntArray::Create();
	m_type = kNone;
	m_break = false;

	// ... no lexical analyzer
	m_lex = NULL;

    fdo_fgft_yydebug = 0;
    fdo_fgft_yynerrs = 0;
    fdo_fgft_yyerrflag = 0;
    fdo_fgft_yychar = -1;
    fdo_fgft_yyssp = NULL;
    fdo_fgft_yyvsp = NULL;
#ifndef _WIN32
        fdo_fgft_yyss = NULL;
        fdo_fgft_yysslim = NULL;
        fdo_fgft_yyvs = NULL;
        fdo_fgft_yystacksize = 0;
#endif
    fdo_fgft_yylineno = 0;
}

FdoParseFgft::~FdoParseFgft()
{
	if (m_lex != NULL)
		delete m_lex;
	FDO_SAFE_RELEASE(m_starts);
	FDO_SAFE_RELEASE(m_dims);
	FDO_SAFE_RELEASE(m_types);
	FDO_SAFE_RELEASE(m_values);
	FDO_SAFE_RELEASE(m_gf);
	// note: m_geometry is not released in destructor since it 
	// only created once parsing is successful and it returned to caller
}

FdoIGeometry* FdoParseFgft::ParseFgft(FdoString* pwzFgft)
{
	m_lex = new FdoLexFgft(pwzFgft);
	if (m_lex == NULL)
		return NULL;
	try
	{
		fdo_fgft_yyparse(this);
	    if (m_geometry == NULL)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_8_INCORRECTSTRINGFORMAT)));
	}
	catch (FdoException* exception)
	{
		Error();

        FdoException* exceptionOuter = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_7_INVALIDFGF)), exception);
        exception->Release();
		throw exceptionOuter;
	}
	return m_geometry;
}

// count how many of the same type, starting at iContext and counting it
// but don't move iContext forward (call by value)
FdoInt32 FdoParseFgft::CountSame(FdoInt32 iContext, _types type)
{
	FdoInt32	iResult = 0;
	FdoInt32	countContext = m_types->GetCount();
	while (iContext < countContext && (*m_types)[iContext] == type)
	{
		iResult++;
		iContext++;
	}
	return iResult;
}

FdoIGeometry* FdoParseFgft::DoPoint(FdoInt32& iContext, double* doubles)
{
	// points have single context entry and use DimToCount values
	FdoIGeometry*	geom = NULL;
	_dims	dim = (_dims) (*m_dims)[iContext];
	geom = m_gf->CreatePoint(DimToDimensionality(dim), &doubles[(*m_starts)[iContext]]);
	iContext++;
	return geom;
}

FdoIGeometry* FdoParseFgft::DoLineString(FdoInt32& iContext, double* doubles, _types type)
{
	// line strings can have multiple context entries and corresponding n-values per point
	FdoIGeometry*	geom = NULL;
	_dims			dim = (_dims) (*m_dims)[iContext];
	FdoInt32		countSame = CountSame(iContext, type);
	FdoInt32		countValues = countSame*DimToCount(dim);
	geom = m_gf->CreateLineString(DimToDimensionality(dim), countValues, &doubles[(*m_starts)[iContext]]);
	iContext += countSame;
	return geom;
}

FdoIGeometry* FdoParseFgft::DoPolygon(FdoInt32& iContext, double* doubles, _types type)
{
	// polygons have 1 exterior ring and 0 or more interior rings
	FdoIGeometry*	geom = NULL;
	FdoILinearRing* extRing = NULL;
    FdoLinearRingCollection* intRings = NULL;
    FdoILinearRing* intRing = NULL;

    try
    {
	    _dims			dim = (_dims) (*m_dims)[iContext];

	    // exterior ring
	    FdoInt32		countSame = CountSame(iContext, type);
	    FdoInt32		countValues = countSame*DimToCount(dim);
	    extRing = m_gf->CreateLinearRing(DimToDimensionality(dim), countValues, &doubles[(*m_starts)[iContext]]);
	    iContext += countSame;

	    intRings = FdoLinearRingCollection::Create();	
	    while (iContext < m_types->GetCount() && (*m_types)[iContext] == -type)
	    {
		    (*m_types)[iContext] = type;
		    countSame = CountSame(iContext, type);
		    countValues = countSame*DimToCount(dim);
		    intRing = m_gf->CreateLinearRing(DimToDimensionality(dim), countValues, &doubles[(*m_starts)[iContext]]);
		    intRings->Add(intRing);
		    FDO_SAFE_RELEASE(intRing);
		    iContext += countSame;
	    }
	    geom = m_gf->CreatePolygon(extRing, intRings);
	    FDO_SAFE_RELEASE(intRings);
	    FDO_SAFE_RELEASE(extRing);
	    return geom;
    }
    catch (FdoException* exception)
    {
	    FDO_SAFE_RELEASE(intRing);
	    FDO_SAFE_RELEASE(intRings);
	    FDO_SAFE_RELEASE(extRing);
        FDO_SAFE_RELEASE(geom);
        throw exception;
    }
}

FdoIGeometry* FdoParseFgft::DoMultiPoint(FdoInt32& iContext, double* doubles)
{
	FdoIGeometry*	geom = NULL;
    FdoPointCollection* pnts = NULL;
    FdoIPoint*	pnt = NULL;

    try
    {
	    _dims			dim = (_dims) (*m_dims)[iContext];

	    // there can be n-context entries for each point
	    pnts = FdoPointCollection::Create();
	    FdoInt32			countContext = CountSame(iContext, kMultiPoint);
	    for (int i=0; i<countContext; i++)
	    {
		    // add each point to the collection
		    dim = (_dims) (*m_dims)[i];
		    pnt = m_gf->CreatePoint(DimToDimensionality(dim), &doubles[(*m_starts)[iContext]]);
		    pnts->Add(pnt);
		    FDO_SAFE_RELEASE(pnt);
		    iContext++;
	    }
	    geom = m_gf->CreateMultiPoint(pnts);
	    FDO_SAFE_RELEASE(pnts);
	    return geom;
    }
    catch (FdoException* exception)
    {
        FDO_SAFE_RELEASE(pnts);
        FDO_SAFE_RELEASE(pnt);
        FDO_SAFE_RELEASE(geom);
        throw exception;
    }
}

FdoIGeometry* FdoParseFgft::DoMultiLineString(FdoInt32& iContext, double* doubles)
{
	FdoIGeometry*	geom = NULL;
    FdoLineStringCollection* lineStrings = NULL;
    FdoILineString* lineString = NULL;

    try
    {
	    _dims			dim = (_dims) (*m_dims)[iContext];

	    lineString = (FdoILineString*)DoLineString(iContext, doubles, kMultiLineString);
	    if (lineString != NULL)
	    {
		    lineStrings = FdoLineStringCollection::Create();
		    lineStrings->Add(lineString);
		    FDO_SAFE_RELEASE(lineString);
	    }
	    while (iContext < m_types->GetCount() && (*m_types)[iContext] == -kMultiLineString)
	    {
		    (*m_types)[iContext] = kMultiLineString;
		    lineString = (FdoILineString*)DoLineString(iContext, doubles, kMultiLineString);
		    if (lineString != NULL)
		    {
			    lineStrings->Add(lineString);
			    FDO_SAFE_RELEASE(lineString);
		    }
	    }	
	    geom = m_gf->CreateMultiLineString(lineStrings);
	    FDO_SAFE_RELEASE(lineStrings);
	    return geom;
    }
    catch (FdoException* exception)
    {
        FDO_SAFE_RELEASE(lineStrings);
        FDO_SAFE_RELEASE(lineString);
        FDO_SAFE_RELEASE(geom);
        throw exception;
    }
}

FdoIGeometry* FdoParseFgft::DoMultiPolygon(FdoInt32& iContext, double* doubles)
{
	FdoIGeometry*	geom = NULL;
    FdoPolygonCollection* polygons = NULL;
    FdoIPolygon* polygon = NULL;

    try
    {
	    _dims			dim = (_dims) (*m_dims)[iContext];

	    polygon = (FdoIPolygon*)DoPolygon(iContext, doubles, kMultiPolygon);
	    if (polygon != NULL)
	    {
		    polygons = FdoPolygonCollection::Create();
		    polygons->Add(polygon);
		    FDO_SAFE_RELEASE(polygon);
	    }
	    while (iContext < m_types->GetCount() && (*m_types)[iContext] == kRing)
	    {
			// skip polygons separator (ring)
		    iContext++;
		    polygon = (FdoIPolygon*)DoPolygon(iContext, doubles, kMultiPolygon);
		    if (polygon != NULL)
		    {
			    polygons->Add(polygon);
			    FDO_SAFE_RELEASE(polygon);
		    }
	    }	
	    geom = m_gf->CreateMultiPolygon(polygons);
	    FDO_SAFE_RELEASE(polygons);
	    return geom;
    }
    catch (FdoException* exception)
    {
	    FDO_SAFE_RELEASE(polygon);
	    FDO_SAFE_RELEASE(polygons);
        FDO_SAFE_RELEASE(geom);
        throw exception;
    }
}

FdoIDirectPosition* FdoParseFgft::CreatePosition(_dims dim, double *doubles)
{
	if (dim == kXY)
		return m_gf->CreatePositionXY(doubles[0], doubles[1]);
	else if (dim == kXYZ)
		return m_gf->CreatePositionXYZ(doubles[0], doubles[1], doubles[2]);
	else if (dim == kXYM)
		return m_gf->CreatePositionXYM(doubles[0], doubles[1], doubles[2]);
	else if (dim == kXYZM)
		return m_gf->CreatePositionXYZM(doubles[0], doubles[1], doubles[2], doubles[3]);
	else
		return NULL;
}

FdoCurveSegmentCollection* FdoParseFgft::DoCurveSegmentCollection(FdoInt32& iContext, double* doubles)
{
    FdoCurveSegmentCollection* segs = NULL;
    FdoICurveSegmentAbstract* seg = NULL;
	FdoIDirectPosition*	start = NULL;
	FdoIDirectPosition*	mid = NULL;
	FdoIDirectPosition*	end = NULL;

    try
    {
    	_dims			dim = (_dims) (*m_dims)[iContext];
	    segs = FdoCurveSegmentCollection::Create();

	    // skip the first point (look back at the previous point for each segment)
	    iContext++;

	    // while kCircularArcSegment or kLineStringSegment create curve segments
        _types			typeSegment;
	    while (iContext < m_types->GetCount())
        { 
    	    typeSegment = (_types)(*m_types)[iContext];

		    if (typeSegment == kCircularArcSegment)
		    {
			    // this trick (-1,+0,+1) works because the previous point is either the first point
			    // or the last point from the previous segment
			    start = CreatePosition(dim, &doubles[(*m_starts)[iContext-1]]);
			    mid = CreatePosition(dim, &doubles[(*m_starts)[iContext]]);
			    end = CreatePosition(dim, &doubles[(*m_starts)[iContext+1]]);
			    seg = m_gf->CreateCircularArcSegment(start, mid, end);
                FDO_SAFE_RELEASE(end);
                FDO_SAFE_RELEASE(mid);
                FDO_SAFE_RELEASE(start);
			    segs->Add(seg);
			    FDO_SAFE_RELEASE(seg);
			    iContext += 2;
		    }
		    else if (typeSegment == kLineStringSegment)
		    {
			    FdoInt32	countSame = CountSame(iContext, kLineStringSegment);
			    FdoInt32	countValues = countSame*DimToCount(dim);
   				seg = m_gf->CreateLineStringSegment(DimToDimensionality(dim), countValues+DimToCount(dim), &doubles[(*m_starts)[iContext-1]]);
			    segs->Add(seg);
			    FDO_SAFE_RELEASE(seg);
    			iContext += countSame;
		    }
            else
                break;
	    }
	    return segs;
    }
    catch (FdoException* exception)
    {
        FDO_SAFE_RELEASE(segs);
        FDO_SAFE_RELEASE(seg);
        FDO_SAFE_RELEASE(start);
        FDO_SAFE_RELEASE(mid);
        FDO_SAFE_RELEASE(end);
        throw exception;
    }
}

FdoIGeometry* FdoParseFgft::DoCurveString(FdoInt32& iContext, double* doubles)
{
	FdoCurveSegmentCollection* segs = NULL;
	FdoIGeometry*	geom = NULL;

    try
    {
	    segs = DoCurveSegmentCollection(iContext, doubles);
	    geom = m_gf->CreateCurveString(segs);
	    FDO_SAFE_RELEASE(segs);
	    return geom;
    }
    catch (FdoException* exception)
    {
        FDO_SAFE_RELEASE(segs);
        FDO_SAFE_RELEASE(geom);
        throw exception;
    }
}

FdoICurvePolygon* FdoParseFgft::DoCurvePolygon(FdoInt32& iContext, double* doubles)
{
	FdoICurvePolygon*	geom = NULL;
    FdoCurveSegmentCollection* extSegs = NULL;
    FdoIRing* extRing = NULL;
    FdoRingCollection* intRings = NULL;
    FdoCurveSegmentCollection* intSegs = NULL;
    FdoIRing* intRing = NULL;

    try
    {
	    _dims			dim = (_dims) (*m_dims)[iContext];

	    // exterior ring
	    extSegs = DoCurveSegmentCollection(iContext, doubles);
	    extRing = m_gf->CreateRing(extSegs);
	    FDO_SAFE_RELEASE(extSegs);
#ifdef DEBUG_TRACEACTIONS
		printf("extRing of Polygon\n");
#endif

	    // interior rings
	    intRings = FdoRingCollection::Create();
	    _types			typeSegment;
	    while (iContext < m_types->GetCount())
	    {
            typeSegment = (_types)(*m_types)[iContext];

            if ((typeSegment != -kCircularArcSegment) && (typeSegment != -kLineStringSegment))
                break;

		    (*m_types)[iContext] = -typeSegment;
		    intSegs = DoCurveSegmentCollection(iContext, doubles);
		    if (intSegs != NULL)
		    {
			    intRing = m_gf->CreateRing(intSegs);
			    if (intRing != NULL)
				    intRings->Add(intRing);
			    FDO_SAFE_RELEASE(intRing);
			    FDO_SAFE_RELEASE(intSegs);
#ifdef DEBUG_TRACEACTIONS
				printf("intRing of Polygon\n");
#endif
		    }
	    }	
	    geom = m_gf->CreateCurvePolygon(extRing, intRings);
	    FDO_SAFE_RELEASE(intRings);
	    FDO_SAFE_RELEASE(extRing);
	    return geom;
    }
    catch (FdoException* exception)
    {
	    FDO_SAFE_RELEASE(geom);
        FDO_SAFE_RELEASE(extSegs);
        FDO_SAFE_RELEASE(extRing);
        FDO_SAFE_RELEASE(intRings);
        FDO_SAFE_RELEASE(intSegs);
        FDO_SAFE_RELEASE(intRing);
        throw exception;
    }
}

FdoIGeometry* FdoParseFgft::DoMultiCurveString(FdoInt32& iContext, double* doubles)
{
	FdoIGeometry*	geom = NULL;
    FdoCurveStringCollection* curveStrings = NULL;
	FdoICurveString* curveString = NULL;
    
    try
    {
	    _dims			dim = (_dims) (*m_dims)[iContext];
        curveString = (FdoICurveString*)DoCurveString(iContext, doubles);
	    if (curveString != NULL)
	    {
		    curveStrings = FdoCurveStringCollection::Create();
		    curveStrings->Add(curveString);
		    FDO_SAFE_RELEASE(curveString);
	    }
	    _types			typeSegment;
	    while (iContext < m_types->GetCount())
	    {
            typeSegment = (_types)(*m_types)[iContext];

            if ((typeSegment != -kCircularArcSegment) && (typeSegment != -kLineStringSegment))
                break;

		    (*m_types)[iContext] = -typeSegment;
		    curveString = (FdoICurveString*)DoCurveString(iContext, doubles);
		    if (curveString != NULL)
		    {
			    curveStrings->Add(curveString);
			    FDO_SAFE_RELEASE(curveString);
		    }
	    }	
	    geom = m_gf->CreateMultiCurveString(curveStrings);
	    FDO_SAFE_RELEASE(curveStrings);
        return geom;
    }
    catch (FdoException* exception)
    {
        FDO_SAFE_RELEASE(curveString);
        FDO_SAFE_RELEASE(curveStrings);
        FDO_SAFE_RELEASE(geom);
        throw exception;
    }
}

FdoIGeometry* FdoParseFgft::DoMultiCurvePolygon(FdoInt32& iContext, double* doubles)
{
	FdoIGeometry*	geom = NULL;
	FdoCurvePolygonCollection * polygons = NULL;
	FdoICurvePolygon* polygon = NULL;

    try
    {
    	_dims		dim = (_dims) (*m_dims)[iContext];
        polygons = FdoCurvePolygonCollection::Create();
        polygon = DoCurvePolygon(iContext, doubles);
	    if (polygon != NULL)
	    {
#ifdef DEBUG_TRACEACTIONS
			printf("First Polygon\n");
#endif
			polygons->Add(polygon);
		    FDO_SAFE_RELEASE(polygon);
	    }

	    _types		typeSegment;
	    while (iContext < m_types->GetCount())
	    {
		    typeSegment = (_types)(*m_types)[iContext];

            if (typeSegment != kRing)
                break;

			// skip ring separator
			iContext++;
		    polygon = DoCurvePolygon(iContext, doubles);
		    if (polygon != NULL)
		    {
#ifdef DEBUG_TRACEACTIONS
				printf("Next Polygon\n");
#endif
				polygons->Add(polygon);
			    FDO_SAFE_RELEASE(polygon);
		    }
	    }	
	    geom = m_gf->CreateMultiCurvePolygon(polygons);
	    FDO_SAFE_RELEASE(polygons);
    	return geom;
    }
    catch (FdoException* exception)
    {
        FDO_SAFE_RELEASE(polygon);
        FDO_SAFE_RELEASE(polygons);
        throw exception;
    }
}

FdoIGeometry* FdoParseFgft::DoGeometryCollection(FdoInt32 iContextStart, bool bCollect)
{
	_types			type = kNone;
	FdoIGeometry*	geom = NULL;
    FdoGeometryCollection* geomCollection = NULL;

    try
    {
	    // take the value and try to constuct the geometry
	    double*			doubles = m_values->GetData();
	    FdoInt32		countContext = m_types->GetCount();
	    if (bCollect)
		    geomCollection = FdoGeometryCollection::Create();

	    FdoInt32	iCurrent = iContextStart;
	    while (iCurrent < countContext)
	    {
		    type = (_types) (*m_types)[iCurrent];
		    if (type == kPoint)
			    geom = DoPoint(iCurrent, doubles);
		    else if (type == kLineString)
			    geom = DoLineString(iCurrent, doubles, kLineString);
		    else if (type == kPolygon)
			    geom = DoPolygon(iCurrent, doubles, kPolygon);
		    else if (type == kMultiPoint)
			    geom = DoMultiPoint(iCurrent, doubles);
		    else if (type == kMultiLineString)
			    geom = DoMultiLineString(iCurrent, doubles);
		    else if (type == kMultiPolygon)
			    geom = DoMultiPolygon(iCurrent, doubles);
		    else if (type == kCurveString)
			    geom = DoCurveString(iCurrent, doubles);
		    else if (type == kCurvePolygon)
			    geom = DoCurvePolygon(iCurrent, doubles);
		    else if (type == kMultiCurveString)
			    geom = DoMultiCurveString(iCurrent, doubles);
		    else if (type == kMultiCurvePolygon)
			    geom = DoMultiCurvePolygon(iCurrent, doubles);

		    // recursion time?
		    else if (type == kMultiGeometry)
			    geom = DoGeometryCollection(++iCurrent, true);

		    // invalid type
		    else
			    return NULL;

		    // some error occured constructing geometry so give up
		    if (geom == NULL)
			    break;

		    if (bCollect)
		    {
			    // add to FdoGeometryCollection now
                geomCollection->Add(geom);
#ifdef DEBUG_TRACEACTIONS
			    printf("GeometryCollection::Add(%ls)\n", geom->GetFgftString());
#endif
			    FDO_SAFE_RELEASE(geom);
		    }
	    }
	    if (bCollect)
	    {
	        FdoIMultiGeometry* multi = m_gf->CreateMultiGeometry(geomCollection);
		    FDO_SAFE_RELEASE(geomCollection);
		    return multi;
	    }
	    else
		    return geom;
    }
    catch (FdoException* exception)
    {
        FDO_SAFE_RELEASE(geom);
        FDO_SAFE_RELEASE(geomCollection);
        throw exception;
    }
}

void FdoParseFgft::Done()
{
#ifdef DEBUG_TRACEACTIONS
	printf("Fgft::Done()\n");
	for (int i=0; i<m_types->GetCount(); i++)
	{
		_dims ii = (_dims)(*m_dims)[i];
		printf("context[%d]=%d, %d(%d,%d), %d\n", i, (*m_types)[i], 
			ii, DimToCount(ii), DimToDimensionality(ii), (*m_starts)[i]);
	}
	// redundant with SetPoint trace
//	for (int j=0; j<m_values->GetCount(); j++)
//		printf("value[%d]=%g\n", j, (*m_values)[j]);
#endif
	// if we never got any points or geometry type give up
	if (m_values->GetCount() == 0 || m_types->GetCount() == 0 ||
		// also do consistency check before we get started
		m_types->GetCount() != m_dims->GetCount() ||
		m_types->GetCount() != m_starts->GetCount())
	{
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_7_INVALIDFGF)));
	}

	_types		type = (_types) (*m_types)[0];
	bool		bMultiGeometry = type == kMultiGeometry;
	FdoInt32	iContextStart = 0;
	if (bMultiGeometry)
		iContextStart++;

	// let it throw exception if any
    m_geometry = DoGeometryCollection(iContextStart, bMultiGeometry);
}

void FdoParseFgft::Error()
{
#ifdef DEBUG_TRACEACTIONS
	printf("Fgft::Error()\n");
#endif	
	FDO_SAFE_RELEASE(m_geometry);
}

void FdoParseFgft::SetGeometryType(FdoInt32 iToken)
{
#ifdef DEBUG_TRACEACTIONS
	printf("SetGeometryType::");
	m_lex->PrintToken(iToken);
#endif
	switch (iToken)
	{
	case FdoToken_POINT:
		m_type = kPoint;
		break;
	case FdoToken_LINESTRING:
		m_type = kLineString;
		break;
	case FdoToken_POLYGON:
		m_type = kPolygon;
		break;
	case FdoToken_CURVESTRING:
		m_type = kCurveString;
		break;
	case FdoToken_CURVEPOLYGON:
		m_type = kCurvePolygon;
		break;
	case FdoToken_MULTIPOINT:
		m_type = kMultiPoint;
		break;
	case FdoToken_MULTILINESTRING:
		m_type = kMultiLineString;
		break;
	case FdoToken_MULTIPOLYGON:
		m_type = kMultiPolygon;
		break;
	case FdoToken_MULTICURVESTRING:
		m_type = kMultiCurveString;
		break;
	case FdoToken_MULTICURVEPOLYGON:
		m_type = kMultiCurvePolygon;
		break;
	case FdoToken_GEOMETRYCOLLECTION:
		m_type = kMultiGeometry;
		// special case to force entry in context table
		m_types = FdoIntArray::Append(m_types, m_type);
		m_dims = FdoIntArray::Append(m_dims, m_dim);
		m_starts = FdoIntArray::Append(m_starts, -1);
		break;
	default:
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
		break;
 	}
}

void FdoParseFgft::SetDimensionality(FdoInt32 iToken)
{
	switch (iToken)
	{
	case FdoToken_XY:
#ifdef DEBUG_TRACEACTIONS
		printf("Set XY\n");
#endif
		m_dim = kXY;
		break;
	case FdoToken_XYZ:
#ifdef DEBUG_TRACEACTIONS
		printf("Set XYZ\n");
#endif
		m_dim = kXYZ;
		break;
	case FdoToken_XYM:
#ifdef DEBUG_TRACEACTIONS
		printf("Set XYM\n");
#endif
		m_dim = kXYM;
		break;
	case FdoToken_XYZM:
#ifdef DEBUG_TRACEACTIONS
		printf("Set XYZM\n");
#endif
		m_dim = kXYZM;
		break;
	default:
#ifdef DEBUG_TRACEACTIONS
		printf("Set XY (default)\n");
#endif
		m_dim = kXY;
		break;
	}
}

void FdoParseFgft::SetBreak()
{
#ifdef DEBUG_TRACEACTIONS
		printf("SetBreak\n");
#endif
	m_break = true;
}

void FdoParseFgft::SetOuterBreak()
{
#ifdef DEBUG_TRACEACTIONS
		printf("SetOuterBreak\n");
#endif
	// force entry in context table
	m_types = FdoIntArray::Append(m_types, kRing);
	m_dims = FdoIntArray::Append(m_dims, m_dim);
	m_starts = FdoIntArray::Append(m_starts, -1);
}

FdoInt32 FdoParseFgft::DimToCount(_dims dim)
{
	switch (dim)
	{
	case kXY:
		return 2;
	case kXYZ:
	case kXYM:
		return 3;
	case kXYZM:
		return 4;
	}
	return 0;
}

FdoInt32 FdoParseFgft::DimToDimensionality(_dims dim)
{
	switch (dim)
	{
	case kXY:
		return FdoDimensionality_XY;
	case kXYZ:
		return FdoDimensionality_XY|FdoDimensionality_Z;
	case kXYM:
		return FdoDimensionality_XY|FdoDimensionality_M;
	case kXYZM:
		return FdoDimensionality_XY|FdoDimensionality_Z|FdoDimensionality_M;
	}
	return 0;
}

#ifdef _DEBUG
void FdoParseFgft::Trace(FdoString* pTrace)
{
#ifdef DEBUG_TRACETREE
	wprintf(pTrace);
#endif
}
#endif

void FdoParseFgft::SetSegmentType(FdoInt32 iToken)
{
	switch (iToken)
	{
	case FdoToken_LINESTRINGSEGMENT:
#ifdef DEBUG_TRACEACTIONS
		printf("Set LINESTRINGSEGMENT\n");
#endif
		m_type = kLineStringSegment;
		break;
	case FdoToken_CIRCULARARCSEGMENT:
#ifdef DEBUG_TRACEACTIONS
		printf("Set CIRCULARARCSEGMENT\n");
#endif
		m_type = kCircularArcSegment;
		break;
	default:
#ifdef DEBUG_TRACEACTIONS
		printf("Set LINESTRINGSEGMENT (default)\n");
#endif
		m_type = kLineStringSegment;
		break;
	}
}

void FdoParseFgft::AddPoint(double x, double y, double zm, double m)
{
	// register current state along with values
	m_types = FdoIntArray::Append(m_types, m_break ? -m_type : m_type);
	m_dims = FdoIntArray::Append(m_dims, m_dim);
	m_starts = FdoIntArray::Append(m_starts, m_values->GetCount());
	m_break = false;
#ifdef DEBUG_TRACEACTIONS
	FdoInt32	start = m_values->GetCount();
#endif

	double	axyzm[4];
	switch (m_dim)
	{
	case kXY:
#ifdef DEBUG_TRACEACTIONS
		printf("PointXY=%g, %g (%d)\n", x, y, start);
#endif
		axyzm[0] = x;
		axyzm[1] = y;
		m_values = FdoDoubleArray::Append(m_values, 2, axyzm);
		break;
	case kXYZ:
#ifdef DEBUG_TRACEACTIONS
		printf("PointXYZ=%g, %g, %g (%d)\n", x, y, zm, start);
#endif
		axyzm[0] = x;
		axyzm[1] = y;
		axyzm[2] = zm;
		m_values = FdoDoubleArray::Append(m_values, 3, axyzm);
		break;
	case kXYM:
#ifdef DEBUG_TRACEACTIONS
		printf("PointXYM=%g, %g, %g (%d)\n", x, y, zm, start);
#endif
		axyzm[0] = x;
		axyzm[1] = y;
		axyzm[2] = zm;
		m_values = FdoDoubleArray::Append(m_values, 3, axyzm);
		break;
	case kXYZM:
#ifdef DEBUG_TRACEACTIONS
		printf("PointXYZM=%g, %g, %g, %g (%d)\n", x, y, zm, m, start);
#endif
		axyzm[0] = x;
		axyzm[1] = y;
		axyzm[2] = zm;
		axyzm[3] = m;
		m_values = FdoDoubleArray::Append(m_values, 4, axyzm);
		break;
	default:
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_11_INVALIDPOINT)));
		break;
	}
}

// special lexical analyzer for limited FGFT grammar
FdoInt32 FdoParseFgft::GetLexeme(void* pyylval)
{
	FDO_FGFT_YYSTYPE* yylval = (FDO_FGFT_YYSTYPE*) pyylval;

	FdoInt32 iToken = m_lex->GetToken();
#ifdef DEBUG_TRACETOKEN
	m_lex->PrintToken(iToken);
#endif
	switch (iToken)
	{
	case FdoToken_Integer:
		yylval->m_double = (double)m_lex->m_integer;
		return FdoToken_DOUBLE;
	case FdoToken_Double:
		yylval->m_double = m_lex->m_double;
		return FdoToken_DOUBLE;

	case FdoToken_LeftParenthesis:
		return '(';
	case FdoToken_RightParenthesis:
		return ')';
	case FdoToken_Comma:
		return ',';

	// synonyms
	case FdoToken_ARC:
		return FdoToken_CIRCULARARCSEGMENT;

#ifdef _DEBUG
	case FdoToken_End:
	// keywords
	case FdoToken_BEZIERSEGMENT:
	case FdoToken_CIRCLE:
	case FdoToken_CIRCULARARCSEGMENT:
	case FdoToken_CURVEPOLYGON:
	case FdoToken_CURVESTRING:
	case FdoToken_CUSTOM:
	case FdoToken_GEOMETRYCOLLECTION:
	case FdoToken_GRIDTERRAIN:
	case FdoToken_LINESTRING:
	case FdoToken_LINESTRINGSEGMENT:
	case FdoToken_MULTICURVEPOLYGON:
	case FdoToken_MULTICURVESTRING:
	case FdoToken_MULTILINESTRING:
	case FdoToken_MULTIPOINT:
	case FdoToken_MULTIPOLYGON:
	case FdoToken_POINT:
	case FdoToken_POLYGON:
	case FdoToken_RECTANGLE:
	case FdoToken_TINTERRAIN:
	case FdoToken_XY:
	case FdoToken_XYM:
	case FdoToken_XYZ:
	case FdoToken_XYZM:
		return iToken;
	default:
		printf("!ERROR! Unknown FdoToken_=%d\n", iToken);
		return FdoToken_Error;
#else
	default:
		return iToken;
#endif // _DEBUG
	}
// unreachable	return FdoToken_Error;
}

// use separate lexical analyzer for FGFT
FdoInt32 fdo_fgft_yylex(FdoParseFgft* pParse)
{
	// pass in pointer to union to set LLVAL
	return pParse->GetLexeme(&pParse->fdo_fgft_yylval);
}

