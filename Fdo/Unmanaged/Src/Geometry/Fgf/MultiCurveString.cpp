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

#include "MultiCurveString.h"
#include "CircularArcSegment.h"
#include "CurveString.h"
#include "LineStringSegment.h"
#include "Geometry/EnvelopeImpl.h"
#include "Util.h"

/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoFgfMultiCurveString::FdoFgfMultiCurveString(
    FdoFgfGeometryFactory * factory, 
    FdoFgfGeometryPools * pools,
    FdoCurveStringCollection* curveStrings
    )
    : FdoFgfGeometryImpl<FdoIMultiCurveString>(factory, pools)
{
	if ( (NULL == curveStrings) ||
         (0 == curveStrings->GetCount()) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoFgfMultiCurveString",
                                                               L"curveStrings"));

    FdoByteArray * newByteArray = FgfUtil::GetPoolsNoRef(m_pools)->GetByteArray();

	// FdoGeometryType
	FGFUTIL_WRITE_INT32(&newByteArray, FdoGeometryType_MultiCurveString);

    FdoInt32 numCurveStrings = curveStrings->GetCount();

	// NumCurveStrings
	FGFUTIL_WRITE_INT32(&newByteArray, numCurveStrings);

	for (FdoInt32 i=0; i< numCurveStrings; i++)
	{
        FdoPtr<FdoICurveString> curveString = curveStrings->GetItem(i);
		FgfUtil::WriteGeometry(curveString, &newByteArray);
	}

    SetFgf(newByteArray, NULL, 0);
    FDO_SAFE_RELEASE(newByteArray);
}

FdoFgfMultiCurveString::FdoFgfMultiCurveString(
    FdoFgfGeometryFactory * factory,
    FdoFgfGeometryPools * pools,
    FdoByteArray * byteArray,
    const FdoByte * data,
    FdoInt32 count
    )
    : FdoFgfGeometryImpl<FdoIMultiCurveString>(factory, pools)
{
    Reset(byteArray, data, count);
}

void FdoFgfMultiCurveString::Reset(
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
FdoFgfMultiCurveString::~FdoFgfMultiCurveString()
{
}


/************************************************************************/
/* GetEnvelope                                                                     */
/************************************************************************/
FdoIEnvelope* FdoFgfMultiCurveString::GetEnvelope() const
{
	return this->ComputeEnvelope();
}


/************************************************************************/
/* GetDimensionality                                                                     */
/************************************************************************/
FdoInt32 FdoFgfMultiCurveString::GetDimensionality() const
{
	// DimType of first CurveString

	// Skip over geomtype, numCurveStrings and
    // GeomType of 1st curve string
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 3);

	// DimType
	return FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);
}


/************************************************************************/
/* GetDerivedType                                                                     */
/************************************************************************/
FdoGeometryType FdoFgfMultiCurveString::GetDerivedType() const
{
	return FdoGeometryType_MultiCurveString;
}


/************************************************************************/
/* GetCount                                                                     */
/************************************************************************/
FdoInt32 FdoFgfMultiCurveString::GetCount() const
{
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);  // Skip over geomtype

	// Read NumCurveStrings
    return FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);
}


/************************************************************************/
/* GetItem                                                                     */
/************************************************************************/
FdoICurveString* FdoFgfMultiCurveString::GetItem(FdoInt32 Index) const
{
	m_streamPtr = m_data;

    FdoPtr<FdoFgfGeometryFactory> gf = GetFactory();

    FdoPtr<FdoIGeometry> geometry =
        FgfUtil::ReadGeometryFromAggregate(gf, Index, FdoGeometryType_CurveString, &m_streamPtr, m_streamEnd);

    // Smart pointer assignments have problems with non-identical types.  Work around...
    FdoICurveString * derivedGeometry = static_cast<FdoICurveString *>(geometry.p);
    FdoPtr<FdoICurveString> value = FDO_SAFE_ADDREF(derivedGeometry);
    return FDO_SAFE_ADDREF(value.p);
}


/************************************************************************/
/* Dispose                                                                     */
/************************************************************************/
void FdoFgfMultiCurveString::Dispose()
{
    SurrenderByteArray();
    FGFUTIL_DISPOSE_TO_POOL_OR_HEAP(MultiCurveString);
}


/************************************************************************/
/* ComputeEnvelope                                                                     */
/************************************************************************/
FdoIEnvelope* FdoFgfMultiCurveString::ComputeEnvelope() const
{
    FdoPtr<FdoEnvelopeImpl> envlImpl = FdoEnvelopeImpl::Create();

	FdoInt32 numCurveStrings = this->GetCount();

	for (FdoInt32 i=0; i<numCurveStrings; i++)
	{
		FdoPtr<FdoICurveString> curveString = this->GetItem(i);
		FdoIEnvelope* envl = curveString->GetEnvelope();
		envlImpl->Expand(envl);
	}

	return FDO_SAFE_ADDREF(envlImpl.p);
}

