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


#include "Point.h"
#include <Geometry/DirectPositionImpl.h>
#include <Geometry/EnvelopeImpl.h>
#include "Util.h"


/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoFgfPoint::FdoFgfPoint(
        FdoFgfGeometryFactory * factory,
        FdoInt32 dimensionType,
        double* ordinates
        )
    : FdoFgfGeometryImpl<FdoIPoint>(factory)
{
	if ( (NULL == factory) ||
		 (NULL == ordinates) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

    // Cannot use smart pointer for updating a FdoArray.
    FdoByteArray * newByteArray = m_factory->GetByteArray();

	FGFUTIL_WRITE_INT32(&newByteArray, FdoGeometryType_Point);
	FGFUTIL_WRITE_INT32(&newByteArray, dimensionType);
	
    FdoInt32 numOrds = GeometryUtility::DimensionalityToNumOrdinates(dimensionType);

    FGFUTIL_WRITE_DOUBLES(&newByteArray, numOrds, ordinates);

    SetFgf(newByteArray, NULL, 0);
    FDO_SAFE_RELEASE(newByteArray);
}


/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoFgfPoint::FdoFgfPoint(
    FdoFgfGeometryFactory * factory,
    FdoIDirectPosition* position
    )
    : FdoFgfGeometryImpl<FdoIPoint>(factory)
{
	if ( (NULL == factory) ||
		 (NULL == position) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

    FdoByteArray * newByteArray = m_factory->GetByteArray();

	FGFUTIL_WRITE_INT32(&newByteArray, FdoGeometryType_Point);
	FGFUTIL_WRITE_INT32(&newByteArray, position->GetDimensionality());
    FgfUtil::WriteDirectPosition(&newByteArray, position);

    SetFgf(newByteArray, NULL, 0);
    FDO_SAFE_RELEASE(newByteArray);
}

FdoFgfPoint::FdoFgfPoint(
    FdoFgfGeometryFactory * factory,
    FdoByteArray * byteArray,
    const FdoByte * data,
    FdoInt32 count
    )
    : FdoFgfGeometryImpl<FdoIPoint>(factory, byteArray, data, count)
{
}


/************************************************************************/
/* Destructor                                                                     */
/************************************************************************/
FdoFgfPoint::~FdoFgfPoint()
{
}


/************************************************************************/
/* GetPosition() - return DirectPosition                                                                     */
/************************************************************************/
FdoIDirectPosition* FdoFgfPoint::GetPosition() const
{
	FdoInt32 dimensionality = GetDimensionality();
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 2);      // Skip over geomtype and dimtype
    return FgfUtil::ReadDirectPosition(m_factory, dimensionality, &m_streamPtr, m_streamEnd);
}

void FdoFgfPoint::GetPositionByMembers(double *x, double *y, double *z, double *m, FdoInt32 *dimensionality) const
{
	*dimensionality = GetDimensionality();
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 2);      // Skip over geomtype and dimtype
	FGFUTIL_READ_DIRECTPOSITION_BY_MEMBERS(&m_streamPtr, m_streamEnd, *dimensionality, x, y, z, m);
}

const double * FdoFgfPoint::GetOrdinates()
{
	m_streamPtr = m_data;
    // Skip Geometry type and dimensionality.
    FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 2);
    const double * ordinates = (const double *) m_streamPtr;

    return ordinates;
}


/************************************************************************/
/* GweteEnvelope() - return an empty envelope                                                                     */
/************************************************************************/
FdoIEnvelope* FdoFgfPoint::GetEnvelope() const
{
	FdoPtr<FdoIDirectPosition> pos = this->GetPosition();

    FdoPtr<FdoEnvelopeImpl> envelope = 
        FdoEnvelopeImpl::Create(pos->GetX(), pos->GetY(), pos->GetZ(),
		                        pos->GetX(), pos->GetY(), pos->GetZ());

    return FDO_SAFE_ADDREF(envelope.p);
}


/************************************************************************/
/* GetDimensionality() - return dimensionality                                                                      */
/************************************************************************/
FdoInt32 FdoFgfPoint::GetDimensionality() const
{
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);  // Skip over geomtype
    return FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);
}


/************************************************************************/
/* GetDerivedType() - return the type of this geometry                                                                     */
/************************************************************************/
FdoGeometryType FdoFgfPoint::GetDerivedType() const
{
	return FdoGeometryType_Point;
}


/************************************************************************/
/* Dispose() - dispose ourself                                                                     */
/************************************************************************/
void FdoFgfPoint::Dispose()
{
	delete this;
}

