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

#include "stdafx.h"
#include "TestCommonConstraints.h"
#include "TestCommonMiscUtil.h"
#include "Parse/Parse.h"

#ifndef _WIN32
#   define LLONG_MAX    9223372036854775807LL
#   define LLONG_MIN    (-LLONG_MAX - 1LL)
#endif

#define            SCHEMA_NAME            L"constraints"
#define            CLASS_NAME             L"cdataclass"    // lower case to compensate for MySQl on Linux
#define            CLASS_NAME_BASE        L"CDataBaseClass"
#define            CLASS_NAME_SUB        L"CDataSubClass"

#define            PROP_FEATID            L"FeatureId"

#define            PROP_UNIQUE1           L"unique1"
#define            PROP_UNIQUE2_1         L"unique2_1"
#define            PROP_UNIQUE2_2         L"unique2_2"
#define			   PROP_UNIQUE3_1		  L"unique3_1"
#define			   PROP_UNIQUE3_2		  L"unique3_2"
#define			   PROP_UNIQUE3_3		  L"unique3_3"

#define            PROP_BYTE_R            L"ByteRange"
#define            PROP_DATE_R            L"DateRange"
#define            PROP_DECIMAL_R         L"DecimalRange"
#define            PROP_DOUBLE_R          L"DoubleRange"
#define            PROP_INT16_R           L"Int16Range"
#define            PROP_INT32_R           L"Int32Range"
#define            PROP_INT64_R           L"Int64Range"
#define            PROP_SINGLE_R          L"SingleRange"
#define            PROP_STRING_R          L"StringRange"


#define            PROP_BYTE_L            L"ByteList"
#define            PROP_DATE_L            L"DatetimeList"
#define            PROP_DOUBLE_L          L"DoubleList"
#define            PROP_INT32_L           L"Int32List"
#define            PROP_INT64_L           L"Int64List"
#define            PROP_SINGLE_L          L"SingleList"
#define            PROP_STRING_L          L"StringList"
#define            PROP_LSTR_L            L"LargeStringList"

#define            NUM_UNIQUE_KEYS        6

#define            MIN_INCLUSIVE          false
#define            MAX_INCLUSIVE          true

#define            INT_MIN_INCLUSIVE      true
#define            INT_MAX_INCLUSIVE      false

static int         LARGE_STRING_COUNT     = 100;

static FdoByte    BYTE_RANGE[2]           = {11, 22};
static FdoDouble  DOUBLE_RANGE[2]         = {50.123456789012345678901234567890, 100.123456789012345678901234567890};
static FdoInt16   INT16_RANGE[2]          = {-100, 200};
static FdoInt32   INT32_RANGE[2]          = {10, 20};
static FdoInt64   INT64_RANGE[2]          = {100, 200};
static FdoFloat   SINGLE_RANGE[1]         = { (float) 0.000001};
static FdoString* STRING_RANGE[2]         = {L"MOM", L"PA'PA"};
static FdoString* DATETIME_RANGE[2]       = { L"2005-05-15-00-02-01", L"2006-02-01-18-00-00"};

static FdoByte    BYTE_LIST[3]            = {1, 2, 3};
static FdoString* DATETIME_LIST[3]        = {L"2003-10-31-03-02-01", L"2005-10-31-03-02-01", L"2005-10-31-15-02-01"};
static FdoDouble  DOUBLE_LIST[3]          = {0.123456789012345678901234567890, 100, 0.123456789012345678901234567890};
static FdoInt32   INT32_LIST[5]           = {10, 20, 30, LONG_MIN, LONG_MAX};
static FdoInt64   INT64_LIST[4]           = {LLONG_MIN, 52, LLONG_MAX - 1, LLONG_MAX};
static FdoFloat   SINGLE_LIST[3]          = { (float) 0.1234567, (float) 100, (float) 1.12345678};
static FdoString* STRING_LIST[]           = { L"op'en", L"close" };
static wchar_t    LARGE_STRING_LIST[395][20];

static FdoByte    UPD_BYTE_RANGE[2]       = {11, 23};
static FdoInt32   UPD_INT32_RANGE[2]      = {10, 2001};
static FdoInt32   UPD_INT32_LIST[6]       = {10, 20, 30, LONG_MIN, LONG_MAX, 77};
static FdoString* UPD_STRING_LIST[]       = { L"semiclosed", L"close", L"'enclosed'" };


TestCommonConstraints::TestCommonConstraints(void)
{
}

TestCommonConstraints::~TestCommonConstraints(void)
{
}

void TestCommonConstraints::setUp ()
{
}

void TestCommonConstraints::TestCreateConstraints ()
{
    Context context;

    DoTestCreateConstraints( context );
}

void TestCommonConstraints::TestDescribeConstraints ()
{
    Context context;

    DoTestDescribeConstraints( context );
}

void TestCommonConstraints::TestUpdateCheckConstraints ()
{
    Context context;

    DoTestUpdateCheckConstraints( context );
}

void TestCommonConstraints::TestUpdateUniqueConstraints ()
{
    Context context;

    DoTestUpdateUniqueConstraints( context );
}

void TestCommonConstraints::TestDescribeUpdatedConstraints ()
{
    Context context;

    DoTestDescribeUpdatedConstraints( context );
}

void TestCommonConstraints::TestCreateLTConstraints ()
{
    Context context( GetLtMethod() );

    if ( context.ltMethod > 0 ) 
        DoTestCreateConstraints( context );
}

void TestCommonConstraints::TestDescribeLTConstraints ()
{
    Context context( GetLtMethod() );

    if ( context.ltMethod > 0 ) 
        DoTestDescribeConstraints( context );
}

void TestCommonConstraints::TestUpdateLTCheckConstraints ()
{
    Context context( GetLtMethod() );

    if ( context.ltMethod > 0 ) 
        DoTestUpdateCheckConstraints( context );
}

void TestCommonConstraints::TestUpdateLTUniqueConstraints ()
{
    Context context( GetLtMethod() );

    if ( context.ltMethod > 0 ) 
        DoTestUpdateUniqueConstraints( context );
}

void TestCommonConstraints::TestDescribeLTUpdatedConstraints ()
{
    Context context( GetLtMethod() );

    if ( context.ltMethod > 0 ) 
        DoTestDescribeUpdatedConstraints( context );
}

