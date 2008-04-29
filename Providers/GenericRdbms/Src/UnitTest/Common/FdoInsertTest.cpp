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
#include "FdoInsertTest.h"
#include "UnitTestUtil.h"

#ifdef _DEBUG
#define  DBG(X)    X
#else
#define  DBG(X)
#endif

#ifndef _LLONG_MAX
#   define LLONG_MAX    9223372036854775807LL
#endif
#ifndef LLONG_MIN
#   define LLONG_MIN    (-LLONG_MAX - 1LL)
#endif

FdoInsertTest::FdoInsertTest(void)
{
    mSuffix[0] = '\0';
}

FdoInsertTest::FdoInsertTest(FdoString *suffix)
{
    m_DisableFailures = false;
    wcsncpy(mSuffix, suffix, 11 );
    mSuffix[11] = '\0';
}

FdoInsertTest::~FdoInsertTest(void)
{
}

void FdoInsertTest::setUp ()
{
    set_provider();
}
typedef struct _featinfo_ {
    int  featNum;
    int  seq;
    double x;
    double y;
} FeatInfo_t;

typedef struct _featattr_ {
    int  featNum;
    int  seq;
    char layer[12];
} FeatAttr_t;

static FeatInfo_t  feats[10000];
static FeatAttr_t  attrs[1100];

void FdoInsertTest::insert ()
{
    MainInsertTest();
}

FdoPropertyValue* FdoInsertTest::AddNewProperty( FdoPropertyValueCollection* propertyValues, const wchar_t *name )
{
    FdoPropertyValue*  propertyValue = NULL;

    propertyValue = propertyValues->FindItem( name );
    if ( !propertyValue )
    {
        propertyValue =  FdoPropertyValue::Create();
        propertyValue->SetName( name );
        propertyValues->Add( propertyValue );
    }

    return propertyValue;
}

void FdoInsertTest::MainInsertTest (FdoIConnection *conn)
{
	clock_t start, finish;
    FdoPtr<FdoIConnection> connection;
    try
    {
        if( conn == NULL )
            connection = UnitTestUtil::GetConnection(mSuffix, true);
        else
        {
            conn->AddRef();
            connection = conn;
        }

        try
        {
            static double       coordsBuffer[400];
            int                 nRetCode = 0;
            int                 i;
            char                line[256];
            int                 coordNum,attrNum,featId,segCount;
            int                 count = 0;

            char                coord_file[256];
            char                attr_file[256];
            bool supportsZ = (FdoPtr<FdoIGeometryCapabilities>(connection->GetGeometryCapabilities())->GetDimensionalities() & FdoDimensionality_Z);

#ifdef _WIN32
            strcpy(coord_file,"g_coords.txt");
            strcpy(attr_file,"polyAttr.txt");
#else
            char *root = getenv("ROOT");
            char dir[256];
            if (root)
                sprintf(dir, "%s/src/fdo/unit_test/", root);
            else
                strcpy(dir, "./");
            sprintf(coord_file, "%s%s", dir, "g_coords.txt");
            sprintf(attr_file, "%s%s", dir, "polyAttr.txt");
#endif

            FILE *coor = fopen(coord_file, "r");
            FILE *attr = fopen(attr_file, "r");

            for( i=0;!feof(coor) && i < 10000; i++ )
            {
                fscanf(coor, "%d,%d,%lf,%lf",&feats[i].featNum, &feats[i].seq,&feats[i].x,&feats[i].y);
            }
            coordNum = i;

            for( i=0;!feof(attr) && i < 1100; i++ )
            {
                fscanf(attr, "%d,%d,%s",&attrs[i].featNum, &attrs[i].seq,&attrs[i].layer);
            }

            attrNum = i;
            featId = -1;
            segCount = 0;
            line[0] = '\0';
            FdoPtr<FdoITransaction> featureTransaction = connection->BeginTransaction();

            FdoPtr<FdoIInsert> insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
            insertCommand->SetFeatureClassName(L"Acad:AcDb3dPolyline");
            FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();

            FdoPtr<FdoIInsert> insertCommand2 = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
            insertCommand2->SetFeatureClassName(L"AcDbEntity.xdata2");
            FdoPtr<FdoPropertyValueCollection> propertyValues2 = insertCommand2->GetPropertyValues();


            // Used to insert new parcels
            FdoPtr<FdoIInsert>insertCommand3 = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
            insertCommand3->SetFeatureClassName(L"L\x00e4nd:Parcel");
            FdoPtr<FdoPropertyValueCollection> propertyValues3 = insertCommand3->GetPropertyValues();

            int length = 0;
            start = clock();
            
			printf("start insert feature class\n");
            FdoPtr<FdoDataValue> dataValue;
            FdoPtr<FdoPropertyValue> propertyValue;
            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
            int count1 = 0;
            long feat;
            for (i=0; i<coordNum; i++)
            {
                feat = 0;
                if (feats[i].featNum != featId)
                {
                    if (length != 0)
                    {
                        propertyValue = AddNewProperty( propertyValues, L"Geometry" );
                        FdoPtr<FdoILineString> line1;
                        if( supportsZ )
                            line1 = gf->CreateLineString(FdoDimensionality_XY|FdoDimensionality_Z, segCount*3, coordsBuffer);
                        else
                            line1 = gf->CreateLineString(FdoDimensionality_XY, segCount*2, coordsBuffer);

                        FdoPtr<FdoByteArray> byteArray = gf->GetFgf(line1);

                        FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
                        propertyValue->SetValue(geometryValue);

                        int j;
                        for(j=0;j<attrNum;j++)
                            if( featId == attrs[j].featNum )
                                break;


                        if( j != attrNum )
                        {
                            dataValue = FdoDataValue::Create(L"256");
                            propertyValue = AddNewProperty( propertyValues,L"color"); //AddNewProperty( propertyValues, L"color");
                            propertyValue->SetValue(dataValue);

                            dataValue = FdoDataValue::Create(segCount);
                            propertyValue = AddNewProperty( propertyValues, L"segcount");
                            propertyValue->SetValue(dataValue);

                            FdoStringP  layer( attrs[j].layer );
                            dataValue = FdoDataValue::Create((const wchar_t*)layer);
                            propertyValue = AddNewProperty( propertyValues, L"layer");
                            propertyValue->SetValue(dataValue);

                            dataValue = FdoDataValue::Create(1);
                            propertyValue = AddNewProperty( propertyValues, L"xdata.seq");
                            propertyValue->SetValue(dataValue);

                            dataValue = FdoDataValue::Create(L"abc");
                            propertyValue = AddNewProperty( propertyValues, L"xdata.DataValue");
                            propertyValue->SetValue(dataValue);

                            dataValue = FdoDataValue::Create(22);
                            propertyValue = AddNewProperty( propertyValues, L"xdata.AcXObj.intdata");
                            propertyValue->SetValue(dataValue);
                            dataValue = FdoDataValue::Create(L"This a string value");
                            propertyValue = AddNewProperty( propertyValues, L"xdata.AcXObj.strdata");
                            propertyValue->SetValue(dataValue);

                        }
                        else
                        {
                            dataValue = FdoDataValue::Create(L"256");
                            propertyValue = AddNewProperty( propertyValues, L"color");
                            propertyValue->SetValue(dataValue);

                            dataValue = FdoDataValue::Create(1);
                            propertyValue = AddNewProperty( propertyValues, L"segcount");
                            propertyValue->SetValue(dataValue);

                            dataValue = FdoDataValue::Create(L"100");
                            propertyValue = AddNewProperty( propertyValues, L"layer");
                            propertyValue->SetValue(dataValue);

                            dataValue = FdoDataValue::Create(1);
                            propertyValue = AddNewProperty( propertyValues, L"xdata.seq");
                            propertyValue->SetValue(dataValue);

                            dataValue = FdoDataValue::Create(L"abc");
                            propertyValue = AddNewProperty( propertyValues, L"xdata.DataValue");
                            propertyValue->SetValue(dataValue);

                            dataValue = FdoDataValue::Create(22);
                            propertyValue = AddNewProperty( propertyValues, L"xdata.AcXObj.intdata");
                            propertyValue->SetValue(dataValue);
                            dataValue = FdoDataValue::Create(L"This a string value");
                            propertyValue = AddNewProperty( propertyValues, L"xdata.AcXObj.strdata");
                            propertyValue->SetValue(dataValue);

                        }
                        FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();

                        while( reader->ReadNext() )
                        {
                            if( ! reader->IsNull( L"FeatId" ) )
                            {
                                feat = (long)reader->GetInt64(L"FeatId");
                                dataValue = FdoDataValue::Create((FdoInt64) feat);
                                propertyValue = AddNewProperty( propertyValues2, L"AcDbEntity_FeatId");
                                propertyValue->SetValue(dataValue);
                            }
                        }

                        dataValue = FdoDataValue::Create((FdoInt64) feat);
                        propertyValue = AddNewProperty( propertyValues2, L"AcDbEntity_FeatId");
                        propertyValue->SetValue(dataValue);

                        dataValue = FdoDataValue::Create(2);
                        propertyValue = AddNewProperty( propertyValues2, L"seq");
                        propertyValue->SetValue(dataValue);

                        dataValue = FdoDataValue::Create(L"xyz");
                        propertyValue = AddNewProperty( propertyValues2, L"DataValue");
                        propertyValue->SetValue(dataValue);

                        FdoPtr<FdoIFeatureReader> reader2 = insertCommand2->Execute();
                        if (reader2->ReadNext())
                        {
                            FdoPtr<FdoClassDefinition> classDef = reader2->GetClassDefinition();
                        }

                        dataValue = FdoDataValue::Create((FdoInt64) feat);
                        propertyValue = AddNewProperty( propertyValues2, L"AcDbEntity_FeatId");
                        propertyValue->SetValue(dataValue);

                        dataValue = FdoDataValue::Create(1);
                        propertyValue = AddNewProperty( propertyValues2, L"seq");
                        propertyValue->SetValue(dataValue);

                        dataValue = FdoDataValue::Create(L"xyz2");
                        propertyValue = AddNewProperty( propertyValues2, L"DataValue");
                        propertyValue->SetValue(dataValue);

                        FdoPtr<FdoIFeatureReader> reader3 = insertCommand2->Execute();

                        // Insert Parcel record
                        dataValue = FdoDataValue::Create(39);
                        propertyValue = AddNewProperty( propertyValues3, L"Value");
                        propertyValue->SetValue(dataValue);

                        dataValue = FdoDataValue::Create(L"Ontario");
                        propertyValue = AddNewProperty( propertyValues3, L"Pr\x00f6vince");
                        propertyValue->SetValue(dataValue);
                        FdoStringP id = FdoStringP::Format(L"P%d_%d",feat,count1);
                        dataValue = FdoDataValue::Create((FdoString*) id);
                        propertyValue = AddNewProperty( propertyValues3, L"PIN");
                        propertyValue->SetValue(dataValue);

                        propertyValue = AddNewProperty( propertyValues3, UnitTestUtil::GetNlsObjectName(L"Geometry"));
                        propertyValue->SetValue(geometryValue);

//                        if( count1 > 7 )
						{
							FdoPtr<FdoIFeatureReader> reader4 = insertCommand3->Execute();
							while( reader4->ReadNext() ) {
								if( !reader4->IsNull( L"PIN" ) )	{
									FdoString* pin = reader4->GetString(L"PIN");
									DBG(printf("inserted pin=%ls\n", pin));
								}
							}
						}

                        count1++;
                        if (count1 == 10)
                            break;

                    }
                    featId = feats[i].featNum;

                    length = -1;
                    coordsBuffer[0] = feats[i].x;
                    coordsBuffer[1] = feats[i].y;
                    if( supportsZ )
                        coordsBuffer[2] = 0;

                    segCount=1;
                }
                else
                {
                    int off = ( supportsZ )?3:2;
                    length = -1;
                    coordsBuffer[segCount*off] = feats[i].x;
                    coordsBuffer[segCount*off+1] = feats[i].y;
                    if( supportsZ )
                        coordsBuffer[segCount*off+2] = 0;
                    segCount++;
                }
            }

            featureTransaction->Commit();

			finish = clock();
			printf( "Elapsed: %f seconds\n", ((double)(finish - start) / CLOCKS_PER_SEC) );
        }
        catch (...)
        {
            if (connection)
                connection->Close ();
            throw;
        }
        if (connection)
            connection->Close();
    }
    catch (FdoException *ex)
    {
        if (m_DisableFailures)
            throw ex;
        else
            TestCommonFail(ex);
    }
}

