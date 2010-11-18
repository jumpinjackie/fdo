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
#include "FdoUpdateTest.h"
#include "UnitTestUtil.h"
#include "ConnectionUtil.h"
#include "../SchemaMgr/Ph/Mgr.h"
#include "../SchemaMgr/Ph/Owner.h"

#ifndef _WIN32
#include <stdint.h>
#endif

#define NO_META_SUFFIX "_no_meta2"

FdoUpdateTest::FdoUpdateTest(void)
{
	m_hasGeom = m_hasAssoc = true;
    mSuffix[0] = '\0';
}

FdoUpdateTest::FdoUpdateTest(wchar_t *suffix)
{
    m_DisableFailures = false;
	m_hasGeom = m_hasAssoc = true;
    wcsncpy(mSuffix, suffix, 11 );
    mSuffix[11] = '\0';
}

FdoUpdateTest::~FdoUpdateTest(void)
{
}

void FdoUpdateTest::setUp ()
{
    set_provider();
}
typedef struct _updatefeatinfo_ {
    int  featNum;
    int  seq;
    double x;
    double y;
} FeatInfo_def;

typedef struct _updatefeatattr_ {
    int  featNum;
    int  seq;
    char layer[12];
} FeatAttr_def;

static FeatInfo_def  feats[10000];
static FeatAttr_def  attrs[1100];

void FdoUpdateTest::Update ()
{
    MainFdoUpdateTest();
}

void FdoUpdateTest::MainFdoUpdateTest (FdoIConnection* Conn)
{
	clock_t start, finish;
    FdoPtr<FdoITransaction> featureTransaction;
    try
    {
        FdoPtr<FdoIConnection> connection;
        if( Conn == NULL )
            connection = UnitTestUtil::GetConnection(mSuffix, true);
        else
        {
            Conn->AddRef();
            connection = Conn;
        }

        try
        {
            static double       coordsBuffer[400];
            int                 nRetCode = 0;
            int                 i;
            char                line[255];
            int                 coordNum,attrNum,featId,segCount;
            int                 count = 0;
            char                coord_file[256];
            char                attr_file[256];
			int					rows_count = 0;
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
            FdoPtr<FdoFilter>   filter = FdoComparisonCondition::Create(
                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"layer") ),
                FdoComparisonOperations_EqualTo,
                FdoPtr<FdoDataValue>(FdoDataValue::Create(L"200") ) );


			FdoPtr<FdoISelect>selCmd = (FdoISelect*)connection->CreateCommand( FdoCommandType_Select );
            selCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
            selCmd->SetFilter(filter);

			wchar_t* strColor = L"256";
			wchar_t* strColor2 = L"128";
            FdoPtr<FdoIFeatureReader>myReader = selCmd->Execute();
            if( myReader->ReadNext()  )
			{
				if( wcscmp(myReader->GetString(L"color"), strColor) == 0 )
					strColor = strColor2;  // Force an update so that the number of rows is correct
			}
			myReader->Close();

            featureTransaction = connection->BeginTransaction();
            FdoPtr<FdoIUpdate> UpdateCommand = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
            UpdateCommand->SetFeatureClassName(L"Acad:AcDb3dPolyline");
            UpdateCommand->SetFilter( filter );
            FdoPtr<FdoPropertyValueCollection> propertyValues = UpdateCommand->GetPropertyValues();

            int length = 0;
            start = clock();

            FdoPtr<FdoDataValue> dataValue;
            FdoPtr<FdoPropertyValue> propertyValue;
            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
            for (i=0; i<coordNum; i++)
            {
                if (feats[i].featNum != featId)
                {
                    if (length != 0)
                    {
						propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"Geometry");
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
                            dataValue = FdoDataValue::Create(strColor);
                            propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"color");
                            propertyValue->SetValue(dataValue);

                            dataValue = FdoDataValue::Create(segCount);
                            propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"segcount");
                            propertyValue->SetValue(dataValue);

                            FdoStringP  layer( attrs[j].layer );
                            dataValue = FdoDataValue::Create((const wchar_t*)layer);
                            propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"layer");
                            propertyValue->SetValue(dataValue);
                        }
                        else
                        {
                            dataValue = FdoDataValue::Create(strColor);
                            propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"color");
                            propertyValue->SetValue(dataValue);

                            dataValue = FdoDataValue::Create(1);
                            propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"segcount");
                            propertyValue->SetValue(dataValue);

                            dataValue = FdoDataValue::Create(L"New");
                            propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"layer");
                            propertyValue->SetValue(dataValue);
                        }
                        rows_count = UpdateCommand->Execute();

  
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
            // The number of rows varies, depending on the order of tests.
            CPPUNIT_ASSERT_MESSAGE ("Update failed to update 10 records", rows_count>=10);
        }
        catch (...)
        {
            if (connection)
                connection->Close ();
            throw;
        }
    }
    catch (FdoException *ex)
    {
        if (m_DisableFailures)
            throw ex;
        else
            TestCommonFail(ex);
    }
}

void FdoUpdateTest::UpdateObjectProperty()
{
    FdoPtr<FdoIConnection> connection;
    FdoPtr<FdoITransaction> featureTransaction;

    try
    {
        connection = UnitTestUtil::GetConnection(mSuffix, false);
#ifdef _WIN32
        _putenv("GVC_TRACE_FILE=e:\\Danube\\gvc.trace");
#else
        putenv("GVC_TRACE_FILE=e:\\Danube\\gvc.trace");
#endif

        FdoPtr<FdoFilter>   filter = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"seq") ),
            FdoComparisonOperations_LessThan,
            FdoPtr<FdoDataValue>(FdoDataValue::Create(L"32001") ) );

        featureTransaction = connection->BeginTransaction();
        FdoPtr<FdoIUpdate> updateCommand = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
        updateCommand->SetFeatureClassName(L"Acad:AcDbEntity.xdata2");
        updateCommand->SetFilter( filter );

        FdoPtr<FdoPropertyValueCollection> properties = updateCommand->GetPropertyValues();

#if 1
        FdoPtr<FdoPropertyValue> prop;
        prop = FdoUpdateTest::AddNewProperty( properties, L"seq");
        if( prop )
            prop->SetValue( FdoPtr<FdoDataValue>(FdoDataValue::Create( (int)4 )) );

        prop = FdoUpdateTest::AddNewProperty( properties, L"DataValue");
        if( prop )
            prop->SetValue( L"'ABCDE'" );
#endif
        // Add this feature to db
        updateCommand->Execute( );

        featureTransaction->Commit();

        connection->Close ();
    }
    catch (FdoException *ex)
    {
        if( connection )
        {
            connection->Close ();
        }
        TestCommonFail(ex);
    }
    catch (...)
    {
        if (connection)
            connection->Close ();
        CPPUNIT_FAIL ("Update threw an exception");
    }
}



void FdoUpdateTest::UpdateAttributesOnly()
{
#ifdef _WIN32
    _putenv("GVC_TRACE_FILE=e:\\Danube\\gvc.trace");
#else
    putenv("GVC_TRACE_FILE=e:\\Danube\\gvc.trace");
#endif
    FdoPtr<FdoIConnection> connection;
    FdoPtr<FdoITransaction> featureTransaction;

    try
    {
        connection = UnitTestUtil::GetConnection(mSuffix, false);

        FdoPtr<FdoFilter>   filter = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"segcount") ),
            FdoComparisonOperations_EqualTo,
            FdoPtr<FdoDataValue>(FdoDataValue::Create(L"10") ) );

        featureTransaction = connection->BeginTransaction();
        FdoPtr<FdoIUpdate> updateCommand = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
        updateCommand->SetFeatureClassName(L"Acad:AcDb3dPolyline");
        updateCommand->SetFilter( filter );

        FdoPtr<FdoPropertyValueCollection> properties = updateCommand->GetPropertyValues();

        FdoPtr<FdoPropertyValue> prop = FdoUpdateTest::AddNewProperty( properties, L"color");
        if( prop )
            prop->SetValue( L"'0'" );

        prop = FdoUpdateTest::AddNewProperty( properties, L"layer");
        if( prop )
            prop->SetValue( L"'32001'" );
#if 1
        prop = FdoUpdateTest::AddNewProperty( properties, L"xdata.seq");
        if( prop )
            prop->SetValue( FdoPtr<FdoDataValue>(FdoDataValue::Create( (int)4 )) );

        prop = FdoUpdateTest::AddNewProperty( properties, L"xdata.DataValue");
        if( prop )
            prop->SetValue( L"'ABCD'" );
#endif
        // Add this feature to db
        updateCommand->Execute( );

        featureTransaction->Commit();

        connection->Close ();
    }
    catch (FdoException *ex)
    {
        if( connection )
        {
            connection->Close ();
        }
        TestCommonFail(ex);
    }
    catch (...)
    {
        if (connection)
            connection->Close ();
        CPPUNIT_FAIL ("Update threw an exception");
    }
}

void FdoUpdateTest::UpdateNonFeatureClass()
{
    //putenv("GVC_TRACE_FILE=e:\\Danube\\gvc.trace");
    FdoPtr<FdoIConnection> connection;
    FdoPtr<FdoITransaction> featureTransaction;

    try
    {
        connection = UnitTestUtil::GetConnection(mSuffix, false);
        try
        {
        UnitTestUtil::Sql2Db(L"delete from TESTCLASS_OBJECTCLASS where TESTCLASS_FIRSTNAME = 'Firstname'", connection);
        } catch(FdoException *ex){ex->Release();}
        featureTransaction = connection->BeginTransaction();
        FdoPtr<FdoIUpdate> updateCommand = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
        updateCommand->SetFeatureClassName(L"Acad:testClass");
        FdoPtr<FdoFilter> filter = FdoComparisonCondition::Create(
                    FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"FirstName") ),
                    FdoComparisonOperations_EqualTo,
                    FdoPtr<FdoDataValue>(FdoDataValue::Create(L"AnotherFirstName" ) ) );

        updateCommand->SetFilter(filter);

        FdoPtr<FdoPropertyValueCollection> propertyValues = updateCommand->GetPropertyValues();
        FdoPtr<FdoDataValue> dataValue;
        FdoPtr<FdoPropertyValue> propertyValue;

        dataValue = FdoDataValue::Create(5);
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"Age");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(43);
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"Object.ObjectWeight");
        propertyValue->SetValue(dataValue);

        updateCommand->Execute();

        featureTransaction->Commit();

        connection->Close();
    }
    catch (FdoException *ex)
    {
        if( connection )
            connection->Close ();
        TestCommonFail(ex);
    }
    catch (...)
    {
        if (connection)
            connection->Close ();
        throw;
    }

}

void FdoUpdateTest::FdoUpdateTestTypes ()
{
    FdoPtr<FdoIConnection> connection;
    FdoPtr<FdoITransaction> featureTransaction;

    try
    {
#ifdef _WIN32
        _putenv("GVC_TRACE_FILE=e:\\Danube\\gvc.trace");
#else
        putenv("GVC_TRACE_FILE=e:\\Danube\\gvc.trace");
#endif
        connection = UnitTestUtil::GetConnection(mSuffix, false);

        featureTransaction = connection->BeginTransaction();
        FdoPtr<FdoIUpdate> updateCommand = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
        updateCommand->SetFeatureClassName(L"Acad:AcDb3dPolyline");
        FdoPtr<FdoFilter> filter = FdoComparisonCondition::Create(
                    FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"FeatId") ),
                    FdoComparisonOperations_EqualTo,
                    FdoPtr<FdoDataValue>(FdoDataValue::Create((int)1002) ) );

        updateCommand->SetFilter(filter);

        FdoPtr<FdoPropertyValueCollection> propertyValues = updateCommand->GetPropertyValues();
        FdoPtr<FdoDataValue> dataValue;
        FdoPtr<FdoPropertyValue> propertyValue;

        dataValue = FdoDataValue::Create((bool) 1);
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"boolean");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create((FdoByte) 12);
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"byte");
        propertyValue->SetValue(dataValue);

        FdoDateTime dateTime;
        dateTime.day = 31;
        dateTime.month = 10;
        dateTime.year = 2003;
        dateTime.hour = 3;
        dateTime.minute = 2;
        dateTime.seconds = 1;

        dataValue = FdoDataValue::Create(dateTime);
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"datetime");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(12.3, FdoDataType_Decimal);
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"decimal");
        propertyValue->SetValue(dataValue);

#ifdef _WIN32
        dataValue = FdoDataValue::Create((double)((double)_I64_MAX)/2.0, FdoDataType_Double);
#else
        dataValue = FdoDataValue::Create((double)((double)INT64_MAX)/2.0, FdoDataType_Double);
#endif
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"double");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create((float)((float)INT_MAX/2.0));
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"single");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create((FdoInt16) SHRT_MAX);
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"int16");
        propertyValue->SetValue(dataValue);
#ifdef _WIN32
        int i = _I32_MAX;
#else
        int i = INT32_MAX;
#endif
        dataValue = FdoDataValue::Create((FdoInt32) INT_MAX );
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"int32");
        propertyValue->SetValue(dataValue);

#ifdef _WIN32
        dataValue = FdoDataValue::Create((FdoInt64) _I64_MAX );
#else
        dataValue = FdoDataValue::Create((FdoInt64) INT64_MAX );
