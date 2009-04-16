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
#include "SqlServerFdoInsertTest.h"
#include "UnitTestUtil.h"
#include "ConnectionUtil.h"
#include "../SchemaMgr/Ph/Owner.h"

CPPUNIT_TEST_SUITE_REGISTRATION( SqlServerFdoInsertTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SqlServerFdoInsertTest, "FdoInsertTest");

FdoString* SqlServerFdoInsertTest::mDefect1206136Suffix = L"_defect1206136";

void SqlServerFdoInsertTest::set_provider()
{
	UnitTestUtil::SetProvider( "SQLServerSpatial" );
}

// The function adds a test to the insert test suite that is triggered by a QA
// test scenario. In this case, QA attempted to inserted a row into a table where
// some of the date-time properties still contain its initial value (-1). The
// insert command did not fail and the reader inidicated that an object had been
// inserted. However, the data was not actually entered in case of a SQL Server
// environment.
void SqlServerFdoInsertTest::insertDate ()
{
//TestCommonMiscUtil::SetupLeakReport();
    wprintf(L"InsertDate: Test insert command with different date/time settings.\n");
    FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(mSuffix, true);

    int         baseColorIndex    = 990,
                numberTestCases   = 9;
    bool        exceptionExpected = false;
    FdoStringP  colorIndex;
    FdoDateTime dateTime;

    for (int j = 0; j < numberTestCases; j++)
    {
        // Reinitialize the date-time object as defined by the constructor.

        dateTime.year    = -1;
        dateTime.month   = -1;
        dateTime.day     = -1;
        dateTime.hour    = -1;
        dateTime.minute  = -1;
        dateTime.seconds = 0.0f;

        colorIndex = FdoStringP::Format(L"%d", (baseColorIndex+j));
        switch (j)
        {
            case 0:
                wprintf(L"  > (1) No date or time specification: should fail.\n");
                exceptionExpected = true;
                break;

            case 1:
                wprintf(L"  > (2)Complete date, but no time specification: should work.\n");
                dateTime.year    = 2003;
                dateTime.month   = 1;
                dateTime.day     = 1;
                exceptionExpected = false;
                break;

            case 2:
                wprintf(L"  > (3) Complete time, but no date specification: should fail.\n");
                dateTime.hour    = 1;
                dateTime.minute  = 11;
                dateTime.seconds = 0;
                exceptionExpected = true;
                break;

            case 3:
                wprintf(L"  > (4) Complete date and time specification: should work.\n");
                dateTime.year    = 2003;
                dateTime.month   = 1;
                dateTime.day     = 1;
                dateTime.hour    = 1;
                dateTime.minute  = 11;
                dateTime.seconds = 11;
                exceptionExpected = false;
                break;

            case 4:
                wprintf(L"  > (5) Partial date, but no time specification: should fail.\n");
                dateTime.year     = 2003;
                dateTime.day      = 1;
                exceptionExpected = true;
                break;

            case 5:
                wprintf(L"  > (6) Partial time, but no date specification: should fail.\n");
                dateTime.hour     = 1;
                dateTime.seconds  = 0;
                exceptionExpected = true;
                break;

            case 6:
                wprintf(L"  > (7) Partial date and time specification: should fail.\n");
                dateTime.year     = 2003;
                dateTime.day      = 1;
                dateTime.hour     = 1;
                dateTime.seconds  = 0;
                exceptionExpected = true;
                break;

            case 7:
                wprintf(L"  > (8) Partial date and complete time specification: should fail.\n");
                dateTime.year     = 2003;
                dateTime.day      = 1;
                dateTime.hour     = 1;
                dateTime.minute   = 11;
                dateTime.seconds  = 0;
                exceptionExpected = true;
                break;

            case 8:
                wprintf(L"  > (9) Partial time and complete date specification: should fail.\n");
                dateTime.year     = 2003;
                dateTime.month    = 1;
                dateTime.day      = 1;
                dateTime.hour     = 1;
                dateTime.seconds  = 0;
                exceptionExpected = true;
                break;

        }

        // Attempt to insert a record using the constructed date-time object.
        try
        {
            FdoInsertTest::insertDate(connection, dateTime, colorIndex);
        }
        catch (FdoException *ex)
        {
            if (!exceptionExpected)
            {
                wprintf(L"    Unexpected Exception: %ls\n", ex->GetExceptionMessage());
                connection->Close();
                throw ex;
            }

            FdoStringP expectedErrMsg = FdoStringP::Format(L"Incomplete date/time setting.");

            if ((wcscmp(ex->GetExceptionMessage(), expectedErrMsg)) != 0)
            {
                wprintf(L"    Unexpected Exception: %ls\n", ex->GetExceptionMessage());
                connection->Close();
                throw ex;
            }
            else
            {
                wprintf(L"    Expected Exception: %ls\n", ex->GetExceptionMessage());
                FDO_SAFE_RELEASE(ex);
            }
        }
        catch ( ... )
        {
            wprintf(L"    Unexpected Exception in insertDate()\n");
            connection->Close();
            throw;
        }
    }

    wprintf(L"  > Test Verification.\n");
    FdoInsertTest::insertDateVerification(connection, 2);

    wprintf(L"  > Test Cleanup.\n");
    FdoInsertTest::insertDateCleanUp(connection);

    connection->Close();
}