void FdoInsertTest::insert2 ()
{
	clock_t start, finish;
    try
    {
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(mSuffix, false);

        try
        {
           // FdoPtr<FdoITransaction> featureTransaction = connection->BeginTransaction();
            FdoPtr<FdoIInsert> insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
            insertCommand->SetFeatureClassName(L"testClass");
            FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();
            FdoPtr<FdoIInsert> insertCommand2 = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
            insertCommand2->SetFeatureClassName(L"testClass.Object2");
            FdoPtr<FdoPropertyValueCollection> propertyValues2 = insertCommand2->GetPropertyValues();

            start = clock();
            
			printf("start insert non-feature class\n");
            FdoPtr<FdoDataValue> dataValue;
            FdoPtr<FdoPropertyValue> propertyValue;

            try {
                UnitTestUtil::Sql2Db(L"delete from testclass_objectclass2 where testclass_lastname = 'AnotherLastname'", static_cast<FdoIConnection *>(connection));
            } catch (FdoException *ex){ex->Release();}
            try {
                UnitTestUtil::Sql2Db(L"delete from testclass_objectclass where testclass_lastname = 'AnotherLastname'", static_cast<FdoIConnection *>(connection));
            } catch(FdoException *ex){ex->Release();}

            try {
                UnitTestUtil::Sql2Db(L"delete from testclass where lastname = 'AnotherLastname'", static_cast<FdoIConnection *>(connection));
            }catch (FdoException *ex){ex->Release();}
            dataValue = FdoDataValue::Create(L"AnotherFirstName");
            propertyValue = AddNewProperty( propertyValues, L"FirstName");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(L"AnotherLastname");
            propertyValue = AddNewProperty( propertyValues, L"LastName");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(36);
            propertyValue = AddNewProperty( propertyValues, L"Age");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(130);
            propertyValue = AddNewProperty( propertyValues, L"Object.ObjectWeight");
            propertyValue->SetValue(dataValue);

            FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();


            dataValue = FdoDataValue::Create(L"AnotherFirstName");
            propertyValue = AddNewProperty( propertyValues2, L"testClass_FirstName");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(L"AnotherLastname");
            propertyValue = AddNewProperty( propertyValues2, L"testClass_LastName");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(1);
            propertyValue = AddNewProperty( propertyValues2, L"Colours");
            propertyValue->SetValue(dataValue);

            FdoPtr<FdoIFeatureReader> reader2 = insertCommand2->Execute();

            dataValue = FdoDataValue::Create(L"AnotherFirstName");
            propertyValue = AddNewProperty( propertyValues2, L"testClass_FirstName");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(L"AnotherLastname");
            propertyValue = AddNewProperty( propertyValues2, L"testClass_LastName");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(2);
            propertyValue = AddNewProperty( propertyValues2, L"Colours");
            propertyValue->SetValue(dataValue);

            FdoPtr<FdoIFeatureReader> reader3 = insertCommand2->Execute();

          //  featureTransaction->Commit();
			finish = clock();
			printf( "Elapsed: %f seconds\n", ((double)(finish - start) / CLOCKS_PER_SEC) );
        }
        catch (...)
        {
            if (connection)
                connection->Close ();
            throw;
        }
        if (connection)
            connection->Close();
    }
    catch (FdoException *ex)
    {
        TestCommonFail(ex);
    }
}

void FdoInsertTest::insert3 ()
{
    try
    {
        wchar_t *data[] = {
            L"12                                                                             1234567",
            L"1234",
            L"aaaaaaaaaabbbbbbbbbbccccccccccddddddddddeeeeeeeeeefffffffffffgggggggggghhhhhhhhhhiiiiiiiiii",
            L"3333"};

        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(mSuffix, false);

        try
        {
            double       coordsBuffer[400];
            int                 segCount = 2;
            bool supportsZ = (FdoPtr<FdoIGeometryCapabilities>(connection->GetGeometryCapabilities())->GetDimensionalities() & FdoDimensionality_Z);

            FdoPtr<FdoPropertyValue> propertyValue;

            FdoPtr<FdoITransaction> featureTransaction = connection->BeginTransaction();
            FdoPtr<FdoIInsert> insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
            insertCommand->SetFeatureClassName(L"Acad:AcDb3dPolyline");
            FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();

            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

            int ord = 0;
            coordsBuffer[ord++] = 1.1;
            coordsBuffer[ord++] = 2.2;
            if ( supportsZ ) 
                coordsBuffer[ord++] = 0.0;
            coordsBuffer[ord++] = 1.1;
            coordsBuffer[ord++] = 3.3;
            if ( supportsZ ) 
                coordsBuffer[ord++] = 0.0;

            propertyValue = AddNewProperty( propertyValues, L"Geometry");
            FdoPtr<FdoILineString> line1 = gf->CreateLineString(
                supportsZ ? FdoDimensionality_XY|FdoDimensionality_Z : FdoDimensionality_XY, 
                ord, 
                coordsBuffer
             );
            FdoPtr<FdoByteArray> byteArray = gf->GetFgf(line1);

            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

            FdoPtr<FdoDataValue> dataValue;
            dataValue = FdoDataValue::Create(L"652");
            propertyValue = AddNewProperty( propertyValues, L"color");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(segCount);
            propertyValue = AddNewProperty( propertyValues, L"segcount");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(L"200");
            propertyValue = AddNewProperty( propertyValues, L"layer");
            propertyValue->SetValue(dataValue);

			dataValue = FdoDataValue::Create(true);
			propertyValue = AddNewProperty( propertyValues, L"boolean");
			propertyValue->SetValue(dataValue);

			FdoInt16 byteValue = 7;
			dataValue = FdoDataValue::Create((FdoByte)byteValue);
			propertyValue = AddNewProperty( propertyValues, L"byte");
			propertyValue->SetValue(dataValue);

			FdoFloat floatValue = (FdoFloat)3.1415901;
			dataValue = FdoDataValue::Create(floatValue);
			propertyValue = AddNewProperty( propertyValues, L"single");
			propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(1);
            propertyValue = AddNewProperty( propertyValues, L"xdata.seq");
            propertyValue->SetValue(dataValue);

            for (int j=0; j<4; j++)
            {
                dataValue = FdoDataValue::Create(data[j]);
                propertyValue = AddNewProperty( propertyValues, L"xdata.DataValue");
                propertyValue->SetValue(dataValue);
                FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();
            }

            featureTransaction->Commit();
			//featureTransaction->Rollback();
			// check 
			FdoPtr<FdoISelect> selectCmd = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);
			selectCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
			FdoPtr<FdoFilter> filter = FdoComparisonCondition::Create(
					   FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"color")),
					   FdoComparisonOperations_EqualTo, 
					   FdoPtr<FdoDataValue>(FdoDataValue::Create(L"652")));
			selectCmd->SetFilter(filter);

			FdoPtr<FdoIFeatureReader> featureReader = selectCmd->Execute();
			bool isTrue;
			while (featureReader->ReadNext())
			{
				if (!featureReader->IsNull(L"boolean"))
				{
					isTrue = featureReader->GetBoolean(L"boolean");
					if (!isTrue)
						throw FdoException::Create(L"boolean: 'false' is not valid");
				}
				else
					throw FdoException::Create(L"boolean: 'null' is not valid");

				if (!featureReader->IsNull(L"byte"))
				{
					char bValue = featureReader->GetByte(L"byte");
					if (bValue != 7)
						throw FdoException::Create(L"byte: value is not valid");
				}
				else
					throw FdoException::Create(L"byte: 'null' is not valid");

                if (!featureReader->IsNull(L"single"))
				{
					FdoFloat bValue = featureReader->GetSingle(L"single");
					if (bValue != (FdoFloat) 3.1415901)
						throw FdoException::Create(L"single: value is not valid");
				}
				else
					throw FdoException::Create(L"single: 'null' is not valid");
			}
			featureReader->Close();
        }
        catch (...)
        {
            if (connection)
                connection->Close ();
            throw;
        }
        if (connection)
            connection->Close();
    }
    catch (FdoCommandException *ex)
    {
        UnitTestUtil::FailOnException(ex);
    }
    catch (FdoException *ex)
    {
        UnitTestUtil::FailOnException(ex);
    }
}

