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
static const wchar_t* SRC_VIEW_TEST_FILE = L"..\\..\\TestData\\ViewTests.sqlite";
static const wchar_t* SRC_SPATIAL_TEST_FILE = L"..\\..\\TestData\\SpatialTests.sqlite";
static const wchar_t* SRC_SPATIAL2_TEST_FILE = L"..\\..\\TestData\\CrOvTcTest.sqlite";
#else
#include <unistd.h>
static const wchar_t* SC_TEST_FILE = L"../../TestData/SelectTest.sqlite";
static const wchar_t* SRC_TEST_FILE = L"../../TestData/PARCEL_Source.sqlite";
static const wchar_t* SRC_VIEW_TEST_FILE = L"../../TestData/ViewTests.sqlite";
static const wchar_t* SRC_SPATIAL_TEST_FILE = L"../../TestData/SpatialTests.sqlite";
static const wchar_t* SRC_SPATIAL2_TEST_FILE = L"../../TestData/CrOvTcTest.sqlite";
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

int SelectTest::InsertValue(FdoIConnection* conn, FdoString* className, FdoString* valTxtFlt)
{
    FdoPtr<FdoFilter> filter = FdoFilter::Parse(valTxtFlt);
    FdoSpatialCondition* spFlt = static_cast<FdoSpatialCondition*>(filter.p);
    FdoPtr<FdoGeometryValue> gv = static_cast<FdoGeometryValue*>(spFlt->GetGeometry());
    
    FdoPtr<FdoISQLCommand> sqlCmd = static_cast<FdoISQLCommand*>(conn->CreateCommand(FdoCommandType_SQLCommand));
    std::wstring sql(L"INSERT INTO ");
    sql.append(className);
    sql.append(L" (Geometry) VALUES(:GVAL);");
    sqlCmd->SetSQLStatement(sql.c_str());
    
    FdoPtr<FdoParameterValueCollection> params = sqlCmd->GetParameterValues();
    FdoPtr<FdoParameterValue> param = FdoParameterValue::Create(L":GVAL", gv);
    params->Add(param);
    sqlCmd->ExecuteNonQuery();

    FdoPtr<FdoISQLCommand> sqlCmd2 = static_cast<FdoISQLCommand*>(conn->CreateCommand(FdoCommandType_SQLCommand));
    sqlCmd->SetSQLStatement(L"select last_insert_rowid() as rowid from fdo_columns limit 1;");
    FdoPtr<FdoISQLDataReader> rdr = sqlCmd->ExecuteReader();
    if(rdr->ReadNext())
        return rdr->GetInt32(L"rowid");
    return -1;
}

void SelectTest::UpdateValue(FdoIConnection* conn, int rowid, FdoString* className, FdoString* valTxtFlt)
{
    FdoPtr<FdoFilter> filter = FdoFilter::Parse(valTxtFlt);
    FdoSpatialCondition* spFlt = static_cast<FdoSpatialCondition*>(filter.p);
    FdoPtr<FdoGeometryValue> gv = static_cast<FdoGeometryValue*>(spFlt->GetGeometry());
    
    FdoPtr<FdoISQLCommand> sqlCmd = static_cast<FdoISQLCommand*>(conn->CreateCommand(FdoCommandType_SQLCommand));
    FdoStringP sql = FdoStringP::Format(L"UPDATE %ls SET Geometry=:GVAL WHERE rowid=%d;", className, rowid);
    sqlCmd->SetSQLStatement(sql);
    
    FdoPtr<FdoParameterValueCollection> params = sqlCmd->GetParameterValues();
    FdoPtr<FdoParameterValue> param = FdoParameterValue::Create(L":GVAL", gv);
    params->Add(param);
    sqlCmd->ExecuteNonQuery();
}

