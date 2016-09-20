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

#ifndef _ASSOCIATIONSELECTTEST_H_
#define _ASSOCIATIONSELECTTEST_H_

#ifdef _WIN32
#pragma once
#endif

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <map>
#include "AssociationInsertTest.h"

/* 
 * A test case for the insert.
 */

enum AssociationDeleteType {
    Delete_NoIdentity,
    Delete_WithIdentityBothSet,
    Delete_WithIdentityParent,
    Delete_WithIdentityAssociated,
    Delete_WithIdentityError,
    Delete_NoIdentityObjNested
};

class AssociationDeleteTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( AssociationDeleteTest );
  
  // The next 3 tests should be run in that order
  CPPUNIT_TEST( delete_NoIdent ); // This test sets the data for itself and the next 2 tests
  CPPUNIT_TEST( delete_NoIdentPrevent );
  CPPUNIT_TEST( delete_NoIdentCascade );

  CPPUNIT_TEST( delete_NoIdentCascadeFeatClass );

  // The next 3 tests should be run in that order
  CPPUNIT_TEST( delete_WithIdent ); // This test sets the data for itself and the next 2 tests
  CPPUNIT_TEST( delete_WithIdentPrevent );
  CPPUNIT_TEST( delete_WithIdentCascade );

  CPPUNIT_TEST_SUITE_END();

public:
    AssociationDeleteTest(void);
    virtual ~AssociationDeleteTest(void);
    void setUp () { insertUtil.setUp(); }
    void tearDown () { insertUtil.tearDown (); }
    
    void delete_NoIdent();
    void delete_NoIdentPrevent();
    void delete_NoIdentCascade();

    void delete_NoIdentCascadeFeatClass();

    void delete_WithIdent();
    void delete_WithIdentPrevent();
    void delete_WithIdentCascade();

private:
    void masterTestNoObj( AssociationDeleteType type, FdoDeleteRule deleteRule, bool assocIsFeat = false );
    void masterTestNoObjReverse( AssociationDeleteType type  );
    AssociationInsertTest   insertUtil;
};

#endif

