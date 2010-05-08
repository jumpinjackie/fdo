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
#include "PostGisFdoImportExportTest.h"
#include "UnitTestUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION( PostGisFdoImportExportTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( PostGisFdoImportExportTest, "FdoImportExportTest");

void PostGisFdoImportExportTest::set_provider()
{
	UnitTestUtil::SetProvider( "PostGIS" );
}

void PostGisFdoImportExportTest::AddMapping( FdoXmlWriter* writer, FdoString* name, FdoInt32 providerVersion )
{
    _addMapping( writer, name, (providerVersion == 3) ? L"OSGeo.PostgreSQL.3.2" :
        ((providerVersion == 2) ? L"OSGeo.PostgreSQL.2.1" : L"OSGeo.PostgreSQL.1.9"),L"http://fdoPostgreSQL.osgeo.org/schemas");
}

FdoStringP PostGisFdoImportExportTest::OverrideBend( FdoString* inFile )
{
    FdoStringP outFile = UnitTestUtil::GetOutputFileName( inFile);

    _overrideBend( inFile, outFile, L"OSGeo.PostgreSQL.3.6", L"http://fdopostgresql.osgeo.org/schemas" );

    return outFile;
}
