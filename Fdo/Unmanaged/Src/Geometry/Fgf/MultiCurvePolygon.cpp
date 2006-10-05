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

#include "MultiCurvePolygon.h"
#include "Util.h"
#include <Geometry/EnvelopeImpl.h>

/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoFgfMultiCurvePolygon::FdoFgfMultiCurvePolygon(
    FdoFgfGeometryFactory * factory,
    FdoCurvePolygonCollection* curvePolygons
    )
    : FdoFgfGeometryImpl<FdoIMultiCurvePolygon>(factory)
{
	if ( (NULL == curvePolygons) ||
         (0 == curvePolygons->GetCount()) ||
		 (NULL == factory) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoFgfMultiCurvePolygon",
                                                               L"curvePolygons/factory"));

    FdoByteArray * newByteArray = m_factory->GetByteArray();

    // FdoGeometryType
	FGFUTIL_WRITE_INT32(&newByteArray, FdoGeometryType_MultiCurvePolygon);

    FdoInt32 numCurvePolygons = curvePolygons->GetCount();

	// NumCurvePolygons
	FGFUTIL_WRITE_INT32(&newByteArray, numCurvePolygons);

	// Polygons
	for (FdoInt32 i=0; i<numCurvePolygons; i++)
	{
        FdoPtr<FdoICurvePolygon> curvePolygon = curvePolygons->GetItem(i);
		FgfUtil::WriteGeometry(curvePolygon, &newByteArray);
	}

    SetFgf(newByteArray, NULL, 0);
    FDO_SAFE_RELEASE(newByteArray);
}

FdoFgfMultiCurvePolygon::FdoFgfMultiCurvePolygon(
    FdoFgfGeometryFactory * factory,
    FdoByteArray * byteArray,
    const FdoByte * data,
    FdoInt32 count
    )
    : FdoFgfGeometryImpl<FdoIMultiCurvePolygon>(factory, byteArray, data, count)
{
}


/************************************************************************/
/* Destructor                                                                     */
/************************************************************************/
FdoFgfMultiCurvePolygon::~FdoFgfMultiCurvePolygon()
{
}


/************************************************************************/
/* GetEnvelope                                                                     */
/************************************************************************/
FdoIEnvelope* FdoFgfMultiCurvePolygon::GetEnvelope() const
{
	return this->ComputeEnvelope();
}


/************************************************************************/
/* GetDimensionality                                                                     */
/************************************************************************/
FdoInt32 FdoFgfMultiCurvePolygon::GetDimensionality() const
{
	// FdoDimensionality of first curvepolygon

	// Skip over FdoGeometryType, NumCurvePolygons and 
    // FdoGeometryType of 1st CurvePolygon
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 3);

	// Read FdoDimensionality
	return FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);
}


/************************************************************************/
/* GetDerivedType                                                                     */
/************************************************************************/
FdoGeometryType FdoFgfMultiCurvePolygon::GetDerivedType() const
{
	return FdoGeometryType_MultiCurvePolygon;
}


/************************************************************************/
/* GetCount                                                                     */
/************************************************************************/
FdoInt32 FdoFgfMultiCurvePolygon::GetCount() const
{
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);  // Skip over geomtype

	// Read NumCurvePolygons
    return FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);
}


/************************************************************************/
/* GetItem                                                                     */
/************************************************************************/
FdoICurvePolygon* FdoFgfMultiCurvePolygon::GetItem(FdoInt32 Index) const
{
	m_streamPtr = m_data;

    FdoPtr<FdoIGeometry> geometry =
        FgfUtil::ReadGeometryFromAggregate(m_factory, Index, FdoGeometryType_CurvePolygon, &m_streamPtr, m_streamEnd);

    // Smart pointer assignments have problems with non-identical types.  Work around...
    FdoICurvePolygon * derivedGeometry = static_cast<FdoICurvePolygon *>(geometry.p);
    FdoPtr<FdoICurvePolygon> value = FDO_SAFE_ADDREF(derivedGeometry);
    return FDO_SAFE_ADDREF(value.p);
}


/************************************************************************/
/* Dispose()                                                                     */
/************************************************************************/
void FdoFgfMultiCurvePolygon::Dispose()
{
	delete this;
}


/************************************************************************/
/* Helper: ComputeEnvelope()                                                                     */
/************************************************************************/
FdoIEnvelope* FdoFgfMultiCurvePolygon::ComputeEnvelope() const
{
    FdoPtr<FdoEnvelopeImpl> envlImpl = FdoEnvelopeImpl::Create();

	// NumCurvePolygons
	FdoInt32 numCurvePolygons = this->GetCount();

	for (FdoInt32 i=0; i<numCurvePolygons; i++)
	{
		FdoPtr<FdoICurvePolygon> curvePolygon = this->GetItem(i);
		FdoPtr<FdoIEnvelope> envl = curvePolygon->GetEnvelope();
		envlImpl->Expand(envl);
	}

	return FDO_SAFE_ADDREF(envlImpl.p);
}

