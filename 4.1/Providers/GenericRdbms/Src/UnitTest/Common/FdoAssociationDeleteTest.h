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

#ifndef CPP_UNIT_ASSOCIATIONDELETETEST_H
#define CPP_UNIT_ASSOCIATIONDELETETEST_H

#include "FdoAssociationInsertTest.h"

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

class FdoAssociationDeleteTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( FdoAssociationDeleteTest );
  
  // The next 3 tests should be run in that order
  CPPUNIT_TEST( delete_NoIdent ); // This test sets the data for itself and the next 2 tests
  CPPUNIT_TEST( delete_NoIdentPrevent );
  CPPUNIT_TEST( delete_NoIdentCascade );

  CPPUNIT_TEST( delete_NoIdentCascadeFeatClass );

  // The next 3 tests should be run in that order
  CPPUNIT_TEST( delete_WithIdent ); // This test sets the data for itself and the next 2 tests
  CPPUNIT_TEST( delete_WithIdentPrevent );
  CPPUNIT_TEST( delete_WithIdentCascade );

  CPPUNIT_TEST( delete_NoIdentObj );
  CPPUNIT_TEST( delete_WithIdentObj );
  CPPUNIT_TEST( delete_NoIdentObjNested );
  CPPUNIT_TEST_SUITE_END();

public:
    FdoAssociationDeleteTest(void);
    virtual ~FdoAssociationDeleteTest(void);
    void setUp () { mInsertUtil = GetInsertUtil(); mInsertUtil->setUp(); }
    void tearDown () { mInsertUtil->tearDown (); }
    
    void delete_NoIdent();
    void delete_NoIdentPrevent();
    void delete_NoIdentCascade();

    void delete_NoIdentCascadeFeatClass();

    void delete_WithIdent();
    void delete_WithIdentPrevent();
    void delete_WithIdentCascade();

    void delete_NoIdentObj();
    void delete_WithIdentObj();
    void delete_NoIdentObjNested(); 

private:
    void masterTestNoObj( AssociationDeleteType type, FdoDeleteRule deleteRule, bool assocIsFeat = false );
    void masterTestNoObjReverse( AssociationDeleteType type  );
    void masterTestWithObj( AssociationDeleteType type );
    FdoAssociationInsertTest   *mInsertUtil;

protected:
    virtual void  set_provider() {};
	virtual bool  SupportsTransaction() { return true; }
	virtual FdoAssociationInsertTest* GetInsertUtil() {return NULL;}
};

#endif // CPP_UNIT_ASSOCIATIONDELETETEST_H