#endif
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"int64");
        propertyValue->SetValue(dataValue);

        updateCommand->Execute();

        featureTransaction->Commit();

        connection->Close();
    }
    catch (FdoCommandException *ex)
    {
        if( connection )
            connection->Close ();
        TestCommonFail(ex);
    }
    catch (FdoException *ex)
    {
        if( connection )
            connection->Close ();
        TestCommonFail(ex);
    }
    catch (...)
    {
        if( connection )
            connection->Close ();
        throw;
    }
}

void FdoUpdateTest::CreateMultiGeometriesSchema( FdoIConnection* connection)
{
    // Version must be incremented each time the following Land schema is updated.
    // This forces a re-create of the Schema in existing datastores.
    static wchar_t* currVersion = L"1.9";
    FdoPtr<FdoIDescribeSchema> pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);

    FdoPtr<FdoFeatureSchemaCollection> pSchemas = pDescCmd->Execute();

    // Check if Multiple Geometries schema already exists
    FdoPtr<FdoFeatureSchema> pSchema = pSchemas->FindItem( L"MG" );

    // If it does, check if it has the current version. The version is kept in
    // the Schema Attribute dictionary.

    if ( pSchema ) {
        if ( (!FdoSADP(pSchema->GetAttributes())->ContainsAttribute(L"version")) ||
             (wcscmp(FdoSADP(pSchema->GetAttributes())->GetAttributeValue(L"version"), currVersion) != 0) ) {

            // No version or wrong version, destroy and re-create the schema.
            // First, delete any objects, since these will prevent schema destruction.

            FdoClassesP classes = pSchema->GetClasses();

            for ( int idx = 0; idx < classes->GetCount(); idx++ ) {
                FdoClassDefinitionP classDef = classes->GetItem(idx);

                if ( (!classDef->GetIsAbstract()) && 
                     ((FdoDataPropertiesP(classDef->GetIdentityProperties())->GetCount() > 0) ||
                      (FdoPtr<FdoReadOnlyDataPropertyDefinitionCollection>(classDef->GetBaseIdentityProperties())->GetCount() > 0)) ) {
                    FdoPtr<FdoIDelete> deleteCommand = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);
                    deleteCommand->SetFeatureClassName( classDef->GetQualifiedName() );
                    deleteCommand->Execute();
                }
            }

            FdoPtr<FdoIDestroySchema>  pCmd = (FdoIDestroySchema*) connection->CreateCommand(FdoCommandType_DestroySchema);
            pCmd->SetSchemaName(L"MG");
            pCmd->Execute();
            pSchema = NULL;
        }
    }

    if(!pSchema)
    {
        FdoPtr<FdoIApplySchema> pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

        FdoPtr<FdoFeatureSchema> pSchema = FdoFeatureSchema::Create(L"MG", L"Multiple Geometries schema");
        FdoSADP (pSchema->GetAttributes())->Add(L"version", currVersion);

        FdoPtr<FdoFeatureClass> pFeatureClass = FdoFeatureClass::Create(L"FC", L"Feature Class with Multiple Geometries");
        pFeatureClass->SetIsAbstract(false);
        
        FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create(L"FeatId", L"");
        pProp->SetDataType( FdoDataType_Int64);
        pProp->SetNullable(false);
        pProp->SetIsAutoGenerated(true);
        FdoPropertiesP(pFeatureClass->GetProperties())->Add(pProp);
        FdoDataPropertiesP(pFeatureClass->GetIdentityProperties())->Add(pProp);

        pProp = FdoDataPropertyDefinition::Create(L"Name", L"");
        pProp->SetDataType(FdoDataType_String);
        pProp->SetNullable(true);
        pProp->SetLength(20);
        FdoPropertiesP(pFeatureClass->GetProperties())->Add(pProp);

        bool supportsZ = (FdoPtr<FdoIGeometryCapabilities>(connection->GetGeometryCapabilities())->GetDimensionalities() & FdoDimensionality_Z);
        FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = FdoGeometricPropertyDefinition::Create(L"G1", L"");
        pGeomProp->SetGeometryTypes(FdoGeometricType_Point);
        if ( supportsZ ) 
            pGeomProp->SetHasElevation( true );
        FdoPropertiesP(pFeatureClass->GetProperties())->Add(pGeomProp);

        pGeomProp = FdoGeometricPropertyDefinition::Create(L"G2", L"");
        pGeomProp->SetGeometryTypes(FdoGeometricType_Curve);
        if ( supportsZ ) 
            pGeomProp->SetHasElevation( true );
        FdoPropertiesP(pFeatureClass->GetProperties())->Add(pGeomProp);
        
        pFeatureClass->SetGeometryProperty(pGeomProp);

        FdoClassesP(pSchema->GetClasses())->Add(pFeatureClass);
        
        pCmd->SetFeatureSchema(pSchema);
        pCmd->Execute();
    }
}

void FdoUpdateTest::UpdateFeatureWithMultiGeometries()
{
    FdoPtr<FdoIConnection> connection;
    FdoPtr<FdoITransaction> featureTransaction;

    try
    {
        connection = UnitTestUtil::GetConnection(mSuffix, false);
        CreateMultiGeometriesSchema(connection);
        featureTransaction = connection->BeginTransaction();

        // Clear out any data from previous run.
        FdoPtr<FdoIDelete> deleteCommand = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);
        deleteCommand->SetFeatureClassName(FdoStringP(L"FC")); 
        deleteCommand->Execute();
        deleteCommand = NULL;
        
        // Create the test data
        // Create feature0
        FdoPtr<FdoIInsert> insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
        insertCommand->SetFeatureClassName(FdoStringP(L"FC"));
        FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();
        FdoPtr<FdoDataValue> dataValue;
        FdoPtr<FdoPropertyValue> propertyValue;

        dataValue = FdoDataValue::Create(L"feature0");
        propertyValue = FdoUpdateTest::AddNewProperty(propertyValues, L"Name");
        propertyValue->SetValue(dataValue);

        // feature0: Create First Geometry Value
        propertyValue = FdoUpdateTest::AddNewProperty(propertyValues, L"G1");
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        bool supportsZ = (FdoPtr<FdoIGeometryCapabilities>(connection->GetGeometryCapabilities())->GetDimensionalities() & FdoDimensionality_Z);
        double coordsPoint[3];
        coordsPoint[0] = 5;
        coordsPoint[1] = 10;
        coordsPoint[2] = 0; 
        FdoPtr<FdoIPoint> point;
        if ( supportsZ )
			point = gf->CreatePoint(FdoDimensionality_XY|FdoDimensionality_Z, coordsPoint);
		else
			point = gf->CreatePoint(FdoDimensionality_XY, coordsPoint);
        FdoPtr<FdoByteArray> byteArray = gf->GetFgf(point);
        FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
        propertyValue->SetValue(geometryValue);

        // feature0: Create Second Geometry Value
        propertyValue = FdoUpdateTest::AddNewProperty(propertyValues, L"G2");
        double       coordsLine1[6];
        int          segCount = 2;
        if ( supportsZ )
        {
            coordsLine1[0] = 2;
            coordsLine1[1] = 15;
            coordsLine1[2] = 0;
            coordsLine1[3] = 10;
            coordsLine1[4] = 15;
            coordsLine1[5] = 0;
        }
        else
        {
            coordsLine1[0] = 2;
            coordsLine1[1] = 15;
            coordsLine1[2] = 10;
            coordsLine1[3] = 15;
        }       		
        FdoPtr<FdoILineString> line;
		if ( supportsZ )
			line = gf->CreateLineString(FdoDimensionality_XY|FdoDimensionality_Z, segCount*3, coordsLine1);
		else
			line = gf->CreateLineString(FdoDimensionality_XY, segCount*2, coordsLine1);
        byteArray = gf->GetFgf(line);
        geometryValue = FdoGeometryValue::Create(byteArray);
        propertyValue->SetValue(geometryValue);

        FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();

        // Create feature1&feature2, their geometries are the same with feature0 at the beginning.
        dataValue = FdoDataValue::Create(L"feature1");
        propertyValue = propertyValues->FindItem(L"Name");
        propertyValue->SetValue(dataValue);
        reader = insertCommand->Execute();

        dataValue = FdoDataValue::Create(L"feature2");
        propertyValue = propertyValues->FindItem(L"Name");
        propertyValue->SetValue(dataValue);
        reader = insertCommand->Execute();

        insertCommand = NULL;
        featureTransaction->Commit();

        // feature1 does not change
        // feature1 - test changing one geometry property
        FdoPtr<FdoFilter> filter = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Name")),
            FdoComparisonOperations_EqualTo,
            FdoPtr<FdoDataValue>(FdoDataValue::Create(L"feature1"))
            );

        FdoPtr<FdoIUpdate> updateCommand = (FdoIUpdate*) connection->CreateCommand(FdoCommandType_Update);
        updateCommand->SetFeatureClassName(L"FC");
        updateCommand->SetFilter(filter);

        propertyValues=updateCommand->GetPropertyValues();
        
        // feature 1: Recreate First Geometry Value
        propertyValue = FdoUpdateTest::AddNewProperty(propertyValues, L"G1");
        coordsPoint[0] = 10;
        coordsPoint[1] = 10;
        coordsPoint[2] = 0; 
        if ( supportsZ )
			point = gf->CreatePoint(FdoDimensionality_XY|FdoDimensionality_Z, coordsPoint);
		else
			point = gf->CreatePoint(FdoDimensionality_XY, coordsPoint);
        byteArray = gf->GetFgf(point);
        geometryValue = FdoGeometryValue::Create(byteArray);
        propertyValue->SetValue(geometryValue);

        updateCommand->Execute();

        // feature2 - test changing two geometry
        filter = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Name") ),
            FdoComparisonOperations_EqualTo,
            FdoPtr<FdoDataValue>(FdoDataValue::Create(L"feature2") )
        );

        updateCommand = (FdoIUpdate*) connection->CreateCommand(FdoCommandType_Update);
        updateCommand->SetFeatureClassName(L"FC");
        updateCommand->SetFilter(filter);

        propertyValues=updateCommand->GetPropertyValues();
        
        // feature 2: Recreate First Geometry Value
        propertyValue = FdoUpdateTest::AddNewProperty(propertyValues, L"G1");
        coordsPoint[0] = 15;
        coordsPoint[1] = 15;
        coordsPoint[2] = 5; 
        if ( supportsZ )
			point = gf->CreatePoint(FdoDimensionality_XY|FdoDimensionality_Z, coordsPoint);
		else
			point = gf->CreatePoint(FdoDimensionality_XY, coordsPoint);
        byteArray = gf->GetFgf(point);
        geometryValue = FdoGeometryValue::Create(byteArray);
        propertyValue->SetValue(geometryValue);

        // feature 2: Recreate Second Geometry Value
        propertyValue = FdoUpdateTest::AddNewProperty(propertyValues, L"G2");
        double       coordsLine2[6];
        if ( supportsZ )
        {
            coordsLine2[0] = 2;
            coordsLine2[1] = 2;
            coordsLine2[2] = 0;
            coordsLine2[3] = 15;
            coordsLine2[4] = 15;
            coordsLine2[5] = 0;
        }
        else
        {
            coordsLine2[0] = 2;
            coordsLine2[1] = 2;
            coordsLine2[2] = 15;
            coordsLine2[3] = 15;
        }  
		if ( supportsZ )
			line = gf->CreateLineString(FdoDimensionality_XY|FdoDimensionality_Z, segCount*3, coordsLine2);
		else
			line = gf->CreateLineString(FdoDimensionality_XY, segCount*2, coordsLine2);
        byteArray = gf->GetFgf(line);
        geometryValue = FdoGeometryValue::Create(byteArray);
        propertyValue->SetValue(geometryValue);

        updateCommand->Execute();

        featureTransaction->Commit();

        // Select and verify 
        FdoPtr<FdoISelect> selectCommand = (FdoISelect*) connection->CreateCommand(FdoCommandType_Select);
        selectCommand->SetFeatureClassName(L"FC");
        FdoPtr<FdoIFeatureReader> rdr = selectCommand->Execute();

        int rowCount = 0;
        while(rdr->ReadNext())
        {
            FdoString * pName = rdr->GetString(L"Name");
            if(wcscmp(pName,L"feature0") == 0)
            {
                CheckPointGeometry( rdr, L"G1", 5, 10, 0 );
                CheckLineGeometry( rdr, L"G2", coordsLine1);
                rowCount++;
            }
            else if(wcscmp(pName,L"feature1") == 0)
            {
                CheckPointGeometry( rdr, L"G1", 10, 10, 0 );
                CheckLineGeometry( rdr, L"G2", coordsLine1);
                rowCount++;
            }
            else if(wcscmp(pName,L"feature2") == 0)
            {
                CheckPointGeometry( rdr, L"G1", 15, 15, 5 );
                CheckLineGeometry( rdr, L"G2", coordsLine2);
                rowCount++;
            }
        }
        CPPUNIT_ASSERT( rowCount == 3 );
        rdr->Close();
        connection->Close();
    }
    catch (FdoException *ex)
    {
        try {
            if( connection )
                connection->Close ();
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
        }
        catch ( ... )
        {
        }
        throw;
    }
}

