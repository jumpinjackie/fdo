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

#define			RECREATE_CONSTRAINTS_DB		true

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

FdoIConnection* FdoConstraintsTest::CreateConnection( FdoBoolean recreateDb )
{
	FdoIConnection* connection;

    FdoBoolean lRecreateDb = recreateDb && RECREATE_CONSTRAINTS_DB;
    FdoBoolean destroySchema = recreateDb && !RECREATE_CONSTRAINTS_DB;

    // delete, re-create and open the datastore
	connection = UnitTestUtil::CreateConnection(
		lRecreateDb,
		lRecreateDb,
        DB_NAME_CONSTRAINTS_SUFFIX,
        NULL,
        NULL,
        0
    );

    if ( destroySchema )	{
		try {
			// Drop the schema
			FdoPtr<FdoIDestroySchema>  pCmd = (FdoIDestroySchema*) connection->CreateCommand(FdoCommandType_DestroySchema);
			FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
            FdoFeatureSchemasP schemas = pDescCmd->Execute();
            FdoInt32 idx;
            for ( idx = 0; idx < schemas->GetCount(); idx++ ) {
                pCmd->SetSchemaName( FdoFeatureSchemaP(schemas->GetItem(idx))->GetName() );
                pCmd->Execute();
            }
		} catch (...) {
			// ignore
		}
	}
    
    return connection;
}

void FdoConstraintsTest::TestParser()
{
	bool	success = true;

	success &= TestParser(L"([INTLIST] = (30) or [INTLIST] = (20) or [INTLIST] = (10))");

	success &= TestParser(L"([INTLIST] = 30 or ([INTLIST] = 20 or [INTLIST] = 10))");
	success &= TestParser(L"([INTRANGE] >= 10 and [INTRANGE] < 20))");
	success &= TestParser(L"([STRINGLIST] = 'close' or [STRINGLIST] = 'open')");

	success &= TestParser(L"(INTLIST = 30 or (INTLIST = 20 or INTLIST = 10))");
	success &= TestParser(L"(INTRANGE >= 10 and INTRANGE < 20))");
	success &= TestParser(L"(STRINGLIST = 'close' or STRINGLIST = 'open')");

	success &= TestParser(L"INTLIST = 30 or INTLIST = 20 or INTLIST = 10)");
	success &= TestParser(L"INTRANGE >= 10 and INTRANGE < 20");
	success &= TestParser(L"STRINGLIST = 'close' or STRINGLIST = 'open'");

	CPPUNIT_ASSERT_MESSAGE("Constraints parser failed", success == true );
}

bool FdoConstraintsTest::TestParser(FdoString* clause)
{
	bool			ret = false;
	FdoCommonParse* parser = NULL;

	DBG(printf("'%ls'\n", clause ));
	try
	{
		parser = new FdoCommonParse();

		parser->ParseConstraint( clause );

		delete parser;
		ret = true;

	} catch (FdoException *ex) {
		delete parser;
		DBG(printf("Parse failed: %ls\n", (FdoString* )ex->GetExceptionMessage()));
		ex->Release();
	}
	return ret;
}

FdoBoolean FdoConstraintsTest::CanRestrictCheckConstraint()
{
    return true;
}
