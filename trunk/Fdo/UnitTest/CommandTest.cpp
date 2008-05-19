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
#include "CommandTest.h"
#ifdef _DEBUG
//#define DEBUG_DETAIL	1
#include "TestProcessor.h"
#endif

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE	0
#endif

CPPUNIT_TEST_SUITE_REGISTRATION (CommandTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (CommandTest, "CommandTest");

CommandTest::CommandTest(void)
{
}

CommandTest::~CommandTest(void)
{
}

void CommandTest::setUp()
{
    // nothing to do at setup time... yet
}

void CommandTest::testException()
{
    bool bException = false;
    try
    {
        FdoPtr<FdoIdentifier> pIdent;
        pIdent->GetName();
    }
    catch (FdoException *exp)
    {
        bException = true;
        exp->Release();
    }
    FDO_CPPUNIT_ASSERT(bException == true);
}

void CommandTest::testBatchParameterValueCollection()
{
    FdoParameterValueCollection*    pColl;

    // create data
    FdoBatchParameterValueCollection*   pBatch = FdoBatchParameterValueCollection::Create();
    FdoParameterValueCollection*    pColl_1 = FdoParameterValueCollection::Create();
    FdoParameterValueCollection*    pColl_2 = FdoParameterValueCollection::Create();
    FdoParameterValueCollection*    pColl_3 = FdoParameterValueCollection::Create();

    // add to collection
    pBatch->Add(pColl_1);
    pBatch->Add(pColl_2);
    pBatch->Insert(1, pColl_3);

    // check collection is consistent
    pColl = pBatch->GetItem(0);
    FDO_CPPUNIT_ASSERT(pColl == pColl_1);
    pColl->Release();

    pColl = pBatch->GetItem(1);
    FDO_CPPUNIT_ASSERT(pColl == pColl_3);
    pColl->Release();
    
    pColl = pBatch->GetItem(2);
    FDO_CPPUNIT_ASSERT(pColl == pColl_2);
    pColl->Release();

    FDO_CPPUNIT_ASSERT(pBatch->GetCount() == 3);

    FDO_CPPUNIT_ASSERT(pBatch->Contains(pColl_1) == TRUE);
    FDO_CPPUNIT_ASSERT(pBatch->Contains(pColl_2) == TRUE);
    FDO_CPPUNIT_ASSERT(pBatch->Contains(pColl_3) == TRUE);

    FDO_CPPUNIT_ASSERT(pBatch->IndexOf(pColl_1) == 0);
    FDO_CPPUNIT_ASSERT(pBatch->IndexOf(pColl_2) == 2);
    FDO_CPPUNIT_ASSERT(pBatch->IndexOf(pColl_3) == 1);

    // remove an item
    pBatch->Remove(pColl_3);

    // check collection is consistent
    FDO_CPPUNIT_ASSERT(pBatch->GetCount() == 2);

    pColl = pBatch->GetItem(0);
    FDO_CPPUNIT_ASSERT(pColl == pColl_1);
    pColl->Release();

    pColl = pBatch->GetItem(1);
    FDO_CPPUNIT_ASSERT(pColl == pColl_2);
    pColl->Release();

    FDO_CPPUNIT_ASSERT(pBatch->Contains(pColl_1) == TRUE);
    FDO_CPPUNIT_ASSERT(pBatch->Contains(pColl_2) == TRUE);
    FDO_CPPUNIT_ASSERT(pBatch->Contains(pColl_3) == FALSE);

    FDO_CPPUNIT_ASSERT(pBatch->IndexOf(pColl_1) == 0);
    FDO_CPPUNIT_ASSERT(pBatch->IndexOf(pColl_2) == 1);
    FDO_CPPUNIT_ASSERT(pBatch->IndexOf(pColl_3) == -1);

    // remove another item
    pBatch->RemoveAt(1);

    // check collection is consistent
    FDO_CPPUNIT_ASSERT(pBatch->GetCount() == 1);

    pColl = pBatch->GetItem(0);
    FDO_CPPUNIT_ASSERT(pColl == pColl_1);
    pColl->Release();

    FDO_CPPUNIT_ASSERT(pBatch->Contains(pColl_1) == TRUE);
    FDO_CPPUNIT_ASSERT(pBatch->Contains(pColl_2) == FALSE);
    FDO_CPPUNIT_ASSERT(pBatch->Contains(pColl_3) == FALSE);

    FDO_CPPUNIT_ASSERT(pBatch->IndexOf(pColl_1) == 0);
    FDO_CPPUNIT_ASSERT(pBatch->IndexOf(pColl_2) == -1);
    FDO_CPPUNIT_ASSERT(pBatch->IndexOf(pColl_3) == -1);

    // release all objects
    pBatch->Release();
    pColl_1->Release();
    pColl_2->Release();
    pColl_3->Release();
}

void CommandTest::testIdentifierCollection()
{
    FdoIdentifier*      pIdent;

    // create data
    FdoIdentifierCollection*    pIdentColl = FdoIdentifierCollection::Create();
    FdoIdentifier*              pIdent1 = FdoIdentifier::Create(L"Ident1");
    FdoIdentifier*              pIdent2 = FdoIdentifier::Create();
    pIdent2->SetText(L"Ident2.set.later");
    FdoIdentifier*              pIdent3 = FdoIdentifier::Create(L"Ident2");
    FdoIdentifier*              pIdent4 = FdoIdentifier::Create(L"Ident3");

    // add identifiers to collection
    pIdentColl->Add(pIdent2);
    pIdentColl->Insert(0, pIdent1);
    pIdentColl->Add(pIdent4);
    pIdentColl->Insert(2, pIdent3);

    // check collection is consistent
    FDO_CPPUNIT_ASSERT(pIdentColl->GetCount() == 4);

    pIdent = pIdentColl->GetItem(0);
    FDO_CPPUNIT_ASSERT(pIdent == pIdent1);
    pIdent->Release();
    pIdent = pIdentColl->GetItem(1);
    FDO_CPPUNIT_ASSERT(pIdent == pIdent2);
    pIdent->Release();
    pIdent = pIdentColl->GetItem(2);
    FDO_CPPUNIT_ASSERT(pIdent == pIdent3);
    pIdent->Release();
    pIdent = pIdentColl->GetItem(3);
    FDO_CPPUNIT_ASSERT(pIdent == pIdent4);
    pIdent->Release();

    FDO_CPPUNIT_ASSERT(pIdentColl->Contains(pIdent1) == TRUE);
    FDO_CPPUNIT_ASSERT(pIdentColl->Contains(pIdent2) == TRUE);
    FDO_CPPUNIT_ASSERT(pIdentColl->Contains(pIdent3) == TRUE);
    FDO_CPPUNIT_ASSERT(pIdentColl->Contains(pIdent4) == TRUE);

    FDO_CPPUNIT_ASSERT(pIdentColl->IndexOf(pIdent1) == 0);
    FDO_CPPUNIT_ASSERT(pIdentColl->IndexOf(pIdent2) == 1);
    FDO_CPPUNIT_ASSERT(pIdentColl->IndexOf(pIdent3) == 2);
    FDO_CPPUNIT_ASSERT(pIdentColl->IndexOf(pIdent4) == 3);

    pIdent = pIdentColl->GetItem(L"Ident3");
    FDO_CPPUNIT_ASSERT(pIdent == pIdent4);
    pIdent->Release();
    FDO_CPPUNIT_ASSERT(pIdentColl->FindItem(L"Bogus") == NULL);

    // remove first and last items
    pIdentColl->RemoveAt(0);
    pIdentColl->RemoveAt(2);

    // check collection is consistent
    FDO_CPPUNIT_ASSERT(pIdentColl->GetCount() == 2);

    pIdent = pIdentColl->GetItem(0);
    FDO_CPPUNIT_ASSERT(pIdent == pIdent2);
    pIdent->Release();
    pIdent = pIdentColl->GetItem(1);
    FDO_CPPUNIT_ASSERT(pIdent == pIdent3);
    pIdent->Release();

    FDO_CPPUNIT_ASSERT(pIdentColl->Contains(pIdent1) == FALSE);
    FDO_CPPUNIT_ASSERT(pIdentColl->Contains(pIdent2) == TRUE);
    FDO_CPPUNIT_ASSERT(pIdentColl->Contains(pIdent3) == TRUE);
    FDO_CPPUNIT_ASSERT(pIdentColl->Contains(pIdent4) == FALSE);

    FDO_CPPUNIT_ASSERT(pIdentColl->IndexOf(pIdent1) == -1);
    FDO_CPPUNIT_ASSERT(pIdentColl->IndexOf(pIdent2) == 0);
    FDO_CPPUNIT_ASSERT(pIdentColl->IndexOf(pIdent3) == 1);
    FDO_CPPUNIT_ASSERT(pIdentColl->IndexOf(pIdent4) == -1);

    // attempt to add an identifier with a duplicate text value:
    FdoPtr<FdoIdentifier> pIdent2Duplicate = FdoIdentifier::Create(L"Ident2.set.later");
    try
    {
        pIdentColl->Add(pIdent2Duplicate);
        CPPUNIT_FAIL("Expected error due to duplicate entry in FdoIdentifierCollection");
    }
    catch (FdoException *e)
    {
        e->Release();  // we expect an exception
    }

    // release memory
    pIdentColl->Release();
    pIdent1->Release();
    pIdent2->Release();
    pIdent3->Release();
    pIdent4->Release();
}

void CommandTest::testParameterValueCollection()
{
    FdoParameterValue*  pVal;

    // create some data
    FdoParameterValueCollection* pColl = FdoParameterValueCollection::Create();
    FdoParameterValue*      pVal1 = FdoParameterValue::Create(L"Name1");
    FdoParameterValue*      pVal2 = FdoParameterValue::Create();
    FdoDoubleValue*         pLitVal = FdoDoubleValue::Create(3.14);
    FdoParameterValue*      pVal3 = FdoParameterValue::Create(L"Name3", pLitVal);

    // Fill collection
    pColl->Insert(0, pVal1);
    pColl->Insert(1, pVal2);
    pColl->Insert(2, pVal3);

    // check collection
    FDO_CPPUNIT_ASSERT(pColl->GetCount() == 3);

    FDO_CPPUNIT_ASSERT(pColl->Contains(pVal1));
    FDO_CPPUNIT_ASSERT(pColl->Contains(pVal2));
    FDO_CPPUNIT_ASSERT(pColl->Contains(pVal3));

    FDO_CPPUNIT_ASSERT(pColl->IndexOf(pVal1) == 0);
    FDO_CPPUNIT_ASSERT(pColl->IndexOf(pVal2) == 1);
    FDO_CPPUNIT_ASSERT(pColl->IndexOf(pVal3) == 2);

    pVal = pColl->GetItem(0);
    FDO_CPPUNIT_ASSERT(pVal == pVal1);
    pVal->Release();
    pVal = pColl->GetItem(1);
    FDO_CPPUNIT_ASSERT(pVal == pVal2);
    pVal->Release();
    pVal = pColl->GetItem(2);
    FDO_CPPUNIT_ASSERT(pVal == pVal3);
    pVal->Release();

    pVal = pColl->GetItem(L"Name3");
    FDO_CPPUNIT_ASSERT(pVal == pVal3);
    pVal->Release();
    FDO_CPPUNIT_ASSERT(pColl->FindItem(L"Bogus") == NULL );

    pColl->Release();
    pLitVal->Release();
    pVal1->Release();
    pVal2->Release();
    pVal3->Release();
}

void CommandTest::testPropertyValueCollection()
{
    FdoPropertyValue*   pVal;

    // create some data
    FdoPropertyValueCollection* pColl = FdoPropertyValueCollection::Create();

    FdoIdentifier*          pIdent = FdoIdentifier::Create(L"Ident.For.Test");
    FdoDoubleValue*         pDblVal = FdoDoubleValue::Create(3.14);
    FdoByteValue*           pByteVal = FdoByteValue::Create(0x12);
    FdoInt16Value*          pInt16Val = FdoInt16Value::Create(123);

    FdoPropertyValue*       pVal1 = FdoPropertyValue::Create(L"Name1", pByteVal);
    FdoPropertyValue*       pVal2 = FdoPropertyValue::Create(pIdent, pInt16Val);
    FdoPropertyValue*       pVal3 = FdoPropertyValue::Create(L"Name3", pDblVal);

    // Fill collection
    pColl->Insert(0, pVal1);
    pColl->Insert(1, pVal2);
    pColl->Insert(2, pVal3);

    // check collection
    FDO_CPPUNIT_ASSERT(pColl->GetCount() == 3);

    FDO_CPPUNIT_ASSERT(pColl->Contains(pVal1));
    FDO_CPPUNIT_ASSERT(pColl->Contains(pVal2));
    FDO_CPPUNIT_ASSERT(pColl->Contains(pVal3));

    FDO_CPPUNIT_ASSERT(pColl->IndexOf(pVal1) == 0);
    FDO_CPPUNIT_ASSERT(pColl->IndexOf(pVal2) == 1);
    FDO_CPPUNIT_ASSERT(pColl->IndexOf(pVal3) == 2);


    FDO_CPPUNIT_ASSERT(pColl->FindItem(L"Bogus") == NULL);

	// check that both item by index and name give same result
    pVal = pColl->GetItem(0);
    FDO_CPPUNIT_ASSERT(pVal == pVal1);
    pVal->Release();
    pVal = pColl->GetItem(L"Name1");
    FDO_CPPUNIT_ASSERT(pVal == pVal1);
    pVal->Release();
    pVal = pColl->FindItem(L"Name1");
    FDO_CPPUNIT_ASSERT(pVal == pVal1);
    pVal->Release();
	// and the rest are there
    pVal = pColl->GetItem(1);
    FDO_CPPUNIT_ASSERT(pVal == pVal2);
    pVal->Release();
    pVal = pColl->GetItem(2);
    FDO_CPPUNIT_ASSERT(pVal == pVal3);
    pVal->Release();

    // release everything.
    pColl->Release();

    pDblVal->Release();
    pByteVal->Release();
    pInt16Val->Release();
    pIdent->Release();

    pVal1->Release();
    pVal2->Release();
    pVal3->Release();
}

