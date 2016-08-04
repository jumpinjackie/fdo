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
#include "CapabilitiesTest.h"
#ifdef _DEBUG
//#define DEBUG_DETAIL	1
#include "TestProcessor.h"
#endif

CPPUNIT_TEST_SUITE_REGISTRATION (CapabilitiesTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (CapabilitiesTest, "CapabilitiesTest");

CapabilitiesTest::CapabilitiesTest(void)
{
}

CapabilitiesTest::~CapabilitiesTest(void)
{
}

void CapabilitiesTest::setUp()
{
    // nothing to do at setup time... yet
}

void CapabilitiesTest::testFunctionDefinition()
{
    FdoArgumentDefinition*  paArgs[4];
    paArgs[0] = FdoArgumentDefinition::Create(L"Arg1", L"Argument 1 (Boolean)", FdoDataType_Boolean);
    paArgs[1] = FdoArgumentDefinition::Create(L"Arg2", NULL, FdoDataType_Byte);
    paArgs[2] = FdoArgumentDefinition::Create(L"Arg3", L"Argument 3 (BLOB)", FdoDataType_BLOB);
    paArgs[3] = FdoArgumentDefinition::Create(L"Arg 4", L"Argument 4 (Int64)", FdoDataType_Int64);

    FDO_CPPUNIT_ASSERT(wcscmp(paArgs[0]->GetName(), L"Arg1") == 0);
    FDO_CPPUNIT_ASSERT(wcscmp(paArgs[0]->GetDescription(), L"Argument 1 (Boolean)") == 0);
    FDO_CPPUNIT_ASSERT(paArgs[0]->GetDataType() == FdoDataType_Boolean);

    FDO_CPPUNIT_ASSERT(wcscmp(paArgs[1]->GetName(), L"Arg2") == 0);
    FDO_CPPUNIT_ASSERT(paArgs[1]->GetDescription() == NULL);
    FDO_CPPUNIT_ASSERT(paArgs[1]->GetDataType() == FdoDataType_Byte);

    FDO_CPPUNIT_ASSERT(wcscmp(paArgs[2]->GetName(), L"Arg3") == 0);
    FDO_CPPUNIT_ASSERT(wcscmp(paArgs[2]->GetDescription(), L"Argument 3 (BLOB)") == 0);
    FDO_CPPUNIT_ASSERT(paArgs[2]->GetDataType() == FdoDataType_BLOB);

    FDO_CPPUNIT_ASSERT(wcscmp(paArgs[3]->GetName(), L"Arg 4") == 0);
    FDO_CPPUNIT_ASSERT(wcscmp(paArgs[3]->GetDescription(), L"Argument 4 (Int64)") == 0);
    FDO_CPPUNIT_ASSERT(paArgs[3]->GetDataType() == FdoDataType_Int64);

    FdoArgumentDefinitionCollection*    pBaseCollection = FdoArgumentDefinitionCollection::Create(paArgs, 4);

    FdoFunctionDefinition*   pFuncDef = FdoFunctionDefinition::Create(
        L"Function", 
        L"Test of FdoFunctionDefinition", 
        FdoDataType_Double, 
        pBaseCollection);
    pBaseCollection->Release();

    FdoReadOnlyArgumentDefinitionCollection* paArgsRet = pFuncDef->GetArguments();

    FDO_CPPUNIT_ASSERT(paArgsRet->GetCount() == (sizeof(paArgs) / sizeof(paArgs[0])));
    for (int i = 0; i < paArgsRet->GetCount(); i++)
    {
        FdoArgumentDefinition*  pArg = paArgsRet->GetItem(i);
        FDO_CPPUNIT_ASSERT(pArg == paArgs[i]);
        pArg->Release();
    }
	FDO_CPPUNIT_ASSERT(paArgsRet->Contains(paArgs[0]));
	FdoArgumentDefinition*	pDef = paArgsRet->GetItem(L"Arg2");
	FDO_CPPUNIT_ASSERT(pDef != NULL);
	pDef->Release();
    paArgsRet->Release();

    paArgs[0]->Release();
    paArgs[1]->Release();
    paArgs[2]->Release();
    paArgs[3]->Release();

    FDO_CPPUNIT_ASSERT(pFuncDef->GetReturnType() == FdoDataType_Double);
    FDO_CPPUNIT_ASSERT(wcscmp(pFuncDef->GetName(), L"Function") == 0);
    FDO_CPPUNIT_ASSERT(wcscmp(pFuncDef->GetDescription(), L"Test of FdoFunctionDefinition") == 0);

    pFuncDef->Release();
}


