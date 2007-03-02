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
#include "Pch.h"
#include "PgGeometryTest.h"
// EWKB<->FGF
// NOTE: The PgGeometry stuff is built-in directly into UnitTest program
//       (see UnitTest project, Folder Misc), but not called from PostGisProvider.dll
//       library, because the PgGeomery operations are not publicly exported.
#include <PgGeometry.h>
//std
#include <string>
// boost
#include <boost/cstdint.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(PgGeometryTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PgGeometryTest, "PgGeometryTest");

// Namespace defining EWKB geometry parser
using namespace fdo::postgis::ewkb;

PgGeometryTest::PgGeometryTest()
{
}

PgGeometryTest::~PgGeometryTest()
{
}

void PgGeometryTest::setUp()
{
}

void PgGeometryTest::tearDown()
{
}

void PgGeometryTest::testPoint()
{
    // POINT(1.234 5.678)

    ewkb_t ewkb;
    std::string original("POINT (1.234 5.678)");
    std::string ewkbhex("01010000005839B4C876BEF33F83C0CAA145B61640");

    std::string output = BuildTestGeometry(ewkbhex);
    CPPUNIT_ASSERT_EQUAL(original, output);
}

void PgGeometryTest::testPointSRID()
{
    // SRID=32632;POINT(1.234 5.678)

    ewkb_t ewkb;
    std::string original("POINT (1.234 5.678)");
    std::string ewkbhex("0101000020787F00005839B4C876BEF33F83C0CAA145B61640");

    std::string output = BuildTestGeometry(ewkbhex);
    CPPUNIT_ASSERT_EQUAL(original, output);
}

void PgGeometryTest::testPointZ()
{
    // POINT(1.234 5.678 99)

    ewkb_t ewkb;
    std::string original("POINT XYZ (1.234 5.678 99)");
    std::string ewkbhex("01010000805839b4c876bef33f83c0caa145b616400000000000c05840");

    std::string output = BuildTestGeometry(ewkbhex);
    CPPUNIT_ASSERT_EQUAL(original, output);
}

void PgGeometryTest::testPointSRIDZ()
{
    // SRID=4326;POINT(1.234 5.678 99)

    ewkb_t ewkb;
    std::string original("POINT XYZ (1.234 5.678 99)");
    std::string ewkbhex("01010000a0e61000005839b4c876bef33f83c0caa145b616400000000000c05840");

    std::string output = BuildTestGeometry(ewkbhex);
    CPPUNIT_ASSERT_EQUAL(original, output);
}

void PgGeometryTest::testPointM()
{
    // POINTM(1.234 5.678 99)

    ewkb_t ewkb;
    std::string original("POINT XYM (1.234 5.678 99)");
    std::string ewkbhex("01010000405839B4C876BEF33F83C0CAA145B616400000000000C05840");

    std::string output = BuildTestGeometry(ewkbhex);
    CPPUNIT_ASSERT_EQUAL(original, output);
}

void PgGeometryTest::testPointSRIDM()
{
    // SRID=32632;POINTM(1.234 5.678 99)

    ewkb_t ewkb;
    std::string original("POINT XYM (1.234 5.678 99)");
    std::string ewkbhex("0101000060787f00005839b4c876bef33f83c0caa145b616400000000000c05840");

    std::string output = BuildTestGeometry(ewkbhex);
    CPPUNIT_ASSERT_EQUAL(original, output);
}

void PgGeometryTest::testPointZM()
{
    // POINT(1.234 5.678 15 79)

    ewkb_t ewkb;
    std::string original("POINT XYZM (1.234 5.678 15 79)");
    std::string ewkbhex("01010000C05839B4C876BEF33F83C0CAA145B616400000000000002E400000000000C05340");

    std::string output = BuildTestGeometry(ewkbhex);
    CPPUNIT_ASSERT_EQUAL(original, output);
}

void PgGeometryTest::testPointSRIDZM()
{
    // SRID=4326;POINT(1.234 5.678 15 79)
    
    std::string original("POINT XYZM (1.234 5.678 15 79)");
    std::string ewkbhex("01010000e0e61000005839b4c876bef33f83c0caa145b616400000000000002e400000000000c05340");

    std::string output = BuildTestGeometry(ewkbhex);
    CPPUNIT_ASSERT_EQUAL(original, output);
}

void PgGeometryTest::testLineString()
{
    // LINESTRING(1.123 1.456, 2.123 2.456, 3.123 3.456)

    std::string original("LINESTRING (1.123 1.456, 2.123 2.456, 3.123 3.456)");
    std::string ewkbhex("0102000000030000002b8716d9cef7f13fb29defa7c64bf73f96438b6ce7fb0040d9cef753e3a5034096438b6ce7fb0840d9cef753e3a50b40");

    std::string output = BuildTestGeometry(ewkbhex);
    CPPUNIT_ASSERT_EQUAL(original, output);
}

