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

#ifndef _ASSOCIATIONIUPDATETEST_H_
#define _ASSOCIATIONIUPDATETEST_H_

#ifdef _WIN32
#pragma once
#endif

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <map>
#include "AssociationSchemaTest.h"

/* 
 * A test case for the insert.
 */

enum AssociationUpdateType {
    NoIdentity,
    WithIdentityBothSet,
    WithIdentityParent,
    WithIdentityAssociated,
    WithIdentityError,
    NoAssocUpdate
};

class AssociationUpdateTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( AssociationUpdateTest );
  CPPUNIT_TEST( update_NoIdent );
  CPPUNIT_TEST( update_NoAssocSet );
  CPPUNIT_TEST( update_WithIdent );
  CPPUNIT_TEST( update_WithIdentNoFeatClass );
  CPPUNIT_TEST( update_NoIdentAssocFeatClass );

  CPPUNIT_TEST( update_WithIdentParent );
  CPPUNIT_TEST( update_WithIdentError );

  CPPUNIT_TEST_SUITE_END();

public:
    AssociationUpdateTest(void);
    virtual ~AssociationUpdateTest(void);
    void setUp () { schemaUtil.setUp(); }
    void tearDown () { schemaUtil.tearDown (); }
    void update_NoIdent();
    void update_NoIdentAssocFeatClass ();
    void update_WithIdent();
    void update_WithIdentNoFeatClass();
    void update_WithIdentParent();
    void update_WithIdentError();
	void update_NoAssocSet();

private:
    void masterTestNoObj( AssociationUpdateType type, const wchar_t* name1, const wchar_t* name2, int id, bool assocIsFeat=false, bool ownerIsFeat=true );
    AssociationSchemaTest   schemaUtil;
};

#endif

