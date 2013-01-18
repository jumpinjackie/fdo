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

#ifndef CPP_UNIT_ASSOCIATIONUPDATETEST_H
#define CPP_UNIT_ASSOCIATIONUPDATETEST_H

#include "FdoAssociationSchemaTest.h"

/* 
 * A test case for the insert.
 */

enum AssociationUpdateType {
    Update_NoIdentity,
    Update_WithIdentityBothSet,
    Update_WithIdentityParent,
    Update_WithIdentityAssociated,
    Update_WithIdentityError,
    Update_NoIdentityObjNested
};

class FdoAssociationUpdateTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( FdoAssociationUpdateTest );
  CPPUNIT_TEST( update_NoIdent );
  CPPUNIT_TEST( update_WithIdent );
  CPPUNIT_TEST( update_WithIdentNoFeatClass );
  CPPUNIT_TEST( update_NoIdentAssocFeatClass );

  CPPUNIT_TEST( update_WithIdentParent );
  CPPUNIT_TEST( update_WithIdentError );

  CPPUNIT_TEST( update_NoIdentObj );
  CPPUNIT_TEST( update_WithIdentObj );
  CPPUNIT_TEST( update_NoIdentObjNested );
  CPPUNIT_TEST_SUITE_END();

public:
    FdoAssociationUpdateTest(void);
    virtual ~FdoAssociationUpdateTest(void);
    void setUp () { mSchemaUtil = GetSchemaUtil(); mSchemaUtil->setUp(); }
    void tearDown () { mSchemaUtil->tearDown (); }
    virtual void update_NoIdent();
    virtual void update_NoIdentAssocFeatClass ();
    virtual void update_WithIdent();
    virtual void update_WithIdentNoFeatClass();
    virtual void update_WithIdentParent();
    virtual void update_WithIdentError();
    virtual void update_NoIdentObj();
    virtual void update_WithIdentObj();
    virtual void update_NoIdentObjNested(); 

private:
    void masterTestNoObj( AssociationUpdateType type, const wchar_t* name1, const wchar_t* name2, int id, bool assocIsFeat=false, bool ownerIsFeat=true );
    void masterTestWithObj( AssociationUpdateType type, const wchar_t* name1, const wchar_t* name2, int id );
    FdoAssociationSchemaTest   *mSchemaUtil;
protected:
    virtual void  set_provider() {};
	virtual FdoAssociationSchemaTest* GetSchemaUtil() {return NULL;}

protected:
	static FdoPropertyValue* AddNewProperty( FdoPropertyValueCollection* propertyValues, const wchar_t *name );
};

#endif // CPP_UNIT_ASSOCIATIONUPDATETEST_H
