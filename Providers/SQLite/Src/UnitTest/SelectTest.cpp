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
#include "SelectTest.h"
#include "UnitTestUtil.h"
#include <ctime>
#include <cppunit/extensions/HelperMacros.h>
#include "FdoCommonFile.h"

#ifdef _WIN32
static const wchar_t* SC_TEST_FILE = L"..\\..\\TestData\\SelectTest.sqlite";
static const wchar_t* SRC_TEST_FILE = L"..\\..\\TestData\\PARCEL_Source.sqlite";
#else
#include <unistd.h>
static const wchar_t* SC_TEST_FILE = L"../../TestData/SelectTest.sqlite";
static const wchar_t* SRC_TEST_FILE = L"../../TestData/PARCEL_Source.sqlite";
#endif


CPPUNIT_TEST_SUITE_REGISTRATION( SelectTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SelectTest, "SelectTest");

SelectTest::SelectTest(void)
{
    
}

SelectTest::~SelectTest(void)
{
    
}


void SelectTest::setUp ()
{

}

void SelectTest::tearDown ()
{
}

void SelectTest::TestSimpleSelect ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
	    FdoPtr<FdoISelect> SelectCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
	    SelectCmd->SetFeatureClassName(L"DaKlass");

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"ID < 30");
        SelectCmd->SetFilter(filter);

        FdoPtr<FdoIFeatureReader>reader = SelectCmd->Execute();
        int rez = 0;
        while(reader->ReadNext())rez++;
        printf ("Selectd features : %d\n", rez);
        CPPUNIT_ASSERT(rez == 29);
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
	printf( "Done\n" );
}

void SelectTest::TestBindSelect ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
	    FdoPtr<FdoISelect> SelectCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
	    SelectCmd->SetFeatureClassName(L"DaKlass");

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"ID < :parm");
        SelectCmd->SetFilter(filter);

        FdoPtr<FdoParameterValueCollection>parms = SelectCmd->GetParameterValues();
        FdoPtr<FdoInt32Value> intval = FdoInt32Value::Create(30);
        FdoPtr<FdoParameterValue>parm = FdoParameterValue::Create(L"parm",intval);
        parms->Add(parm);
        FdoPtr<FdoIFeatureReader>reader = SelectCmd->Execute();
        int rez = 0;
        while(reader->ReadNext())rez++;
        printf ("Selectd features : %d\n", rez);
        CPPUNIT_ASSERT(rez == 29);
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
	printf( "Done\n" );
}

void SelectTest::TestSpatialSelect ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
	    FdoPtr<FdoISelect> SelectCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
	    SelectCmd->SetFeatureClassName(L"DaKlass");

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"ID > :lowerbound AND ID < :upperbound AND Data2 INSIDE GeomFromText('POLYGON XYZ ((7.1770013502456 43.7501967446194 0, 7.1770013502456 43.6912771493358 0, 7.27407112243824 43.6912771493358 0, 7.27407112243824 43.7501967446194 0, 7.1770013502456 43.7501967446194 0))')");
        SelectCmd->SetFilter(filter);
        
        FdoPtr<FdoParameterValueCollection>parms = SelectCmd->GetParameterValues();
        FdoPtr<FdoInt32Value> intval;
        FdoPtr<FdoParameterValue>parm;

        // Add the bound paramaters in the reverse order.
        intval = FdoInt32Value::Create(10000);
        parm = FdoParameterValue::Create(L"upperbound",intval);
        parms->Add(parm);

        intval = FdoInt32Value::Create(5000);
        parm = FdoParameterValue::Create(L"lowerbound",intval);
        parms->Add(parm);

        FdoPtr<FdoIFeatureReader>reader = SelectCmd->Execute();
        int rez = 0;
        while(reader->ReadNext())rez++;
        
        printf ("Selectd features : %d\n", rez);
        CPPUNIT_ASSERT(rez == 4954);
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
	printf( "Done\n" );
}

