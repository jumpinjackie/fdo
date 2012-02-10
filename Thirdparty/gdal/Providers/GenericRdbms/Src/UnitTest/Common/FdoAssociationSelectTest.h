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

#ifndef CPP_UNIT_ASSOCIATIONSELECTTEST_H
#define CPP_UNIT_ASSOCIATIONSELECTTEST_H

#include "FdoAssociationInsertTest.h"

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

class FdoAssociationSelectTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( FdoAssociationSelectTest );
  CPPUNIT_TEST( select_NoIdent );
  CPPUNIT_TEST( select_NoIdentFeatClass );
  CPPUNIT_TEST( select_WithIdent );
  CPPUNIT_TEST( select_WithIdentNoFeatClass );
  CPPUNIT_TEST( select_NoIdentReverse );
  CPPUNIT_TEST( select_WithIdentReverse );


  CPPUNIT_TEST( select_NoIdentObj );
  CPPUNIT_TEST( select_WithIdentObj );
  CPPUNIT_TEST( select_NoIdentObjNested );
  CPPUNIT_TEST( select_NoIdentFeatClassCirc );
  CPPUNIT_TEST( select_NoIdentNoFeatClassCirc );
  CPPUNIT_TEST( select_NoIdentBothClassCirc );
  CPPUNIT_TEST_SUITE_END();

public:
    FdoAssociationSelectTest(void);
    virtual ~FdoAssociationSelectTest(void);
    void setUp () { mInsertUtil = GetInsertUtil(); mInsertUtil->setUp(); }
    void tearDown () { mInsertUtil->tearDown (); }
    void select_NoIdent();
    void select_NoIdentFeatClass();
    void select_NoIdentReverse();
    void select_WithIdent();
    void select_WithIdentNoFeatClass();
    void select_WithIdentReverse();
    void select_NoIdentObj();
    void select_WithIdentObj();
    void select_NoIdentObjNested(); 
    void select_NoIdentFeatClassCirc(); 
    void select_NoIdentNoFeatClassCirc(); 
    void select_NoIdentBothClassCirc(); 

private:
    void masterTestNoObj( AssociationSelectType type, bool assocIsFeat = false, bool ownerIsFeat = true, int circularType = 0 );
    void masterTestNoObjReverse( AssociationSelectType type  );
    void masterTestWithObj( AssociationSelectType type );
    FdoAssociationInsertTest   *mInsertUtil;
protected:
    virtual void  set_provider() {};
	virtual FdoAssociationInsertTest* GetInsertUtil() {return NULL;}
};

#endif // CPP_UNIT_ASSOCIATIONSELECTTEST_H
