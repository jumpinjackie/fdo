 //
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

#ifndef CPP_UNIT_DESCRIBESCHEMATEST_H
#define CPP_UNIT_DESCRIBESCHEMATEST_H

/*
 * A test case for the schema.
 * Loads a set of Feature schemas, in logical-physical
 * form, from a database.
 *
 * The schemas are then written to an XML file.
 */

class DescribeSchemaTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( DescribeSchemaTest );
  CPPUNIT_TEST( describe );

  // Quick test any database specified in "datastore" env.
  //CPPUNIT_TEST( describe2 );

  // Activate when necessary. 
  //CPPUNIT_TEST( dropDb );
  CPPUNIT_TEST_SUITE_END();

public:
    DescribeSchemaTest(void);
    virtual ~DescribeSchemaTest(void);
    void setUp ();

protected:
    virtual void  set_provider() {};   
    virtual void dropDb();
    virtual void describe ();
    void describe2 ();
    virtual FdoInt32 GetLockTypeCount();
    virtual void LoadTestData(FdoIConnection* connection/*, FdoSchemaManagerP sm*/);
    virtual FdoString** GetSchema();
    // Checks if a property is in a readonly property collection.
    // Returns true if present.
    bool Find_ROP( FdoReadOnlyPropertyDefinitionCollection* props, FdoString* propName );

    const wchar_t** mInputSchema;
    const wchar_t** mInputData;
	
    static const wchar_t* mData[];
};
#endif //CPP_UNIT_DESCRIBESCHEMATEST_H
