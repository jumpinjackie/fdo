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

#include "Pch.h"
#include "FdoDatastoreTest.h"
#include "UnitTestUtil.h"

#define DBG(x)		//x
#define DBG_MAX(x)		//x

#define UNIT_TEST_DB_DESCRIPTION	L"Fdo datastore test"
#define UNIT_TEST_DB_PASSWORD		L"test"
#define UNIT_TEST_DB_TABLESPACE		L"SYSTEM"

FdoDatastoreTest::FdoDatastoreTest(void)
{
}

FdoDatastoreTest::~FdoDatastoreTest(void)
{
}

void FdoDatastoreTest::setUp ()
{
    set_provider();

    // The following checks on the availability of lock/LT support. It is
    // required to know this before actually executing the tests so the
    // tests work in any environment.
    SetLtMode();
}

void FdoDatastoreTest::SetLtMode ()
{
    // There is nothing to do here - ORACLE only
}

void FdoDatastoreTest::predelete()
{
	try {

		DestroyDatastore( true ); // ignore if doesn't exist
	} catch (FdoException *ex) {
		DBG(printf("Expected error: %ls\n", ex->GetExceptionMessage()));
		ex->Release();
	}
}

void FdoDatastoreTest::Cmd_ListDatastores()
{
	int count1 = ListDatastores(true);
	int count2 = ListDatastores(false);

	printf(".Found %d total datastores [%d fdo + %d non-fdo]\n", count1, count2, count1 - count2);

	CPPUNIT_ASSERT_MESSAGE("Wrong number of datastores", count1 >= count2 );
}

void FdoDatastoreTest::ListDatabaseProperties( FdoIDataStorePropertyDictionary* dictionary)
{
	int				count = 0;
    try
    {
		FdoString **names = dictionary->GetPropertyNames(count);
		
		DBG(printf("\n.PROPERTIES.\n"));
		for ( int i = 0; i < count; i++)
		{
			FdoString  *name = names[i];

			bool propertyRequired = dictionary->IsPropertyRequired(name);
			bool propertyEnumerable = dictionary->IsPropertyEnumerable(name);
			bool propertyProtected = dictionary->IsPropertyProtected(name);
			const wchar_t *propertyLocalized = dictionary->GetLocalizedName(name);
			bool propertyFile = dictionary->IsPropertyFileName(name);
			bool propertyDatastore = dictionary->IsPropertyDatastoreName(name);	
			bool propertyPath = dictionary->IsPropertyFilePath(name);

			FdoString* value = dictionary->GetProperty(name);

			DBG(printf(" %d. name= %ls value= %ls \n", i, name, value ));
			DBG(printf("\tloc:'%ls' req:%d enum:%d prot:%d isfile:%d ispath:%d isdb:%d\n ",
				  propertyLocalized, propertyRequired, propertyEnumerable, propertyProtected,
				  propertyFile, propertyPath, propertyDatastore ));
			
			int n;
			if ( propertyEnumerable ) {
				FdoString**  enums = dictionary->EnumeratePropertyValues(name, n);	
				for ( int j = 0; j < n; j++ ) {
					DBG(printf("\t enum %d. %s\n", j, UnitTestUtil::w2a(enums[j]) ));
				}
			}
		}
		printf(".found %d properties\n", count);
    }
    catch (FdoException *ex)
    {
        TestCommonFail(ex);
    }
}