void SelectTest::CreateFeatureClass(FdoIConnection* conn, FdoString* className, FdoString* scName)
{
    FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create(L"Default", L"");
    FdoPtr<FdoFeatureClass> clas = FdoFeatureClass::Create(className,L"");
    FdoPtr<FdoClassCollection>(schema->GetClasses())->Add(clas);
    FdoPtr<FdoPropertyDefinitionCollection> properties = clas->GetProperties();
    
    FdoPtr<FdoGeometricPropertyDefinition> geometry = FdoGeometricPropertyDefinition::Create(L"Geometry", L"");
    geometry->SetGeometryTypes(7); // Point, Line, Polygon
    geometry->SetSpatialContextAssociation(scName);
    properties->Add(geometry);
    clas->SetGeometryProperty(geometry);
    FdoPtr<FdoDataPropertyDefinition> dpd = FdoDataPropertyDefinition::Create(L"FeatId", L"");
    dpd->SetDataType(FdoDataType_Int32);
    dpd->SetIsAutoGenerated(true);
    properties->Add(dpd);
    FdoPtr<FdoDataPropertyDefinitionCollection>(clas->GetIdentityProperties())->Add(dpd);
    
    FdoPtr<FdoIApplySchema> applyschema = static_cast<FdoIApplySchema*>(conn->CreateCommand(FdoCommandType_ApplySchema));
    applyschema->SetIgnoreStates(true);
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
        CreateFeatureClass(conn1, L"TestClass", L"LL84");
        conn2 = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
	    
        FdoPtr<FdoIDescribeSchema> decrCmd1 = (FdoIDescribeSchema*)conn1->CreateCommand(FdoCommandType_DescribeSchema); 
        FdoPtr<FdoFeatureSchemaCollection> schColl1 = decrCmd1->Execute();

	    FdoPtr<FdoIDescribeSchema> decrCmd2 = (FdoIDescribeSchema*)conn2->CreateCommand(FdoCommandType_DescribeSchema); 
        FdoPtr<FdoFeatureSchemaCollection> schColl2 = decrCmd2->Execute();

        InsertValue(conn1, L"TestClass", L"Geometry INTERSECTS GeomFromText('POINT XYZ (-77.2727843079006 39.0938931173285 0)')");
        int cnt1 = SelectObjects(conn1, L"conn1");
        int cnt2 = SelectObjects(conn2, L"conn2");
        CPPUNIT_ASSERT(cnt1 == cnt2);

        InsertValue(conn1, L"TestClass", L"Geometry INTERSECTS GeomFromText('POINT XYZ (-77.2727843079006 39.0651450294108 0)')");
        cnt1 = SelectObjects(conn1, L"conn1");
        cnt2 = SelectObjects(conn2, L"conn2");
        CPPUNIT_ASSERT(cnt1 == cnt2);

        InsertValue(conn2, L"TestClass", L"Geometry INTERSECTS GeomFromText('POINT XYZ (-77.3544138327724 39.0938931173285 0)')");
        cnt1 = SelectObjects(conn1, L"conn1");
        cnt2 = SelectObjects(conn2, L"conn2");
        CPPUNIT_ASSERT(cnt1 == cnt2);

        int lastRowid = InsertValue(conn1, L"TestClass", L"Geometry INTERSECTS GeomFromText('POINT XYZ (-77.3464596345164 39.0575513745066 0)')");
        cnt1 = SelectObjects(conn1, L"conn1");
        cnt2 = SelectObjects(conn2, L"conn2");
        CPPUNIT_ASSERT(cnt1 == cnt2);

        FdoPtr<FdoISQLCommand> sqlCmd = static_cast<FdoISQLCommand*>(conn2->CreateCommand(FdoCommandType_SQLCommand));
        FdoStringP sql = FdoStringP::Format(L"DELETE FROM \"TestClass\" WHERE rowid=%d;", lastRowid);
        sqlCmd->SetSQLStatement(sql);
        CPPUNIT_ASSERT(sqlCmd->ExecuteNonQuery());
        int cnt3 = SelectObjects(conn1, L"conn1");
        CPPUNIT_ASSERT(cnt3 == (cnt1-1));
        
        lastRowid = InsertValue(conn2, L"TestClass", L"Geometry INTERSECTS GeomFromText('POINT XYZ (-77.3464596345164 39.0575513745066 0)')");
        cnt1 = SelectObjects(conn1, L"conn1");
        cnt2 = SelectObjects(conn2, L"conn2");
        CPPUNIT_ASSERT(cnt1 == cnt2);
        CPPUNIT_ASSERT(cnt1 == (cnt3+1));

        UpdateValue(conn1, lastRowid, L"TestClass", L"Geometry INTERSECTS GeomFromText('POINT XYZ (-77.1464596345164 39.1575513745066 0)')");
        cnt1 = SelectObjects(conn1, L"conn1");
        cnt2 = SelectObjects(conn2, L"conn2");
        CPPUNIT_ASSERT(cnt1 == cnt2);

        InsertValue(conn2, L"TestClass", L"Geometry INTERSECTS GeomFromText('POINT XYZ (-77.3544138327724 39.0575513745066 0)')");
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

void SelectTest::TestSelectMultipleCS ()
{
    FdoPtr<FdoIConnection> conn;
    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, true, true );
        {
	        FdoPtr<FdoICreateSpatialContext> pCreateCreateSpatialContext = (FdoICreateSpatialContext*) conn->CreateCommand(FdoCommandType_CreateSpatialContext);
	        pCreateCreateSpatialContext->SetCoordinateSystemWkt(L"PROJCS[\"CA83IIIF\",GEOGCS[\"LL83\",DATUM[\"NAD83\",SPHEROID[\"GRS1980\",6378137.000,298.25722210]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.017453292519943295]],PROJECTION[\"Lambert_Conformal_Conic_2SP\"],PARAMETER[\"false_easting\",6561666.667],PARAMETER[\"false_northing\",1640416.667],PARAMETER[\"central_meridian\",-120.50000000000000],PARAMETER[\"latitude_of_origin\",36.50000000000000],PARAMETER[\"standard_parallel_1\",38.43333333333333],PARAMETER[\"standard_parallel_2\",37.06666666666666],UNIT[\"Foot_US\",0.30480060960122]]");
	        pCreateCreateSpatialContext->SetDescription(L"CA83IIIF Coordinate System" );
	        pCreateCreateSpatialContext->SetName( L"CA83IIIF" );
	        pCreateCreateSpatialContext->Execute();
        }
        CreateFeatureClass(conn, L"TestClassLL84", L"LL84");
        CreateFeatureClass(conn, L"TestClassCA83IIIF", L"CA83IIIF");

        InsertValue(conn, L"TestClassCA83IIIF", L"Geometry INSIDE GeomFromText('POLYGON ((5990102.49997365 2107167.50006662, 5990296.00007366 2102409.50006628, 5984903.49997329 2102225.00016627, 5984902.00007329 2102264.50006628, 5984740.00007328 2107065.0001666, 5990100.49997365 2107250.25006663, 5990102.49997365 2107167.50006662))')");
        InsertValue(conn, L"TestClassLL84", L"Geometry INSIDE GeomFromText('POLYGON XYZ ((-73.9089788473758 44.7028354893831 0, -73.9089788473758 44.4642768507282 0, -73.5090664079954 44.4642768507282 0, -73.5090664079954 44.7028354893831 0, -73.9089788473758 44.7028354893831 0))')");
        
        {
        FdoPtr<FdoISelect> selCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
        selCmd->SetFeatureClassName(L"TestClassLL84");
        FdoPtr<FdoIdentifierCollection> props =  selCmd->GetPropertyNames();
        props->Add(FdoPtr<FdoComputedIdentifier>(FdoComputedIdentifier::Create(L"area", FdoPtr<FdoExpression>(FdoExpression::Parse(L"Area2D(Geometry)")))));
        FdoPtr<FdoIFeatureReader> reader = selCmd->Execute();
        while(reader->ReadNext())
        {
            CPPUNIT_ASSERT(TestCommonMiscUtil::FuzzyEqual(reader->GetDouble(L"area"),840286918.3210836));
            break;
        }
        reader->Close();
        }

        {
        FdoPtr<FdoISelect> selCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
        selCmd->SetFeatureClassName(L"TestClassCA83IIIF");
        FdoPtr<FdoIdentifierCollection> props =  selCmd->GetPropertyNames();
        props->Add(FdoPtr<FdoComputedIdentifier>(FdoComputedIdentifier::Create(L"area", FdoPtr<FdoExpression>(FdoExpression::Parse(L"Area2D(Geometry)")))));
        FdoPtr<FdoIFeatureReader> reader = selCmd->Execute();
        while(reader->ReadNext())
        {
            CPPUNIT_ASSERT(TestCommonMiscUtil::FuzzyEqual(reader->GetDouble(L"area"), 26054613.81218502));
            break;
        }
        reader->Close();
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

int SelectTest::SelectObjects(FdoIConnection* conn, FdoFilter* filter, FdoString* clsName, FdoString* id)
{
    int cnt = 0;
    FdoPtr<FdoISelect> selCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
    selCmd->SetFeatureClassName(clsName);
    selCmd->SetFilter(filter);
    FdoPtr<FdoIFeatureReader> reader = selCmd->Execute();
    while(reader->ReadNext())
    {
        reader->GetInt32(id);
        cnt++;
    }
    reader->Close();
    return cnt;
}

void SelectTest::TestViewSelects ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_VIEW_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );

        FdoPtr<FdoIDescribeSchema> decrCmd = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema); 
        FdoPtr<FdoFeatureSchemaCollection> schColl = decrCmd->Execute();
        
        FdoPtr<FdoFilter> filter1 = FdoFilter::Parse(L"Geom INSIDE GeomFromText('POLYGON XYZ ((-77.0856930537741 38.9299833022685 0, -77.0856930537741 38.8861160395816 0, -77.0212774526814 38.8861160395816 0, -77.0212774526814 38.9299833022685 0, -77.0856930537741 38.9299833022685 0))')");
        FdoPtr<FdoFilter> filter2 = FdoFilter::Parse(L"Geom INSIDE GeomFromText('POLYGON XYZ ((-77.055464531456 39.0044139935043 0, -77.0972087408102 38.9533553301582 0, -77.0579835749826 38.9303429256581 0, -77.0266755220682 38.9781654635581 0, -77.0219972091465 38.9199153995527 0, -76.9910489303633 38.9619849333397 0, -76.9568618515887 38.9378938526949 0, -76.9978863044778 38.9141623954396 0, -76.9471456598165 38.89079036755 0, -76.9532632774659 38.9932673176318 0, -77.0122810173743 39.0098074712398 0, -77.055464531456 39.0044139935043 0))') ");

        int cnt11 = SelectObjects(conn, filter1, L"ViewNotWellDefined", L"id");
        int cnt12 = SelectObjects(conn, filter2, L"ViewNotWellDefined", L"id");
        CPPUNIT_ASSERT(cnt11 == 5);
        CPPUNIT_ASSERT(cnt12 == 0);
        
        int cnt21 = SelectObjects(conn, filter1, L"ViewWellDefined", L"id");
        int cnt22 = SelectObjects(conn, filter2, L"ViewWellDefined", L"id");

        CPPUNIT_ASSERT(cnt11 == cnt21);
        CPPUNIT_ASSERT(cnt12 == cnt22);
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

