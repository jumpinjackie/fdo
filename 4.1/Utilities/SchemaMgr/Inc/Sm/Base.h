#ifndef FDOSMBASE_H
#define FDOSMBASE_H		1
/*
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

#ifdef _WIN32
#pragma once
#endif

// Common base class for all Schema Manager classes.
// Its main purpose is to centralize memory management.
class FdoSmBase
{
public:
	~FdoSmBase(void);

//TODO: Currently, we're using the C new, which returns 
//NULL instead of exception on error.
//We might switch to the C++ new, which throws an exception.
//If the switch is not possible then uncomment the following
//to force an exception to be thrown.
/*
	void * operator new (size_t size);
	void * operator new (size_t size, const char* filler, int line);
	void   operator delete (void*);
*/
// For memory debugging, when Purify license is busy.
#ifdef SCHEMA_DEBUG
    /// Writes out a report to "memrpt.txt" listing all the schema objects that
    /// were created and whether they have been deleted.
    /// This function can only be called once in a session.
	static void Report();
#endif

protected:
// unused constructor needed only to build Linux
    FdoSmBase() {}

    FdoSmBase(const wchar_t* parent, const wchar_t* name);
};

#endif


