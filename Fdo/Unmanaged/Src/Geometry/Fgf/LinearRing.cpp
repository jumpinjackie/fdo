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


#include "LinearRing.h"
#include "GeometryFactory2.h"


/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoFgfLinearRing::FdoFgfLinearRing(
    FdoFgfGeometryFactory * factory, 
    FdoInt32 dimensionType,
    FdoInt32 numOrdinates,
    double* ordinates)
{
	if ( (NULL == ordinates) ||
		 (NULL == factory) )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	// Should be closed
	if (!IsClosed(dimensionType, numOrdinates, ordinates))
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	m_lineString = factory->CreateLineString(dimensionType, numOrdinates, ordinates);

#ifdef EXTRA_DEBUG
    {
        FILE * FdoDebugFile = fopen("D:\\temp\\FdoDebugFile_FdoFgfLinearRing_alloc.log", "a+");
        fprintf(FdoDebugFile, "    FdoFgfLinearRing allocated at 0x%lx.\n", (long)this);
        fclose(FdoDebugFile);
    }
#endif
}

void FdoFgfLinearRing::Reset(FdoInt32 dimensionType, FdoInt32 numOrdinates, double* ordinates)
{
	if (NULL == ordinates)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	// Should be closed
	if (!IsClosed(dimensionType, numOrdinates, ordinates))
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

    FdoFgfLineString * ls = static_cast<FdoFgfLineString *>(m_lineString.p);
    ls->Reset(dimensionType, numOrdinates, ordinates);

#ifdef EXTRA_DEBUG
    {
        FILE * FdoDebugFile = fopen("D:\\temp\\FdoDebugFile_FdoFgfLinearRing_reset.log", "a+");
        fprintf(FdoDebugFile, "    FdoFgfLinearRing reset at 0x%lx.\n", (long)this);
        fclose(FdoDebugFile);
    }
#endif
}


/************************************************************************/
/* Constructor                                                                     */
/************************************************************************/
FdoFgfLinearRing::FdoFgfLinearRing(
    FdoFgfGeometryFactory * factory, 
    FdoDirectPositionCollection* positions)
{
    if ( NULL == positions || positions->GetCount() < 3)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

    m_lineString = factory->CreateLineString(positions);

#ifdef EXTRA_DEBUG
    {
        FILE * FdoDebugFile = fopen("D:\\temp\\FdoDebugFile_FdoFgfLinearRing_alloc.log", "a+");
        fprintf(FdoDebugFile, "    FdoFgfLinearRing allocated at 0x%lx.\n", (long)this);
        fclose(FdoDebugFile);
    }
#endif
}

void FdoFgfLinearRing::Reset(FdoDirectPositionCollection* positions)
{
    if ( NULL == positions ||  positions->GetCount() < 3)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

    FdoFgfLineString * ls = static_cast<FdoFgfLineString *>(m_lineString.p);
    ls->Reset(positions);

#ifdef EXTRA_DEBUG
    {
        FILE * FdoDebugFile = fopen("D:\\temp\\FdoDebugFile_FdoFgfLinearRing_Reset.log", "a+");
        fprintf(FdoDebugFile, "    FdoFgfLinearRing reset at 0x%lx.\n", (long)this);
        fclose(FdoDebugFile);
    }
#endif
}


/************************************************************************/
/* Destructor                                                                     */
/************************************************************************/
FdoFgfLinearRing::~FdoFgfLinearRing()
{

#ifdef EXTRA_DEBUG
    {
        FILE * FdoDebugFile = fopen("D:\\temp\\FdoDebugFile_FdoFgfLinearRing_free.log", "a+");
        fprintf(FdoDebugFile, "    FdoFgfLinearRing freed at 0x%lx.\n", (long)this);
        fclose(FdoDebugFile);
    }
#endif
}


/************************************************************************/
/* GetDimensionality() - return dimensionality                                                                     */
/************************************************************************/
FdoInt32 FdoFgfLinearRing::GetDimensionality() const
{
	return m_lineString->GetDimensionality();
}