void TestCommonConstraints::DoTestCreateConstraints (Context& context)
{
    try {
        // delete, re-create and open the datastore
        printf( "Initializing Connection ... \n" );
        CreateConnection( context, true );

        printf( "Creating Constraints Schema ... \n" );
        CreateConstraintsSchema( context );
    }
    catch ( FdoException* e ) 
    {
        TestCommonFail( e );
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

void TestCommonConstraints::DoTestDescribeConstraints (Context& context)
{
    try
    {
        printf( "Initializing Connection ... \n" );
        CreateConnection( context );

        printf( "Describe Constraints Schema class %ls... \n", (FdoString *)CLASS_NAME_BASE );
        DescribeConstraintsSchema( context, CLASS_NAME_BASE, NUM_UNIQUE_KEYS, GetExpectedCheckConstraintCount(context.connection), false );

        printf( "Describe Constraints Schema class %ls... \n", (FdoString *)CLASS_NAME );
        DescribeConstraintsSchema( context, CLASS_NAME, 0, 0, false );
    }
    catch ( FdoException* e ) 
    {
        TestCommonFail( e );
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

void TestCommonConstraints::DoTestUpdateCheckConstraints (Context& context)
{
    try
    {
        printf( "Initializing Connection ... \n" );
        CreateConnection( context );

        printf( "Describe Constraints Schema ... \n" );
        UpdateCheckConstraints( context );
    }
    catch ( FdoException* e ) 
    {
        TestCommonFail( e );
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

void TestCommonConstraints::DoTestUpdateUniqueConstraints (Context& context)
{
    try
    {
        printf( "Initializing Connection ... \n" );
        CreateConnection( context );

        printf( "Updating Constraints Schema ... \n" );
        UpdateUniqueConstraints( context );
    }
    catch ( FdoException* e ) 
    {
        TestCommonFail( e );
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

void TestCommonConstraints::DoTestDescribeUpdatedConstraints (Context& context)
{
    try
    {
        printf( "Initializing Connection ... \n" );
        CreateConnection( context );

        printf( "Describe Constraints Schema class %ls... \n", (FdoString *)CLASS_NAME_BASE );

        FdoInt32 NumCheckConstraints = GetExpectedCheckConstraintCount(context.connection);

        DescribeConstraintsSchema( 
            context, 
            CLASS_NAME_BASE, NUM_UNIQUE_KEYS - 4, 
            CanRestrictCheckConstraint() ? NumCheckConstraints : NumCheckConstraints - 1, 
            true 
        );

        printf( "Describe Constraints Schema class %ls... \n", (FdoString *)CLASS_NAME );
        DescribeConstraintsSchema( context, CLASS_NAME, 0, 0, false );
    }
    catch ( FdoException* e ) 
    {
        TestCommonFail( e );
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

void TestCommonConstraints::TestRestrictCheckConstraints ()
{
    if ( FdoValidatesData() ) {
        try
        {
            Context context;

            // delete, re-create and open the datastore
            printf( "Initializing Connection ... \n" );
            CreateConnection( context, true );

             printf( "Creating Constraints Schema ... \n" );
            CreateConstraintsSchema( context );

            printf( "Restrict Constraints  ... \n" );
            RestrictCheckConstraints( context );
        }
        catch ( FdoException* e ) 
        {
            TestCommonFail( e );
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
}

void TestCommonConstraints::TestCheckConstraintsData ()
{
    if ( FdoValidatesData() && CanRestrictCheckConstraint() ) {
        try
        {
            Context context;

            // delete, re-create and open the datastore
            printf( "Initializing Connection ... \n" );
            CreateConnection( context, true );

             printf( "Creating Constraints Schema ... \n" );
            CreateConstraintsSchema( context );

            printf( "Restrict Constraints  ... \n" );
            CheckConstraintsData( context );
        }
        catch ( FdoException* e ) 
        {
            TestCommonFail( e );
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
}

void TestCommonConstraints::TestDateTimeConstraints ()
{
    if ( FdoValidatesData() ) {
        try
        {
            Context context;

            // delete, re-create and open the datastore
            printf( "Initializing Connection ... \n" );
            CreateConnection( context, true );

             printf( "Creating Constraints Schema ... \n" );
            CreateConstraintsSchema( context );

            printf( "Testing DateTime Constraints  ... \n" );
            DateTimeConstraints( context );
        }
        catch ( FdoException* e ) 
        {
            TestCommonFail( e );
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
}

void TestCommonConstraints::TestBaseReferences ()
{
    try
    {
        Context context;

        // delete, re-create and open the datastore
        printf( "Initializing Connection ... \n" );
        CreateConnection( context, true );

        printf( "Creating Constraints Schema ... \n" );
            
        FdoPtr<FdoISchemaCapabilities>    schemaCap = context.connection->GetSchemaCapabilities();

        if ( schemaCap->SupportsUniqueValueConstraints() && schemaCap->SupportsCompositeUniqueValueConstraints() ) {
            FdoPtr<FdoIApplySchema>            pCmd = (FdoIApplySchema*) context.connection->CreateCommand(FdoCommandType_ApplySchema);

            FdoFeatureSchemasP pSchemas = FdoFeatureSchemaCollection::Create(NULL);

            FdoPtr<FdoFeatureSchema> pSchema = FdoFeatureSchema::Create( SCHEMA_NAME, L"AutoCAD schema" );
            pSchemas->Add( pSchema );

            FdoPtr<FdoFeatureClass> pCData = FdoFeatureClass::Create( CLASS_NAME_BASE, L"Constraints" );
            pCData->SetIsAbstract(false);

            FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( PROP_FEATID, L"" );
            pProp->SetDataType( FdoDataType_Int32 );
            pProp->SetNullable(false);
            FdoPropertiesP(pCData->GetProperties())->Add( pProp );
            FdoDataPropertiesP(pCData->GetIdentityProperties())->Add( pProp );

            //// Add the class to schema
            FdoClassesP(pSchema->GetClasses())->Add( pCData );

            FdoClassDefinitionP pClass = FdoClassesP( pSchema->GetClasses() )->GetItem( CLASS_NAME_BASE );

            //////////////  1st unique property - single ///////////////
            FdoPtr<FdoDataPropertyDefinition> pUnique1Int = FdoDataPropertyDefinition::Create( PROP_UNIQUE1, L"" );
            pUnique1Int->SetDataType( FdoDataType_Int32 );
            pUnique1Int->SetNullable(true);
            FdoPropertiesP(pCData->GetProperties())->Add( pUnique1Int  );

            //////////////  2nd unique property - Composite ///////////////
            FdoPtr<FdoDataPropertyDefinition> pUnique21Int = FdoDataPropertyDefinition::Create( PROP_UNIQUE2_1, L"" );
            pUnique21Int->SetDataType( FdoDataType_Int32 );
            pUnique21Int->SetNullable(true);
            FdoPropertiesP(pCData->GetProperties())->Add( pUnique21Int  );

            // Create a new class based on the previous ...
            FdoPtr<FdoFeatureClass> pCDataDerived = FdoFeatureClass::Create( CLASS_NAME, L"Constraits" );
            pCDataDerived->SetBaseClass( pCData );
            pCDataDerived->SetIsAbstract(false);
            FdoPtr<FdoUniqueConstraintCollection> constraints = pCDataDerived->GetUniqueConstraints();

            //// Add the class to schema
            FdoClassesP(pSchema->GetClasses())->Add( pCDataDerived );

            // Create constaint on base property
            FdoPtr<FdoUniqueConstraint>  newUniqueConstr1 = FdoUniqueConstraint::Create();
            FdoPtr<FdoDataPropertyDefinitionCollection> pDataPropColl = newUniqueConstr1->GetProperties();
            pDataPropColl->Add( pUnique1Int );
            constraints->Add( newUniqueConstr1 );

            FdoPtr<FdoDataPropertyDefinition> pUnique22Int = FdoDataPropertyDefinition::Create( PROP_UNIQUE2_2, L"" );
            pUnique22Int->SetDataType( FdoDataType_Int32 );
            pUnique22Int->SetNullable(true);
            FdoPropertiesP(pCDataDerived->GetProperties())->Add( pUnique22Int  );

            // Create composite constraint on property and base property.
            FdoPtr<FdoUniqueConstraint>  newUniqueConstr2 = FdoUniqueConstraint::Create();
            pDataPropColl = newUniqueConstr2->GetProperties();
            pDataPropColl->Add( pUnique21Int );
            pDataPropColl->Add( pUnique22Int );
            constraints->Add( newUniqueConstr2 );

            // Create a new class based on the previous ...
            FdoPtr<FdoFeatureClass> pCDataSubDerived = FdoFeatureClass::Create( CLASS_NAME_SUB, L"Constraits" );
            pCDataSubDerived->SetBaseClass( pCDataDerived);
            pCDataSubDerived->SetIsAbstract(false);
            constraints = pCDataSubDerived->GetUniqueConstraints();

            //// Add the class to schema
            FdoClassesP(pSchema->GetClasses())->Add( pCDataSubDerived );

            // Create duplicate constaint on base property (should disappear on DescribeSchema)
            newUniqueConstr1 = FdoUniqueConstraint::Create();
            pDataPropColl = newUniqueConstr1->GetProperties();
            pDataPropColl->Add( pUnique1Int );
            constraints->Add( newUniqueConstr1 );

            // Create constraint on subset of properties from a base class constraint - OK.
            newUniqueConstr2 = FdoUniqueConstraint::Create();
            pDataPropColl = newUniqueConstr2->GetProperties();
            pDataPropColl->Add( pUnique22Int );
            constraints->Add( newUniqueConstr2 );

            ///////// Done.
            pCmd->SetFeatureSchema( pSchema );
            pCmd->Execute();

            FdoPtr<FdoIInsert> insertCmd;

            // This should succeed
            TestCommonMiscUtil::InsertObject(
                context.connection,
                insertCmd,
                SCHEMA_NAME,
                CLASS_NAME_BASE,
                PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection,CLASS_NAME_BASE),
                PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 1000,
                (FdoString*) NULL
            );

            // Unique constraint not on base class so should succeed.
            TestCommonMiscUtil::InsertObject(
                context.connection,
                insertCmd,
                SCHEMA_NAME,
                CLASS_NAME_BASE,
                PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME_BASE),
                PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 1000,
                (FdoString*) NULL
            );

            // This should succeed
            TestCommonMiscUtil::InsertObject(
                context.connection,
                insertCmd,
                SCHEMA_NAME,
                CLASS_NAME,
                PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
                PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 2000,
                (FdoString*) NULL
            );

            bool    uniqueSuccess1 = true;
            try {
                TestCommonMiscUtil::InsertObject(
                    context.connection,
                    insertCmd,
                    SCHEMA_NAME,
                    CLASS_NAME,
                    PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
                    PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 2000,
                    (FdoString*) NULL
                );
            } catch (FdoException *ex) {
                DBG(printf("Expected unique constraint violation exception: %ls", (FdoString* )ex->GetExceptionMessage()));
                ex->Release();
                uniqueSuccess1 = false;
            }
            CPPUNIT_ASSERT_MESSAGE("Should get unique constraint violation prop #4", uniqueSuccess1 == false );

            // This should succeed
            TestCommonMiscUtil::InsertObject(
                context.connection,
                insertCmd,
                SCHEMA_NAME,
                CLASS_NAME_BASE,
                PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME_BASE),
                PROP_UNIQUE2_1,  FdoDataType_Int32,  (FdoInt32) 1000,
                (FdoString*) NULL
            );

            // Unique constraint not on base class so should succeed.
            TestCommonMiscUtil::InsertObject(
                context.connection,
                insertCmd,
                SCHEMA_NAME,
                CLASS_NAME_BASE,
                PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME_BASE),
                PROP_UNIQUE2_1,  FdoDataType_Int32,  (FdoInt32) 1000,
                (FdoString*) NULL
                );

            // This should succeed
            TestCommonMiscUtil::InsertObject(
                context.connection,
                insertCmd,
                SCHEMA_NAME,
                CLASS_NAME,
                PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
                PROP_UNIQUE2_1,  FdoDataType_Int32,  (FdoInt32) 1000,
                PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 2000,
                (FdoString*) NULL
                );

            bool    uniqueSuccess2 = true;

            try {
                TestCommonMiscUtil::InsertObject(
                    context.connection,
                    insertCmd,
                    SCHEMA_NAME,
                    CLASS_NAME,
                    PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
                    PROP_UNIQUE2_1,  FdoDataType_Int32,  (FdoInt32) 1000,
                    PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 2000,
                    (FdoString*) NULL
                    );

            } catch (FdoException *ex) {
                DBG(printf("Expected unique constraint violationexception: %ls", (FdoString* )ex->GetExceptionMessage()));
                ex->Release();
                uniqueSuccess2 = false;
            }

            CPPUNIT_ASSERT_MESSAGE("Should get unique constraint violation prop #[5,6]", uniqueSuccess2 == false ); 
            
            // This should succeed

            TestCommonMiscUtil::InsertObject(
                context.connection,
                insertCmd,
                SCHEMA_NAME,
                CLASS_NAME,
                PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
                PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 3000,
                PROP_UNIQUE2_1,  FdoDataType_Int32,  (FdoInt32) 2000,
                PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 2000,
                (FdoString*) NULL
                );
           
            // This should succeed

            TestCommonMiscUtil::InsertObject(
                context.connection,
                insertCmd,
                SCHEMA_NAME,
                CLASS_NAME,
                PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
                PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 4000,
                PROP_UNIQUE2_1,  FdoDataType_Int32,  (FdoInt32) 1000,
                PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 3000,
                (FdoString*) NULL
                );
            
            FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) context.connection->CreateCommand(FdoCommandType_DescribeSchema);

            pDescCmd->SetSchemaName( SCHEMA_NAME );
            FdoPtr<FdoFeatureSchemaCollection> pSchemas2 = pDescCmd->Execute();
            FdoPtr<FdoFeatureSchema> pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
            FdoPtr<FdoClassCollection> pClasses2 = pSchema2->GetClasses();
            FdoPtr<FdoClassDefinition> pClass2 = pClasses2->GetItem( CLASS_NAME );
            FdoPtr<FdoClassDefinition> pClassBase2 = pClass2->GetBaseClass();
            FdoPtr<FdoClassDefinition> pClassSub2 = pClasses2->GetItem( CLASS_NAME_SUB );

            ///////////////// UNIQUE() CONSTRAINTS //////////////////////////////////////////
            FdoPtr<FdoUniqueConstraintCollection> pUniqueCs = pClass2->GetUniqueConstraints();

            int     count = 0;
            bool    found_unique1 = false;
            bool    found_unique2 = false;
            bool    found_unique3 = false;

            for ( int i = 0; i < pUniqueCs->GetCount(); i++ ) {
                FdoPtr<FdoUniqueConstraint>  pUniqueC = pUniqueCs->GetItem(i);
                FdoDataPropertiesP             pProps = pUniqueC->GetProperties();
                    
                printf("Unique key #%d:\n", i);

                for ( int j = 0; j < pProps->GetCount(); j++ ) {
                    FdoDataPropertyP    pProp = pProps->GetItem(j);

                    printf("\t%d %ls\n", j, pProp->GetName());
                    count++;
                    
                    if ( wcscmp(pProp->GetName(), PROP_UNIQUE1 ) == 0 )
                        found_unique1 = true;
                    else if ( wcscmp(pProp->GetName(), PROP_UNIQUE2_1 ) == 0 )
                        found_unique2 = true;
                    else if ( wcscmp(pProp->GetName(), PROP_UNIQUE2_2 ) == 0 )
                        found_unique3 = true;
                }    
            }

            CPPUNIT_ASSERT_MESSAGE("Wrong number of unique keys", count == 3 );
            CPPUNIT_ASSERT_MESSAGE("Unique keys properties not found", found_unique1 && found_unique2 && found_unique3);

            // None of the unique constraints on derived class should find their way to the base
            // class.
            pUniqueCs = pClassBase2->GetUniqueConstraints();
            CPPUNIT_ASSERT_MESSAGE("Base class has unique keys", pUniqueCs->GetCount() == 0 );

            // Sub class should have on 1 constraint (one with subset of properties). Duplicate
            // constraint should disappear
            pUniqueCs = pClassSub2->GetUniqueConstraints();
            CPPUNIT_ASSERT_MESSAGE("Sub class does not have 1 unique key", pUniqueCs->GetCount() == 1 );
            FdoPtr<FdoUniqueConstraint>  pUniqueC = pUniqueCs->GetItem(0);
            FdoDataPropertiesP             pProps = pUniqueC->GetProperties();
            CPPUNIT_ASSERT_MESSAGE("Sub class unique key does not have 1 property ", pProps->GetCount() == 1 );
            pProp = pProps->GetItem(0);
            CPPUNIT_ASSERT( wcscmp(pProp->GetName(), PROP_UNIQUE2_2) == 0);
        }
    }
    catch ( FdoException* e ) 
    {
        TestCommonFail( e );
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

void TestCommonConstraints::CreateConstraintsSchema( Context& context )
{
    FdoPtr<FdoISchemaCapabilities>    schemaCap = context.connection->GetSchemaCapabilities();
    FdoInt32                          typeCount;
    FdoInt32                          idx;
    FdoDataType*                      supportedTypes = schemaCap->GetDataTypes( typeCount );

    FdoPtr<FdoIApplySchema>            pCmd = (FdoIApplySchema*) context.connection->CreateCommand(FdoCommandType_ApplySchema);

    FdoFeatureSchemasP pSchemas = FdoFeatureSchemaCollection::Create(NULL);

    FdoPtr<FdoFeatureSchema> pSchema = FdoFeatureSchema::Create( SCHEMA_NAME, L"AutoCAD schema" );
    pSchemas->Add( pSchema );

    FdoPtr<FdoFeatureClass> pCData = FdoFeatureClass::Create( CLASS_NAME_BASE, L"Constraints" );
    pCData->SetIsAbstract(true);

    FdoPtr<FdoDataPropertyDefinition> pIdProp = FdoDataPropertyDefinition::Create( PROP_FEATID, L"" );
    pIdProp->SetDataType( FdoDataType_Int32 );
    pIdProp->SetNullable(false);
    FdoPropertiesP(pCData->GetProperties())->Add( pIdProp );
    FdoDataPropertiesP(pCData->GetIdentityProperties())->Add( pIdProp );

    for ( idx = 0; idx < typeCount; idx++ ) {
        switch ( supportedTypes[idx] ) {
        case FdoDataType_Byte:
            //////////////  Byte range property ///////////////
            AddRangeProperty(
                schemaCap,
                pCData, 
                PROP_BYTE_R, 
                FdoDataType_Byte, 
                BYTE_RANGE, 
                sizeof(BYTE_RANGE) / sizeof(FdoByte),
                INT_MIN_INCLUSIVE,
                INT_MAX_INCLUSIVE
            );

            //////////////  Byte List property ///////////////
            AddListProperty(
                schemaCap,
                pCData, 
                PROP_BYTE_L, 
                FdoDataType_Byte, 
                BYTE_LIST, 
                sizeof(BYTE_LIST) / sizeof(FdoByte)
            );

            break;
        
        case FdoDataType_DateTime:
            {
                //////////////  DateTime Range property ///////////////
                FdoPtr<FdoDataPropertyDefinition> pDatetimeRange9 = FdoDataPropertyDefinition::Create( PROP_DATE_R, L"" );
                pDatetimeRange9->SetDataType( FdoDataType_DateTime );
                pDatetimeRange9->SetNullable(true);

                // Add CHECK constraint
                if ( schemaCap->SupportsExclusiveValueRangeConstraints() && schemaCap->SupportsInclusiveValueRangeConstraints())
                {
                    FdoDateTime dateTime;
                    dateTime.day = 15;
                    dateTime.month = 5;
                    dateTime.year = 2005;
                    dateTime.hour = 0;
                    dateTime.minute = 2;
                    dateTime.seconds = 1;

                    FdoPtr<FdoPropertyValueConstraintRange>  newRangeConstr9R =  FdoPropertyValueConstraintRange::Create();
                    newRangeConstr9R->SetMinInclusive(true);
                    FdoPtr<FdoDataValue>   val9R = FdoDataValue::Create( dateTime );
                    newRangeConstr9R->SetMinValue( val9R );

                    dateTime.day = 1;
                    dateTime.month = 2;
                    dateTime.year = 2006;
                    dateTime.hour = 18;
                    dateTime.minute = 0;
                    dateTime.seconds = 0;

                    newRangeConstr9R->SetMaxInclusive(true);
                    val9R = FdoDataValue::Create( dateTime );
                    newRangeConstr9R->SetMaxValue( val9R );
                    
                    pDatetimeRange9->SetValueConstraint(newRangeConstr9R);
                }

                FdoPropertiesP(pCData->GetProperties())->Add( pDatetimeRange9  );

                //////////////  DateTime List property ///////////////
                FdoPtr<FdoDataPropertyDefinition> pDatetimeList9 = FdoDataPropertyDefinition::Create( PROP_DATE_L, L"" );
                pDatetimeList9->SetDataType( FdoDataType_DateTime );
                pDatetimeList9->SetNullable(true);

                // Add CHECK constraint
                if ( schemaCap->SupportsValueConstraintsList() )
                {
                    FdoPtr<FdoPropertyValueConstraintList>  newListConstr9 =  FdoPropertyValueConstraintList::Create();
                    FdoPtr<FdoDataValueCollection> list9 = newListConstr9->GetConstraintList();

                    FdoDateTime dateTime;
                    dateTime.day = 31;
                    dateTime.month = 10;
                    dateTime.year = 2003;
                    dateTime.hour = 3;
                    dateTime.minute = 2;
                    dateTime.seconds = 1;

                    FdoPtr<FdoDataValue> val39 = FdoDataValue::Create(dateTime);

                    list9->Add( val39 );

                    dateTime.year = 2005;
                    val39 = FdoDataValue::Create(dateTime);
                    list9->Add( val39 );

                    dateTime.hour = 15;
                    val39 = FdoDataValue::Create(dateTime);
                    list9->Add( val39 );

                    pDatetimeList9->SetValueConstraint(newListConstr9);
                }

                FdoPropertiesP(pCData->GetProperties())->Add( pDatetimeList9  );
            }
            break;

        case FdoDataType_Decimal:
            {
                //////////////  Decimal Range property ///////////////
                FdoPtr<FdoDataPropertyDefinition> pProp = AddDoubleRangeProperty(
                    schemaCap,
                    pCData, 
                    PROP_DECIMAL_R, 
                    FdoDataType_Decimal, 
                    DOUBLE_RANGE, 
                    sizeof(DOUBLE_RANGE) / sizeof(FdoDouble),
                    MIN_INCLUSIVE,
                    MAX_INCLUSIVE
                );

                pProp->SetPrecision(8);
                pProp->SetScale(2);
            }
            break;

        case FdoDataType_Double:
            //////////////  Double Range property ///////////////
            AddDoubleRangeProperty(
                schemaCap,
                pCData, 
                PROP_DOUBLE_R, 
                FdoDataType_Double, 
                DOUBLE_RANGE, 
                sizeof(DOUBLE_RANGE) / sizeof(FdoDouble),
                MIN_INCLUSIVE,
                MAX_INCLUSIVE
            );

            //////////////  Double List property ///////////////
            AddDoubleListProperty(
                schemaCap,
                pCData, 
                PROP_DOUBLE_L, 
                FdoDataType_Double, 
                DOUBLE_LIST, 
                sizeof(DOUBLE_LIST) / sizeof(FdoDouble)
            );

            break;

        case FdoDataType_Int16:
            //////////////  Int16 Range property ///////////////
            AddRangeProperty(
                schemaCap,
                pCData, 
                PROP_INT16_R, 
                FdoDataType_Int16, 
                INT16_RANGE, 
                sizeof(INT16_RANGE) / sizeof(FdoInt16),
                INT_MIN_INCLUSIVE,
                INT_MAX_INCLUSIVE
            );

            break;

        case FdoDataType_Int32:
            //////////////  Int32 Range property ///////////////
            AddRangeProperty(
                schemaCap,
                pCData, 
                PROP_INT32_R, 
                FdoDataType_Int32, 
                INT32_RANGE, 
                sizeof(INT32_RANGE) / sizeof(FdoInt32),
                INT_MIN_INCLUSIVE,
                INT_MAX_INCLUSIVE
            );

            //////////////  Int32 List property ///////////////
            AddListProperty(
                schemaCap,
                pCData, 
                PROP_INT32_L, 
                FdoDataType_Int32, 
                INT32_LIST, 
                sizeof(INT32_LIST) / sizeof(FdoInt32)
            );

            break;

        case FdoDataType_Int64:
            AddRangeProperty(
                schemaCap,
                pCData, 
                PROP_INT64_R, 
                FdoDataType_Int64, 
                INT64_RANGE, 
                sizeof(INT64_RANGE) / sizeof(FdoInt64),
                INT_MIN_INCLUSIVE,
                INT_MAX_INCLUSIVE
            );

            //////////////  Int64 List property ///////////////
            AddListProperty(
                schemaCap,
                pCData, 
                PROP_INT64_L, 
                FdoDataType_Int64, 
                INT64_LIST, 
                sizeof(INT64_LIST) / sizeof(FdoInt64)
            );

            break;

        case FdoDataType_Single:
            AddRangeProperty(
                schemaCap,
                pCData, 
                PROP_SINGLE_R, 
                FdoDataType_Single, 
                SINGLE_RANGE, 
                sizeof(SINGLE_RANGE) / sizeof(FdoFloat),
                MIN_INCLUSIVE,
                MAX_INCLUSIVE
            );

            //////////////  Float List property ///////////////
            AddListProperty(
                schemaCap,
                pCData, 
                PROP_SINGLE_L, 
                FdoDataType_Single, 
                SINGLE_LIST, 
                sizeof(SINGLE_LIST) / sizeof(FdoFloat)
            );

            break;

        case FdoDataType_String:
            //////////////  String range property ///////////////
            FdoPtr<FdoDataPropertyDefinition> pRangeString = AddRangeProperty(
                schemaCap,
                pCData, 
                PROP_STRING_R, 
                FdoDataType_String, 
                STRING_RANGE, 
                sizeof(STRING_RANGE) / sizeof(FdoString*),
                MIN_INCLUSIVE,
                MAX_INCLUSIVE
            );

            pRangeString->SetLength(10);

            //////////////  String List property ///////////////
            FdoPtr<FdoDataPropertyDefinition> pListString = AddListProperty(
                schemaCap,
                pCData, 
                PROP_STRING_L, 
                FdoDataType_String, 
                STRING_LIST, 
                sizeof(STRING_LIST) / sizeof(FdoString*)
            );

            pListString->SetLength(10);

            {
                //////////////  Long String List property ///////////////
                FdoPtr<FdoDataPropertyDefinition> pListLargeString = FdoDataPropertyDefinition::Create( PROP_LSTR_L, L"" );
                pListLargeString->SetDataType( FdoDataType_String );
                pListLargeString->SetLength(20);
                pListLargeString->SetNullable(true);

                // Add CHECK constraint
                if ( schemaCap->SupportsValueConstraintsList() )
                {
                    FdoPtr<FdoPropertyValueConstraintList>  newListConstr12 =  FdoPropertyValueConstraintList::Create();
                    FdoPtr<FdoDataValueCollection> list12 = newListConstr12->GetConstraintList();

                    FdoInt32 idx;
                    for ( idx = 0; idx < LARGE_STRING_COUNT; idx++ ) {
                        wcscpy( LARGE_STRING_LIST[idx], FdoStringP::Format( L"%06d", idx ) );
                        FdoPtr<FdoDataValue>   val338 = FdoDataValue::Create( LARGE_STRING_LIST[idx] );
                        list12->Add( val338 );
                    }

                    pListLargeString->SetValueConstraint(newListConstr12);
                }

                FdoPropertiesP(pCData->GetProperties())->Add( pListLargeString  );
            }
            break;
        }
    }

    //// Add the class to schema
    FdoClassesP(pSchema->GetClasses())->Add( pCData );

    // Add UNIQUE constraints - 
    if ( schemaCap->SupportsUniqueValueConstraints() && schemaCap->SupportsCompositeUniqueValueConstraints() )
    {
        FdoClassDefinitionP pClass = FdoClassesP( pSchema->GetClasses() )->GetItem( CLASS_NAME_BASE );
        FdoPtr<FdoUniqueConstraintCollection> constraints = pClass->GetUniqueConstraints();

        //////////////  1st unique property - single ///////////////
        FdoPtr<FdoDataPropertyDefinition> pUnique1Int = FdoDataPropertyDefinition::Create( PROP_UNIQUE1, L"" );
        pUnique1Int->SetDataType( FdoDataType_Int32 );
        pUnique1Int->SetNullable(true);
        FdoPropertiesP(pCData->GetProperties())->Add( pUnique1Int  );

        FdoPtr<FdoUniqueConstraint>  newUniqueConstr1 = FdoUniqueConstraint::Create();
        FdoPtr<FdoDataPropertyDefinitionCollection> pDataPropColl = newUniqueConstr1->GetProperties();
        pDataPropColl->Add( pUnique1Int );
        constraints->Add( newUniqueConstr1 );

        //////////////  2nd unique property - Composite ///////////////
        FdoPtr<FdoDataPropertyDefinition> pUnique21Int = FdoDataPropertyDefinition::Create( PROP_UNIQUE2_1, L"" );
        pUnique21Int->SetDataType( FdoDataType_Int32 );
        pUnique21Int->SetNullable(true);
        FdoPropertiesP(pCData->GetProperties())->Add( pUnique21Int  );

        FdoPtr<FdoDataPropertyDefinition> pUnique22Int = FdoDataPropertyDefinition::Create( PROP_UNIQUE2_2, L"" );
        pUnique22Int->SetDataType( FdoDataType_Int32 );
        pUnique22Int->SetNullable(true);
        FdoPropertiesP(pCData->GetProperties())->Add( pUnique22Int  );

        FdoPtr<FdoUniqueConstraint>  newUniqueConstr2 = FdoUniqueConstraint::Create();
        pDataPropColl = newUniqueConstr2->GetProperties();
        pDataPropColl->Add( pUnique21Int );
        pDataPropColl->Add( pUnique22Int );
        constraints->Add( newUniqueConstr2 );

        //////////////  3rd unique property - Composite ///////////////
	    FdoPtr<FdoDataPropertyDefinition> pUnique31Int = FdoDataPropertyDefinition::Create( PROP_UNIQUE3_1, L"" );
	    pUnique31Int->SetDataType( FdoDataType_Int32 );
	    pUnique31Int->SetNullable(true);
	    FdoPropertiesP(pCData->GetProperties())->Add( pUnique31Int  );

	    FdoPtr<FdoDataPropertyDefinition> pUnique32Int = FdoDataPropertyDefinition::Create( PROP_UNIQUE3_2, L"" );
	    pUnique32Int->SetDataType( FdoDataType_Int32 );
	    pUnique32Int->SetNullable(true);
	    FdoPropertiesP(pCData->GetProperties())->Add( pUnique32Int  );

	    FdoPtr<FdoDataPropertyDefinition> pUnique33Int = FdoDataPropertyDefinition::Create( PROP_UNIQUE3_3, L"" );
	    pUnique33Int->SetDataType( FdoDataType_Int32 );
	    pUnique33Int->SetNullable(true);
	    FdoPropertiesP(pCData->GetProperties())->Add( pUnique33Int  );

	    FdoPtr<FdoUniqueConstraint>  newUniqueConstr3 = FdoUniqueConstraint::Create();
	    newUniqueConstr3->GetProperties()->Add( pUnique31Int );
	    newUniqueConstr3->GetProperties()->Add( pUnique32Int );
	    newUniqueConstr3->GetProperties()->Add( pUnique33Int );
	    constraints->Add( newUniqueConstr3 );

	    //////////////  4th unique constraint - on identity property ///////////////
        //////////////  should be silently ignored                   ///////////////

	    FdoPtr<FdoUniqueConstraint>  newUniqueConstr4 = FdoUniqueConstraint::Create();
	    newUniqueConstr4->GetProperties()->Add( pIdProp );
	    constraints->Add( newUniqueConstr4 );
    }

    // Create a new class based on the previous ...
    FdoPtr<FdoFeatureClass> pCDataDerived = FdoFeatureClass::Create( CLASS_NAME, L"Constraits" );
    pCDataDerived->SetBaseClass( pCData );
    pCDataDerived->SetIsAbstract(false);

    //// Add the class to schema
    FdoClassesP(pSchema->GetClasses())->Add( pCDataDerived );

    ///////// Done.
    pCmd->SetFeatureSchema( pSchema );
    pCmd->Execute();

    FdoPtr<FdoIInsert> insertCmd;

    //////////////////////////////// Test UNIQUE ////////////////////////////////////////

    if ( schemaCap->SupportsUniqueValueConstraints() && schemaCap->SupportsCompositeUniqueValueConstraints() )
    {
        // This should succeed
        TestCommonMiscUtil::InsertObject(
            context.connection,
            insertCmd,
            SCHEMA_NAME,
            CLASS_NAME,
            PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
            PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
            PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
            PROP_STRING_L,  FdoDataType_String, L"op'en",
            PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 1000,
            PROP_STRING_R, FdoDataType_String, L"PA",
            PROP_SINGLE_R, FdoDataType_Single, (FdoFloat) 0.000002,
            (FdoString*) NULL
        );

        bool    uniqueSuccess1 = true;
        try {
            TestCommonMiscUtil::InsertObject(
                context.connection,
                insertCmd,
                SCHEMA_NAME,
                CLASS_NAME,
                PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
                PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 30,
                PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 40,
                PROP_STRING_L,  FdoDataType_String, L"close",
                PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 1000,
               PROP_UNIQUE3_1,  FdoDataType_Int32,  (FdoInt32) 1000,
                (FdoString*) NULL
            );
        } catch (FdoException *ex) {
            DBG(printf("Expected unique constraint violation exception: %ls", (FdoString* )ex->GetExceptionMessage()));
            ex->Release();
            uniqueSuccess1 = false;
        }
        CPPUNIT_ASSERT_MESSAGE("Should get unique constraint violation prop #4", uniqueSuccess1 == false );

        // This should succeed
        TestCommonMiscUtil::InsertObject(
            context.connection,
            insertCmd,
            SCHEMA_NAME,
            CLASS_NAME,
            PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
            PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
            PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
            PROP_STRING_L,  FdoDataType_String, L"op'en",
            PROP_UNIQUE2_1,  FdoDataType_Int32,  (FdoInt32) 1000,
            PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 2000,
            PROP_UNIQUE3_1,  FdoDataType_Int32,  (FdoInt32) 1000,
            (FdoString*) NULL
        );

        bool    uniqueSuccess2 = true;
        try {
            TestCommonMiscUtil::InsertObject(
                context.connection,
                insertCmd,
                SCHEMA_NAME,
                CLASS_NAME,
                PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
                PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 30,
                PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 40,
                PROP_STRING_L,  FdoDataType_String, L"close",
                PROP_UNIQUE2_1,  FdoDataType_Int32,  (FdoInt32) 1000,
                PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 2000,
                PROP_UNIQUE3_1,  FdoDataType_Int32,  (FdoInt32) 2000,
                 (FdoString*) NULL
            );
        } catch (FdoException *ex) {
            DBG(printf("Expected unique constraint violationexception: %ls", (FdoString* )ex->GetExceptionMessage()));
            ex->Release();
            uniqueSuccess2 = false;
        }
        CPPUNIT_ASSERT_MESSAGE("Should get unique constraint violation prop #[5,6]", uniqueSuccess2 == false );
    }

    //////////////////////////////// Test CHECK ////////////////////////////////////////

    if ( schemaCap->SupportsValueConstraintsList() )
    {
        bool    checkSuccess = true;
        try {
            TestCommonMiscUtil::InsertObject(
                context.connection,
                insertCmd,
                SCHEMA_NAME,
                CLASS_NAME,
                PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
                PROP_INT32_R , FdoDataType_Int32,  INT32_RANGE[1],
                PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
                PROP_STRING_L,  FdoDataType_String, L"op'en",
                PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 3000,
                PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 3000,
                PROP_UNIQUE3_1,  FdoDataType_Int32,  (FdoInt32) 3000,
                 (FdoString*) NULL
            );
        } catch (FdoException *ex) {
            DBG(printf("Expected RangeInt constraint exception: %ls", (FdoString* )ex->GetExceptionMessage()));
            ex->Release();
            checkSuccess = false;
        }
        CPPUNIT_ASSERT_MESSAGE("Should get check constraint violation prop #1", checkSuccess == false );

        checkSuccess = true;
        try {
            TestCommonMiscUtil::InsertObject(
                context.connection,
                insertCmd,
                SCHEMA_NAME,
                CLASS_NAME,
                PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
                PROP_INT32_R , FdoDataType_Int32,  INT32_RANGE[0] - 1,
                PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
                PROP_STRING_L,  FdoDataType_String, L"op'en",
                PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 4000,
                PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 4000,
                PROP_UNIQUE3_1,  FdoDataType_Int32,  (FdoInt32) 4000,
                (FdoString*) NULL
            );
        } catch (FdoException *ex) {
            DBG(printf("Expected RangeInt constraint exception: %ls", (FdoString* )ex->GetExceptionMessage()));
            ex->Release();
            checkSuccess = false;
        }
        CPPUNIT_ASSERT_MESSAGE("Should get check constraint violation prop #1", checkSuccess == false );


        checkSuccess = true;
        try {
            TestCommonMiscUtil::InsertObject(
                context.connection,
                insertCmd,
                SCHEMA_NAME,
                CLASS_NAME,
                PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
                PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
                PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 40,
                PROP_STRING_L,  FdoDataType_String, L"op'en",
                PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 5000,
                PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 5000,
                PROP_UNIQUE3_1,  FdoDataType_Int32,  (FdoInt32) 5000,
                (FdoString*) NULL
            );
        } catch (FdoException *ex) {
            DBG(printf("Expected ListInt constraint exception: %ls", (FdoString* )ex->GetExceptionMessage()));
            ex->Release();
            checkSuccess = false;
        }
        CPPUNIT_ASSERT_MESSAGE("Should get check constraint violation prop #2", checkSuccess == false );

        checkSuccess = true;
        try {
            TestCommonMiscUtil::InsertObject(
                context.connection,
                insertCmd,
                SCHEMA_NAME,
                CLASS_NAME,
                PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
                PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
                PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
                PROP_STRING_L,  FdoDataType_String, L"xxxxx",
                PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 6000,
                PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 6000,
                PROP_UNIQUE3_1,  FdoDataType_Int32,  (FdoInt32) 6000,
                (FdoString*) NULL
            );
        } catch (FdoException *ex) {
            DBG(printf("Expected ListString constraint exception: %ls", (FdoString* )ex->GetExceptionMessage()));
            ex->Release();
            checkSuccess = false;
        }
        CPPUNIT_ASSERT_MESSAGE("Should get check constraint violation prop #3", checkSuccess == false );


        TestCommonMiscUtil::InsertObject(
            context.connection,
            insertCmd,
            SCHEMA_NAME,
            CLASS_NAME,
            PROP_FEATID, FdoDataType_Int32, 99999999,
            PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
            PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
            PROP_STRING_L,  FdoDataType_String, L"op'en",
            PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 8000,
            PROP_UNIQUE2_1,  FdoDataType_Int32,  (FdoInt32) 8000,
            PROP_UNIQUE3_1,  FdoDataType_Int32,  (FdoInt32) 8000,
            (FdoString*) NULL
        );

        UpdateAllValues( context.connection, CLASS_NAME, 99999999);

        TestCommonMiscUtil::DeleteObjects( 
            context.connection,
            SCHEMA_NAME,
            CLASS_NAME,
            PROP_FEATID,
            FdoDataType_Int32,
            (FdoInt32) 99999999,
            NULL
        );
    }
}

void TestCommonConstraints::UpdateAllValues(     
    FdoIConnection* connection,
    FdoString* pClassName, 
    FdoInt32 featId 
)
{
    UpdateRangeIntegralValues( connection, pClassName, PROP_FEATID, featId, PROP_BYTE_R, BYTE_RANGE );
    UpdateRangeIntegralValues( connection, pClassName, PROP_FEATID, featId, PROP_INT16_R, INT16_RANGE );
    UpdateRangeIntegralValues( connection, pClassName, PROP_FEATID, featId, PROP_INT32_R, INT32_RANGE );
    UpdateRangeIntegralValues( connection, pClassName, PROP_FEATID, featId, PROP_INT64_R, INT64_RANGE );
    UpdateRangeDoubleValues( connection, pClassName, PROP_FEATID, featId, PROP_DOUBLE_R, FdoDataType_Double, DOUBLE_RANGE );
    if ( GetDoubleRounding(FdoDataType_Decimal) == 0 )
        UpdateRangeDoubleValues( connection, pClassName, PROP_FEATID, featId, PROP_DECIMAL_R, FdoDataType_Decimal, DOUBLE_RANGE );
    UpdateRangeSingleValues( connection, pClassName, PROP_FEATID, featId);
    UpdateRangeStringValues( connection, pClassName, PROP_FEATID, featId);
    UpdateRangeDateValues( connection, pClassName, PROP_FEATID, featId );

    UpdateListValues( connection, pClassName, PROP_FEATID, featId, PROP_BYTE_L, BYTE_LIST[1], (FdoByte) 4 );
    UpdateListValues( connection, pClassName, PROP_FEATID, featId, PROP_INT32_L, INT32_LIST[1], (FdoInt32) 500 );
    UpdateListValues( connection, pClassName, PROP_FEATID, featId, PROP_INT64_L, INT64_LIST[1], (FdoInt64) 800 );
    
    if ( GetDoubleRounding(FdoDataType_Decimal) == 0 ) {
        UpdateListDoubleValues( connection, pClassName, PROP_FEATID, featId, PROP_DOUBLE_L, FdoDataType_Double);
        UpdateListValues( connection, pClassName, PROP_FEATID, featId, PROP_SINGLE_L, SINGLE_LIST[1], (FdoFloat) 200.43 );
    }

    UpdateListValues( connection, pClassName, PROP_FEATID, featId, PROP_STRING_L, STRING_LIST[1], L"not_in" );
    UpdateListDateValues( connection, pClassName, PROP_FEATID, featId );

}

void TestCommonConstraints::UpdateRangeDoubleValues( 
    FdoIConnection* connection,
    FdoString* pClassName, 
    FdoString* pIdName, 
    FdoInt32 idValue, 
    FdoString* pPropName,
    FdoDataType dataType, 
    FdoDouble* pRange
)
{
    FdoPtr<FdoDataValueCollection> badValues = FdoDataValueCollection::Create();
    FdoPtr<FdoDataValueCollection> goodValues = FdoDataValueCollection::Create();

    badValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create((FdoDouble)(pRange[0] - GetDoubleRounding(dataType)), dataType)) );
    badValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create((FdoDouble)(pRange[1] + 0.05), dataType)) );
    goodValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create((FdoDouble) (pRange[0] + 0.05), dataType)) );
    goodValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create((FdoDouble)(pRange[1] - GetDoubleRounding(dataType)), dataType)) );

    UpdateRangeValues( connection, pClassName, pIdName, idValue, pPropName, goodValues, badValues );
}

void TestCommonConstraints::UpdateRangeDateValues( 
    FdoIConnection* connection,
    FdoString* pClassName, 
    FdoString* pIdName, 
    FdoInt32 idValue
)
{
    FdoDateTime minDateTime;
    minDateTime.year = 2005;
    minDateTime.month = 5;
    minDateTime.day = 15;
    minDateTime.hour = 0;
    minDateTime.minute = 2;
    minDateTime.seconds = 1;

    FdoDateTime maxDateTime;
    maxDateTime.year = 2006;
    maxDateTime.month = 2;
    maxDateTime.day = 1;
    maxDateTime.hour = 18;
    maxDateTime.minute = 0;
    maxDateTime.seconds = 0;

    FdoDateTime dateTime;

    FdoPtr<FdoDataValueCollection> badValues = FdoDataValueCollection::Create();
    FdoPtr<FdoDataValueCollection> goodValues = FdoDataValueCollection::Create();

    goodValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create(minDateTime)) );
    goodValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create(maxDateTime)) );

    dateTime = minDateTime;
    dateTime.year = dateTime.year - 1;
    dateTime.month = dateTime.month + 1;
    badValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create(dateTime)) );
    dateTime = minDateTime;
    dateTime.month = dateTime.month - 1;
    dateTime.day = dateTime.day + 1;
    badValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create(dateTime)) );
    dateTime = minDateTime;
    dateTime.day = dateTime.day - 1;
    dateTime.hour = dateTime.hour + 1;
    badValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create(dateTime)) );
    dateTime = minDateTime;
    dateTime.minute = dateTime.minute - 1;
    badValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create(dateTime)) );
    dateTime = minDateTime;
    dateTime.minute = dateTime.minute - 1;
    dateTime.seconds = (FdoFloat)(dateTime.seconds + GetSecondsIncrement());
    badValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create(dateTime)) );
    dateTime = minDateTime;
    dateTime.seconds = (FdoFloat)(dateTime.seconds - GetSecondsIncrement());
    badValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create(dateTime)) );

    dateTime = maxDateTime;
    dateTime.year = dateTime.year + 1;
    dateTime.month = dateTime.month - 1;
    badValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create(dateTime)) );
    dateTime = maxDateTime;
    dateTime.month = dateTime.month + 1;
    dateTime.day = dateTime.day - 1;
    badValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create(dateTime)) );
    dateTime = maxDateTime;
    dateTime.day = dateTime.day + 1;
    dateTime.hour = dateTime.hour - 1;
    badValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create(dateTime)) );
    dateTime = maxDateTime;
    dateTime.hour = dateTime.hour + 1;
    dateTime.minute = dateTime.minute - 1;
    badValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create(dateTime)) );
    dateTime = maxDateTime;
    dateTime.minute = dateTime.minute + 1;
    badValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create(dateTime)) );
    dateTime.seconds = (FdoFloat)(dateTime.seconds - GetSecondsIncrement());
    dateTime = maxDateTime;
    dateTime.seconds = (FdoFloat)(dateTime.seconds + GetSecondsIncrement());
    badValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create(dateTime)) );

    UpdateRangeValues( connection, pClassName, pIdName, idValue, PROP_DATE_R, goodValues, badValues );
}

