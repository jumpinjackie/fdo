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

#include "Pch.h"
#include "FdoConstraintsTest.h"
#include "UnitTestUtil.h"
#include "Parse/Parse.h"

#define			DB_NAME_CONSTRAINTS_SUFFIX "_constraints"

#define			SCHEMA_NAME			L"constraints"
#define			CLASS_NAME			L"cdataclass"	// lower case to compensate for MySQl on Linux
#define			CLASS_NAME_BASE		L"CDataBaseClass"
#define			PROPERTY_1			L"IntRange"
#define			PROPERTY_2			L"IntList"
#define			PROPERTY_3			L"StringList"

#define			PROPERTY_4		L"unique1"
#define			PROPERTY_5		L"unique2_1"
#define			PROPERTY_6		L"unique2_2"

#define			NUM_UNIQUE_KEYS			3
#define			NUM_CHECK_CONSTRAINTS	3

#define			INT_MIN_INCLUSIVE	true
#define			INT_MAX_INCLUSIVE	false

static int		INT_RANGE[2]	= {10, 20};
static int		INT_LIST[3]		= {10, 20, 30};
const wchar_t*	STRING_LIST[]	= { L"open", L"close" };

#define			RECREATE_CONSTRAINTS_DB		true

#define DBG(x)		//x
#define DBG_MAX(x)		//x

static FdoPropertyValue* AddNewProperty( FdoPropertyValueCollection* propertyValues, const wchar_t *name )
{
    FdoPropertyValue*  propertyValue = propertyValues->FindItem( name );

    if ( !propertyValue ) 
    {
        propertyValue =  FdoPropertyValue::Create();
        propertyValue->SetName( name );
        propertyValues->Add( propertyValue );
    }
    
    return propertyValue;
}

FdoConstraintsTest::FdoConstraintsTest(void)
{
}

FdoConstraintsTest::~FdoConstraintsTest(void)
{
}

void FdoConstraintsTest::setUp ()
{
	set_provider();
}

void FdoConstraintsTest::TestCreateConstraints ()
{
	FdoPtr<FdoIConnection> connection;
    StaticConnection* staticConn = NULL;
    FdoStringP provider = UnitTestUtil::GetEnv("provider","Oracle");

	try {
		// delete, re-create and open the datastore
		printf( "Initializing Connection ... \n" );
		connection = UnitTestUtil::CreateConnection(
			RECREATE_CONSTRAINTS_DB? true : false,
			RECREATE_CONSTRAINTS_DB? true : false,
            DB_NAME_CONSTRAINTS_SUFFIX,
            NULL,
            NULL,
            0
		);

		if ( !RECREATE_CONSTRAINTS_DB )	{
			try {
				// Drop the schema
				FdoPtr<FdoIDestroySchema>  pCmd = (FdoIDestroySchema*) connection->CreateCommand(FdoCommandType_DestroySchema);
				pCmd->SetSchemaName( SCHEMA_NAME );
				pCmd->Execute();
			} catch (...) {
				// ignore
			}
		}

 		printf( "Creating Constraints Schema ... \n" );
		CreateConstraintsSchema( connection );

        printf( "Closing Connection ... \n" );
		UnitTestUtil::CloseConnection(
			connection,
			false,
            DB_NAME_CONSTRAINTS_SUFFIX
		);

	}
	catch ( FdoException* e ) 
	{
		try {
			if ( connection) connection->Close(); 
		}
		catch ( ... ) 
		{
		}
		UnitTestUtil::FailOnException( e );
	}
   	catch (...)
   	{
        try {
		    if (connection) connection->Close(); 
        }
        catch ( ... ) 
		{
		}
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}
}

void FdoConstraintsTest::TestDescribeConstraints ()
{
	FdoPtr<FdoIConnection> connection;
    StaticConnection* staticConn = NULL;
    FdoStringP provider = UnitTestUtil::GetEnv("provider","Oracle");

	try
	{
		printf( "Initializing Connection ... \n" );
		connection = UnitTestUtil::CreateConnection(
			false,
			false,
            DB_NAME_CONSTRAINTS_SUFFIX,
            NULL,
            NULL,
            0
		);

		printf( "Describe Constraints Schema class %ls... \n", (FdoString *)CLASS_NAME_BASE );
		DescribeConstraintsSchema( connection, CLASS_NAME_BASE, NUM_UNIQUE_KEYS, NUM_CHECK_CONSTRAINTS );

		printf( "Describe Constraints Schema class %ls... \n", (FdoString *)CLASS_NAME );
		DescribeConstraintsSchema( connection, CLASS_NAME, 0, 0 );

        printf( "Closing Connection ... \n" );
		UnitTestUtil::CloseConnection(
			connection,
			false,
            DB_NAME_CONSTRAINTS_SUFFIX
		);

        connection = NULL;
    }
	catch ( FdoException* e ) 
	{
		try {
			if ( connection) connection->Close(); 
		}
		catch ( ... ) 
		{
		}
		UnitTestUtil::FailOnException( e );
	}
	catch ( CppUnit::Exception e ) 
	{
		if (connection) connection->Close(); 
		throw;
	}
   	catch (...)
   	{
        try {
		    if (connection) connection->Close(); 
        }
        catch ( ... ) 
		{
		}
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}
}