void SelectTest::TestJoinViewSelects ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_VIEW_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );

        FdoPtr<FdoIDescribeSchema> decrCmd = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema); 
        FdoPtr<FdoFeatureSchemaCollection> schColl = decrCmd->Execute();
        
        FdoPtr<FdoFilter> filter1 = FdoFilter::Parse(L"Geom INSIDE GeomFromText('POLYGON XYZ ((-77.0856930537741 38.9299833022685 0, -77.0856930537741 38.8861160395816 0, -77.0212774526814 38.8861160395816 0, -77.0212774526814 38.9299833022685 0, -77.0856930537741 38.9299833022685 0))')");
        FdoPtr<FdoFilter> filter2 = FdoFilter::Parse(L"Geom INSIDE GeomFromText('POLYGON XYZ ((-77.055464531456 39.0044139935043 0, -77.0972087408102 38.9533553301582 0, -77.0579835749826 38.9303429256581 0, -77.0266755220682 38.9781654635581 0, -77.0219972091465 38.9199153995527 0, -76.9910489303633 38.9619849333397 0, -76.9568618515887 38.9378938526949 0, -76.9978863044778 38.9141623954396 0, -76.9471456598165 38.89079036755 0, -76.9532632774659 38.9932673176318 0, -77.0122810173743 39.0098074712398 0, -77.055464531456 39.0044139935043 0))') ");

        int cnt11 = SelectObjects(conn, filter1, L"JoinViewNotWellDefined", L"id");
        int cnt12 = SelectObjects(conn, filter2, L"JoinViewNotWellDefined", L"id");
        CPPUNIT_ASSERT(cnt11 == 5);
        CPPUNIT_ASSERT(cnt12 == 0);
        
        int cnt21 = SelectObjects(conn, filter1, L"JoinViewWellDefined", L"id");
        int cnt22 = SelectObjects(conn, filter2, L"JoinViewWellDefined", L"id");

        CPPUNIT_ASSERT(cnt11 == cnt21);
        CPPUNIT_ASSERT(cnt12 == cnt22);
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


void SelectTest::TestSubSelect ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_VIEW_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );

        FdoPtr<FdoIDescribeSchema> decrCmd = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema); 
        FdoPtr<FdoFeatureSchemaCollection> schColl = decrCmd->Execute();

        int cnt = 0;
        FdoPtr<FdoISelect> selCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
        selCmd->SetFeatureClassName(L"MainTable");
        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"FeatId IN (SELECT(MainTable, FeatId as FID, GEOMETRY INSIDE GeomFromText('POLYGON XYZ ((-77.0856930537741 38.9299833022685 0, -77.0856930537741 38.8861160395816 0, -77.0212774526814 38.8861160395816 0, -77.0212774526814 38.9299833022685 0, -77.0856930537741 38.9299833022685 0))')))");
        selCmd->SetFilter(filter);
        FdoPtr<FdoIFeatureReader> reader = selCmd->Execute();
        while(reader->ReadNext())
        {
            reader->GetInt32(L"FeatId");
            cnt++;
        }
        reader->Close();
        printf ("\nCount = %d -> OK\n", cnt);
        CPPUNIT_ASSERT(cnt == 5);
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

void SelectTest::TestJoinType (FdoIConnection* conn, FdoJoinType jtype, int expCount)
{
    int cnt = 0;
    FdoPtr<FdoISelect> selCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
    selCmd->SetFeatureClassName(L"MainTable");

    FdoPtr<FdoIdentifierCollection> idpColl = selCmd->GetPropertyNames();
    FdoPtr<FdoIdentifier> idf = FdoIdentifier::Create(L"a.FeatId");
    idpColl->Add(idf);
    idf = FdoIdentifier::Create(L"a.GEOMETRY");
    idpColl->Add(idf);
    idf = FdoComputedIdentifier::Create(L"MainName", FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"a.Name")));
    idpColl->Add(idf);
    idf = FdoComputedIdentifier::Create(L"SecondName", FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"b.Name")));
    idpColl->Add(idf);

    FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"a.GEOMETRY INSIDE GeomFromText('POLYGON XYZ ((-77.1292540392099 39.02968253293 0, -77.1453959885122 38.9948388680958 0, -77.1156948434364 38.9774170901157 0, -77.0349855133297 38.938056575283 0, -77.0524187352952 38.8896625782011 0, -77.0156153823694 38.8741765775241 0, -76.9975363158697 38.917408538089 0, -77.0169066550326 38.917408538089 0, -77.0401510203875 38.993548386185 0, -76.9613785992755 38.9703192762956 0, -76.9613785992755 38.9057939831443 0, -76.9471737671705 38.8967603920212 0, -76.8935828286107 38.9148274653936 0, -77.0104498753116 39.0793671207965 0, -77.0814740358369 39.0574284928179 0, -77.1292540392099 39.02968253293 0))')");
    selCmd->SetFilter(filter);
    selCmd->SetAlias(L"a");
    FdoPtr<FdoJoinCriteriaCollection> jcColl = selCmd->GetJoinCriteria();
    FdoPtr<FdoIdentifier> jcClass = FdoIdentifier::Create(L"SecondTable");
    FdoPtr<FdoFilter> jcFilter = FdoFilter::Parse(L"a.ID=b.ID");
    FdoPtr<FdoJoinCriteria> jc = FdoJoinCriteria::Create(L"b", jcClass, jtype, jcFilter);
    jcColl->Add(jc);

    FdoPtr<FdoIFeatureReader> reader = selCmd->Execute();
    while(reader->ReadNext())
    {
        reader->GetInt32(L"FeatId");
        cnt++;
    }
    reader->Close();

    printf ("\nCount = %d -> OK\n", cnt);
    CPPUNIT_ASSERT(cnt == expCount);
}

void SelectTest::TestJoin ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_VIEW_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );

        FdoPtr<FdoIDescribeSchema> decrCmd = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema); 
        FdoPtr<FdoFeatureSchemaCollection> schColl = decrCmd->Execute();

        printf ("\n### Inner Join Test ###");
        TestJoinType(conn, FdoJoinType_Inner, 7);
        printf ("\n### LeftOuter Join Test ###");
        TestJoinType(conn, FdoJoinType_LeftOuter, 10);
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

