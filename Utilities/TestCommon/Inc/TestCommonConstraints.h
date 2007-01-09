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

#ifndef TESTCOMMONCONSTRAINTS_H
#define TESTCOMMONCONSTRAINTS_H

#define DBG(x)		//x
#define DBG_MAX(x)		//x

class TestCommonConstraints : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( TestCommonConstraints );

	CPPUNIT_TEST( TestCreateConstraints );
	CPPUNIT_TEST( TestDescribeConstraints );
	CPPUNIT_TEST( TestUpdateUniqueConstraints );
    CPPUNIT_TEST( TestUpdateCheckConstraints );
	CPPUNIT_TEST( TestDescribeUpdatedConstraints );
    CPPUNIT_TEST( TestRestrictCheckConstraints );
	CPPUNIT_TEST( TestDateTimeConstraints );

	CPPUNIT_TEST_SUITE_END();

public:
    virtual void TestCreateConstraints(void);
	virtual void TestDescribeConstraints(void);
	virtual void TestUpdateUniqueConstraints(void);
	virtual void TestUpdateCheckConstraints(void);
	virtual void TestDescribeUpdatedConstraints(void);
	virtual void TestRestrictCheckConstraints(void);
	virtual void TestDateTimeConstraints(void);

    void CreateConstraintsSchema(FdoIConnection * connection);
	void DescribeConstraintsSchema(FdoIConnection * connection, FdoString *className, int numUkeys, int numCkeys, bool afterUpdate);
	void UpdateCheckConstraints(FdoIConnection * connection);
	void UpdateUniqueConstraints(FdoIConnection * connection);
	void RestrictCheckConstraints(FdoIConnection * connection);
	void DateTimeConstraints(FdoIConnection * connection);

	TestCommonConstraints(void);
    virtual ~TestCommonConstraints(void);
    virtual void setUp ();