void FdoConstraintsTest::TestUpdateCheckConstraints ()
{
	FdoPtr<FdoIConnection> connection;
    StaticConnection* staticConn = NULL;
    FdoStringP provider = UnitTestUtil::GetEnv("provider","Oracle");

	try
	{
		printf( "Initializing Connection ... \n" );
		connection = UnitTestUtil::CreateConnection(
			false,
			false,
            DB_NAME_CONSTRAINTS_SUFFIX,
            "oracle",
            NULL,
            0
		);

		printf( "Describe Constraints Schema ... \n" );
		UpdateCheckConstraints( connection );

        printf( "Closing Connection ... \n" );
		UnitTestUtil::CloseConnection(
			connection,
			false,
            DB_NAME_CONSTRAINTS_SUFFIX
		);

        connection = NULL;
    }
	catch ( FdoException* e ) 
	{
		UnitTestUtil::FailOnException( e );
	}
	catch ( CppUnit::Exception e ) 
	{
		throw;
	}
   	catch (...)
   	{
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}
}

void FdoConstraintsTest::TestUpdateUniqueConstraints ()
{
	FdoPtr<FdoIConnection> connection;
    StaticConnection* staticConn = NULL;
    FdoStringP provider = UnitTestUtil::GetEnv("provider","Oracle");

	try
	{
		printf( "Initializing Connection ... \n" );
		connection = UnitTestUtil::CreateConnection(
			false,
			false,
            DB_NAME_CONSTRAINTS_SUFFIX,
            "oracle",
            NULL,
            0
		);

		printf( "Describe Constraints Schema ... \n" );
		UpdateUniqueConstraints( connection );

        printf( "Closing Connection ... \n" );
		UnitTestUtil::CloseConnection(
			connection,
			false,
            DB_NAME_CONSTRAINTS_SUFFIX
		);

        connection = NULL;
    }
	catch ( FdoException* e ) 
	{
		UnitTestUtil::FailOnException( e );
	}
	catch ( CppUnit::Exception e ) 
	{
		throw;
	}
   	catch (...)
   	{
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}
}