void TestCommonConstraints::UpdateRangeSingleValues( 
    FdoIConnection* connection,
    FdoString* pClassName, 
    FdoString* pIdName, 
    FdoInt32 idValue
)
{
    FdoFloat singleInside = (FdoFloat)(SINGLE_RANGE[0] + 0.000001);

    FdoPtr<FdoDataValueCollection> badValues = FdoDataValueCollection::Create();
    FdoPtr<FdoDataValueCollection> goodValues = FdoDataValueCollection::Create();

    goodValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create(singleInside)) );
    badValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create(SINGLE_RANGE[0])) );

    UpdateRangeValues( connection, pClassName, pIdName, idValue, PROP_SINGLE_R, goodValues, badValues );
}

void TestCommonConstraints::UpdateRangeStringValues( 
    FdoIConnection* connection,
    FdoString* pClassName, 
    FdoString* pIdName, 
    FdoInt32 idValue
)
{
    FdoPtr<FdoDataValueCollection> badValues = FdoDataValueCollection::Create();
    FdoPtr<FdoDataValueCollection> goodValues = FdoDataValueCollection::Create();

    goodValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create(L"MOMA")) );
    goodValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create(STRING_RANGE[1])) );
    badValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create(STRING_RANGE[0])) );
    badValues->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create(L"PAPAA")) );

    UpdateRangeValues( connection, pClassName, pIdName, idValue, PROP_STRING_R, goodValues, badValues );
}