void SelectTest::TestComplexSelect ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
	    FdoPtr<FdoISelect> SelectCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
	    SelectCmd->SetFeatureClassName(L"DaKlass");

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"ID < 10000 AND Data2 INSIDE GeomFromText('POLYGON XYZ ((7.1770013502456 43.7501967446194 0, 7.1770013502456 43.6912771493358 0, 7.27407112243824 43.6912771493358 0, 7.27407112243824 43.7501967446194 0, 7.1770013502456 43.7501967446194 0))')");
        SelectCmd->SetFilter(filter);

        FdoPtr<FdoIFeatureReader>reader = SelectCmd->Execute();
        int rez = 0;
        while(reader->ReadNext())rez++;
        printf ("Selectd features : %d\n", rez);
        CPPUNIT_ASSERT(rez == 9471);
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
	printf( "Done\n" );
}

void SelectTest::TestComplexWithBindSelect ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
	    FdoPtr<FdoISelect> SelectCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
	    SelectCmd->SetFeatureClassName(L"DaKlass");

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"ID < :parm AND Data2 INSIDE GeomFromText('POLYGON XYZ ((7.1770013502456 43.7501967446194 0, 7.1770013502456 43.6912771493358 0, 7.27407112243824 43.6912771493358 0, 7.27407112243824 43.7501967446194 0, 7.1770013502456 43.7501967446194 0))')");
        SelectCmd->SetFilter(filter);

        FdoPtr<FdoParameterValueCollection>parms = SelectCmd->GetParameterValues();
        FdoPtr<FdoInt32Value> intval = FdoInt32Value::Create(10000);
        FdoPtr<FdoParameterValue>parm = FdoParameterValue::Create(L"parm",intval);
        parms->Add(parm);
        FdoPtr<FdoIFeatureReader>reader = SelectCmd->Execute();
        int rez = 0;
        while(reader->ReadNext())rez++;
        printf ("Selectd features : %d\n", rez);
        CPPUNIT_ASSERT(rez == 9471);
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
	printf( "Done\n" );
}

