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
  CPPUNIT_TEST( insertAutoGen );
  //CPPUNIT_TEST( insertLongString );
  CPPUNIT_TEST( featureReaderTest );
  CPPUNIT_TEST( insertNonExistProp );
  CPPUNIT_TEST_SUITE_END();

public:
    FdoInsertTest(void);
    FdoInsertTest(FdoString *suffix);
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
    virtual void DisableFailures(){m_DisableFailures=true;};
	virtual void insertAutoGen(); //Insert empty non-feature
	virtual void insertLongString();
    virtual void featureReaderTest();
    virtual void insertNonExistProp();

	static FdoPropertyValue* AddNewProperty( FdoPropertyValueCollection* propertyValues, const wchar_t *name );

protected:
    virtual void  set_provider() {};   
    void insertDate (FdoIConnection *connection, FdoDateTime dateTime, FdoString *colorIndex);
    void insertDateVerification (FdoIConnection *connection, int numOfSuccess);
    void insertDateCleanUp (FdoIConnection *connection);
    void insertBoundaryCleanup( FdoIConnection* connection );

    // Functions used to setup the test environment for the unit test
    // featureReaderTest and execute it.
    FdoIFeatureReader *AddFeature (FdoIConnection *connection, FdoString *className, bool isSpatial, int idScenario);

    FdoDataPropertyDefinition *CreateDataProperty (FdoString   *propertyName,
                                                   FdoDataType dataType,
                                                   FdoInt32    dataSize,
                                                   bool        isNullable);
    FdoClass *CreateFdoClass (FdoString *className);
    FdoFeatureClass *CreateFdoFeatureClass (FdoString *className,
                                            FdoString *idColName1,
                                            FdoString *idColName2,
                                            int       idScenario);
    FdoGeometricPropertyDefinition *CreateGeometricProperty (FdoString *propertyName);


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

    wchar_t  mSuffix[12];
    bool m_DisableFailures;
};

#endif