void FdoInsertTest::insertBoundary()
{
    try
    {

        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(mSuffix, true);

        try
        {
            bool supportsZ = (FdoPtr<FdoIGeometryCapabilities>(connection->GetGeometryCapabilities())->GetDimensionalities() & FdoDimensionality_Z);
            insertBoundaryCleanup( connection );

            double       coordsBuffer[400];
            int                 oordCount = 0;

            FdoPtr<FdoPropertyValue> propertyValue;

            FdoPtr<FdoITransaction> featureTransaction = connection->BeginTransaction();
            FdoPtr<FdoIInsert> insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
            insertCommand->SetFeatureClassName(L"Acad:AcDb3dPolyline");
            FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();

            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

			// Use 2D points to accomodate MySql 
            coordsBuffer[oordCount++] = 1.1;
            coordsBuffer[oordCount++] = 2.2;
            if ( supportsZ ) 
                coordsBuffer[oordCount++] = 0.0;

            coordsBuffer[oordCount++] = 1.1;
            coordsBuffer[oordCount++] = 3.3;
            if ( supportsZ ) 
                coordsBuffer[oordCount++] = 0.0;

            propertyValue = AddNewProperty( propertyValues, L"Geometry");
            FdoPtr<FdoILineString> line1 = gf->CreateLineString(
                supportsZ ? FdoDimensionality_XY|FdoDimensionality_Z : FdoDimensionality_XY, 
                oordCount, 
                coordsBuffer
            );
            FdoPtr<FdoByteArray> byteArray = gf->GetFgf(line1);

            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

            FdoPtr<FdoDataValue> dataValue;
            dataValue = FdoDataValue::Create(L"801");
            propertyValue = AddNewProperty( propertyValues, L"color");
            propertyValue->SetValue(dataValue);

			dataValue = FdoDataValue::Create(GetMinByteValue());
			propertyValue = AddNewProperty( propertyValues, L"byte");
			propertyValue->SetValue(dataValue);

			dataValue = FdoDataValue::Create(GetMinInt16Value());
			propertyValue = AddNewProperty( propertyValues, L"int16");
			propertyValue->SetValue(dataValue);

			dataValue = FdoDataValue::Create(GetMinInt32Value());
			propertyValue = AddNewProperty( propertyValues, L"int32");
			propertyValue->SetValue(dataValue);

			dataValue = FdoDataValue::Create(GetMinInt64Value());
			propertyValue = AddNewProperty( propertyValues, L"int64");
			propertyValue->SetValue(dataValue);

			dataValue = FdoDataValue::Create(GetMinSingleValue());
			propertyValue = AddNewProperty( propertyValues, L"single");
			propertyValue->SetValue(dataValue);

			dataValue = FdoDataValue::Create(GetMinDoubleValue(), FdoDataType_Double);
			propertyValue = AddNewProperty( propertyValues, L"double");
			propertyValue->SetValue(dataValue);

            FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();

            dataValue = FdoDataValue::Create(L"802");
            propertyValue = AddNewProperty( propertyValues, L"color");
            propertyValue->SetValue(dataValue);

			dataValue = FdoDataValue::Create(GetMaxByteValue());
			propertyValue = AddNewProperty( propertyValues, L"byte");
			propertyValue->SetValue(dataValue);

			dataValue = FdoDataValue::Create(GetMaxInt16Value());
			propertyValue = AddNewProperty( propertyValues, L"int16");
			propertyValue->SetValue(dataValue);

			dataValue = FdoDataValue::Create(GetMaxInt32Value());
			propertyValue = AddNewProperty( propertyValues, L"int32");
			propertyValue->SetValue(dataValue);

			dataValue = FdoDataValue::Create(GetMaxInt64Value());
			propertyValue = AddNewProperty( propertyValues, L"int64");
			propertyValue->SetValue(dataValue);

			dataValue = FdoDataValue::Create(GetMaxSingleValue());
			propertyValue = AddNewProperty( propertyValues, L"single");
			propertyValue->SetValue(dataValue);

			dataValue = FdoDataValue::Create(GetMaxDoubleValue(), FdoDataType_Double);
			propertyValue = AddNewProperty( propertyValues, L"double");
			propertyValue->SetValue(dataValue);

            reader = insertCommand->Execute();

            dataValue = FdoDataValue::Create(L"803");
            propertyValue = AddNewProperty( propertyValues, L"color");
            propertyValue->SetValue(dataValue);

			dataValue = FdoDataValue::Create(GetMaxInt64Value() - 1);
			propertyValue = AddNewProperty( propertyValues, L"int64");
			propertyValue->SetValue(dataValue);

			dataValue = FdoDataValue::Create(GetSmallestSingleValue());
			propertyValue = AddNewProperty( propertyValues, L"single");
			propertyValue->SetValue(dataValue);

			dataValue = FdoDataValue::Create(GetSmallestDoubleValue(), FdoDataType_Double);
			propertyValue = AddNewProperty( propertyValues, L"double");
			propertyValue->SetValue(dataValue);

            reader = insertCommand->Execute();

            featureTransaction->Commit();
			//featureTransaction->Rollback();
			// check 
			FdoPtr<FdoISelect> selectCmd = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);
			selectCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
			FdoPtr<FdoFilter> filter = FdoComparisonCondition::Create(
					   FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"color")),
					   FdoComparisonOperations_EqualTo, 
					   FdoPtr<FdoDataValue>(FdoDataValue::Create(L"801")));
			selectCmd->SetFilter(filter);

			FdoPtr<FdoIFeatureReader> featureReader = selectCmd->Execute();
			CPPUNIT_ASSERT(featureReader->ReadNext());
            CPPUNIT_ASSERT ( featureReader->GetByte(L"byte") == GetMinByteValue() );
            CPPUNIT_ASSERT ( featureReader->GetInt16(L"int16") == GetMinInt16Value() );
            CPPUNIT_ASSERT ( featureReader->GetInt32(L"int32") == GetMinInt32Value() );
            CPPUNIT_ASSERT ( featureReader->GetInt64(L"int64") == GetMinInt64Value() );
            CPPUNIT_ASSERT ( featureReader->GetSingle(L"single") == GetMinSingleValue() );
            CPPUNIT_ASSERT ( featureReader->GetDouble(L"double") == GetMinDoubleValue() );

			filter = FdoComparisonCondition::Create(
					   FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"color")),
					   FdoComparisonOperations_EqualTo, 
					   FdoPtr<FdoDataValue>(FdoDataValue::Create(L"802")));
			selectCmd->SetFilter(filter);

			featureReader = selectCmd->Execute();
			CPPUNIT_ASSERT(featureReader->ReadNext());
            CPPUNIT_ASSERT ( featureReader->GetByte(L"byte") == GetMaxByteValue() );
            CPPUNIT_ASSERT ( featureReader->GetInt16(L"int16") == GetMaxInt16Value() );
            CPPUNIT_ASSERT ( featureReader->GetInt32(L"int32") == GetMaxInt32Value() );
            CPPUNIT_ASSERT ( featureReader->GetInt64(L"int64") == GetMaxInt64Value() );
            CPPUNIT_ASSERT ( featureReader->GetSingle(L"single") == GetMaxSingleValue() );
            CPPUNIT_ASSERT ( featureReader->GetDouble(L"double") == GetMaxDoubleValue() );

			filter = FdoComparisonCondition::Create(
					   FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"color")),
					   FdoComparisonOperations_EqualTo, 
					   FdoPtr<FdoDataValue>(FdoDataValue::Create(L"803")));
			selectCmd->SetFilter(filter);

			featureReader = selectCmd->Execute();
			CPPUNIT_ASSERT(featureReader->ReadNext());

// Reinstate when defect 917085 is fixed.
#ifndef RDBI_DEF_ORA
            CPPUNIT_ASSERT ( featureReader->GetInt64(L"int64") == (GetMaxInt64Value() - 1) );
            CPPUNIT_ASSERT ( featureReader->GetSingle(L"single") == GetSmallestSingleValue() );
#endif
//            CPPUNIT_ASSERT ( featureReader->GetDouble(L"double") == GetSmallestDoubleValue() );

            insertBoundaryCleanup( connection );
        }
        catch (...)
        {
            if (connection)
                connection->Close ();
            throw;
        }
        if (connection)
            connection->Close ();
    }
    catch (FdoCommandException *ex)
    {
        UnitTestUtil::FailOnException(ex);
    }
    catch (FdoException *ex)
    {
        UnitTestUtil::FailOnException(ex);
    }
}

void FdoInsertTest::insertBoundaryCleanup( FdoIConnection* connection )
{
    FdoPtr<FdoIDelete> deleteCmd = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);
    deleteCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
    FdoPtr<FdoFilter> filter = FdoComparisonCondition::Create(
        FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"color")),
        FdoComparisonOperations_Like, 
        FdoPtr<FdoDataValue>(FdoDataValue::Create(L"80%")));
    deleteCmd->SetFilter(filter);
    deleteCmd->Execute();
}

