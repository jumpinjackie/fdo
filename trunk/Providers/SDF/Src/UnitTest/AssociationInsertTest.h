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

#ifndef _ASSOCIATIONINSERTTEST_H_
#define _ASSOCIATIONINSERTTEST_H_

#ifdef _WIN32
#pragma once
#endif
#include "Fdo.h"
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <map>
#include "AssociationSchemaTest.h"

/* 
 * A test case for the insert.
 */

enum AssociationInsertType {
	NoSchema,
    NoIdentity,
    WithIdentityBothSet,
    WithIdentityParent,
    WithIdentityAssociated,
    WithIdentityError,
    NoIdentityObjNested
};

class AssociationInsertTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( AssociationInsertTest );
  CPPUNIT_TEST( insert_NoIdent );
  CPPUNIT_TEST( insert_WithIdent );
  CPPUNIT_TEST( insert_ManyWithIdent );
  CPPUNIT_TEST( insert_NoIdentAssocFeatClass );
  CPPUNIT_TEST( insert_Cardinality_1_1 );

  CPPUNIT_TEST( insert_WithIdentParent );
  CPPUNIT_TEST( insert_WithIdentAssociated );
  CPPUNIT_TEST( insert_WithIdentError );

  CPPUNIT_TEST_SUITE_END();

public:
    AssociationInsertTest(void);
    virtual ~AssociationInsertTest(void);
    void setUp () { schemaUtil.setUp(); }
    void tearDown () { schemaUtil.tearDown (); }
    void insert_NoIdent();
    void insert_NoIdentAssocFeatClass ();
    void insert_WithIdent();
	void insert_Cardinality_1_1();
    void insert_WithIdentNoFeatClass();
    void insert_ManyWithIdent();
    void insert_WithIdentParent();
    void insert_WithIdentAssociated();
    void insert_WithIdentError();

	FdoIConnection* openConnection( bool re_create=true) { return schemaUtil.openConnection(re_create); }

private:
    void masterTestNoObj( AssociationInsertType type, const wchar_t* name1, const wchar_t* name2, int id, bool assocIsFeat=false, bool ownerIsFeat=true );
    AssociationSchemaTest   schemaUtil;
};

#endif