void PgGeometryTest::testLineStringSRID()
{
    // SRID=32632;LINESTRING(1.123 1.456, 2.123 2.456, 3.123 3.456)

    std::string original("LINESTRING (1.123 1.456, 2.123 2.456, 3.123 3.456)");
    std::string ewkbhex("0102000020787f0000030000002b8716d9cef7f13fb29defa7c64bf73f96438b6ce7fb0040d9cef753e3a5034096438b6ce7fb0840d9cef753e3a50b40");

    std::string output = BuildTestGeometry(ewkbhex);
    CPPUNIT_ASSERT_EQUAL(original, output);
}

void PgGeometryTest::testLineStringZ()
{
    // LINESTRING(1.123 1.456 100, 2.123 2.456 105, 3.123 3.456 110)

    std::string original("LINESTRING XYZ (1.123 1.456 100, 2.123 2.456 105, 3.123 3.456 110)");
    std::string ewkbhex("0102000080030000002b8716d9cef7f13fb29defa7c64bf73f000000000000594096438b6ce7fb0040d9cef753e3a503400000000000405a4096438b6ce7fb0840d9cef753e3a50b400000000000805b40");

    std::string output = BuildTestGeometry(ewkbhex);
    CPPUNIT_ASSERT_EQUAL(original, output);
}

void PgGeometryTest::testLineStringSRIDZ()
{
    // SRID=32632;LINESTRING(1.123 1.456 100, 2.123 2.456 105, 3.123 3.456 110)

    std::string original("LINESTRING XYZ (1.123 1.456 100, 2.123 2.456 105, 3.123 3.456 110)");
    std::string ewkbhex("01020000a0787f0000030000002b8716d9cef7f13fb29defa7c64bf73f000000000000594096438b6ce7fb0040d9cef753e3a503400000000000405a4096438b6ce7fb0840d9cef753e3a50b400000000000805b40");

    std::string output = BuildTestGeometry(ewkbhex);
    CPPUNIT_ASSERT_EQUAL(original, output);
}

void PgGeometryTest::testLineStringM()
{
    // LINESTRINGM(2 3 4,3 4 5)

    std::string original("LINESTRING XYM (2 3 4, 3 4 5)");
    std::string ewkbhex("010200004002000000000000000000004000000000000008400000000000001040000000000000084000000000000010400000000000001440");

    std::string output = BuildTestGeometry(ewkbhex);
    CPPUNIT_ASSERT_EQUAL(original, output);
}

void PgGeometryTest::testLineStringSRIDM()
{
    // SRID=32632;LINESTRINGM(2 3 4,3 4 5)

    std::string original("LINESTRING XYM (2 3 4, 3 4 5)");
    std::string ewkbhex("0102000060787f000002000000000000000000004000000000000008400000000000001040000000000000084000000000000010400000000000001440");

    std::string output = BuildTestGeometry(ewkbhex);
    CPPUNIT_ASSERT_EQUAL(original, output);
}

void PgGeometryTest::testLineStringZM()
{
    // LINESTRING(1.123 1.456 100 1, 2.123 2.456 105 1, 3.123 3.456 110 1)

    std::string original("LINESTRING XYZM (1.123 1.456 100 1, 2.123 2.456 105 1, 3.123 3.456 110 1)");
    std::string ewkbhex("01020000c0030000002b8716d9cef7f13fb29defa7c64bf73f0000000000005940000000000000f03f96438b6ce7fb0040d9cef753e3a503400000000000405a40000000000000f03f96438b6ce7fb0840d9cef753e3a50b400000000000805b40000000000000f03f");

    std::string output = BuildTestGeometry(ewkbhex);
    CPPUNIT_ASSERT_EQUAL(original, output);
}

void PgGeometryTest::testLineStringSRIDZM()
{
    // SRID=32632;LINESTRING(1.123 1.456 100 1, 2.123 2.456 105 1, 3.123 3.456 110 1)

    std::string original("LINESTRING XYZM (1.123 1.456 100 1, 2.123 2.456 105 1, 3.123 3.456 110 1)");
    std::string ewkbhex("01020000e0787f0000030000002b8716d9cef7f13fb29defa7c64bf73f0000000000005940000000000000f03f96438b6ce7fb0040d9cef753e3a503400000000000405a40000000000000f03f96438b6ce7fb0840d9cef753e3a50b400000000000805b40000000000000f03f");

    std::string output = BuildTestGeometry(ewkbhex);
    CPPUNIT_ASSERT_EQUAL(original, output);
}