void FdoInsertTest::insertDate (FdoIConnection *connection, FdoDateTime dateTime, FdoString*colorIndex)
{
    double                    coordsBuffer[400];
    FdoPtr<FdoPropertyValue>  propertyValue;
    bool supportsZ = (FdoPtr<FdoIGeometryCapabilities>(connection->GetGeometryCapabilities())->GetDimensionalities() & FdoDimensionality_Z);

    FdoPtr<FdoITransaction> featureTransaction = connection->BeginTransaction();
    FdoPtr<FdoIInsert> insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
    insertCommand->SetFeatureClassName(L"Acad:AcDb3dPolyline");
    FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();

    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
	FdoPtr<FdoILineString> line1;
	if ( supportsZ ) 
	{
		coordsBuffer[0] = 1.1;
		coordsBuffer[1] = 2.2;
		coordsBuffer[2] = 0.0;
		coordsBuffer[3] = 1.1;
		coordsBuffer[4] = 3.3;
		coordsBuffer[5] = 0.0;
		line1 = gf->CreateLineString(FdoDimensionality_XY|FdoDimensionality_Z, 2*3, coordsBuffer);
	}
	else
	{
		coordsBuffer[0] = 1.1;
		coordsBuffer[1] = 2.2;
		coordsBuffer[2] = 1.1;
		coordsBuffer[3] = 3.3;
		line1 = gf->CreateLineString(FdoDimensionality_XY, 2*2, coordsBuffer);
	}
    propertyValue = AddNewProperty( propertyValues, L"Geometry");
    FdoPtr<FdoByteArray> byteArray = gf->GetFgf(line1);
    FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
    propertyValue->SetValue(geometryValue);

    FdoPtr<FdoDataValue> dataValue;
    dataValue = FdoDataValue::Create(colorIndex);
    propertyValue = AddNewProperty( propertyValues, L"color");
    propertyValue->SetValue(dataValue);

    dataValue = FdoDataValue::Create(dateTime);
    propertyValue = AddNewProperty( propertyValues, L"datetime");
    propertyValue->SetValue(dataValue);

    FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();

    int numberInsertedObjects = 0;

    while (reader->ReadNext())
        numberInsertedObjects++;
    wprintf(L"    Number of rows entered: %d \n", numberInsertedObjects);

    featureTransaction->Commit();
}

void FdoInsertTest::insertDateVerification (FdoIConnection *connection, int numOfSuccess)
{
    int               numberOfObjects     = 0;
    bool              dateObjectFound     = false,
                      dateTimeObjectFound = false,
					  timeObjectFound = false;
    FdoInt64          currentFid;
    FdoStringP        currentColor,
                      currentObjectData;
    FdoDateTime       currentDateTime;
    FdoPtr<FdoISelect> selectCmd;
    FdoPtr<FdoFilter> filter;
    FdoPtr<FdoIFeatureReader> featureReader;

    try {

        selectCmd = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);
        selectCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
        filter = FdoComparisonCondition::Create(
                   FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"color")),
                   FdoComparisonOperations_GreaterThan, 
                   FdoPtr<FdoDataValue>(FdoDataValue::Create(L"989")));
        selectCmd->SetFilter(filter);

        featureReader = selectCmd->Execute();

        while (featureReader->ReadNext())
        {
            numberOfObjects++;
            currentFid      = featureReader->GetInt64(L"FeatId");
            currentColor    = featureReader->GetString(L"color");
            currentDateTime = featureReader->GetDateTime(L"datetime");

            if (!dateObjectFound)
                dateObjectFound = wcscmp(currentColor, L"991") == 0    &&
                                  currentDateTime.year         == 2003 &&
                                  currentDateTime.month        == 1    &&
                                  currentDateTime.day          == 1    &&
                                  currentDateTime.hour         == 0    &&
                                  currentDateTime.minute       == 0    &&
                                  currentDateTime.seconds      == 0;


            if (!dateTimeObjectFound)
                dateTimeObjectFound = wcscmp(currentColor, L"993") == 0    &&
                                      currentDateTime.year         == 2003 &&
                                      currentDateTime.month        == 1    &&
                                      currentDateTime.day          == 1    &&
                                      currentDateTime.hour         == 1    &&
                                      currentDateTime.minute       == 11   &&
                                      currentDateTime.seconds      == 11;

			if (numOfSuccess == 3) // MySQL server
			{
				if (!timeObjectFound)
					timeObjectFound = wcscmp(currentColor, L"992") == 0    &&
                                      currentDateTime.hour         == 1    &&
                                      currentDateTime.minute       == 11   &&
                                      currentDateTime.seconds      == 0;
			}


            if (currentDateTime.hour == 0)
            {
#ifdef _WIN32
      currentObjectData =
                FdoStringP::Format(L"      (Featid, Color, Date): (%I64d, %ls, %04d-%02d-%02d) \n",
                                   currentFid, (FdoString *)currentColor,
                                   currentDateTime.year, currentDateTime.month, currentDateTime.day);
#else
      currentObjectData =
                FdoStringP::Format(L"      (Featid, Color, Date): (%lld, %ls, %04d-%02d-%02d) \n",
                                   currentFid, (FdoString *)currentColor,
                                   currentDateTime.year, currentDateTime.month, currentDateTime.day);
#endif
            }
            else
            {
#ifdef _WIN32
      currentObjectData =
                FdoStringP::Format(L"      (Featid, Color, Date): (%I64d, %ls, %04d-%02d-%02d %02d:%02d:%02f)\n",
                                   currentFid, (FdoString *)currentColor,
                                   currentDateTime.year, currentDateTime.month, currentDateTime.day,
                                   currentDateTime.hour, currentDateTime.minute, currentDateTime.seconds);
#else
      currentObjectData =
                FdoStringP::Format(L"      (Featid, Color, Date): (%lld, %ls, %04d-%02d-%02d %02d:%02d:%02f)\n",
                                   currentFid, (FdoString *)currentColor,
                                   currentDateTime.year, currentDateTime.month, currentDateTime.day,
                                   currentDateTime.hour, currentDateTime.minute, currentDateTime.seconds);
#endif
            }

            wprintf(currentObjectData);

        }

        if (featureReader != NULL)
            featureReader->Close();

        if ((numberOfObjects != numOfSuccess) ||
            (!dateObjectFound)     ||
            (!dateTimeObjectFound) || ((numOfSuccess == 3) && !timeObjectFound))
        {
            wprintf(L"  > unexpected results \n");
            throw FdoException::Create(
                            L"insertDate execution verification: unexpected results");
        }
        else
            wprintf(L"  > expected results \n");

        return;

    }  //  try ...

    catch ( ... ) {

        if (featureReader != NULL)
            featureReader->Close();
        throw;
    } 
}

void FdoInsertTest::insertDateCleanUp (FdoIConnection *connection)
{
    FdoIDelete        *deleteCmd = NULL;
    FdoPtr<FdoFilter> filter;

    try {

        deleteCmd = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);
        deleteCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
        filter = FdoComparisonCondition::Create(
                   FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"color")),
                   FdoComparisonOperations_GreaterThan, 
                   FdoPtr<FdoDataValue>(FdoDataValue::Create(L"989")));
        deleteCmd->SetFilter(filter);
        deleteCmd->Execute();
        FDO_SAFE_RELEASE(deleteCmd);
        return;

    }  //  try ...

    catch ( ... ) {
        FDO_SAFE_RELEASE(deleteCmd);
        throw;
    } 
}

void FdoInsertTest::smartPointerInsert()
{
try
    {
        wchar_t *data[] = {
            L"121234567",
            L"1234",
            L"aaaaaaaaaabbbbiiiii",
            L"3333"};

        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(mSuffix, false);

        try
        {
            double       coordsBuffer[400];
            int          segCount = 2;
			bool		 supportsZ = (FdoPtr<FdoIGeometryCapabilities>(connection->GetGeometryCapabilities())->GetDimensionalities() & FdoDimensionality_Z);

            FdoPtr<FdoPropertyValue> propertyValue;

            FdoPtr<FdoIInsert>insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
            insertCommand->SetFeatureClassName(L"Acad:AcDb3dPolyline");
            FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();

            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
			FdoPtr<FdoILineString> line1;

			if ( supportsZ ) 
			{
				coordsBuffer[0] = 1.1;
				coordsBuffer[1] = 2.2;
				coordsBuffer[2] = 0.0;
				coordsBuffer[3] = 1.1;
				coordsBuffer[4] = 3.3;
				coordsBuffer[5] = 0.0;
				line1 = gf->CreateLineString(FdoDimensionality_XY|FdoDimensionality_Z, 2*3, coordsBuffer);
			}
			else
			{
				coordsBuffer[0] = 1.1;
				coordsBuffer[1] = 2.2;
				coordsBuffer[2] = 1.1;
				coordsBuffer[3] = 3.3;
				line1 = gf->CreateLineString(FdoDimensionality_XY, 2*2, coordsBuffer);
			}

            propertyValue = AddNewProperty( propertyValues, L"Geometry");
            FdoPtr<FdoByteArray> byteArray = gf->GetFgf(line1);

            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

            FdoPtr<FdoDataValue> dataValue;
            dataValue = FdoDataValue::Create(L"256");
            propertyValue = AddNewProperty( propertyValues, L"color");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(segCount);
            propertyValue = AddNewProperty( propertyValues, L"segcount");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(L"200");
            propertyValue = AddNewProperty( propertyValues, L"layer");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(1);
            propertyValue = AddNewProperty( propertyValues, L"xdata.seq");
            propertyValue->SetValue(dataValue);

            for (int j=0; j<4; j++)
            {
                dataValue = FdoDataValue::Create(data[j]);
                propertyValue = AddNewProperty( propertyValues, L"xdata.DataValue");
                propertyValue->SetValue(dataValue);
                FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();
            }
        }
        catch (...)
        {
            if (connection)
                connection->Close ();
            throw;
        }
        if (connection)
            connection->Close();
    }
    catch (FdoCommandException *ex)
    {
        TestCommonFail(ex);
    }
    catch (FdoException *ex)
    {
        TestCommonFail(ex);
    }
}

