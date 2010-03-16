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

#include "Pch.h"
#include "PostGisFdoUpdateTest.h"
#include "UnitTestUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION( PostGisFdoUpdateTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( PostGisFdoUpdateTest, "FdoUpdateTest");

void PostGisFdoUpdateTest::set_provider()
{
	UnitTestUtil::SetProvider( "PostGIS" );
	m_hasAssoc = false;
	m_hasGeom = true;
}

FdoString* PostGisFdoUpdateTest::NoMetaSuffix()
{
    FdoString* noMetaSuffix = L"_no_meta";

    return noMetaSuffix;
}

FdoInt64 PostGisFdoUpdateTest::GetSrid( FdoInt64 srid ) 
{
    FdoInt64 outSrid;

    switch (srid)
    {
    case 81989:
        outSrid = 27700;   // British National Grid
        break;
    case 524288:
        outSrid = 4326;
        break;
    default:
        outSrid = srid;
    }

    return outSrid;
}