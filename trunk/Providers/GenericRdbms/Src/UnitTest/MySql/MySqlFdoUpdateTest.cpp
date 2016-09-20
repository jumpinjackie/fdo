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
#include "MySqlFdoUpdateTest.h"
#include "UnitTestUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION( MySqlFdoUpdateTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( MySqlFdoUpdateTest, "FdoUpdateTest");

void MySqlFdoUpdateTest::set_provider()
{
	UnitTestUtil::SetProvider( "MySql" );
	m_hasAssoc = false;
	m_hasGeom = true;
}

void MySqlFdoUpdateTest::testForeignDataStore()
{
    FdoPtr<FdoIConnection> connection;
    try
    {
        printf( "Initializing Connection ... \n" );
	    FdoStringP userConnectString = UnitTestUtil::GetConnectionString(Connection_NoDatastore, L"");
	    connection = UnitTestUtil::GetProviderConnectionObject();
	    connection->SetConnectionString ( userConnectString);
	    connection->Open();

        FdoStringP datastore = UnitTestUtil::GetEnviron("datastore", L"_foreign_sch");
        FdoStringP pQueryStringCr = L"CREATE DATABASE ";
	    pQueryStringCr += datastore;
	    pQueryStringCr += L";";
        try
        {
            UnitTestUtil::Sql2Db( (const wchar_t*) pQueryStringCr, connection.p );
            connection->Close();
            userConnectString = UnitTestUtil::GetConnectionString(Connection_WithDatastore, L"_foreign_sch");
	        connection->SetConnectionString ( userConnectString);
	        connection->Open();
            UnitTestUtil::Sql2Db( (const wchar_t**) mForeignPolygonTest, connection.p );
            connection->Close();
        }
        catch(FdoException* e){e->Release();}
        catch(...){}
        
        userConnectString = UnitTestUtil::GetConnectionString(Connection_WithDatastore, L"_foreign_sch");
        connection->SetConnectionString ( userConnectString);
	    connection->Open();

        FdoPtr<FdoIDescribeSchema>  pDescSchemaCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
        FdoFeatureSchemasP fsc = pDescSchemaCmd->Execute();
        fsc->WriteXml( UnitTestUtil::GetOutputFileName( L"describeSchemasf.xml" ) );

        double pPoints[10];
        double pPoints2[10];
        pPoints[0] = 249.54; pPoints[1] = 576.84;
        pPoints[2] = 249.54; pPoints[3] = 516.33;
        pPoints[4] = 146.90; pPoints[5] = 516.33;
        pPoints[6] = 146.90; pPoints[7] = 576.84;
        pPoints[8] = 249.54; pPoints[9] = 576.84;

        pPoints2[0] = 29.54; pPoints2[1] = 76.84;
        pPoints2[2] = 29.54; pPoints2[3] = 16.33;
        pPoints2[4] = 16.90; pPoints2[5] = 16.33;
        pPoints2[6] = 16.90; pPoints2[7] = 76.84;
        pPoints2[8] = 29.54; pPoints2[9] = 76.84;
        try
        {
            FdoPtr<FdoITransaction> featureTransaction = connection->BeginTransaction();
            FdoPtr<FdoIInsert> insertCommand = (FdoIInsert *)connection->CreateCommand(FdoCommandType_Insert);
            insertCommand->SetFeatureClassName(L"foreignpolygon");
            FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();
            
            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
            FdoPtr<FdoPropertyValue> propertyValue;

            propertyValue = AddNewProperty( propertyValues, L"GEOMETRY");
            FdoPtr<FdoILineString> line1 = gf->CreateLineString(FdoDimensionality_XY, 2*2, pPoints);
            FdoPtr<FdoByteArray> byteArray = gf->GetFgf(line1);

            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

            FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();
            featureTransaction->Commit();
            UnitTestUtil::FailOnException( FdoException::Create( L"testForeignDataStore step 1 should fail!" ));
        }
        catch ( FdoException* e )
        {
            printf( "Expected exception:%ls\n", e->GetExceptionMessage());
            e->Release();
        }
        try
        {
            FdoPtr<FdoITransaction> featureTransaction = connection->BeginTransaction();
            FdoPtr<FdoIInsert> insertCommand = (FdoIInsert *)connection->CreateCommand(FdoCommandType_Insert);
            insertCommand->SetFeatureClassName(L"foreignpolygon");
            FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();
            
            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
            FdoPtr<FdoPropertyValue> propertyValue;

            propertyValue = AddNewProperty( propertyValues, L"GEOMETRY");
            FdoPtr<FdoILinearRing> extRing = gf->CreateLinearRing(FdoDimensionality_XY, 10, pPoints);
            FdoPtr<FdoIPolygon> poly1 = gf->CreatePolygon(extRing, NULL );
            
            FdoPtr<FdoByteArray> byteArray = gf->GetFgf(poly1);
            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

            FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();
            featureTransaction->Commit();
        }
        catch ( FdoException* e )
        {
            UnitTestUtil::FailOnException( e );
        }
        try
        {
            FdoPtr<FdoITransaction> featureTransaction = connection->BeginTransaction();
            FdoPtr<FdoIInsert> insertCommand = (FdoIInsert *)connection->CreateCommand(FdoCommandType_Insert);
            insertCommand->SetFeatureClassName(L"foreignpolygon");
            FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();
            
            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
            FdoPtr<FdoPropertyValue> propertyValue;

            propertyValue = AddNewProperty( propertyValues, L"GEOMETRY");
            FdoPtr<FdoILinearRing> extRing1 = gf->CreateLinearRing(FdoDimensionality_XY, 10, pPoints);
            FdoPtr<FdoIPolygon> poly1 = gf->CreatePolygon(extRing1, NULL );

            FdoPtr<FdoILinearRing> extRing2 = gf->CreateLinearRing(FdoDimensionality_XY, 10, pPoints2);
            FdoPtr<FdoIPolygon> poly2 = gf->CreatePolygon(extRing2, NULL );
            FdoPtr<FdoPolygonCollection> pPolCol = FdoPolygonCollection::Create();
            pPolCol->Add(poly1);
            pPolCol->Add(poly2);
            FdoPtr<FdoIMultiPolygon> pObjIns = gf->CreateMultiPolygon(pPolCol);

            FdoPtr<FdoByteArray> byteArray = gf->GetFgf(pObjIns);
            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

            FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();
            featureTransaction->Commit();
            UnitTestUtil::FailOnException( FdoException::Create( L"testForeignDataStore step 3 should fail!" ));
        }
        catch ( FdoException* e )
        {
            printf( "Expected exception:%ls\n", e->GetExceptionMessage());
            e->Release();
        }
    }
    catch ( FdoException* e )
    {
        try {
            if (connection)
                connection->Close();
        }
        catch ( ... )
        {
        }
        UnitTestUtil::FailOnException( e );
    }
    catch (...)
    {
        if (connection)
            connection->Close();
        throw;
    }
}

