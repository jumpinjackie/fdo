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


#include "Ring.h"
#include "GeometryFactory2.h"



/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoFgfRing::FdoFgfRing(
    FdoFgfGeometryFactory * factory, 
    FdoCurveSegmentCollection* curveSegs)
{
	if ( (NULL == curveSegs) ||
		 (NULL == factory) ||
         (0 == curveSegs->GetCount()) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_CREATION),
                                                               L"FdoFgfRing",
                                                               L"curveSegs/factory"));

	m_curveString = factory->CreateCurveString(curveSegs);
}


/************************************************************************/
/* Destructor                                                                     */
/************************************************************************/
FdoFgfRing::~FdoFgfRing()
{
}


/************************************************************************/
/* GetEnvelope                                                                     */
/************************************************************************/
FdoIEnvelope* FdoFgfRing::GetEnvelope() const
{
	return m_curveString->GetEnvelope();
}


/************************************************************************/
/* GetDimensionType                                                                     */
/************************************************************************/
FdoInt32 FdoFgfRing::GetDimensionality() const
{
	return m_curveString->GetDimensionality();
}


/************************************************************************/
/* GetCount                                                                     */
/************************************************************************/
FdoInt32 FdoFgfRing::GetCount() const
{
	return m_curveString->GetCount();
}


/************************************************************************/
/* GetItem                                                                     */
/************************************************************************/
FdoICurveSegmentAbstract* FdoFgfRing::GetItem(FdoInt32 index) const
{
	return m_curveString->GetItem(index);
}

/************************************************************************/
/* GetCurveSegments                                                                     */
/************************************************************************/
FdoCurveSegmentCollection* FdoFgfRing::GetCurveSegments() const
{
	return m_curveString->GetCurveSegments();
}

/************************************************************************/
/* Dispose                                                                     */
/************************************************************************/
void FdoFgfRing::Dispose()
{
	delete this;
}

