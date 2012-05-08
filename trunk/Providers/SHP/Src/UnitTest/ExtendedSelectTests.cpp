
#include "Pch.h"

#include <ctime>
#include "ExtendedSelectTests.h"
#include "SHP/IExtendedSelect.h"
#include "SHP/ShpCommandType.h"

#ifndef _WIN32
#include <unistd.h>
#endif

#ifdef _WIN32
#define LOCATION L"..\\..\\TestData\\World_Countries"
#else
#define LOCATION L"../../TestData/World_Countries"
#endif

#define	CLASS_NAME	L"World_Countries"

CPPUNIT_TEST_SUITE_REGISTRATION (ExtendedSelectTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (ExtendedSelectTests, "ExtendedSelectTests");


ExtendedSelectTests::ExtendedSelectTests(void)
{
}

ExtendedSelectTests::~ExtendedSelectTests(void)
{
}

void ExtendedSelectTests::setUp ()
{
}

void ExtendedSelectTests::WorldTest()
{
    int count = 0;

    try
    {
		FdoPtr<FdoIConnection> connection = ShpTests::GetConnection ();

        connection->SetConnectionString (L"DefaultFileLocation=" LOCATION);
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == connection->Open ());

        FdoPtr<FdoIExtendedSelect> select = (FdoIExtendedSelect*)connection->CreateCommand(FdoCommandType_ExtendedSelect); 
        select->SetFeatureClassName (CLASS_NAME);

        FdoPtr<FdoIdentifierCollection>selecProperties = select->GetPropertyNames();
        selecProperties->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"MAPKEY")) );
        selecProperties->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"NAME")) );
        selecProperties->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"KEY")) );
        selecProperties->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"FeatId")) );

		// Naked select, no ordering properties.
		clock_t start;
        clock_t finish;
        start = clock ();

        FdoPtr<FdoIScrollableFeatureReader>	reader = select->ExecuteScrollable();

        finish = clock ();
        printf ("Creation  time: %2.3f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);
 

		///// Try ReadAt() with no ordering properties.

		FdoString*	keyName = L"Tunisia";
		FdoInt32	keyFeatid = 9;

        FdoPtr<FdoPropertyValueCollection> key1 = FdoPropertyValueCollection::Create();
        FdoPtr<FdoStringValue> svname1 = FdoStringValue::Create(L"Tunisia");
        key1->Add( FdoPropertyValue::Create(L"NAME", svname1) );

        start = clock ();
		count = 0;
        if( reader->ReadAt( key1 ) )
        {
			count++;
			const wchar_t *name = reader->GetString(L"NAME");
			FdoInt32 featid = reader->GetInt32( L"FeatId" );
			printf("name: %ls \t featid: %d\n", name, featid );
            CPPUNIT_ASSERT_MESSAGE("Unexpected value for keyName", wcscmp(name, keyName)==0 );
			CPPUNIT_ASSERT_MESSAGE("Unexpected value for keyFeatid", featid == keyFeatid );
        }

        finish = clock ();
        printf ("Jump to record '%ls' (featid=%d)  time: %2.3f seconds\n\n\n", keyName, keyFeatid, (double)(finish - start) / CLOCKS_PER_SEC);
        CPPUNIT_ASSERT_MESSAGE("Unexpected number of records", count==1 );

		/// Should get the same result with IndexOf()
		unsigned int rowid = reader->IndexOf( key1 );
		CPPUNIT_ASSERT_MESSAGE("Unexpected rowid", rowid == keyFeatid );

		// Try ReadAt() with no ordering properties.
        FdoPtr<FdoPropertyValueCollection> key0 = FdoPropertyValueCollection::Create();
		FdoPtr<FdoInt32Value> svname0 = FdoInt32Value::Create(keyFeatid);
        key0->Add( FdoPropertyValue::Create(L"FeatId", svname0) );

        start = clock ();
		count = 0;
        if( reader->ReadAt( key0 ) )
        {
			count++;
			FdoInt32 featid = reader->GetInt32( L"FeatId" );
			const wchar_t *name = reader->GetString(L"NAME");
            CPPUNIT_ASSERT_MESSAGE("Unexpected value for keyName", wcscmp(name, keyName)==0 );
			CPPUNIT_ASSERT_MESSAGE("Unexpected value for keyFeatid", featid == keyFeatid );
        }

        finish = clock ();
        printf ("Jump to record featid=%d ('%ls') time: %2.3f seconds\n\n\n", keyFeatid, keyName, (double)(finish - start) / CLOCKS_PER_SEC);
        CPPUNIT_ASSERT_MESSAGE("Unexpected number of records", count==1 );

		// Double check
		bool status = reader->ReadAtIndex( keyFeatid );
		CPPUNIT_ASSERT_MESSAGE("ReadAtIndex", status );

		{
			FdoInt32 featid = reader->GetInt32( L"FeatId" );
			const wchar_t *name = reader->GetString(L"NAME");
            CPPUNIT_ASSERT_MESSAGE("Unexpected value for keyName", wcscmp(name, keyName)==0 );
			CPPUNIT_ASSERT_MESSAGE("Unexpected value for keyFeatid", featid == keyFeatid );
		}

        // Iterate in the forward direction from first to last, no ordering
        wchar_t*   expected0[] = {L"South Africa",L"Namibia",L"Botswana",L"Algeria",L"Libya",L"Morocco",L"Western Sahara",L"Sudan",L"Tunisia",L"Egypt"};
        start = clock ();
        count = 0;
        if( reader->ReadFirst() )
        do
        {
            if( count++ < 10 )
            {
                const wchar_t *name = reader->GetString(L"NAME");
                CPPUNIT_ASSERT_MESSAGE("Unexpected value", wcscmp(name,expected0[count-1])==0 );
                printf("%ls \t %d\n", name, reader->GetInt32( L"FeatId" ) );
            }
        }while( reader->ReadNext() );

        // Iterate in the backward direction from last to first
        finish = clock ();
        printf ("Read forward no ordering(%d)  time: %2.3f seconds\n\n\n",count, (double)(finish - start) / CLOCKS_PER_SEC);
        CPPUNIT_ASSERT_MESSAGE("Unexpected number of records", count==419 );

		// Use ordering properties
        FdoPtr<FdoIdentifierCollection> orders = select->GetOrdering();
        orders->Add( FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"NAME")));
        select->SetOrderingOption(L"NAME", FdoOrderingOption_Ascending);

		// Reexecute!!!
		reader = select->ExecuteScrollable();

        // Iterate in the forward direction from first to last
        wchar_t*   expected[] = {L"Afghanistan",L"Albania",L"Algeria",L"Andorra",L"Angola",L"Angola",L"Antigua and Barbuda",L"Argentina",L"Argentina",L"Argentina"};
        start = clock ();
        count = 0;
        if( reader->ReadFirst() )
        do
        {
            if( count++ < 10 )
            {
                const wchar_t *name = reader->GetString(L"NAME");
				CPPUNIT_ASSERT_MESSAGE("Unexpected value", wcscmp(name,expected[count-1])==0 );
                printf("%ls \t %d\n", name, reader->GetInt32( L"FeatId" ) );
            }
        }while( reader->ReadNext() );

        finish = clock ();
        printf ("Read forward(%d)  time: %2.3f seconds\n\n\n",count, (double)(finish - start) / CLOCKS_PER_SEC);
        CPPUNIT_ASSERT_MESSAGE("Unexpected number of records", count==419 );


        // Iterate in the backward direction from last to first
        wchar_t*   expected2[] = {L"Zimbabwe",L"Zambia",L"Zaire",L"Western Sahara",L"Virgin Islands",L"Vietnam",L"Venezuela",L"Venezuela",L"Uzbekistan",L"Uruguay"};
        count = 0;
        start = clock ();
        if( reader->ReadLast() )
        do
        {
            if( count++ < 10 )
            {
                const wchar_t *name = reader->GetString(L"NAME");
                CPPUNIT_ASSERT_MESSAGE("Unexpected value", wcscmp(name,expected2[count-1])==0 );
                printf("%ls \t %d\n", name, reader->GetInt32( L"FeatId" ) );
            }
        }while( reader->ReadPrevious() );

        finish = clock ();
        printf ("Read backward(%d)  time: %2.3f seconds\n\n\n",count, (double)(finish - start) / CLOCKS_PER_SEC);
        CPPUNIT_ASSERT_MESSAGE("Unexpected number of records", count==419 );

        // Jump to a particular record
        wchar_t*   expected3[] = {L"Tunisia",L"Turkey",L"Turkmenistan",L"Turks and Caicos Islands",L"Turks and Caicos Islands",L"Uganda",L"Ukraine",L"United Kingdom",L"United Kingdom",L"United Kingdom"};
        FdoPtr<FdoPropertyValueCollection>key = FdoPropertyValueCollection::Create();
        FdoPtr<FdoStringValue> svname = FdoStringValue::Create(L"Tunisia");
        key->Add( FdoPropertyValue::Create(L"NAME", svname) );
        count = 0;

        start = clock ();
        if( reader->ReadAt( key ) )
        do
        {
            if( count++ < 10 )
            {
                const wchar_t *name = reader->GetString(L"NAME");
                CPPUNIT_ASSERT_MESSAGE("Unexpected value", wcscmp(name,expected3[count-1])==0 );
                printf("%ls \t %d\n", name, reader->GetInt32( L"FeatId" ) );
            }
        }while( reader->ReadNext() );

        finish = clock ();
        printf ("Jump to record '%ls' (count=%d)  time: %2.3f seconds\n\n\n", L"Tunisia", count, (double)(finish - start) / CLOCKS_PER_SEC);
        CPPUNIT_ASSERT_MESSAGE("Unexpected number of records", count==40 );

    }
    catch (FdoException *ex )
    {
        printf("FDO error: %ls\n", ex->GetExceptionMessage());
        return;
    }
}