void SelectTest::TestMSelect ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_VIEW_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );

        FdoPtr<FdoIDescribeSchema> decrCmd = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema); 
        FdoPtr<FdoFeatureSchemaCollection> schColl = decrCmd->Execute();

        printf ("\n### Cross Join Test ###");
        int cnt = 0;
        FdoPtr<FdoISelect> selCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
        selCmd->SetFeatureClassName(L"MainTable");

        FdoPtr<FdoIdentifierCollection> idpColl = selCmd->GetPropertyNames();
        FdoPtr<FdoIdentifier> idf = FdoIdentifier::Create(L"a.FeatId");
        idpColl->Add(idf);
        idf = FdoComputedIdentifier::Create(L"Geom", FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"a.GEOMETRY")));
        idpColl->Add(idf);
        idf = FdoComputedIdentifier::Create(L"MainName", FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"a.Name")));
        idpColl->Add(idf);
        idf = FdoComputedIdentifier::Create(L"SecondName", FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"b.Name")));
        idpColl->Add(idf);

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"a.ID=b.ID AND Geom INSIDE GeomFromText('POLYGON XYZ ((-77.1292540392099 39.02968253293 0, -77.1453959885122 38.9948388680958 0, -77.1156948434364 38.9774170901157 0, -77.0349855133297 38.938056575283 0, -77.0524187352952 38.8896625782011 0, -77.0156153823694 38.8741765775241 0, -76.9975363158697 38.917408538089 0, -77.0169066550326 38.917408538089 0, -77.0401510203875 38.993548386185 0, -76.9613785992755 38.9703192762956 0, -76.9613785992755 38.9057939831443 0, -76.9471737671705 38.8967603920212 0, -76.8935828286107 38.9148274653936 0, -77.0104498753116 39.0793671207965 0, -77.0814740358369 39.0574284928179 0, -77.1292540392099 39.02968253293 0))')");
        selCmd->SetFilter(filter);
        selCmd->SetAlias(L"a");
        FdoPtr<FdoJoinCriteriaCollection> jcColl = selCmd->GetJoinCriteria();
        FdoPtr<FdoIdentifier> jcClass = FdoIdentifier::Create(L"SecondTable");
        FdoPtr<FdoJoinCriteria> jc = FdoJoinCriteria::Create(L"b", jcClass, FdoJoinType_Cross);
        jcColl->Add(jc);

        FdoPtr<FdoIFeatureReader> reader = selCmd->Execute();
        while(reader->ReadNext())
        {
            reader->GetInt32(L"FeatId");
            cnt++;
        }
        reader->Close();

        printf ("\nCount = %d -> OK\n", cnt);
        CPPUNIT_ASSERT(cnt == 7);
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

void SelectTest::TestJoinTypeAggregates (FdoIConnection* conn, FdoJoinType jtype, int expMax)
{
    int cnt = 0;
    FdoPtr<FdoISelectAggregates> selCmd = static_cast<FdoISelectAggregates*>(conn->CreateCommand(FdoCommandType_SelectAggregates));
    selCmd->SetFeatureClassName(L"MainTable");

    FdoPtr<FdoIdentifierCollection> idpColl = selCmd->GetPropertyNames();
    FdoPtr<FdoExpression> exp = FdoExpression::Parse(L"Max(a.FeatId)");
    FdoPtr<FdoIdentifier> idf = FdoComputedIdentifier::Create(L"MAXID", exp);
    idpColl->Add(idf);

    FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"a.GEOMETRY INSIDE GeomFromText('POLYGON XYZ ((-77.1292540392099 39.02968253293 0, -77.1453959885122 38.9948388680958 0, -77.1156948434364 38.9774170901157 0, -77.0349855133297 38.938056575283 0, -77.0524187352952 38.8896625782011 0, -77.0156153823694 38.8741765775241 0, -76.9975363158697 38.917408538089 0, -77.0169066550326 38.917408538089 0, -77.0401510203875 38.993548386185 0, -76.9613785992755 38.9703192762956 0, -76.9613785992755 38.9057939831443 0, -76.9471737671705 38.8967603920212 0, -76.8935828286107 38.9148274653936 0, -77.0104498753116 39.0793671207965 0, -77.0814740358369 39.0574284928179 0, -77.1292540392099 39.02968253293 0))')");
    selCmd->SetFilter(filter);
    selCmd->SetAlias(L"a");
    FdoPtr<FdoJoinCriteriaCollection> jcColl = selCmd->GetJoinCriteria();
    FdoPtr<FdoIdentifier> jcClass = FdoIdentifier::Create(L"SecondTable");
    FdoPtr<FdoFilter> jcFilter = FdoFilter::Parse(L"a.ID=b.ID");
    FdoPtr<FdoJoinCriteria> jc = FdoJoinCriteria::Create(L"b", jcClass, jtype, jcFilter);
    jcColl->Add(jc);

    FdoPtr<FdoIDataReader> reader = selCmd->Execute();
    while(reader->ReadNext())
    {
        int mid = reader->GetInt32(L"MAXID");
        printf ("\nMAXID = %d\n", mid);
        CPPUNIT_ASSERT(mid == expMax);
        cnt++;
    }
    reader->Close();

    printf ("\nCount = %d -> OK\n", cnt);
    CPPUNIT_ASSERT(cnt == 1);
}

void SelectTest::TestJoinAggregatesSelect ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_VIEW_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );

        FdoPtr<FdoIDescribeSchema> decrCmd = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema); 
        FdoPtr<FdoFeatureSchemaCollection> schColl = decrCmd->Execute();

        printf ("\n### Inner Join Aggregates Test ###");
        TestJoinTypeAggregates(conn, FdoJoinType_Inner, 13);
        printf ("\n### LeftOuter Join Aggregates Test ###");
        TestJoinTypeAggregates(conn, FdoJoinType_LeftOuter, 695);
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

void SelectTest::TestMAggregatesSelect ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_VIEW_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );

        FdoPtr<FdoIDescribeSchema> decrCmd = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema); 
        FdoPtr<FdoFeatureSchemaCollection> schColl = decrCmd->Execute();

        int cnt = 0;
        FdoPtr<FdoISelectAggregates> selCmd = static_cast<FdoISelectAggregates*>(conn->CreateCommand(FdoCommandType_SelectAggregates));
        selCmd->SetFeatureClassName(L"MainTable");

        FdoPtr<FdoIdentifierCollection> idpColl = selCmd->GetPropertyNames();
        FdoPtr<FdoExpression> exp = FdoExpression::Parse(L"Max(a.FeatId)");
        FdoPtr<FdoIdentifier> idf = FdoComputedIdentifier::Create(L"MAXID", exp);
        idpColl->Add(idf);

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"a.ID=b.ID AND a.GEOMETRY INSIDE GeomFromText('POLYGON XYZ ((-77.1292540392099 39.02968253293 0, -77.1453959885122 38.9948388680958 0, -77.1156948434364 38.9774170901157 0, -77.0349855133297 38.938056575283 0, -77.0524187352952 38.8896625782011 0, -77.0156153823694 38.8741765775241 0, -76.9975363158697 38.917408538089 0, -77.0169066550326 38.917408538089 0, -77.0401510203875 38.993548386185 0, -76.9613785992755 38.9703192762956 0, -76.9613785992755 38.9057939831443 0, -76.9471737671705 38.8967603920212 0, -76.8935828286107 38.9148274653936 0, -77.0104498753116 39.0793671207965 0, -77.0814740358369 39.0574284928179 0, -77.1292540392099 39.02968253293 0))')");
        selCmd->SetFilter(filter);
        selCmd->SetAlias(L"a");
        FdoPtr<FdoJoinCriteriaCollection> jcColl = selCmd->GetJoinCriteria();
        FdoPtr<FdoIdentifier> jcClass = FdoIdentifier::Create(L"SecondTable");
        FdoPtr<FdoJoinCriteria> jc = FdoJoinCriteria::Create(L"b", jcClass, FdoJoinType_Cross);
        jcColl->Add(jc);

        FdoPtr<FdoIDataReader> reader = selCmd->Execute();
        while(reader->ReadNext())
        {
            int mid = reader->GetInt32(L"MAXID");
            printf ("\nMAXID = %d\n", mid);
            CPPUNIT_ASSERT(mid == 13);
            cnt++;
        }
        reader->Close();

        printf ("\nCount = %d -> OK\n", cnt);
        CPPUNIT_ASSERT(cnt == 1);
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