void TestCommonConstraints::UpdateRangeValues( 
    FdoIConnection* connection,
    FdoString* pClassName, 
    FdoString* pIdName, 
    FdoInt32 idValue, 
    FdoString* pPropName,
    FdoDataValueCollection* goodValues,
    FdoDataValueCollection* badValues
)
{
    FdoInt32 idx;
    FdoPtr<FdoDataValue> val;

    for ( idx = 0; idx < goodValues->GetCount(); idx++ ) {
        val = goodValues->GetItem( idx );
        UpdateValue( connection, pClassName, pIdName, idValue, pPropName, val, true );
    }

    for ( idx = 0; idx < badValues->GetCount(); idx++ ) {
        val = badValues->GetItem( idx );
        UpdateValue( connection, pClassName, pIdName, idValue, pPropName, val, false );
    }
}

void TestCommonConstraints::UpdateListDoubleValues( 
    FdoIConnection* connection,
    FdoString* pClassName, 
    FdoString* pIdName, 
    FdoInt32 idValue, 
    FdoString* pPropName,
    FdoDataType dataType
)
{
    UpdateValue( 
        connection, 
        pClassName, 
        pIdName, 
        idValue, 
        pPropName, 
        FdoPtr<FdoDataValue>( FdoDataValue::Create(DOUBLE_LIST[1],dataType) ), 
        true 
    );

    UpdateValue( 
        connection, 
        pClassName, 
        pIdName, 
        idValue, 
        pPropName, 
        FdoPtr<FdoDataValue>( FdoDataValue::Create((FdoDouble)(5000.54),dataType) ), 
        false 
    );
}

void TestCommonConstraints::UpdateListDateValues( 
    FdoIConnection* connection,
    FdoString* pClassName, 
    FdoString* pIdName, 
    FdoInt32 idValue
)
{
    FdoDateTime dateTime;
    dateTime.day = 31;
    dateTime.month = 10;
    dateTime.year = 2003;
    dateTime.hour = 3;
    dateTime.minute = 2;
    dateTime.seconds = 1;

    UpdateValue( 
        connection, 
        pClassName, 
        pIdName, 
        idValue, 
        PROP_DATE_L, 
        FdoPtr<FdoDataValue>( FdoDataValue::Create(dateTime) ), 
        true 
    );

    dateTime.seconds = 55;

    UpdateValue( 
        connection, 
        pClassName, 
        pIdName, 
        idValue, 
        PROP_DATE_L, 
        FdoPtr<FdoDataValue>( FdoDataValue::Create(dateTime) ), 
        false 
    );
}

void TestCommonConstraints::UpdateValue( 
    FdoIConnection* connection,
    FdoString* pClassName, 
    FdoString* pIdName,
    FdoInt32 featId,
    FdoString* pPropName,
    FdoDataValue* pValue,
    bool expectedSuccess
)
{
    bool testSucceeded = true;
    FdoInt32 count = 0;
    FdoStringP errMsg;

    FdoPtr<FdoIUpdate> update = (FdoIUpdate*)connection->CreateCommand(FdoCommandType_Update); 

    update->SetFeatureClassName( pClassName );

    FdoPtr<FdoFilter> filter = FdoFilter::Parse(
        FdoStringP::Format( L"(%ls = %d)", pIdName, featId )
    );

    update->SetFilter(filter);

    FdoPtr<FdoPropertyValue> pv = FdoPropertyValue::Create( pPropName, pValue );
    FdoPtr<FdoPropertyValueCollection> pvc = update->GetPropertyValues();
    pvc->Add(pv);

    try {
        count = update->Execute();
    }
    catch ( FdoException* ex ) {
        testSucceeded = false;
        errMsg = ex->GetExceptionMessage();
        ex->Release();
    }

    if ( expectedSuccess ) {
        CPPUNIT_ASSERT_MESSAGE(
            (const char*) FdoStringP::Format( 
                L"Update %ls.%ls to %ls should have succeeded: %ls",
                pClassName, pPropName, pValue->ToString(), (FdoString*) errMsg
            ),
            testSucceeded == true 
        );

        CPPUNIT_ASSERT_MESSAGE(
            (const char*) FdoStringP::Format( 
                L"Update %ls.%ls to %ls should have updated one feature",
                pClassName, pPropName, pValue->ToString()
            ),
            (testSucceeded == false) || (count == 1)
        );
    }
    else {
        CPPUNIT_ASSERT_MESSAGE(
            (const char*) FdoStringP::Format( 
                L"Update %ls.%ls to %ls should have failed with constraint violation",
                pClassName, pPropName, pValue->ToString()
            ),
            testSucceeded == false 
        );
    }
}

void TestCommonConstraints::DescribeConstraintsSchema( Context& context, FdoString* className, int numUkeys, int numCkeys, bool afterUpdate )
{
    FdoPtr<FdoISchemaCapabilities>    schemaCap = context.connection->GetSchemaCapabilities();

    FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) context.connection->CreateCommand(FdoCommandType_DescribeSchema);

    pDescCmd->SetSchemaName( SCHEMA_NAME );
    FdoPtr<FdoFeatureSchemaCollection> pSchemas2 = pDescCmd->Execute();
    FdoPtr<FdoFeatureSchema> pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
    FdoPtr<FdoClassCollection> pClasses2 = pSchema2->GetClasses();
    FdoPtr<FdoClassDefinition> pClass2 = pClasses2->GetItem( className );
    FdoPtr<FdoClassDefinition> pClassBase2 = pClass2->GetBaseClass();

    int count1 = pClass2->GetProperties()->GetCount();
    int count2 = pClass2->GetBaseProperties()->GetCount();

    printf("#props=%ld #baseProps=%ld\n", count1, count2);

    if ( schemaCap->SupportsUniqueValueConstraints() && schemaCap->SupportsCompositeUniqueValueConstraints() )
    {
        ///////////////// UNIQUE() CONSTRAINTS //////////////////////////////////////////
        FdoPtr<FdoUniqueConstraintCollection> pUniqueCs = pClass2->GetUniqueConstraints();

        int        count = 0;
        bool    found_unique1 = false;
        bool    found_unique2_1 = false;
        bool    found_unique2_2 = false;
        bool    found_unique3_1 = false;
        bool    found_unique3_2 = false;
        bool    found_unique3_3 = false;

        //if ( pUniqueCs->GetCount() == 0 )
        //    pUniqueCs = pClassBase2->GetUniqueConstraints();

        for ( int i = 0; i < pUniqueCs->GetCount(); i++ ) {
            FdoPtr<FdoUniqueConstraint>  pUniqueC = pUniqueCs->GetItem(i);
            FdoDataPropertiesP             pProps = pUniqueC->GetProperties();
            
            printf("Unique key #%d:\n", i);

            for ( int j = 0; j < pProps->GetCount(); j++ ) {
                FdoDataPropertyP    pProp = pProps->GetItem(j);

                printf("\t%d %ls\n", j, pProp->GetName());
                count++;
            
                if ( wcscmp(pProp->GetName(), PROP_UNIQUE1 ) == 0 )
                    found_unique1 = true;
                else if ( wcscmp(pProp->GetName(), PROP_UNIQUE2_1 ) == 0 )
                    found_unique2_1 = true;
                else if ( wcscmp(pProp->GetName(), PROP_UNIQUE2_2 ) == 0 )
                    found_unique2_2 = true;
                else if ( wcscmp(pProp->GetName(), PROP_UNIQUE3_1 ) == 0 )
                    found_unique3_1 = true;
                else if ( wcscmp(pProp->GetName(), PROP_UNIQUE3_2 ) == 0 )
                    found_unique3_2 = true;
                else if ( wcscmp(pProp->GetName(), PROP_UNIQUE3_3 ) == 0 )
                    found_unique3_3 = true;
            }    
        }

        CPPUNIT_ASSERT_MESSAGE("Wrong number of unique keys", count == numUkeys );
        if ( numUkeys != 0 ) {
            if ( afterUpdate ) 
                CPPUNIT_ASSERT_MESSAGE("Unique keys properties not found", found_unique1 && found_unique2_2);
            else
                CPPUNIT_ASSERT_MESSAGE("Unique keys properties not found", found_unique1 && found_unique2_1 && found_unique2_2 && found_unique3_1 && found_unique3_2 && found_unique3_3);
        }
    }

    ///////////////// CHECK() CONSTRAINTS //////////////////////////////////////////

    /// Validate only if check constraints supported
    if ( schemaCap->SupportsValueConstraintsList() &&
         schemaCap->SupportsExclusiveValueRangeConstraints() &&
         schemaCap->SupportsInclusiveValueRangeConstraints())
    {
        FdoInt32 count = 0;

        FdoPropertiesP    pProps = (FdoPropertyDefinitionCollection *)pClass2->GetProperties();
        for ( int i = 0; i < pProps->GetCount(); i++ ) {
            FdoPtr<FdoDataPropertyDefinition>    pProp = (FdoDataPropertyDefinition *)pProps->GetItem(i);

            if (pProp == NULL )
                continue;

            FdoPtr<FdoPropertyValueConstraint>        pConstr = pProp->GetValueConstraint();

            if (pConstr == NULL )
                continue;

            if ( pConstr->GetConstraintType() == FdoPropertyValueConstraintType_Range )
            {
                FdoPropertyValueConstraintRange*        pConstrR = (FdoPropertyValueConstraintRange*)(pConstr.p);

                if ( wcscmp( pProp->GetName(), PROP_INT32_R ) == 0 ) 
                {
                    CPPUNIT_ASSERT_MESSAGE("Wrong MinInclusive", pConstrR->GetMinInclusive() == INT_MIN_INCLUSIVE );
                    CPPUNIT_ASSERT_MESSAGE("Wrong MaxInclusive", pConstrR->GetMaxInclusive() == INT_MAX_INCLUSIVE );

                    FdoPtr<FdoDataValue>    valMin = pConstrR->GetMinValue();
                    FdoPtr<FdoDataValue>    valMax = pConstrR->GetMaxValue();

                    FdoPtr<FdoDataValue>   val1 = FdoDataValue::Create( afterUpdate ? UPD_INT32_RANGE[0] : INT32_RANGE[0] );
                    FdoPtr<FdoDataValue>   val2 = FdoDataValue::Create( afterUpdate ? UPD_INT32_RANGE[1] : INT32_RANGE[1] );

                    CPPUNIT_ASSERT_MESSAGE("Wrong Min Value Prop1R",  wcscmp(valMin->ToString(), val1->ToString()) == 0 );
                    CPPUNIT_ASSERT_MESSAGE("Wrong Max Value Prop1R",  wcscmp(valMax->ToString(), val2->ToString()) == 0 );
                }
                else if ( wcscmp( pProp->GetName(), PROP_BYTE_R) == 0 ) 
                {
                    CPPUNIT_ASSERT_MESSAGE("Wrong MinInclusive", pConstrR->GetMinInclusive() == INT_MIN_INCLUSIVE );
                    CPPUNIT_ASSERT_MESSAGE("Wrong MaxInclusive", pConstrR->GetMaxInclusive() == INT_MAX_INCLUSIVE );

                    FdoPtr<FdoDataValue>    valMin = pConstrR->GetMinValue();
                    FdoPtr<FdoDataValue>    valMax = pConstrR->GetMaxValue();

                    FdoPtr<FdoByteValue>   expMin = FdoByteValue::Create( afterUpdate ? UPD_BYTE_RANGE[0] : BYTE_RANGE[0] );
                    FdoPtr<FdoByteValue>   expMax = FdoByteValue::Create( afterUpdate ? UPD_BYTE_RANGE[1] : BYTE_RANGE[1] );

                    FdoByteValue*    valMin1 = dynamic_cast<FdoByteValue*>(valMin.p);
                    FdoByteValue*    valMax1 = dynamic_cast<FdoByteValue*>(valMax.p);

                    if ( valMin1 == NULL || valMax1 == NULL )
                    {
                        CPPUNIT_ASSERT_MESSAGE("Wrong data type for Prop2R", false);
                    }

                    CPPUNIT_ASSERT_MESSAGE("Wrong Min Value Prop2R",  valMin1->GetByte() == expMin->GetByte() );
                    CPPUNIT_ASSERT_MESSAGE("Wrong Max Value Prop2R",  valMax1->GetByte() == expMax->GetByte() );
                }
                else if ( wcscmp( pProp->GetName(), PROP_STRING_R) == 0 ) 
                {
                    CPPUNIT_ASSERT_MESSAGE("Wrong MinInclusive", pConstrR->GetMinInclusive() == MIN_INCLUSIVE );
                    CPPUNIT_ASSERT_MESSAGE("Wrong MaxInclusive", pConstrR->GetMaxInclusive() == MAX_INCLUSIVE );

                    FdoPtr<FdoDataValue>    valMin = pConstrR->GetMinValue();
                    FdoPtr<FdoDataValue>    valMax = pConstrR->GetMaxValue();

                    FdoPtr<FdoDataValue>   val1 = FdoDataValue::Create( STRING_RANGE[0] );
                    FdoPtr<FdoDataValue>   val2 = FdoDataValue::Create( STRING_RANGE[1] );

                    CPPUNIT_ASSERT_MESSAGE("Wrong Min Value Prop3R",  wcscmp(valMin->ToString(), val1->ToString()) == 0 );
                    CPPUNIT_ASSERT_MESSAGE("Wrong Max Value Prop3R",  wcscmp(valMax->ToString(), val2->ToString()) == 0 );
                }
                else if ( wcscmp( pProp->GetName(), PROP_SINGLE_R) == 0 ) 
                {
                    CPPUNIT_ASSERT_MESSAGE("Wrong MinInclusive", pConstrR->GetMinInclusive() == MIN_INCLUSIVE );

                    FdoPtr<FdoDataValue>    valMin = pConstrR->GetMinValue();
                    FdoPtr<FdoDataValue>    valMax = pConstrR->GetMaxValue();

                    FdoPtr<FdoDataValue>   val1 = FdoDataValue::Create( SINGLE_RANGE[0] );

                    CPPUNIT_ASSERT_MESSAGE("Wrong Min Value Prop4R",  wcscmp(valMin->ToString(), val1->ToString()) == 0 );
                    CPPUNIT_ASSERT_MESSAGE("Wrong Max Value Prop4R (not null)",  valMax == NULL );
                }
			    else if ( wcscmp( pProp->GetName(), PROP_DATE_R) == 0 ) 
			    {
				    CPPUNIT_ASSERT_MESSAGE("Wrong MinInclusive", pConstrR->GetMinInclusive() == true );
				    CPPUNIT_ASSERT_MESSAGE("Wrong MaxInclusive", pConstrR->GetMaxInclusive() == true );

				    FdoDataValue*	valMin = pConstrR->GetMinValue();
				    FdoDataValue*	valMax = pConstrR->GetMaxValue();

				    CPPUNIT_ASSERT_MESSAGE("Wrong Min Value PropDateR",  wcscmp(FixDatetimeFormat(valMin), DATETIME_RANGE[0]) == 0 );
				    CPPUNIT_ASSERT_MESSAGE("Wrong Max Value PropDateR",  wcscmp(FixDatetimeFormat(valMax), DATETIME_RANGE[1]) == 0 );
			    }
                
                count++;
            }
            else if (pConstr->GetConstraintType() == FdoPropertyValueConstraintType_List)
            {
                // NOTE: SQL SErver is reversing the order of items in the list. Therefore we scan 
                // the entire list for a match.
                FdoPropertyValueConstraintList*        pConstrL = (FdoPropertyValueConstraintList*)(pConstr.p);
                FdoPtr<FdoDataValueCollection>        pList = pConstrL->GetConstraintList();

                if ( wcscmp( pProp->GetName(), PROP_INT32_L ) == 0) {
                    DBG(printf("Check List key #%d: (%ls in (", count, pProp->GetName()));
                    CheckListConstraint( 
                        pProp->GetName(), 
                        pList, 
                        afterUpdate ? UPD_INT32_LIST : INT32_LIST, 
                        (afterUpdate ? sizeof(UPD_INT32_LIST) : sizeof(INT32_LIST)) / sizeof(int) 
                    );
                    count++;
                }
                else if ( wcscmp( pProp->GetName(), PROP_STRING_L ) == 0) {
                    DBG(printf("Check List key #%d: (%ls in (", count, pProp->GetName()));
                    CheckListConstraint( 
                        pProp->GetName(), 
                        pList, 
                        afterUpdate ? UPD_STRING_LIST : STRING_LIST, 
                        (afterUpdate ? sizeof(UPD_STRING_LIST) : sizeof(STRING_LIST)) / sizeof(wchar_t*) 
                    );
                    count++;
                }
                else if ( wcscmp( pProp->GetName(), PROP_BYTE_L ) == 0) {
                    DBG(printf("Check List key #%d: (%ls in (", count, pProp->GetName()));
                    CheckListConstraint( pProp->GetName(), pList, BYTE_LIST, sizeof(BYTE_LIST) );
                    count++;
                }
                else if ( wcscmp( pProp->GetName(), PROP_DOUBLE_L ) == 0) {
                    DBG(printf("Check List key #%d: (%ls in (", count, pProp->GetName()));
                    CheckDoubleListConstraint( pProp->GetName(), pList, DOUBLE_LIST, sizeof(DOUBLE_LIST) / sizeof(double), FdoDataType_Double );
                    count++;
                }
                else if ( wcscmp( pProp->GetName(), PROP_DATE_L ) == 0) {
                    DBG(printf("Check List key #%d: (%ls in (", count, pProp->GetName()));
                    CheckDateListConstraint( pProp->GetName(), pList, DATETIME_LIST, sizeof(DATETIME_LIST) / sizeof(wchar_t*) );
                    count++;
                }
                else if ( wcscmp( pProp->GetName(), PROP_INT64_L ) == 0) {
                    DBG(printf("Check List key #%d: (%ls in (", count, pProp->GetName()));
                    CheckListConstraint( pProp->GetName(), pList, INT64_LIST, sizeof(INT64_LIST) / sizeof(FdoInt64) );
                    count++;
                }
                else if ( wcscmp( pProp->GetName(), PROP_SINGLE_L ) == 0) {
                    DBG(printf("Check List key #%d: (%ls in (", count, pProp->GetName()));
                    CheckListConstraint( pProp->GetName(), pList, SINGLE_LIST, sizeof(SINGLE_LIST) / sizeof(float) );
                    count++;
                }
                else if ( wcscmp( pProp->GetName(), PROP_LSTR_L ) == 0) {
                    DBG(printf("Check List key #%d: (%ls in (", count, pProp->GetName()));

                    FdoInt32 idx;
                    for ( idx = 0; idx < LARGE_STRING_COUNT; idx++ ) {
                        wcscpy( LARGE_STRING_LIST[idx], FdoStringP::Format( L"%06d", idx ) );
                    }

                    CheckListConstraint( pProp->GetName(), pList, LARGE_STRING_LIST, LARGE_STRING_COUNT );
                    count++;
                }
            }
        }
        CPPUNIT_ASSERT_MESSAGE("Wrong number of CHECK constraints", count == numCkeys );
    }
}