void FdoUpdateTest::UpdateMultiIdFeatureClass()
{
    //putenv("GVC_TRACE_FILE=e:\\Danube\\gvc.trace");
    FdoPtr<FdoIConnection> connection;
    FdoPtr<FdoITransaction> featureTransaction;

    try
    {
        connection = UnitTestUtil::GetConnection(this->MultiIdSuffix(), true);
        UnitTestUtil::CreateLandSchema(connection);
        UnitTestUtil::CreateNonUniqueSchema(connection);

        featureTransaction = connection->BeginTransaction();

        // Clear out any data from previous run.

        FdoPtr<FdoIDelete> deleteCommand = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);
        deleteCommand->SetFeatureClassName(FdoStringP(L"L\x00e4nd:") + UnitTestUtil::GetNlsObjectName(L"Industrial Parcel"));
        deleteCommand->Execute();
        deleteCommand = NULL;

        // Create the test data

        FdoPtr<FdoIInsert> insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
        insertCommand->SetFeatureClassName(FdoStringP(L"L\x00e4nd:") + UnitTestUtil::GetNlsObjectName(L"Industrial Parcel"));
        FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();
        FdoPtr<FdoDataValue> dataValue;
        FdoPtr<FdoPropertyValue> propertyValue;

        dataValue = FdoDataValue::Create((FdoString*) UnitTestUtil::GetNlsValue(4));
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"Pr\x00f6vince");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(L"1");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"PIN");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create((double) 5000000, FdoDataType_Decimal);
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"Value");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create((FdoInt32) 200);
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"# employees");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(L"J.R.");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"owner.First N\x00e4me");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(L"Booth");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"owner.Last Name");
        propertyValue->SetValue(dataValue);

        double       coordsBuffer[3];

        coordsBuffer[0] = 5;
        coordsBuffer[1] = 10;
        coordsBuffer[2] = 0;

        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, UnitTestUtil::GetNlsObjectName(L"Geometry"));
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

        bool supportsZ = (FdoPtr<FdoIGeometryCapabilities>(connection->GetGeometryCapabilities())->GetDimensionalities() & FdoDimensionality_Z);
		FdoPtr<FdoIPoint> point1;

		if ( supportsZ )
			point1 = gf->CreatePoint(FdoDimensionality_XY|FdoDimensionality_Z, coordsBuffer);
		else
			point1 = gf->CreatePoint(FdoDimensionality_XY, coordsBuffer);

        FdoPtr<FdoByteArray> byteArray = gf->GetFgf(point1);
        FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
        propertyValue->SetValue(geometryValue);

        FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();
        dataValue = FdoDataValue::Create(L"2");
        propertyValue = propertyValues->GetItem(L"PIN");
        propertyValue->SetValue(dataValue);

        reader = insertCommand->Execute();

        dataValue = FdoDataValue::Create(L"3");
        propertyValue = propertyValues->GetItem(L"PIN");
        propertyValue->SetValue(dataValue);

        reader = insertCommand->Execute();

        dataValue = FdoDataValue::Create(L"4");
        propertyValue = propertyValues->GetItem(L"PIN");
        propertyValue->SetValue(dataValue);

        reader = insertCommand->Execute();

        dataValue = FdoDataValue::Create((FdoString*) UnitTestUtil::GetNlsValue(2));
        propertyValue = propertyValues->GetItem(L"PIN");
        propertyValue->SetValue(dataValue);

        reader = insertCommand->Execute();

        dataValue = FdoDataValue::Create(L"6");
        propertyValue = propertyValues->GetItem(L"PIN");
        propertyValue->SetValue(dataValue);

        reader = insertCommand->Execute();

        dataValue = FdoDataValue::Create(L"7");
        propertyValue = propertyValues->GetItem(L"PIN");
        propertyValue->SetValue(dataValue);

        reader = insertCommand->Execute();

        insertCommand = NULL;
        featureTransaction->Commit();

        // PIN 1 does not change.

        // PIN 2 - test changing a feature class property

        FdoPtr<FdoFilter>   filter = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"PIN") ),
            FdoComparisonOperations_EqualTo,
            FdoPtr<FdoDataValue>(FdoDataValue::Create(L"2") )
        );


        FdoPtr<FdoIUpdate> UpdateCommand = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
        UpdateCommand->SetFeatureClassName(FdoStringP(L"L\x00e4nd:") + UnitTestUtil::GetNlsObjectName(L"Industrial Parcel"));
        UpdateCommand->SetFilter( filter );

        propertyValues = UpdateCommand->GetPropertyValues();

        dataValue = FdoDataValue::Create((FdoInt32) 250 );
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"# employees");
        propertyValue->SetValue(dataValue);

        UpdateCommand->Execute();

        // PIN 3 - test changing a feature class object property

        filter = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"PIN") ),
            FdoComparisonOperations_EqualTo,
            FdoPtr<FdoDataValue>(FdoDataValue::Create(L"3") )
        );
        UpdateCommand = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
        UpdateCommand->SetFeatureClassName(FdoStringP(L"L\x00e4nd:") + UnitTestUtil::GetNlsObjectName(L"Industrial Parcel"));
        UpdateCommand->SetFilter( filter );
        propertyValues = UpdateCommand->GetPropertyValues();

        dataValue = FdoDataValue::Create(L"E.\x00df.");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"owner.First N\x00e4me");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(L"Eddy");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"owner.Last Name");
        propertyValue->SetValue(dataValue);

        UpdateCommand->Execute();

        // PIN 4 - test changing both data property and object property

        filter = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"PIN") ),
            FdoComparisonOperations_EqualTo,
            FdoPtr<FdoDataValue>(FdoDataValue::Create(L"4") )
        );
        UpdateCommand = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
        UpdateCommand->SetFeatureClassName(FdoStringP(L"L\x00e4nd:") + UnitTestUtil::GetNlsObjectName(L"Industrial Parcel"));
        UpdateCommand->SetFilter( filter );
        propertyValues = UpdateCommand->GetPropertyValues();

        dataValue = FdoDataValue::Create((FdoInt32) 250 );
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"# employees");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(L"E.B.");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"owner.First N\x00e4me");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(L"Eddy");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"owner.Last Name");
        propertyValue->SetValue(dataValue);

        UpdateCommand->Execute();

        // PIN 5 - test data, object and geometry property change

        filter = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"PIN") ),
            FdoComparisonOperations_EqualTo,
            FdoPtr<FdoDataValue>(FdoDataValue::Create((FdoString*) UnitTestUtil::GetNlsValue(2)) )
        );
        UpdateCommand = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
        UpdateCommand->SetFeatureClassName(FdoStringP(L"L\x00e4nd:") + UnitTestUtil::GetNlsObjectName(L"Industrial Parcel"));
        UpdateCommand->SetFilter( filter );
        propertyValues = UpdateCommand->GetPropertyValues();

        dataValue = FdoDataValue::Create((FdoInt32) 250 );
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"# employees");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(L"E.B.");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"owner.First N\x00e4me");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(L"Eddy");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"owner.Last Name");
        propertyValue->SetValue(dataValue);

        coordsBuffer[2] = 5;

        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, UnitTestUtil::GetNlsObjectName(L"Geometry"));

		if ( supportsZ )
			point1 = gf->CreatePoint(FdoDimensionality_XY|FdoDimensionality_Z, coordsBuffer);
		else
			point1 = gf->CreatePoint(FdoDimensionality_XY, coordsBuffer);

        byteArray = gf->GetFgf(point1);
        geometryValue = FdoGeometryValue::Create(byteArray);
        propertyValue->SetValue(geometryValue);

        UpdateCommand->Execute();

        UpdateCommand = NULL;

        // PIN 6,7 - test deleting features.

        deleteCommand = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);

        FdoPtr<FdoFilter> filter1 = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"PIN") ),
            FdoComparisonOperations_EqualTo,
            FdoPtr<FdoDataValue>(FdoDataValue::Create(L"6") )
        );

        FdoPtr<FdoFilter> filter2 = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"PIN") ),
            FdoComparisonOperations_EqualTo,
            FdoPtr<FdoDataValue>(FdoDataValue::Create(L"7") )
        );

        filter = FdoFilter::Combine(filter1, FdoBinaryLogicalOperations_Or , filter2 );

        deleteCommand->SetFeatureClassName(FdoStringP(L"L\x00e4nd:") + UnitTestUtil::GetNlsObjectName(L"Industrial Parcel"));
        deleteCommand->SetFilter( filter );

        deleteCommand->Execute();
        deleteCommand = NULL;

        featureTransaction->Commit();

        // Select and verify all data (post-update state).

        FdoPtr<FdoISelect> selectCommand = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);

        selectCommand->SetFeatureClassName( FdoStringP(L"L\x00e4nd:") + UnitTestUtil::GetNlsObjectName(L"Industrial Parcel") );

        FdoPtr<FdoIFeatureReader> rdr = selectCommand->Execute();
        FdoPtr<FdoIdentifierCollection> props = selectCommand->GetPropertyNames();

        int rowCount = 0;

        while ( rdr->ReadNext() )
        {
            const wchar_t* pPin = rdr->GetString(L"PIN");
            CPPUNIT_ASSERT( !rdr->IsNull( L"owner" ) );
            FdoPtr<FdoIFeatureReader> rdr2 = rdr->GetFeatureObject( L"owner" );

            CPPUNIT_ASSERT( rdr2->ReadNext() );

            if ( wcscmp( pPin, L"1" ) == 0 )
            {
                CPPUNIT_ASSERT( rdr->GetInt32(L"Value") == 5000000 );
                CPPUNIT_ASSERT( rdr->GetInt32(L"# employees") == 200 );
                CPPUNIT_ASSERT( wcscmp( rdr->GetString(L"Pr\x00f6vince"), UnitTestUtil::GetNlsValue(4) ) == 0 );
                CPPUNIT_ASSERT( wcscmp( rdr2->GetString(L"First N\x00e4me"), L"J.R." ) == 0 );
                CPPUNIT_ASSERT( wcscmp( rdr2->GetString(L"Last Name"), L"Booth" ) == 0 );
                CPPUNIT_ASSERT( rdr->GetDouble(L"RevisionNumber") == 0 );
                CheckPointGeometry( rdr, UnitTestUtil::GetNlsObjectName(L"Geometry"), 5, 10, 0 );
                rowCount++;
            }

            if ( wcscmp( pPin, L"2" ) == 0 )
            {
                CPPUNIT_ASSERT( rdr->GetInt32(L"Value") == 5000000 );
                CPPUNIT_ASSERT( rdr->GetInt32(L"# employees") == 250 );
                CPPUNIT_ASSERT( wcscmp( rdr2->GetString(L"First N\x00e4me"), L"J.R." ) == 0 );
                CPPUNIT_ASSERT( wcscmp( rdr2->GetString(L"Last Name"), L"Booth" ) == 0 );
                CPPUNIT_ASSERT( rdr->GetDouble(L"RevisionNumber") == 1 );
                CheckPointGeometry( rdr, UnitTestUtil::GetNlsObjectName(L"Geometry"), 5, 10, 0 );
                rowCount++;
            }

            if ( wcscmp( pPin, L"3" ) == 0 )
            {
                CPPUNIT_ASSERT( rdr->GetInt32(L"Value") == 5000000 );
                CPPUNIT_ASSERT( rdr->GetInt32(L"# employees") == 200 );
                CPPUNIT_ASSERT( wcscmp( rdr2->GetString(L"First N\x00e4me"), L"E.\x00df." ) == 0 );
                CPPUNIT_ASSERT( wcscmp( rdr2->GetString(L"Last Name"), L"Eddy" ) == 0 );
                CPPUNIT_ASSERT( rdr->GetDouble(L"RevisionNumber") == 0 );
                CheckPointGeometry( rdr, UnitTestUtil::GetNlsObjectName(L"Geometry"), 5, 10, 0 );
                rowCount++;
            }

            if ( wcscmp( pPin, L"4" ) == 0 )
            {
                CPPUNIT_ASSERT( rdr->GetInt32(L"Value") == 5000000 );
                CPPUNIT_ASSERT( rdr->GetInt32(L"# employees") == 250 );
                CPPUNIT_ASSERT( wcscmp( rdr2->GetString(L"First N\x00e4me"), L"E.B." ) == 0 );
                CPPUNIT_ASSERT( wcscmp( rdr2->GetString(L"Last Name"), L"Eddy" ) == 0 );
                CPPUNIT_ASSERT( rdr->GetDouble(L"RevisionNumber") == 1 );
                CheckPointGeometry( rdr, UnitTestUtil::GetNlsObjectName(L"Geometry"), 5, 10, 0 );
                rowCount++;
            }

            if ( wcscmp( pPin, UnitTestUtil::GetNlsValue(2) ) == 0 )
            {
                CPPUNIT_ASSERT( rdr->GetInt32(L"Value") == 5000000 );
                CPPUNIT_ASSERT( rdr->GetInt32(L"# employees") == 250 );
                CPPUNIT_ASSERT( wcscmp( rdr2->GetString(L"First N\x00e4me"), L"E.B." ) == 0 );
                CPPUNIT_ASSERT( wcscmp( rdr2->GetString(L"Last Name"), L"Eddy" ) == 0 );
                CPPUNIT_ASSERT( rdr->GetDouble(L"RevisionNumber") == 1 );
				CheckPointGeometry( rdr, UnitTestUtil::GetNlsObjectName(L"Geometry"), 5, 10, 5 );
                rowCount++;
            }
        }

        CPPUNIT_ASSERT( rowCount == 5 );

        rdr->Close();

        // Try select with filter

        filter1 = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"PIN") ),
            FdoComparisonOperations_EqualTo,
            FdoPtr<FdoDataValue>(FdoDataValue::Create(L"1") )
        );

        filter2 = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"PIN") ),
            FdoComparisonOperations_EqualTo,
            FdoPtr<FdoDataValue>( FdoDataValue::Create((FdoString*) UnitTestUtil::GetNlsValue(2)) )
        );

        filter = FdoFilter::Combine(filter1, FdoBinaryLogicalOperations_Or , filter2 );

        selectCommand = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);
        selectCommand->SetFeatureClassName( FdoStringP(L"L\x00e4nd:") + UnitTestUtil::GetNlsObjectName(L"Industrial Parcel") );
        selectCommand->SetFilter(filter);

        rdr = selectCommand->Execute();

        rowCount = 0;

        while ( rdr->ReadNext() )
        {
            rowCount++;
        }

        CPPUNIT_ASSERT( rowCount == 2 );

        connection->Close ();
    }
    catch (FdoException *ex)
    {
        try {
            if( connection )
                connection->Close ();
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
        }
        catch ( ... )
        {
        }
        throw;
    }

}