void FdoConstraintsTest::CreateConstraintsSchema( FdoIConnection* connection )
{
	FdoPtr<FdoISchemaCapabilities>	schemaCap = connection->GetSchemaCapabilities();

	FdoPtr<FdoIApplySchema>			pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	FdoFeatureSchemasP pSchemas = FdoFeatureSchemaCollection::Create(NULL);

	FdoPtr<FdoFeatureSchema> pSchema = FdoFeatureSchema::Create( SCHEMA_NAME, L"AutoCAD schema" );
	pSchemas->Add( pSchema );

	FdoPtr<FdoFeatureClass> pCData = FdoFeatureClass::Create( CLASS_NAME_BASE, L"Constraits" );
	pCData->SetIsAbstract(true);

	FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
	FdoPropertiesP(pCData->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pCData->GetIdentityProperties())->Add( pProp );

	//////////////  1st property ///////////////
	FdoPtr<FdoDataPropertyDefinition> pRangeInt = FdoDataPropertyDefinition::Create( PROPERTY_1, L"" );
	pRangeInt->SetDataType( FdoDataType_Int32 );
	pRangeInt->SetNullable(false);

	// Add CHECK constraint
 	if ( schemaCap->SupportsExclusiveValueRangeConstraints() && schemaCap->SupportsInclusiveValueRangeConstraints())
	{
		FdoPtr<FdoPropertyValueConstraintRange>  newRangeConstr1 =  FdoPropertyValueConstraintRange::Create();
		newRangeConstr1->SetMinInclusive(INT_MIN_INCLUSIVE);

		FdoPtr<FdoDataValue>   val1 = FdoDataValue::Create( INT_RANGE[0] );
		newRangeConstr1->SetMinValue( val1 );

		newRangeConstr1->SetMaxInclusive(INT_MAX_INCLUSIVE);
		FdoPtr<FdoDataValue>   val2 = FdoDataValue::Create( INT_RANGE[1] );
		newRangeConstr1->SetMaxValue( val2 );
		pRangeInt->SetValueConstraint(newRangeConstr1);
	}
	FdoPropertiesP(pCData->GetProperties())->Add( pRangeInt  );

	//////////////  2nd property ///////////////
	FdoPtr<FdoDataPropertyDefinition> pListInt = FdoDataPropertyDefinition::Create( PROPERTY_2, L"" );
	pListInt->SetDataType( FdoDataType_Int32 );
	pListInt->SetNullable(false);

	if ( schemaCap->SupportsValueConstraintsList() )
	{
		// Add CHECK constraint
		FdoPtr<FdoPropertyValueConstraintList>  newListConstr2 =  FdoPropertyValueConstraintList::Create();
		FdoPtr<FdoDataValueCollection> list2 = newListConstr2->GetConstraintList();

		FdoPtr<FdoDataValue>   val3 = FdoDataValue::Create( INT_LIST[0] );
		list2->Add( val3 );
		FdoPtr<FdoDataValue>   val4 = FdoDataValue::Create( INT_LIST[1] );
		list2->Add( val4 );
		FdoPtr<FdoDataValue>   val5 = FdoDataValue::Create( INT_LIST[2] );
		list2->Add( val5 );
		pListInt->SetValueConstraint(newListConstr2);
	}
	FdoPropertiesP(pCData->GetProperties())->Add( pListInt  );

	//////////////  3rd property ///////////////
	FdoPtr<FdoDataPropertyDefinition> pListString = FdoDataPropertyDefinition::Create( PROPERTY_3, L"" );
	pListString->SetDataType( FdoDataType_String );
	pListString->SetLength(10);
	pListString->SetNullable(false);

	if ( schemaCap->SupportsValueConstraintsList() )
	{
		// Add CHECK constraint
		FdoPtr<FdoPropertyValueConstraintList>  newListConstr3 =  FdoPropertyValueConstraintList::Create();
		FdoPtr<FdoDataValueCollection> list3 = newListConstr3->GetConstraintList();

		FdoPtr<FdoDataValue>   val31 = FdoDataValue::Create( STRING_LIST[0]);
		list3->Add( val31 );
		FdoPtr<FdoDataValue>   val32 = FdoDataValue::Create( STRING_LIST[1] );
		list3->Add( val32 );
		pListString->SetValueConstraint(newListConstr3);
	}
	FdoPropertiesP(pCData->GetProperties())->Add( pListString  );

	//// Add the class to schema
	FdoClassesP(pSchema->GetClasses())->Add( pCData );

	// Add UNIQUE constraints - actually all 3 providers are the same, supporting both
	if ( schemaCap->SupportsUniqueValueConstraints() && schemaCap->SupportsCompositeUniqueValueConstraints() )
	{
		FdoClassDefinitionP pClass = FdoClassesP( pSchema->GetClasses() )->GetItem( CLASS_NAME_BASE );
		FdoPtr<FdoUniqueConstraintCollection> constraints = pClass->GetUniqueConstraints();

		//////////////  1st unique property - single ///////////////
		FdoPtr<FdoDataPropertyDefinition> pUnique1Int = FdoDataPropertyDefinition::Create( PROPERTY_4, L"" );
		pUnique1Int->SetDataType( FdoDataType_Int32 );
		pUnique1Int->SetNullable(true);
		FdoPropertiesP(pCData->GetProperties())->Add( pUnique1Int  );

		FdoPtr<FdoUniqueConstraint>  newUniqueConstr1 = FdoUniqueConstraint::Create();
		newUniqueConstr1->GetProperties()->Add( pUnique1Int );
		constraints->Add( newUniqueConstr1 );

		//////////////  2nd unique property - Composite ///////////////
		FdoPtr<FdoDataPropertyDefinition> pUnique21Int = FdoDataPropertyDefinition::Create( PROPERTY_5, L"" );
		pUnique21Int->SetDataType( FdoDataType_Int32 );
		pUnique21Int->SetNullable(true);
		FdoPropertiesP(pCData->GetProperties())->Add( pUnique21Int  );

		FdoPtr<FdoDataPropertyDefinition> pUnique22Int = FdoDataPropertyDefinition::Create( PROPERTY_6, L"" );
		pUnique22Int->SetDataType( FdoDataType_Int32 );
		pUnique22Int->SetNullable(true);
		FdoPropertiesP(pCData->GetProperties())->Add( pUnique22Int  );

		FdoPtr<FdoUniqueConstraint>  newUniqueConstr2 = FdoUniqueConstraint::Create();
		newUniqueConstr2->GetProperties()->Add( pUnique21Int );
		newUniqueConstr2->GetProperties()->Add( pUnique22Int );
		constraints->Add( newUniqueConstr2 );

		// Create a new class based on the previous ...
		FdoPtr<FdoFeatureClass> pCDataDerived = FdoFeatureClass::Create( CLASS_NAME, L"Constraits" );
		pCDataDerived->SetBaseClass( pCData );
		pCDataDerived->SetIsAbstract(false);

		//// Add the class to schema
		FdoClassesP(pSchema->GetClasses())->Add( pCDataDerived );
	}

	///////// Done.
	pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();

	//////////////////////////////// Test UNIQUE ////////////////////////////////////////

	// This should succeed

	FdoStringP	insertSql2 = FdoStringP::Format(L"insert into %ls (classid, revisionnumber, %ls, %ls, %ls, %ls) values ", 
					CLASS_NAME, PROPERTY_1, PROPERTY_2, PROPERTY_3, PROPERTY_4 );

	FdoStringP	insertSql = FdoStringP::Format(L"%ls ( 0, 0, 10, 20, 'open', 1000 )", (FdoString *) insertSql2 );

	// FIX for ORACLE FEATID!!!
	try {
		UnitTestUtil::Sql2Db( insertSql, connection );
	} catch (FdoException *ex) {
		ex->Release();
	}

	bool	uniqueSuccess1 = true;
	try {
		FdoStringP	insertSql = FdoStringP::Format(L"%ls ( 0, 0, 10, 20, 'open', 1000 )", (FdoString *) insertSql2 );
		UnitTestUtil::Sql2Db( insertSql, connection );
	} catch (FdoException *ex) {
		DBG(printf("Expected unique constraint violation exception: %ls", (FdoString* )ex->GetExceptionMessage()));
		ex->Release();
		uniqueSuccess1 = false;
	}
	CPPUNIT_ASSERT_MESSAGE("Should get unique constraint violation prop #4", uniqueSuccess1 == false );

	// This should succeed
	insertSql2 = FdoStringP::Format(L"insert into %ls (classid, revisionnumber, %ls, %ls, %ls, %ls, %ls) values ", 
					CLASS_NAME, PROPERTY_1, PROPERTY_2, PROPERTY_3, PROPERTY_5, PROPERTY_6 );

	insertSql = FdoStringP::Format(L"%ls ( 0, 0, 10, 20, 'open', 1000, 2000 )", (FdoString *) insertSql2 );

	// FIX for ORACLE FEATID!!!
	try {
		UnitTestUtil::Sql2Db( insertSql, connection );
	} catch (FdoException *ex) {
		ex->Release();
	}
	
	bool	uniqueSuccess2 = true;
	try {
		FdoStringP	insertSql = FdoStringP::Format(L"%ls ( 0, 0, 10, 20, 'open', 1000, 2000)", (FdoString *) insertSql2 );
		UnitTestUtil::Sql2Db( insertSql, connection );
	} catch (FdoException *ex) {
		DBG(printf("Expected unique constraint violationexception: %ls", (FdoString* )ex->GetExceptionMessage()));
		ex->Release();
		uniqueSuccess2 = false;
	}
	CPPUNIT_ASSERT_MESSAGE("Should get unique constraint violation prop #[5,6]", uniqueSuccess2 == false );

	//////////////////////////////// Test CHECK ////////////////////////////////////////

	if ( schemaCap->SupportsValueConstraintsList() )
	{

		FdoStringP	insertSql1 = FdoStringP::Format(L"insert into %ls (classid, revisionnumber, %ls, %ls, %ls) values ", 
						CLASS_NAME, PROPERTY_1, PROPERTY_2, PROPERTY_3 );

		bool	checkSuccess = true;
		try {
			FdoStringP	insertSql = FdoStringP::Format(L"%ls (0, 0, 22, 20, 'open' )", (FdoString *) insertSql1 );
			UnitTestUtil::Sql2Db( insertSql, connection );
		} catch (FdoException *ex) {
			DBG(printf("Expected RangeInt constraint exception: %ls", (FdoString* )ex->GetExceptionMessage()));
			ex->Release();
			checkSuccess = false;
		}
		CPPUNIT_ASSERT_MESSAGE("Should get check constraint violation prop #1", checkSuccess == false );

		checkSuccess = true;
		try {
			FdoStringP	insertSql = FdoStringP::Format(L"%ls ( 0, 0, 10, 40, 'open' )", (FdoString *) insertSql1 );
			UnitTestUtil::Sql2Db( insertSql, connection );
		} catch (FdoException *ex) {
			DBG(printf("Expected ListInt constraint exception: %ls", (FdoString* )ex->GetExceptionMessage()));
			ex->Release();
			checkSuccess = false;
		}
		CPPUNIT_ASSERT_MESSAGE("Should get check constraint violation prop #2", checkSuccess == false );

		checkSuccess = true;
		try {
			FdoStringP	insertSql = FdoStringP::Format(L"%ls ( 0, 0, 10, 20, 'xxxx' )", (FdoString *) insertSql1 );
			UnitTestUtil::Sql2Db( insertSql, connection );
		} catch (FdoException *ex) {
			DBG(printf("Expected ListString constraint exception: %ls", (FdoString* )ex->GetExceptionMessage()));
			ex->Release();
			checkSuccess = false;
		}
		CPPUNIT_ASSERT_MESSAGE("Should get check constraint violation prop #3", checkSuccess == false );
	}
}

