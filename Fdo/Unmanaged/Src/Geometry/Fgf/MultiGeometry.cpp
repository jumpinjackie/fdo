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

#include "MultiGeometry.h"
#include "Polygon.h"
#include "LinearRing.h"
#include <Geometry/EnvelopeImpl.h>
#include "Util.h"
#include "Geometry/GeometryStd.h"
#include "LineString.h"
#include "Point.h"
#include "LinearRing.h"
#include "LineStringSegment.h"
#include "Polygon.h"
#include "MultiPoint.h"
#include "MultiLineString.h"
#include "MultiPolygon.h"
#include "CircularArcSegment.h"
#include "CurveString.h"
#include "MultiCurveString.h"
#include "Ring.h"
#include "CurvePolygon.h"
#include "MultiCurvePolygon.h"


/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoFgfMultiGeometry::FdoFgfMultiGeometry(
    FdoFgfGeometryFactory * factory,
    FdoGeometryCollection* geometries
    )
    : FdoFgfGeometryImpl<FdoIMultiGeometry>(factory)
{
	if ( (NULL == geometries) ||
         (0 == geometries->GetCount()) ||
		 (NULL == factory) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoFgfMultiGeometry",
                                                               L"geometries/factory"));

    FdoByteArray * newByteArray = m_factory->GetByteArray();

    FdoInt32 numGeometries = geometries->GetCount();

	FGFUTIL_WRITE_INT32(&newByteArray, FdoGeometryType_MultiGeometry);
	FGFUTIL_WRITE_INT32(&newByteArray, numGeometries);

	for (FdoInt32 i=0; i<numGeometries; i++)
    {
        FdoPtr<FdoIGeometry> geometry = geometries->GetItem(i);
        FgfUtil::WriteGeometry(geometry, &newByteArray);
    }

    SetFgf(newByteArray, NULL, 0);
    FDO_SAFE_RELEASE(newByteArray);
}

FdoFgfMultiGeometry::FdoFgfMultiGeometry(
    FdoFgfGeometryFactory * factory,
    FdoByteArray * byteArray,
    const FdoByte * data,
    FdoInt32 count
    )
    : FdoFgfGeometryImpl<FdoIMultiGeometry>(factory)
{
    Reset(byteArray, data, count);
}

void FdoFgfMultiGeometry::Reset(
    FdoByteArray * byteArray,
    const FdoByte * data,
    FdoInt32 count
    )
{
    SetFgf(byteArray, data, count);
}


/************************************************************************/
/* Destructor                                                                     */
/************************************************************************/
FdoFgfMultiGeometry::~FdoFgfMultiGeometry()
{
}


/************************************************************************/
/* GetEnvelope                                                                     */
/************************************************************************/
FdoIEnvelope* FdoFgfMultiGeometry::GetEnvelope() const
{
	return ComputeEnvelope();
}


/************************************************************************/
/* GetDimensionality                                                                     */
/************************************************************************/
FdoInt32 FdoFgfMultiGeometry::GetDimensionality() const
{
	int dimensionality = 0;

	FdoInt32 numGeometries = this->GetCount();

	for (FdoInt32 i=0; i<numGeometries; i++)
	{
        FdoPtr<FdoIGeometry>    geometry = this->GetItem(i);
        dimensionality |= geometry->GetDimensionality();
	}

	return dimensionality;
}


/************************************************************************/
/* GetDerivedType                                                                     */
/************************************************************************/
FdoGeometryType FdoFgfMultiGeometry::GetDerivedType() const
{
	return FdoGeometryType_MultiGeometry;
}

// From FdoIGeometricAggregateAbstract
/************************************************************************/
/* GetCount                                                                     */
/************************************************************************/
FdoInt32 FdoFgfMultiGeometry::GetCount() const
{
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);  // Skip over geomtype

	// Read NumGeometries
    return FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);
}


/************************************************************************/
/* GetItem                                                                     */
/************************************************************************/
FdoIGeometry* FdoFgfMultiGeometry::GetItem(FdoInt32 Index) const
{
	m_streamPtr = m_data;

    FdoPtr<FdoIGeometry> geometry =
        FgfUtil::ReadGeometryFromAggregate(m_factory, Index, FdoGeometryType_None, &m_streamPtr, m_streamEnd);

    return FDO_SAFE_ADDREF(geometry.p);
}


/************************************************************************/
/* Dispose                                                                     */
/************************************************************************/
void FdoFgfMultiGeometry::Dispose()
{
	delete this;
}

/************************************************************************/
/* Helper: ComputeEnvelope                                                                     */
/************************************************************************/
FdoIEnvelope* FdoFgfMultiGeometry::ComputeEnvelope() const
{
    FdoPtr<FdoEnvelopeImpl> envlImpl = FdoEnvelopeImpl::Create();

	FdoInt32 numGeometries = this->GetCount();

	for (FdoInt32 i=0; i<numGeometries; i++)
	{
        FdoPtr<FdoIGeometry>    geometry = this->GetItem(i);
		FdoPtr<FdoIEnvelope>   envl = geometry->GetEnvelope();
		envlImpl->Expand(envl);
	}

	return FDO_SAFE_ADDREF(envlImpl.p);
}