void SelectTest::BooleanDataTest ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
		conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, true, true);
		 
        //apply schema
		FdoPtr<FdoIApplySchema> applyschema = static_cast<FdoIApplySchema*>(conn->CreateCommand(FdoCommandType_ApplySchema));
        FdoPtr<FdoFeatureSchemaCollection> schColl = FdoFeatureSchemaCollection::Create(NULL);
        schColl->ReadXml(L"SchBooleanTest.xml");
        CPPUNIT_ASSERT(schColl->GetCount() == 1);
        
        FdoPtr<FdoFeatureSchema> schema = schColl->GetItem(0);
		applyschema->SetFeatureSchema(schema);
		applyschema->Execute();

        {
            FdoPtr<FdoIInsert> insCmd = static_cast<FdoIInsert*>(conn->CreateCommand(FdoCommandType_Insert));
            FdoPtr<FdoPropertyValueCollection> vals = insCmd->GetPropertyValues();
            FdoPtr<FdoPropertyValue> propIns;
            
		    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
		    double coords[] = { 7.2068, 43.7556, 
							    7.2088, 43.7556, 
							    7.2088, 43.7574, 
							    7.2068, 43.7574, 
							    7.2068, 43.7556 }; 
		    FdoPtr<FdoILinearRing> outer = gf->CreateLinearRing(0, 10, coords);
		    FdoPtr<FdoIPolygon> poly = gf->CreatePolygon(outer, NULL);
		    FdoPtr<FdoByteArray> polyfgf = gf->GetFgf(poly);
		    FdoPtr<FdoGeometryValue> gv = FdoGeometryValue::Create(polyfgf);

            FdoPtr<FdoPropertyValue> propGeomIns = FdoPropertyValue::Create(L"Geometry", gv);
            vals->Add(propGeomIns);
            
            FdoPtr<FdoBooleanValue> boolVal = FdoBooleanValue::Create(true);
            FdoPtr<FdoPropertyValue> propBoolIns = FdoPropertyValue::Create(L"BoolProp", boolVal);
            vals->Add(propBoolIns);

            FdoPtr<FdoStringValue> strVal = FdoStringValue::Create(L"sky is blue");
            FdoPtr<FdoPropertyValue> propStrIns = FdoPropertyValue::Create(L"Name", strVal);
            vals->Add(propStrIns);

            insCmd->SetFeatureClassName(L"BoolTest");
            
            FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());
            rdr->Close();
            
            boolVal->SetBoolean(false);
            strVal->SetString(L"water is red");
            rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());
            rdr->Close();

            boolVal->SetNull();
            strVal->SetString(L"snow is white or black");
            rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());
            rdr->Close();
        }

        FdoPtr<FdoISelect> selectCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 

        FdoPtr<FdoIdentifierCollection> props =  selectCmd->GetPropertyNames();
        props->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"FeatId")));
        props->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Geometry")));
        props->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"BoolProp")));
        props->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Name")));
        FdoPtr<FdoComputedIdentifier> cmpid = FdoComputedIdentifier::Create(L"JustTrue", FdoPtr<FdoExpression>(FdoExpression::Parse(L"TRUE")));
        props->Add(cmpid);

        FdoPtr<FdoComputedIdentifier> cmpid2 = FdoComputedIdentifier::Create(L"CanBe", FdoPtr<FdoExpression>(FdoExpression::Parse(L"concat(Name, ' and this can be ', TRUE)")));
        props->Add(cmpid2);
        
        FdoPtr<FdoComputedIdentifier> cmpid3 = FdoComputedIdentifier::Create(L"ConcText", FdoPtr<FdoExpression>(FdoExpression::Parse(L"concat(Name, ' and this is ', BoolProp)")));
        props->Add(cmpid3);

        selectCmd->SetFeatureClassName(L"BoolTest");
        FdoPtr<FdoIFeatureReader>reader = selectCmd->Execute();
        int idx = 0;
        while(reader->ReadNext())
        {
            if (!reader->IsNull(L"JustTrue"))
            {
                bool justTrue = reader->GetBoolean(L"JustTrue");
                CPPUNIT_ASSERT(justTrue);
            }
            else
            {
                CPPUNIT_FAIL ("Invalid result data");
            }
            if (!reader->IsNull(L"CanBe"))
            {
                FdoStringP canBe = reader->GetString(L"CanBe");
                switch(idx)
                {
                case 0:
                    CPPUNIT_ASSERT(canBe == L"sky is blue and this can be 1");
                    break;
                case 1:
                    CPPUNIT_ASSERT(canBe == L"water is red and this can be 1");
                    break;
                case 2:
                    CPPUNIT_ASSERT(canBe == L"snow is white or black and this can be 1");
                    break;
                default:
                    CPPUNIT_FAIL ("Invalid row index");
                }
            }
            else
            {
                CPPUNIT_FAIL ("Invalid result data");
            }
            if (!reader->IsNull(L"ConcText"))
            {
                FdoStringP concText = reader->GetString(L"ConcText");
                switch(idx)
                {
                case 0:// bool is TRUE
                    CPPUNIT_ASSERT(concText == L"sky is blue and this is 1");
                    break;
                case 1:// bool is FALSE
                    CPPUNIT_ASSERT(concText == L"water is red and this is 0");
                    break;
                case 2: // bool is NULL
                    CPPUNIT_ASSERT(concText == L"snow is white or black and this is ");
                    break;
                default:
                    CPPUNIT_FAIL ("Invalid row index");
                }
            }
            else
            {
                CPPUNIT_FAIL ("Invalid result data");
            }
            idx++;
        }
        reader->Close();
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
		
	printf( "Done\n" );
}

void SelectTest::TestAggregatesSelect ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
        FdoPtr<FdoISelectAggregates> selectCmd = static_cast<FdoISelectAggregates*>(conn->CreateCommand(FdoCommandType_SelectAggregates));
	    selectCmd->SetFeatureClassName(L"DaKlass");

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"ID > 1");
        selectCmd->SetFilter(filter);
        
        FdoPtr<FdoIdentifierCollection> idfc = selectCmd->GetPropertyNames();
        FdoPtr<FdoIdentifier> idf = FdoIdentifier::Create(L"Name");
        idfc->Add(idf);
        FdoPtr<FdoExpression> exp1 = FdoExpression::Parse(L"ID * 12.44 - 4.5");
        FdoPtr<FdoComputedIdentifier> cidf = FdoComputedIdentifier::Create(L"dbVal", exp1);
        idfc->Add(cidf);

        FdoPtr<FdoIdentifierCollection> idfcg = selectCmd->GetGrouping();
        FdoPtr<FdoIdentifier> idfg = FdoIdentifier::Create(L"Name");
        idfcg->Add(idfg);
        FdoPtr<FdoFilter> grFilter = FdoFilter::Parse(L"Name='AB0026'");
        selectCmd->SetGroupingFilter(grFilter);
        
        FdoPtr<FdoIdentifierCollection> idfco = selectCmd->GetOrdering();
        idfco->Add(cidf);

        FdoPtr<FdoIDataReader> rdr = selectCmd->Execute();
        int idx = 0;
        while(rdr->ReadNext())
        {
            FdoDataType dt = rdr->GetDataType(L"dbVal");
            FdoString* name = rdr->GetString(L"Name");
            double dbVal = rdr->GetDouble(L"dbVal");
            printf("[%d] = '%ls' & %g", idx++, name, dbVal);
        }
        rdr->Close();
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
	printf( "Done\n" );
}