void TestCommonConstraints::UpdateUniqueConstraints( Context& context )
{
    int        count = 0;
    bool    found_unique1 = false;
    bool    found_unique2 = false;
    bool    found_unique3 = false;

    FdoPtr<FdoISchemaCapabilities>    schemaCap = context.connection->GetSchemaCapabilities();

    if ( schemaCap->SupportsUniqueValueConstraints() && schemaCap->SupportsCompositeUniqueValueConstraints() )
    {
        printf(".UpdateUniqueConstraints ...\n");

        FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) context.connection->CreateCommand(FdoCommandType_DescribeSchema);

        pDescCmd->SetSchemaName( SCHEMA_NAME );
        FdoPtr<FdoFeatureSchemaCollection> pSchemas2 = pDescCmd->Execute();
        FdoPtr<FdoFeatureSchema> pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
        FdoPtr<FdoClassCollection> pClasses2 = pSchema2->GetClasses();
        FdoPtr<FdoClassDefinition> pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
       
        ///////////////// UPDATE UNIQUE() CONSTRAINTS //////////////////////////////////////////
        
        FdoPtr<FdoUniqueConstraintCollection> pUniqueCs = pClass2->GetUniqueConstraints();

        FdoPtr<FdoIApplySchema>  pApplyCmd = (FdoIApplySchema*) context.connection->CreateCommand(FdoCommandType_ApplySchema);
        pApplyCmd->SetFeatureSchema(pSchema2);

        /// MySql reverses the order of the constraints and also the constrainted props in the list
        int        index1;
        int        index2 = -1;
        int        index3 = -1;

        for ( index1 = 0; index1 < pUniqueCs->GetCount(); index1++ ) 
        {
            //////// Remove the unique key on PROP_UNIQUE1
            FdoPtr<FdoUniqueConstraint>  pUniqueC1 = pUniqueCs->GetItem(index1);
            FdoDataPropertiesP             pProps1 = pUniqueC1->GetProperties();
            FdoDataPropertyP             pProp1 = pProps1->FindItem(PROP_UNIQUE1);
            if ( pProp1 ) 
                index2 = index1;
            pProp1 = pProps1->FindItem(PROP_UNIQUE3_1);
            if ( pProp1 ) 
                index3 = index1;
        }

        printf("Unique key on (%ls) removed\n", PROP_UNIQUE1);
        pUniqueCs->RemoveAt(index2);
        if ( index3 > index2 ) 
            index3--;
        pUniqueCs->RemoveAt(index3);
        pApplyCmd->Execute();

        // Just one constraint left
        index2 = 0;

        ///////// Combined ukey (PROP_UNIQUE2_1, PROP_UNIQUE2_2). Update by removing (PROP_UNIQUE2_1)
        FdoPtr<FdoUniqueConstraint>  pUniqueC0 = pUniqueCs->GetItem(index2);
        FdoDataPropertiesP             pProps0 = pUniqueC0->GetProperties();
        printf("Unique key on (%ls, %ls) updated to (%ls)\n", PROP_UNIQUE2_1, PROP_UNIQUE2_2, PROP_UNIQUE2_2);

        pProps0->RemoveAt( pProps0->IndexOf(PROP_UNIQUE2_1) );
        pApplyCmd->Execute();


        // Add some duplicate data. It should succeed because the constraints on PROP_UNIQUE1 and PROP_UNIQUE2_1 were removed above
        FdoStringP    insertSql2 = FdoStringP::Format(L"insert into %ls (classid, revisionnumber, %ls, %ls, %ls, %ls, %ls, %ls) values ", 
                        CLASS_NAME, PROP_INT32_R , PROP_INT32_L, PROP_STRING_L, PROP_UNIQUE1, PROP_UNIQUE2_1, PROP_UNIQUE2_2 );

        FdoInt32   value = 7777;

        FdoPtr<FdoIInsert> insertCmd;
        TestCommonMiscUtil::InsertObject(
            context.connection,
            insertCmd,
            SCHEMA_NAME,
            CLASS_NAME,
            PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
            PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
            PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
            PROP_STRING_L,  FdoDataType_String, L"op'en",
            PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 1000,
            PROP_UNIQUE2_1,  FdoDataType_Int32,  (FdoInt32) 1000,
            PROP_UNIQUE2_2,  FdoDataType_Int32,  value,
            (FdoString*) NULL
        );

        // Restore the uniqueness of PROP_UNIQUE1
        FdoPtr<FdoDataPropertyDefinition> pUnique4 = FdoDataPropertyDefinition::Create( PROP_UNIQUE1, L"" );
        FdoPtr<FdoUniqueConstraint>  newUniqueConstr4 = FdoUniqueConstraint::Create();
        FdoPtr<FdoDataPropertyDefinitionCollection> pDatPropCol = newUniqueConstr4->GetProperties();
        pDatPropCol->Add( pUnique4 );
        pUniqueCs->Add( newUniqueConstr4 );

        bool error = false;
        try    {
            pApplyCmd->Execute();
        } catch (FdoException *ex) {
            DBG(printf("Expected unique constraint violation exception: %ls\n", (FdoString* )ex->GetExceptionMessage()));
            ex->Release();
            error = true;
        }
        CPPUNIT_ASSERT_MESSAGE("Expected unique constraint violation exception on PROP_UNIQUE1", error == true);

        // Remove the offending row
        TestCommonMiscUtil::DeleteObjects( 
            context.connection,
            SCHEMA_NAME,
            CLASS_NAME,
            PROP_UNIQUE2_2,
            FdoDataType_Int32,
            value,
            NULL
        );

        // Try again.
        pApplyCmd->Execute();

        // Do a new describe schema
        pSchemas2 = pDescCmd->Execute();
        pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
        pClasses2 = pSchema2->GetClasses();
        pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
          pUniqueCs = pClass2->GetUniqueConstraints();

	    // Add unique constraint on identity property. Should be ignored.
	    FdoPtr<FdoDataPropertyDefinition> pUnique5 = FdoDataPropertiesP(pClass2->GetIdentityProperties())->GetItem(0);
	    FdoPtr<FdoUniqueConstraint>  newUniqueConstr5 = FdoUniqueConstraint::Create();
	    newUniqueConstr5->GetProperties()->Add( pUnique5 );
	    pUniqueCs->Add( newUniqueConstr5 );

	    pApplyCmd->Execute();

	    pSchemas2 = pDescCmd->Execute();
	    pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
        pClasses2 = pSchema2->GetClasses();
	    pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
	    pUniqueCs = pClass2->GetUniqueConstraints();
        
        // Check results
        for ( int i = 0; i < pUniqueCs->GetCount(); i++ ) {
            FdoPtr<FdoUniqueConstraint>  pUniqueC = pUniqueCs->GetItem(i);
            FdoDataPropertiesP             pProps = pUniqueC->GetProperties();

            for ( int j = 0; j < pProps->GetCount(); j++ ) {
                FdoDataPropertyP    pProp = pProps->GetItem(j);

                printf("\t%d %ls\n", j, pProp->GetName());
                count++;

                if ( wcscmp(pProp->GetName(), PROP_UNIQUE1 ) == 0 )
                    found_unique1 = true;
                else if ( wcscmp(pProp->GetName(), PROP_UNIQUE2_1 ) == 0 )
                    found_unique2 = true;
                else if ( wcscmp(pProp->GetName(), PROP_UNIQUE2_2 ) == 0 )
                    found_unique3 = true;
            }    
        }
        
        CPPUNIT_ASSERT_MESSAGE("Wrong number of unique keys", count == NUM_UNIQUE_KEYS - 4);
        CPPUNIT_ASSERT_MESSAGE("Unique keys properties not found", found_unique1 && !found_unique2 && found_unique3);
    }
}

