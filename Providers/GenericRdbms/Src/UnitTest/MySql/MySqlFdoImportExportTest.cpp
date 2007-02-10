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
#include "MySqlFdoImportExportTest.h"
#include "UnitTestUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION( MySqlFdoImportExportTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( MySqlFdoImportExportTest, "FdoImportExportTest");

void MySqlFdoImportExportTest::set_provider()
{
	UnitTestUtil::SetProvider( "MySql" );
}

void MySqlFdoImportExportTest::AddMapping( FdoXmlWriter* writer, FdoString* name, FdoInt32 providerVersion )
{
    _addMapping( writer, name, (providerVersion == 3) ? L"OSGeo.MySQL.3.3" :
        ((providerVersion == 2) ? L"OSGeo.MySQL.2.1" : L"OSGeo.MySQL.1.9"),L"http://fdomysql.osgeo.org/schemas");
}

FdoStringP MySqlFdoImportExportTest::OverrideBend( FdoString* inFile )
{
    FdoStringP outFile = UnitTestUtil::GetOutputFileName( inFile);

    _overrideBend( inFile, outFile, L"OSGeo.MySQL.3.3", L"http://fdomysql.osgeo.org/schemas" );

    return outFile;
}
