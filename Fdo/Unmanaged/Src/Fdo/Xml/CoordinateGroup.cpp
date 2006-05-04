// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include "CoordinateGroup.h"


FdoXmlCoordinateGroup::FdoXmlCoordinateGroup()
{
	m_dimensionality = FdoDimensionality_XY;
	m_doubleArray = NULL;
}

FdoXmlCoordinateGroup::~FdoXmlCoordinateGroup()
{
	if(m_doubleArray != NULL)
		delete m_doubleArray;
}


FdoXmlCoordinateGroup* FdoXmlCoordinateGroup::Create()
{
	return new FdoXmlCoordinateGroup();
}

void FdoXmlCoordinateGroup::AddCoordinate(FdoString* coordinate)
{
	parseCoordinates(coordinate);
}

void FdoXmlCoordinateGroup::AddCoordX(FdoString* x)
{
	parseCoordinates(x);
}

void FdoXmlCoordinateGroup::AddCoordY(FdoString* y)
{
	parseCoordinates(y);
}

void FdoXmlCoordinateGroup::AddCoordZ(FdoString* z)
{
	m_dimensionality = FdoDimensionality_Z;
	parseCoordinates(z);

}


FdoDimensionality FdoXmlCoordinateGroup::GetDimensionality()
{
	return m_dimensionality;
}

int FdoXmlCoordinateGroup::GetSize()
{
	return (int)m_coordinates.size();
}

double* FdoXmlCoordinateGroup::GetDoubleArray()
{
	if(m_doubleArray == NULL){
		int size = (int)m_coordinates.size();
		m_doubleArray = new double[size];
		for(int i = 0; i < size; i++)
			m_doubleArray[i] = m_coordinates[i];
	}

	return m_doubleArray;
}

// Helper macro to convert a wide character string into a multibyte string, allocating space on the stack
#ifndef _WIN32
#ifndef wide_to_multibyte 
#define wide_to_multibyte(mb,w)\
{\
	const wchar_t* p = (w);\
	size_t i = wcslen(p);\
	i++;\
	mb = (char*)alloca(i * 6);\
	i = wcstombs (mb, p, i);\
	if (0 > i)\
		mb = NULL;\
	if (NULL == mb) throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));\
}

#endif
#endif//_WIN32

///////////////////////////////////////////////////////////////
/// Helper functions
///////////////////////////////////////////////////////////////
#include <iostream>
void FdoXmlCoordinateGroup::parseCoordinates(FdoString* value)
{
	FdoStringP coordString = value;

	FdoString* startPos = coordString;
	FdoString* endPos;

	int coordinateCounter = 0;

	while(*startPos != L'\0'){

		//find next start
		while(*startPos !=  L'\0' && !isCoordinateChar(*startPos))
			startPos++;

		//find end
		endPos = startPos;
		while(*endPos !=  L'\0' && isCoordinateChar(*endPos) )
			endPos++;

		//calculate number of coordinates in a point
		//if there are 3 coordinates in point, set dimensionality
		if(*endPos == L','){
			coordinateCounter ++;

			if(coordinateCounter == 2){
				m_dimensionality = FdoDimensionality_Z;
				coordinateCounter = 0;
			}

		}else{
			coordinateCounter = 0;
		}
		

		//convert number
		if(endPos > startPos){
            // FdoStringP::Mid is extremely slow so we can not rely on it.
			//FdoStringP buf = coordString.Mid(startPos - coordString, endPos - startPos);
            wchar_t backup = *endPos;
            *const_cast<wchar_t*>(endPos) = 0;
#ifdef _WIN32
			double value = _wtof(startPos);
#else
            char* mb;
            wide_to_multibyte(mb, startPos);
            double value = atof(mb);
#endif
            *const_cast<wchar_t*>(endPos) = backup;
			m_coordinates.push_back(value);          
		}
		
		startPos = endPos;
	}

}

bool FdoXmlCoordinateGroup::isCoordinateChar(FdoCharacter c)
{
	if( c == L'E' || c == L'e' || c == L'.' || c==L'-' || (c >= L'0' && c <= L'9'))
		return true;
	return false;
}


