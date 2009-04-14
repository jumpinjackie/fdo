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
#ifndef CPP_UNIT_ASSOCIATIONINSERTTEST_H
#define CPP_UNIT_ASSOCIATIONINSERTTEST_H

#include "FdoAssociationSchemaTest.h"

/* 
 * A test case for the insert.
 */

enum FdoAssociationInsertType {
    Insert_NoIdentity,
    Insert_WithIdentityBothSet,
    Insert_WithIdentityParent,
    Insert_WithIdentityAssociated,
    Insert_WithIdentityError,
    Insert_NoIdentityObjNested
};

class FdoAssociationInsertTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( FdoAssociationInsertTest );
  CPPUNIT_TEST( insert_NoIdent );
  CPPUNIT_TEST( insert_WithIdent );
  CPPUNIT_TEST( insert_ManyWithIdent );
  CPPUNIT_TEST( insert_NoIdentAssocFeatClass );

  CPPUNIT_TEST( insert_WithIdentParent );
  CPPUNIT_TEST( insert_WithIdentAssociated );
  CPPUNIT_TEST( insert_WithIdentError );

  CPPUNIT_TEST( insert_NoIdentObj );
  CPPUNIT_TEST( insert_WithIdentObj );
  CPPUNIT_TEST( insert_NoIdentObjNested );
  CPPUNIT_TEST_SUITE_END();

public:
    FdoAssociationInsertTest(void);
    virtual ~FdoAssociationInsertTest(void);
    void setUp () { mSchemaUtil = GetSchemaUtil(); mSchemaUtil->setUp(); }
    void tearDown () { mSchemaUtil->tearDown (); }
    void insert_NoIdent();
    void insert_NoIdentCirc(int circularType);
    void insert_NoIdentAssocFeatClass ();
    void insert_NoIdentAssocFeatClassCirc (int circularType);
    void insert_WithIdent();
    void insert_WithIdentCirc(int circularType);
    void insert_WithIdentNoFeatClass();
    void insert_WithIdentNoFeatClassCirc(int circularType);
    void insert_ManyWithIdent();
    void insert_WithIdentParent();
    void insert_WithIdentAssociated();
    void insert_WithIdentError();
    void insert_NoIdentObj();
    void insert_WithIdentObj();
    void insert_NoIdentObjNested(); 

private:
    void masterTestNoObj( FdoAssociationInsertType type, const wchar_t* name1, const wchar_t* name2, int id, bool assocIsFeat=false, bool ownerIsFeat=true, int circularType = 0 );
    void masterTestWithObj( FdoAssociationInsertType type, const wchar_t* name1, const wchar_t* name2, int id );
	FdoAssociationSchemaTest   *mSchemaUtil;
	
protected:
    virtual void  set_provider() {};
	virtual FdoAssociationSchemaTest* GetSchemaUtil() {return NULL;}
};
#endif // CPP_UNIT_ASSOCIATIONINSERTTEST_H
