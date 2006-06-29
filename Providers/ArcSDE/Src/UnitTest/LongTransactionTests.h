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

#ifndef CPP_UNIT_LongTransactionTests_H
#define CPP_UNIT_LongTransactionTests_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ArcSDETests.h"

class LongTransactionTests : public ArcSDETests
{
    CPPUNIT_TEST_SUITE (LongTransactionTests);
    CPPUNIT_TEST (create_then_rollback);
    CPPUNIT_TEST (name_test);
    CPPUNIT_TEST (get);
    CPPUNIT_TEST (get_root);
    CPPUNIT_TEST (get_children);
    CPPUNIT_TEST (get_active);
    CPPUNIT_TEST (get_specific);
    CPPUNIT_TEST (get_specific_qualified);
    CPPUNIT_TEST (activate_root);
    CPPUNIT_TEST (activate_all);
    CPPUNIT_TEST (deactivate);
    CPPUNIT_TEST (insert);
    CPPUNIT_TEST (commit_insert);
    CPPUNIT_TEST (del);
    CPPUNIT_TEST (commit_delete);
    CPPUNIT_TEST (update);
    CPPUNIT_TEST (commit_update);
    CPPUNIT_TEST (commit_update_conflict_child_wins);
    CPPUNIT_TEST (commit_update_conflict_parent_wins);
    CPPUNIT_TEST (commit_leaf);
    CPPUNIT_TEST (update_parent);
    CPPUNIT_TEST (rollback_arccatalog_version);
    CPPUNIT_TEST (activate_arccatalog_version);
    CPPUNIT_TEST (commit_arccatalog_version);
    CPPUNIT_TEST_SUITE_END ();

    static FdoPtr<FdoIConnection> mConnection;

public:
    LongTransactionTests (void);
    virtual ~LongTransactionTests (void);
	void setUp ();
	void tearDown ();

protected:
    // Helper methods:
    void CleanUp ();
    void LongTransactionGone (const wchar_t* name);
    void MakeTwoObjectsInTwoVersions (int* item1, int* item2);
    void MakeUpdateConflict (int* item1);
    void CommitUpdateConflict (bool child);
    FdoStringP CreateArcCatalogLikeVersion();

    // data:
    static FdoString* NAME() { return L"My Long Transaction"; };
    static FdoStringP DECORATED_NAME() { return  FdoStringP::Format(L"%ls.My Long Transaction", (FdoString*)ArcSDETestConfig::UserNameMetadcov()); }
    static FdoString* DESCRIPTION() { return L"long transaction test version"; }
    static FdoStringP PARENT() { return FdoStringP::Format(L"%ls.DEFAULT", (const wchar_t *) ArcSDETestConfig::SdeUsername()); }
    static FdoStringP OWNER() { return ArcSDETestConfig::UserNameMetadcov(); }
    static FdoString* NAMEA() { return L"Transaction A"; }
    static FdoStringP DECORATED_NAMEA() { return FdoStringP::Format(L"%ls.Transaction A", (FdoString*)ArcSDETestConfig::UserNameMetadcov()); }
    static FdoString* DESCRIPTIONA() { return L"long transaction A"; }
    static FdoString* NAMEB() { return L"Transaction B"; }
    static FdoStringP DECORATED_NAMEB() { return FdoStringP::Format(L"%ls.Transaction B", (FdoString*)ArcSDETestConfig::UserNameMetadcov()); }
    static FdoString* DESCRIPTIONB() { return L"long transaction B"; }

    //                                                                   length precision scale?
    UnitTestData *property0; // (L"Id", L"Unique identifier", FdoDataType_Int32, 0,  0, 0, false, true, true,
                             //    NULL, NULL, (wchar_t*)NULL);
    UnitTestData *property1; // (L"LtProperty", L"A string property.", FdoDataType_String, 512,  0, 0, false, false, false,
                            //     L"'version 1 data'", L"'version 2 data'", (wchar_t*)NULL);

    UnitTestClass *theClass; // (ArcSDETestConfig::ClassNameExample(), L"Example class definition.", 0, false, true, &property0, &property1, NULL);

    UnitTestData *property1Modified; // (L"LtProperty", L"A string property.", FdoDataType_String, 512,  0, 0, false, false, false,
                                // L"'version 3 data'", L"'version 2 data'", (wchar_t*)NULL);

    UnitTestClass *theClassModified; // (ArcSDETestConfig::ClassNameExample(), L"Example class definition.", 0, false, true, &property0, &property1Modified, NULL);

    UnitTestData *property1A; // (L"LtProperty", L"A string property.", FdoDataType_String, 512,  0, 0, false, false, false,
                              //   L"'version A data'", L"'version 2 data'", (wchar_t*)NULL);

    UnitTestClass *theClassA; // (ArcSDETestConfig::ClassNameExample(), L"Example class definition.", 0, false, true, &property0, &property1A, NULL);

    UnitTestData *property1B; // (L"LtProperty", L"A string property.", FdoDataType_String, 512,  0, 0, false, false, false,
                              //   L"'version B data'", L"'version 2 data'", (wchar_t*)NULL);

    UnitTestClass *theClassB; // (ArcSDETestConfig::ClassNameExample(), L"Example class definition.", 0, false, true, &property0, &property1B, NULL);


    // tests:
    void create_then_rollback ();
    void name_test ();
    void get ();
    void get_root ();
    void get_active ();
    void get_children ();
    void get_specific ();
    void get_specific_qualified ();
    void activate_root ();
    void activate_all ();
    void deactivate ();
    void insert ();
    void commit_insert ();
    void del ();
    void commit_delete ();
    void update ();
    void commit_update ();
    void commit_update_conflict_child_wins ();
    void commit_update_conflict_parent_wins ();
    void commit_leaf ();
    void update_parent ();
    void rollback_arccatalog_version ();
    void activate_arccatalog_version();
    void commit_arccatalog_version();
};

#endif // CPP_UNIT_LongTransactionTests_H