/************************************************************************/
/* GetCount() - return the number of positions                                                                    */
/************************************************************************/
FdoInt32 FdoFgfLinearRing::GetCount() const
{
	return m_lineString->GetCount();
}


/************************************************************************/
/* GetItem() - get indexed DirectPosition                                                                     */
/************************************************************************/
FdoIDirectPosition* FdoFgfLinearRing::GetItem(FdoInt32 index) const
{
	return m_lineString->GetItem(index);
}

void FdoFgfLinearRing::GetItemByMembers(FdoInt32 index, double *x, double *y, double *z, double *m, FdoInt32 *dimensionality) const
{
	return m_lineString->GetItemByMembers(index, x, y, z, m, dimensionality);
}

/************************************************************************/
/* GetEnvelope() - return Envelope                                                                    */
/************************************************************************/
FdoIEnvelope* FdoFgfLinearRing::GetEnvelope() const
{
	return m_lineString->GetEnvelope();
}

/************************************************************************/
/* GetPositions - return DirectPosition array                                                                     */
/************************************************************************/
FdoDirectPositionCollection* FdoFgfLinearRing::GetPositions()
{
	return m_lineString->GetPositions();
}

const double * FdoFgfLinearRing::GetOrdinates()
{
    return m_lineString->GetOrdinates();
}


/************************************************************************/
/* IsClosed() - Helper to check if ordinates form a closed ring                                                                     */
/************************************************************************/
bool FdoFgfLinearRing::IsClosed(FdoInt32 dimensionality, FdoInt32 numOrdinates, double* ordinates) const
{
	// Must check if numOrdinates
	// to avoid pointer arithmatic problems

	double x1, y1, z1;
	double x2, y2, z2;

	x1 = y1 = z1 = 0.0;
	x2 = y2 = z2 = 0.0;

	// There should be minimum 4 ordinates for (x,y)

	FdoInt32	endIndex;

    if ( dimensionality == FdoDimensionality_XY )
    {
	    if (numOrdinates < 4)
	        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

        endIndex = numOrdinates - 2;    // (X,Y)

	    x1 = ordinates[0]; 
        y1 = ordinates[1];
	    x2 = ordinates[endIndex]; 
        y2 = ordinates[endIndex+1];
    } 
    else if ( dimensionality == (FdoDimensionality_XY | FdoDimensionality_Z) )
    {
	    if (numOrdinates < 6)
	        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));
        
        endIndex = numOrdinates - 3;    // (X,Y,Z)

	    x1 = ordinates[0]; 
        y1 = ordinates[1];
        z1 = ordinates[2];
	    x2 = ordinates[endIndex]; 
        y2 = ordinates[endIndex+1];
        z2 = ordinates[endIndex+2];
    }
    else if ( dimensionality == ( FdoDimensionality_XY | FdoDimensionality_M ) )
    {
	    if (numOrdinates < 6)
	        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));
        
        endIndex = numOrdinates - 3;    // (X,Y,M)

	    x1 = ordinates[0]; 
        y1 = ordinates[1];
 	    x2 = ordinates[endIndex]; 
        y2 = ordinates[endIndex+1];
    }
    else if ( dimensionality == ( FdoDimensionality_XY | FdoDimensionality_Z | FdoDimensionality_M ) )
    {
	    if (numOrdinates < 8)
	        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

        endIndex = numOrdinates - 4;    // (X,Y,Z,M)

        x1 = ordinates[0]; 
        y1 = ordinates[1];
        z1 = ordinates[2];
	    x2 = ordinates[endIndex]; 
        y2 = ordinates[endIndex+1];
        z2 = ordinates[endIndex+2];
    }

	// Note: no tolerance here -- being topological, start and end positions must
    // be identical, not just close.
	return ( (x1 == x2) &&
			 (y1 == y2) &&
			 (z1 == z2) );
}

/************************************************************************/
/* Dispose ourself                                                                     */
/************************************************************************/
void FdoFgfLinearRing::Dispose()
{
	delete this;
}