const wchar_t* MySqlFdoUpdateTest::mForeignPolygonTest[] = {
    L"CREATE TABLE foreignpolygon",
    L"(GEOMETRY POLYGON,",
    L"ENTITYID INT AUTO_INCREMENT PRIMARY KEY,",
    L"VERSIONNUMBER SMALLINT,",
    L"LOCKID TINYINT,",
    L"FSBLOCKNAME TEXT(255),",
    L"DESCR TEXT(50),",
    L"FSCLOSED CHAR(1),",
    L"FSCOLOR INT,",
    L"FSENTITYTYPE CHAR(30),",
    L"FSHYPERLINK DATETIME,",
    L"FSJUSTIFICATION DECIMAL(5,2),",
    L"FSLAYER VARCHAR(255),",
    L"FSLINETYPE TINYTEXT,",
    L"FSGEOMETRYTYPE SET('1','2','3','4','5','6','7','8','9','10','11','12'),",
    L"FSLINEWEIGHT DECIMAL(38,8),",
    L"FSOLYPATCOLOR BIGINT,",
    L"FSOLYPATTYPE VARCHAR(100),",
    L"FSPLOTSTYLE MEDIUMTEXT,",
    L"FSROTATION DECIMAL,",
    L"FSTEXTCONTENT LONGTEXT,",
    L"FSTEXTHEIGHT MEDIUMINT,",
    L"FSTHICKNESS DECIMAL(35),",
    L"FSWIDTH DOUBLE,",
    L"FSBLOB MEDIUMBLOB,",
    L"FSGEOMETRICTYPE ENUM('one', 'two', 'three', 'four'),",
    L"FSTIME TIME,",
    L"FSYEAR YEAR,",
    L"FSDATE DATE) ENGINE=MyISAM;",
    NULL
};

FdoString* MySqlFdoUpdateTest::NoMetaSuffix()
{
    FdoString* noMetaSuffix = L"_no meta\x30b0\x30b1";

    return noMetaSuffix;
}

