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

#ifndef TESTCOMMONINSERT_H
#define TESTCOMMONINSERT_H

class TestCommonInsert : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( TestCommonInsert );

    CPPUNIT_TEST( TestDefaultValues );

	CPPUNIT_TEST_SUITE_END();

public:
	TestCommonInsert(void);
    virtual ~TestCommonInsert(void);
    virtual void setUp ();

    virtual void TestDefaultValues();

    class Context 
    {
    public:
        Context( FdoString* suffix );
        virtual ~Context();

        FdoPtr<FdoIConnection> connection;
        FdoStringP mSuffix;
    };

protected:
    virtual FdoString* GetDefaultSchemaName(void);

    virtual void CreateConnection( Context& context, FdoBoolean recreateDb = false );

    void DoTestDefaultValues(Context& context);
    void DoSubtestDefaultValues(
        FdoIConnection* connection,
        FdoString* testName,
        FdoBoolean boolValue,
        FdoByte byteValue,
        FdoString* dateTimeValue1,
        FdoString* dateTimeValue2,
        FdoDouble decimalValue,
        FdoDouble doubleValue,
        FdoInt16 int16Value,
        FdoInt32 int32Value,
        FdoInt64 int64Value,
        FdoFloat singleValue,
        FdoString* stringValue
    );

    void VldDateTime( FdoStringP expected, FdoIFeatureReader* rdr, FdoStringP propName );
    
    FdoPtr<FdoDataValue> FixDataValueType( FdoPtr<FdoDataValue> val, FdoDataType propType );

    bool SupportsDataType( FdoISchemaCapabilities* cap, FdoDataType dt );

    virtual bool DefaultsTime();

    virtual bool SupportsTime();

    virtual bool SupportsTimeOnly();

    virtual bool InsertsDefaultValues();

    virtual FdoDataType GetAutoIncrementType();

};

#endif	//TESTCOMMONINSERT_H