void FdoUpdateTest::UpdateSingleIdFeatureClass()
{
    //putenv("GVC_TRACE_FILE=e:\\Danube\\gvc.trace");
    FdoPtr<FdoIConnection> connection;
    FdoPtr<FdoITransaction> featureTransaction;

    try
    {
        connection = UnitTestUtil::GetConnection(mSuffix, false);
        UnitTestUtil::CreateLandSchema(connection);
        UnitTestUtil::CreateNonUniqueSchema(connection);

        featureTransaction = connection->BeginTransaction();

        // Clear out any data from previous run.

        FdoPtr<FdoIDelete> deleteCommand = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);
        deleteCommand->SetFeatureClassName(L"L\x00e4nd:Municipality");
        deleteCommand->Execute();
        deleteCommand = NULL;

        // Create the test data

        FdoPtr<FdoIInsert> insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
        insertCommand->SetFeatureClassName(L"L\x00e4nd:Municipality");
        FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();
        FdoPtr<FdoDataValue> dataValue;
        FdoPtr<FdoPropertyValue> propertyValue;

        dataValue = FdoDataValue::Create(L"Ottawa");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"Name");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create((FdoInt32)800000);
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"Population");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(L"Mayor");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"mayor.First N\x00e4me");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create((FdoString*) UnitTestUtil::GetNlsValue(3));
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"mayor.Last Name");
        propertyValue->SetValue(dataValue);

        double       coordsBuffer[3];
        int          segCount = 1;

        coordsBuffer[0] = 5;
        coordsBuffer[1] = 10;
        coordsBuffer[2] = 0;

        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"Geometry");
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        bool supportsZ = (FdoPtr<FdoIGeometryCapabilities>(connection->GetGeometryCapabilities())->GetDimensionalities() & FdoDimensionality_Z);

        FdoPtr<FdoILineString> line1;
		
		if ( supportsZ )
			line1 = gf->CreateLineString(FdoDimensionality_XY|FdoDimensionality_Z, segCount*3, coordsBuffer);
		else
			line1 = gf->CreateLineString(FdoDimensionality_XY, segCount*2, coordsBuffer);

        FdoPtr<FdoByteArray> byteArray = gf->GetFgf(line1);
        FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
        propertyValue->SetValue(geometryValue);

        FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();
        dataValue = FdoDataValue::Create(L"Toronto");
        propertyValue = propertyValues->GetItem(L"Name");
        propertyValue->SetValue(dataValue);

        reader = insertCommand->Execute();

        dataValue = FdoDataValue::Create(L"Cornwall");
        propertyValue = propertyValues->GetItem(L"Name");
        propertyValue->SetValue(dataValue);

        reader = insertCommand->Execute();

        dataValue = FdoDataValue::Create(L"Windsor");
        propertyValue = propertyValues->GetItem(L"Name");
        propertyValue->SetValue(dataValue);

        reader = insertCommand->Execute();

        dataValue = FdoDataValue::Create(L"Kingston");
        propertyValue = propertyValues->GetItem(L"Name");
        propertyValue->SetValue(dataValue);

        reader = insertCommand->Execute();

        dataValue = FdoDataValue::Create(L"Sudbury");
        propertyValue = propertyValues->GetItem(L"Name");
        propertyValue->SetValue(dataValue);

        reader = insertCommand->Execute();

        dataValue = FdoDataValue::Create(L"Hamilton");
        propertyValue = propertyValues->GetItem(L"Name");
        propertyValue->SetValue(dataValue);

        reader = insertCommand->Execute();

        insertCommand = NULL;
        featureTransaction->Commit();

        // Ottawa does not change.

        // Toronto - test changing a feature class property

        FdoPtr<FdoFilter>   filter = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Name") ),
            FdoComparisonOperations_EqualTo,
            FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Toronto") )
        );


        FdoPtr<FdoIUpdate> UpdateCommand = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
        UpdateCommand->SetFeatureClassName(L"L\x00e4nd:Municipality");
        UpdateCommand->SetFilter( filter );

        propertyValues = UpdateCommand->GetPropertyValues();

        dataValue = FdoDataValue::Create((FdoInt32) 2500000 );
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"Population");
        propertyValue->SetValue(dataValue);

        UpdateCommand->Execute();

        // Cornwall - test changing a feature class object property

        filter = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Name") ),
            FdoComparisonOperations_EqualTo,
            FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Cornwall") )
        );
        UpdateCommand = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
        UpdateCommand->SetFeatureClassName(L"L\x00e4nd:Municipality");
        UpdateCommand->SetFilter( filter );
        propertyValues = UpdateCommand->GetPropertyValues();

        dataValue = FdoDataValue::Create(L"Old");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"mayor.First N\x00e4me");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(L"Grey");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"mayor.Last Name");
        propertyValue->SetValue(dataValue);

        UpdateCommand->Execute();

        // Windsor - test changing both data property and object property

        filter = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Name") ),
            FdoComparisonOperations_EqualTo,
            FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Windsor") )
        );
        UpdateCommand = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
        UpdateCommand->SetFeatureClassName(L"L\x00e4nd:Municipality");
        UpdateCommand->SetFilter( filter );
        propertyValues = UpdateCommand->GetPropertyValues();

        dataValue = FdoDataValue::Create((FdoInt32) 300000 );
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"Population");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(L"Old");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"mayor.First N\x00e4me");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(L"Grey");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"mayor.Last Name");
        propertyValue->SetValue(dataValue);

        UpdateCommand->Execute();

        // Kingston - test data, object and geometry property change

        filter = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Name") ),
            FdoComparisonOperations_EqualTo,
            FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Kingston") )
        );
        UpdateCommand = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
        UpdateCommand->SetFeatureClassName(L"L\x00e4nd:Municipality");
        UpdateCommand->SetFilter( filter );
        propertyValues = UpdateCommand->GetPropertyValues();

        dataValue = FdoDataValue::Create((FdoInt32) 200000 );
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"Population");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(L"Old");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"mayor.First N\x00e4me");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(L"Grey");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"mayor.Last Name");
        propertyValue->SetValue(dataValue);

        coordsBuffer[2] = 5;

        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"Geometry");
	
		if ( supportsZ )
			line1 = gf->CreateLineString(FdoDimensionality_XY|FdoDimensionality_Z, segCount*3, coordsBuffer);
		else
			line1 = gf->CreateLineString(FdoDimensionality_XY, segCount*2, coordsBuffer);

        byteArray = gf->GetFgf(line1);
        geometryValue = FdoGeometryValue::Create(byteArray);
        propertyValue->SetValue(geometryValue);

        UpdateCommand->Execute();

        UpdateCommand = NULL;

        // Sudbury and Hamilton - test deleting features.
        // My apologies to anyone from either place.

        deleteCommand = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);

        FdoPtr<FdoFilter> filter1 = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Name") ),
            FdoComparisonOperations_EqualTo,
            FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Sudbury") )
        );

        FdoPtr<FdoFilter> filter2 = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Name") ),
            FdoComparisonOperations_EqualTo,
            FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Hamilton") )
        );

        filter = FdoFilter::Combine(filter1, FdoBinaryLogicalOperations_Or , filter2 );

        deleteCommand->SetFeatureClassName(L"L\x00e4nd:Municipality");
        deleteCommand->SetFilter( filter );

        deleteCommand->Execute();
        deleteCommand = NULL;

        featureTransaction->Commit();

        // Select and verify all data (post-update state).

        FdoPtr<FdoISelect> selectCommand = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);

        selectCommand->SetFeatureClassName( L"L\x00e4nd:Municipality" );

        FdoPtr<FdoIFeatureReader> rdr = selectCommand->Execute();
        FdoPtr<FdoIdentifierCollection> props = selectCommand->GetPropertyNames();

        int rowCount = 0;

        while ( rdr->ReadNext() )
        {
            rowCount++;
            const wchar_t* pName = rdr->GetString(L"Name");
            CPPUNIT_ASSERT( !rdr->IsNull( L"mayor" ) );
            FdoPtr<FdoIFeatureReader> rdr2 = rdr->GetFeatureObject( L"mayor" );

            CPPUNIT_ASSERT( rdr2->ReadNext() );

            if ( wcscmp( pName, L"Ottawa" ) == 0 )
            {
                CPPUNIT_ASSERT( rdr->GetInt32(L"Population") == 800000 );
                CPPUNIT_ASSERT( wcscmp( rdr2->GetString(L"First N\x00e4me"), L"Mayor" ) == 0 );
                CPPUNIT_ASSERT( wcscmp( rdr2->GetString(L"Last Name"), UnitTestUtil::GetNlsValue(3) ) == 0 );
                CheckPointGeometry( rdr, L"Geometry", 5, 10, 0 );
            }

            if ( wcscmp( pName, L"Toronto" ) == 0 )
            {
                CPPUNIT_ASSERT( rdr->GetInt32(L"Population") == 2500000 );
                CPPUNIT_ASSERT( wcscmp( rdr2->GetString(L"First N\x00e4me"), L"Mayor" ) == 0 );
                CPPUNIT_ASSERT( wcscmp( rdr2->GetString(L"Last Name"), UnitTestUtil::GetNlsValue(3) ) == 0 );
                CheckPointGeometry( rdr, L"Geometry", 5, 10, 0 );
            }

            if ( wcscmp( pName, L"Cornwall" ) == 0 )
            {
                CPPUNIT_ASSERT( rdr->GetInt32(L"Population") == 800000 );
                CPPUNIT_ASSERT( wcscmp( rdr2->GetString(L"First N\x00e4me"), L"Old" ) == 0 );
                CPPUNIT_ASSERT( wcscmp( rdr2->GetString(L"Last Name"), L"Grey" ) == 0 );
                CheckPointGeometry( rdr, L"Geometry", 5, 10, 0 );
            }

            if ( wcscmp( pName, L"Windsor" ) == 0 )
            {
                CPPUNIT_ASSERT( rdr->GetInt32(L"Population") == 300000 );
                CPPUNIT_ASSERT( wcscmp( rdr2->GetString(L"First N\x00e4me"), L"Old" ) == 0 );
                CPPUNIT_ASSERT( wcscmp( rdr2->GetString(L"Last Name"), L"Grey" ) == 0 );
                CheckPointGeometry( rdr, L"Geometry", 5, 10, 0 );
            }

            if ( wcscmp( pName, L"Kingston" ) == 0 )
            {
                CPPUNIT_ASSERT( rdr->GetInt32(L"Population") == 200000 );
                CPPUNIT_ASSERT( wcscmp( rdr2->GetString(L"First N\x00e4me"), L"Old" ) == 0 );
                CPPUNIT_ASSERT( wcscmp( rdr2->GetString(L"Last Name"), L"Grey" ) == 0 );
                CheckPointGeometry( rdr, L"Geometry", 5, 10, 5 );
            }
        }

        CPPUNIT_ASSERT( rowCount == 5 );

        rdr->Close();

        // Try select with filter (tests defect 547462 fix)

        filter1 = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Name") ),
            FdoComparisonOperations_EqualTo,
            FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Ottawa") )
        );

        filter2 = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Name") ),
            FdoComparisonOperations_EqualTo,
            FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Windsor") )
        );

        filter = FdoFilter::Combine(filter1, FdoBinaryLogicalOperations_Or , filter2 );

        selectCommand = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);
        selectCommand->SetFeatureClassName( L"L\x00e4nd:Municipality" );
        selectCommand->SetFilter(filter);

        rdr = selectCommand->Execute();

        rowCount = 0;

        while ( rdr->ReadNext() )
        {
            rowCount++;
        }

        CPPUNIT_ASSERT( rowCount == 2 );

        connection->Close ();
    }
    catch (FdoException *ex)
    {
        try {
            if( connection )
                connection->Close ();
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
        }
        catch ( ... )
        {
        }
        throw;
    }

}