void TestCommonConstraints::UpdateCheckConstraints( Context& context )
{

    FdoPtr<FdoISchemaCapabilities>    schemaCap = context.connection->GetSchemaCapabilities();
    FdoPtr<FdoIInsert> insertCmd;

    // MySql doesn't have check constraints
    if ( !schemaCap->SupportsValueConstraintsList() )
        return;

    printf(".UpdateCheckConstraints ...\n");

    FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) context.connection->CreateCommand(FdoCommandType_DescribeSchema);

    pDescCmd->SetSchemaName( SCHEMA_NAME );
    FdoPtr<FdoFeatureSchemaCollection> pSchemas2 = pDescCmd->Execute();
    FdoPtr<FdoFeatureSchema> pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
    FdoPtr<FdoClassCollection> pClasses2 = pSchema2->GetClasses();
    FdoPtr<FdoClassDefinition> pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
   
    FdoPtr<FdoIApplySchema>  pApplyCmd = (FdoIApplySchema*) context.connection->CreateCommand(FdoCommandType_ApplySchema);
    pApplyCmd->SetFeatureSchema(pSchema2);

    ///////////////// CHECK() CONSTRAINTS //////////////////////////////////////////
    FdoPtr<FdoPropertyDefinitionCollection> pProps = pClass2->GetProperties();

    for ( int i = 0; i < pProps->GetCount(); i++ ) {
        FdoPtr<FdoDataPropertyDefinition>    pProp = (FdoDataPropertyDefinition *)pProps->GetItem(i);

        if (pProp == NULL )
            continue;

        FdoPtr<FdoPropertyValueConstraint>        pConstr = pProp->GetValueConstraint();

        if (pConstr == NULL )
            continue;

        if ( pConstr->GetConstraintType() == FdoPropertyValueConstraintType_Range )
        {
            if ( wcscmp( pProp->GetName(), PROP_INT32_R  ) == 0) {
                // Remove the constraint
                pProp->SetValueConstraint( NULL );

                pApplyCmd->Execute();

                // New value to insert into PROPERTY_1, outside range
                // This should succeed since the constraint on PROPERTY_1 was removed
                FdoInt32 value = 2000;

                TestCommonMiscUtil::InsertObject(
                    context.connection,
                    insertCmd,
                    SCHEMA_NAME,
                    CLASS_NAME,
                    PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
                    PROP_INT32_R , FdoDataType_Int32,  value,
                    PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
                    PROP_STRING_L,  FdoDataType_String, L"op'en",
                    PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 6666,
                    PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 8888,
                    (FdoString*) NULL
                );

                // Restore the range CHECK constraint
                FdoPtr<FdoPropertyValueConstraintRange>  newRangeConstr1 =  FdoPropertyValueConstraintRange::Create();
                newRangeConstr1->SetMinInclusive(INT_MIN_INCLUSIVE);

                FdoPtr<FdoDataValue>   val1 = FdoDataValue::Create( INT32_RANGE[0] );
                newRangeConstr1->SetMinValue( val1 );

                newRangeConstr1->SetMaxInclusive(INT_MAX_INCLUSIVE);
                FdoPtr<FdoDataValue>   val2 = FdoDataValue::Create( INT32_RANGE[1] );
                newRangeConstr1->SetMaxValue( val2 );
                pProp->SetValueConstraint(newRangeConstr1);

                bool error = false;
                if ( context.ltMethod != 2 ) {
                    try    {
                        pApplyCmd->Execute();
                    } catch (FdoException *ex) {
                        DBG(printf("Expected check constraint violation exception: %ls\n", (FdoString* )ex->GetExceptionMessage()));
                        ex->Release();
                        error = true;
                    }
                    CPPUNIT_ASSERT_MESSAGE("Expected check constraint violation exception on PROPERTY_1", error == true);
                }

                // Enlarge the range CHECK constraint
                newRangeConstr1->SetMaxInclusive(INT_MAX_INCLUSIVE);
                val2 = FdoDataValue::Create( value + 1 ); // Note: INT_MAX_INCLUSIVE is FALSE
                newRangeConstr1->SetMaxValue( val2 ); 
                pProp->SetValueConstraint(newRangeConstr1);

                error = false;
                try    {
                    pApplyCmd->Execute();
                } catch (FdoException *ex) {
                    if ( CanRestrictCheckConstraint() )
                        throw;
                    DBG(printf("Expected check constraint violation exception: %ls\n", (FdoString* )ex->GetExceptionMessage()));
                    ex->Release();
                    error = true;
                    pProp->SetValueConstraint( NULL );
                }
                CPPUNIT_ASSERT_MESSAGE("Expected 2nd check constraint violation exception on PROPERTY_1", error == !CanRestrictCheckConstraint());

                // Try insert again. It should succeed.
                TestCommonMiscUtil::InsertObject(
                    context.connection,
                    insertCmd,
                    SCHEMA_NAME,
                    CLASS_NAME,
                    PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
                    PROP_INT32_R , FdoDataType_Int32,  value,
                    PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
                    PROP_STRING_L,  FdoDataType_String, L"op'en",
                    PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 6667,
                    PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 8889,
                    (FdoString*) NULL
                );
            }
            if ( wcscmp( pProp->GetName(), PROP_BYTE_R ) == 0) {
                // New value to insert into PROPERTY_1, outside range
                // This should fail
                FdoByte value = BYTE_RANGE[1];

                bool error = false;
                try    {
                    TestCommonMiscUtil::InsertObject(
                        context.connection,
                        insertCmd,
                        SCHEMA_NAME,
                        CLASS_NAME,
                        PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
                        PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
                        PROP_BYTE_R, FdoDataType_Byte,  value,
                        PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
                        PROP_STRING_L,  FdoDataType_String, L"op'en",
                        PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 8000,
                        PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 9000,
                        (FdoString*) NULL
                    );
                } catch (FdoException *ex) {
                    DBG(printf("Expected check constraint violation exception: %ls\n", (FdoString* )ex->GetExceptionMessage()));
                    ex->Release();
                    error = true;
                }
                CPPUNIT_ASSERT_MESSAGE("Expected check constraint violation exception on PROP_BYTE_R", error == true);

                // Enlarge the range CHECK constraint
                FdoPropertyValueConstraintRange*  newRangeConstr1 =  static_cast<FdoPropertyValueConstraintRange*>((FdoPropertyValueConstraint*) pConstr);
                FdoPtr<FdoDataValue> newValue = FdoDataValue::Create( UPD_BYTE_RANGE[1] );
                newRangeConstr1->SetMaxValue( newValue );
                pProp->SetValueConstraint(newRangeConstr1);

                pApplyCmd->Execute();

                // Try insert again. It should succeed.
                TestCommonMiscUtil::InsertObject(
                    context.connection,
                    insertCmd,
                    SCHEMA_NAME,
                    CLASS_NAME,
                    PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
                    PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
                    PROP_BYTE_R, FdoDataType_Int32,  value,
                    PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
                    PROP_STRING_L,  FdoDataType_String, L"op'en",
                    PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 6677,
                    PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 8899,
                    (FdoString*) NULL
                );
            }
        }
        else if (pConstr->GetConstraintType() == FdoPropertyValueConstraintType_List)
        {
            if ( wcscmp( pProp->GetName(), PROP_INT32_L ) == 0) {

                // Add a new value to the list.
                FdoPtr<FdoPropertyValueConstraintList>  pConstrList =  FDO_SAFE_ADDREF((FdoPropertyValueConstraintList *)pConstr.p);
                FdoPtr<FdoDataValueCollection>            pList = pConstrList->GetConstraintList();

                FdoInt32        newValue = 77;
                FdoPtr<FdoDataValue>   newVal = FdoDataValue::Create( newValue );

                pList->Add( newVal );
                pProp->SetValueConstraint( pConstrList );
                pApplyCmd->Execute();

                // Now try to insert. It should succeed.
                TestCommonMiscUtil::InsertObject(
                    context.connection,
                    insertCmd,
                    SCHEMA_NAME,
                    CLASS_NAME,
                    PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
                    PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
                    PROP_INT32_L,  FdoDataType_Int32,  newValue,
                    PROP_STRING_L,  FdoDataType_String, L"op'en",
                    PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 6668,
                    PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 8890,
                    (FdoString*) NULL
                );
            }
            else if ( wcscmp( pProp->GetName(), PROP_STRING_L ) == 0) {

                // Replace the 2nd constraint in ('open', 'close') with ('open', 'semi-close');
                FdoPtr<FdoPropertyValueConstraintList>  pConstrList =  FDO_SAFE_ADDREF((FdoPropertyValueConstraintList *)pConstr.p);
                FdoPtr<FdoDataValueCollection>            pList = pConstrList->GetConstraintList();

                // Note: SqlServer doesn't like constraints like "semi-close" or "semi_close"! 
                // Getting "String or binary data would be truncated" error.
                FdoString*              newValue = L"semiclosed";
                FdoPtr<FdoDataValue>    newVal = FdoDataValue::Create( newValue );
				FdoString*				newValue2 = L"'enclosed'";
				FdoPtr<FdoDataValue>    newVal2 = FdoDataValue::Create( newValue2 );
               
                FdoInt32 i;
                for ( i = 0; i < pList->GetCount(); i++ ) 
                {
                    FdoPtr<FdoDataValue> val = pList->GetItem(i);
                    if ( wcscmp(((FdoStringValue*)(FdoDataValue*)val)->GetString(),L"op'en") == 0 ) 
                    {
                        pList->RemoveAt(i);
                        break;
                    }
                }

                pList->Add( newVal );
                pList->Add( newVal2 );
                pProp->SetValueConstraint( pConstrList );
                bool error = false;

                // Following fails because some rows have stringlist='open'
                try    {
                    pApplyCmd->Execute();
                } catch (FdoException *ex) {
                    DBG(printf("Expected check constraint violation exception: %ls\n", (FdoString* )ex->GetExceptionMessage()));
                    ex->Release();
                    error = true;
                }
                CPPUNIT_ASSERT_MESSAGE("Expected check constraint violation exception on PROP_STRING_L", error == true);

                // Now try to delete. It should succeed.
                TestCommonMiscUtil::DeleteObjects( 
                    context.connection,
                    SCHEMA_NAME,
                    CLASS_NAME,
                    PROP_STRING_L,
                    FdoDataType_String,
                    L"op'en",
                    NULL
                );

                // For Oracle Workspace Manager, even deletes against the LIVE long transaction are done conditionally
                // so adding the constraint still fails even if the violating rows are deleted. 
                // Therefore, skip the rest of this test for OWM.
                if ( context.ltMethod != 2 ) {
                    // No "open" stringlist's, ApplySchema should succeed this time.
                    pApplyCmd->Execute();
                    
                    // Now try to insert. It should succeed.
                    TestCommonMiscUtil::InsertObject(
                        context.connection,
                        insertCmd,
                        SCHEMA_NAME,
                        CLASS_NAME,
                        PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
                        PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
                        PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 10,
                        PROP_STRING_L,  FdoDataType_String, newValue,
                        PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 6669,
                        PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 8891,
                        (FdoString*) NULL
                    );

                    TestCommonMiscUtil::InsertObject(
                        context.connection,
                        insertCmd,
                        SCHEMA_NAME,
                        CLASS_NAME,
                        PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
                        PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
                        PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 10,
                        PROP_STRING_L,  FdoDataType_String, newValue,
                        PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 6670,
                        PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 8892,
                        (FdoString*) NULL
                    );
                }
            }
        }
    }
}

