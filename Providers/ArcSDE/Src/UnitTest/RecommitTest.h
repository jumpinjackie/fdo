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

#ifndef CPP_UNIT_RecommitTest_H
#define CPP_UNIT_RecommitTest_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ArcSDETests.h"

class RecommitTest :
    public ArcSDETests
{
    CPPUNIT_TEST_SUITE (RecommitTest);
    CPPUNIT_TEST (recommit);
    CPPUNIT_TEST (multibranch);
    CPPUNIT_TEST_SUITE_END ();

    static FdoPtr<FdoIConnection> mConnection;

public:
    RecommitTest (void);
    virtual ~RecommitTest (void);
    void setUp ();
    void tearDown ();

protected:
    // tests:
    void recommit ();
    void multibranch ();

    void CreateSchema ();
    void AddData();
    const FdoString* getActiveLongTransaction ();
    void activateSpatialContext (const FdoString* spatialContextName);
    void activateLongTransaction (const FdoString* LTName);    
    void deactivateLongTransaction ();    
    void createLongTransaction (const FdoString* LTName);
    void updateData (FdoString* val);
    void createVersionConflict ();
    void rollbackLongTransaction (const FdoString *LTName);
    bool longTransactionExists (const FdoString *LTName);
    void updateData1 ();
    void insertData1 ();
    void deleteData1 ();
    void updateData2 ();
    void deleteData2 ();
    void insertData2 ();

    void CleanUpLTs();
};
#endif // CPP_UNIT_RecommitTest_H


