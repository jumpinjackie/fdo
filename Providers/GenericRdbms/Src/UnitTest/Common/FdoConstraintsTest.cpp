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

#define			DB_NAME_CONSTRAINTS_SUFFIX L"_constraints"

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

void FdoConstraintsTest::CreateConnection( Context& context, FdoBoolean recreateDb )
{
    FdoBoolean lRecreateDb = recreateDb && RECREATE_CONSTRAINTS_DB;
    FdoBoolean destroySchema = recreateDb && !RECREATE_CONSTRAINTS_DB;

    // delete, re-create and open the datastore
	context.connection = UnitTestUtil::CreateConnection(
		lRecreateDb,
		lRecreateDb,
        DB_NAME_CONSTRAINTS_SUFFIX,
        NULL,
        NULL,
        context.ltMethod
    );

    if ( destroySchema )	{
		try {
			// Drop the schema
			FdoPtr<FdoIDestroySchema>  pCmd = (FdoIDestroySchema*) context.connection->CreateCommand(FdoCommandType_DestroySchema);
			FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) context.connection->CreateCommand(FdoCommandType_DescribeSchema);
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

	success &= TestParser(L"INTLIST = 30 or INTLIST = 20 or INTLIST = 10");
	success &= TestParser(L"INTRANGE >= 10 and INTRANGE < 20");
	success &= TestParser(L"STRINGLIST = 'close' or STRINGLIST = 'open'");

	// Test for the stack limit with 260 items
	success &= TestParser(L"([List]= '_260' OR [List]= '_259' OR [List]= '_258' OR [List]= '_257' OR [List]= '_256' OR [List]= '_255' OR [List]= '_254' OR [List]= '_253' OR [List]= '_252' OR [List]= '_251' OR [List]= '_250' OR [List]= '_249' OR [List]= '_248' OR [List]= '_247' OR [List]= '_246' OR [List]= '_245' OR [List]= '_244' OR [List]= '_243' OR [List]= '_242' OR [List]= '_241' OR [List]= '_240' OR [List]= '_239' OR [List]= '_238' OR [List]= '_237' OR [List]= '_236' OR [List]= '_235' OR [List]= '_234' OR [List]= '_233' OR [List]= '_232' OR [List]= '_231' OR [List]= '_230' OR [List]= '_229' OR [List]= '_228' OR [List]= '_227' OR [List]= '_226' OR [List]= '_225' OR [List]= '_224' OR [List]= '_223' OR [List]= '_222' OR [List]= '_221' OR [List]= '_220' OR [List]= '_219' OR [List]= '_218' OR [List]= '_217' OR [List]= '_216' OR [List]= '_215' OR [List]= '_214' OR [List]= '_213' OR [List]= '_212' OR [List]= '_211' OR [List]= '_210' OR [List]= '_209' OR [List]= '_208' OR [List]= '_207' OR [List]= '_206' OR [List]= '_205' OR [List]= '_204' OR [List]= '_203' OR [List]= '_202' OR [List]= '_201' OR [List]= '_200' OR [List]= '_199' OR [List]= '_198' OR [List]= '_197' OR [List]= '_196' OR [List]= '_195' OR [List]= '_194' OR [List]= '_193' OR [List]= '_192' OR [List]= '_191' OR [List]= '_190' OR [List]= '_189' OR [List]= '_188' OR [List]= '_187' OR [List]= '_186' OR [List]= '_185' OR [List]= '_184' OR [List]= '_183' OR [List]= '_182' OR [List]= '_181' OR [List]= '_180' OR [List]= '_179' OR [List]= '_178' OR [List]= '_177' OR [List]= '_176' OR [List]= '_175' OR [List]= '_174' OR [List]= '_173' OR [List]= '_172' OR [List]= '_171' OR [List]= '_170' OR [List]= '_169' OR [List]= '_168' OR [List]= '_167' OR [List]= '_166' OR [List]= '_165' OR [List]= '_164' OR [List]= '_163' OR [List]= '_162' OR [List]= '_161' OR [List]= '_160' OR [List]= '_159' OR [List]= '_158' OR [List]= '_157' OR [List]= '_156' OR [List]= '_155' OR [List]= '_154' OR [List]= '_153' OR [List]= '_152' OR [List]= '_151' OR [List]= '_150' OR [List]= '_149' OR [List]= '_148' OR [List]= '_147' OR [List]= '_146' OR [List]= '_145' OR [List]= '_144' OR [List]= '_143' OR [List]= '_142' OR [List]= '_141' OR [List]= '_140' OR [List]= '_139' OR [List]= '_138' OR [List]= '_137' OR [List]= '_136' OR [List]= '_135' OR [List]= '_134' OR [List]= '_133' OR [List]= '_132' OR [List]= '_131' OR [List]= '_130' OR [List]= '_129' OR [List]= '_128' OR [List]= '_127' OR [List]= '_126' OR [List]= '_125' OR [List]= '_124' OR [List]= '_123' OR [List]= '_122' OR [List]= '_121' OR [List]= '_120' OR [List]= '_119' OR [List]= '_118' OR [List]= '_117' OR [List]= '_116' OR [List]= '_115' OR [List]= '_114' OR [List]= '_113' OR [List]= '_112' OR [List]= '_111' OR [List]= '_110' OR [List]= '_109' OR [List]= '_108' OR [List]= '_107' OR [List]= '_106' OR [List]= '_105' OR [List]= '_104' OR [List]= '_103' OR [List]= '_102' OR [List]= '_101' OR [List]= '_100' OR [List]= '_99' OR [List]= '_98' OR [List]= '_97' OR [List]= '_96' OR [List]= '_95' OR [List]= '_94' OR [List]= '_93' OR [List]= '_92' OR [List]= '_91' OR [List]= '_90' OR [List]= '_89' OR [List]= '_88' OR [List]= '_87' OR [List]= '_86' OR [List]= '_85' OR [List]= '_84' OR [List]= '_83' OR [List]= '_82' OR [List]= '_81' OR [List]= '_80' OR [List]= '_79' OR [List]= '_78' OR [List]= '_77' OR [List]= '_76' OR [List]= '_75' OR [List]= '_74' OR [List]= '_73' OR [List]= '_72' OR [List]= '_71' OR [List]= '_70' OR [List]= '_69' OR [List]= '_68' OR [List]= '_67' OR [List]= '_66' OR [List]= '_65' OR [List]= '_64' OR [List]= '_63' OR [List]= '_62' OR [List]= '_61' OR [List]= '_60' OR [List]= '_59' OR [List]= '_58' OR [List]= '_57' OR [List]= '_56' OR [List]= '_55' OR [List]= '_54' OR [List]= '_53' OR [List]= '_52' OR [List]= '_51' OR [List]= '_50' OR [List]= '_49' OR [List]= '_48' OR [List]= '_47' OR [List]= '_46' OR [List]= '_45' OR [List]= '_44' OR [List]= '_43' OR [List]= '_42' OR [List]= '_41' OR [List]= '_40' OR [List]= '_39' OR [List]= '_38' OR [List]= '_37' OR [List]= '_36' OR [List]= '_35' OR [List]= '_34' OR [List]= '_33' OR [List]= '_32' OR [List]= '_31' OR [List]= '_30' OR [List]= '_29' OR [List]= '_28' OR [List]= '_27' OR [List]= '_26' OR [List]= '_25' OR [List]= '_24' OR [List]= '_23' OR [List]= '_22' OR [List]= '_21' OR [List]= '_20' OR [List]= '_19' OR [List]= '_18' OR [List]= '_17' OR [List]= '_16' OR [List]= '_15' OR [List]= '_14' OR [List]= '_13' OR [List]= '_12' OR [List]= '_11' OR [List]= '_10' OR [List]= '_9' OR [List]= '_8' OR [List]= '_7' OR [List]= '_6' OR [List]= '_5' OR [List]= '_4' OR [List]= '_3' OR [List]= '_2' OR [List]= '_1')");

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

FdoDouble FdoConstraintsTest::GetDoubleRounding( FdoDataType dataType )
{
    if ( dataType == FdoDataType_Decimal ) 
        return (FdoDouble) 0.05;
    else
        return (FdoDouble) 0.000000000001;
}