void FdoInsertTest::insertEmptyProps()
{
    FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(mSuffix, false);

    try
    {
        FdoPtr<FdoPropertyValue> propertyValue;

        FdoPtr<FdoIInsert>insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
        insertCommand->SetFeatureClassName(L"Acad:AcDb3dPolyline");
        //FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();
        FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();
    }
    catch (FdoCommandException *ex)
    {
        TestCommonFail(ex);
    }
    catch (FdoException *ex)
    {
        TestCommonFail(ex);
    }
    if (connection)
        connection->Close();
}

void FdoInsertTest::insertFlushTest()
{
try
    {
        wchar_t *data[] = {
            L"121234567",
            L"1234",
            L"aaaaaaaaaabbbbiiiii",
            L"3333"};

        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(mSuffix, false);

        try
        {
            double       coordsBuffer[400];
            int          segCount = 99;
			bool		 supportsZ = (FdoPtr<FdoIGeometryCapabilities>(connection->GetGeometryCapabilities())->GetDimensionalities() & FdoDimensionality_Z);

            FdoPtr<FdoPropertyValue> propertyValue;

            FdoPtr<FdoIInsert>insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
            insertCommand->SetFeatureClassName(L"Acad:AcDb3dPolyline");
            FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();

            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

			FdoPtr<FdoILineString> line1;
			if ( supportsZ ) 
			{
				coordsBuffer[0] = 1.1;
				coordsBuffer[1] = 2.2;
				coordsBuffer[2] = 0.0;
				coordsBuffer[3] = 1.1;
				coordsBuffer[4] = 3.3;
				coordsBuffer[5] = 0.0;
				line1 = gf->CreateLineString(FdoDimensionality_XY|FdoDimensionality_Z, 2*3, coordsBuffer);
			}
			else
			{
				coordsBuffer[0] = 1.1;
				coordsBuffer[1] = 2.2;
				coordsBuffer[2] = 1.1;
				coordsBuffer[3] = 3.3;
				line1 = gf->CreateLineString(FdoDimensionality_XY, 2*2, coordsBuffer);
			}

            propertyValue = AddNewProperty( propertyValues, L"Geometry");
            FdoPtr<FdoByteArray> byteArray = gf->GetFgf(line1);

            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

            FdoPtr<FdoDataValue> dataValue;
            dataValue = FdoDataValue::Create(L"256");
            propertyValue = AddNewProperty( propertyValues, L"color");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(segCount);
            propertyValue = AddNewProperty( propertyValues, L"segcount");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(L"200");
            propertyValue = AddNewProperty( propertyValues, L"layer");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(1);
            propertyValue = AddNewProperty( propertyValues, L"xdata.seq");
            propertyValue->SetValue(dataValue);

            FdoPtr<FdoIDelete>delCmd = (FdoIDelete*)connection->CreateCommand( FdoCommandType_Delete );
            delCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
            delCmd->SetFilter(L"segcount = 99");
            delCmd->Execute();

            FdoPtr<FdoITransaction> featureTransaction = connection->BeginTransaction();
            for (int j=0; j<15; j++)
            {
                dataValue = FdoDataValue::Create(data[j%4]);
                propertyValue = AddNewProperty( propertyValues, L"xdata.DataValue");
                propertyValue->SetValue(dataValue);
                FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();
            }


            FdoPtr<FdoISelect>selCmd = (FdoISelect*)connection->CreateCommand( FdoCommandType_Select );
            selCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
            selCmd->SetFilter(L"segcount = 99");
            FdoPtr<FdoIFeatureReader>myReader = selCmd->Execute();
            int count = 0;
            while(myReader->ReadNext())count++;
            if( count != 15 )
            {
                printf("Unexpected number(%d) of returned objects\n", count);
                throw "Unexpected number of returned objects";
            }

            featureTransaction->Commit();
        }
        catch (...)
        {
            if (connection)
                connection->Close ();
            throw;
        }
        // close the connection
        if (connection)
            connection->Close();
    }
    catch (FdoCommandException *ex)
    {
        TestCommonFail(ex);
    }
    catch (FdoException *ex)
    {
        TestCommonFail(ex);
    }
}

void FdoInsertTest::conditionalInsert()
{

bool ltCreated = false;
FdoPtr<FdoIConnection> connection;
FdoStringP insLtName;

try
    {

        wchar_t *data[] = {
            L"121234567",
            L"1234", 
            L"aaaaaaaaaabbbbiiiii", 
            L"3333"};

        connection = UnitTestUtil::GetConnection(L"LT", true, true, Connection_WithDatastore, 1, true );

        // Setup the long transaction environment required for this test.
        // NOTE: The long transaction names are prefixed with the current user
        //       name to ensure there is no overlap when multiple users execute
        //       the test as this would result in long transactionaccessibility
        //       errors.

        static char connUser[1024];
        unsigned int   size = 1024;

        UnitTestUtil::GetRealUserName(connUser, size);

        wchar_t connUserW[1024];
        mbstowcs(connUserW, connUser, 1024);
        insLtName = FdoStringP::Format(L"%ls_InsTestLt", connUserW);

        FdoPtr<FdoICreateLongTransaction>create_lt =(FdoICreateLongTransaction *) connection->CreateCommand(
											   FdoCommandType_CreateLongTransaction);
		create_lt->SetName((FdoString *)insLtName);
		create_lt->Execute();
        ltCreated = true;
			
		FdoPtr<FdoIActivateLongTransaction>activate_lt =(FdoIActivateLongTransaction *) connection->CreateCommand(
												FdoCommandType_ActivateLongTransaction );
		activate_lt->SetName((FdoString *)insLtName);
		activate_lt->Execute();

        // Execute the test.

        double       coordsBuffer[400];
        int          segCount = 99;
		bool		 supportsZ = (FdoPtr<FdoIGeometryCapabilities>(connection->GetGeometryCapabilities())->GetDimensionalities() & FdoDimensionality_Z);

		// Insert new features. Each feature should insert a sentinel row.
        FdoPtr<FdoPropertyValue> propertyValue;

        FdoPtr<FdoIInsert>insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
        insertCommand->SetFeatureClassName(L"Acad:AcDb3dPolyline");
        FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();

        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
		FdoPtr<FdoILineString> line1;
		if ( supportsZ ) 
		{
			coordsBuffer[0] = 1.1;
			coordsBuffer[1] = 2.2;
			coordsBuffer[2] = 0.0;
			coordsBuffer[3] = 1.1;
			coordsBuffer[4] = 3.3;
			coordsBuffer[5] = 0.0;
			line1 = gf->CreateLineString(FdoDimensionality_XY|FdoDimensionality_Z, 2*3, coordsBuffer);
		}
		else
		{
			coordsBuffer[0] = 1.1;
			coordsBuffer[1] = 2.2;
			coordsBuffer[2] = 1.1;
			coordsBuffer[3] = 3.3;
			line1 = gf->CreateLineString(FdoDimensionality_XY, 2*2, coordsBuffer);
		}

        propertyValue = AddNewProperty( propertyValues, L"Geometry");
        FdoPtr<FdoByteArray> byteArray = gf->GetFgf(line1);

        FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
        propertyValue->SetValue(geometryValue);

        FdoPtr<FdoDataValue> dataValue;
        dataValue = FdoDataValue::Create(L"256");
        propertyValue = AddNewProperty( propertyValues, L"color");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(segCount);
        propertyValue = AddNewProperty( propertyValues, L"segcount");
        propertyValue->SetValue(dataValue);

#if 1
        dataValue = FdoDataValue::Create(1);
        propertyValue = AddNewProperty( propertyValues, L"xdata.seq");
        propertyValue->SetValue(dataValue);
#endif
        FdoPtr<FdoITransaction> featureTransaction = connection->BeginTransaction();
        for (int j=0; j<4; j++)
        {
        	if(j%2)
		    	dataValue = FdoDataValue::Create(L"200");
			else
				dataValue = FdoDataValue::Create(L"300");
			propertyValue = AddNewProperty( propertyValues, L"layer");
			propertyValue->SetValue(dataValue);
#if 1
            dataValue = FdoDataValue::Create(data[j%4]);
            propertyValue = AddNewProperty( propertyValues, L"xdata.DataValue");
            propertyValue->SetValue(dataValue);
#endif
            FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();
        }

        featureTransaction->Commit();

		//check the data we just inserted
		FdoPtr<FdoISQLCommand>selCmd = (FdoISQLCommand*)connection->CreateCommand( FdoCommandType_SQLCommand );
        selCmd->SetSQLStatement( L"select * from ACDB3DPOLYLINE" );
        FdoPtr<FdoISQLDataReader>myReader = selCmd->ExecuteReader();
		int  nnull_count = 0; // data rows
		int  null_count = 0;  // sentinel rows
        if( myReader != NULL  )
        {
            while ( myReader->ReadNext() )
            {
                if( myReader->IsNull(L"segcount") )
		    		null_count++;
			    else
					nnull_count++;
            }
        }
		CPPUNIT_ASSERT_MESSAGE("Wrong number of null objects", null_count == 4);
		CPPUNIT_ASSERT_MESSAGE("Wrong number of not null objects", nnull_count == 4);

		selCmd->SetSQLStatement( L"select * from ACDBENTITY_ACXDATA" );
        myReader = selCmd->ExecuteReader();
		int  count = 0;
        if( myReader != NULL  )
            while ( myReader->ReadNext() ) count++;
		    CPPUNIT_ASSERT_MESSAGE("Wrong number of object properties", count == 8);

        // Remove the long transaction environment to not discover issues the test
        // is executed next time.

    	FdoPtr<FdoIRollbackLongTransaction>rollback_lt =(FdoIRollbackLongTransaction *) connection->CreateCommand(
											FdoCommandType_RollbackLongTransaction);
		rollback_lt->SetName((FdoString *)insLtName);
		rollback_lt->Execute();
        ltCreated = false;
    }
    catch (FdoCommandException *ex)
    {
        if (ltCreated)
        {
    	    FdoPtr<FdoIRollbackLongTransaction>rollback_lt =(FdoIRollbackLongTransaction *) connection->CreateCommand(
											                                                FdoCommandType_RollbackLongTransaction);
		    rollback_lt->SetName((FdoString *)insLtName);
		    rollback_lt->Execute();
            ltCreated = false;
        }
        connection->Close ();
        TestCommonFail(ex);
    }
    catch (FdoException *ex)
    {
        if (ltCreated)
        {
    	    FdoPtr<FdoIRollbackLongTransaction>rollback_lt =(FdoIRollbackLongTransaction *) connection->CreateCommand(
											                                                FdoCommandType_RollbackLongTransaction);
		    rollback_lt->SetName((FdoString *)insLtName);
		    rollback_lt->Execute();
            ltCreated = false;
        }
        connection->Close ();
        TestCommonFail(ex);
    }
    if (connection)
        connection->Close ();
}