int SelectTest::SelectSpatial(FdoIConnection* conn, FdoString* sql)
{
    int cnt = 0;
    FdoPtr<FdoISQLCommand> sqlCmd = static_cast<FdoISQLCommand*>(conn->CreateCommand(FdoCommandType_SQLCommand));
    sqlCmd->SetSQLStatement(sql);
    FdoPtr<FdoISQLDataReader> rdr = sqlCmd->ExecuteReader();
    while(rdr->ReadNext()) cnt++;

    return cnt;
}

void SelectTest::TestSpatialJoins ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_SPATIAL_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );

        FdoPtr<FdoIDescribeSchema> decrCmd = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema); 
        FdoPtr<FdoFeatureSchemaCollection> schColl = decrCmd->Execute();

        printf ("\n### Spatial Join Test ###");
        std::wstring sql;
        sql.append(L"SELECT a.FeatId as mFid, b.FeatId as sFid, b.PID FROM SlaveTB b, MainTB a WHERE geom_inside(b.Geometry, a.Geometry);");
        int cnt1 = SelectSpatial(conn, sql.c_str());
        sql.assign (L"SELECT a.FeatId as mFid, b.FeatId as sFid, b.PID FROM MainTB a, SlaveTB b WHERE geom_inside(b.Geometry, a.Geometry);");
        int cnt2 = SelectSpatial(conn, sql.c_str());
        sql.assign (L"SELECT a.FeatId as mFid, b.FeatId as sFid, b.PID FROM SlaveTB b INNER JOIN MainTB a ON(geom_inside(b.Geometry, a.Geometry));");
        int cnt3 = SelectSpatial(conn, sql.c_str());
        CPPUNIT_ASSERT(cnt1 == cnt2);
        CPPUNIT_ASSERT(cnt2 == cnt3);
        printf ("\nCnt = %d\n", cnt1);
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

void SelectTest::TestSpatialJoinsWFilter1 ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_SPATIAL_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );

        FdoPtr<FdoIDescribeSchema> decrCmd = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema); 
        FdoPtr<FdoFeatureSchemaCollection> schColl = decrCmd->Execute();
        
        printf ("\n### Spatial Join with extra filter Test(I) ###");
        std::wstring sql;
        sql.append(L"SELECT a.FeatId as mFid, b.FeatId as sFid, b.PID FROM SlaveTB b, MainTB a WHERE geom_inside(b.Geometry, a.Geometry) AND sFid > 1;");
        int cnt1 = SelectSpatial(conn, sql.c_str());
        sql.assign (L"SELECT a.FeatId as mFid, b.FeatId as sFid, b.PID FROM MainTB a, SlaveTB b WHERE  sFid > 1 AND geom_inside(b.Geometry, a.Geometry);");
        int cnt2 = SelectSpatial(conn, sql.c_str());
        sql.assign (L"SELECT a.FeatId as mFid, b.FeatId as sFid, b.PID FROM SlaveTB b INNER JOIN MainTB a ON(geom_inside(b.Geometry, a.Geometry)) AND sFid > 1;");
        int cnt3 = SelectSpatial(conn, sql.c_str());
        CPPUNIT_ASSERT(cnt1 == cnt2);
        CPPUNIT_ASSERT(cnt2 == cnt3);
        printf ("\nCnt = %d\n", cnt1);
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

void SelectTest::TestSpatialJoinsWFilter2 ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_SPATIAL_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );

        FdoPtr<FdoIDescribeSchema> decrCmd = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema); 
        FdoPtr<FdoFeatureSchemaCollection> schColl = decrCmd->Execute();
        
        printf ("\n### Spatial Join with extra filter Test(II) ###");
        std::wstring sql;
        sql.append(L"SELECT a.FeatId as mFid, b.FeatId as sFid, b.PID FROM SlaveTB b, MainTB a WHERE geom_inside(b.Geometry, a.Geometry) AND mFid >= 2;");
        int cnt1 = SelectSpatial(conn, sql.c_str());
        sql.assign (L"SELECT a.FeatId as mFid, b.FeatId as sFid, b.PID FROM MainTB a, SlaveTB b WHERE mFid >= 2 AND geom_inside(b.Geometry, a.Geometry);");
        int cnt2 = SelectSpatial(conn, sql.c_str());
        sql.assign (L"SELECT a.FeatId as mFid, b.FeatId as sFid, b.PID FROM SlaveTB b INNER JOIN MainTB a ON(geom_inside(b.Geometry, a.Geometry)) AND mFid >= 2;");
        int cnt3 = SelectSpatial(conn, sql.c_str());
        CPPUNIT_ASSERT(cnt1 == cnt2);
        CPPUNIT_ASSERT(cnt2 == cnt3);
        printf ("\nCnt = %d\n", cnt1);
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

void SelectTest::TestSpatialJoinsWFilter3 ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_SPATIAL_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );

        FdoPtr<FdoIDescribeSchema> decrCmd = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema); 
        FdoPtr<FdoFeatureSchemaCollection> schColl = decrCmd->Execute();
        
        printf ("\n### Spatial Join with extra filter Test(III) ###");
        std::wstring sql;
        sql.append(L"SELECT a.FeatId as mFid, b.FeatId as sFid, b.PID FROM SlaveTB b, MainTB a WHERE geom_inside(b.Geometry, a.Geometry) AND sFid IN(1,2,8);");
        int cnt1 = SelectSpatial(conn, sql.c_str());
        sql.assign (L"SELECT a.FeatId as mFid, b.FeatId as sFid, b.PID FROM MainTB a, SlaveTB b WHERE sFid IN(1,2,8) AND geom_inside(b.Geometry, a.Geometry);");
        int cnt2 = SelectSpatial(conn, sql.c_str());
        sql.assign (L"SELECT a.FeatId as mFid, b.FeatId as sFid, b.PID FROM SlaveTB b INNER JOIN MainTB a ON(geom_inside(b.Geometry, a.Geometry)) AND sFid IN(1,2,8);");
        int cnt3 = SelectSpatial(conn, sql.c_str());
        CPPUNIT_ASSERT(cnt1 == cnt2);
        CPPUNIT_ASSERT(cnt2 == cnt3);
        printf ("\nCnt = %d\n", cnt1);
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

