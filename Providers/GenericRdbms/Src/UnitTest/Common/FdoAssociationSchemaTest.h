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

#ifndef CPP_UNIT_ASSOCIATIONSCHEMATEST_H
#define CPP_UNIT_ASSOCIATIONSCHEMATEST_H

/* 
 * A test case for the DeleteCommand.
 */

class FdoAssociationSchemaTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( FdoAssociationSchemaTest );
  CPPUNIT_TEST( TestCreate_NoIdent );
  CPPUNIT_TEST( TestDelete_rollbak );
  CPPUNIT_TEST( TestDelete_commit );
  CPPUNIT_TEST( TestCreate_NoIdentAssocFeatClass );
  CPPUNIT_TEST( TestCreate_WithIdent );
  CPPUNIT_TEST( TestLoadWithNoObj );
  CPPUNIT_TEST( TestCreate_WithIdentNoFeatClass );
  CPPUNIT_TEST( TestCreate_WithIdentNoFeatClassSubClass );
  CPPUNIT_TEST( TestCreate_NoIdentObj );
  CPPUNIT_TEST( TestCreate_WithIdentObj );
  CPPUNIT_TEST( TestLoadWithObj );
  CPPUNIT_TEST( TestCreate_Rollback );
  CPPUNIT_TEST( TestCreate_NoIdentObjNested );
  CPPUNIT_TEST( TestCreateMultiple );
  CPPUNIT_TEST( TestLoadMultiple );
  CPPUNIT_TEST_SUITE_END();

public:
    FdoAssociationSchemaTest(void);
    virtual ~FdoAssociationSchemaTest(void);
    void setUp ();
	void tearDown ();

    void TestCreate_NoIdent () { TestCreate( false, false ); }

    void TestCreate_NoIdentAssocFeatClass () { TestCreate( false, false,false,false,true,true ); }
    void TestCreate_WithIdent (){ TestCreate( true, false ); }
    void TestCreate_WithIdentNoFeatClass (){ TestCreate( true, false, false, false, true, false, false ); }
    void TestCreate_WithIdentNoFeatClassSubClass (){ TestCreate( true, false, false, false, true, false, false, true ); }
    void TestCreate_NoIdentObj () { TestCreate( false, true ); }
    void TestCreate_WithIdentObj (){ TestCreate( true, true ); }
    void TestCreate_NoIdentObjNested () { TestCreate( false, true, true ); }
    void TestCreate_Rollback () { TestCreate( false, false, false, true, false ); }
    void TestLoadWithObj() { TestLoad(true); }
    void TestLoadWithNoObj() { TestLoad(false); }

    void TestCreate (bool useIdent, bool useObjProp, bool useNetstedObj = false, bool useTransaction = false, 
            bool commitTransaction=true, bool associatedIsFeat=false, bool ownerIsFeat=true, bool addToSubclass=false );

    void TestLoad ( bool withOjb );

    void TestDelete ( bool rollbak );
    void TestDelete_rollbak (  ) { TestDelete( true ); }
    void TestDelete_commit (  ) { TestDelete( false ); }
    void TestCreateMultiple();
    void TestLoadMultiple();

protected:
    virtual void  set_provider() {};

private:
    char dataSource[128];

	void createFreshDb();
};
#endif // CPP_UNIT_ASSOCIATIONSCHEMATEST_H
