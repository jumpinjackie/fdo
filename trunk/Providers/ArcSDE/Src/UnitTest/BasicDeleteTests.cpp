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
#include "BasicDeleteTests.h"

CPPUNIT_TEST_SUITE_REGISTRATION (BasicDeleteTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (BasicDeleteTests, "BasicDeleteTests");

//NO MULTIPLE SCHEMA SUPPORT:static const wchar_t*    schema_name = L"DeleteSchema";
//NO MULTIPLE SCHEMA SUPPORT:static const wchar_t*    schema_description = L"Basic delete test schema.";

static const wchar_t*    geom_prop_name = L"SHAPE";
static const wchar_t*    id_prop_name = L"OBJECTID";

FdoPtr<FdoIConnection> BasicDeleteTests::mConnection;

BasicDeleteTests::BasicDeleteTests(void)
{
}

BasicDeleteTests::~BasicDeleteTests(void)
{
}

void BasicDeleteTests::setUp ()
{
    if (!CreateSchemaOnly() /* || !bSchemaCreated */ )
    {
        mConnection = ArcSDETests::GetConnection ();
        mConnection->SetConnectionString (ArcSDETestConfig::ConnStringMetadcov());
        mConnection->Open ();
    }
}

void BasicDeleteTests::tearDown ()
{
    if (mConnection)
        mConnection->Close ();
}


/* Test delete operation. */
void BasicDeleteTests::simple_delete ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (ArcSDETestConfig::QClassNameTrees());
        FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
        FdoPtr<FdoValueExpression> expression = (FdoValueExpression*)FdoExpression::Parse (L"GeomFromText('POINT (-88 44)')");
        FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create (geom_prop_name, expression);
        values->Add (value);
        FdoPtr<FdoIFeatureReader> reader = insert->Execute ();
        CPPUNIT_ASSERT_MESSAGE("Failed to read inserted row's id", reader->ReadNext());
        int id = reader->GetInt32(id_prop_name);
        CPPUNIT_ASSERT_MESSAGE("Shouldn't be anymore rows", !reader->ReadNext());
        reader->Close ();

        // check by doing a select
        FdoPtr<FdoISelect> selectCmd = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        selectCmd->SetFeatureClassName (ArcSDETestConfig::QClassNameTrees());
        wchar_t filter[1024];
        FdoCommonOSUtil::swprintf(filter, ELEMENTS(filter), L"%ls = %d", id_prop_name, id);
        selectCmd->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (filter)));
        reader = selectCmd->Execute ();
        int id2 = 0;
        while (reader->ReadNext ())
        {
            id2 = reader->GetInt32 (id_prop_name);
        }
        reader->Close ();
        CPPUNIT_ASSERT_MESSAGE("Id mismatch", id==id2);

        // delete the inserted record
        FdoPtr<FdoIDelete> del = (FdoIDelete*)mConnection->CreateCommand (FdoCommandType_Delete);
        del->SetFeatureClassName (ArcSDETestConfig::QClassNameTrees());
        FdoCommonOSUtil::swprintf(filter, ELEMENTS(filter), L"%ls = %d", id_prop_name, id);
        del->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (filter)));
        del->Execute ();

        // check it's deleted by doing a select again
        reader = selectCmd->Execute ();
        while (reader->ReadNext ())
        {
            CPPUNIT_FAIL ("feature not deleted");
        }
        reader->Close();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* Test spatial filter. */
void BasicDeleteTests::spatial_filter ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        wchar_t fgftFilter[1024];

        // Clean up previous tests, via a spatial filtered delete:
        FdoPtr<FdoIDelete> del = (FdoIDelete*)mConnection->CreateCommand (FdoCommandType_Delete);
        del->SetFeatureClassName (ArcSDETestConfig::QClassNameTrees());
        FdoCommonOSUtil::swprintf(fgftFilter, ELEMENTS(fgftFilter), L"%ls WITHIN GEOMFROMTEXT ('POLYGON XY (( -93 41, -91 41, -91 43, -93 43, -93 41 ))')", geom_prop_name);
        del->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (fgftFilter)));
        del->Execute ();

        // insert a few points
        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (ArcSDETestConfig::QClassNameTrees());
        FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
        FdoPtr<FdoValueExpression> expression = (FdoValueExpression*)FdoExpression::Parse (L"GeomFromText('POINT (-95 42)')");
        FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create (geom_prop_name, expression);
        values->Add (value);
        FdoPtr<FdoIFeatureReader> reader = insert->Execute ();
        int id1;
        while (reader->ReadNext ())
        {
            id1 = reader->GetInt32 (id_prop_name);
        }
        reader->Close();
        expression = (FdoValueExpression*)FdoExpression::Parse (L"GeomFromText('POINT (-92 42)')");
        value->SetValue (expression);
        reader = insert->Execute ();
        int id2;
        while (reader->ReadNext ())
        {
            id2 = reader->GetInt32 (id_prop_name);
        }
        reader->Close();
        expression = (FdoValueExpression*)FdoExpression::Parse (L"NULL");
        value->SetValue (expression);
        reader = insert->Execute ();
        int id3;
        while (reader->ReadNext ())
        {
            id3 = reader->GetInt32 (id_prop_name);
        }
        reader->Close();

        // do a spatial filtered delete
        del = (FdoIDelete*)mConnection->CreateCommand (FdoCommandType_Delete);
        del->SetFeatureClassName (ArcSDETestConfig::QClassNameTrees());
        FdoCommonOSUtil::swprintf(fgftFilter, ELEMENTS(fgftFilter), L"%ls INTERSECTS GEOMFROMTEXT ('POLYGON XY (( -93 41, -91 41, -91 43, -93 43, -93 41 ))')", geom_prop_name);
        del->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (fgftFilter)));
        del->Execute ();

        // test with a select
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameTrees());
        reader = select->Execute ();
        bool bFoundId1 = false;
        bool bFoundId3 = false;
        while (reader->ReadNext ())
        {
            int id = reader->GetInt32 (id_prop_name);
            CPPUNIT_ASSERT_MESSAGE ("feature not deleted", id2 != id);
            if (id1==id) bFoundId1 = true;
            if (id3==id) bFoundId3 = true;
        }
        CPPUNIT_ASSERT_MESSAGE ("feature erroneously deleted", bFoundId1);
        CPPUNIT_ASSERT_MESSAGE ("feature erroneously deleted", bFoundId3);
        reader->Close();

        // Clean up other 2 features:
        FdoCommonOSUtil::swprintf(fgftFilter, ELEMENTS(fgftFilter), L"%ls IN (%d, %d)", id_prop_name, id1, id3);
        del->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (fgftFilter)));
        del->Execute ();

    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}