void SelectTest::TestSelectInsertLowerId ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
        FdoPtr<FdoFilter> qfilter = FdoFilter::Parse(L"Data2 INSIDE GeomFromText(\'POLYGON XYZ ((7.2946747590295 43.7578741868765 0, 7.2946747590295 43.7391013130616 0, 7.33112234364711 43.7391013130616 0, 7.33112234364711 43.7578741868765 0, 7.2946747590295 43.7578741868765 0))\')");
	    FdoPtr<FdoISelect> selectCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
	    selectCmd->SetFeatureClassName(L"DaKlass");
        selectCmd->SetFilter(qfilter);

        // step (1) how many objects are there
        {
            FdoPtr<FdoIFeatureReader>reader = selectCmd->Execute();
            int rez = 0;
            while(reader->ReadNext())rez++;
            reader->Close();
            printf ("Selectd features step (I): %d\n", rez);
            CPPUNIT_ASSERT(rez == 616);
        }
        FdoPtr<FdoFilter> outfilter = FdoFilter::Parse(L"Data2 INSIDE GeomFromText(\'POLYGON XYZ ((7.32293365332046 43.7431125935668 0, 7.31362107201967 43.7362131560892 0, 7.33337016667614 43.7291532665027 0, 7.32919556122942 43.7431125935668 0, 7.32518151743022 43.7431125935668 0, 7.32293365332046 43.7431125935668 0))\')");
        FdoSpatialCondition* outsp = static_cast<FdoSpatialCondition*>(outfilter.p);
        FdoPtr<FdoGeometryValue> out_geom_value = static_cast<FdoGeometryValue*>(outsp->GetGeometry());

        FdoPtr<FdoFilter> infilter = FdoFilter::Parse(L"Data2 INSIDE GeomFromText(\'POLYGON XYZ ((7.32084638976835 43.7546651639026 0, 7.31747455597957 43.7480866303767 0, 7.32084638976835 43.7448776074028 0, 7.32534216815341 43.7490493571394 0, 7.32309427896089 43.7516165622715 0, 7.32084638976835 43.7546651639026 0))\')");
        FdoSpatialCondition* insp = static_cast<FdoSpatialCondition*>(infilter.p);
        FdoPtr<FdoGeometryValue> in_geom_value = static_cast<FdoGeometryValue*>(insp->GetGeometry());

        // step (2) insert outside geometry and test again after
        {
            FdoPtr<FdoITransaction> tr = conn->BeginTransaction();
            FdoPtr<FdoIInsert> insCmd = static_cast<FdoIInsert*>(conn->CreateCommand(FdoCommandType_Insert));
            insCmd->SetFeatureClassName(L"DaKlass");
            FdoPtr<FdoPropertyValueCollection> vals = insCmd->GetPropertyValues();
            
            FdoPtr<FdoInt32Value> int32Value = FdoInt32Value::Create(35525);
            FdoPtr<FdoPropertyValue> idProp = FdoPropertyValue::Create(L"FeatId", int32Value);
            vals->Add(idProp);            
            FdoPtr<FdoPropertyValue> propGeomIns = FdoPropertyValue::Create(L"Data2", out_geom_value);
            vals->Add(propGeomIns);
            FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());
            tr->Commit();
        }
        // step (3) again how many objects are there
        {
            FdoPtr<FdoIFeatureReader>reader = selectCmd->Execute();
            int rez = 0;
            while(reader->ReadNext())rez++;
            reader->Close();
            printf ("Selectd features step (II): %d\n", rez);
            CPPUNIT_ASSERT(rez == 616); // should remain the same since new geom added is outside of spatial query
        }
        // step (3) insert inside geometry and test again after
        {
            FdoPtr<FdoITransaction> tr = conn->BeginTransaction();
            FdoPtr<FdoIInsert> insCmd = static_cast<FdoIInsert*>(conn->CreateCommand(FdoCommandType_Insert));
            insCmd->SetFeatureClassName(L"DaKlass");
            FdoPtr<FdoPropertyValueCollection> vals = insCmd->GetPropertyValues();
            
            FdoPtr<FdoInt32Value> int32Value = FdoInt32Value::Create(35521);
            FdoPtr<FdoPropertyValue> idProp = FdoPropertyValue::Create(L"FeatId", int32Value);
            vals->Add(idProp);            
            FdoPtr<FdoPropertyValue> propGeomIns = FdoPropertyValue::Create(L"Data2", in_geom_value);
            vals->Add(propGeomIns);
            FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());
            tr->Commit();
        }
        // step (3) again how many objects are there
        {
            FdoPtr<FdoIFeatureReader>reader = selectCmd->Execute();
            int rez = 0;
            while(reader->ReadNext())rez++;
            reader->Close();
            printf ("Selectd features step (III): %d\n", rez);
            CPPUNIT_ASSERT(rez == 617); // should increase with one since new geom added is outside of spatial query
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
	printf( "Done\n" );
}