void FdoConstraintsTest::TestParser()
{
	TestParser(L"([INTLIST] = 30 or ([INTLIST] = 20 or [INTLIST] = 10))");
	TestParser(L"([INTRANGE] >= 10 and [INTRANGE] < 20))");
	TestParser(L"([STRINGLIST] = 'close' or [STRINGLIST] = 'open')");

	TestParser(L"(INTLIST = 30 or (INTLIST = 20 or INTLIST = 10))");
	TestParser(L"(INTRANGE >= 10 and INTRANGE < 20))");
	TestParser(L"(STRINGLIST = 'close' or STRINGLIST = 'open')");

	TestParser(L"INTLIST = 30 or INTLIST = 20 or INTLIST = 10)");
	TestParser(L"INTRANGE >= 10 and INTRANGE < 20");
	TestParser(L"STRINGLIST = 'close' or STRINGLIST = 'open'");

}

void FdoConstraintsTest::TestParser(FdoString* clause)
{
	printf("'%ls'\n", clause );
	try
	{
		FdoCommonParse* parser = new FdoCommonParse();

		parser->ParseConstraint( clause );

		delete parser;

	} catch (FdoException *ex) {
		printf("Parse failed: %ls\n", (FdoString* )ex->GetExceptionMessage());
	}
	printf("\n");
}