FdoByte FdoInsertTest::GetMinByteValue()
{
    return (FdoByte) 0;
}

FdoInt16 FdoInsertTest::GetMinInt16Value()
{
    return (FdoInt16) SHRT_MIN;
}

FdoInt32 FdoInsertTest::GetMinInt32Value()
{
    return (FdoInt32) LONG_MIN;
}

FdoInt64 FdoInsertTest::GetMinInt64Value()
{
    return (FdoInt64) LLONG_MIN;
}

FdoFloat FdoInsertTest::GetMinSingleValue()
{
    return (FdoFloat) -3.4028233e38;
}

FdoDouble FdoInsertTest::GetMinDoubleValue()
{
    return (FdoDouble) -1.797693134862315e308;
}

FdoByte FdoInsertTest::GetMaxByteValue()
{
    return (FdoByte) UCHAR_MAX;
}

FdoInt16 FdoInsertTest::GetMaxInt16Value()
{
    return (FdoInt16) SHRT_MAX;
}

FdoInt32 FdoInsertTest::GetMaxInt32Value()
{
    return (FdoInt32) LONG_MAX;
}

FdoInt64 FdoInsertTest::GetMaxInt64Value()
{
    return (FdoInt64) LLONG_MAX;
}

FdoFloat FdoInsertTest::GetMaxSingleValue()
{
    return (FdoFloat) 3.4028233e38;
}

FdoDouble FdoInsertTest::GetMaxDoubleValue()
{
    return (FdoDouble) 1.797693134862315e308;
}

FdoFloat FdoInsertTest::GetSmallestSingleValue()
{
    return (FdoFloat) 1.175495e-38;
}

FdoDouble FdoInsertTest::GetSmallestDoubleValue()
{
    return (FdoDouble) 2.225073858507202e-308;
}

void FdoInsertTest::insertAutoGen()
{
    FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(mSuffix, true);
    try
    {
	    FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	    FdoFeatureSchemasP pSchemas = pDescCmd->Execute();
	    if (pSchemas)
	    {
		    FdoFeatureSchemaP pSchema = pSchemas->FindItem( L"TestEmpty" );
		    if (!pSchema)
		    {
			    FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
			    //FdoPtr<FdoFeatureSchemaCollection> pSchemas = FdoFeatureSchemaCollection::Create(NULL);

			    FdoPtr<FdoFeatureSchema> pNewSchema = FdoFeatureSchema::Create( L"TestEmpty", L"Test schema" );
			    //pSchemas->Add( pSchema );
			    FdoPtr<FdoClass> pNewClass = FdoClass::Create( L"Empty", L"Test class" );
			    pNewClass->SetIsAbstract(false);
			    FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"AttrId", L"id" );
			    pProp->SetDataType( FdoDataType_Int64 );
			    pProp->SetNullable(false);
			    pProp->SetIsAutoGenerated(true);
                FdoPtr<FdoPropertyDefinitionCollection> pProps = pNewClass->GetProperties();
			    pProps->Add( pProp );
                FdoPtr<FdoDataPropertyDefinitionCollection> pIdProps = pNewClass->GetIdentityProperties();
			    pIdProps->Add( pProp );

			    pProp = FdoDataPropertyDefinition::Create( L"AttrName", L"Name" );
			    pProp->SetDataType( FdoDataType_String );
			    pProp->SetLength(64);
			    pProp->SetNullable(true);
			    pProps->Add( pProp );

                FdoPtr<FdoClassCollection> pClassColl = pNewSchema->GetClasses();
			    pClassColl->Add( pNewClass );
			    pCmd->SetFeatureSchema( pNewSchema );
			    pCmd->Execute();
		    }
	    }

	    FdoPtr<FdoIInsert> insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
	    insertCommand->SetFeatureClassName(L"TestEmpty:Empty");
	    FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();
	    FdoPtr<FdoIFeatureReader> myReader = insertCommand->Execute();
	    CPPUNIT_ASSERT(myReader != NULL);
		
	    while (myReader->ReadNext())
	    { 
		    CPPUNIT_ASSERT(!myReader->IsNull(L"AttrId"));
	    }
	    myReader->Close();
	    connection->Close();
    }
    catch (FdoException* e)
    {
        connection->Close ();
        TestCommonFail(e);
    }
}

void FdoInsertTest::insertLongString()
{
	try
    {

        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(mSuffix, true);

        try
        {
			//delete row
			FdoPtr<FdoISQLCommand> delCmd = (FdoISQLCommand*)connection->CreateCommand( FdoCommandType_SQLCommand );
            delCmd->SetSQLStatement(L"delete from ACDBENTITY_ACXDATA_ACXOBJ where intdata=200");
			delCmd->ExecuteNonQuery();

			FdoIInsert *insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
            insertCommand->SetFeatureClassName(L"Acad:AcDb3dPolyline");
	        FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();

			FdoPtr<FdoDataValue> dataValue;
	        FdoPtr<FdoPropertyValue> propertyValue;

			dataValue = FdoDataValue::Create(L"256");
	        propertyValue = AddNewProperty( propertyValues,L"color"); 
	        propertyValue->SetValue(dataValue);

	        dataValue = FdoDataValue::Create(0);
	        propertyValue = AddNewProperty( propertyValues, L"segcount");
	        propertyValue->SetValue(dataValue);
		
	        dataValue = FdoDataValue::Create(200);
	        propertyValue = AddNewProperty( propertyValues, L"layer");
	        propertyValue->SetValue(dataValue);

	        dataValue = FdoDataValue::Create(1);
	        propertyValue = AddNewProperty( propertyValues, L"xdata.seq");
	        propertyValue->SetValue(dataValue);

	        dataValue = FdoDataValue::Create(L"xyz");
	        propertyValue = AddNewProperty( propertyValues, L"xdata.DataValue");
	        propertyValue->SetValue(dataValue);
                            
            dataValue = FdoDataValue::Create(200);
	        propertyValue = AddNewProperty( propertyValues, L"xdata.AcXObj.intdata");
	        propertyValue->SetValue(dataValue);
			// Generate very long string
			FdoStringP strData = L"abcdefghi";
			for (int i=1; i<400; i++)
				strData += L"abcdefghij";
			strData += L"X";
            dataValue = FdoDataValue::Create((FdoString *) strData);
	        propertyValue = AddNewProperty( propertyValues, L"xdata.AcXObj.strdata");
	        propertyValue->SetValue(dataValue);

			FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();
			CPPUNIT_ASSERT(reader != NULL);
			reader->Close();
			FdoISelect *selCmd;
			selCmd = (FdoISelect*)connection->CreateCommand( FdoCommandType_Select );
            selCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline.xdata.AcXObj");
            FdoComparisonCondition* filterPtr = FdoComparisonCondition::Create(
						FdoPtr<FdoIdentifier> (FdoIdentifier::Create(L"intdata")), 
						FdoComparisonOperations_EqualTo, 
						FdoPtr<FdoDataValue>(FdoDataValue::Create((int)200)));
			selCmd->SetFilter(filterPtr);
            filterPtr->Release();
            reader = selCmd->Execute();
			CPPUNIT_ASSERT(reader != NULL);
			while (reader->ReadNext())
			{
				if (!reader->IsNull(L"strdata"))
				{
					FdoStringP strSel = reader->GetString(L"strdata");
					CPPUNIT_ASSERT(strSel.ICompare(strData) == 0);
				}
			}
			reader->Close();
			connection->Close();
		}
		catch (...)
        {
            if (connection)
                connection->Close ();
            throw;
        }
    }
    catch (FdoCommandException *ex)
    {
        UnitTestUtil::FailOnException(ex);
    }
    catch (FdoException *ex)
    {
        UnitTestUtil::FailOnException(ex);
    }
}