void FdoUpdateTest::UpdateNlsIdFeatureClass()
{
    FdoPtr<FdoIConnection> connection;
    FdoPtr<FdoITransaction> featureTransaction;

    try
    {
        connection = UnitTestUtil::GetConnection(mSuffix, false);
        UnitTestUtil::CreateLandSchema(connection);
        UnitTestUtil::CreateNonUniqueSchema(connection);

        featureTransaction = connection->BeginTransaction();

        // Clear out any data from previous run.

        FdoPtr<FdoIDelete> deleteCommand = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);
        deleteCommand->SetFeatureClassName(L"L\x00e4nd:Building");
        deleteCommand->Execute();
        deleteCommand = NULL;

        // Create the test data

        FdoPtr<FdoIInsert> insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
        insertCommand->SetFeatureClassName(L"L\x00e4nd:Building");
        FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();
        FdoPtr<FdoDataValue> dataValue;
        FdoPtr<FdoPropertyValue> propertyValue;

        dataValue = FdoDataValue::Create(L"Keep");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"Value");
        propertyValue->SetValue(dataValue);

        double       coordsBuffer[6];
        int          segCount = 2;

        bool supportsZ = (FdoPtr<FdoIGeometryCapabilities>(connection->GetGeometryCapabilities())->GetDimensionalities() & FdoDimensionality_Z);

        if ( supportsZ )
        {
            coordsBuffer[0] = 2;
            coordsBuffer[1] = 15;
            coordsBuffer[2] = 0;
            coordsBuffer[3] = 10;
            coordsBuffer[4] = 15;
            coordsBuffer[5] = 0;
        }
        else
        {
            coordsBuffer[0] = 2;
            coordsBuffer[1] = 15;
            coordsBuffer[2] = 10;
            coordsBuffer[3] = 15;
        }

        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"Geometry");
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

        FdoPtr<FdoILineString> line1;
		
		if ( supportsZ )
			line1 = gf->CreateLineString(FdoDimensionality_XY|FdoDimensionality_Z, segCount*3, coordsBuffer);
		else
			line1 = gf->CreateLineString(FdoDimensionality_XY, segCount*2, coordsBuffer);

        FdoPtr<FdoByteArray> byteArray = gf->GetFgf(line1);
        FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
        propertyValue->SetValue(geometryValue);

        FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();
        dataValue = FdoDataValue::Create(L"Discard");
        propertyValue = propertyValues->GetItem(L"Value");
        propertyValue->SetValue(dataValue);

        reader = insertCommand->Execute();

        dataValue = FdoDataValue::Create(L"Update");
        propertyValue = propertyValues->GetItem(L"Value");
        propertyValue->SetValue(dataValue);

        reader = insertCommand->Execute();

        insertCommand = NULL;
        featureTransaction->Commit();


        FdoPtr<FdoFilter>   filter = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Value") ),
            FdoComparisonOperations_EqualTo,
            FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Update") )
        );


        FdoPtr<FdoIUpdate> UpdateCommand = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
        UpdateCommand->SetFeatureClassName(L"L\x00e4nd:Building");
        UpdateCommand->SetFilter( filter );

        propertyValues = UpdateCommand->GetPropertyValues();

        dataValue = FdoDataValue::Create(L"Updated" );
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"Value");
        propertyValue->SetValue(dataValue);

        UpdateCommand->Execute();

        UpdateCommand = NULL;

        deleteCommand = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);

        filter = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Value") ),
            FdoComparisonOperations_EqualTo,
            FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Discard") )
        );

        deleteCommand->SetFeatureClassName(L"L\x00e4nd:Building");
        deleteCommand->SetFilter( filter );

        deleteCommand->Execute();
        deleteCommand = NULL;

        featureTransaction->Commit();

        // Select and verify all data (post-update state).

        FdoPtr<FdoISelect> selectCommand = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);

        selectCommand->SetFeatureClassName( L"L\x00e4nd:Building" );

        FdoPtr<FdoIFeatureReader> rdr = selectCommand->Execute();
        FdoPtr<FdoIdentifierCollection> props = selectCommand->GetPropertyNames();

        int rowCount = 0;
        bool foundKeep = false;
        bool foundUpdated = false;

        while ( rdr->ReadNext() )
        {
            rowCount++;
            CPPUNIT_ASSERT( !rdr->IsNull(L"Value") );
            FdoStringP val = rdr->GetString(L"Value");
            
            if ( val == L"Keep" ) 
                foundKeep = true;

            if ( val == L"Updated" ) 
                foundUpdated = true;

        }

        CPPUNIT_ASSERT( rowCount == 2 );
        CPPUNIT_ASSERT( foundKeep );
        CPPUNIT_ASSERT( foundUpdated );

        connection->Close ();
    }
    catch (FdoException *ex)
    {
        try {
            if( connection )
                connection->Close ();
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
        }
        catch ( ... )
        {
        }
        throw;
    }

}

void FdoUpdateTest::CheckPointGeometry(FdoPtr<FdoIFeatureReader> rdr, FdoString* propName, double expectedX, double expectedY, double expectedZ)
{
    CPPUNIT_ASSERT( !rdr->IsNull(propName) );

    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoByteArray> sGeom = rdr->GetGeometry( propName );
    CPPUNIT_ASSERT( sGeom );

    FdoPtr<FdoIPoint> point1 = (FdoIPoint*) gf->CreateGeometryFromFgf( sGeom );
    FdoPtr<FdoIDirectPosition> pos = point1->GetPosition();

    CPPUNIT_ASSERT( expectedX == pos->GetX() );
    CPPUNIT_ASSERT( expectedY == pos->GetY() );

    if ( point1->GetDimensionality() ==  (FdoDimensionality_XY|FdoDimensionality_Z)) 
        CPPUNIT_ASSERT( expectedZ == pos->GetZ() );
}

void FdoUpdateTest::CheckLineGeometry(FdoPtr<FdoIFeatureReader> rdr, FdoString* propName, double* coordsLine)
{
    CPPUNIT_ASSERT( !rdr->IsNull(propName) );
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoByteArray> sGeom = rdr->GetGeometry(propName);
    CPPUNIT_ASSERT( sGeom );

    FdoPtr<FdoILineString> line = (FdoILineString*) gf->CreateGeometryFromFgf( sGeom );
    FdoPtr<FdoIDirectPosition> startPos = line->GetStartPosition();
    FdoPtr<FdoIDirectPosition> endPos = line->GetEndPosition();

    if ( line->GetDimensionality() == (FdoDimensionality_XY|FdoDimensionality_Z)) 
    {
        CPPUNIT_ASSERT( coordsLine[0] == startPos->GetX() );
        CPPUNIT_ASSERT( coordsLine[1] == startPos->GetY() );
        CPPUNIT_ASSERT( coordsLine[2] == startPos->GetZ() );
        CPPUNIT_ASSERT( coordsLine[3] == endPos->GetX() );
        CPPUNIT_ASSERT( coordsLine[4] == endPos->GetY() );
        CPPUNIT_ASSERT( coordsLine[5] == endPos->GetZ() );
    }
    else
    {
        CPPUNIT_ASSERT( coordsLine[0] == startPos->GetX() );
        CPPUNIT_ASSERT( coordsLine[1] == startPos->GetY() );
        CPPUNIT_ASSERT( coordsLine[2] == endPos->GetX() );
        CPPUNIT_ASSERT( coordsLine[3] == endPos->GetY() );
    }
}

void FdoUpdateTest::CheckSpatialContexts(FdoPtr<FdoIConnection> connection, int expected)
{
}

void FdoUpdateTest::ConditionalUpdate()
{
    bool ltCreated = false;
	FdoPtr<FdoIConnection> connection;
    FdoStringP updLtName1;
    FdoStringP updLtName2;
    FdoStringP insLtName;
	FdoStringP delLtName;

	try
	{
        connection = UnitTestUtil::GetConnection(L"LT", true, true, Connection_WithDatastore, 1, true);

        // Generate the necessary long transaction names.

        static char connUser[1024];
	UnitTestUtil::GetRealUserName(connUser, 1024);

        wchar_t connUserW[100];
        mbstowcs(connUserW, connUser, 100);
        updLtName1 = FdoStringP::Format(L"%ls_UpdTestLt", connUserW);
        updLtName2 = FdoStringP::Format(L"%ls_UpdTestLt2", connUserW);
        insLtName = FdoStringP::Format(L"%ls_UpdTestInsLt", connUserW);
		delLtName = FdoStringP::Format(L"%ls_UpdTestDelLt", connUserW);

        // The test will be executed on conditionally inserted data. This data
        // is created first.
        // NOTE: The original test operated on conditionally inserted data into
        //       the test tables by the unit test FdoInsertTest::conditionalInsert().
        //       To be able to execute the tests independently, this test has been
        //       modified to create its own conditional data to operate on. 

        ConditionalUpdateSetup (connection, insLtName);
        ltCreated = true;

        // Create the necessary long transaction for the first part of the test.
        // Note: When the previous function terminates the active long transaction
        //       is the one that was passed to the routine. This is important as
        //       all the other long transactions to be created depend on this one.
        //       Also, if the called function failed for some reason, the long
        //       transaction environment setup there has been removed.

		FdoPtr<FdoICreateLongTransaction>create_lt =(FdoICreateLongTransaction *) connection->CreateCommand(
                                           FdoCommandType_CreateLongTransaction);
		create_lt->SetName((FdoString *)updLtName1);
		create_lt->Execute();

		FdoPtr<FdoIActivateLongTransaction>activate_lt =(FdoIActivateLongTransaction *) connection->CreateCommand(
											FdoCommandType_ActivateLongTransaction );
		activate_lt->SetName((FdoString *)updLtName1);
		activate_lt->Execute();

        // Execute the first part of the test.

		FdoPtr<FdoFilter>   filter = FdoFilter::Parse(L"layer = 200");

		FdoPtr<FdoITransaction> featureTransaction = connection->BeginTransaction();
	    
		FdoPtr<FdoIUpdate> UpdateCommand = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
		UpdateCommand->SetFeatureClassName(L"Acad:AcDb3dPolyline");
		UpdateCommand->SetFilter( filter );
		FdoPtr<FdoPropertyValueCollection> propertyValues = UpdateCommand->GetPropertyValues();
		FdoPtr<FdoDataValue> dataValue = FdoDataValue::Create(L"56");
		FdoPtr<FdoPropertyValue> propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"color");
		propertyValue->SetValue(dataValue);
		dataValue = FdoDataValue::Create(L"Lt_Data");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"xdata.DataValue");
        propertyValue->SetValue(dataValue);
		int rows = UpdateCommand->Execute();

        // Create the rest of the long transactions.

		create_lt->SetName((FdoString *)updLtName2);
		create_lt->Execute();

		activate_lt->SetName((FdoString *)updLtName2);
		activate_lt->Execute();
		
		create_lt->SetName((FdoString *)delLtName);
		create_lt->Execute();

        // Execute the next steps of the test.

		propertyValues = UpdateCommand->GetPropertyValues();
		dataValue = FdoDataValue::Create(L"Black");
		propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"color");
		propertyValue->SetValue(dataValue);
		rows = UpdateCommand->Execute();

		featureTransaction->Commit();

		// Check the data we just updated
		FdoPtr<FdoISQLCommand>selCmd = (FdoISQLCommand*)connection->CreateCommand( FdoCommandType_SQLCommand );
        selCmd->SetSQLStatement( L"select * from ACDB3DPOLYLINE" );
        FdoPtr<FdoISQLDataReader>myReader = selCmd->ExecuteReader();
		int  nnull_count = 0; // data rows
		int  null_count = 0;  // sentinel rows
		int  count256 = 0;
		int  count56 = 0;
		int  countBlack = 0;
        if( myReader != NULL  )
        {
            while ( myReader->ReadNext() )
            {
                if( myReader->IsNull(L"color") )
					null_count++;
				else 
				{
					nnull_count++;
					FdoString* color = myReader->GetString(L"color");
					if( wcsncmp(color, L"256",3) == 0 )
						count256++;
					else if( wcsncmp(color, L"56",2) == 0 )
						count56++;
					else if( wcsncmp(color, L"Black",5) == 0 )
						countBlack++;
				}
            }
        }
		CPPUNIT_ASSERT_MESSAGE("Wrong number of null objects", null_count == 4);
		CPPUNIT_ASSERT_MESSAGE("Wrong number of not null objects", nnull_count == 8);
		CPPUNIT_ASSERT_MESSAGE("Wrong number of color=256", count256 == 4);
		CPPUNIT_ASSERT_MESSAGE("Wrong number of color=56", count56 == 2);
		CPPUNIT_ASSERT_MESSAGE("Wrong number of color=Black", countBlack == 2);


		// Activate an LT and make fdo lt based query
		activate_lt->SetName((FdoString *)updLtName1);
		activate_lt->Execute();
	    FdoPtr<FdoISelect>selFdoCmd = (FdoISelect*)connection->CreateCommand( FdoCommandType_Select );
        selFdoCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
        selFdoCmd->SetFilter(L"segcount = 99");
        FdoPtr<FdoIFeatureReader>myFdoReader = selFdoCmd->Execute();
        int count = 0;
		int xdt_count = 0;
        while(myFdoReader->ReadNext())
		{
			count++;
			if( myFdoReader->IsNull(L"color") )
				CPPUNIT_FAIL("Unexpected null color");
			FdoString* color = myFdoReader->GetString(L"color");
			if( wcsncmp(color, L"56",2) != 0 &&
				wcsncmp(color, L"256",3) != 0 )
				CPPUNIT_FAIL("Unexpected color value");
			if( myFdoReader->IsNull(L"xdata") )
				CPPUNIT_FAIL("Unexpected null xdata");
			FdoPtr<FdoIFeatureReader>xdr = myFdoReader->GetFeatureObject(L"xdata");
			bool more_than_one = false;
			while( xdr && xdr->ReadNext())
			{
				if( more_than_one )
					CPPUNIT_FAIL("More than one xdata value returned");
				if( xdr->IsNull(L"DataValue") )
					CPPUNIT_FAIL("Unexpected null xdata.DataValue");

				FdoString* xdata = xdr->GetString(L"DataValue");
				if( wcsncmp(xdata,L"Lt_Data", 7) == 0 )
					xdt_count++;

				more_than_one = true;
			}
		}
        CPPUNIT_ASSERT_MESSAGE("Wrong number of LT based objects", count == 4);
		CPPUNIT_ASSERT_MESSAGE("Wrong number of LT based xdata", xdt_count == 2);

		// Test an object property based query
		selFdoCmd->SetFilter(L"xdata.DataValue='Lt_Data'");
        myFdoReader = selFdoCmd->Execute();
        count = 0;
        while(myFdoReader->ReadNext())count++;
		CPPUNIT_ASSERT_MESSAGE("Wrong number of objects returned by an LT Object property based filter(1)",count==2);

		activate_lt->SetName((FdoString *)insLtName);
		activate_lt->Execute();
		myFdoReader = selFdoCmd->Execute();
        count = 0;
        while(myFdoReader->ReadNext())count++;
		CPPUNIT_ASSERT_MESSAGE("Wrong number of objects returned by an LT Object property based filter(2)",count==0);


		// OK let's do some conditional deletes
		// For that activate the long transaction referenced by "delLtName"
        // and use it to mark few object as conditionaly deleted
		activate_lt->SetName((FdoString *)delLtName);
		activate_lt->Execute();
		FdoPtr<FdoIDelete> deleteCommand = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);
        deleteCommand->SetFeatureClassName(L"Acad:AcDb3dPolyline");
	    deleteCommand->SetFilter( L"xdata.DataValue='Lt_Data'" );
        count = deleteCommand->Execute();

		// Check if we have 2 objects marked as deleted
		selCmd = (FdoISQLCommand*)connection->CreateCommand( FdoCommandType_SQLCommand );
        selCmd->SetSQLStatement( L"select * from ACDB3DPOLYLINE WHERE LTID<0 " );
        myReader = selCmd->ExecuteReader();
		count = 0;
        if( myReader != NULL  )
            while ( myReader->ReadNext() )count++;
		CPPUNIT_ASSERT_MESSAGE("Wrong number of deleted object properties",count==2);

		// Check if we have 2 objects property marked as deleted
		selCmd = (FdoISQLCommand*)connection->CreateCommand( FdoCommandType_SQLCommand );
        selCmd->SetSQLStatement( L"select * from ACDBENTITY_ACXDATA WHERE LTID<0 AND DataValue='Lt_Data' " );
        myReader = selCmd->ExecuteReader();
		count = 0;
        if( myReader != NULL  )
            while ( myReader->ReadNext() )count++;
		CPPUNIT_ASSERT_MESSAGE("Wrong number of deleted object properties",count==2);

        // Last, clean up the long transaction test environment. To ensure the
        // clean-up works deactivate the currently active long transaction first.

        FdoPtr<FdoIDeactivateLongTransaction>deactivate_lt = (FdoIDeactivateLongTransaction *) connection->CreateCommand(
                                            FdoCommandType_DeactivateLongTransaction);
        deactivate_lt->Execute();
        
		FdoPtr<FdoIRollbackLongTransaction>rollback_lt =(FdoIRollbackLongTransaction *) connection->CreateCommand(
											FdoCommandType_RollbackLongTransaction);

        rollback_lt->SetName((FdoString *)insLtName);
        rollback_lt->Execute();
        ltCreated = false;

	}
    catch (FdoException *ex)
    {
        TestCommonFail(ex);

        // Clean up the long transaction test environment. To ensure the clean-up
        // works deactivate the currently active long transaction first.
        if (ltCreated)
        {
            FdoPtr<FdoIDeactivateLongTransaction>deactivate_lt = (FdoIDeactivateLongTransaction *) connection->CreateCommand(
                                            FdoCommandType_DeactivateLongTransaction);
            deactivate_lt->Execute();
        
		    FdoPtr<FdoIRollbackLongTransaction>rollback_lt =(FdoIRollbackLongTransaction *) connection->CreateCommand(
											FdoCommandType_RollbackLongTransaction);

            rollback_lt->SetName((FdoString *)insLtName);
            rollback_lt->Execute();
            ltCreated = false;
        }
    }
	catch (...)
    {

        // Clean up the long transaction test environment. To ensure the clean-up
        // works deactivate the currently active long transaction first.
        if (ltCreated)
        {
            FdoPtr<FdoIDeactivateLongTransaction>deactivate_lt = (FdoIDeactivateLongTransaction *) connection->CreateCommand(
                                            FdoCommandType_DeactivateLongTransaction);
            deactivate_lt->Execute();
        
		    FdoPtr<FdoIRollbackLongTransaction>rollback_lt =(FdoIRollbackLongTransaction *) connection->CreateCommand(
											FdoCommandType_RollbackLongTransaction);

            rollback_lt->SetName((FdoString *)insLtName);
            rollback_lt->Execute();
            ltCreated = false;
        }
        throw;
    }
}