void FdoConstraintsTest::DescribeConstraintsSchema( FdoIConnection* connection, FdoString* className, int numUkeys, int numCkeys )
{
	FdoPtr<FdoISchemaCapabilities>	schemaCap = connection->GetSchemaCapabilities();

    FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);

    pDescCmd->SetSchemaName( SCHEMA_NAME );
	FdoPtr<FdoFeatureSchemaCollection> pSchemas2 = pDescCmd->Execute();

	FdoPtr<FdoFeatureSchema> pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
    FdoPtr<FdoClassCollection> pClasses2 = pSchema2->GetClasses();
   	FdoPtr<FdoClassDefinition> pClass2 = pClasses2->GetItem( className );
   	FdoPtr<FdoClassDefinition> pClassBase2 = pClass2->GetBaseClass();

	int count1 = pClass2->GetProperties()->GetCount();
	int count2 = pClass2->GetBaseProperties()->GetCount();

	printf("#props=%ld #baseProps=%ld\n", count1, count2);

	///////////////// UNIQUE() CONSTRAINTS //////////////////////////////////////////
	FdoPtr<FdoUniqueConstraintCollection> pUniqueCs = pClass2->GetUniqueConstraints();

	int		count = 0;
	bool	found_unique1 = false;
	bool	found_unique2 = false;
	bool	found_unique3 = false;

	//if ( pUniqueCs->GetCount() == 0 )
	//	pUniqueCs = pClassBase2->GetUniqueConstraints();

	for ( int i = 0; i < pUniqueCs->GetCount(); i++ ) {
		FdoPtr<FdoUniqueConstraint>  pUniqueC = pUniqueCs->GetItem(i);
		FdoDataPropertiesP			 pProps = pUniqueC->GetProperties();
		
		printf("Unique key #%d:\n", i);

		for ( int j = 0; j < pProps->GetCount(); j++ ) {
			FdoDataPropertyP	pProp = pProps->GetItem(j);

			printf("\t%d %ls\n", j, pProp->GetName());
			count++;
		
			if ( wcscmp(pProp->GetName(), PROPERTY_4 ) == 0 )
				found_unique1 = true;
			else if ( wcscmp(pProp->GetName(), PROPERTY_5 ) == 0 )
				found_unique2 = true;
			else if ( wcscmp(pProp->GetName(), PROPERTY_6 ) == 0 )
				found_unique3 = true;
		}	
	}

	CPPUNIT_ASSERT_MESSAGE("Wrong number of unique keys", count == numUkeys );
	if ( numUkeys != 0 )
		CPPUNIT_ASSERT_MESSAGE("Unique keys properties not found", found_unique1 && found_unique2 && found_unique3);

	///////////////// CHECK() CONSTRAINTS //////////////////////////////////////////

	/// If not supported (say MySql) return.
	if ( !schemaCap->SupportsValueConstraintsList() || 
		 !schemaCap->SupportsExclusiveValueRangeConstraints() || 
		 !schemaCap->SupportsInclusiveValueRangeConstraints())
		return;

	count = 0;

	FdoPropertiesP	pProps = (FdoPropertyDefinitionCollection *)pClass2->GetProperties();
	for ( int i = 0; i < pProps->GetCount(); i++ ) {
		FdoPtr<FdoDataPropertyDefinition>	pProp = (FdoDataPropertyDefinition *)pProps->GetItem(i);

		if (pProp == NULL )
			continue;

		FdoPtr<FdoPropertyValueConstraint>		pConstr = pProp->GetValueConstraint();

		if (pConstr == NULL )
			continue;

		if ( pConstr->GetConstraintType() == FdoPropertyValueConstraintType_Range )
		{
			DBG(printf("Check Range key #%d: (%ls >= %ld and %ls <= %ld)\n", count, pProp->GetName(), INT_RANGE[0], pProp->GetName(), INT_RANGE[1] ));
			
			FdoPropertyValueConstraintRange*		pConstrR = (FdoPropertyValueConstraintRange*)(pConstr.p);

			CPPUNIT_ASSERT_MESSAGE("Wrong range property", (wcscmp( pProp->GetName(), PROPERTY_1 ) == 0));
			CPPUNIT_ASSERT_MESSAGE("Wrong MinInclusive", pConstrR->GetMinInclusive() == INT_MIN_INCLUSIVE );
			CPPUNIT_ASSERT_MESSAGE("Wrong MaxInclusive", pConstrR->GetMaxInclusive() == INT_MAX_INCLUSIVE );

			FdoDataValue*	valMin = pConstrR->GetMinValue();
			FdoDataValue*	valMax = pConstrR->GetMaxValue();

			FdoPtr<FdoDataValue>   val1 = FdoDataValue::Create( INT_RANGE[0] );
			FdoPtr<FdoDataValue>   val2 = FdoDataValue::Create( INT_RANGE[1] );

			CPPUNIT_ASSERT_MESSAGE("Wrong Min Value Prop1",  wcscmp(valMin->ToString(), val1->ToString()) == 0 );
			CPPUNIT_ASSERT_MESSAGE("Wrong Max Value Prop1",  wcscmp(valMax->ToString(), val2->ToString()) == 0 );
			
			count++;
		}
		else if (pConstr->GetConstraintType() == FdoPropertyValueConstraintType_List)
		{
			// NOTE: SQL SErver is reversing the order of items in the list. Therefore we scan 
			// the entire list for a match.
			FdoPropertyValueConstraintList*		pConstrL = (FdoPropertyValueConstraintList*)(pConstr.p);
			FdoPtr<FdoDataValueCollection>		pList = pConstrL->GetConstraintList();

			if ( wcscmp( pProp->GetName(), PROPERTY_2 ) == 0) {
				
				DBG(printf("Check List key #%d: (%ls in (", count, pProp->GetName()));

				for ( int j = 0; j < pList->GetCount(); j++ ) {
					FdoPtr<FdoDataValue>	val = pList->GetItem(j);
					bool					valMatched = false;

					for ( int k = 0; k < sizeof(INT_LIST) && !valMatched; k++ ) {
						FdoPtr<FdoDataValue>   val1 = FdoDataValue::Create( INT_LIST[k] );
						valMatched = ( wcscmp(val->ToString(), val1->ToString()) == 0 );
					}	
					CPPUNIT_ASSERT_MESSAGE("Wrong List Value Prop2",  valMatched );
					DBG(printf("%ls,", val->ToString()));
				}
				DBG(printf("))\n"));
				count++;
			}
			else if ( wcscmp( pProp->GetName(), PROPERTY_3 ) == 0) {
				DBG(printf("Check List key #%d: (%ls in (", count, pProp->GetName()));

				for ( int j = 0; j < pList->GetCount(); j++ ) {
					FdoPtr<FdoDataValue>	val = pList->GetItem(j);
					bool					valMatched = false;
					
					for ( int k = 0; k < sizeof(INT_LIST) && !valMatched; k++ ) {
						FdoPtr<FdoDataValue>   val1 = FdoDataValue::Create( STRING_LIST[k] );
						valMatched = ( wcscmp(val->ToString(), val1->ToString() ) == 0 );
					}
					CPPUNIT_ASSERT_MESSAGE("Wrong List Value Prop3",  valMatched );
					DBG(printf("%ls,", val->ToString()));
				}
				DBG(printf("))\n"));
				count++;
			}
		}
	}
	CPPUNIT_ASSERT_MESSAGE("Wrong number of CHECK constraints", count == numCkeys );
}

