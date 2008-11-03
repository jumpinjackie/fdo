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

#include "stdafx.h"
#include "SchemaTest.h"
#include <ctime>
#include <cppunit/extensions/HelperMacros.h>
#include "SDF/SdfCommandType.h"
#include "SDF/ICreateSDFFile.h"

#define SDF_FILE_MG  L"../../TestData/HYDRANTSMULTIGEOM.sdf"

CPPUNIT_TEST_SUITE_REGISTRATION( SchemaTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SchemaTest, "SchemaTest");

SchemaTest::SchemaTest(void)
{
    
}

SchemaTest::~SchemaTest(void)
{
    
}

void SchemaTest::setUp ()
{

}

void SchemaTest::tearDown ()
{
}

void SchemaTest::OpenConnection(FdoIConnection* conn, const wchar_t* path , bool placeDQ)
{
#ifdef _WIN32
    wchar_t fullpath[1024];
    _wfullpath(fullpath, path, 1024);
#else
    char cpath[1024];
    char cfullpath[1024];
    wcstombs(cpath, path, 1024);
    realpath(cpath, cfullpath);
    wchar_t fullpath[1024];
    mbstowcs(fullpath, cfullpath, 1024);
#endif

    std::wstring connStr;
    if (placeDQ)
        connStr = std::wstring(L"File=\"") + std::wstring(fullpath) + std::wstring(L"\"");
    else
        connStr = std::wstring(L"File=") + std::wstring(fullpath);
    connStr += std::wstring(L";ReadOnly=TRUE");

    conn->SetConnectionString(connStr.c_str());
    conn->Open();
}

void SchemaTest::TestMultipleGeom()
{
    try
    {
        FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
        FdoPtr<FdoIConnection> conn = manager->CreateConnection (L"OSGeo.SDF");

        OpenConnection(conn, SDF_FILE_MG);
        
        FdoPtr<FdoIDescribeSchema>pDescSchemaCmd = (FdoIDescribeSchema*) conn->CreateCommand(FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> pfsc = pDescSchemaCmd->Execute();
        
        FdoIoFileStreamP schemaFileStream = FdoIoFileStream::Create(L"SDFTestDescribeSchema.xml", L"w+");
        pfsc->WriteXml(schemaFileStream);
        schemaFileStream = NULL;

        conn->Close();
    }
    catch (FdoException *ex )
    {
        printf("FDO error: %ls\n", ex->GetExceptionMessage());
        ex->Release();
        CPPUNIT_FAIL ("Failed to export schema!");
    }
}

