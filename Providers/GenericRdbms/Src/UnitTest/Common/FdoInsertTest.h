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

#ifndef CPP_UNIT_FDOINSERTTEST_H
#define CPP_UNIT_FDOINSERTTEST_H

/*
 * A test case for the insert.
 */

class FdoInsertTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( FdoInsertTest );
  CPPUNIT_TEST( insert );
  CPPUNIT_TEST( insertEmptyProps );
  CPPUNIT_TEST( insert2 );
  CPPUNIT_TEST( insert3 );
  CPPUNIT_TEST( insertBoundary );
  CPPUNIT_TEST( insertDate );
  CPPUNIT_TEST( smartPointerInsert );
  CPPUNIT_TEST( insertFlushTest );
  //CPPUNIT_TEST( conditionalInsert );
  CPPUNIT_TEST_SUITE_END();

public:
    FdoInsertTest(void);
    FdoInsertTest(char *suffix);
    virtual ~FdoInsertTest(void);
    void setUp ();

    void MainInsertTest (FdoIConnection *conn=NULL);

    virtual void insert ();
    virtual void insertEmptyProps(); // Insert empty features
    virtual void insertFlushTest(); // Insert select while inserting new records
    virtual void insert2 ();
    virtual void insert3 ();
    virtual void insertBoundary ();
    virtual void insertDate () {};
    virtual void smartPointerInsert();
	virtual void conditionalInsert();

protected:
    virtual void  set_provider() {};   
    void insertDate (FdoIConnection *connection, FdoDateTime dateTime, FdoString *colorIndex);
    void insertDateVerification (FdoIConnection *connection, int numOfSuccess);
    void insertDateCleanUp (FdoIConnection *connection);
	static FdoPropertyValue* AddNewProperty( FdoPropertyValueCollection* propertyValues, const wchar_t *name );
    void insertBoundaryCleanup( FdoIConnection* connection );

    //Various Max/Min values for various numeric types.
    //Used by insertBoundary test.
    //Providers can overrides these if they don't support the same ranges of values.

    virtual FdoByte GetMinByteValue();
    virtual FdoInt16 GetMinInt16Value();
    virtual FdoInt32 GetMinInt32Value();
    virtual FdoInt64 GetMinInt64Value();
    virtual FdoFloat GetMinSingleValue();
    virtual FdoDouble GetMinDoubleValue();

    virtual FdoByte GetMaxByteValue();
    virtual FdoInt16 GetMaxInt16Value();
    virtual FdoInt32 GetMaxInt32Value();
    virtual FdoInt64 GetMaxInt64Value();
    virtual FdoFloat GetMaxSingleValue();
    virtual FdoDouble GetMaxDoubleValue();

    virtual FdoFloat GetSmallestSingleValue();
    virtual FdoDouble GetSmallestDoubleValue();

    char  mSuffix[12];
};

#endif