void SelectTest::TestSelectInsertLowerIdSQL ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
        FdoPtr<FdoFilter> qfilter = FdoFilter::Parse(L"Data2 INSIDE GeomFromText(\'POLYGON XYZ ((7.2946747590295 43.7578741868765 0, 7.2946747590295 43.7391013130616 0, 7.33112234364711 43.7391013130616 0, 7.33112234364711 43.7578741868765 0, 7.2946747590295 43.7578741868765 0))\')");
	    FdoPtr<FdoISelect> selectCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
	    selectCmd->SetFeatureClassName(L"DaKlass");
        selectCmd->SetFilter(qfilter);

        // step (1) how many objects are there
        {
            FdoPtr<FdoIFeatureReader>reader = selectCmd->Execute();
            int rez = 0;
            while(reader->ReadNext())rez++;
            reader->Close();
            printf ("Selectd features step (I): %d\n", rez);
            CPPUNIT_ASSERT(rez == 616);
        }
        FdoPtr<FdoFilter> outfilter = FdoFilter::Parse(L"Data2 INSIDE GeomFromText(\'POLYGON XYZ ((7.32293365332046 43.7431125935668 0, 7.31362107201967 43.7362131560892 0, 7.33337016667614 43.7291532665027 0, 7.32919556122942 43.7431125935668 0, 7.32518151743022 43.7431125935668 0, 7.32293365332046 43.7431125935668 0))\')");
        FdoSpatialCondition* outsp = static_cast<FdoSpatialCondition*>(outfilter.p);
        FdoPtr<FdoGeometryValue> out_geom_value = static_cast<FdoGeometryValue*>(outsp->GetGeometry());

        FdoPtr<FdoFilter> infilter = FdoFilter::Parse(L"Data2 INSIDE GeomFromText(\'POLYGON XYZ ((7.32084638976835 43.7546651639026 0, 7.31747455597957 43.7480866303767 0, 7.32084638976835 43.7448776074028 0, 7.32534216815341 43.7490493571394 0, 7.32309427896089 43.7516165622715 0, 7.32084638976835 43.7546651639026 0))\')");
        FdoSpatialCondition* insp = static_cast<FdoSpatialCondition*>(infilter.p);
        FdoPtr<FdoGeometryValue> in_geom_value = static_cast<FdoGeometryValue*>(insp->GetGeometry());

        // step (2) insert outside geometry and test again after
        {
            FdoPtr<FdoITransaction> tr = conn->BeginTransaction();
            FdoPtr<FdoISQLCommand> sqlCmd = static_cast<FdoISQLCommand*>(conn->CreateCommand(FdoCommandType_SQLCommand));
            sqlCmd->SetSQLStatement(L"insert into DaKlass (FeatId,Data2) values (?,?)");
            FdoPtr<FdoParameterValueCollection> parmVals = sqlCmd->GetParameterValues();
            
            FdoPtr<FdoInt32Value> int32Value = FdoInt32Value::Create(35525);
            FdoPtr<FdoParameterValue> idProp = FdoParameterValue::Create(L"FeatId", int32Value);
            parmVals->Add(idProp);            
            FdoPtr<FdoParameterValue> propGeomIns = FdoParameterValue::Create(L"Data2", out_geom_value);
            parmVals->Add(propGeomIns);
            CPPUNIT_ASSERT(sqlCmd->ExecuteNonQuery() == 1 );
            tr->Commit();
        }
        // step (3) again how many objects are there
        {
            FdoPtr<FdoIFeatureReader>reader = selectCmd->Execute();
            int rez = 0;
            while(reader->ReadNext())rez++;
            reader->Close();
            printf ("Selectd features step (II): %d\n", rez);
            CPPUNIT_ASSERT(rez == 616); // should remain the same since new geom added is outside of spatial query
        }
        // step (3) insert inside geometry and test again after
        {
            FdoPtr<FdoITransaction> tr = conn->BeginTransaction();
            FdoPtr<FdoISQLCommand> sqlCmd = static_cast<FdoISQLCommand*>(conn->CreateCommand(FdoCommandType_SQLCommand));
            sqlCmd->SetSQLStatement(L"insert into DaKlass (FeatId,Data2) values (?,?)");
            FdoPtr<FdoParameterValueCollection> parmVals = sqlCmd->GetParameterValues();
            
            FdoPtr<FdoInt32Value> int32Value = FdoInt32Value::Create(35521);
            FdoPtr<FdoParameterValue> idProp = FdoParameterValue::Create(L"FeatId", int32Value);
            parmVals->Add(idProp);            
            FdoPtr<FdoParameterValue> propGeomIns = FdoParameterValue::Create(L"Data2", in_geom_value);
            parmVals->Add(propGeomIns);
            CPPUNIT_ASSERT(sqlCmd->ExecuteNonQuery() == 1 );
            tr->Commit();
        }
        // step (3) again how many objects are there
        {
            FdoPtr<FdoIFeatureReader>reader = selectCmd->Execute();
            int rez = 0;
            while(reader->ReadNext())rez++;
            reader->Close();
            printf ("Selectd features step (III): %d\n", rez);
            CPPUNIT_ASSERT(rez == 617); // should increase with one since new geom added is outside of spatial query
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
	printf( "Done\n" );
}