void FdoInsertTest::featureReaderTest()
{

    bool                                featureReaderError  = false;
    FdoIConnection                      *connection         = NULL;
	FdoIApplySchema                     *applySchemaCmd     = NULL;
    FdoPtr<FdoFeatureSchemaCollection>  schemas;
	FdoFeatureSchema                    *schema             = NULL;
    FdoClass                            *schemaClass        = NULL;
    FdoFeatureClass                     *schemaFeatureClass = NULL;
    FdoClassCollection                  *classes            = NULL;
    FdoIFeatureReader                   *featureReader      = NULL;

    try
    {
        printf(" >>> Feature Reader Test \n");

        // Create the data store used for the test. Note that none of the
        // standard schemas is created.
        printf(" >>> ... creating test database \n");
        if (UnitTestUtil::DatastoreExists(L"_frc"))
            UnitTestUtil::DropDb(L"_frc");
        UnitTestUtil::CreateDB(false, false, L"_frc");

        // Connect and create the test schema.
        connection = UnitTestUtil::GetConnection(L"_frc");

        printf(" >>> ... creating test schema FeatureReaderCheck \n");
        applySchemaCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
        schemas = FdoFeatureSchemaCollection::Create(NULL);
	    schema  = FdoFeatureSchema::Create(L"FeatureReaderCheck", L"Feature Reader Check Schema");
        classes = schema->GetClasses();

        // Add a class with the feature id property as its sole identity property. In this case
        // use the identifier FeatId for the feature id property.
        printf(" >>> ...... adding class FRC_SSFIDC \n");
        schemaFeatureClass = CreateFdoFeatureClass(L"FRC_SSFIDC", L"FeatId", NULL, 1);
        classes->Add(schemaFeatureClass);
        FDO_SAFE_RELEASE(schemaFeatureClass);

        // Add a class with the feature id property as its sole identity property. In this case
        // use the identifier FeatureId for the feature id property.
        printf(" >>> ...... adding class FRC_SCFIDC \n");
        schemaFeatureClass = CreateFdoFeatureClass(L"FRC_SCFIDC", L"FeatureId", NULL, 1);
        classes->Add(schemaFeatureClass);
        FDO_SAFE_RELEASE(schemaFeatureClass);

        // Add a class with two identity properties. The identity properties do not
        // include the feature id property.
        printf(" >>> ...... adding class FRC_MIDC \n");
        schemaFeatureClass = CreateFdoFeatureClass(L"FRC_MIDC", L"Id1", L"Id2", 2);
        classes->Add(schemaFeatureClass);
        FDO_SAFE_RELEASE(schemaFeatureClass);

        // Add a class with two identity properties. The identity properties do include
        // the feature id property.
        printf(" >>> ...... adding class FRC_MIDWFIDC \n");
        schemaFeatureClass =  CreateFdoFeatureClass(L"FRC_MIDWFIDC", L"Id1", L"FeatureId", 3);
        classes->Add(schemaFeatureClass);
        FDO_SAFE_RELEASE(schemaFeatureClass);

        // Add a non-feature class.
        printf(" >>> ...... adding class FRC_NFC \n");
        schemaClass = CreateFdoClass(L"FRC_NFC");
        classes->Add(schemaClass);
        FDO_SAFE_RELEASE(schemaClass);

        schemas->Add(schema);
        applySchemaCmd->SetFeatureSchema(schema);
        printf(" >>> ...... applying schema \n");
        applySchemaCmd->Execute();

        FDO_SAFE_RELEASE(classes);
        FDO_SAFE_RELEASE(schema);
        FDO_SAFE_RELEASE(applySchemaCmd);


        printf(" >>> ... insert data and check the returned feature reader \n");
        printf(" >>> ...... processing class FRC_SSFIDC \n");
        featureReader = AddFeature(connection, L"FRC_SSFIDC", true, 1);
        featureReader->ReadNext();
        try
        {
            FdoInt64 fid = featureReader->GetInt64(L"FeatId");
        }
        catch (...)
        {
            printf(" >>> ......... Feature Reader Error for class FRC_SSFIDC \n");
            featureReaderError = true;
        }
        FDO_SAFE_RELEASE(featureReader);

        printf(" >>> ...... processing class FRC_SCFIDC \n");
        featureReader = AddFeature(connection, L"FRC_SCFIDC", true, 1);
        featureReader->ReadNext();
        try
        {
            FdoInt64 fid = featureReader->GetInt64(L"FeatureId");
        }
        catch (...)
        {
            printf(" >>> ......... Feature Reader Error for class FRC_SCFIDC \n");
            featureReaderError = true;
        }
        FDO_SAFE_RELEASE(featureReader);

        printf(" >>> ...... processing class FRC_MIDC \n");
        featureReader = AddFeature(connection, L"FRC_MIDC", true, 2);
        featureReader->ReadNext();
//Remove this ifdef when defect 918057 fixed
#ifdef RDBI_DEF_SA_ORA
        try
        {
            FdoStringP id1str = featureReader->GetString(L"Id1");
            FdoStringP id2str = featureReader->GetString(L"Id2");
        }
        catch (...)
        {
            printf(" >>> ......... Feature Reader Error for class FRC_MIDC \n");
            featureReaderError = true;
        }
#endif
        FDO_SAFE_RELEASE(featureReader);

        printf(" >>> ...... processing class FRC_MIDWFIDC \n");
        featureReader = AddFeature(connection, L"FRC_MIDWFIDC", true, 3);
        featureReader->ReadNext();
//Remove this ifdef when defect 918057 fixed
#ifdef RDBI_DEF_SA_ORA
        try
        {
            FdoStringP id1str = featureReader->GetString(L"Id1");
            FdoInt64 fid      = featureReader->GetInt64(L"FeatureId");
        }
        catch (...)
        {
            printf(" >>> ......... Feature Reader Error for class FRC_MIDWFIDC \n");
            featureReaderError = true;
        }
#endif
        FDO_SAFE_RELEASE(featureReader);

        printf(" >>> ...... processing class FRC_NFC \n");
        featureReader = AddFeature(connection, L"FRC_NFC", false, 0);
        featureReader->ReadNext();
//Remove this ifdef when defect 918057 fixed
#ifdef RDBI_DEF_SA_ORA
        try
        {
            FdoInt32 fid = featureReader->GetInt32(L"xid");
        }
        catch (...)
        {
            printf(" >>> ......... Feature Reader Error for class FRC_NFC \n");
            featureReaderError = true;
        }
#endif
        FDO_SAFE_RELEASE(featureReader);

		connection->Close();
        if (featureReaderError)
            throw FdoException::Create(L"Feature Reader Test Failed");
        printf(" >>> Feature Reader Test Succeeded\n");
	}

    catch (FdoException *exp)
    {
        printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
        if ( connection ) 
            connection->Close ();
        FDO_SAFE_RELEASE(classes);
        FDO_SAFE_RELEASE(schema);
        FDO_SAFE_RELEASE(applySchemaCmd);
        TestCommonFail(exp);
    }

	catch (...)
    {
        if ( connection ) connection->Close ();
        FDO_SAFE_RELEASE(classes);
        FDO_SAFE_RELEASE(schema);
        FDO_SAFE_RELEASE(applySchemaCmd);
        throw;
    }
}

void FdoInsertTest::insertNonExistProp()
{
    FdoPtr<FdoIConnection> connection;
    bool         succeeded = false;

    try
    {
        connection = UnitTestUtil::GetConnection(mSuffix, true);

        try
        {
            int          segCount = 2;
            double       coordsBuffer[6];

            bool supportsZ = (FdoPtr<FdoIGeometryCapabilities>(connection->GetGeometryCapabilities())->GetDimensionalities() & FdoDimensionality_Z);

            FdoPtr<FdoIInsert> insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
            insertCommand->SetFeatureClassName(L"Acad:AcDb3dPolyline");
            FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();

			printf("start insert feature class\n");
            FdoPtr<FdoDataValue> dataValue;
            FdoPtr<FdoPropertyValue> propertyValue;
            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

            propertyValue = AddNewProperty( propertyValues, L"GEOMETRY" );
            FdoPtr<FdoILineString> line1;

            coordsBuffer[0] = 0;
            coordsBuffer[1] = 0;
            coordsBuffer[2] = 0;
            coordsBuffer[3] = 1;
            coordsBuffer[4] = 1;
            coordsBuffer[5] = 0;

            if( supportsZ )
                line1 = gf->CreateLineString(FdoDimensionality_XY|FdoDimensionality_Z, segCount * 3, coordsBuffer);
            else
                line1 = gf->CreateLineString(FdoDimensionality_XY, segCount * 2, coordsBuffer);

            FdoPtr<FdoByteArray> byteArray = gf->GetFgf(line1);

            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

            dataValue = FdoDataValue::Create(L"256");
            propertyValue = AddNewProperty( propertyValues,L"color"); //AddNewProperty( propertyValues, L"color");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(segCount);
            propertyValue = AddNewProperty( propertyValues, L"segcount");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(L"10");
            propertyValue = AddNewProperty( propertyValues, L"layer");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(1);
 
            FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();

            succeeded = true;
        }
        catch (FdoException *ex)
        {
            // Use wcsncpy to ignore trailing blank on message when originates from message
            // catalogue.
            CPPUNIT_ASSERT( wcsncmp(ex->GetExceptionMessage(), L"Property 'GEOMETRY' not found", 29) == 0 );
            ex->Release();
        }
        catch (...)
        {
            if (connection)
                connection->Close ();
            throw;
        }
        if (connection)
            connection->Close();
    }
    catch (FdoException *ex)
    {
        if (m_DisableFailures)
            throw ex;
        else
            TestCommonFail(ex);
    }

    CPPUNIT_ASSERT_MESSAGE("insert should have failed", !succeeded);
}