void FdoUpdateTest::ConditionalUpdateSetup (FdoIConnection *connection,
                                            FdoStringP insLtName)
{

bool ltCreated = false;

try
    {

        wchar_t *data[] = {
            L"121234567",
            L"1234", 
            L"aaaaaaaaaabbbbiiiii", 
            L"3333"};

        try
        {

			FdoPtr<FdoICreateLongTransaction>create_lt =(FdoICreateLongTransaction *) connection->CreateCommand(
											   FdoCommandType_CreateLongTransaction);
			create_lt->SetName((FdoString *)insLtName);
			create_lt->Execute();
            ltCreated = true;
			
			FdoPtr<FdoIActivateLongTransaction>activate_lt =(FdoIActivateLongTransaction *) connection->CreateCommand(
												FdoCommandType_ActivateLongTransaction );
			activate_lt->SetName((FdoString *)insLtName);
			activate_lt->Execute();

        }
        catch (...)
        {
            throw;
        }

        // Insert the test data.
        try
        {
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

            propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"Geometry");
            FdoPtr<FdoByteArray> byteArray = gf->GetFgf(line1);

            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

            FdoPtr<FdoDataValue> dataValue;
            dataValue = FdoDataValue::Create(L"256");
            propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"color");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(segCount);
            propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"segcount");
            propertyValue->SetValue(dataValue);

#if 1
            dataValue = FdoDataValue::Create(1);
            propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"xdata.seq");
            propertyValue->SetValue(dataValue);
#endif
            FdoPtr<FdoITransaction> featureTransaction = connection->BeginTransaction();
            for (int j=0; j<4; j++)
            {
				if(j%2)
					dataValue = FdoDataValue::Create(L"200");
				else
					dataValue = FdoDataValue::Create(L"300");
				propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"layer");
				propertyValue->SetValue(dataValue);
#if 1
                dataValue = FdoDataValue::Create(data[j%4]);
                propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, L"xdata.DataValue");
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
        }
        catch (...)
        {
            throw;
        }
    }
    catch (FdoCommandException *ex)
    {
        if (ltCreated)
        {
		    // Cleanup the test lt
		    FdoPtr<FdoIRollbackLongTransaction>rollback_lt =(FdoIRollbackLongTransaction *) connection->CreateCommand(
											FdoCommandType_RollbackLongTransaction);
		    rollback_lt->SetName((FdoString *)insLtName);
		    rollback_lt->Execute();
            ltCreated = false;
        }
        throw ex;
    }
    catch (FdoException *ex)
    {
        if (ltCreated)
        {
		    // Cleanup the test lt
		    FdoPtr<FdoIRollbackLongTransaction>rollback_lt =(FdoIRollbackLongTransaction *) connection->CreateCommand(
											FdoCommandType_RollbackLongTransaction);
		    rollback_lt->SetName((FdoString *)insLtName);
		    rollback_lt->Execute();
            ltCreated = false;
        }
        throw ex;
    }
}