int FdoDatastoreTest::ListDatastores( bool include )
{
    FdoPtr<FdoIConnection> connection;
    int				count = 0;
	int				countDb = 0;
	bool			bVal;

	DBG(printf(".include non-fdo databases = %s\n", include? "YES" : "NO" ));

    try
    {
 		wchar_t *connectString = UnitTestUtil::GetConnectionString(Connection_NoDatastore);
		connection = UnitTestUtil::GetProviderConnectionObject();
		connection->SetConnectionString ( connectString);

		connection->Open();
        FdoPtr<FdoIConnectionCapabilities> connectionCapabilities = connection->GetConnectionCapabilities();

        FdoPtr<FdoIListDataStores>		pListDataStoresCmd = (FdoIListDataStores*) connection->CreateCommand(FdoCommandType_ListDataStores);
        
		pListDataStoresCmd->SetIncludeNonFdoEnabledDatastores( include );
		CPPUNIT_ASSERT_MESSAGE("SetIncludeNonFdoEnabledDatastores() failed", 
					pListDataStoresCmd->GetIncludeNonFdoEnabledDatastores() == include );

		FdoPtr<FdoIDataStoreReader>	pReader = pListDataStoresCmd->Execute();

		FdoStringP datastore = UnitTestUtil::GetEnviron("datastore", L"_DBCMD");

		DBG(printf("\n.DATABASES\n"));
		while ( pReader->ReadNext() )
		{
			countDb++;

			FdoString *string = pReader->GetName();
			FdoString *string2 = pReader->GetDescription();  
			bVal = pReader->GetIsFdoEnabled();
			
			if ( FdoCommonOSUtil::wcsicmp( string, datastore ) == 0 )
			{
				/*DBG*/(printf("Checking %ls...\n", string ));
				FdoPtr<FdoIDataStorePropertyDictionary> dictionary = pReader->GetDataStoreProperties();
				ListDatabaseProperties( dictionary );

                if ( connectionCapabilities->SupportsLongTransactions() )
                {
    				FdoStringP ltmode = dictionary->GetProperty(L"LtMode");
    				CPPUNIT_ASSERT_MESSAGE("Wrong LT mode", ( wcscmp( ltmode, get_lt_mode() ) == 0 ));
                }

                if ( connectionCapabilities->SupportsLocking() )
                {
                    FdoStringP lckmode = dictionary->GetProperty(L"LockMode");
				    CPPUNIT_ASSERT_MESSAGE("Wrong LT mode", ( wcscmp( lckmode, get_lt_mode()) == 0 ));
                }

				CPPUNIT_ASSERT_MESSAGE("Description doesn't match", ( wcscmp( string2, UNIT_TEST_DB_DESCRIPTION) == 0));			
				CPPUNIT_ASSERT_MESSAGE("Database is not FDO enabled", bVal == true );
			}
			
			DBG_MAX(printf("%ls  ['%ls'] [fdo: %s]\n", string, string2, bVal? "yes" : "no"));
		}

        // test Close() function for one invokation, test not calling Close() for the other.
        if ( include ) 
        {
            pReader->Close();
            CPPUNIT_ASSERT_MESSAGE("Reader should be closed", !pReader->ReadNext() );
        }

        if ( countDb > 2 ) 
        {
    		pReader = pListDataStoresCmd->Execute();
            CPPUNIT_ASSERT_MESSAGE("Expected at least one datastore", pReader->ReadNext() );
            pReader->Close();
            CPPUNIT_ASSERT_MESSAGE("Reader should be closed", !pReader->ReadNext() );
        }

        pListDataStoresCmd = NULL;
        pReader = NULL;
		connection->Close ();
    }
    catch (FdoException *ex)
    {
        if (connection)
    	    connection->Close ();
        TestCommonFail(ex);
    }
	return countDb;
}

void FdoDatastoreTest::Cmd_CreateDatastore()
{
    FdoPtr<FdoIConnection> connection;

    try
    {
		wchar_t *connectString = UnitTestUtil::GetConnectionString(Connection_NoDatastore);
		connection = UnitTestUtil::GetProviderConnectionObject();
		connection->SetConnectionString ( connectString);

		connection->Open();

        CreateDatastore( 
            connection,
    		UnitTestUtil::GetEnviron("datastore", L"_DBCMD")
		);
    
        connection->Close ();
    }
    catch (FdoException *ex)
    {
        if (connection)
    	    connection->Close ();
        TestCommonFail(ex);
    }
}

void FdoDatastoreTest::CreateDatastore( FdoIConnection* connection, FdoString* dsName, bool setHasMetaSchema, bool hasMetaSchema )
{
    int				count = 0;
	int				countDb = 0;

    FdoPtr<FdoICreateDataStore>		pCreateCmd = (FdoICreateDataStore*) connection->CreateCommand(FdoCommandType_CreateDataStore);
        
    FdoPtr<FdoIDataStorePropertyDictionary> dictionary = pCreateCmd->GetDataStoreProperties();

	FdoString **names = dictionary->GetPropertyNames(count);
		
	// set PROPERTIES
	for ( int i = 0; i < count; i++)
	{
		FdoString  *name = names[i];

        if ( wcscmp( name, L"DataStore" ) == 0 )
		{
			dictionary->SetProperty( name, dsName );
		}
		else if ( wcscmp( name, L"Description" ) == 0 )
			dictionary->SetProperty( name, UNIT_TEST_DB_DESCRIPTION );
		else if ( wcscmp( name, L"Password" ) == 0 && 
	            dictionary->IsPropertyRequired( L"Password" ))
    		dictionary->SetProperty( name, UNIT_TEST_DB_PASSWORD );
		else if ( wcscmp( name, L"LtMode" ) == 0 )
			dictionary->SetProperty( name,  get_lt_mode());
		else if ( wcscmp( name, L"LockMode" ) == 0 )
			dictionary->SetProperty( name,  get_lt_mode()); // The same
		else if ( wcscmp( name, L"TableSpace" ) == 0 )
			dictionary->SetProperty( name,   UNIT_TEST_DB_TABLESPACE); // The same
        else if ( wcscmp( name, L"IsFdoEnabled" ) == 0 ) {
            if ( setHasMetaSchema ) 
                dictionary->SetProperty( name,   hasMetaSchema ? L"true" : L"false"); // The same
        }
		else {
			CPPUNIT_ASSERT_MESSAGE("Unknown property name", false);
		}
	}

    ListDatabaseProperties( dictionary );

	// Create
	pCreateCmd->Execute();
}

void FdoDatastoreTest::Cmd_DestroyDatastore()
{
	DestroyDatastore(false);	
}