void FdoConstraintsTest::UpdateUniqueConstraints( FdoIConnection* connection )
{
	int		count = 0;
	bool	found_unique1 = false;
	bool	found_unique2 = false;
	bool	found_unique3 = false;

	printf(".UpdateUniqueConstraints ...\n");

    FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);

    pDescCmd->SetSchemaName( SCHEMA_NAME );
	FdoPtr<FdoFeatureSchemaCollection> pSchemas2 = pDescCmd->Execute();
	FdoPtr<FdoFeatureSchema> pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
    FdoPtr<FdoClassCollection> pClasses2 = pSchema2->GetClasses();
	FdoPtr<FdoClassDefinition> pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
   
	///////////////// UPDATE UNIQUE() CONSTRAINTS //////////////////////////////////////////
	
	FdoPtr<FdoUniqueConstraintCollection> pUniqueCs = pClass2->GetUniqueConstraints();

	FdoPtr<FdoIApplySchema>  pApplyCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
	pApplyCmd->SetFeatureSchema(pSchema2);

	FdoPtr<FdoISchemaCapabilities>	schemaCap = connection->GetSchemaCapabilities();
	bool isMySQL = 	!schemaCap->SupportsValueConstraintsList();

	/// MySql reverses the order of the constraints and also the constrainted props in the list
	int		index1 = isMySQL? 0 : 1;
	int		index2 = isMySQL? 1 : 0;

	//////// Remove the unique key on PROPERTY_4
	FdoPtr<FdoUniqueConstraint>  pUniqueC1 = pUniqueCs->GetItem(index1);
	FdoDataPropertiesP			 pProps1 = pUniqueC1->GetProperties();
	FdoDataPropertyP			 pProp1 = pProps1->GetItem(0);
	printf("Unique key on (%ls) removed\n", PROPERTY_4);

	pUniqueCs->RemoveAt(index1);
	//pProps1->RemoveAt(0); 
	pApplyCmd->Execute();

	// Just one constraint left
	index2 = 0;

	///////// Combined ukey (PROPERTY_5, PROPERTY_6). Update by removing (PROPERTY_5)
	FdoPtr<FdoUniqueConstraint>  pUniqueC0 = pUniqueCs->GetItem(index2);
	FdoDataPropertiesP			 pProps0 = pUniqueC0->GetProperties();
	FdoDataPropertyP			 pProp0 = pProps0->GetItem(0);
	printf("Unique key on (%ls, %ls) updated to (%ls)\n", PROPERTY_5, PROPERTY_6, PROPERTY_6);

	pProps0->RemoveAt( isMySQL? 1 : 0 );
	//pUniqueCs->RemoveAt(index2);
	pApplyCmd->Execute();


	// Add some duplicate data. It should succeed because the constraints on PROPERTY_4 and PROPERTY_5 were removed above
	FdoStringP	insertSql2 = FdoStringP::Format(L"insert into %ls (classid, revisionnumber, %ls, %ls, %ls, %ls, %ls, %ls) values ", 
					CLASS_NAME, PROPERTY_1, PROPERTY_2, PROPERTY_3, PROPERTY_4, PROPERTY_5, PROPERTY_6 );

	long   value = 7777;
	FdoStringP	insertSql = FdoStringP::Format(L"%ls ( 0, 0, 10, 20, 'open', 1000, 1000, %ld )", (FdoString *) insertSql2, value );
	UnitTestUtil::Sql2Db( insertSql, connection );

	// Restore the uniqueness of PROPERTY_4
	FdoPtr<FdoDataPropertyDefinition> pUnique4 = FdoDataPropertyDefinition::Create( PROPERTY_4, L"" );
	FdoPtr<FdoUniqueConstraint>  newUniqueConstr4 = FdoUniqueConstraint::Create();
	newUniqueConstr4->GetProperties()->Add( pUnique4 );
	pUniqueCs->Add( newUniqueConstr4 );

	bool error = false;
	try	{
		pApplyCmd->Execute();
	} catch (FdoException *ex) {
		DBG(printf("Expected unique constraint violation exception: %ls\n", (FdoString* )ex->GetExceptionMessage()));
		ex->Release();
		error = true;
	}
	CPPUNIT_ASSERT_MESSAGE("Expected unique constraint violation exception on PROPERTY_4", error == true);

	// Remove the offending row
	FdoStringP	delSql = FdoStringP::Format(L"delete from %ls where %ls=%ld", CLASS_NAME, PROPERTY_6, value );
	UnitTestUtil::Sql2Db( delSql, connection );

	// Try again.
	pApplyCmd->Execute();

	// Do a new describe schema
	pSchemas2 = pDescCmd->Execute();
	pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
    pClasses2 = pSchema2->GetClasses();
	pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
  	pUniqueCs = pClass2->GetUniqueConstraints();

	// Check results
	for ( int i = 0; i < pUniqueCs->GetCount(); i++ ) {
		FdoPtr<FdoUniqueConstraint>  pUniqueC = pUniqueCs->GetItem(i);
		FdoDataPropertiesP			 pProps = pUniqueC->GetProperties();

		for ( int j = 0; j < pProps->GetCount(); j++ ) {
			FdoDataPropertyP	pProp = pProps->GetItem(j);

			printf("\t%d %ls\n", j, pProp->GetName());
			count++;

			if ( wcscmp(pProp->GetName(), PROPERTY_4 ) == 0 )
				found_unique1 = true;
			else if ( wcscmp(pProp->GetName(), PROPERTY_5 ) == 0 )
				found_unique2 = true;
			else if ( wcscmp(pProp->GetName(), PROPERTY_6 ) == 0 )
				found_unique3 = true;
		}	
	}
	
	CPPUNIT_ASSERT_MESSAGE("Wrong number of unique keys", count == NUM_UNIQUE_KEYS - 1);
	CPPUNIT_ASSERT_MESSAGE("Unique keys properties not found", found_unique1 && !found_unique2 && found_unique3);

}