void TestCommonConstraints::RestrictCheckConstraints( Context& context )
{
    FdoPtr<FdoISchemaCapabilities>    schemaCap = context.connection->GetSchemaCapabilities();

    if ( schemaCap->SupportsExclusiveValueRangeConstraints() && schemaCap->SupportsInclusiveValueRangeConstraints())
    {
        FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) context.connection->CreateCommand(FdoCommandType_DescribeSchema);

        pDescCmd->SetSchemaName( SCHEMA_NAME );
        FdoPtr<FdoFeatureSchemaCollection> pSchemas2 = pDescCmd->Execute();
        FdoPtr<FdoFeatureSchema> pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
        FdoPtr<FdoClassCollection> pClasses2 = pSchema2->GetClasses();
        FdoPtr<FdoClassDefinition> pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
       
        FdoPtr<FdoIApplySchema>  pApplyCmd = (FdoIApplySchema*) context.connection->CreateCommand(FdoCommandType_ApplySchema);
        pApplyCmd->SetFeatureSchema(pSchema2);

        FdoPtr<FdoIInsert> insertCmd;
        TestCommonMiscUtil::InsertObject(
            context.connection,
            insertCmd,
            SCHEMA_NAME,
            CLASS_NAME,
            PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
            PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
            PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
            PROP_STRING_L,  FdoDataType_String, L"close",
            PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 5000,
            PROP_STRING_R, FdoDataType_String, L"PA",
            (FdoString*) NULL
        );

        FdoInt32 idx;

        printf( "Testing invalid list modifications ... \n" );
        {
            FdoPropertyP prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_BYTE_L );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintList* constrL = (FdoPropertyValueConstraintList*) constr.p;
                FdoPtr<FdoDataValueCollection> listL = constrL->GetConstraintList();
                
                for ( idx = (listL->GetCount() - 1); idx >= 0; idx-- ) {
                    FdoPtr<FdoDataValue> val = listL->GetItem(idx);
                    FdoByteValue* byteVal = (FdoByteValue*) val.p;
                    if ( byteVal->GetByte() == 2 ) 
                        listL->RemoveAt(idx);
                }

                dataProp->SetValueConstraint(constr);
            }

            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT32_L );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintList* constrL = (FdoPropertyValueConstraintList*) constr.p;
                FdoPtr<FdoDataValueCollection> listL = constrL->GetConstraintList();
                
                for ( idx = (listL->GetCount() - 1); idx >= 0; idx-- ) {
                    FdoPtr<FdoDataValue> val = listL->GetItem(idx);
                    FdoInt32Value* int32Val = (FdoInt32Value*) val.p;
                    if ( int32Val->GetInt32() == 30 ) 
                        listL->RemoveAt(idx);
                }

                FdoPtr<FdoInt32Value> newVal = FdoInt32Value::Create(40);
                listL->Add(newVal);

                dataProp->SetValueConstraint(constr);
            }

            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_STRING_L );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraintList> constrL = FdoPropertyValueConstraintList::Create();
                FdoPtr<FdoDataValueCollection> listL = constrL->GetConstraintList();
                listL->Add( FdoPtr<FdoDataValue>(FdoStringValue::Create(L"yes")) );                
                listL->Add( FdoPtr<FdoDataValue>(FdoStringValue::Create(L"no")) );                
                listL->Add( FdoPtr<FdoDataValue>(FdoStringValue::Create(L"maybe")) );                

                dataProp->SetValueConstraint( (FdoPropertyValueConstraint*) constrL.p);
            }

            // Change Range constraint to List (should fail)
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_STRING_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraintList> constrL = FdoPropertyValueConstraintList::Create();
                FdoPtr<FdoDataValueCollection> listL = constrL->GetConstraintList();
                listL->Add( FdoPtr<FdoDataValue>(FdoStringValue::Create(L"on")) );                
                listL->Add( FdoPtr<FdoDataValue>(FdoStringValue::Create(L"off")) );                

                dataProp->SetValueConstraint( (FdoPropertyValueConstraint*) constrL.p);
            }

            // Change List constraint to Range (should fail)
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT64_L );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraintRange> constrR = FdoPropertyValueConstraintRange::Create();
                FdoPtr<FdoDataValue> val = FdoInt64Value::Create( 10LL ); 
                constrR->SetMinValue( val );
                val = FdoInt64Value::Create( 20LL ); 
                constrR->SetMaxValue( val );

                dataProp->SetValueConstraint( (FdoPropertyValueConstraint*) constrR.p);
            }

            FdoStringP messages;
            try {

                pApplyCmd->Execute();
            } catch (FdoException *ex) {
                messages = ex->GetExceptionMessage();
                ex->Release();
            }
            CPPUNIT_ASSERT_MESSAGE("Restrictive list modifications should have failed", messages != L"" );
            
            if ( CanRestrictCheckConstraint() ) {
                CPPUNIT_ASSERT( !messages.Contains(PROP_BYTE_L) );
                CPPUNIT_ASSERT( !messages.Contains(PROP_INT32_L) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_STRING_L) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.StringList'; existing data value 'close' violates new constraint" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_STRING_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.StringRange'; existing data value 'PA' violates new constraint" ) );
                CPPUNIT_ASSERT( !messages.Contains(PROP_INT64_L) );
            }
            else {
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_BYTE_L) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.ByteList'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_INT32_L) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.Int32List'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_STRING_L) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.StringList'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_STRING_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint type for property 'constraints:CDataBaseClass.StringRange'; property has values" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_INT64_L) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint type for property 'constraints:CDataBaseClass.Int64List'; property has values" ) );
            }
        }

        pSchemas2 = pDescCmd->Execute();
        pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
        pClasses2 = pSchema2->GetClasses();
        pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
        pApplyCmd->SetFeatureSchema( pSchema2 );

        // Simple range modifications. Iteration -1 shrinks range (fails), Iteration 1 expands range (succeeds)
        printf( "Testing simple range modifications ... \n" );
        for ( idx = -1; idx < 2; idx += 2 ) 
        {
            FdoPropertyP prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_BYTE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoByteValue* byteVal = (FdoByteValue*) val.p;
                byteVal->SetByte( (FdoByte)(byteVal->GetByte() - idx) );

                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DECIMAL_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMaxValue();
                FdoDecimalValue* decimalVal = (FdoDecimalValue*) val.p;
                decimalVal->SetDecimal( decimalVal->GetDecimal() + (idx * 1e-10) );

                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DATE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMaxValue();
                FdoDateTimeValue* dateVal = (FdoDateTimeValue*) val.p;
                FdoDateTime dateTime = dateVal->GetDateTime();
                if ( idx < 0 ) {
                    dateTime.hour = 17;
                    dateTime.minute = 59;
                    dateTime.seconds = (FdoFloat) 59.99999;
                }
                else {
                    dateTime.seconds = (FdoFloat) 0.00001;
                }
                dateVal->SetDateTime(dateTime);

                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DOUBLE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoDoubleValue* doubleVal = (FdoDoubleValue*) val.p;
                doubleVal->SetDouble( doubleVal->GetDouble() - (idx * 1e-10) );

                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT16_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                FdoPtr<FdoDataValue> val = constrR->GetMaxValue();
                FdoInt16Value* int16Val = (FdoInt16Value*) val.p;
                int16Val->SetInt16( (FdoInt16)(int16Val->GetInt16() + idx) );
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT32_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoInt32Value* int32Val = (FdoInt32Value*) val.p;
                int32Val->SetInt32( int32Val->GetInt32() - idx );
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT64_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                FdoPtr<FdoDataValue> val = constrR->GetMaxValue();
                FdoInt64Value* int64Val = (FdoInt64Value*) val.p;
                int64Val->SetInt64( int64Val->GetInt64() + idx );
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_SINGLE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoSingleValue* singleVal = (FdoSingleValue*) val.p;
                singleVal->SetSingle( (FdoFloat)(singleVal->GetSingle() - (idx * 1e-5)) );

                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_STRING_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoStringValue* stringVal = (FdoStringValue*) val.p;
                stringVal->SetString( (idx < 0) ? L"MOMM" : L"MOL" );
                dataProp->SetValueConstraint(constr);
            }

            if ( idx < 0 ) 
            {
                FdoStringP messages;
                try {
                    pApplyCmd->Execute();
                } catch (FdoException *ex) {
                    messages = ex->GetExceptionMessage();
                    ex->Release();
                }
                CPPUNIT_ASSERT_MESSAGE("Simple restrictive range modifications should have failed", messages != L"" );
                
                if ( CanRestrictCheckConstraint() ) {
                    CPPUNIT_ASSERT( !messages.Contains( PROP_BYTE_R ) );
                    CPPUNIT_ASSERT( !messages.Contains( PROP_DATE_R ) );
                    CPPUNIT_ASSERT( !messages.Contains( PROP_DECIMAL_R ) );
                    CPPUNIT_ASSERT( !messages.Contains( PROP_DOUBLE_R ) );
                    CPPUNIT_ASSERT( !messages.Contains( PROP_INT16_R ) );
                    if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_INT32_R) > -1 )
                        CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.Int32Range'; existing data value '10' violates new constraint" ) );
                    CPPUNIT_ASSERT( !messages.Contains( PROP_INT64_R ) );
                    CPPUNIT_ASSERT( !messages.Contains( PROP_SINGLE_R ) );
                    CPPUNIT_ASSERT( !messages.Contains( PROP_STRING_R ) );
                }
                else {
                    if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_BYTE_R) > -1 )
                        CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.ByteRange'; property has values and new constraint is more restrictive" ) );
                    if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_DATE_R) > -1 )
                        CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.DateRange'; property has values and new constraint is more restrictive" ) );
                    if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_DECIMAL_R) > -1 )
                        CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.DecimalRange'; property has values and new constraint is more restrictive" ) );
                    if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_DOUBLE_R) > -1 )
                        CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.DoubleRange'; property has values and new constraint is more restrictive" ) );
                    if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_INT16_R) > -1 )
                        CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.Int16Range'; property has values and new constraint is more restrictive" ) );
                    if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_INT32_R) > -1 )
                        CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.Int32Range'; property has values and new constraint is more restrictive" ) );
                    if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_INT64_R) > -1 )
                        CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.Int64Range'; property has values and new constraint is more restrictive" ) );
                    if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_SINGLE_R) > -1 )
                        CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.SingleRange'; property has values and new constraint is more restrictive" ) );
                    if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_STRING_R) > -1 )
                        CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.StringRange'; property has values and new constraint is more restrictive" ) );
                }
            }
            else 
            {
                pApplyCmd->Execute();
            }

            pSchemas2 = pDescCmd->Execute();
            pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
            pClasses2 = pSchema2->GetClasses();
            pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
            pApplyCmd->SetFeatureSchema( pSchema2 );
        }

        // Modify both endpoints. Iteration -1 shrinks or shifts range (fails), Iteration 1 expands range (succeeds)
        printf( "Testing both end point modifications ... \n" );
        for ( idx = -1; idx < 2; idx += 2 ) 
        {
            FdoPropertyP prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_BYTE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoByteValue* byteVal = (FdoByteValue*) val.p;
                byteVal->SetByte( (FdoByte)(byteVal->GetByte() - idx) );

                val = constrR->GetMaxValue();
                byteVal = (FdoByteValue*) val.p;
                byteVal->SetByte( (FdoByte)(byteVal->GetByte() + idx) );

                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DOUBLE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                    
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoDoubleValue* doubleVal = (FdoDoubleValue*) val.p;
                if ( idx < 0 ) 
                    doubleVal->SetDouble( doubleVal->GetDouble() + 1e-10 );
                else
                    doubleVal->SetDouble( doubleVal->GetDouble() - 2e-10 );

                val = constrR->GetMaxValue();
                doubleVal = (FdoDoubleValue*) val.p;
                if ( idx < 0 ) 
                    doubleVal->SetDouble( doubleVal->GetDouble() + 1e-10 );

                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT64_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;

                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoInt64Value* int64Val = (FdoInt64Value*) val.p;
                int64Val->SetInt64( int64Val->GetInt64() + idx );

                val = constrR->GetMaxValue();
                int64Val = (FdoInt64Value*) val.p;
                int64Val->SetInt64( int64Val->GetInt64() + idx );

                dataProp->SetValueConstraint(constr);
            }

            if ( (idx <= 0) && (!CanRestrictCheckConstraint()) ) 
            {
                FdoStringP messages;
                try {
                    pApplyCmd->Execute();
                } catch (FdoException *ex) {
                    messages = ex->GetExceptionMessage();
                    ex->Release();
                }
                CPPUNIT_ASSERT_MESSAGE("Restricting both endpoints should have failed", messages != L"" );
                
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_BYTE_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.ByteRange'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_DOUBLE_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.DoubleRange'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_INT64_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.Int64Range'; property has values and new constraint is more restrictive" ) );
            }
            else 
            {
                pApplyCmd->Execute();
            }
        }

        // Tests restricting range by changing inclusive settings.

        printf( "Testing inclusive settings ... \n" );
        FdoPropertyP prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_STRING_R );
        if ( prop ) {
            FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

            FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
            FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
            constrR->SetMaxInclusive(false);
            dataProp->SetValueConstraint(constr);

            FdoStringP messages;
            if ( CanRestrictCheckConstraint() ) {
                pApplyCmd->Execute();
            }
            else {
                try {
                    pApplyCmd->Execute();
                } catch (FdoException *ex) {
                    messages = ex->GetExceptionMessage();
                    ex->Release();
                }
                CPPUNIT_ASSERT_MESSAGE("Setting Maxinclusive false should have failed", messages != L"" );
                CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.StringRange'; property has values and new constraint is more restrictive" ) );
            }

            constrR->SetMaxInclusive(false);
            FdoPtr<FdoDataValue> val = constrR->GetMaxValue();
            FdoStringValue* stringVal = (FdoStringValue*) val.p;
            stringVal->SetString( L"PAPAA" );
            dataProp->SetValueConstraint(constr);
            pApplyCmd->Execute();

            constrR->SetMinInclusive(true);
            constrR->SetMaxInclusive(true);
            dataProp->SetValueConstraint(constr);
            pApplyCmd->Execute();

            constrR->SetMinInclusive(false);
            dataProp->SetValueConstraint(constr);
            if ( CanRestrictCheckConstraint() ) {
                pApplyCmd->Execute();
            }
            else {
                try {
                    pApplyCmd->Execute();
                } catch (FdoException *ex) {
                    messages = ex->GetExceptionMessage();
                    ex->Release();
                }
                CPPUNIT_ASSERT_MESSAGE("Setting ranges to exclusive should have failed.", messages != L"" );
                CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.StringRange'; property has values and new constraint is more restrictive" ) );
                constrR->SetMinInclusive(true);
            }
        }

        prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DOUBLE_R );
        if ( prop ) {
            FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

            FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
            FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
            constrR->SetMinInclusive(true);
            constrR->SetMaxInclusive(false);
            FdoPtr<FdoDataValue> val = constrR->GetMaxValue();
            FdoDoubleValue* doubleVal = (FdoDoubleValue*) val.p;
            doubleVal->SetDouble( doubleVal->GetDouble() + 1e-10 );
            dataProp->SetValueConstraint(constr);

            pApplyCmd->Execute();
        
            constrR->SetMinInclusive(false);
            val = constrR->GetMinValue();
            doubleVal = (FdoDoubleValue*) val.p;
            doubleVal->SetDouble( doubleVal->GetDouble() - 1e-10 );
            dataProp->SetValueConstraint(constr);
            pApplyCmd->Execute();

        }

        // Tests boundary values for numerics
        // Iteration -1 sets range ends to 1 in from boundary values (succeeds)
        // Iteration 0 set range ends to boundary values.            (succeeds)
        // Iteration 1 sets ends back to 1 in from boundaries        (fails)
        // Both Iterations should succeed.

        printf( "Testing boundary values ... \n" );
        for ( idx = -1; idx < 2; idx++ ) 
        {
            FdoPropertyP prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_BYTE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoByteValue* byteVal = (FdoByteValue*) val.p;
                byteVal->SetByte( (FdoByte)(abs(idx)) );

                val = constrR->GetMaxValue();
                byteVal = (FdoByteValue*) val.p;
                byteVal->SetByte( (FdoByte)(UCHAR_MAX - abs(idx)) );

                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DECIMAL_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoDecimalValue* decimalVal = (FdoDecimalValue*) val.p;
                decimalVal->SetDecimal( (FdoDouble)(-1.797693134862315e308 + (abs(idx) * 1e294)) );

                val = constrR->GetMaxValue();
                decimalVal = (FdoDecimalValue*) val.p;
                decimalVal->SetDecimal( (FdoDouble)(1.797693134862315e308 - (abs(idx) * 1e294)) );

                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DOUBLE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoDoubleValue* doubleVal = (FdoDoubleValue*) val.p;
                doubleVal->SetDouble( (FdoDouble)(-1.797693134862315e308 + (abs(idx) * 1e294)) );

                val = constrR->GetMaxValue();
                doubleVal = (FdoDoubleValue*) val.p;
                doubleVal->SetDouble( (FdoDouble)(1.797693134862315e308 - (abs(idx) * 1e294)) );

                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT16_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoInt16Value* int16Val = (FdoInt16Value*) val.p;
                int16Val->SetInt16( (FdoInt16)(SHRT_MIN + abs(idx)) );
                
                val = constrR->GetMaxValue();
                int16Val = (FdoInt16Value*) val.p;
                int16Val->SetInt16( (FdoInt16)(SHRT_MAX - abs(idx)) );
                
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT32_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoInt32Value* int32Val = (FdoInt32Value*) val.p;
                int32Val->SetInt32( LONG_MIN + abs(idx) );
                
                val = constrR->GetMaxValue();
                int32Val = (FdoInt32Value*) val.p;
                int32Val->SetInt32( LONG_MAX - abs(idx) );
                
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT64_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoInt64Value* int64Val = (FdoInt64Value*) val.p;
                int64Val->SetInt64( LLONG_MIN + abs(idx) );
                
                val = constrR->GetMaxValue();
                int64Val = (FdoInt64Value*) val.p;
                int64Val->SetInt64( LLONG_MAX - abs(idx) );
                
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_SINGLE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoSingleValue* singleVal = (FdoSingleValue*) val.p;

                singleVal->SetSingle( (FdoFloat)(-3.4028233e38 + (abs(idx) * 1e32)) );

                dataProp->SetValueConstraint(constr);
            }
                
            if ( (idx == 1) && !CanRestrictCheckConstraint() ) 
            {
                FdoStringP messages;
                try {
                    pApplyCmd->Execute();
                } catch (FdoException *ex) {
                    messages = ex->GetExceptionMessage();
                    ex->Release();
                }
                CPPUNIT_ASSERT_MESSAGE("Restricting maximum ranges should have failed", messages != L"" );
                
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_BYTE_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.ByteRange'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_DECIMAL_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.DecimalRange'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_DOUBLE_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.DoubleRange'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_INT16_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.Int16Range'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_INT32_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.Int32Range'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_INT64_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.Int64Range'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_SINGLE_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.SingleRange'; property has values and new constraint is more restrictive" ) );
            }
            else 
            {
                pApplyCmd->Execute();
            }
        }


        pSchemas2 = pDescCmd->Execute();
        pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
        pClasses2 = pSchema2->GetClasses();
        pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
        pApplyCmd->SetFeatureSchema( pSchema2 );

        // Tests setting range ends to null (succeeds)
        // Iteration 0 tests not-null to null
        // Iteration 1 tests null to null (no change).

        printf( "Testing null endpoints ... \n" );
        for ( idx = 0; idx < 2; idx++ ) 
        {
            FdoPropertyP prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_BYTE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                val->SetNull();
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT16_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                constrR->SetMaxValue((FdoDataValue*) NULL);
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DOUBLE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                FdoPtr<FdoDataValue> val = constrR->GetMaxValue();
                val->SetNull();
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_STRING_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                constrR->SetMinValue((FdoDataValue*) NULL);
                dataProp->SetValueConstraint(constr);
            }
                
            pApplyCmd->Execute();
        }

        // Tests modifying endpoints from null to not null
        // Iteration 0 fails (class as data)
        // Iteration 1 succeeds (class has no data)
        // Iteration 2 succeeds (this tests restricting a range when class has no data).

        printf( "Testing null to not null plus mods without data ... \n" );
        for ( idx = 0; idx < 3; idx++ ) 
        {
            FdoPropertyP prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_BYTE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoByteValue* byteVal = (FdoByteValue*) val.p;
                byteVal->SetByte( (FdoByte)(5 + idx) );
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT16_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                FdoPtr<FdoDataValue> val = FdoDataValue::Create( (FdoInt16) (5000 - idx) );
                constrR->SetMaxValue(val);
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DOUBLE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                FdoPtr<FdoDataValue> val = constrR->GetMaxValue();
                FdoDoubleValue* doubleVal = (FdoDoubleValue*) val.p;
                doubleVal->SetDouble( 20000.12 + idx );
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_STRING_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                FdoPtr<FdoDataValue> val = FdoDataValue::Create( idx > 1 ? L"bbbb" : L"aaaa" );
                constrR->SetMinValue(val);
                dataProp->SetValueConstraint(constr);
            }

            if ( idx == 0 ) 
            {
                FdoStringP messages;
                try {
                    pApplyCmd->Execute();
                } catch (FdoException *ex) {
                    messages = ex->GetExceptionMessage();
                    ex->Release();
                }
                CPPUNIT_ASSERT_MESSAGE("Change null to not null should have failed", messages != L"" );

                if ( CanRestrictCheckConstraint() ) {
                    CPPUNIT_ASSERT( !messages.Contains(PROP_BYTE_R) );
                    CPPUNIT_ASSERT( !messages.Contains(PROP_INT16_R) );
                    CPPUNIT_ASSERT( !messages.Contains(PROP_DOUBLE_R) );
                    if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_STRING_R) > -1 )
                        CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.StringRange'; existing data value 'PA' violates new constraint" ) );
                }
                else {
                    if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_BYTE_R) > -1 )
                        CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.ByteRange'; property has values and new constraint is more restrictive" ) );
                    if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_INT16_R) > -1 )
                        CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.Int16Range'; property has values and new constraint is more restrictive" ) );
                    if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_DOUBLE_R) > -1 )
                        CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.DoubleRange'; property has values and new constraint is more restrictive" ) );
                    if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_STRING_R) > -1 )
                        CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.StringRange'; property has values and new constraint is more restrictive" ) );
                }
            }
            else 
            {
                pApplyCmd->Execute();
            }

            TestCommonMiscUtil::DeleteObjects( 
                context.connection,
                SCHEMA_NAME,
                CLASS_NAME_BASE,
                NULL
            );
        }

    }
}