void PgGeometryTest::testPolygon()
{
    // POLYGON((0 0,4 0,4 4,0 4,0 0),(1 1,2 1,2 2,1 2,1 1))

    std::string original("POLYGON ((0 0, 4 0, 4 4, 0 4, 0 0), (1 1, 2 1, 2 2, 1 2, 1 1))");
    std::string ewkbhex("01030000000200000005000000000000000000000000000000000000000000000000001040000000000000000000000000000010400000000000001040000000000000000000000000000010400000000000000000000000000000000005000000000000000000f03f000000000000f03f0000000000000040000000000000f03f00000000000000400000000000000040000000000000f03f0000000000000040000000000000f03f000000000000f03f");

    std::string output = BuildTestGeometry(ewkbhex);
    CPPUNIT_ASSERT_EQUAL(original, output);
}

void PgGeometryTest::testPolygonSRID()
{
    // SRID=32632;POLYGON((0 0,4 0,4 4,0 4,0 0),(1 1,2 1,2 2,1 2,1 1))

    std::string original("POLYGON ((0 0, 4 0, 4 4, 0 4, 0 0), (1 1, 2 1, 2 2, 1 2, 1 1))");
    std::string ewkbhex("0103000020787f00000200000005000000000000000000000000000000000000000000000000001040000000000000000000000000000010400000000000001040000000000000000000000000000010400000000000000000000000000000000005000000000000000000f03f000000000000f03f0000000000000040000000000000f03f00000000000000400000000000000040000000000000f03f0000000000000040000000000000f03f000000000000f03f");

    std::string output = BuildTestGeometry(ewkbhex);
    CPPUNIT_ASSERT_EQUAL(original, output);
}

void PgGeometryTest::testPolygonZ()
{
    // POLYGON((0 0 0,4 0 0,4 4 0,0 4 0,0 0 0),(1 1 0,2 1 0,2 2 0,1 2 0,1 1 0))

    std::string original("POLYGON XYZ ((0 0 0, 4 0 0, 4 4 0, 0 4 0, 0 0 0), (1 1 0, 2 1 0, 2 2 0, 1 2 0, 1 1 0))");
    std::string ewkbhex("0103000080020000000500000000000000000000000000000000000000000000000000000000000000000010400000000000000000000000000000000000000000000010400000000000001040000000000000000000000000000000000000000000001040000000000000000000000000000000000000000000000000000000000000000005000000000000000000f03f000000000000f03f00000000000000000000000000000040000000000000f03f0000000000000000000000000000004000000000000000400000000000000000000000000000f03f00000000000000400000000000000000000000000000f03f000000000000f03f0000000000000000");

    std::string output = BuildTestGeometry(ewkbhex);
    CPPUNIT_ASSERT_EQUAL(original, output);
}

void PgGeometryTest::testPolygonSRIDZ()
{
    // SRID=32632;POLYGON((0 0 0,4 0 0,4 4 0,0 4 0,0 0 0),(1 1 0,2 1 0,2 2 0,1 2 0,1 1 0))

    std::string original("POLYGON XYZ ((0 0 0, 4 0 0, 4 4 0, 0 4 0, 0 0 0), (1 1 0, 2 1 0, 2 2 0, 1 2 0, 1 1 0))");
    std::string ewkbhex("01030000a0787f0000020000000500000000000000000000000000000000000000000000000000000000000000000010400000000000000000000000000000000000000000000010400000000000001040000000000000000000000000000000000000000000001040000000000000000000000000000000000000000000000000000000000000000005000000000000000000f03f000000000000f03f00000000000000000000000000000040000000000000f03f0000000000000000000000000000004000000000000000400000000000000000000000000000f03f00000000000000400000000000000000000000000000f03f000000000000f03f0000000000000000");

    std::string output = BuildTestGeometry(ewkbhex);
    CPPUNIT_ASSERT_EQUAL(original, output);
}

void PgGeometryTest::testPolygonM()
{
    // POLYGONM((0 0 7,4 0 7,4 4 7,0 4 7,0 0 7),(1 1 7,2 1 7,2 2 7,1 2 7,1 1 7))

    std::string original("POLYGON XYM ((0 0 7, 4 0 7, 4 4 7, 0 4 7, 0 0 7), (1 1 7, 2 1 7, 2 2 7, 1 2 7, 1 1 7))");
    std::string ewkbhex("01030000400200000005000000000000000000000000000000000000000000000000001c40000000000000104000000000000000000000000000001c40000000000000104000000000000010400000000000001c40000000000000000000000000000010400000000000001c40000000000000000000000000000000000000000000001c4005000000000000000000f03f000000000000f03f0000000000001c400000000000000040000000000000f03f0000000000001c40000000000000004000000000000000400000000000001c40000000000000f03f00000000000000400000000000001c40000000000000f03f000000000000f03f0000000000001c40");

    std::string output = BuildTestGeometry(ewkbhex);
    CPPUNIT_ASSERT_EQUAL(original, output);
}

