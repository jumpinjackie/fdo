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

#ifndef CPP_UNIT_FDOCONSTRAINTSTEST_H
#define CPP_UNIT_FDOCONSTRAINTSTEST_H

#define DBG(x)		//x
#define DBG_MAX(x)		//x

class FdoConstraintsTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( FdoConstraintsTest );

	CPPUNIT_TEST( TestParser );
	CPPUNIT_TEST( TestCreateConstraints );
	CPPUNIT_TEST( TestDescribeConstraints );
	CPPUNIT_TEST( TestUpdateUniqueConstraints );
	CPPUNIT_TEST( TestUpdateCheckConstraints );
	CPPUNIT_TEST( TestDescribeUpdatedConstraints );

	CPPUNIT_TEST_SUITE_END();

public:
    void TestCreateConstraints(void);
	void TestDescribeConstraints(void);
	void TestUpdateUniqueConstraints(void);
	void TestUpdateCheckConstraints(void);
	void TestDescribeUpdatedConstraints(void);
	void TestParser(void);

    void CreateConstraintsSchema(FdoIConnection * connection);
	void DescribeConstraintsSchema(FdoIConnection * connection, FdoString *className, int numUkeys, int numCkeys, bool afterUpdate);
	void UpdateCheckConstraints(FdoIConnection * connection);
	void UpdateUniqueConstraints(FdoIConnection * connection);
	bool TestParser(FdoString* clause);

	FdoConstraintsTest(void);
    virtual ~FdoConstraintsTest(void);
    void setUp ();

protected:
    virtual void  set_provider() {};

	void TestConstraints();

    template< class T> void CheckListConstraint(FdoString* pPropName, FdoPtr<FdoDataValueCollection> pList, T* pMaster, FdoInt32 masterCount )
    {
        CPPUNIT_ASSERT_MESSAGE( 
            (const char*) FdoStringP::Format( L"Wrong number of List Values %ls", pPropName ),
            pList->GetCount() == masterCount     
        );

        for ( int j = 0; j < masterCount; j++ ) {
		    FdoPtr<FdoDataValue>   val1 = FdoDataValue::Create( pMaster[j] );
		    bool					valMatched = false;

    	    for ( int k = 0; k < pList->GetCount() && !valMatched; k++ ) {
	    	    FdoPtr<FdoDataValue>	val = pList->GetItem(k);
			    valMatched = ( wcscmp(val->ToString(), val1->ToString()) == 0 );
		    }	

            CPPUNIT_ASSERT_MESSAGE( 
                (const char*) FdoStringP::Format( L"Missing List Value %ls", pPropName ),
                valMatched
            );
		    DBG(printf("%ls,", val1->ToString()));
	    }
	    DBG(printf("))\n"));
    }    
    
    template< class T> void CheckDoubleListConstraint(FdoString* pPropName, FdoPtr<FdoDataValueCollection> pList, T* pMaster, FdoInt32 masterCount, FdoDataType dataType )
    {
        CPPUNIT_ASSERT_MESSAGE( 
            (const char*) FdoStringP::Format( L"Wrong number of List Values %ls", pPropName ),
            pList->GetCount() == masterCount     
        );

	    for ( int j = 0; j < pList->GetCount(); j++ ) {
		    FdoPtr<FdoDataValue>	val = pList->GetItem(j);
		    bool					valMatched = false;

            for ( int k = 0; k < masterCount && !valMatched; k++ ) {
			    FdoPtr<FdoDataValue>   val1 = FdoDataValue::Create( pMaster[k], dataType );
			    valMatched = ( wcscmp(val->ToString(), val1->ToString()) == 0 );
		    }	

            CPPUNIT_ASSERT_MESSAGE( 
                (const char*) FdoStringP::Format( L"Wrong List Value %ls", pPropName ),
                valMatched
            );
		    DBG(printf("%ls,", val->ToString()));
	    }
	    DBG(printf("))\n"));
    }    
    
};

#endif	//CPP_UNIT_FDOCONSTRAINTSTEST_H
