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
 */

#ifndef COMPARISONTESTS_H
#define COMPARISONTESTS_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ShpTests.h"

/// This is commented out because it requires a separate step to create the SDF files:
///    SDFConverter.exe Sheboygan\Trees.shp
///    SDFConverter.exe Sheboygan\Parcels.shp
/// and a providers.xml file that points to both the Shp and Sdf provider:
class ComparisonTests :
    public ShpTests
{
    CPPUNIT_TEST_SUITE (ComparisonTests);
//    CPPUNIT_TEST (compare1);
//    CPPUNIT_TEST (compare2);
    CPPUNIT_TEST_SUITE_END ();

public:
    ComparisonTests (void);
    virtual ~ComparisonTests (void);
	void setUp ();
	void tearDown ();

protected:
	void compare1 ();
	void compare2 ();
};

#endif // COMPARISONTESTS_H