void SelectTest::TestSpatialJoinsComplexFilter ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_SPATIAL_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );

        FdoPtr<FdoIDescribeSchema> decrCmd = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema); 
        FdoPtr<FdoFeatureSchemaCollection> schColl = decrCmd->Execute();
        
        printf ("\n### Spatial Join with complex filter Test ###");
        std::wstring sql;
        sql.append(L"SELECT a.FeatId as mFid, b.FeatId as sFid, b.PID FROM SlaveTB b, MainTB a WHERE sFid > 1 AND geom_inside(b.Geometry, a.Geometry) AND mFid >= 2;");
        int cnt1 = SelectSpatial(conn, sql.c_str());
        sql.assign (L"SELECT a.FeatId as mFid, b.FeatId as sFid, b.PID FROM MainTB a, SlaveTB b WHERE mFid >= 2 AND geom_inside(b.Geometry, a.Geometry) AND sFid > 1;");
        int cnt2 = SelectSpatial(conn, sql.c_str());
        sql.assign (L"SELECT a.FeatId as mFid, b.FeatId as sFid, b.PID FROM SlaveTB b INNER JOIN MainTB a ON(geom_inside(b.Geometry, a.Geometry)) AND mFid >= 2 AND sFid > 1;");
        int cnt3 = SelectSpatial(conn, sql.c_str());
        CPPUNIT_ASSERT(cnt1 == cnt2);
        CPPUNIT_ASSERT(cnt2 == cnt3);
        printf ("\nCnt = %d\n", cnt1);
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

void SelectTest::TestSpatialJoinType(FdoIConnection* conn, FdoJoinType jtype, int expCount)
{
    int cnt = 0;
    FdoPtr<FdoISelect> selCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
    selCmd->SetFeatureClassName(L"SlaveTB");

    FdoPtr<FdoIdentifierCollection> idpColl = selCmd->GetPropertyNames();
    FdoPtr<FdoIdentifier> idf = FdoIdentifier::Create(L"a.FeatId");
    idpColl->Add(idf);
    idf = FdoIdentifier::Create(L"a.Geometry");
    idpColl->Add(idf);
    idf = FdoComputedIdentifier::Create(L"sFid", FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"a.FeatId")));
    idpColl->Add(idf);
    idf = FdoComputedIdentifier::Create(L"mFid", FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"b.FeatId")));
    idpColl->Add(idf);

    FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"sFid > 1");
    selCmd->SetFilter(filter);
    selCmd->SetAlias(L"a");
    FdoPtr<FdoJoinCriteriaCollection> jcColl = selCmd->GetJoinCriteria();
    FdoPtr<FdoIdentifier> jcClass = FdoIdentifier::Create(L"MainTB");
    FdoPtr<FdoFilter> jcFilter = FdoFilter::Parse(L"a.Geometry INSIDE b.Geometry");
    FdoPtr<FdoJoinCriteria> jc = FdoJoinCriteria::Create(L"b", jcClass, jtype, jcFilter);
    jcColl->Add(jc);

    FdoPtr<FdoIFeatureReader> reader = selCmd->Execute();
    while(reader->ReadNext())
    {
        reader->GetInt32(L"FeatId");
        cnt++;
    }
    reader->Close();

    printf ("\nCount = %d -> OK\n", cnt);
    CPPUNIT_ASSERT(cnt == expCount);
}

void SelectTest::TestSpatialJoinsFdo ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_SPATIAL_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );

        FdoPtr<FdoIDescribeSchema> decrCmd = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema); 
        FdoPtr<FdoFeatureSchemaCollection> schColl = decrCmd->Execute();

        printf ("\n### Inner Spatial Join Test ###");
        TestSpatialJoinType(conn, FdoJoinType_Inner, 4);
        printf ("\n### LeftOuter Spatial Join Test ###");
        TestSpatialJoinType(conn, FdoJoinType_LeftOuter, 11);
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

void SelectTest::TestReleaseSchema ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_SPATIAL_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );

        FdoPtr<FdoIDescribeSchema> decrCmd = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema); 
        FdoPtr<FdoFeatureSchemaCollection> schColl = decrCmd->Execute();
        
        std::wstring sql;
        sql.append(L"DROP TABLE SlaveTB;");
        
        FdoPtr<FdoISQLCommand> sqlCmd = static_cast<FdoISQLCommand*>(conn->CreateCommand(FdoCommandType_SQLCommand));
        sqlCmd->SetSQLStatement(sql.c_str());
        sqlCmd->ExecuteNonQuery();

        decrCmd = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema); 
        schColl = decrCmd->Execute();
        CPPUNIT_ASSERT(1 == schColl->GetCount());
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

int SelectTest::SelectSpatial(FdoIConnection* conn, FdoFilter* filter)
{
    FdoPtr<FdoISelect> selCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
    selCmd->SetFeatureClassName(L"TestClassSI");
    selCmd->SetFilter(filter);
    
    FdoPtr<FdoISQLCommand> sqlCmd = static_cast<FdoISQLCommand*>(conn->CreateCommand(FdoCommandType_SQLCommand));
    FdoPtr<FdoIFeatureReader>reader = selCmd->Execute();

    int cnt = 0;
    while(reader->ReadNext())
    {
        FdoInt64 id = reader->GetInt64(L"FeatId");
        cnt++;
    }
    return cnt;
}

