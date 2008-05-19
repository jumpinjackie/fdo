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

#ifndef CPP_UNIT_LOCKTESTS_H
#define CPP_UNIT_LOCKTESTS_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ArcSDETests.h"

class LockTests : public ArcSDETests
{
    CPPUNIT_TEST_SUITE (LockTests);
    CPPUNIT_TEST (acquire_release);
    CPPUNIT_TEST (acquire_release_nonlockable);
    CPPUNIT_TEST (release_only);
    CPPUNIT_TEST (two_user_exclusion);
    CPPUNIT_TEST (get_owner);
    CPPUNIT_TEST (select);
    CPPUNIT_TEST (select_partial);
    CPPUNIT_TEST (select_conflicts_first);
    CPPUNIT_TEST (update_where_conflict);
    CPPUNIT_TEST (update_spatial_conflict);
    CPPUNIT_TEST (delete_where_conflict);
    CPPUNIT_TEST (delete_spatial_conflict);
    CPPUNIT_TEST (select_conflicts_die_first);
    CPPUNIT_TEST (select_reader_dies_first);
    CPPUNIT_TEST (select_select_dies_first);
    CPPUNIT_TEST (release_partial);
    CPPUNIT_TEST (release_anothers);
    CPPUNIT_TEST_SUITE_END ();

    static FdoPtr<FdoIConnection> mConnection;

public:
    LockTests (void);
    virtual ~LockTests (void);
	void setUp ();
	void tearDown ();

protected:
    void CreateSchema (const wchar_t* className, bool lockable);
    void CleanUp(void);

    // tests:
    void acquire_release ();
    void acquire_release_nonlockable ();
    void release_only ();
    void two_user_exclusion ();
    void get_owner ();
    void select ();
    void select_partial ();
    void select_conflicts_first ();
    void update_where_conflict ();
    void update_spatial_conflict ();
    void delete_where_conflict ();
    void delete_spatial_conflict ();
    void select_conflicts_die_first ();
    void select_reader_dies_first ();
    void select_select_dies_first ();
    void release_partial ();
    void release_anothers ();
};

#endif // CPP_UNIT_LOCKTESTS_H
