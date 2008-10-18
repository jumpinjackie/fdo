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

#include "MultiPolygon.h"
#include <Geometry/EnvelopeImpl.h>
#include "Util.h"
#include "GeometryFactory2.h"


/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoFgfMultiPolygon::FdoFgfMultiPolygon(
    FdoFgfGeometryFactory * factory,
    FdoFgfGeometryPools * pools,
    FdoPolygonCollection* polygons
    )
    : FdoFgfGeometryImpl<FdoIMultiPolygon>(factory, pools)
{
	if ( (NULL == polygons) ||
         (0 == polygons->GetCount()) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoFgfMultiPolygon",
                                                               L"polygons/factory"));

    FdoByteArray * newByteArray = FgfUtil::GetPoolsNoRef(m_pools)->GetByteArray();

    FdoInt32 numPolygons = polygons->GetCount();

	FGFUTIL_WRITE_INT32(&newByteArray, FdoGeometryType_MultiPolygon);
	FGFUTIL_WRITE_INT32(&newByteArray, numPolygons);

	for (FdoInt32 i=0; i<numPolygons; i++)
    {
        FdoPtr<FdoIPolygon> polygon = polygons->GetItem(i);
        FgfUtil::WriteGeometry(polygon, &newByteArray);
    }

    SetFgf(newByteArray, NULL, 0);
    FDO_SAFE_RELEASE(newByteArray);
}

FdoFgfMultiPolygon::FdoFgfMultiPolygon(
    FdoFgfGeometryFactory * factory,
    FdoFgfGeometryPools * pools,
    FdoByteArray * byteArray,
    const FdoByte * data,
    FdoInt32 count
    )
    : FdoFgfGeometryImpl<FdoIMultiPolygon>(factory, pools)
{
    Reset(byteArray, data, count);
}

void FdoFgfMultiPolygon::Reset(
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
FdoFgfMultiPolygon::~FdoFgfMultiPolygon()
{
}


/************************************************************************/
/* GetEnvelope                                                                     */
/************************************************************************/
FdoIEnvelope* FdoFgfMultiPolygon::GetEnvelope() const
{
	return ComputeEnvelope();
}


/************************************************************************/
/* GetDimensionality                                                                     */
/************************************************************************/
FdoInt32 FdoFgfMultiPolygon::GetDimensionality() const
{
	// return dimtype of 1st polygon

	// Skip over geomtype, numpolygons of MultiPolygon
	// and geomtype of polygon
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 3);

	return FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);
}


/************************************************************************/
/* GetDerivedType                                                                     */
/************************************************************************/
FdoGeometryType FdoFgfMultiPolygon::GetDerivedType() const
{
	return FdoGeometryType_MultiPolygon;
}

// From FdoIGeometricAggregateAbstract
/************************************************************************/
/* GetCount                                                                     */
/************************************************************************/
FdoInt32 FdoFgfMultiPolygon::GetCount() const
{
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);  // Skip over geomtype

    return FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);
}


/************************************************************************/
/* GetItem                                                                     */
/************************************************************************/
FdoIPolygon* FdoFgfMultiPolygon::GetItem(FdoInt32 Index) const
{
    m_streamPtr = m_data;

    FdoPtr<FdoFgfGeometryFactory> gf = GetFactory();

    FdoPtr<FdoIGeometry> geometry =
        FgfUtil::ReadGeometryFromAggregate(gf, Index, FdoGeometryType_Polygon, &m_streamPtr, m_streamEnd);

    // Smart pointer assignments have problems with non-identical types.  Work around...
    FdoIPolygon * derivedGeometry = static_cast<FdoIPolygon *>(geometry.p);
    FdoPtr<FdoIPolygon> value = FDO_SAFE_ADDREF(derivedGeometry);
    return FDO_SAFE_ADDREF(value.p);
}


/************************************************************************/
/* Dispose                                                                     */
/************************************************************************/
void FdoFgfMultiPolygon::Dispose()
{
    SurrenderByteArray();
    FGFUTIL_DISPOSE_TO_POOL_OR_HEAP(MultiPolygon);
}


/************************************************************************/
/* Helper: ComputeEnvelope                                                                     */
/************************************************************************/
FdoIEnvelope* FdoFgfMultiPolygon::ComputeEnvelope() const
{
    FdoPtr<FdoEnvelopeImpl> envlImpl = FdoEnvelopeImpl::Create();

	FdoInt32 numPolygons = this->GetCount();

	for (FdoInt32 i=0; i<numPolygons; i++)
	{
        FdoPtr<FdoIPolygon>    polygon = this->GetItem(i);
		FdoPtr<FdoIEnvelope>   envl = polygon->GetEnvelope();
		envlImpl->Expand(envl);
	}

	return FDO_SAFE_ADDREF(envlImpl.p);
}



