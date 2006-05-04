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

#include "CircularArcSegment.h"


/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoFgfCircularArcSegment::FdoFgfCircularArcSegment(
    FdoFgfGeometryFactory * factory, 
    FdoIDirectPosition* startPoint,
    FdoIDirectPosition* midPoint,
    FdoIDirectPosition* endPoint)
{
	if ( (NULL == startPoint) ||
		 (NULL == midPoint) ||
		 (NULL == endPoint) ||
		 (NULL == factory) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

    FdoPtr<FdoDirectPositionCollection> positions = FdoDirectPositionCollection::Create();
    positions->Add(startPoint);
    positions->Add(midPoint);
    positions->Add(endPoint);

    m_lineString = factory->CreateLineString(positions);
}
/************************************************************************/
/* Destructor                                                                     */
/************************************************************************/
FdoFgfCircularArcSegment::~FdoFgfCircularArcSegment()
{
}


/************************************************************************/
/* GetEnvelope()                                                                     */
/************************************************************************/
FdoIEnvelope* FdoFgfCircularArcSegment::GetEnvelope() const
{
	return m_lineString->GetEnvelope();
}


/************************************************************************/
/* GetStartPosition                                                                     */
/************************************************************************/
FdoIDirectPosition* FdoFgfCircularArcSegment::GetStartPosition() const
{
	// First postion is StartPoint
	return m_lineString->GetItem(0);		// 0 based index
}


/************************************************************************/
/* GetEndPosition                                                                     */
/************************************************************************/
FdoIDirectPosition* FdoFgfCircularArcSegment::GetEndPosition() const
{
	// 3rd position is EndPoint
	return m_lineString->GetItem(2);		// 0 based index
}


/************************************************************************/
/* GetIsClosed()                                                                     */
/************************************************************************/
bool FdoFgfCircularArcSegment::GetIsClosed() const
{
	return m_lineString->GetIsClosed();
}


/************************************************************************/
/* GetDerivedType                                                                     */
/************************************************************************/
FdoGeometryComponentType FdoFgfCircularArcSegment::GetDerivedType() const
{
	return FdoGeometryComponentType_CircularArcSegment;
}


/************************************************************************/
/* GetDimensionality                                                                     */
/************************************************************************/
FdoInt32 FdoFgfCircularArcSegment::GetDimensionality() const
{
	return m_lineString->GetDimensionality();
}


/************************************************************************/
/* GetMidPoint                                                                     */
/************************************************************************/
FdoIDirectPosition* FdoFgfCircularArcSegment::GetMidPoint() const
{
	// 2nd position is midpoint
	return m_lineString->GetItem(1);		// 0 based index
}


/************************************************************************/
/* Dispose                                                                     */
/************************************************************************/
void FdoFgfCircularArcSegment::Dispose()
{
	delete this;
}