void FdoDatastoreTest::DestroyDatastore(bool ignoreEx)
{
    FdoPtr<FdoIConnection> connection;
    int				count = 0;
	int				countDb = 0;

    try
    {
		wchar_t *connectString = UnitTestUtil::GetConnectionString(Connection_NoDatastore);
		connection = UnitTestUtil::GetProviderConnectionObject();
		connection->SetConnectionString ( connectString);

		connection->Open();

        FdoPtr<FdoIDestroyDataStore>		pDestroyCmd = (FdoIDestroyDataStore*) connection->CreateCommand(FdoCommandType_DestroyDataStore);
        
		FdoPtr<FdoIDataStorePropertyDictionary> dictionary = pDestroyCmd->GetDataStoreProperties();

		FdoString **names = dictionary->GetPropertyNames(count);
		
		// set PROPERTIES
		for ( int i = 0; i < count; i++)
		{
			FdoString  *name = names[i];

			if ( !dictionary->IsPropertyRequired( name ) )
				continue;

			if ( wcscmp( name, L"DataStore" ) == 0 )
			{
				FdoStringP datastore = UnitTestUtil::GetEnviron("datastore", L"_DBCMD");
 				dictionary->SetProperty( name, datastore );
			}
			else if ( wcscmp( name, L"Description" ) == 0 )
				CPPUNIT_ASSERT_MESSAGE("Description is not required", false);
 			else if ( wcscmp( name, L"Password" ) == 0 )
				dictionary->SetProperty( name, UNIT_TEST_DB_PASSWORD );
			else if ( wcscmp( name, L"LtMode" ) == 0 )
				CPPUNIT_ASSERT_MESSAGE("LtMode is not required", false );
			else if ( wcscmp( name, L"LockMode" ) == 0 )
				CPPUNIT_ASSERT_MESSAGE("LckMode is not required", false );
			else if ( wcscmp( name, L"TableSpace" ) == 0 )
				CPPUNIT_ASSERT_MESSAGE("TableSpace is not required", false );
			else {
				CPPUNIT_ASSERT_MESSAGE("Unknown property name", false);
			}
		}
		ListDatabaseProperties( dictionary );

		// Delete
		pDestroyCmd->Execute();

		connection->Close ();
    }
    catch (FdoException *ex)
    {
        if (connection)
    	    connection->Close ();
		if ( !ignoreEx )
			TestCommonFail(ex);
        else
            ex->Release();
    }
}

void FdoDatastoreTest::TestReservedName()
{
    FdoPtr<FdoIConnection> connection;
    int				count = 0;
	int				countDb = 0;

    try
    {
		wchar_t *connectString = UnitTestUtil::GetConnectionString(Connection_NoDatastore);
		connection = UnitTestUtil::GetProviderConnectionObject();
		connection->SetConnectionString ( connectString);

		connection->Open();

        ReservedName( connection);
    
        connection->Close ();
    }
    catch (FdoException *ex)
    {
        if (connection)
    	    connection->Close ();
        TestCommonFail(ex);
    }
}

void FdoDatastoreTest::ReservedName(FdoIConnection* connection)
{
}

void FdoDatastoreTest::DropAllMyDatastores()
{
    FdoPtr<FdoIConnection> connection;

	DBG(printf(".Drop all user's datastores\n"));

    try
    {
 		wchar_t *connectString = UnitTestUtil::GetConnectionString(Connection_NoDatastore);
		connection = UnitTestUtil::GetProviderConnectionObject();
		connection->SetConnectionString ( connectString);

		connection->Open();

        FdoPtr<FdoIListDataStores>		pListDataStoresCmd = (FdoIListDataStores*) connection->CreateCommand(FdoCommandType_ListDataStores);
        
		pListDataStoresCmd->SetIncludeNonFdoEnabledDatastores( false );

		FdoPtr<FdoIDataStoreReader>	pReader = pListDataStoresCmd->Execute();

		FdoStringP datastore = UnitTestUtil::GetEnviron("datastore");

		DBG(printf("\n.DATABASES\n"));
		while ( pReader->ReadNext() )
		{
			FdoString *name = pReader->GetName();

			// Match the FDO_<user>
			if ( FdoCommonOSUtil::wcsnicmp( name, datastore, wcslen(datastore) ) == 0 )
			{
				printf("%ls\n", name );
				try 
				{
					FdoPtr<FdoIDestroyDataStore>		pDestroyCmd = (FdoIDestroyDataStore*) connection->CreateCommand(FdoCommandType_DestroyDataStore);
					FdoPtr<FdoIDataStorePropertyDictionary> dictionary = pDestroyCmd->GetDataStoreProperties();
					dictionary->SetProperty( L"DataStore", name );

					pDestroyCmd->Execute();

				} catch (FdoException *ex) {
					ex->Release();
				}
			}			
		}

		connection->Close ();
    }
    catch (FdoException *ex)
    {
        if (connection)
    	    connection->Close ();
        TestCommonFail(ex);
    }
}