void FdoUpdateTest::UpdateNoMeta()
{
	FdoPtr<FdoIConnection> connection;
    StaticConnection* conn = UnitTestUtil::NewStaticConnection();
    FdoStringP providerName = conn->GetServiceName();
    FdoStringP table_id_geom = TableIdGeomName();
    FdoStringP table_noid_nogeom = TableNoIdNoGeomName();

    try {
        FdoStringP datastore = UnitTestUtil::GetEnviron("datastore", NoMetaSuffix());

        printf( "\nOpening Connection ...\n" );

        conn->connect();

        FdoSchemaManagerP mgr = conn->CreateSchemaManager();

        FdoSmPhMgrP phMgr = mgr->GetPhysicalSchema()->SmartCast<FdoSmPhGrdMgr>();

        FdoSmPhDatabaseP database = phMgr->GetDatabase();

        printf( "Predeleting schema ...\n" );

        FdoSmPhOwnerP owner = phMgr->FindOwner( datastore, L"", false );
        if ( owner ) {
            owner->SetElementState( FdoSchemaElementState_Deleted );
            owner->Commit();
        }

        printf( "Creating schema ...\n" );

        // Create a datastore without MetaSchema
        owner = database->CreateOwner(
            datastore, 
            false
        );
        owner->SetPassword( L"test" );
        owner->Commit();

        owner->SetCurrent();

        CreateExternalTable( owner, table_id_geom, true, m_hasGeom, m_hasAssoc );
        CreateExternalView( owner, L"view_id_geom", table_id_geom, true, m_hasGeom, false );

        // Skip this case for SqlServer 2008, which doesn't allow geometric columns in a
        // table without identity.
        if ( providerName != L"SQLServerSpatial" ) 
            CreateExternalTable( owner, L"table_noid_geom", false, m_hasGeom, false );
        
        CreateExternalTable( owner, table_noid_nogeom, false, false, false );

        // Special tables for special testing. 
        CreateExternalTable( owner, L"table_id_geom_ll", true, m_hasGeom, false );
        CreateExternalTable( owner, L"table_id_geom_xyzm", true, m_hasGeom, false );

        owner->Commit();

        FdoSmPhGrdOwnerP grdOwner = owner->SmartCast<FdoSmPhGrdOwner>();

//TODO: investigate why creating this view causes the SqlServer provider to hang when reading
// feature schemas.
        if ( CanHandleNullCol() ) {
            grdOwner->ActivateAndExecute(
                FdoStringP::Format(
                    L"create view view_w_nullcol as select \"%ls\", \"%ls\", \"%ls\", null as nullcol from \"%ls\"",
                    (FdoString*) phMgr->GetDcColumnName(L"key1"),
                    (FdoString*) phMgr->GetDcColumnName(ValueColName()),
                    (FdoString*) phMgr->GetDcColumnName(Key2ColName()),
                    (FdoString*) phMgr->GetDcDbObjectName(table_id_geom).Replace(L".",L"\".\"")
                )
            );
        }

        connection = UnitTestUtil::CreateConnection(
            false,
            false,
            NoMetaSuffix()
        );

        UpdSpatialMetadata( connection );
        CreateExternalData( connection, phMgr, table_id_geom, m_hasGeom, m_hasAssoc );

        if ( providerName != L"SQLServerSpatial" ) {
            if ( m_hasGeom ) {
                UnitTestUtil::Sql2Db( 
                    FdoStringP::Format(
                        L"insert into table_noid_geom ( key1, \"%ls\", \"%ls\", \"%ls\" ) select key1, \"%ls\", \"%ls\", \"%ls\" from \"%ls\"",
                        (FdoString*) phMgr->GetDcColumnName( Key2ColName() ),
                        (FdoString*) phMgr->GetDcColumnName( GeomColName() ),
                        (FdoString*) phMgr->GetDcColumnName( ValueColName() ),
                        (FdoString*) phMgr->GetDcColumnName( Key2ColName() ),
                        (FdoString*) phMgr->GetDcColumnName( GeomColName() ),
                        (FdoString*) phMgr->GetDcColumnName( ValueColName() ),
                        (FdoString*) phMgr->GetDcDbObjectName( table_id_geom ).Replace(L".",L"\".\"")
                    ),
                    connection 
                );
            }
            else {
                UnitTestUtil::Sql2Db( 
                    FdoStringP::Format(
                        L"insert into table_noid_geom ( key1, \"%ls\", \"%ls\" ) select key1, \"%ls\", \"%ls\" from \"%ls\"", 
                        (FdoString*) phMgr->GetDcColumnName( Key2ColName() ),
                        (FdoString*) phMgr->GetDcColumnName( ValueColName() ),
                        (FdoString*) phMgr->GetDcColumnName( Key2ColName() ),
                        (FdoString*) phMgr->GetDcColumnName( ValueColName() ),
                        (FdoString*) phMgr->GetDcDbObjectName( table_id_geom ).Replace(L".",L"\".\"")
                    ),
                    connection 
                );
            }
        }

        UnitTestUtil::Sql2Db( 
                FdoStringP::Format(
                    L"insert into \"%ls\" ( key1, \"%ls\", \"%ls\" ) select key1, \"%ls\", \"%ls\" from \"%ls\"",
                    (FdoString*) phMgr->GetDcDbObjectName( table_noid_nogeom ).Replace(L".",L"\".\""),
                    (FdoString*) phMgr->GetDcColumnName( Key2ColName() ),
                    (FdoString*) phMgr->GetDcColumnName( ValueColName() ),
                    (FdoString*) phMgr->GetDcColumnName( Key2ColName() ),
                    (FdoString*) phMgr->GetDcColumnName( ValueColName() ),
                    (FdoString*) phMgr->GetDcDbObjectName( table_id_geom ).Replace(L".",L"\".\"")
                ),
                connection 
            );

        // Select and verify all data (post-update state).
        SelectNoMetaAll( connection, phMgr, table_id_geom, m_hasGeom, m_hasAssoc );
        SelectNoMetaAll( connection, phMgr, L"view_id_geom", m_hasGeom, false );
        if ( CanHandleNullCol() ) 
            SelectNoMetaAll( connection, phMgr, L"view_w_nullcol", false, false, true );

        if ( providerName != L"SQLServerSpatial" ) 
            SelectNoMetaAll( connection, phMgr, L"table_noid_geom", m_hasGeom, false );

        SelectNoMetaAll( connection, phMgr, table_noid_nogeom, false, false );

        SelectNoMetaFilter( connection, phMgr, table_id_geom, m_hasGeom, m_hasAssoc );
        SelectNoMetaFilter( connection, phMgr, L"view_id_geom", m_hasGeom, false );

        if ( providerName != L"SQLServerSpatial" ) 
            SelectNoMetaFilter( connection, phMgr, L"table_noid_geom", m_hasGeom, false );

        SelectNoMetaFilter( connection, phMgr, table_noid_nogeom, false, false );

        if ( m_hasGeom ) {
		    SelectNoMetaSpatial( connection, phMgr, table_id_geom, m_hasAssoc );
		    SelectNoMetaSpatial( connection, phMgr, L"view_id_geom", false );
            if ( providerName != L"SQLServerSpatial" ) 
	    	    SelectNoMetaSpatial( connection, phMgr, L"table_noid_geom", false );
        }

        SelectNoMetaProps( connection, phMgr, table_id_geom, m_hasGeom );
        SelectNoMetaProps( connection, phMgr, L"view_id_geom", m_hasGeom );

        if ( providerName != L"SQLServerSpatial" ) 
            SelectNoMetaProps( connection, phMgr, L"table_noid_geom", m_hasGeom );
        
        SelectNoMetaProps( connection, phMgr, table_noid_nogeom, false );

        connection->Close ();

        phMgr = NULL;
        mgr = NULL;
        conn->disconnect();
        delete conn;
        conn = NULL;
    }
    catch (FdoException *ex)
    {
        try {
            if( connection )
                connection->Close ();
            if (conn != NULL)
            {
                conn->disconnect();
                delete conn;
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
            if (conn != NULL)
            {
                conn->disconnect();
                delete conn;
            }
        }
        catch ( ... )
        {
        }
        throw;
    }
}

void FdoUpdateTest::CreateExternalTable( FdoSmPhOwnerP owner, FdoStringP tableName, bool hasKey, bool hasGeom, bool hasAssoc )
{
    FdoSmPhMgrP phMgr = owner->GetManager();
    FdoSmPhTableP table = owner->CreateTable( phMgr->GetDcDbObjectName(tableName) );
    
    FdoSmPhColumnP column = table->CreateColumnChar( phMgr->GetDcColumnName(L"key1"), false, 10 );
    if ( hasKey ) 
        table->AddPkeyCol( column->GetName() );
    
    FdoSmPhColumnP fkColumn = table->CreateColumnChar( phMgr->GetDcColumnName(Key2ColName()), false, 10 );
    if ( hasKey ) 
        table->AddPkeyCol( fkColumn->GetName() );

    if ( hasGeom )
        column = table->CreateColumnGeom( phMgr->GetDcColumnName(GeomColName()), CreateScInfo(tableName), true, false );

    column = table->CreateColumnChar( phMgr->GetDcColumnName(ValueColName()), true, 20 );

    if ( hasKey && hasAssoc ) {
        FdoStringP pkTableName = tableName + L"_assoc";
        FdoStringP  fkeyName = L"lookup";

        FdoSmPhTableP pkTable = owner->CreateTable( phMgr->GetDcDbObjectName(pkTableName) );
        
        FdoSmPhColumnP pkColumn = pkTable->CreateColumnChar( phMgr->GetDcColumnName(Key2ColName()), false, 10 );
        pkTable->AddPkeyCol( pkColumn->GetName() );

        FdoSmPhColumnP column = pkTable->CreateColumnChar( phMgr->GetDcColumnName(L"name"), true, 20 );

        FdoSmPhFkeyP fkey = table->CreateFkey( 
            phMgr->GetDcColumnName(fkeyName), 
            phMgr->GetDcDbObjectName(pkTableName)
        );
        fkey->AddFkeyColumn( fkColumn, pkColumn->GetName() );
    }
}

void FdoUpdateTest::CreateExternalView( FdoSmPhOwnerP owner, FdoStringP viewName, FdoStringP tableName, bool hasKey, bool hasGeom, bool hasAssoc )
{
    FdoSmPhMgrP phMgr = owner->GetManager();

    FdoSmPhViewP view = owner->CreateView( phMgr->GetDcDbObjectName(viewName), L"", owner->GetName(), phMgr->GetDcDbObjectName(tableName) );
    
    FdoStringP columnName = phMgr->GetDcColumnName(L"key1");
    FdoSmPhColumnP column = view->CreateColumnChar( columnName, false, 10, columnName );
    
    columnName = phMgr->GetDcColumnName(Key2ColName());
    FdoSmPhColumnP fkColumn = view->CreateColumnChar( columnName, false, 10, columnName );

    if ( hasGeom ) {
        columnName = phMgr->GetDcColumnName(GeomColName());
        column = view->CreateColumnGeom( columnName, CreateScInfo(tableName), true, false, false, columnName );
    }

    columnName = phMgr->GetDcColumnName(ValueColName());
    column = view->CreateColumnChar( columnName, true, 20, columnName );
}

FdoPtr<FdoSmPhScInfo> FdoUpdateTest::CreateScInfo( FdoStringP objectName )
{
    FdoSmPhScInfo *scinfo = NULL;
		
	// The tables need to have the same SRID since this test is copying from one another
	scinfo = FdoSmPhScInfo::Create();

	// The tables need to have the same SRID since this test is copying from one another
	scinfo->mSrid = GetSrid(81989); // British National Grid
	scinfo->mCoordSysName = L"";

	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
	FdoPtr<FdoIEnvelope>          env;

	if ( objectName == L"TABLE_ID_GEOM" )
		env = gf->CreateEnvelopeXY( -1001, -1002, 1001, 1002 );
	else if ( objectName == L"TABLE_ID_GEOM_LL" )
	{
		env = gf->CreateEnvelopeXY( -180, -90, 180, 90 );
		scinfo->mSrid = GetSrid(524288); // Longitude / Latitude (NAD 83) Datum 33
	}
	else if ( objectName == L"TABLE_ID_GEOM_XYZM" )
	{
		env = gf->CreateEnvelopeXYZ( -3001, -3002, -3003, 3001, 3002, 3003);
	}
	else
		env = gf->CreateEnvelopeXY( -2001, -2002, 2001, 2002 );
	
    FdoPtr<FdoIGeometry>		  geom = gf->CreateGeometry(env); 
	scinfo->mExtent = gf->GetFgf(geom);
	scinfo->mXYTolerance = 0.0333;
	scinfo->mZTolerance = 0.0111;

    return scinfo;
}

bool FdoUpdateTest::GetHasElevation( FdoStringP objectName )
{
	bool						  hasElevation = true;

	if ( objectName == L"TABLE_ID_GEOM_LL" )
		hasElevation = false;

    return hasElevation;
}

void FdoUpdateTest::CreateExternalData( FdoPtr<FdoIConnection> connection, FdoSmPhMgrP phMgr, FdoStringP tableName, bool hasGeom, bool hasAssoc )
{

    FdoPtr<FdoITransaction> featureTransaction = connection->BeginTransaction();
    FdoPtr<FdoIInsert> insertCommand;
    FdoPtr<FdoPropertyValueCollection> propertyValues;
    FdoPtr<FdoDataValue> dataValue;
    FdoPtr<FdoPropertyValue> propertyValue;
    FdoPtr<FdoIFeatureReader> reader;

    // Do some inserts. Verifies that objects, for feature classes without featid,
    // can be created.
    
    FdoStringP qClassName = phMgr->GetDcDbObjectName(tableName).Replace(L".",L":");
    
    if ( hasAssoc ) {    
        FdoStringP qPkClassName = phMgr->GetDcDbObjectName(tableName + L"_assoc").Replace(L".",L":");

        insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
        insertCommand->SetFeatureClassName(qPkClassName);

        propertyValues = insertCommand->GetPropertyValues();

        dataValue = FdoDataValue::Create(L"KEY2_1");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, phMgr->GetDcColumnName(Key2ColName()));
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(L"NAME1");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, phMgr->GetDcColumnName(L"name") );
        propertyValue->SetValue(dataValue);
        
        reader = insertCommand->Execute();

        dataValue = FdoDataValue::Create(L"KEY2_2");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, phMgr->GetDcColumnName(Key2ColName()));
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(L"NAME2");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, phMgr->GetDcColumnName(L"name") );
        propertyValue->SetValue(dataValue);
        
        reader = insertCommand->Execute();

        dataValue = FdoDataValue::Create(L"KEY2_3");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, phMgr->GetDcColumnName(Key2ColName()) );
        propertyValue->SetValue(dataValue);
        
        dataValue = FdoDataValue::Create(L"NAME3");
        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, phMgr->GetDcColumnName(L"name") );
        propertyValue->SetValue(dataValue);

        reader = insertCommand->Execute();
    }

    insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
    insertCommand->SetFeatureClassName(qClassName);

    propertyValues = insertCommand->GetPropertyValues();
    propertyValue;

    dataValue = FdoDataValue::Create(L"KEY1_1");
    propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, phMgr->GetDcColumnName(L"key1"));
    propertyValue->SetValue(dataValue);
    
    dataValue = FdoDataValue::Create(L"KEY2_1");
    propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, phMgr->GetDcColumnName(Key2ColName()));
    propertyValue->SetValue(dataValue);

    dataValue = FdoDataValue::Create(L"A");
    propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, phMgr->GetDcColumnName(ValueColName()) );
    propertyValue->SetValue(dataValue);
    
    double       coordsBuffer[4];
    int          segCount = 2;
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    if ( hasGeom ) {

		// Use 2D to accomodate MySql
        coordsBuffer[0] = 5;
        coordsBuffer[1] = 10;
        coordsBuffer[2] = 15;
        coordsBuffer[3] = 20;

        propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, phMgr->GetDcColumnName(GeomColName()) );
        FdoPtr<FdoILineString> line1 = gf->CreateLineString(FdoDimensionality_XY, segCount*2, coordsBuffer);
        FdoPtr<FdoByteArray> byteArray = gf->GetFgf(line1);
        FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray); 
        propertyValue->SetValue(geometryValue);
    }

    reader = insertCommand->Execute();
    CPPUNIT_ASSERT( reader!= NULL );
    
    dataValue = FdoDataValue::Create(L"KEY1_2");
    propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, phMgr->GetDcColumnName(L"key1") );
    propertyValue->SetValue(dataValue);
    
    dataValue = FdoDataValue::Create(L"KEY2_2");
    propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, phMgr->GetDcColumnName(Key2ColName()) );
    propertyValue->SetValue(dataValue);

    dataValue = FdoDataValue::Create(L"B");
    propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, phMgr->GetDcColumnName(ValueColName()) );
    propertyValue->SetValue(dataValue);
    
    reader = insertCommand->Execute();

    dataValue = FdoDataValue::Create(L"KEY1_3");
    propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, phMgr->GetDcColumnName(L"key1") );
    propertyValue->SetValue(dataValue);
    
    dataValue = FdoDataValue::Create(L"KEY2_3");
    propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, phMgr->GetDcColumnName(Key2ColName()) );
    propertyValue->SetValue(dataValue);
    
    dataValue = FdoDataValue::Create(L"C");
    propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, phMgr->GetDcColumnName(ValueColName()) );
    propertyValue->SetValue(dataValue);
    
    reader = insertCommand->Execute();

    featureTransaction->Commit();
    featureTransaction = NULL;

    FdoPtr<FdoFilter>   filter = FdoComparisonCondition::Create(
        FdoPtr<FdoIdentifier>(FdoIdentifier::Create(phMgr->GetDcColumnName(L"key1")) ),
        FdoComparisonOperations_EqualTo,
        FdoPtr<FdoDataValue>(FdoDataValue::Create(L"KEY1_3"))
    );
    
    // Verifies that features without featid can be updated
    
    FdoPtr<FdoIUpdate> UpdateCommand = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
    UpdateCommand->SetFeatureClassName(qClassName);
    UpdateCommand->SetFilter( filter );
    propertyValues = UpdateCommand->GetPropertyValues();
    
    dataValue = FdoDataValue::Create(L"M\x00f6"L"dified");
    propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, phMgr->GetDcColumnName(ValueColName()) );
    propertyValue->SetValue(dataValue);
    UpdateCommand->Execute();
    
    // Try geometric property update
    
    if ( hasGeom ) {
        filter = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(phMgr->GetDcColumnName(L"key1")) ),
            FdoComparisonOperations_EqualTo,
            FdoPtr<FdoDataValue>(FdoDataValue::Create(L"KEY1_1") )
        );
        
        UpdateCommand = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
        UpdateCommand->SetFeatureClassName(qClassName);
        UpdateCommand->SetFilter( filter );
        propertyValues = UpdateCommand->GetPropertyValues();

        coordsBuffer[1] = 5;
        propertyValue = AddNewProperty( propertyValues, phMgr->GetDcColumnName(GeomColName()) );
        FdoPtr<FdoILineString> line1 = gf->CreateLineString(FdoDimensionality_XY, segCount*2, coordsBuffer);
        FdoPtr<FdoByteArray> byteArray = gf->GetFgf(line1);
        FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);

        propertyValue->SetValue(geometryValue);

        UpdateCommand->Execute();
        UpdateCommand = NULL;
    }
    
    // Verifies that features without featid can be deleted.
    
    FdoPtr<FdoIDelete> deleteCommand = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);
    filter = FdoComparisonCondition::Create(
        FdoPtr<FdoIdentifier>(FdoIdentifier::Create(phMgr->GetDcColumnName(L"key1")) ),
        FdoComparisonOperations_EqualTo,
        FdoPtr<FdoDataValue>(FdoDataValue::Create(L"KEY1_2") )
    );
        
    deleteCommand->SetFeatureClassName(qClassName);
    deleteCommand->SetFilter( filter );
    deleteCommand->Execute();
    deleteCommand = NULL;
}

