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
#ifndef FDOPOSTGIS_POSTGISTESTCASE_H_INCLUDED
#define FDOPOSTGIS_POSTGISTESTCASE_H_INCLUDED

namespace fdo { namespace postgis { namespace test {

extern const wchar_t* providerName;

}}} // namespace fdo::postgis::test

/// Common base test case for PostGIS tests.
///
class PostGisTestCase : public CppUnit::TestCase
{
public:

    // Constructor.
    // Nothing is initialized here.
    PostGisTestCase();

    // Destructor.
    // Nothing is uninitialized here.
    virtual ~PostGisTestCase();

    // Named constructor initializes internal state of test case.
	virtual void setUp();

    // Named destructor uninitializes the test case.
	virtual void tearDown();

    // Factory method creating new instance of PostGIS connection.
    virtual FdoIConnection* GetConnection();

private:

    FdoPtr<FdoIConnection> mConnection;
};

#endif // FDOPOSTGIS_POSTGISTESTCASE_H_INCLUDED