void PgGeometryTest::testPolygonSRIDM()
{
    // SRID=4326;POLYGONM((0 0 7,4 0 7,4 4 7,0 4 7,0 0 7),(1 1 7,2 1 7,2 2 7,1 2 7,1 1 7))

    std::string original("POLYGON XYM ((0 0 7, 4 0 7, 4 4 7, 0 4 7, 0 0 7), (1 1 7, 2 1 7, 2 2 7, 1 2 7, 1 1 7))");
    std::string ewkbhex("0103000060e61000000200000005000000000000000000000000000000000000000000000000001c40000000000000104000000000000000000000000000001c40000000000000104000000000000010400000000000001c40000000000000000000000000000010400000000000001c40000000000000000000000000000000000000000000001c4005000000000000000000f03f000000000000f03f0000000000001c400000000000000040000000000000f03f0000000000001c40000000000000004000000000000000400000000000001c40000000000000f03f00000000000000400000000000001c40000000000000f03f000000000000f03f0000000000001c40");

    std::string output = BuildTestGeometry(ewkbhex);
    CPPUNIT_ASSERT_EQUAL(original, output);
}

void PgGeometryTest::testPolygonZM()
{
    // POLYGON((0 0 0 99,4 0 0 99,4 4 0 99,0 4 0 99,0 0 0 99),(1 1 0 99,2 1 0 99,2 2 0 99,1 2 0 99,1 1 0 99))

    std::string original("POLYGON XYZM ((0 0 0 99, 4 0 0 99, 4 4 0 99, 0 4 0 99, 0 0 0 99), (1 1 0 99, 2 1 0 99, 2 2 0 99, 1 2 0 99, 1 1 0 99))");
    std::string ewkbhex("01030000c002000000050000000000000000000000000000000000000000000000000000000000000000c058400000000000001040000000000000000000000000000000000000000000c058400000000000001040000000000000104000000000000000000000000000c058400000000000000000000000000000104000000000000000000000000000c058400000000000000000000000000000000000000000000000000000000000c0584005000000000000000000f03f000000000000f03f00000000000000000000000000c058400000000000000040000000000000f03f00000000000000000000000000c058400000000000000040000000000000004000000000000000000000000000c05840000000000000f03f000000000000004000000000000000000000000000c05840000000000000f03f000000000000f03f00000000000000000000000000c05840");

    std::string output = BuildTestGeometry(ewkbhex);
    CPPUNIT_ASSERT_EQUAL(original, output);
}

void PgGeometryTest::testPolygonSRIDZM()
{
    // SRID=4326;POLYGON((0 0 0 99,4 0 0 99,4 4 0 99,0 4 0 99,0 0 0 99),(1 1 0 99,2 1 0 99,2 2 0 99,1 2 0 99,1 1 0 99))

    std::string original("POLYGON XYZM ((0 0 0 99, 4 0 0 99, 4 4 0 99, 0 4 0 99, 0 0 0 99), (1 1 0 99, 2 1 0 99, 2 2 0 99, 1 2 0 99, 1 1 0 99))");
    std::string ewkbhex("01030000e0e610000002000000050000000000000000000000000000000000000000000000000000000000000000c058400000000000001040000000000000000000000000000000000000000000c058400000000000001040000000000000104000000000000000000000000000c058400000000000000000000000000000104000000000000000000000000000c058400000000000000000000000000000000000000000000000000000000000c0584005000000000000000000f03f000000000000f03f00000000000000000000000000c058400000000000000040000000000000f03f00000000000000000000000000c058400000000000000040000000000000004000000000000000000000000000c05840000000000000f03f000000000000004000000000000000000000000000c05840000000000000f03f000000000000f03f00000000000000000000000000c05840");

    std::string output = BuildTestGeometry(ewkbhex);
    CPPUNIT_ASSERT_EQUAL(original, output);
}

///////////////////////////////////////////////////////////////////////////////
// Private operations
///////////////////////////////////////////////////////////////////////////////

std::string PgGeometryTest::BuildTestGeometry(std::string const& ewkbhex)
{
    ewkb_t ewkb;

    // Convert EWKB hex-string to raw binary data
    hex_to_bytes(ewkbhex, ewkb);
    CPPUNIT_ASSERT(!ewkb.empty());

    // Build FDO geometry
    FdoPtr<FdoIGeometry> g = CreateGeometryFromExtendedWkb(ewkb);
    CPPUNIT_ASSERT(NULL != g);

    // Get WKT form of FDO geometry
    FdoStringP tmp(g->GetText());
    CPPUNIT_ASSERT(tmp.GetLength() > 0);
 
    // Convert to narrow string
    std::string output(static_cast<char const*>(tmp));
    return output;
}