void FdoUpdateTest::UpdSpatialMetadata( FdoPtr<FdoIConnection> connection )
{
}

void FdoUpdateTest::SelectNoMetaAll( FdoPtr<FdoIConnection> connection, FdoSmPhMgrP phMgr, FdoStringP tableName, bool hasGeom, bool hasAssoc, bool hasNullcol )
{
    FdoPtr<FdoISelect> selectCommand = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);

    selectCommand->SetFeatureClassName( phMgr->GetDcDbObjectName(tableName).Replace(L".",L":") );

    FdoPtr<FdoIFeatureReader> rdr = selectCommand->Execute();

    FdoPtr<FdoIdentifierCollection> props = selectCommand->GetPropertyNames();

    int rowCount = 0;

    while ( rdr->ReadNext() )

    {
        rowCount++;
        VldNoMetaRow( phMgr, rdr, props, hasGeom, false, hasAssoc, hasNullcol );
    }

    CPPUNIT_ASSERT( rowCount == 2 );

    rdr->Close();
}

void FdoUpdateTest::SelectNoMetaFilter( FdoPtr<FdoIConnection> connection, FdoSmPhMgrP phMgr, FdoStringP tableName, bool hasGeom, bool hasAssoc )
{
    FdoPtr<FdoISelect> selectCommand = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);

    // Try select with filter

    FdoPtr<FdoFilter> filter = FdoComparisonCondition::Create(
        FdoPtr<FdoIdentifier>(FdoIdentifier::Create(phMgr->GetDcColumnName(L"key1")) ),
        FdoComparisonOperations_EqualTo,
        FdoPtr<FdoDataValue>(FdoDataValue::Create(L"KEY1_3") )
    );

    selectCommand = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);

    selectCommand->SetFeatureClassName( phMgr->GetDcDbObjectName(tableName).Replace(L".",L":") );
    selectCommand->SetFilter(filter);

    FdoPtr<FdoIFeatureReader> rdr = selectCommand->Execute();

    FdoPtr<FdoIdentifierCollection> props = selectCommand->GetPropertyNames();

    int rowCount = 0;

    while ( rdr->ReadNext() )

    {
        rowCount++;
        CPPUNIT_ASSERT( wcscmp( FixStringVal(rdr->GetString(phMgr->GetDcColumnName(L"key1"))), L"KEY1_3" ) == 0 );
        VldNoMetaRow( phMgr, rdr, props, hasGeom, false, hasAssoc );
    }

    CPPUNIT_ASSERT( rowCount == 1 );

    rdr->Close();
}

void FdoUpdateTest::SelectNoMetaProps( FdoPtr<FdoIConnection> connection, FdoSmPhMgrP phMgr, FdoStringP tableName, bool hasGeom )
{
    FdoPtr<FdoISelect> selectCommand = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);

    // Try select with filter

    FdoPtr<FdoFilter> filter = FdoComparisonCondition::Create(
        FdoPtr<FdoIdentifier>(FdoIdentifier::Create(phMgr->GetDcColumnName(L"key1")) ),
        FdoComparisonOperations_EqualTo,
        FdoPtr<FdoDataValue>(FdoDataValue::Create(L"KEY1_3") )
    );

    selectCommand = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);

    selectCommand->SetFeatureClassName( phMgr->GetDcDbObjectName(tableName).Replace(L".",L":") );
    selectCommand->SetFilter(filter);

    FdoPtr<FdoIdentifierCollection> props = selectCommand->GetPropertyNames();
    FdoPtr<FdoIdentifier> prop = FdoIdentifier::Create(phMgr->GetDcColumnName(L"key1"));
    props->Add(prop);
    prop = FdoIdentifier::Create(phMgr->GetDcColumnName(ValueColName()));
    props->Add(prop);

    FdoPtr<FdoIFeatureReader> rdr = selectCommand->Execute();

    int rowCount = 0;

    while ( rdr->ReadNext() )

    {
        rowCount++;
        CPPUNIT_ASSERT( wcscmp( FixStringVal(rdr->GetString(phMgr->GetDcColumnName(L"key1"))), L"KEY1_3" ) == 0 );
        VldNoMetaRow( phMgr, rdr, props, hasGeom, true, false );
    }

    CPPUNIT_ASSERT( rowCount == 1 );

    rdr->Close();
}

void FdoUpdateTest::SelectNoMetaSpatial( FdoPtr<FdoIConnection> connection, FdoSmPhMgrP phMgr, FdoStringP tableName, bool hasAssoc )
{
    FdoPtr<FdoISelect> selectCommand = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);

    // Try select with filter


    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    double ordsXYExt[15];
    ordsXYExt[0] = 2.5; ordsXYExt[1] = 2.5; ordsXYExt[2] = 0.0;
    ordsXYExt[3] = 2.5; ordsXYExt[4] = 7.5; ordsXYExt[5] = 0.0;
    ordsXYExt[6] = 7.5; ordsXYExt[7] = 7.5; ordsXYExt[8] = 0.0;
    ordsXYExt[9] = 7.5; ordsXYExt[10] = 2.5; ordsXYExt[11] = 0.0;
    ordsXYExt[12] = 2.5; ordsXYExt[13] = 2.5;  ordsXYExt[14] = 0.0;

    FdoPtr<FdoILinearRing> extRing = gf->CreateLinearRing(FdoDimensionality_XY|FdoDimensionality_Z, 15, ordsXYExt);
    FdoPtr<FdoIPolygon> poly = gf->CreatePolygon(extRing, NULL );
    FdoPtr<FdoGeometryValue> geomValue = FdoGeometryValue::Create(FdoPtr<FdoByteArray>(gf->GetFgf(poly)));
    FdoPtr<FdoSpatialCondition> spatialFilter = FdoPtr<FdoSpatialCondition>(FdoSpatialCondition::Create(phMgr->GetDcColumnName(GeomColName()),
                                                                      FdoSpatialOperations_Intersects,
                                                                      geomValue));

    selectCommand = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);

    selectCommand->SetFeatureClassName( phMgr->GetDcDbObjectName(tableName).Replace(L".",L":") );
    selectCommand->SetFilter(spatialFilter);

    FdoPtr<FdoIFeatureReader> rdr = selectCommand->Execute();

    FdoPtr<FdoIdentifierCollection> props = selectCommand->GetPropertyNames();

    int rowCount = 0;

    while ( rdr->ReadNext() )

    {
        rowCount++;
        CPPUNIT_ASSERT( wcscmp( FixStringVal(rdr->GetString(phMgr->GetDcColumnName(L"key1"))), L"KEY1_1" ) == 0 );
        VldNoMetaRow( phMgr, rdr, props, true, false, hasAssoc );
    }

    CPPUNIT_ASSERT( rowCount == 1 );

    rdr->Close();
}

void FdoUpdateTest::VldNoMetaRow( 
    FdoSmPhMgrP phMgr, 
    FdoPtr<FdoIFeatureReader> rdr, 
    FdoPtr<FdoIdentifierCollection> props, 
    bool hasGeom, 
    bool propsPruned,
    bool hasAssoc,
    bool hasNullcol
)
{
    FdoPtr<FdoIFeatureReader> objRdr;
    FdoStringP key1val = FixStringVal( rdr->GetString(phMgr->GetDcColumnName(L"key1")) );

    if ( wcscmp(key1val, L"KEY1_1" ) == 0 )
    {
        if ( !propsPruned ) {
            CPPUNIT_ASSERT( wcscmp( FixStringVal(rdr->GetString(phMgr->GetDcColumnName(Key2ColName()))), L"KEY2_1" ) == 0 );
        
            if ( hasAssoc ) {
                objRdr = rdr->GetFeatureObject( phMgr->GetDcColumnName(L"lookup") );
                CPPUNIT_ASSERT( objRdr != NULL );
                CPPUNIT_ASSERT( objRdr->ReadNext() );
                CPPUNIT_ASSERT( wcscmp( FixStringVal(objRdr->GetString(phMgr->GetDcColumnName(L"name"))), L"NAME1" ) == 0 );
            }
        }

        CPPUNIT_ASSERT( wcscmp( FixStringVal(rdr->GetString(phMgr->GetDcColumnName(ValueColName()))), L"A" ) == 0 );

        if ( hasGeom & !propsPruned )         
            CheckPointGeometry( rdr, phMgr->GetDcColumnName(GeomColName()), 5, 5, 0 );
    }

    if ( wcscmp(key1val, L"KEY1_3" ) == 0 )
    {
        if ( !propsPruned ) {
            CPPUNIT_ASSERT( wcscmp( FixStringVal(rdr->GetString(phMgr->GetDcColumnName(Key2ColName()))), L"KEY2_3" ) == 0 );
                
            if ( hasAssoc ) {
                objRdr = rdr->GetFeatureObject( phMgr->GetDcColumnName(L"lookup") );
                CPPUNIT_ASSERT( objRdr != NULL );
                CPPUNIT_ASSERT( objRdr->ReadNext() );
                CPPUNIT_ASSERT( wcscmp( FixStringVal(objRdr->GetString(phMgr->GetDcColumnName(L"name"))), L"NAME3" ) == 0 );
            }
        }

        CPPUNIT_ASSERT( wcscmp( FixStringVal(rdr->GetString(phMgr->GetDcColumnName(ValueColName()))), L"M\x00f6"L"dified" ) == 0 );

        if ( hasGeom && !propsPruned ) 
            CheckPointGeometry( rdr, phMgr->GetDcColumnName(GeomColName()), 5, 10, 0 );
    }

    if ( hasNullcol ) {
        CPPUNIT_ASSERT( rdr->IsNull(phMgr->GetDcColumnName(L"nullcol")) );

        FdoClassDefinitionP classDef = rdr->GetClassDefinition();
        FdoPropertiesP props = classDef->GetProperties();
        FdoDataPropertyP prop = (FdoDataPropertyDefinition*)(props->FindItem(phMgr->GetDcColumnName(L"NULLCOL")));
        CPPUNIT_ASSERT( prop != NULL );
        CPPUNIT_ASSERT( prop->GetLength() == 0 );
    }
}

FdoString* FdoUpdateTest::NoMetaSuffix()
{
    static FdoString* noMetaSuffix = L"_no_meta";

    return noMetaSuffix;
}


FdoString* FdoUpdateTest::MultiIdSuffix()
{
    static FdoString* multiIdSuffix = L"";

    return multiIdSuffix;
}

FdoStringP FdoUpdateTest::TableIdGeomName()
{
    // Can't create Oracle spatial index when table
    // has non-ASCII7 characters or spaces. 
    // Therefore no special characters in table name
    // tested by default.
    return L"table_id_geom";
}

FdoStringP FdoUpdateTest::TableNoIdNoGeomName()
{
    // Can't create Oracle spatial index when table
    // has non-ASCII7 characters or spaces. 
    // Therefore, for Oracle, test special characters
    // on table without geometry.
    return L"table noid nogeom";
}

FdoStringP FdoUpdateTest::Key2ColName()
{
    return L"key 2\x00e4";
}

FdoStringP FdoUpdateTest::ValueColName()
{
    return L"value \x00e4";
}

FdoStringP FdoUpdateTest::GeomColName()
{
    return UnitTestUtil::GetNlsObjectName(L"Geometry");
}

FdoStringP FdoUpdateTest::FixStringVal( FdoString* val )
{
    return val;
}

FdoPropertyValue* FdoUpdateTest::AddNewProperty( FdoPropertyValueCollection* propertyValues, const wchar_t *name )
{
    FdoPropertyValue*  propertyValue = NULL;
    propertyValue = propertyValues->FindItem( name );

    if ( propertyValue == NULL )
    {
        propertyValue =  FdoPropertyValue::Create();
        propertyValue->SetName( name );
        propertyValues->Add( propertyValue );
    }

    return propertyValue;
}

FdoInt64 FdoUpdateTest::GetSrid( FdoInt64 srid ) 
{
    return srid;
}