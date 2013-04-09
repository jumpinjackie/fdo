//
// Copyright (C) 2004-2007  Autodesk, Inc.
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
//
 

#include "stdafx.h"
#include "Common/StringBuffer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

StringBuffer::StringBuffer()
{
	m_pBuffer = this->createEmptyBuffer( STRINGBUFFER_INIT_CAPACITY );
	m_lCapacity = STRINGBUFFER_INIT_CAPACITY;
}

StringBuffer::StringBuffer( const wchar_t* str ) 
{
	m_lCapacity = (long)wcslen(str) * 2;
	m_pBuffer = this->createEmptyBuffer( m_lCapacity );	
#ifdef WIN32    	
	wcscpy_s(m_pBuffer, m_lCapacity, str);
#else
	wcscpy(m_pBuffer, str);
#endif
}

StringBuffer::StringBuffer( unsigned long initCapacity )
{		
	m_pBuffer = this->createEmptyBuffer( initCapacity );
	m_lCapacity = initCapacity;	
}

StringBuffer::~StringBuffer()
{
	delete [] m_pBuffer;
}


wchar_t* StringBuffer::createEmptyBuffer( unsigned long size ) {	
	wchar_t *newBuffer = new wchar_t[ size + 1 ];
#ifdef _WIN32
	_snwprintf(newBuffer, 1, L"");
#else
	swprintf(newBuffer, 1, L"");
#endif	
	return newBuffer;
}



void StringBuffer::ensureCapacity( unsigned long size )
{	
	wchar_t* newBuffer = NULL;
	unsigned long bufferlength;

	// Don't extend the buffer if the current capacity
	// exceeds the requested size
	if ( m_lCapacity >= size ) {
		return;
	}

	// Extend the buffer to the larger of two values:
	//  - twice the size of the existing buffer
	//  - the requested size
	if (m_lCapacity * 2 + 2 > size ) {
		m_lCapacity = m_lCapacity * 2 + 2;
	}
	else {
		m_lCapacity = size;
	}
	
	newBuffer = this->createEmptyBuffer(m_lCapacity);
#ifdef _WIN32
	bufferlength = _snwprintf(newBuffer, m_lCapacity, L"%ls", m_pBuffer);
#else
	bufferlength = swprintf(newBuffer, m_lCapacity, L"%ls", m_pBuffer);
#endif
	delete [] m_pBuffer;
	m_pBuffer = newBuffer;

}


void StringBuffer::append(const wchar_t* str)
{
	unsigned long prefixLength = (unsigned long)wcslen(m_pBuffer);
	unsigned long suffixLength = (unsigned long)wcslen(str);

	// Determine the required buffer length
	unsigned long requiredLength = prefixLength + suffixLength + 4;	

	// Extend the buffer if necessary
	if (requiredLength > m_lCapacity ) {		
		this->ensureCapacity(requiredLength);
	}

	// Concatenate the suffix string into the buffer
	unsigned long bufferIndex = prefixLength;	
	unsigned long offSet = bufferIndex;

	while(bufferIndex < requiredLength ) {
		m_pBuffer[bufferIndex] = str[bufferIndex - offSet];
		bufferIndex++;
	}

	// Append the string-terminating NULL
	m_pBuffer[bufferIndex] = '\0';
}

unsigned long StringBuffer::getLength()
{
	if (m_pBuffer == NULL) {
		return 0;
	}
	return (unsigned long)wcslen( m_pBuffer );
}

unsigned long StringBuffer::getCapacity()
{
	return m_lCapacity;
}

void StringBuffer::insert(unsigned long offset, const wchar_t *str)
{
	long subStringLength = (long)wcslen(str);
	wchar_t* oldSubString = this->substring(offset, this->getLength());

	// Resize the buffer to the appropriate size
	long requiredLength = (long)wcslen(m_pBuffer) + subStringLength + 1;	
	this->ensureCapacity( requiredLength );

	// Copy the new substring into the offset, and add the terminating NULL
	for (long index = 0; index <= subStringLength; index ++ ) {
		m_pBuffer[index + offset] = str[index];
	}
	m_pBuffer[offset + subStringLength] = L'\0';
	
	// Concatenate the previous substring
	wcscat(m_pBuffer, oldSubString);
}


wchar_t* StringBuffer::substring(unsigned long start, unsigned long end) 
{
	wchar_t* pSubString;
	unsigned long index;
	unsigned long currentwcslength = this->getLength();

	if ((end < start) || (start > currentwcslength )){
		return NULL;
	}	
	if (end > currentwcslength ) {
		end = currentwcslength;
	}

	pSubString = new wchar_t[ end - start + 2 ];

	// Copy the substring from the original buffer
	// into the new subString
	for( index = start; index <= end; index ++ ) {
		pSubString[ index - start ] = m_pBuffer[index];
	}

	// End the substring with a terminating NULL
	pSubString[ end - start + 1] = '\0';

	return pSubString;	
}

const wchar_t* StringBuffer::toString()
{
	long length = (long) wcslen(m_pBuffer) + 1;
	int bufferLength = 0;
	wchar_t *retString = this->createEmptyBuffer(length);

#ifdef _WIN32
	bufferLength = _snwprintf(retString, length, L"%ls", m_pBuffer);
#else
	bufferLength = swprintf(retString, length, L"%ls", m_pBuffer);
#endif
	if (bufferLength < 0) {
		delete [] retString;
		throw L"Cannot convert StringBuffer instance to string.";
	}
	return retString;
}


void StringBuffer::trim()
// Resize the buffer to match the length of the string
{
	long requiredLength = (long)wcslen(m_pBuffer) + 1;
	wchar_t* newBuffer = new wchar_t[requiredLength];
	
	for( long i=0; i < requiredLength; i++) {
		newBuffer[i] = this->m_pBuffer[i];
	}

	delete [] m_pBuffer;
	m_pBuffer = newBuffer;
	this->m_lCapacity = requiredLength;
}
