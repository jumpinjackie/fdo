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

#include "MultiLineString.h"
#include "LineString.h"
#include <Geometry/EnvelopeImpl.h>
#include "Util.h"
#include "GeometryFactory2.h"


/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoFgfMultiLineString::FdoFgfMultiLineString(
    FdoFgfGeometryFactory * factory,
    FdoFgfGeometryPools * pools,
    FdoLineStringCollection* lineStrings
    )
    : FdoFgfGeometryImpl<FdoIMultiLineString>(factory, pools)
{
	if ( (NULL == lineStrings) ||
         (0 == lineStrings->GetCount()) ||
		 (NULL == factory) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoFgfMultiLineString",
                                                               L"lineStrings/factory"));

    FdoByteArray * newByteArray = FgfUtil::GetPoolsNoRef(m_pools)->GetByteArray();

	// FdoGeometryType
	FGFUTIL_WRITE_INT32(&newByteArray, FdoGeometryType_MultiLineString);

    FdoInt32 numLineStrings = lineStrings->GetCount();

	// Number of linestring
	FGFUTIL_WRITE_INT32(&newByteArray, numLineStrings);

	for (FdoInt32 i=0; i<numLineStrings; i++)
    {
        FdoPtr<FdoILineString> lineString = lineStrings->GetItem(i);
        FgfUtil::WriteGeometry(lineString, &newByteArray);
    }

    SetFgf(newByteArray, NULL, 0);
    FDO_SAFE_RELEASE(newByteArray);
}

FdoFgfMultiLineString::FdoFgfMultiLineString(
    FdoFgfGeometryFactory * factory,
    FdoFgfGeometryPools * pools,
    FdoByteArray * byteArray,
    const FdoByte * data,
    FdoInt32 count
    )
    : FdoFgfGeometryImpl<FdoIMultiLineString>(factory, pools)
{
    Reset(byteArray, data, count);
}

void FdoFgfMultiLineString::Reset(
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
FdoFgfMultiLineString::~FdoFgfMultiLineString()
{
}


/************************************************************************/
/* Getenvelope                                                                     */
/************************************************************************/
FdoIEnvelope* FdoFgfMultiLineString::GetEnvelope() const
{
	return this->ComputeEnvelope();
}


/************************************************************************/
/* GetDimensionality                                                                     */
/************************************************************************/
FdoInt32 FdoFgfMultiLineString::GetDimensionality() const
{
	// Dimension of 1st linestring

	// Skip over geomtype, numlinestrings and geomtype of linestring
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 3);

	return FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);
}

/************************************************************************/
/* GetDerivedType                                                                     */
/************************************************************************/
FdoGeometryType FdoFgfMultiLineString::GetDerivedType() const
{
	return FdoGeometryType_MultiLineString;
}


/************************************************************************/
/* GetCount                                                                     */
/************************************************************************/
FdoInt32 FdoFgfMultiLineString::GetCount() const
{
	m_streamPtr = m_data;
	FGFUTIL_SKIP_INT32S(&m_streamPtr, m_streamEnd, 1);  // Skip over geomtype

	// NumLineStrings
	return FgfUtil::ReadInt32(&m_streamPtr, m_streamEnd);
}


/************************************************************************/
/* GetItem                                                                     */
/************************************************************************/
FdoILineString* FdoFgfMultiLineString::GetItem(FdoInt32 Index) const
{
	m_streamPtr = m_data;

    FdoPtr<FdoFgfGeometryFactory> gf = GetFactory();

    FdoPtr<FdoIGeometry> geometry =
        FgfUtil::ReadGeometryFromAggregate(gf, Index, FdoGeometryType_LineString, &m_streamPtr, m_streamEnd);

    // Smart pointer assignments have problems with non-identical types.  Work around...
    FdoILineString * derivedGeometry = static_cast<FdoILineString *>(geometry.p);
    FdoPtr<FdoILineString> value = FDO_SAFE_ADDREF(derivedGeometry);
    return FDO_SAFE_ADDREF(value.p);
}


/************************************************************************/
/* Dispose                                                                     */
/************************************************************************/
void FdoFgfMultiLineString::Dispose()
{
    SurrenderByteArray();
    FGFUTIL_DISPOSE_TO_POOL_OR_HEAP(MultiLineString);
}


/************************************************************************/
/* Helper: ComputeEnvelope                                                                     */
/************************************************************************/
FdoIEnvelope* FdoFgfMultiLineString::ComputeEnvelope() const
{
	FdoInt32 numLineStrings = this->GetCount();
    FdoPtr<FdoEnvelopeImpl> envlImpl = FdoEnvelopeImpl::Create();

	for (FdoInt32 i=0; i<numLineStrings; i++)
	{
		FdoPtr<FdoILineString> linestring = this->GetItem(i);
		FdoPtr<FdoIEnvelope> tmpEnvl = linestring->GetEnvelope();
		envlImpl->Expand(tmpEnvl);
	}

	return FDO_SAFE_ADDREF(envlImpl.p);
}