int SelectTest::SelectObjects(FdoIConnection* conn, FdoString* txt, FdoString* clsName)
{
    printf ("\n* Select on '%ls'-[FeatId]: ", txt);
    int cnt = 0;
    FdoPtr<FdoFilter> flt = FdoFilter::Parse(L"Geometry INSIDE GeomFromText('POLYGON XYZ ((-77.4457704165028 39.1002636898979 0, -77.4457704165028 39.0183200297496 0, -77.2331415357993 39.0183200297496 0, -77.2331415357993 39.1002636898979 0, -77.4457704165028 39.1002636898979 0))')");
    FdoPtr<FdoISelect> selCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
    selCmd->SetFeatureClassName((clsName == NULL) ? L"TestClass" : clsName);
    selCmd->SetFilter(flt);
    FdoPtr<FdoIFeatureReader> reader = selCmd->Execute();
    while(reader->ReadNext())
    {
        int id = reader->GetInt32(L"FeatId");
        printf ("%d, ", id);
        cnt++;
    }
    reader->Close();
    printf ("\n");
    return cnt;
}

void SelectTest::InsertValue(FdoIConnection* conn, FdoString* valTxtFlt)
{
    FdoPtr<FdoFilter> filter = FdoFilter::Parse(valTxtFlt);
    FdoSpatialCondition* spFlt = static_cast<FdoSpatialCondition*>(filter.p);
    FdoPtr<FdoGeometryValue> gv = static_cast<FdoGeometryValue*>(spFlt->GetGeometry());
    
    FdoPtr<FdoISQLCommand> sqlCmd = static_cast<FdoISQLCommand*>(conn->CreateCommand(FdoCommandType_SQLCommand));
    sqlCmd->SetSQLStatement(L"INSERT INTO TestClass (Geometry) VALUES(:GVAL);");
    
    FdoPtr<FdoParameterValueCollection> params = sqlCmd->GetParameterValues();
    FdoPtr<FdoParameterValue> param = FdoParameterValue::Create(L":GVAL", gv);
    params->Add(param);
    sqlCmd->ExecuteNonQuery();
}

