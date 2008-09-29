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

#if !defined(AFX_STRINGBUFFER_H__6F001295_BC04_4F33_A1F1_8C376B1A4076__INCLUDED_)
#define AFX_STRINGBUFFER_H__6F001295_BC04_4F33_A1F1_8C376B1A4076__INCLUDED_

#if _WIN32
#pragma warning( disable : 4290 )
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <wchar.h>
#include <string.h>
#define STRINGBUFFER_INIT_CAPACITY 32


class StringBuffer  
// This class is modelled after the Java StringBuffer, and provides a convenient way 
// of building a large string of an unknown size.
{
protected:
	wchar_t* m_pBuffer;
	unsigned long m_lCapacity;

	// Create a new buffer at a given size, initialized with the null character
	wchar_t* createEmptyBuffer( unsigned long size );


public:
	// Constructors
	StringBuffer();
	StringBuffer( const wchar_t* str );
	StringBuffer( unsigned long initCapacity );

	// Desctructor
	virtual ~StringBuffer();

	// Retrieve the length and capacity of the buffer
	unsigned long getCapacity();
	unsigned long getLength();

	// Ensure that the capacity is at least the given size
	void ensureCapacity( unsigned long size );	

	// Resize the buffer to match the actual length of the string
	void trim();
	
	// Insert and append strings into the buffer
	// The buffer is extended automatically if the resulting string
	// exceeds the capacity
	void insert(unsigned long offset, const wchar_t* str );	
	void append(const wchar_t* str);

	// Retreive a substring of the current buffer.
	// If end < start, a NULL is returned.
	// If start > the string length, a NULL is returned.
	wchar_t* substring(unsigned long start, unsigned long end);

	// Returns a copy of the string value of the buffer
	const wchar_t* toString();
};

#endif // !defined(AFX_STRINGBUFFER_H__6F001295_BC04_4F33_A1F1_8C376B1A4076__INCLUDED_)