void SelectTest::TestCrosses ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_SPATIAL2_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
        printf("Crosses Test\n");
        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"Geometry CROSSES GeomFromText('POLYGON XYZ ((5.64107995094196 5.72418567722121 0, 4.49969068092314 4.53237498298507 0, 7.56610964914775 1.67202932380327 0, 9.32078273800774 3.39164189541864 0, 6.08400716043732 5.62203046388181 0, 5.64107995094196 5.72418567722121 0))') ");
        CPPUNIT_ASSERT(1 == SelectSpatial(conn, filter));
        
        filter = FdoFilter::Parse(L"Geometry CROSSES GeomFromText('POLYGON XYZ ((9.70441429799531 6.69114612596216 0, 10.4971321778807 3.37354836996973 0, 13.123010210118 7.73099019143822 0, 9.35760024916692 8.47373595249256 0, 9.70441429799531 6.69114612596216 0))') ");
        CPPUNIT_ASSERT(1 == SelectSpatial(conn, filter));
        
        filter = FdoFilter::Parse(L"Geometry CROSSES GeomFromText('POLYGON XYZ ((-0.055924779192004 7.33485910723438 0, -1.29454649013447 5.304687348711 0, 2.42131864269291 5.304687348711 0, 3.51130574328302 7.33485910723438 0, -0.055924779192004 7.33485910723438 0))') ");
        CPPUNIT_ASSERT(1 == SelectSpatial(conn, filter));
        
        filter = FdoFilter::Parse(L"Geometry CROSSES GeomFromText('POLYGON XYZ ((-2.08726437001989 7.5329246667986 0, 2.81767758263562 7.5329246667986 0, 2.81767758263562 3.02693369311924 0, -1.49272592861051 3.02693369311924 0, -3.96996935049543 6.83969527817302 0, -2.08726437001989 7.5329246667986 0))') ");
        CPPUNIT_ASSERT(1 == SelectSpatial(conn, filter));
        
        filter = FdoFilter::Parse(L"Geometry CROSSES GeomFromText('POLYGON XYZ ((15.5011639127649 9.95922747460123 0, 14.7579908610032 1.93757322028982 0, 16.7397856237073 -0.637278739723669 0, 23.378797946486 1.83854047543231 0, 22.387900565134 10.256325779023 0, 19.712477704773 10.256325779023 0, 15.5011639127649 9.95922747460123 0))') ");
        CPPUNIT_ASSERT(0 == SelectSpatial(conn, filter));
        
        filter = FdoFilter::Parse(L"Geometry CROSSES GeomFromText('POLYGON XYZ ((7.50743893828114 17.7352088200722 0, 12.866208702344 17.7352088200722 0, 8.87036307314368 8.2621405371578 0, 6.42329479788964 12.1937734558655 0, 6.42329479788964 16.2801950644052 0, 7.50743893828114 17.7352088200722 0))') ");
        CPPUNIT_ASSERT(0 == SelectSpatial(conn, filter));
        
        filter = FdoFilter::Parse(L"Geometry CROSSES GeomFromText('POLYGON XYZ ((21.7871664615633 9.09799951254206 0, 16 8 0, 13.7644995867972 10.3363090940624 0, 17.2957120392973 13.6487872115325 0, 22.437652967911 12.967716946935 0, 21.7871664615633 9.09799951254206 0))') ");
        CPPUNIT_ASSERT(0 == SelectSpatial(conn, filter));
        
        filter = FdoFilter::Parse(L"Geometry CROSSES GeomFromText('POLYGON XYZ ((16.7691277404297 6.09509877473591 0, 18.9993671802346 2.25633908250027 0, 21.880093126387 3.71135283816731 0, 21.880093126387 7.64298575687501 0, 18.9374160580673 7.64298575687501 0, 16.7691277404297 6.09509877473591 0))') ");
        CPPUNIT_ASSERT(1 == SelectSpatial(conn, filter));
        
        filter = FdoFilter::Parse(L"Geometry CROSSES GeomFromText('POLYGON XYZ ((18.5717275612748 9.81912636856469 0, 17.638265907894 7.40907014083032 0, 13.4765827615909 6.47614516432039 0, 13.4765827615909 4.92127018950067 0, 21.1776413098462 4.92127018950067 0, 22.8889876620929 8.45861075512006 0, 18.5717275612748 9.81912636856469 0))') ");
        CPPUNIT_ASSERT(1 == SelectSpatial(conn, filter));
        
        filter = FdoFilter::Parse(L"Geometry CROSSES GeomFromText('POLYGON XYZ ((21.7871664615633 9.09799951254206 0, 16 7.5 0, 13.7644995867972 10.3363090940624 0, 17.2957120392973 13.6487872115325 0, 22.437652967911 12.967716946935 0, 21.7871664615633 9.09799951254206 0))') ");
        CPPUNIT_ASSERT(1 == SelectSpatial(conn, filter));
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

void SelectTest::TestOverlaps ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_SPATIAL2_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
        printf("Overlaps Test\n");
        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"Geometry OVERLAPS GeomFromText('POLYGON XYZ ((16.7181543649235 9.18095589981505 0, 24.1487287709618 11.0065031058681 0, 30.2960956331682 11.832695546637 0, 31.3026820425922 13.2336305621859 0, 36.0809848883546 13.2336305621859 0, 36.7253897236272 8.56769885711159 0, 29.6062507775767 4.12158541276267 0, 16.7181543649235 9.18095589981505 0))') ");
        CPPUNIT_ASSERT(1 == SelectSpatial(conn, filter));
        
        filter = FdoFilter::Parse(L"Geometry OVERLAPS GeomFromText('POLYGON XYZ ((17.1273936605245 10.3291603574335 0, 24.1487287709618 11.0065031058681 0, 31.3766288155446 13.1615292327876 0, 35.760421745192 8.887139892799 0, 30.2978675767441 5.43956131692004 0, 21.9963303877134 6.24233130458029 0, 17.1273936605245 10.3291603574335 0))') ");
        CPPUNIT_ASSERT(1 == SelectSpatial(conn, filter));
        
        filter = FdoFilter::Parse(L"Geometry OVERLAPS GeomFromText('POLYGON XYZ ((17.1273936605245 10.3291603574335 0, 24.1487287709618 11.0065031058681 0, 31.3766288155446 13.1615292327876 0, 35.760421745192 8.887139892799 0, 30.2978675767441 5.43956131692004 0, 21.9963303877134 6.24233130458029 0, 17.1273936605245 10.3291603574335 0))') ");
        CPPUNIT_ASSERT(1 == SelectSpatial(conn, filter));
        
        filter = FdoFilter::Parse(L"Geometry OVERLAPS GeomFromText('POLYGON XYZ ((1.10092232192566 16.2550387727144 0, 0.233606980128833 11.1160540146437 0, 5.09676798798053 10.7755188928224 0, 6.2738387931957 15.2024756231834 0, 8.07042056210213 7.27729432240809 0, 13.8628479970634 8.11315329779234 0, 14.4204078754419 11.7042510737239 0, 11.9423640575315 13.2211803137057 0, 7.38895851309812 15.3882220761276 0, 17.7967425778055 14.6761940693727 0, 15.7833319734499 18.793573399115 0, 8.56602935516784 18.2363341031469 0, 0.357509187608901 18.2363341031469 0, 1.10092232192566 16.2550387727144 0))') ");
        CPPUNIT_ASSERT(2 == SelectSpatial(conn, filter));
        
        filter = FdoFilter::Parse(L"Geometry OVERLAPS GeomFromText('POLYGON XYZ ((-0.254621764529062 14.076636807366 0, -0.254621764529064 7.27405882396183 0, 5.26835964164376 8.44021503412186 0, 6.51297516719999 12.3662743434462 0, 11.7636968845443 6.84646820798188 0, 16.6254762893103 9.99509000894158 0, 11.9581680592608 12.9882243208012 0, 8.49658113456603 15.0484336561509 0, 5.57951351381918 18.7801335831454 0, 1.84566693715049 18.7801335831454 0, -1.49923725323075 17.0308992469506 0, -0.254621764529062 14.076636807366 0))')");
        CPPUNIT_ASSERT(0 == SelectSpatial(conn, filter));
        
        filter = FdoFilter::Parse(L"Geometry OVERLAPS GeomFromText('POLYGON XYZ ((14 14 0, 9.80855087933362 19.1300937779063 0, 4 15 0, 2 15 0, 2 13 0, 8.00420231045137 8.61077817237079 0, 12.3432310627054 12.8614404076817 0, 14 14 0))') ");
        CPPUNIT_ASSERT(2 == SelectSpatial(conn, filter));
        
        filter = FdoFilter::Parse(L"Geometry OVERLAPS GeomFromText('MULTILINESTRING ((14 16, 12.5749377755851 14.9025166869653), (14.7172699545801 12.9807339797083, 13.2078995698835 12.177963992048))') ");
        CPPUNIT_ASSERT(1 == SelectSpatial(conn, filter));
        
        filter = FdoFilter::Parse(L"Geometry OVERLAPS GeomFromText('POLYGON XYZ ((2 15 0, 2 13 0, -1.94674288802481 7.74193391855054 0, 18.1473053962809 -1.0247283724833 0, 26.1102718727124 19.306467178796 0, 8.19359722200315 26.2700571008214 0, -16.0063556015169 18.4981933401415 0, 2 15 0))') ");
        CPPUNIT_ASSERT(1 == SelectSpatial(conn, filter));
        
        filter = FdoFilter::Parse(L"Geometry OVERLAPS GeomFromText('LINESTRING XYZ (16 8 0, 16 2.80461446806285 0, 20.3465571196173 2.80461446806285 0)') ");
        CPPUNIT_ASSERT(1 == SelectSpatial(conn, filter));

        filter = FdoFilter::Parse(L"Geometry OVERLAPS GeomFromText('POLYGON XYZ ((4 16.5487622016098 0, 2.66838249647226 13.7935234010625 0, 12.6734844329376 14.8151287874814 0, 11.3725114202424 17.4155788981967 0, 7.81032342508592 18.0037759572769 0, 4 16.5487622016098 0))') ");
        CPPUNIT_ASSERT(2 == SelectSpatial(conn, filter));

        filter = FdoFilter::Parse(L"Geometry OVERLAPS GeomFromText('POLYGON XYZ ((1.10092232192566 16.2550387727144 0, 0.233606980128833 11.1160540146437 0, 5.09676798798053 10.7755188928224 0, 6.2738387931957 15.2024756231834 0, 8.07042056210213 7.27729432240809 0, 13.8628479970634 8.11315329779234 0, 14.4204078754419 11.7042510737239 0, 11.9423640575315 13.2211803137057 0, 7.38895851309812 15.3882220761276 0, 17.7967425778055 14.6761940693727 0, 15.7833319734499 18.793573399115 0, 8.56602935516784 18.2363341031469 0, 0.357509187608901 18.2363341031469 0, 1.10092232192566 16.2550387727144 0))')");
        CPPUNIT_ASSERT(2 == SelectSpatial(conn, filter));
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

