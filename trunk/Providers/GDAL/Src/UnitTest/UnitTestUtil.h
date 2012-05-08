//
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
//

#ifndef CPP_FDOGRFP_UNIT_TESTUTIL_H
#define CPP_FDOGRFP_UNIT_TESTUTIL_H

#ifdef _WIN32
#pragma once
#endif

// Various utility functions to help with unit testing.
class UnitTestUtil
{
public:
	UnitTestUtil(void);
   ~UnitTestUtil(void);

public:
	// Compare an output file against its master (expected) copy.
	// Fail if they are different.
	static void CheckOutput( const char* masterFileName, const char* outFileName );

	// Compare two files.
	// Returns:
	//		0 - they are the same
	//	   -1 - they are different.
	static FdoInt32 CompareFiles( const char* file1Name, const char* file2Name );
};

#endif // CPP_FDOGRFP_UNIT_TESTUTIL_H