void FdoConstraintsTest::UpdateCheckConstraints( FdoIConnection* connection )
{
	int		count = 0;
	bool	found_unique1 = false;
	bool	found_unique2 = false;
	bool	found_unique3 = false;

	FdoPtr<FdoISchemaCapabilities>	schemaCap = connection->GetSchemaCapabilities();

	// MySql doesn't have check constraints
	if ( !schemaCap->SupportsValueConstraintsList() )
		return;

	printf(".UpdateCheckConstraints ...\n");

    FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);

    pDescCmd->SetSchemaName( SCHEMA_NAME );
	FdoPtr<FdoFeatureSchemaCollection> pSchemas2 = pDescCmd->Execute();
	FdoPtr<FdoFeatureSchema> pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
    FdoPtr<FdoClassCollection> pClasses2 = pSchema2->GetClasses();
	FdoPtr<FdoClassDefinition> pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
   
	FdoPtr<FdoIApplySchema>  pApplyCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
	pApplyCmd->SetFeatureSchema(pSchema2);

	///////////////// CHECK() CONSTRAINTS //////////////////////////////////////////
	FdoPtr<FdoPropertyDefinitionCollection> pProps = pClass2->GetProperties();

	FdoStringP	insertSql1 = FdoStringP::Format(L"insert into %ls (classid, revisionnumber, %ls, %ls, %ls, %ls, %ls) values ", 
								CLASS_NAME, PROPERTY_1, PROPERTY_2, PROPERTY_3, PROPERTY_4, PROPERTY_6 );

	for ( int i = 0; i < pProps->GetCount(); i++ ) {
		FdoPtr<FdoDataPropertyDefinition>	pProp = (FdoDataPropertyDefinition *)pProps->GetItem(i);

		if (pProp == NULL )
			continue;

		FdoPtr<FdoPropertyValueConstraint>		pConstr = pProp->GetValueConstraint();

		if (pConstr == NULL )
			continue;

		if ( pConstr->GetConstraintType() == FdoPropertyValueConstraintType_Range )
		{
			// Remove the constraint
			pProp->SetValueConstraint( NULL );

			pApplyCmd->Execute();

			// New value to insert into PROPERTY_1, outside range
			// This should succeed since the constraint on PROPERTY_1 was removed
			FdoInt32 value = 2000;
			FdoStringP	insertSql = FdoStringP::Format(L"%ls (0, 0, %ld, 20, 'open', 6666, 8888 )", (FdoString *) insertSql1, value );
			UnitTestUtil::Sql2Db( insertSql, connection );

			// Restore the range CHECK constraint
			FdoPtr<FdoPropertyValueConstraintRange>  newRangeConstr1 =  FdoPropertyValueConstraintRange::Create();
			newRangeConstr1->SetMinInclusive(INT_MIN_INCLUSIVE);

			FdoPtr<FdoDataValue>   val1 = FdoDataValue::Create( INT_RANGE[0] );
			newRangeConstr1->SetMinValue( val1 );

			newRangeConstr1->SetMaxInclusive(INT_MAX_INCLUSIVE);
			FdoPtr<FdoDataValue>   val2 = FdoDataValue::Create( INT_RANGE[1] );
			newRangeConstr1->SetMaxValue( val2 );
			pProp->SetValueConstraint(newRangeConstr1);

			bool error = false;
			try	{
				pApplyCmd->Execute();
			} catch (FdoException *ex) {
				DBG(printf("Expected check constraint violation exception: %ls\n", (FdoString* )ex->GetExceptionMessage()));
				ex->Release();
				error = true;
			}
			CPPUNIT_ASSERT_MESSAGE("Expected check constraint violation exception on PROPERTY_1", error == true);

			// Enlarge the range CHECK constraint
			newRangeConstr1->SetMaxInclusive(INT_MAX_INCLUSIVE);
			val2 = FdoDataValue::Create( value + 1 ); // Note: INT_MAX_INCLUSIVE is FALSE
			newRangeConstr1->SetMaxValue( val2 ); 
			pProp->SetValueConstraint(newRangeConstr1);

			pApplyCmd->Execute();

			// Try insert again. It should succeed.
			insertSql = FdoStringP::Format(L"%ls (0, 0, %ld, 20, 'open', 6667, 8889 )", (FdoString *) insertSql1, value );
			UnitTestUtil::Sql2Db( insertSql, connection );
		}
		else if (pConstr->GetConstraintType() == FdoPropertyValueConstraintType_List)
		{
			if ( wcscmp( pProp->GetName(), PROPERTY_2 ) == 0) {

				// Add a new value to the list.
				FdoPtr<FdoPropertyValueConstraintList>  pConstrList =  (FdoPropertyValueConstraintList *)pConstr.p;
				FdoPtr<FdoDataValueCollection>			pList = pConstrList->GetConstraintList();

				int		newValue = 77;
				FdoPtr<FdoDataValue>   newVal = FdoDataValue::Create( newValue );

				pList->Add( newVal );
				pProp->SetValueConstraint( pConstrList );
				pApplyCmd->Execute();

				// Now try to insert. It should succeed.
				FdoStringP insertSql = FdoStringP::Format(L"%ls (0, 0, 10, %ld, 'open', 6668, 8890 )", (FdoString *) insertSql1, newValue );
				UnitTestUtil::Sql2Db( insertSql, connection );
			}
			else if ( wcscmp( pProp->GetName(), PROPERTY_3 ) == 0) {

				// Replace the 2nd constraint in ('open', 'close') with ('open', 'semi-close');
				FdoPtr<FdoPropertyValueConstraintList>  pConstrList =  (FdoPropertyValueConstraintList *)pConstr.p;
				FdoPtr<FdoDataValueCollection>			pList = pConstrList->GetConstraintList();

				// Note: SqlServer doesn't like constraints like "semi-close" or "semi_close"! 
				// Getting "String or binary data would be truncated" error.
				FdoString*				newValue = L"semiclosed";
				FdoPtr<FdoDataValue>   newVal = FdoDataValue::Create( newValue );
				
				pList->RemoveAt(1);
				pList->Add( newVal );
				pProp->SetValueConstraint( pConstrList );
				pApplyCmd->Execute();

				// Now try to insert. It should succeed.
				FdoStringP insertSql = FdoStringP::Format(L"%ls (0, 0, 10, 10, '%ls', 6669, 8891 )", (FdoString *) insertSql1, newValue );
				UnitTestUtil::Sql2Db( insertSql, connection );
			}
		}
	}

	//DescribeConstraintsSchema( connection, CLASS_NAME_BASE, NUM_UNIQUE_KEYS-1, NUM_CHECK_CONSTRAINTS-1 );
}
