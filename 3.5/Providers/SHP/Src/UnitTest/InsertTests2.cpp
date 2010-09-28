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

#include "Pch.h"
#include "InsertTests2.h"
#include "ShpProvider.h"
#include "FdoCommonFile.h"
#include "ShpTests.h"

CPPUNIT_TEST_SUITE_REGISTRATION( InsertTests2 );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( InsertTests2, "InsertTest");

#ifdef _WIN32
static const wchar_t* LOCATION = L"..\\..\\TestData\\InsertTest";
#else
#include <unistd.h>
static const wchar_t* LOCATION = L"../../TestData/InsertTest";
#endif

InsertTests2::InsertTests2()
{
}

InsertTests2::~InsertTests2()
{
}

void InsertTests2::CreateConnection( Context& context, FdoBoolean recreateDb )
{
    FdoStringP dirName = LOCATION;
    dirName += context.mSuffix;

    if (FdoCommonFile::FileExists (dirName)) {
        std::vector<std::wstring> files;
        FdoCommonFile::GetAllFiles (dirName, files );
        for ( size_t i = 0; i < files.size(); i++ ) 
            FdoCommonFile::Delete( 
                dirName + 
#ifdef _WIN32
                L"\\"
#else
                L"/"
#endif
                + files[i].c_str() 
            );

        FdoCommonFile::RmDir (dirName);
    }

    FdoCommonFile::MkDir (dirName);

    context.connection = ShpTests::GetConnection ();
    context.connection->SetConnectionString (FdoStringP(L"DefaultFileLocation=") + dirName);
    CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == context.connection->Open ());
}

bool InsertTests2::SupportsTime()
{
    return false;
}

FdoDataType InsertTests2::GetAutoIncrementType()
{
    return FdoDataType_Int32;
}

