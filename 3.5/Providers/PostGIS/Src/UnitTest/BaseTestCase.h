//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#ifndef FDOPOSTGIS_BASETESTCASE_H_INCLUDED
#define FDOPOSTGIS_BASETESTCASE_H_INCLUDED

//
// Forward declarations
//
class FdoException;


namespace fdo { namespace postgis { namespace test {

// Common base test case for PostGIS tests.
//
class BaseTestCase : public CppUnit::TestCase
{
public:

    // Constructor.
    // Nothing is initialized here.
    BaseTestCase();

    // Destructor.
    // Nothing is uninitialized here.
    virtual ~BaseTestCase();

    // Named constructor initializes connection instance.
    virtual void setUp();

    // Named destructor uninitializes connection instance.
	virtual void tearDown();

    // Factory method creating new instance of PostGIS connection.
    FdoIConnection* GetConnection();

    // Handles failure with FDO exception
    static void fail(FdoException* e);

    // Handles failure with standard exception
    static void fail(std::exception& e);

    // Handles failure with error message
    static void fail(char const* e);

    // Process unknown and unhandled exception
    static void unknown();

private:

    FdoPtr<FdoIConnection> mConn;

};

}}} // namespace fdo::postgis::test

#endif // FDOPOSTGIS_BASETESTCASE_H_INCLUDED
