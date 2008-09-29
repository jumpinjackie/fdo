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

enum AssociationSelectType {
    Select_NoIdentity,
    Select_WithIdentityBothSet,
    Select_WithIdentityParent,
    Select_WithIdentityAssociated,
    Select_WithIdentityError,
    Select_NoIdentityObjNested
};

class AssociationSelectTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( AssociationSelectTest );
  CPPUNIT_TEST( select_NoIdent );
  CPPUNIT_TEST( select_NoIdentFeatClass );
  CPPUNIT_TEST( select_WithIdent );
  CPPUNIT_TEST( select_WithIdentNoFeatClass );
  CPPUNIT_TEST( select_NoIdentReverse );
  CPPUNIT_TEST( select_WithIdentReverse );
  CPPUNIT_TEST_SUITE_END();

public:
    AssociationSelectTest(void);
    virtual ~AssociationSelectTest(void);
    void setUp () { insertUtil.setUp(); }
    void tearDown () { insertUtil.tearDown (); }
    void select_NoIdent();
    void select_NoIdentFeatClass();
    void select_NoIdentReverse();
    void select_WithIdent();
    void select_WithIdentNoFeatClass();
    void select_WithIdentReverse();

private:
    void masterTestNoObj( AssociationSelectType type, bool assocIsFeat = false, bool ownerIsFeat = true );
    void masterTestNoObjReverse( AssociationSelectType type  );
    AssociationInsertTest   insertUtil;
};

#endif

