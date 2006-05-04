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

#include "LineStringSegment.h"
#include <Geometry/DirectPositionImpl.h>
#include <Geometry/EnvelopeImpl.h>
#include "Util.h"


/* 
* Constructor, copying the positions.  Dimensionality is taken from the first position.
*/
FdoFgfLineStringSegment::FdoFgfLineStringSegment(
    FdoFgfGeometryFactory * factory, 
    FdoDirectPositionCollection * positions)
{

	if ( (positions == NULL) ||
		 (factory == NULL) ||
         (0 == positions->GetCount()) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));
	
    m_lineString = factory->CreateLineString(positions);
}


/* 
* Constructor, copying the ordinates.
*/
FdoFgfLineStringSegment::FdoFgfLineStringSegment(
    FdoFgfGeometryFactory * factory, 
    FdoInt32 dimensionType,
    FdoInt32 numOrdinates,
    double* ordinates)
{
	if ( (NULL == ordinates) ||
	     (NULL == factory) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	m_lineString = factory->CreateLineString(dimensionType, numOrdinates, ordinates);
}

/************************************************************************/
/* Destructor                                                                     */
/************************************************************************/
FdoFgfLineStringSegment::~FdoFgfLineStringSegment()
{
}



/************************************************************************/
/* GetDimensionality() - return dimension                                                                     */
/************************************************************************/
FdoInt32 FdoFgfLineStringSegment::GetDimensionality() const
{
	return m_lineString->GetDimensionality();
}


/************************************************************************/
/* GetEnvelope() - return envelope                                                                     */
/************************************************************************/
FdoIEnvelope* FdoFgfLineStringSegment::GetEnvelope() const
{
	return m_lineString->GetEnvelope();
}


/************************************************************************/
/* GetStartPoint() - return start point                                                                     */
/************************************************************************/
FdoIDirectPosition* FdoFgfLineStringSegment::GetStartPosition() const
{
	return m_lineString->GetStartPosition();
}


/************************************************************************/
/* GetEndPoint() - return end point                                                                     */
/************************************************************************/
FdoIDirectPosition* FdoFgfLineStringSegment::GetEndPosition() const
{
	return m_lineString->GetEndPosition();
}

/************************************************************************/
/* GetIsClosed() - return is closed                                                                     */
/************************************************************************/
bool FdoFgfLineStringSegment::GetIsClosed() const
{
	return m_lineString->GetIsClosed();	
}

/************************************************************************/
/* GetPositions()                                                                     */
/************************************************************************/
FdoDirectPositionCollection* FdoFgfLineStringSegment::GetPositions()
{
	return m_lineString->GetPositions();
}

const double * FdoFgfLineStringSegment::GetOrdinates()
{
    return m_lineString->GetOrdinates();
}


/************************************************************************/
/* GetDeriveType() - return derived type                                                                     */
/************************************************************************/
FdoGeometryComponentType FdoFgfLineStringSegment::GetDerivedType() const
{
	return 	FdoGeometryComponentType_LineStringSegment;
}

/************************************************************************/
/* GetItem() - return indexed position                                                                     */
/************************************************************************/
FdoIDirectPosition* FdoFgfLineStringSegment::GetItem(FdoInt32 index) const
{
	return m_lineString->GetItem(index);
}

/************************************************************************/
/* GetCount() - return number of positions                                                                     */
/************************************************************************/
FdoInt32 FdoFgfLineStringSegment::GetCount() const
{
	return m_lineString->GetCount();
}


/************************************************************************/
/* Dispose() : Dispose ourself                                                                     */
/************************************************************************/
void FdoFgfLineStringSegment::Dispose()
{
	delete this;
}