FdoIFeatureReader *FdoInsertTest::AddFeature (FdoIConnection *connection, FdoString *className, bool isSpatial, int idScenario)
{
    double                     coordinateBuffer[5];
    FdoIInsert                 *insertCommand      = NULL;
	FdoDataValue               *dataValue          = NULL;
    FdoByteArray               *byteArray          = NULL;
    FdoILineString             *lineStr            = NULL;
    FdoGeometryValue           *geometryValue      = NULL;
	FdoPropertyValue           *propertyValue      = NULL;
    FdoIFeatureReader          *featureReader      = NULL;
    FdoFgfGeometryFactory      *geometryFactory    = NULL;
    FdoPropertyValueCollection *propertyValues     = NULL;

    try
    {
        insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
        insertCommand->SetFeatureClassName(className);
	    propertyValues = insertCommand->GetPropertyValues();

        if (isSpatial)
        {
            coordinateBuffer[0] = 100.0;
            coordinateBuffer[1] = 100.0;
            coordinateBuffer[2] = 101.0;
            coordinateBuffer[3] = 101.0;

            geometryFactory = FdoFgfGeometryFactory::GetInstance();
            lineStr         = geometryFactory->CreateLineString(
                                    FdoDimensionality_XY,
                                    4, 
                                    coordinateBuffer);
            byteArray       = geometryFactory->GetFgf(lineStr);
            geometryValue   = FdoGeometryValue::Create(byteArray);

            propertyValue = AddNewProperty(propertyValues, L"RDBMS_GEOM");
            propertyValue->SetValue(geometryValue);
            FDO_SAFE_RELEASE(geometryValue);
            FDO_SAFE_RELEASE(lineStr);
            FDO_SAFE_RELEASE(byteArray);
            FDO_SAFE_RELEASE(geometryValue);
            FDO_SAFE_RELEASE(propertyValue);
        }
        else
        {
            dataValue     = FdoDataValue::Create(L"1001");
            propertyValue = AddNewProperty(propertyValues, L"xid");
            propertyValue->SetValue(dataValue);
            FDO_SAFE_RELEASE(propertyValue);
            FDO_SAFE_RELEASE(dataValue);
        }

        switch (idScenario)
        {
            case 1:  // Single identity property representing the feature id property.
                //Nothing to do.
                break;

            case 2: // Multiple identity properties; no feature id property included.
                dataValue     = FdoDataValue::Create(L"1001");
                propertyValue = AddNewProperty(propertyValues, L"Id1");
                propertyValue->SetValue(dataValue);
                FDO_SAFE_RELEASE(dataValue);
                FDO_SAFE_RELEASE(propertyValue);

                dataValue     = FdoDataValue::Create(L"1001");
                propertyValue = AddNewProperty(propertyValues, L"Id2");
                propertyValue->SetValue(dataValue);
                FDO_SAFE_RELEASE(dataValue);
                FDO_SAFE_RELEASE(propertyValue);
                break;

            case 3: // Multiple identity properties; feature id property included.
                dataValue     = FdoDataValue::Create(L"1001");
                propertyValue = AddNewProperty(propertyValues, L"Id1");
                propertyValue->SetValue(dataValue);
                FDO_SAFE_RELEASE(dataValue);
                FDO_SAFE_RELEASE(propertyValue);
                break;
        }

        dataValue     = FdoDataValue::Create(L"Blue");
        propertyValue = AddNewProperty(propertyValues, L"color");
        propertyValue->SetValue(dataValue);
        FDO_SAFE_RELEASE(dataValue);
        FDO_SAFE_RELEASE(propertyValue);

        featureReader = insertCommand->Execute();

        FDO_SAFE_RELEASE(propertyValues);
        FDO_SAFE_RELEASE(insertCommand);

        return featureReader;
    
    }  //  try ...

    catch ( ... )
    {
        FDO_SAFE_RELEASE(propertyValue);
        FDO_SAFE_RELEASE(geometryValue);
        FDO_SAFE_RELEASE(lineStr);
        FDO_SAFE_RELEASE(byteArray);
        FDO_SAFE_RELEASE(geometryValue);
        FDO_SAFE_RELEASE(dataValue);
        FDO_SAFE_RELEASE(propertyValues);
        FDO_SAFE_RELEASE(insertCommand);
        throw;
    }
}

FdoDataPropertyDefinition *FdoInsertTest::CreateDataProperty (FdoString   *propertyName,
                                                           FdoDataType dataType,
                                                           FdoInt32    dataSize,
                                                           bool        isNullable)
{
    FdoDataPropertyDefinition *dataPropertyDefinition = NULL;

	dataPropertyDefinition = FdoDataPropertyDefinition::Create(propertyName, propertyName);
	dataPropertyDefinition->SetDataType(dataType);
    if (dataType == FdoDataType_String)
        dataPropertyDefinition->SetLength(dataSize);
	dataPropertyDefinition->SetNullable(isNullable);
    return dataPropertyDefinition;
}

FdoClass *FdoInsertTest::CreateFdoClass (FdoString *className)
{
    FdoClass                            *theClass                = NULL;
    FdoDataPropertyDefinition           *dataPropertyDefinition  = NULL;
    FdoPropertyDefinitionCollection     *dataPropertyDefinitions = NULL;
    FdoDataPropertyDefinitionCollection *idPropertyDefinitions   = NULL;

    try
    {
        theClass = FdoClass::Create(className, className);
	    theClass->SetIsAbstract(false);
        dataPropertyDefinitions = theClass->GetProperties();
        idPropertyDefinitions   = theClass->GetIdentityProperties();

        dataPropertyDefinition  = CreateDataProperty(L"xid", FdoDataType_Int32, 0, false);
	    dataPropertyDefinitions->Add(dataPropertyDefinition);
        idPropertyDefinitions->Add(dataPropertyDefinition);
	    FDO_SAFE_RELEASE(dataPropertyDefinition);

        dataPropertyDefinition = CreateDataProperty(L"color", FdoDataType_String, 30, false);
	    dataPropertyDefinitions->Add(dataPropertyDefinition);
	    FDO_SAFE_RELEASE(dataPropertyDefinition);

        FDO_SAFE_RELEASE(idPropertyDefinitions);
        FDO_SAFE_RELEASE(dataPropertyDefinitions);

        return theClass;
    }

    catch ( ... )
    {
	    FDO_SAFE_RELEASE(dataPropertyDefinition);
        FDO_SAFE_RELEASE(idPropertyDefinitions);
        FDO_SAFE_RELEASE(dataPropertyDefinitions);

        throw;
    }
}

FdoFeatureClass *FdoInsertTest::CreateFdoFeatureClass (FdoString *className,
                                                    FdoString *idColName1,
                                                    FdoString *idColName2,
                                                    int       idScenario)
{
    FdoFeatureClass                     *theClass                    = NULL;
    FdoDataPropertyDefinition           *dataPropertyDefinition      = NULL;
    FdoGeometricPropertyDefinition      *geometricPropertyDefinition = NULL;
    FdoPropertyDefinitionCollection     *dataPropertyDefinitions     = NULL;
    FdoDataPropertyDefinitionCollection *idPropertyDefinitions       = NULL;

    try
    {
        theClass = FdoFeatureClass::Create(className, className);
	    theClass->SetIsAbstract(false);
        dataPropertyDefinitions = theClass->GetProperties();
        idPropertyDefinitions   = theClass->GetIdentityProperties();

        switch (idScenario)
        {
            case 1: // Single identity property representing the feature id property.
                dataPropertyDefinition = CreateDataProperty(idColName1, FdoDataType_Int64, 0, false);
                dataPropertyDefinition->SetIsAutoGenerated(true);
                dataPropertyDefinitions->Add(dataPropertyDefinition);
                idPropertyDefinitions->Add(dataPropertyDefinition);
                FDO_SAFE_RELEASE(dataPropertyDefinition);
                break;

            case 2: // Multiple identity properties; no feature id property included.
                dataPropertyDefinition = CreateDataProperty(idColName1, FdoDataType_String, 50, false);
                dataPropertyDefinitions->Add(dataPropertyDefinition);
                idPropertyDefinitions->Add(dataPropertyDefinition);
                FDO_SAFE_RELEASE(dataPropertyDefinition);

                dataPropertyDefinition = CreateDataProperty(idColName2, FdoDataType_String, 50, false);
                dataPropertyDefinitions->Add(dataPropertyDefinition);
                idPropertyDefinitions->Add(dataPropertyDefinition);
                FDO_SAFE_RELEASE(dataPropertyDefinition);
                break;

            case 3: // Multiple identity properties; feature id property included.
                dataPropertyDefinition = CreateDataProperty(idColName1, FdoDataType_String, 50, false);
                dataPropertyDefinitions->Add(dataPropertyDefinition);
                idPropertyDefinitions->Add(dataPropertyDefinition);
                FDO_SAFE_RELEASE(dataPropertyDefinition);

                dataPropertyDefinition = CreateDataProperty(idColName2, FdoDataType_Int64, 0, false);
                dataPropertyDefinition->SetIsAutoGenerated(true);
                dataPropertyDefinitions->Add(dataPropertyDefinition);
                idPropertyDefinitions->Add(dataPropertyDefinition);
                FDO_SAFE_RELEASE(dataPropertyDefinition);
                break;
        }

        dataPropertyDefinition = CreateDataProperty(L"color", FdoDataType_String, 30, false);
	    dataPropertyDefinitions->Add(dataPropertyDefinition);
	    FDO_SAFE_RELEASE(dataPropertyDefinition);

        geometricPropertyDefinition = CreateGeometricProperty(L"RDBMS_GEOM");
	    dataPropertyDefinitions->Add(geometricPropertyDefinition);
        theClass->SetGeometryProperty(geometricPropertyDefinition);
	    FDO_SAFE_RELEASE(geometricPropertyDefinition);

        FDO_SAFE_RELEASE(idPropertyDefinitions);
        FDO_SAFE_RELEASE(dataPropertyDefinitions);

        theClass->SetBaseClass(NULL);
        return theClass;

    }  //  try ...

    catch ( ... )
    {
	    FDO_SAFE_RELEASE(geometricPropertyDefinition);
	    FDO_SAFE_RELEASE(dataPropertyDefinition);
        FDO_SAFE_RELEASE(idPropertyDefinitions);
        FDO_SAFE_RELEASE(dataPropertyDefinitions);

        throw;
    }
}

FdoGeometricPropertyDefinition *FdoInsertTest::CreateGeometricProperty (FdoString *propertyName)
{
    FdoGeometricPropertyDefinition *geometricPropertyDefinition = NULL;
	geometricPropertyDefinition = FdoGeometricPropertyDefinition::Create(propertyName, propertyName);
    geometricPropertyDefinition->SetGeometryTypes(FdoGeometricType_Point   | 
                                                  FdoGeometricType_Curve   | 
                                                  FdoGeometricType_Surface | 
                                                  FdoGeometricType_Solid     );
    geometricPropertyDefinition->SetHasElevation(false);
    return geometricPropertyDefinition;
}



