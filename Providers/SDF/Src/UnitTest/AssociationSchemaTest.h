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

#ifndef _ASSOCIATIONSCHEMATEST_H_
#define _ASSOCIATIONSCHEMATEST_H_

#ifdef _WIN32
#pragma once
#endif

#include "Fdo.h"
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <map>
#include <TestCommonMiscUtil.h>

/* 
 * The unit test of the association schema.
 */

class AssociationSchemaTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( AssociationSchemaTest );
  CPPUNIT_TEST( TestCreate_NoIdent );
  CPPUNIT_TEST( TestCreate_NoIdentAssocFeatClass );
  CPPUNIT_TEST( TestCreate_Cardinality_1_1 );
  CPPUNIT_TEST( TestCreate_WithIdent );
  CPPUNIT_TEST( TestLoadWithNoObj );
  CPPUNIT_TEST( TestCreate_WithIdentNoFeatClass );
  CPPUNIT_TEST( TestCreate_WithIdentNoFeatClassSubClass );
  CPPUNIT_TEST( TestCreateMultiple );
  CPPUNIT_TEST( TestLoadMultiple );
  CPPUNIT_TEST_SUITE_END();

public:
    AssociationSchemaTest(void);
    virtual ~AssociationSchemaTest(void);
    void setUp ();
	void tearDown ();

    void TestCreate_NoIdent () { TestCreate( false, false ); }
    void TestCreate_NoIdentAssocFeatClass () { TestCreate( false, false,false,false,true,true ); }
    void TestCreate_WithIdent (){ TestCreate( true, false ); }
	void TestCreate_Cardinality_1_1 ();
    void TestCreate_WithIdentNoFeatClass (){ TestCreate( true, false, false, false, true, false, false ); }
    void TestCreate_WithIdentNoFeatClassSubClass (){ TestCreate( true, false, false, false, true, false, false, true ); }
    void TestCreate_Rollback () { TestCreate( false, false, false, true, false ); }
    void TestLoadWithNoObj() { TestCreate_WithIdent ();TestLoad(false); }
    void TestCreate (bool useIdent, bool useObjProp, bool useNetstedObj = false, bool useTransaction = false, 
            bool commitTransaction=true, bool associatedIsFeat=false, bool ownerIsFeat=true, bool addToSubclass=false );
    void TestLoad ( bool withOjb );
    void TestCreateMultiple();
    void TestLoadMultiple();

	FdoIConnection* openConnection( bool re_create=true);

	void SetMultiplicity( wchar_t *val );
	void SetRevMultiplicity( wchar_t *val );
	static void SetDeleteRule( FdoDeleteRule rule );

private:
	void CheckResult( FdoIConnection* connection, bool useIdent, bool addToSubclass );
    char dataSource[128];
};

#endif