void SelectTest::CreateSchoolSchema(FdoIConnection* conn)
{
    FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create(L"Default", L"");
    FdoPtr<FdoFeatureClass> clas = FdoFeatureClass::Create(L"TestClass",L"");
    FdoPtr<FdoClassCollection>(schema->GetClasses())->Add(clas);
    FdoPtr<FdoPropertyDefinitionCollection> properties = clas->GetProperties();
    
    FdoPtr<FdoGeometricPropertyDefinition> geometry = FdoGeometricPropertyDefinition::Create(L"Geometry", L"");
    geometry->SetGeometryTypes(7); // Point, Line, Polygon
    properties->Add(geometry);
    clas->SetGeometryProperty(geometry);
    FdoPtr<FdoDataPropertyDefinition> dpd = FdoDataPropertyDefinition::Create(L"FeatId", L"");
    dpd->SetDataType(FdoDataType_Int32);
    dpd->SetIsAutoGenerated(true);
    properties->Add(dpd);
    FdoPtr<FdoDataPropertyDefinitionCollection>(clas->GetIdentityProperties())->Add(dpd);
    
    FdoPtr<FdoIApplySchema> applyschema = static_cast<FdoIApplySchema*>(conn->CreateCommand(FdoCommandType_ApplySchema));
    applyschema->SetFeatureSchema(schema);
    applyschema->Execute();
}

void SelectTest::TestDualConnection ()
{
    FdoPtr<FdoIConnection> conn1;
    FdoPtr<FdoIConnection> conn2;
    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);

        conn1 = UnitTestUtil::OpenConnection( SC_TEST_FILE, true, true );
        CreateSchoolSchema(conn1);
        conn2 = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
	    
        FdoPtr<FdoIDescribeSchema> decrCmd1 = (FdoIDescribeSchema*)conn1->CreateCommand(FdoCommandType_DescribeSchema); 
        FdoPtr<FdoFeatureSchemaCollection> schColl1 = decrCmd1->Execute();

	    FdoPtr<FdoIDescribeSchema> decrCmd2 = (FdoIDescribeSchema*)conn2->CreateCommand(FdoCommandType_DescribeSchema); 
        FdoPtr<FdoFeatureSchemaCollection> schColl2 = decrCmd2->Execute();

        InsertValue(conn1, L"Geometry INTERSECTS GeomFromText('POINT XYZ (-77.2727843079006 39.0938931173285 0)')");
        int cnt1 = SelectObjects(conn1, L"conn1");
        int cnt2 = SelectObjects(conn2, L"conn2");
        CPPUNIT_ASSERT(cnt1 == cnt2);

        InsertValue(conn1, L"Geometry INTERSECTS GeomFromText('POINT XYZ (-77.2727843079006 39.0651450294108 0)')");
        cnt1 = SelectObjects(conn1, L"conn1");
        cnt2 = SelectObjects(conn2, L"conn2");
        CPPUNIT_ASSERT(cnt1 == cnt2);

        InsertValue(conn2, L"Geometry INTERSECTS GeomFromText('POINT XYZ (-77.3544138327724 39.0938931173285 0)')");
        cnt1 = SelectObjects(conn1, L"conn1");
        cnt2 = SelectObjects(conn2, L"conn2");
        CPPUNIT_ASSERT(cnt1 == cnt2);

        InsertValue(conn1, L"Geometry INTERSECTS GeomFromText('POINT XYZ (-77.3464596345164 39.0575513745066 0)')");
        cnt1 = SelectObjects(conn1, L"conn1");
        cnt2 = SelectObjects(conn2, L"conn2");
        CPPUNIT_ASSERT(cnt1 == cnt2);

        InsertValue(conn2, L"Geometry INTERSECTS GeomFromText('POINT XYZ (-77.3544138327724 39.0575513745066 0)')");
        conn2->Close();
        cnt1 = SelectObjects(conn1, L"conn1");
        CPPUNIT_ASSERT(cnt1 == (cnt2+1));
        conn1->Close();
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
	printf( "Done\n" );
}
