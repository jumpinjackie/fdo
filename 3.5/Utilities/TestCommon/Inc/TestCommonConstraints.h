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
    CPPUNIT_TEST( TestCheckConstraintsData );
	CPPUNIT_TEST( TestDateTimeConstraints );
    CPPUNIT_TEST( TestBaseReferences );
    CPPUNIT_TEST( TestCreateLTConstraints );
    CPPUNIT_TEST( TestDescribeLTConstraints );
    CPPUNIT_TEST( TestUpdateLTUniqueConstraints );
    CPPUNIT_TEST( TestUpdateLTCheckConstraints );
    CPPUNIT_TEST( TestDescribeLTUpdatedConstraints );

	CPPUNIT_TEST_SUITE_END();

public:
	TestCommonConstraints(void);
    virtual ~TestCommonConstraints(void);
    virtual void setUp ();

    virtual void TestCreateConstraints(void);
	virtual void TestDescribeConstraints(void);
	virtual void TestUpdateUniqueConstraints(void);
	virtual void TestUpdateCheckConstraints(void);
	virtual void TestDescribeUpdatedConstraints(void);
	virtual void TestRestrictCheckConstraints(void);
	virtual void TestCheckConstraintsData(void);
	virtual void TestDateTimeConstraints(void);
    virtual void TestCreateLTConstraints(void);
	virtual void TestDescribeLTConstraints(void);
	virtual void TestUpdateLTUniqueConstraints(void);
	virtual void TestUpdateLTCheckConstraints(void);
	virtual void TestDescribeLTUpdatedConstraints(void);

protected:
    virtual FdoString* GetDefaultSchemaName(void);

    class Context 
    {
    public:
        Context( FdoInt32 ltMethodIn = 0 );
        virtual ~Context();

        FdoPtr<FdoIConnection> connection;
        FdoInt32 ltMethod;
    };

    void DoTestCreateConstraints(Context& context);
	void DoTestDescribeConstraints(Context& context);
	void DoTestUpdateUniqueConstraints(Context& context);
	void DoTestUpdateCheckConstraints(Context& context);
	void DoTestDescribeUpdatedConstraints(Context& context);

    // Tests unique constraints that reference base properties.
	virtual void TestBaseReferences(void);

    void CreateConstraintsSchema(Context& context);
	void DescribeConstraintsSchema(Context& context, FdoString *className, int numUkeys, int numCkeys, bool afterUpdate);
	void UpdateCheckConstraints(Context& context);
	void UpdateUniqueConstraints(Context& context);
	void RestrictCheckConstraints(Context& context);
	void CheckConstraintsData(Context& context);
	void DateTimeConstraints(Context& context);

    virtual void CreateConnection( Context& context, FdoBoolean recreateDb = false );
    virtual FdoBoolean CanRestrictCheckConstraint();
    virtual FdoBoolean FdoValidatesData();
    virtual FdoDouble GetDoubleRounding(FdoDataType dataType);
    virtual FdoFloat GetSecondsIncrement();
    virtual FdoInt32 GetLtMethod();

    virtual FdoInt32 GetExpectedCheckConstraintCount( FdoIConnection* connection );

    FdoInt32 GetNextFeatId( FdoIConnection* connection, FdoString* className );

    void UpdateAllValues( 
        FdoIConnection* connection,
        FdoString* pClassName, 
        FdoInt32 featId 
    );

    void UpdateRangeDoubleValues( 
        FdoIConnection* connection,
        FdoString* pClassName, 
        FdoString* pIdName, 
        FdoInt32 idValue, 
        FdoString* pPropName,
        FdoDataType dataType, 
        FdoDouble* pRange
    );

    void UpdateRangeDateValues( 
        FdoIConnection* connection,
        FdoString* pClassName, 
        FdoString* pIdName, 
        FdoInt32 idValue
    );

    void UpdateRangeSingleValues( 
        FdoIConnection* connection,
        FdoString* pClassName, 
        FdoString* pIdName, 
        FdoInt32 idValue
    );

    void UpdateRangeStringValues( 
        FdoIConnection* connection,
        FdoString* pClassName, 
        FdoString* pIdName, 
        FdoInt32 idValue
    );

    void UpdateRangeValues( 
        FdoIConnection* connection,
        FdoString* pClassName, 
        FdoString* pIdName, 
        FdoInt32 idValue, 
        FdoString* pPropName,
        FdoDataValueCollection* goodValues,
        FdoDataValueCollection* badValues
    );

    void UpdateListDoubleValues( 
        FdoIConnection* connection,
        FdoString* pClassName, 
        FdoString* pIdName, 
        FdoInt32 idValue, 
        FdoString* pPropName,
        FdoDataType dataType
    );

    void UpdateListDateValues( 
        FdoIConnection* connection,
        FdoString* pClassName, 
        FdoString* pIdName, 
        FdoInt32 idValue
    );

    void UpdateValue( 
        FdoIConnection* connection,
        FdoString* pClassName, 
        FdoString* pIdName,
        FdoInt32 featId,
        FdoString* pPropName,
        FdoDataValue* pValue,
        bool expectedSuccess
    );

    FdoStringP	FixDatetimeFormat( FdoDataValue*  val );
    
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

            if ( val->GetDataType() != FdoDataType_DateTime ) 
                CPPUNIT_ASSERT_MESSAGE( "Wrong type for datetime constraint returned", false);

            for ( int k = 0; k < masterCount && !valMatched; k++ ) {
		        valMatched = ( FixDatetimeFormat(val) == pMaster[k] );
	        }	

            CPPUNIT_ASSERT_MESSAGE( 
                (const char*) FdoStringP::Format( L"Wrong List Value %ls", pPropName ),
                valMatched
            );

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

            newRangeConstrR->SetMaxInclusive(maxInclusive);
            if ( pRangeCount > 1 ) {
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

    template< class T> void UpdateRangeIntegralValues( 
        FdoIConnection* connection,
        FdoString* pClassName, 
        FdoString* pIdName, 
        FdoInt32 idValue, 
        FdoString* pPropName,
        T* pRange
    )
    {
        FdoPtr<FdoDataValueCollection> badValues = FdoDataValueCollection::Create();
        FdoPtr<FdoDataValueCollection> goodValues = FdoDataValueCollection::Create();

        badValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create((T)(pRange[0] - 1))) );
        badValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create((T) pRange[1])) );
        goodValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create((T) pRange[0])) );
        goodValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create((T)(pRange[1] - 1))) );

        UpdateRangeValues( connection, pClassName, pIdName, idValue, pPropName, goodValues, badValues );
    }

    template< class T> void UpdateListValues( 
        FdoIConnection* connection,
        FdoString* pClassName, 
        FdoString* pIdName, 
        FdoInt32 idValue, 
        FdoString* pPropName,
        T goodValue,
        T badValue
    )
    {
        UpdateValue( 
            connection, 
            pClassName, 
            pIdName, 
            idValue, 
            pPropName, 
            FdoPtr<FdoDataValue>( FdoDataValue::Create(goodValue) ), 
            true 
        );

        UpdateValue( 
            connection, 
            pClassName, 
            pIdName, 
            idValue, 
            pPropName, 
            FdoPtr<FdoDataValue>( FdoDataValue::Create(badValue) ), 
            false 
        );
    }
};

#endif	//TESTCOMMONCONSTRAINTS_H