void SelectTest::TestTouches ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_SPATIAL2_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
        printf("Touches Test\n");
        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"Geometry TOUCHES GeomFromText('POLYGON XYZ ((16.6260965417702 9.18095589981505 0, 24.1487287709618 11.0065031058681 0, 30.2960956331682 11.832695546637 0, 35.760421745192 8.887139892799 0, 31.3026820425922 13.2336305621859 0, 36.0502989473035 13.2336305621859 0, 36.6947037825761 8.35305891521561 0, 29.69830860073 4.15224825861142 0, 20.9835005736234 7.61715046816241 0, 16.6260965417702 9.18095589981505 0))') ");
        CPPUNIT_ASSERT(1 == SelectSpatial(conn, filter));
        
        filter = FdoFilter::Parse(L"Geometry TOUCHES GeomFromText('POLYGON XYZ ((17.1273936605245 10.3291603574335 0, 24.1487287709618 11.0065031058681 0, 31.3766288155446 13.1615292327876 0, 35.760421745192 8.887139892799 0, 30.2978675767441 5.43956131692004 0, 21.9963303877134 6.24233130458029 0, 17.1273936605245 10.3291603574335 0))') ");
        CPPUNIT_ASSERT(0 == SelectSpatial(conn, filter));
        
        filter = FdoFilter::Parse(L"Geometry TOUCHES GeomFromText('POLYGON XYZ ((14 16 0, 14 14 0, 14.8633380468798 12.2022903495955 0, 17.6629766778234 13.248323975595 0, 17.2978064470741 16.1674875937566 0, 15.6910573219778 16.4350775896433 0, 14 16 0))') ");
        CPPUNIT_ASSERT(1 == SelectSpatial(conn, filter));
        
        filter = FdoFilter::Parse(L"Geometry TOUCHES GeomFromText('POLYGON XYZ ((4 15 0, 4 13 0, 6.75655837524837 12.4212276094325 0, 7.09738394834758 14.5132948614315 0, 4 15 0))') ");
        CPPUNIT_ASSERT(1 == SelectSpatial(conn, filter));

        filter = FdoFilter::Parse(L"Geometry TOUCHES GeomFromText('LINESTRING XYZ (16 8 0, 16.9646885381517 12.0652662234523 0, 21.1747387629147 9.74529454204749 0, 19.1625824157176 6.52826714914131 0)') ");
        CPPUNIT_ASSERT(1 == SelectSpatial(conn, filter));
        
        filter = FdoFilter::Parse(L"Geometry TOUCHES GeomFromText('POLYGON XYZ ((14 16 0, 17.8624198495349 16 0, 14.5810572034165 10.7351491325397 0, 12 10 0, 12.9713321110189 7.08506033926743 0, 21.1747387629147 14.4780367628933 0, 16.6551260485366 19.5201085314808 0, 10.8972632658988 19.1489130783817 0, 14 16 0))') ");
        CPPUNIT_ASSERT(1 == SelectSpatial(conn, filter));
        
        filter = FdoFilter::Parse(L"Geometry TOUCHES GeomFromText('LINESTRING XYZ (16 8 0, 18.4906959529213 9.79398036566004 0, 20.2435131703174 7.65326039856605 0, 16.8839467912252 5.34225586577529 0, 14.1816869006814 5.60984586166204 0)') ");
        CPPUNIT_ASSERT(0 == SelectSpatial(conn, filter));
        
        filter = FdoFilter::Parse(L"Geometry TOUCHES GeomFromText('POLYGON XYZ ((14 16 0, 14 11.9972809420408 0, 7.88242268225574 11.9972809420408 0, 7.88242268225574 16 0, 14 16 0))') ");
        CPPUNIT_ASSERT(0 == SelectSpatial(conn, filter));

        filter = FdoFilter::Parse(L"Geometry TOUCHES GeomFromText('POLYGON XYZ ((24.1487287709618 11.0065031058681 0, 30.2960956331682 11.832695546637 0, 35.760421745192 8.887139892799 0, 25 3 0, 24 3 0, 22.7957913506804 3 0, 23 4 0, 21.1387503875221 8.28707592379031 0, 24.1487287709618 11.0065031058681 0))') ");
        CPPUNIT_ASSERT(5 == SelectSpatial(conn, filter));

        filter = FdoFilter::Parse(L"Geometry TOUCHES GeomFromText('LINESTRING XYZ (10 5 0, 12 5 0, 12 3 0, 13.8048098175142 1.93986568058828 0)') ");
        CPPUNIT_ASSERT(1 == SelectSpatial(conn, filter));

        filter = FdoFilter::Parse(L"Geometry TOUCHES GeomFromText('POLYGON XYZ ((22 2 0, 24 2 0, 23 3 0, 24 4 0, 22 4 0, 22 2 0, 22 2 0))') ");
        CPPUNIT_ASSERT(1 == SelectSpatial(conn, filter));
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