protected:

    virtual FdoIConnection* CreateConnection( FdoBoolean recreateDb = false );
    virtual FdoBoolean CanRestrictCheckConstraint();

    virtual FdoInt32 GetExpectedCheckConstraintCount( FdoIConnection* connection );

    FdoInt32 GetNextFeatId( FdoIConnection* connection, FdoString* className );

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
    
    void CheckDateListConstraint(FdoString* pPropName, FdoPtr<FdoDataValueCollection> pList, FdoString** pMaster, FdoInt32 masterCount )
    {
        CPPUNIT_ASSERT_MESSAGE( 
            (const char*) FdoStringP::Format( L"Wrong number of List Values %ls", pPropName ),
            pList->GetCount() == masterCount     
        );

	    for ( int j = 0; j < pList->GetCount(); j++ ) {
		    FdoPtr<FdoDataValue>	val = pList->GetItem(j);
		    bool					valMatched = false;

            // Some providers' DescribeSchema implementations set the constraint
            // values for date properties to string. Temporarily skip this check
            // for these providers until this bug is fixed. 
            if ( val->GetDataType() == FdoDataType_DateTime ) {
                for ( int k = 0; k < masterCount && !valMatched; k++ ) {
			        valMatched = ( wcscmp(val->ToString(), pMaster[k]) == 0 );
		        }	

                CPPUNIT_ASSERT_MESSAGE( 
                    (const char*) FdoStringP::Format( L"Wrong List Value %ls", pPropName ),
                    valMatched
                );
            }

		    DBG(printf("%ls,", val->ToString()));
	    }
	    DBG(printf("))\n"));
    }    

    template< class T> FdoPtr<FdoDataPropertyDefinition> AddRangeProperty(
        FdoISchemaCapabilities* schemaCap,
        FdoFeatureClass* pClass, 
        FdoString* pPropName, 
        FdoDataType dataType, 
        T* pRange, 
        FdoInt32 pRangeCount,
        FdoBoolean minInclusive,
        FdoBoolean maxInclusive
    )
    {
        FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( pPropName, L"" );
        pProp->SetDataType( dataType );
        pProp->SetNullable(true);

        // Add CHECK constraint
        if ( schemaCap->SupportsExclusiveValueRangeConstraints() && schemaCap->SupportsInclusiveValueRangeConstraints())
        {
            FdoPtr<FdoPropertyValueConstraintRange>  newRangeConstrR =  FdoPropertyValueConstraintRange::Create();
            newRangeConstrR->SetMinInclusive(minInclusive);
            FdoPtr<FdoDataValue>   valR = FdoDataValue::Create( pRange[0] );
            newRangeConstrR->SetMinValue( valR );

            if ( pRangeCount > 1 ) {
                newRangeConstrR->SetMaxInclusive(maxInclusive);
                valR = FdoDataValue::Create( pRange[1] );
                newRangeConstrR->SetMaxValue( valR );
            }    
                
            pProp->SetValueConstraint(newRangeConstrR);
        }

        FdoPropertiesP(pClass->GetProperties())->Add( pProp  );

        return pProp;
    }

    template< class T> FdoPtr<FdoDataPropertyDefinition> AddDoubleRangeProperty(
        FdoISchemaCapabilities* schemaCap,
        FdoFeatureClass* pClass, 
        FdoString* pPropName, 
        FdoDataType dataType, 
        T* pRange, 
        FdoInt32 pRangeCount,
        FdoBoolean minInclusive,
        FdoBoolean maxInclusive
    )
    {
        FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( pPropName, L"" );
        pProp->SetDataType( dataType );
        pProp->SetNullable(true);

        // Add CHECK constraint
        if ( schemaCap->SupportsExclusiveValueRangeConstraints() && schemaCap->SupportsInclusiveValueRangeConstraints())
        {
            FdoPtr<FdoPropertyValueConstraintRange>  newRangeConstrR =  FdoPropertyValueConstraintRange::Create();
            newRangeConstrR->SetMinInclusive(minInclusive);
            FdoPtr<FdoDataValue>   valR = FdoDataValue::Create( pRange[0], dataType );
            newRangeConstrR->SetMinValue( valR );

            if ( pRangeCount > 1 ) {
                newRangeConstrR->SetMaxInclusive(maxInclusive);
                valR = FdoDataValue::Create( pRange[1], dataType );
                newRangeConstrR->SetMaxValue( valR );                    
            }

            pProp->SetValueConstraint(newRangeConstrR);
        }

        FdoPropertiesP(pClass->GetProperties())->Add( pProp  );

        return pProp;
    }

    template< class T> FdoPtr<FdoDataPropertyDefinition> AddListProperty(
        FdoISchemaCapabilities* schemaCap,
        FdoFeatureClass* pClass, 
        FdoString* pPropName, 
        FdoDataType dataType, 
        T* pList, 
        FdoInt32 pListCount
    )
    {
        FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( pPropName, L"" );
        pProp->SetDataType( dataType );
        pProp->SetNullable(true);

        // Add CHECK constraint
        if ( schemaCap->SupportsValueConstraintsList() )
        {
            FdoPtr<FdoPropertyValueConstraintList>  newListConstr =  FdoPropertyValueConstraintList::Create();
            FdoPtr<FdoDataValueCollection> list = newListConstr->GetConstraintList();

            FdoInt32 idx;

            for ( idx = 0; idx < pListCount; idx++ ) {
                FdoPtr<FdoDataValue>   val = FdoDataValue::Create( pList[idx] );
                list->Add( val );
            }
                    
            pProp->SetValueConstraint(newListConstr);
        }

        FdoPropertiesP(pClass->GetProperties())->Add( pProp  );

        return pProp;
    }

    template< class T> FdoPtr<FdoDataPropertyDefinition> AddDoubleListProperty(
        FdoISchemaCapabilities* schemaCap,
        FdoFeatureClass* pClass, 
        FdoString* pPropName, 
        FdoDataType dataType, 
        T* pList, 
        FdoInt32 pListCount
    )
    {
        FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( pPropName, L"" );
        pProp->SetDataType( dataType );
        pProp->SetNullable(true);

        // Add CHECK constraint
        if ( schemaCap->SupportsValueConstraintsList() )
        {
            FdoPtr<FdoPropertyValueConstraintList>  newListConstr =  FdoPropertyValueConstraintList::Create();
            FdoPtr<FdoDataValueCollection> list = newListConstr->GetConstraintList();

            FdoInt32 idx;

            for ( idx = 0; idx < pListCount; idx++ ) {
                FdoPtr<FdoDataValue>   val = FdoDataValue::Create( pList[idx], dataType );
                list->Add( val );
            }
                    
            pProp->SetValueConstraint(newListConstr);
        }

        FdoPropertiesP(pClass->GetProperties())->Add( pProp  );

        return pProp;
    }
};

#endif	//TESTCOMMONCONSTRAINTS_H