void TestCommonConstraints::CheckConstraintsData( Context& context )
{
    FdoPtr<FdoISchemaCapabilities>    schemaCap = context.connection->GetSchemaCapabilities();

    if ( schemaCap->SupportsExclusiveValueRangeConstraints() && schemaCap->SupportsInclusiveValueRangeConstraints())
    {
        FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) context.connection->CreateCommand(FdoCommandType_DescribeSchema);

        pDescCmd->SetSchemaName( SCHEMA_NAME );
        FdoPtr<FdoFeatureSchemaCollection> pSchemas2 = pDescCmd->Execute();
        FdoPtr<FdoFeatureSchema> pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
        FdoPtr<FdoClassCollection> pClasses2 = pSchema2->GetClasses();
        FdoPtr<FdoClassDefinition> pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
       
        FdoPtr<FdoIApplySchema>  pApplyCmd = (FdoIApplySchema*) context.connection->CreateCommand(FdoCommandType_ApplySchema);
        pApplyCmd->SetFeatureSchema(pSchema2);

        FdoPtr<FdoIInsert> insertCmd;

        FdoInt32 unique1val = 5500;
        FdoInt32 idx;

        for ( idx = 0; idx < 15; idx++ ) {
            TestCommonMiscUtil::InsertObject(
                context.connection,
                insertCmd,
                SCHEMA_NAME,
                CLASS_NAME,
                PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
                PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 15,
                PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
                PROP_STRING_L,  FdoDataType_String, L"close",
                PROP_UNIQUE1,  FdoDataType_Int32,  (unique1val++),
                PROP_STRING_R, FdoDataType_String, L"PA",
                (FdoString*) NULL
            );
        }

        TestCommonMiscUtil::InsertObject(
            context.connection,
            insertCmd,
            SCHEMA_NAME,
            CLASS_NAME,
            PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
            PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
            PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
            PROP_STRING_L,  FdoDataType_String, L"op'en",
            PROP_UNIQUE1,  FdoDataType_Int32,  (unique1val++),
            PROP_STRING_R, FdoDataType_String, L"PA",
            (FdoString*) NULL
        );

        for ( idx = 0; idx < 15; idx++ ) {
            TestCommonMiscUtil::InsertObject(
                context.connection,
                insertCmd,
                SCHEMA_NAME,
                CLASS_NAME,
                PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
                PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 15,
                PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
                PROP_STRING_L,  FdoDataType_String, L"close",
                PROP_UNIQUE1,  FdoDataType_Int32,  (unique1val++),
                PROP_STRING_R, FdoDataType_String, L"PA",
                (FdoString*) NULL
            );
        }

        TestCommonMiscUtil::InsertObject(
            context.connection,
            insertCmd,
            SCHEMA_NAME,
            CLASS_NAME,
            PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
            PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 15,
            PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 30,
            PROP_STRING_L,  FdoDataType_String, L"close",
            PROP_UNIQUE1,  FdoDataType_Int32,  (unique1val++),
            PROP_STRING_R, FdoDataType_String, L"PA",
            (FdoString*) NULL
        );

        for ( idx = 0; idx < 15; idx++ ) {
            TestCommonMiscUtil::InsertObject(
                context.connection,
                insertCmd,
                SCHEMA_NAME,
                CLASS_NAME,
                PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
                PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 15,
                PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 30,
                PROP_STRING_L,  FdoDataType_String, L"close",
                PROP_UNIQUE1,  FdoDataType_Int32,  (unique1val++),
                PROP_STRING_R, FdoDataType_String, L"PA",
                (FdoString*) NULL
            );
        }

        for ( idx = 0; idx < 15; idx++ ) {
            TestCommonMiscUtil::InsertObject(
                context.connection,
                insertCmd,
                SCHEMA_NAME,
                CLASS_NAME,
                PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
                PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 15,
                PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
                PROP_STRING_L,  FdoDataType_String, L"close",
                PROP_UNIQUE1,  FdoDataType_Int32,  (unique1val++),
                PROP_STRING_R, FdoDataType_String, L"PA",
                (FdoString*) NULL
            );
        }

        printf( "Testing checking constraint mods against multiple features ... \n" );
        {
            FdoPropertyP prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_BYTE_L );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintList* constrL = (FdoPropertyValueConstraintList*) constr.p;
                FdoPtr<FdoDataValueCollection> listL = constrL->GetConstraintList();
                
                for ( idx = (listL->GetCount() - 1); idx >= 0; idx-- ) {
                    FdoPtr<FdoDataValue> val = listL->GetItem(idx);
                    FdoByteValue* byteVal = (FdoByteValue*) val.p;
                    if ( byteVal->GetByte() == 2 ) 
                        listL->RemoveAt(idx);
                }

                dataProp->SetValueConstraint(constr);
            }

            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT32_L );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintList* constrL = (FdoPropertyValueConstraintList*) constr.p;
                FdoPtr<FdoDataValueCollection> listL = constrL->GetConstraintList();
                
                for ( idx = (listL->GetCount() - 1); idx >= 0; idx-- ) {
                    FdoPtr<FdoDataValue> val = listL->GetItem(idx);
                    FdoInt32Value* int32Val = (FdoInt32Value*) val.p;
                    if ( int32Val->GetInt32() == 30 ) 
                        listL->RemoveAt(idx);
                }

                FdoPtr<FdoInt32Value> newVal = FdoInt32Value::Create(40);
                listL->Add(newVal);

                dataProp->SetValueConstraint(constr);
            }

            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT32_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                constrR->SetMinInclusive(false);
                                
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_STRING_L );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraintList> constrL = FdoPropertyValueConstraintList::Create();
                FdoPtr<FdoDataValueCollection> listL = constrL->GetConstraintList();
                listL->Add( FdoPtr<FdoDataValue>(FdoStringValue::Create(L"close")) );                

                dataProp->SetValueConstraint( (FdoPropertyValueConstraint*) constrL.p);
            }

            // Change Range constraint to List (should fail)
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_STRING_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraintList> constrL = FdoPropertyValueConstraintList::Create();
                FdoPtr<FdoDataValueCollection> listL = constrL->GetConstraintList();
                listL->Add( FdoPtr<FdoDataValue>(FdoStringValue::Create(L"PA")) );                
 
                dataProp->SetValueConstraint( (FdoPropertyValueConstraint*) constrL.p);
            }

            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT64_L );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraintRange> constrR = FdoPropertyValueConstraintRange::Create();
                FdoPtr<FdoDataValue> val = FdoInt64Value::Create( 10LL ); 
                constrR->SetMinValue( val );
                val = FdoInt64Value::Create( 20LL ); 
                constrR->SetMaxValue( val );

                dataProp->SetValueConstraint( (FdoPropertyValueConstraint*) constrR.p);
            }

            FdoStringP messages;
            try {
                pApplyCmd->Execute();
            } catch (FdoException *ex) {
                messages = ex->GetExceptionMessage();
                ex->Release();
            }
            CPPUNIT_ASSERT_MESSAGE("Restrictive list modifications should have failed", messages != L"" );
            
            CPPUNIT_ASSERT( !messages.Contains(PROP_BYTE_L) );
            if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_INT32_L) > -1 )
                CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.Int32List'; existing data value '30' violates new constraint" ) );
            if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_INT32_R) > -1 )
                CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.Int32Range'; existing data value '10' violates new constraint" ) );
            if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_STRING_L) > -1 )
                CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.StringList'; existing data value 'op'en' violates new constraint" ) );
            CPPUNIT_ASSERT( !messages.Contains(PROP_STRING_R) );
            CPPUNIT_ASSERT( !messages.Contains(PROP_INT64_L) );
        }
    }
}

void TestCommonConstraints::DateTimeConstraints( Context& context )
{
    FdoPtr<FdoISchemaCapabilities>    schemaCap = context.connection->GetSchemaCapabilities();

    if ( schemaCap->SupportsExclusiveValueRangeConstraints() && schemaCap->SupportsInclusiveValueRangeConstraints())
    {
        FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) context.connection->CreateCommand(FdoCommandType_DescribeSchema);

        pDescCmd->SetSchemaName( SCHEMA_NAME );
        FdoPtr<FdoFeatureSchemaCollection> pSchemas2 = pDescCmd->Execute();
        FdoPtr<FdoFeatureSchema> pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
        FdoPtr<FdoClassCollection> pClasses2 = pSchema2->GetClasses();
        FdoPtr<FdoClassDefinition> pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
       
        FdoPtr<FdoIApplySchema>  pApplyCmd = (FdoIApplySchema*) context.connection->CreateCommand(FdoCommandType_ApplySchema);
        pApplyCmd->SetFeatureSchema(pSchema2);

        FdoPtr<FdoIInsert> insertCmd;
        FdoDateTime dateTime = FdoDateTime( 2005, 12, 30, 23, 59, 58.99999 );
        TestCommonMiscUtil::InsertObject(
            context.connection,
            insertCmd,
            SCHEMA_NAME,
            CLASS_NAME,
            PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
            PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
            PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
            PROP_STRING_L,  FdoDataType_String, L"close",
            PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 5000,
            PROP_STRING_R, FdoDataType_String, L"PA",
            PROP_DATE_R, FdoDataType_DateTime, &dateTime,
            (FdoString*) NULL
        );

        FdoInt32 idx;

        printf( "Testing date value changes ... \n" );
        for ( idx = 0; idx < 13; idx++ ) 
        {
            printf( "   Iteration %d \n", idx );
            FdoPropertyP prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DATE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoDateTimeValue* dateVal = (FdoDateTimeValue*) val.p;
                FdoDateTime dateTime = dateVal->GetDateTime();
                switch (idx) {
                case 0:
                    dateTime.year = 2005;
                    dateTime.month = 5;
                    dateTime.day = 15;
                    dateTime.hour = 0;
                    dateTime.minute = 2;
                    dateTime.seconds = 1;
                    break;
                case 1:
                    dateTime.year = 2004;
                    dateTime.month = 12;
                    break;
                case 2:
                    dateTime.month = 11;
                    dateTime.day = 30;
                    break;
                case 3:
                    dateTime.day = 29;
                    dateTime.hour = 23;
                    break;
                case 4:
                    dateTime.hour = 22;
                    dateTime.minute = 59;
                    break;
                case 5:
                    dateTime.minute = 58;
                    dateTime.seconds = 59;
                    break;
                case 6:
                    dateTime.seconds = (FdoFloat) 58.99999;
                    break;
                case 7:
                    dateTime.year = 2005;
                    break;
                case 8:
                    dateTime.month = 12;
                    break;
                case 9:
                    dateTime.day = 30;
                    break;
                case 10:
                    dateTime.hour = 23;
                    break;
                case 11:
                    dateTime.minute = 59;
                    break;
                case 12:
                    dateTime.seconds = 59;
                    break;
                }
                dateVal->SetDateTime(dateTime);

                dataProp->SetValueConstraint(constr);
            }
                
            if ( ((idx > 6) && !CanRestrictCheckConstraint()) || (idx > 11) ) 
            {
                FdoStringP messages;
                try {
                    pApplyCmd->Execute();
                } catch (FdoException *ex) {
                    messages = ex->GetExceptionMessage();
                    ex->Release();
                }
                CPPUNIT_ASSERT_MESSAGE("Date value modification should have failed", messages != L"" );
                
                if ( CanRestrictCheckConstraint() ) {
                    if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_DATE_R) > -1 )
                        CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.DateRange'; existing data value '2005-12-30T23:59:58.999989' violates new constraint" ) );
                }
                else {
                    if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_DATE_R) > -1 )
                        CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.DateRange'; property has values and new constraint is more restrictive" ) );
                }
            }
            else 
            {
                pApplyCmd->Execute();
            }

            pSchemas2 = pDescCmd->Execute();
            pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
            pClasses2 = pSchema2->GetClasses();
            pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
            pApplyCmd->SetFeatureSchema( pSchema2 );

        }

        printf( "Testing undefined date or time components ... \n" );
        for ( idx = 0; idx < 4; idx++ ) 
        {
            printf( "   Iteration %d \n", idx );
            FdoPropertyP prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DATE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoDateTimeValue* dateVal = (FdoDateTimeValue*) val.p;
                FdoDateTime dateTime = dateVal->GetDateTime();
                switch (idx) {
                case 0:
                    dateTime.hour = -1;
                    break;
                case 1:
                    dateTime.year = 2003;
                    break;
                case 2:
                    dateTime.year = -1;
                    break;
                case 3:
                    dateTime.hour = 0;
                    dateTime.seconds = (FdoFloat) 0.00001;
                    break;
                }
                dateVal->SetDateTime(dateTime);

                dataProp->SetValueConstraint(constr);
            }
                
            if ( (idx > 1) && !CanRestrictCheckConstraint() ) 
            {
                FdoStringP messages;
                try {
                    pApplyCmd->Execute();
                } catch (FdoException *ex) {
                    messages = ex->GetExceptionMessage();
                    ex->Release();
                }
                CPPUNIT_ASSERT_MESSAGE("Date modifications should have failed", messages != L"" );
                
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_DATE_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.DateRange'; property has values and new constraint is more restrictive" ) );
            }
            else 
            {
                pApplyCmd->Execute();
            }

            pSchemas2 = pDescCmd->Execute();
            pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
            pClasses2 = pSchema2->GetClasses();
            pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
            pApplyCmd->SetFeatureSchema( pSchema2 );

        }

        TestCommonMiscUtil::DeleteObjects( 
            context.connection,
            SCHEMA_NAME,
            CLASS_NAME_BASE,
            NULL
        );

        FdoPropertyP prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DATE_R );
        if ( prop ) {
            FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

            FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
            FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
            FdoPtr<FdoDataValue> val = constrR->GetMinValue();
            FdoDateTimeValue* dateVal = (FdoDateTimeValue*) val.p;
            FdoDateTime dateTime1( 3, 0, (FdoFloat) 0 );
            dateVal->SetDateTime(dateTime1);

            val = constrR->GetMaxValue();
            dateVal = (FdoDateTimeValue*) val.p;

            FdoDateTime dateTime2( 15, 10, (FdoFloat) 15 );
            dateVal->SetDateTime(dateTime2);

            dataProp->SetValueConstraint(constr);

        }
                
        pApplyCmd->Execute();
        FdoDateTime dateTime2 = FdoDateTime( 2005, 05, (FdoInt8) 30 );

        TestCommonMiscUtil::InsertObject(
            context.connection,
            insertCmd,
            SCHEMA_NAME,
            CLASS_NAME,
            PROP_FEATID, FdoDataType_Int32, GetNextFeatId(context.connection, CLASS_NAME),
            PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
            PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
            PROP_STRING_L,  FdoDataType_String, L"close",
            PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 5000,
            PROP_STRING_R, FdoDataType_String, L"PA",
            PROP_DATE_R, FdoDataType_DateTime, &dateTime2,
            (FdoString*) NULL
        );

        printf( "Testing date value changes ... \n" );
        for ( idx = 0; idx < 9; idx++ ) 
        {
            printf( "   Iteration %d \n", idx );
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DATE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMaxValue();
                FdoDateTimeValue* dateVal = (FdoDateTimeValue*) val.p;
                FdoDateTime dateTime = dateVal->GetDateTime();
                switch (idx) {
                case 0:
                    dateTime.hour = 15;
                    dateTime.minute = 10;
                    dateTime.seconds = 15;
                    break;
                case 1:
                    dateTime.hour = 16;
                    dateTime.minute = 5;
                    break;
                case 2:
                    dateTime.minute = 6;
                    dateTime.seconds = 10;
                    break;
                case 3:
                    dateTime.seconds = (FdoFloat) 10.00001;
                    break;
                case 4:
                    dateTime.hour = 15;
                    break;
                case 5:
                    dateTime.minute = 5;
                    break;
                case 6:
                    dateTime.seconds = 10;
                    break;
                case 7:
                    dateTime.hour = -1;
                    break;
                case 8:
                    dateTime.year = 2005;
                    dateTime.month = 5;
                    dateTime.day = 24;
                    break;
                }
                dateVal->SetDateTime(dateTime);

                dataProp->SetValueConstraint(constr);
            }
                
            if ( ((idx > 3) && !CanRestrictCheckConstraint()) || (idx == 8) ) 
            {
                FdoStringP messages;
                try {
                    pApplyCmd->Execute();
                } catch (FdoException *ex) {
                    messages = ex->GetExceptionMessage();
                    ex->Release();
                }
                CPPUNIT_ASSERT_MESSAGE("Time value modification should have failed", messages != L"" );
                
                if ( CanRestrictCheckConstraint() ) {
                    if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_DATE_R) > -1 )
                        CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.DateRange'; existing data value '2005-05-30T-1:-1:00' violates new constraint" ) );
                }
                else {
                    if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_DATE_R) > -1 )
                        CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.DateRange'; property has values and new constraint is more restrictive" ) );
                }
            }
            else 
            {
                pApplyCmd->Execute();
            }

            pSchemas2 = pDescCmd->Execute();
            pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
            pClasses2 = pSchema2->GetClasses();
            pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
            pApplyCmd->SetFeatureSchema( pSchema2 );
        }
    }
}

void TestCommonConstraints::CreateConnection( Context&, FdoBoolean )
{
    throw FdoException::Create( L"base TestCommonConstraints::CreateConnection called, need implementation on subclass" );
}

FdoBoolean TestCommonConstraints::CanRestrictCheckConstraint()
{
    return true;
}

FdoBoolean TestCommonConstraints::FdoValidatesData()
{
    return false;
}

FdoDouble TestCommonConstraints::GetDoubleRounding(FdoDataType)
{
    return (FdoDouble) 0.0;
}

FdoFloat TestCommonConstraints::GetSecondsIncrement()
{
    return (FdoFloat) 0.001;
}

FdoInt32 TestCommonConstraints::GetLtMethod()
{
    return 0; // No long transactions by default
}

FdoInt32 TestCommonConstraints::GetExpectedCheckConstraintCount( FdoIConnection* connection )
{
    FdoPtr<FdoISchemaCapabilities>    schemaCap = connection->GetSchemaCapabilities();
    FdoInt32                          typeCount;
    FdoInt32                          idx;
    FdoDataType*                      supportedTypes = schemaCap->GetDataTypes( typeCount );
    FdoInt32                          checkCount = 0;

    for ( idx = 0; idx < typeCount; idx++ ) {
        switch ( supportedTypes[idx] ) {
        case FdoDataType_Byte:
            checkCount += 2;
            break;
        
        case FdoDataType_DateTime:
            checkCount += 2;
            break;

        case FdoDataType_Decimal:
            checkCount++;
            break;

        case FdoDataType_Double:
            checkCount += 2;
            break;

        case FdoDataType_Int16:
            checkCount++;
            break;


        case FdoDataType_Int32:
            checkCount += 2;
            break;

        case FdoDataType_Int64:
            checkCount += 2;
            break;

        case FdoDataType_Single:
            checkCount += 2;
            break;

        case FdoDataType_String:
            checkCount += 3;
            break;
        }
    }

    return checkCount;
}

FdoInt32 TestCommonConstraints::GetNextFeatId( FdoIConnection* connection, FdoString* className )
{
    FdoInt32 nextFeatId = 1;
    FdoPtr<FdoISelect> selCmd = (FdoISelect*)connection->CreateCommand( FdoCommandType_Select );
    selCmd->SetFeatureClassName(className);

    FdoPtr<FdoIFeatureReader> myReader = selCmd->Execute();

    while ( myReader->ReadNext() ) {
        FdoInt32 featId = myReader->GetInt32( L"FeatureId" );
        if ( featId >= nextFeatId ) 
            nextFeatId = featId + 1;
    }

    return nextFeatId; 
}

FdoStringP	TestCommonConstraints::FixDatetimeFormat( FdoDataValue*  val )
{
	FdoStringP	val2 = val->ToString();

	val2 = val2.Replace(L"'", L"");
	val2 = val2.Replace(L"TIMESTAMP ", L"");
	val2 = val2.Replace(L" ", L"-");
	val2 = val2.Replace(L":", L"-");
   
	return val2;
}

TestCommonConstraints::Context::Context( FdoInt32 ltMethodIn ) 
{
    ltMethod = ltMethodIn;
}

TestCommonConstraints::Context::~Context() 
{
    if ( connection ) {
        try {
            connection->Close();
        }
        catch ( ... ) {
        }
    }
}