// Tests the following scenario
//  - datastore has Table A and Table B, each with an autoincremented column
//  - Table A has an insertion trigger that inserts a row in Table B
//  - An FdoIInsert is done to insert a feature in Table A
//
//  The test verifies that:
//  - an infinite loop does not occur when multiple features are inserted
//  - the FdoIFeatureReader from FdoIInsert retrieves the autoincremented column
//    value for the new row in Table A, rather than Table B
void SqlServerFdoInsertTest::testDefect1206136()
{
	FdoPtr<FdoIConnection> connection;
    StaticConnection* staticConn = UnitTestUtil::NewStaticConnection();

    try {
        printf( "\nCreating Datastore ...\n" );

        staticConn->connect();
        FdoSchemaManagerP mgr = staticConn->CreateSchemaManager();

        UnitTestUtil::CreateDBNoMeta( 
            mgr,
            UnitTestUtil::GetEnviron("datastore", mDefect1206136Suffix)
        );

        FdoStringP datastore = UnitTestUtil::GetEnviron("datastore", mDefect1206136Suffix);

        printf( "\nAdding Schema ...\n" );

        FdoSmPhMgrP phMgr = mgr->GetPhysicalSchema();

        FdoSmPhOwnerP owner = phMgr->FindOwner( datastore, L"", false );
        FdoSmPhGrdOwnerP grdOwner = owner->SmartCast<FdoSmPhGrdOwner>();

        grdOwner->ActivateAndExecute(
            L"CREATE TABLE [dbo].[S_LOT](\
           	[FeatID] [bigint] IDENTITY(1,1) NOT NULL,\
	        [geometry] [geometry] NULL,\
	        [obj_id] [decimal](18, 0) NULL,\
        	[name] [nvarchar](128) NULL,\
        	[FeatureSource] [nvarchar](50) NULL,\
             CONSTRAINT [pk_dbo_s_lot] PRIMARY KEY CLUSTERED \
            (\
            	[FeatID] ASC\
            )WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]\
            ) ON [PRIMARY]"
        );

        grdOwner->ActivateAndExecute(
            L"CREATE TABLE [dbo].[LOG_S_LOT](\
        	[AuditID] [int] IDENTITY(1000,1) NOT NULL,\
        	[OperationTypeID] [int] NOT NULL,\
        	[obj_id] [decimal](18, 0) NULL,\
        	[featid] [bigint] NULL,\
        	[name] [varchar](128) NULL,\
        	[FeatureSource] [varchar](50) NULL,\
            CONSTRAINT [PK__LOG_S_LO__A17F23B85D6C935F] PRIMARY KEY CLUSTERED \
            (\
        	    [AuditID] ASC\
            )WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]\
            ) ON [PRIMARY]"
        );

        grdOwner->ActivateAndExecute(
            L"CREATE TRIGGER [dbo].[TR_LOG_S_LOT_INSERT] ON [dbo].[S_LOT]\
            FOR INSERT\
            AS\
            BEGIN\
	            DECLARE @operationID int \
	            SET @operationID=1\
	            DECLARE   @obj_id decimal, @featid bigint, @name varchar(128), @FeatureSource varchar(50)\
	            SELECT  @obj_id=obj_id, @featid=FeatID, @name=name, @FeatureSource=FeatureSource\
	            FROM INSERTED\
                \
	            INSERT INTO LOG_S_LOT(OperationTypeID, obj_id, featid, name, FeatureSource)	\
	            VALUES(1, 1.0, 1, 'N1', 'A')\
            END"
        );

        owner->Commit();

        grdOwner = NULL;
        owner = NULL;
        phMgr = NULL;
        mgr = NULL;
        staticConn->disconnect();
        delete staticConn;
        staticConn = NULL;

        printf( "\nInserting Features ...\n" );
        connection = UnitTestUtil::CreateConnection(
            false,
            false,
            mDefect1206136Suffix
        );

        FdoPtr<FdoITransaction> trans = connection->BeginTransaction();
        FdoPtr<FdoIInsert> insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
        insertCommand->SetFeatureClassName(L"dbo:S_LOT");

        int i;
        int idList[5];

        for (i = 0; i < 5; i++)
        {
            FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();
            FdoPtr<FdoDataValue> dataValue;
            FdoPtr<FdoPropertyValue> propertyValue;

            dataValue = FdoDataValue::Create( FdoStringP::Format(L"Item%d", i));
            propertyValue = AddNewProperty( propertyValues, L"name");
            propertyValue->SetValue(dataValue);

            FdoPtr<FdoIFeatureReader> rdr = insertCommand->Execute();
            rdr->ReadNext();

            idList[i] = (int) rdr->GetInt64(L"FeatID");
        }

        trans->Commit();

        FdoPtr<FdoISelect> select = (FdoISelect*)connection->CreateCommand(FdoCommandType_Select); 

        select->SetFeatureClassName(L"dbo:S_LOT");

        FdoPtr<FdoIFeatureReader> rdr2 = select->Execute();

        int count = 0;
        while (rdr2->ReadNext())
        {
            int id = (int)rdr2->GetInt64(L"FeatID");

            bool found = false;

            for ( i = 0; i < 5; i++ ) 
            {
                if ( id == idList[i] ) 
                {
                    found = true;
                    break;
                }
            }

            CPPUNIT_ASSERT( found );
            count++;
        }

        CPPUNIT_ASSERT( count == 5 );

        connection->Close();

        printf( "\nDone\n" );
    }
    catch (FdoException *ex)
    {
        try {
            if( connection )
                connection->Close ();
            if (staticConn != NULL)
            {
                staticConn->disconnect();
                delete staticConn;
            }
        }
        catch ( ... )
        {
        }
        UnitTestUtil::FailOnException(ex);
    }
    catch (...)
    {
        try {
            if( connection )
                connection->Close ();
            if (staticConn != NULL)
            {
                staticConn->disconnect();
                delete staticConn;
            }
        }
        catch ( ... )
        {
        }
        throw;
    }
}

