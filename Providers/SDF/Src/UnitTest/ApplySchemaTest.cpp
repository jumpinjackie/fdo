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

#include "ApplySchemaTest.h"
#include "UnitTestUtil.h"
/*
#include "XmlFormatter.h"
#include <Inc/Fdo/Rdbms/FdoIDescribeSchema.h>
#include <Inc/Fdo/Rdbms/FdoRdbmsDescribeSchemaMappingCommand.h>
#include <Inc/Fdo/Rdbms/FdoRdbmsDestroySchemaCommand.h>
*/
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

CPPUNIT_TEST_SUITE_REGISTRATION( ApplySchemaTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ApplySchemaTest, "ApplySchemaTest");

#ifdef _WIN32
static const wchar_t* APPLY_SCHEMA_TEST_FILE = L"..\\..\\TestData\\ApplySchemaTest.SDX";
static const wchar_t* APPLY_SCHEMA_COPY_FILE = L"..\\..\\TestData\\ApplySchemaCopy.SDX";
#else
#include <unistd.h>
static const wchar_t* APPLY_SCHEMA_TEST_FILE = L"../../TestData/ApplySchemaTest.SDX";
static const wchar_t* APPLY_SCHEMA_COPY_FILE = L"../../TestData/ApplySchemaCopy.SDX";
#endif


ApplySchemaTest::ApplySchemaTest(void)
{
}

ApplySchemaTest::~ApplySchemaTest(void)
{
}

void ApplySchemaTest::setUp ()
{
}

void ApplySchemaTest::TestSchema ()
{
	FdoPtr<FdoIConnection> connection;

    try {
		// delete, re-create and open the datastore
		printf( "Initializing Connection ... \n" );
		connection = UnitTestUtil::OpenConnection( APPLY_SCHEMA_TEST_FILE, true );
/*
        printf( "Deleting Acad Schema ... \n" );
		DeleteAcadSchema( connection);
*/
		printf( "Creating Acad Schema ... \n" );
		CreateAcadSchema( connection );

        printf( "Creating Electric Schema ... \n" );
   		CreateElectricSchema( connection );

        FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
        FdoFeatureSchemasP pCopySchemas = pDescCmd->Execute();

		printf( "Creating Error Schema ... \n" );
		CreateErrorSchema( connection );

		bool succeeded = false;

		printf( "Bad Acad schema create (already exists) ... \n" );
		try {
			CreateAcadSchema( connection );
			succeeded = true;
		}
		catch ( FdoSchemaException* e )
		{
            succeeded = false;
			UnitTestUtil::PrintException(e, "apply_schema_err1.txt", true );
			FDO_SAFE_RELEASE(e);
		}

		if ( succeeded ) 
			CPPUNIT_FAIL( "2nd Acad schema create was supposed to fail" );

        printf( "Modifying Electric Schema ... \n" );
   		ModElectricSchema( connection );

		printf( "Redefining a geometry ... \n" );
		RedefineGeometry( connection );

		printf( "Bad Land schema create (multiple schemas not supported) ... \n" );
		try {
    		CreateLandSchema( connection );
			succeeded = true;
		}
		catch ( FdoSchemaException* e )
		{
            succeeded = false;
			UnitTestUtil::PrintException(e, "apply_schema_err10.txt", true );
			FDO_SAFE_RELEASE(e);
		}

/*
		printf( "Deleting Properties with data ... \n" );
		DelPropertyError( connection );

        printf( "Writing 1st LogicalPhysical Schema ... \n" );

        mgr = staticConn->CreateSchemaManager();
        lp = mgr->RefLogicalPhysicalSchemas();
        lp->XMLSerialize( L"apply_schema_test1.xml" );

        printf( "Applying mixed updates and deletes ... \n" );
		ModDelSchemas( connection );
*/
		succeeded = false;

		printf( "Deleting Acad Schema ... \n" );
		try {
 			DeleteAcadSchema( connection );
			succeeded = true;
		}
		catch ( FdoSchemaException* e )
		{
			UnitTestUtil::PrintException(e, "apply_schema_err2.txt", true);
			FDO_SAFE_RELEASE(e);
		}

		if ( succeeded ) 
			CPPUNIT_FAIL( "2nd Acad schema delete was supposed to fail" );
/*
       	FdoPtr<FdoFeatureSchemaCollection> pCopySchemas2 = pDescCmd->Execute();

        printf( "Deleting Land Schema ... \n" );
		DeleteLandSchema( connection );

        printf( "Writing 2nd LogicalPhysical Schema ... \n" );
        mgr = staticConn->CreateSchemaManager();
        lp = mgr->RefLogicalPhysicalSchemas();
        lp->XMLSerialize( L"apply_schema_test2.xml" );

        printf( "Re-Adding some elements ... \n" );
 		ReAddElements(connection);

        printf( "Re-Adding Land Schema ... \n" );
		CreateLandSchema(connection);

		printf( "Modifying Land Schema ... \n" );
		ModLandSchema(connection);
*/
		printf( "Trying some invalid modifications ... \n" );

		ModErrors( connection );
		ModErrors2( connection );

		printf( "Testing long element names ... \n" );

        //TODO: element names are too long for other providers so just
        //test against Oracle for now
        CreateLongStringSchema( connection );

/*
        printf( "Writing 3rd LogicalPhysical Schema ... \n" );
        mgr = staticConn->CreateSchemaManager();
        lp = mgr->RefLogicalPhysicalSchemas();
        lp->XMLSerialize( L"apply_schema_test3.xml" );

		printf( "Testing Class Capabilities ... \n" );
		GetClassCapabilities( connection );

		printf( "Testing Base Properties ... \n" );
		CheckBaseProperties( connection );
*/

        FdoFeatureSchemasP pCopySchemas2 = pDescCmd->Execute();

        FdoIoFileStreamP outFile = FdoIoFileStream::Create( L"apply_schema_test1.xml", L"wt" );
        UnitTestUtil::ExportDb( connection, (FdoIoFileStream*) outFile );

        connection = NULL;

        CopySchemas( pCopySchemas, pCopySchemas2 );

		// Compare output files with expected results.


        UnitTestUtil::CheckOutput( "apply_schema_test1_master.xml", "apply_schema_test1.xml" );
        UnitTestUtil::CheckOutput( "apply_schema_test4_master.xml", "apply_schema_test4.xml" );
        UnitTestUtil::CheckOutput( "apply_schema_test5_master.xml", "apply_schema_test5.xml" );

        // Next do the error messages. Some checks are not done for some providers
        // since their expect output is different from the master
        // TODO: create special masters for these cases.

#ifdef _WIN32
        UnitTestUtil::CheckOutput( "apply_schema_err1_master.txt", "apply_schema_err1.txt" );
        UnitTestUtil::CheckOutput( "apply_schema_err2_master.txt", "apply_schema_err2.txt" );
        UnitTestUtil::CheckOutput( "apply_schema_err3_imaster.txt", "apply_schema_err3.txt" );
        UnitTestUtil::CheckOutput( "apply_schema_err4_master.txt", "apply_schema_err4.txt" );
        UnitTestUtil::CheckOutput( "apply_schema_err5_imaster.txt", "apply_schema_err5.txt" );
        UnitTestUtil::CheckOutput( "apply_schema_err6_imaster.txt", "apply_schema_err6.txt" );
        UnitTestUtil::CheckOutput( "apply_schema_err7_master.txt", "apply_schema_err7.txt" );
        UnitTestUtil::CheckOutput( "apply_schema_err10_master.txt", "apply_schema_err10.txt" );
#endif

    }
	catch ( FdoException* e ) 
	{
		UnitTestUtil::FailOnException( e );
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

void ApplySchemaTest::CreateAcadSchema( FdoIConnection* connection )
{
	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	FdoPtr<FdoFeatureSchema> pSchema = FdoFeatureSchema::Create( L"Acad", L"AutoCAD schema" );

	// A non-abstract base class

	FdoPtr<FdoFeatureClass> pEntClass = FdoFeatureClass::Create( L"AcDbEntity", L"AutoCAD entity base class" );
	pEntClass->SetIsAbstract(false);
    // Test unsetting geometry for class with no geometric properties - should have no effect.
    pEntClass->SetGeometryProperty(NULL);

	FdoDataPropertyP pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"id" );
	pProp->SetDataType( FdoDataType_Int32 );
	pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
	FdoPropertiesP(pEntClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pEntClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Layer", L"Acad layer" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(10);
	pProp->SetNullable(true);
	FdoPropertiesP(pEntClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"ColourIndex", L"Acad Colour" );
	pProp->SetDataType( FdoDataType_Byte );
	pProp->SetNullable(true);
	FdoPropertiesP(pEntClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pEntClass );

	// A sub-class to test property inheritance.

	FdoPtr<FdoFeatureClass> pPlineClass = FdoFeatureClass::Create( L"AcDb3dPolyline", L"AutoCAD 3d polyline" );
	pPlineClass->SetIsAbstract(false);
	pPlineClass->SetBaseClass( pEntClass );
	FdoClassesP(pSchema->GetClasses())->Add( pPlineClass );

	pProp = FdoDataPropertyDefinition::Create( L"Width", L"line width" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetPrecision(10);
	pProp->SetScale(5);
	pProp->SetNullable(false);
	FdoPropertiesP(pPlineClass->GetProperties())->Add( pProp );
/* TODO: support add property 
	pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();
*/
	pProp = FdoDataPropertyDefinition::Create( L"Closed", L"is first and last points the same" );
	pProp->SetDataType( FdoDataType_Boolean );
	pProp->SetNullable(true);
	FdoPropertiesP(pPlineClass->GetProperties())->Add( pProp );

	pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();

    // The following tests assocoatopm properties nesting to 3 levels.

	FdoPtr<FdoFeatureClass> pHatchClass = FdoFeatureClass::Create( L"AcDbHatch", L"AutoCAD hatched polygon" );
	pHatchClass->SetIsAbstract(false);
	pHatchClass->SetBaseClass( pEntClass );
#if 0
    FdoAssociationPropertyP pAssocProp = FdoAssociationPropertyDefinition::Create(L"Boundary", L"Hatch Boundary features");
    pAssocProp->SetLockCascade( true );
    pAssocProp->SetReverseName( L"Polygon" );
    pAssocProp->SetMultiplicity( L"m" );
    pAssocProp->SetReverseMultiplicity( L"0_1" );
    pAssocProp->SetAssociatedClass(pPlineClass);
	FdoPropertiesP(pHatchClass->GetProperties())->Add( pAssocProp );
#endif

	FdoClassesP(pSchema->GetClasses())->Add( pHatchClass );

	pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();

  	// Insert a row with null colour. Subsequent removal of colour property should succeed.
/*
    InsertObject( connection, false, L"Acad", L"AcDbEntity", L"Layer", L"default", NULL );
    InsertObject( connection, false, L"Acad", L"Entity", L"FeatId", L"1", L"Layer", L"default", NULL );
*/
}

void ApplySchemaTest::CreateElectricSchema( FdoIConnection* connection )
{
	FdoPtr<FdoIDescribeSchema> pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);

	pDescCmd->SetSchemaName( L"Acad" );
    
	FdoPtr<FdoFeatureSchemaCollection> pSchemas = pDescCmd->Execute();
	FdoPtr<FdoFeatureSchema> pAcadSchema = pSchemas->GetItem( L"Acad" );

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	/* A schema with dictionary */

	FdoPtr<FdoFeatureSchema> pSchema = pAcadSchema;
    // Multiple schema not supported - FdoFeatureSchema::Create( L"Electric'l", L"Electrical '' schema'" );
	//pAcadSchema->Add( pSchema );

	FdoSADP(pSchema->GetAttributes())->Add( L"'Author", L"Thomas O'Edison" );

	/* An abstract base class */

	FdoPtr<FdoFeatureClass> pDevClass = FdoFeatureClass::Create( L"ElectricDevice", L"electic base class" );
	pDevClass->SetIsAbstract(true);

	FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"id" );
	pProp->SetDataType( FdoDataType_Int32 );
	pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
	FdoPropertiesP(pDevClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pDevClass->GetIdentityProperties())->Add( pProp );

	// Test geometry property

	FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"location and shape" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve );
	pGeomProp->SetHasElevation(true);
    FdoPropertiesP(pDevClass->GetProperties())->Add( pGeomProp );

	pDevClass->SetGeometryProperty( pGeomProp );

	FdoClassesP(pSchema->GetClasses())->Add( pDevClass );

	/* Subclass with dictionary */

	FdoPtr<FdoFeatureClass> pClass = FdoFeatureClass::Create( L"Transformer", L"" );
	pClass->SetIsAbstract(false);
	pClass->SetBaseClass( pDevClass );

	// Add data properties of various types

	pProp = FdoDataPropertyDefinition::Create( L"Voltage", L"voltage" );
	pProp->SetDataType( FdoDataType_Decimal );
	pProp->SetPrecision(10);
	pProp->SetScale(1);
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Phase", L"A, B or C" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(1);
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"InstallDate", L"" );
	pProp->SetDataType( FdoDataType_DateTime );
	pProp->SetNullable(true);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"LastInspectDate", L"" );
	pProp->SetDataType( FdoDataType_DateTime );
	pProp->SetNullable(true);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"LastRepairDate", L"" );
	pProp->SetDataType( FdoDataType_DateTime );
	pProp->SetNullable(true);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"PartNum", L"" );
	pProp->SetDataType( FdoDataType_Int16 );
	pProp->SetNullable(true);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Volume", L"" );
	pProp->SetDataType( FdoDataType_Single );
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"SerialNum", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

	pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();
}

void ApplySchemaTest::CreateLongStringSchema( FdoIConnection* connection )
{
	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

    /* Test various long schema, class and property names. */

//	FdoPtr<FdoFeatureSchema> pSchema = FdoFeatureSchema::Create( L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );
	FdoPtr<FdoIDescribeSchema> pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	FdoPtr<FdoFeatureSchemaCollection> pSchemas = pDescCmd->Execute();
	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Acad" );

    FdoPtr<FdoFeatureClass> pBaseClass = FdoFeatureClass::Create( L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );
	pBaseClass->SetIsAbstract(false);

	FdoDataPropertyP pProp = FdoDataPropertyDefinition::Create( L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	pProp->SetLength(50);

	FdoPropertiesP(pBaseClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pBaseClass->GetIdentityProperties())->Add( pProp );

    FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = FdoGeometricPropertyDefinition::Create( L"gbcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );
    pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve );
	FdoPropertiesP(pBaseClass->GetProperties())->Add( pGeomProp );

	pBaseClass->SetGeometryProperty( pGeomProp );

	FdoClassesP(pSchema->GetClasses())->Add( pBaseClass );

    // Test creating a sub-class of a class with long name

	FdoPtr<FdoFeatureClass> pClass = FdoFeatureClass::Create( L"bbcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789", L"abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef123456789" );
	pClass->SetIsAbstract(false);
    pClass->SetBaseClass(pBaseClass);

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

    pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();
}

void ApplySchemaTest::CreateLandSchema( FdoIConnection* connection )
{
	FdoPtr<FdoIDescribeSchema> pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);

	pDescCmd->SetSchemaName( L"Acad" );

	FdoPtr<FdoFeatureSchemaCollection> pSchemas = pDescCmd->Execute();

    CreateLandSchema( pSchemas );

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem(L"Land");

    pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();

	// Test GetFeatureSchema
    CPPUNIT_ASSERT(wcscmp(FdoFeatureSchemaP(pCmd->GetFeatureSchema())->GetName(), L"Land") == 0);

//    InsertObject(connection, false, L"Land", L"1-8 School", L"# Rooms", L"20", NULL );
//    InsertObject(connection, false, L"Land", L"Driveway", L"Pav'd", L"1", NULL );
//	UnitTestUtil::Sql2Db( L"insert into parcel_person ( first_name, last_name, parcel_province, parcel_pin ) values ( 'Fraser', 'Simon', 'Ontario', '1234-5678' )", connection );
}

void ApplySchemaTest::CreateLandSchema( FdoFeatureSchemaCollection* pSchemas )
{
	FdoPtr<FdoFeatureSchema> pAcadSchema = pSchemas->GetItem(L"Acad");
	FdoPtr<FdoClassDefinition> pEntityClass = FdoClassesP(pAcadSchema->GetClasses())->GetItem(L"AcDbEntity");

    /* Create a schema to test successful schema deletion */

	FdoPtr<FdoFeatureSchema> pSchema = FdoFeatureSchema::Create( L"Land", L"Property schema" );
	pSchemas->Add( pSchema );

	FdoPtr<FdoClass> pPersonClass = FdoClass::Create( L"Person", L"" );
	pPersonClass->SetIsAbstract(false);

	FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"First Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	pProp->SetLength(50);
	FdoPropertiesP(pPersonClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Last Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	pProp->SetLength(50);
	FdoPropertiesP(pPersonClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pPersonClass );

	FdoPtr<FdoFeatureClass> pDrvClass = FdoFeatureClass::Create( L"Driveway", L"" );
	pDrvClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( L"FeatureId", L"" );
	pProp->SetDataType( FdoDataType_Int32 );
	pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
	FdoPropertiesP(pDrvClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pDrvClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Pav'd", L"'''" );
	pProp->SetDataType( FdoDataType_Boolean );
	pProp->SetNullable(false);
	FdoPropertiesP(pDrvClass->GetProperties())->Add( pProp );

	FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry'", L"location's" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Curve );
    pGeomProp->SetHasMeasure(true);
	FdoPropertiesP(pDrvClass->GetProperties())->Add( pGeomProp );
	pDrvClass->SetGeometryProperty(pGeomProp);

    // Test adding second geometric property.
	pGeomProp = FdoGeometricPropertyDefinition::Create( L"LabelPoint", L"secondary geometry" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point  );
	FdoPropertiesP(pDrvClass->GetProperties())->Add( pGeomProp );

	FdoClassesP(pSchema->GetClasses())->Add( pDrvClass );

	/* Test feature class with id properties not FeatId */

	FdoPtr<FdoFeatureClass> pClass = FdoFeatureClass::Create( L"Parcel", L"land parcel" );
	pClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( L"Province", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(20);
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"PIN", L"parcel id" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(15);
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Value", L"" );
	pProp->SetDataType( FdoDataType_Decimal );
	pProp->SetNullable(true);
	pProp->SetPrecision( 8 );
	pProp->SetScale( 0 );
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	FdoPtr<FdoObjectPropertyDefinition> pObjProp = FdoObjectPropertyDefinition::Create( L"owner", L"" );
	pObjProp->SetObjectType( FdoObjectType_Value );
	pObjProp->SetClass( pPersonClass );
	FdoPropertiesP(pClass->GetProperties())->Add( pObjProp );

    // Test adding geometric property that is not the geometry property.
	pGeomProp = FdoGeometricPropertyDefinition::Create( L"Grading", L"secondary geometry" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Surface  );
	FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

	/* Create class with base class in different schema */

	pClass = FdoFeatureClass::Create( L"Cogo Point", L"'Surveyor's point" );
	pClass->SetIsAbstract(false);
	pClass->SetBaseClass( pEntityClass );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

    FdoPtr<FdoFeatureClass> pBldgClass = FdoFeatureClass::Create( L"Build'g", L"'" );
	pBldgClass->SetIsAbstract(true);

	pProp = FdoDataPropertyDefinition::Create( L"FeatureId", L"" );
	pProp->SetDataType( FdoDataType_Int32 );
	pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
	FdoPropertiesP(pBldgClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pBldgClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"# Rooms", L"" );
	pProp->SetDataType( FdoDataType_Int16 );
	pProp->SetNullable(true);
	FdoPropertiesP(pBldgClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"% Occupied", L"" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(true);
	FdoPropertiesP(pBldgClass->GetProperties())->Add( pProp );

	// Add a property with same name as a system property except for capitalization.
	// Since columns are case-insensitve, this property's column should end up being
	// named "CLASSNAME1" so that the column doesn't get accidently matched to the 
	// "ClassName" system property as well.
	pProp = FdoDataPropertyDefinition::Create( L"classname", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(255);
	pProp->SetNullable(true);
	FdoPropertiesP(pBldgClass->GetProperties())->Add( pProp  );

	pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point );
    pGeomProp->SetHasElevation(true);
    pGeomProp->SetHasMeasure(true);
	FdoPropertiesP(pBldgClass->GetProperties())->Add( pGeomProp );
	pBldgClass->SetGeometryProperty(pGeomProp);

	FdoClassesP(pSchema->GetClasses())->Add( pBldgClass );

	pClass = FdoFeatureClass::Create( L"1-8 School", L"" );
	pClass->SetIsAbstract(false);
	pClass->SetBaseClass( pBldgClass );
    // Test setting geometry property to an inherited geometric property.
    pClass->SetGeometryProperty(pGeomProp);

	pProp = FdoDataPropertyDefinition::Create( L"# Occupied", L"" );
	pProp->SetDataType( FdoDataType_Int16 );
	pProp->SetNullable(true);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

    /* Test feature class with a non-FeatId autogenerated id property */

	pClass = FdoFeatureClass::Create( L"Zoning", L"land use zone" );
	pClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( L"ByLaw", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(20);
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"PolyNum", L"polygon id" );
	pProp->SetDataType( FdoDataType_Int32 );
	pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
    FdoPropertiesP(pClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Authority", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(30);
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"ZoningType", L"" );
	pProp->SetDataType( FdoDataType_Int32 );
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );
}

void ApplySchemaTest::CreateErrorSchema( FdoIConnection* connection )
{
	FdoPtr<FdoIDescribeSchema> pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	FdoPtr<FdoFeatureSchemaCollection> pSchemas = pDescCmd->Execute();
	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Acad" );

    bool succeeded = false;
	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	/* Create schema with errors */

	/* First create a schema with some bad base classes */

	FdoPtr<FdoFeatureSchema> pGhostSchema = FdoFeatureSchema::Create( L"Acad", L"Not in database" );

	// AcDbEntity with different class type than one in datastore.

	FdoPtr<FdoClass> pEntity = FdoClass::Create( L"AcDbEntity", L"" );
	pEntity->SetIsAbstract(false);

	FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"ID", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
	FdoPropertiesP(pEntity->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pEntity->GetIdentityProperties())->Add( pProp );

	// Base class that isn't in datastore.

	FdoPtr<FdoClass> pGhostClass = FdoClass::Create( L"Ghost", L"" );

	pProp = FdoDataPropertyDefinition::Create( L"ID", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
	FdoPropertiesP(pGhostClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pGhostClass->GetIdentityProperties())->Add( pProp );

	// Orphan base and object property class

	FdoPtr<FdoClass> pOrphanClass = FdoClass::Create( L"Orphan Class", L"" );
	FdoPtr<FdoClass> pOrphanSubClass = FdoClass::Create( L"Orphan SubClass", L"" );
	pOrphanSubClass->SetBaseClass( pOrphanClass );

	// Create class whose class type differs from base class.

	FdoPtr<FdoClass> pBaseConf = FdoClass::Create( L"Base Conflict", L"" );
	pBaseConf->SetBaseClass( pEntity );

	// Create class whose base class will not be in the database.

	FdoPtr<FdoClass> pBaseMissing = FdoClass::Create( L"Base Missing", L"" );
	pBaseMissing->SetBaseClass( pGhostClass );

	// Create some invalid data properties

	pProp = FdoDataPropertyDefinition::Create( L"type not autogen supported", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength( 5 );
	pProp->SetIsAutoGenerated( true );
	FdoPropertiesP(pBaseMissing->GetProperties())->Add( pProp );

	// Blob and Clob ( not supported )

	pProp = FdoDataPropertyDefinition::Create( L"Blob Prop", L"" );
	pProp->SetDataType( FdoDataType_BLOB );
	pProp->SetLength( 20000 );
	FdoPropertiesP(pBaseMissing->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Clob Prop", L"" );
	pProp->SetDataType( FdoDataType_CLOB );
	pProp->SetLength( 20000 );
	FdoPropertiesP(pBaseMissing->GetProperties())->Add( pProp );

	// Class with 1 nullable id property

	FdoPtr<FdoFeatureClass> pNullIds1 = FdoFeatureClass::Create( L"NullIds1", L"" );

    // Set Geometry Property to Geometric property that doesn't belong to the class.
   	FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Curve );
	pNullIds1->SetGeometryProperty(pGeomProp);

    // Add class that already exists.

	FdoPtr<FdoFeatureClass> pDevClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"ElectricDevice"));
	FdoClassesP(pSchema->GetClasses())->Remove( pDevClass );
    pDevClass = FdoFeatureClass::Create( L"ElectricDevice", L"electic base class" );
	pDevClass->SetIsAbstract(true);

	pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"id" );
	pProp->SetDataType( FdoDataType_Int32 );
	pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
	FdoPropertiesP(pDevClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pDevClass->GetIdentityProperties())->Add( pProp );

	// Test geometry property

	pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"location and shape" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve );
	pGeomProp->SetHasElevation(true);
    FdoPropertiesP(pDevClass->GetProperties())->Add( pGeomProp );

	pDevClass->SetGeometryProperty( pGeomProp );

//TODO: are these errors for SDF?

    pProp = FdoDataPropertyDefinition::Create( L"Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pNullIds1->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pNullIds1->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Data", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pNullIds1->GetProperties())->Add( pProp );

	// Class with multiple nullable id properties

	FdoPtr<FdoFeatureClass> pNullIds2 = FdoFeatureClass::Create( L"NullIds2", L"" );

	pProp = FdoDataPropertyDefinition::Create( L"First Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pNullIds2->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pNullIds2->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Middle Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pNullIds2->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pNullIds2->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Last Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pNullIds2->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pNullIds2->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Data", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pNullIds2->GetProperties())->Add( pProp );

	// Class with readonly id properties

	FdoPtr<FdoFeatureClass> pReadOnlyId = FdoFeatureClass::Create( L"ReadOnlyId", L"" );

	pProp = FdoDataPropertyDefinition::Create( L"First Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	pProp->SetReadOnly(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pReadOnlyId->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pReadOnlyId->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Middle Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pReadOnlyId->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pReadOnlyId->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Last Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	pProp->SetReadOnly(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pReadOnlyId->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pReadOnlyId->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Data", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pReadOnlyId->GetProperties())->Add( pProp );

	// A feature class without identity properties.

	FdoPtr<FdoFeatureClass> pFeatNoId = FdoFeatureClass::Create( L"FeatNoId", L"" );
	pProp = FdoDataPropertyDefinition::Create( L"Data", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pFeatNoId->GetProperties())->Add( pProp );

	// A feature class based on a class without identity properties

	FdoPtr<FdoFeatureClass> pSubFeatNoId = FdoFeatureClass::Create( L"SubFeatNoId", L"" );
	pProp = FdoDataPropertyDefinition::Create( L"Dataw", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(true);
	pProp->SetLength( 20 );
	FdoPropertiesP(pSubFeatNoId->GetProperties())->Add( pProp );

	FdoClassesP(pGhostSchema->GetClasses())->Add( pEntity );
	FdoClassesP(pGhostSchema->GetClasses())->Add( pGhostClass);
	FdoClassesP(pSchema->GetClasses())->Add( pBaseConf );
	FdoClassesP(pSchema->GetClasses())->Add( pBaseMissing );
	FdoClassesP(pSchema->GetClasses())->Add( pOrphanSubClass );
	FdoClassesP(pSchema->GetClasses())->Add( pNullIds1 );
	FdoClassesP(pSchema->GetClasses())->Add( pNullIds2 );
	FdoClassesP(pSchema->GetClasses())->Add( pReadOnlyId );
	FdoClassesP(pSchema->GetClasses())->Add( pFeatNoId );
	FdoClassesP(pSchema->GetClasses())->Add( pSubFeatNoId );
	FdoClassesP(pSchema->GetClasses())->Add( pDevClass );


	pCmd->SetFeatureSchema( pSchema );

	try {
		pCmd->Execute();
		succeeded = true;
	}
	catch ( FdoSchemaException* e )
	{
		UnitTestUtil::PrintException(e, "apply_schema_err3.txt", true);
		FDO_SAFE_RELEASE(e);
	}

	CPPUNIT_ASSERT( !succeeded );

	/* Test modifying a non-existent schema */

	pSchemas = FdoFeatureSchemaCollection::Create(NULL);
	pSchema = FdoFeatureSchema::Create( L"Non Existent", L"" );
	pSchemas->Add(pSchema);
	pSchema->AcceptChanges();
	pSchema->SetDescription( L"Modify non-existent schema" );

	pCmd->SetFeatureSchema( pSchema );

	try {
		pCmd->Execute();
		succeeded = true;
	}
	catch ( FdoSchemaException* e )
	{
		UnitTestUtil::PrintException(e, "apply_schema_err4.txt", true);
		FDO_SAFE_RELEASE(e);
	}

	CPPUNIT_ASSERT( !succeeded );

}

void ApplySchemaTest::DeleteAcadSchema( FdoIConnection* connection )
{
	FdoPtr<FdoIDescribeSchema> pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);

	FdoPtr<FdoFeatureSchemaCollection> pSchemas = pDescCmd->Execute();
	FdoPtr<FdoFeatureSchema> pAcadSchema = pSchemas->GetItem( L"Acad" );
    pAcadSchema->Delete();

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
    pCmd->SetFeatureSchema( pAcadSchema );
    pCmd->Execute();
}
void ApplySchemaTest::ModElectricSchema( FdoIConnection* connection )
{
	/* Test modifying an existing schema */

	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
//	pDescCmd->SetSchemaName( L"Electric'l" );
	pDescCmd->SetSchemaName( L"Acad" );
	FdoFeatureSchemasP pSchemas = pDescCmd->Execute();

    ModElectricSchema( pSchemas );

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

//	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Electric'l" );
	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Acad" );
	pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();
}

void ApplySchemaTest::ModElectricSchema( FdoFeatureSchemaCollection* pSchemas )
{
	/* Test modifying an existing schema */

//	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Electric'l" );
	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Acad" );

	// Update Geometry property

	FdoPtr<FdoFeatureClass> pDevClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"ElectricDevice"));
/*
    FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = (FdoGeometricPropertyDefinition*) FdoPropertiesP(pDevClass->GetProperties())->GetItem(L"Geometry");
	pGeomProp->SetDescription( L"updated geometry description" );

	// Add the nested object property. Since added to base class, this also tests inheritance.

	pObjProp = FdoObjectPropertyDefinition::Create( L"maintenance history", L"maintenance history for this device" );
	pObjProp->SetClass( pMaintHist );
	pObjProp->SetIdentityProperty( pHistId );
	pObjProp->SetObjectType( FdoObjectType_Collection );
	FdoPropertiesP(pDevClass->GetProperties())->Add( pObjProp );

    // Add properties already defined on subclass (Transformer)

	pProp = FdoDataPropertyDefinition::Create( L"InstallDate", L"" );
	pProp->SetDataType( FdoDataType_DateTime );
	pProp->SetNullable(true);
	FdoPropertiesP(pDevClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"LastInspectDate", L"" );
	pProp->SetDataType( FdoDataType_DateTime );
	pProp->SetNullable(true);
	FdoPropertiesP(pDevClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"LastRepairDate", L"modified description" );
	pProp->SetDataType( FdoDataType_DateTime );
	pProp->SetNullable(true);
	pProp->SetReadOnly(true);
	FdoPropertiesP(pDevClass->GetProperties())->Add( pProp );

	FdoPtr<FdoFeatureClass> pClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"Transformer"));
	
	// Delete property that is also being added to base class ( property should stay but become inherited )
	pProp = (FdoDataPropertyDefinition*) FdoPropertiesP( pClass->GetProperties() )->GetItem( L"InstallDate" );
	pProp->Delete();

	// Modify property that is also being added to base class.
	pProp = (FdoDataPropertyDefinition*) FdoPropertiesP( pClass->GetProperties() )->GetItem( L"LastRepairDate" );
	pProp->SetDescription( L"modified description" );
*/
	// Add a new sub-class to the schema

	FdoFeatureClassP pClass = FdoFeatureClass::Create( L"Conductor", L"" );
	pClass->SetIsAbstract(false);
	pClass->SetBaseClass( pDevClass );

	FdoDataPropertyP pProp = FdoDataPropertyDefinition::Create( L"underground", L"" );
	pProp->SetDataType( FdoDataType_Boolean );
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

	// Add a new non-inherited class that will later be removed.

	pClass = FdoFeatureClass::Create( L"Pole", L"" );
	pClass->SetIsAbstract(false);

   	FdoGeometricPropertyP pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point );
	FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );
	pClass->SetGeometryProperty(pGeomProp);

	pProp = FdoDataPropertyDefinition::Create( L"FeatureId", L"" );
	pProp->SetDataType( FdoDataType_Int32 );
	pProp->SetIsAutoGenerated(true);
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Height", L"" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

	FdoPtr<FdoClass> pStClass = FdoClass::Create( L"Street", L"" );
	pStClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( L"Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength( 30 );
	pProp->SetNullable(false);
	FdoPropertiesP(pStClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pStClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Type", L"" );
	pProp->SetDataType( FdoDataType_Int32 );
	pProp->SetNullable(false);
	FdoPropertiesP(pStClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pStClass );

	FdoPtr<FdoClass> pAddrClass = FdoClass::Create( L"'Address", L"" );
	pAddrClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( L"Number", L"" );
	pProp->SetDataType( FdoDataType_Int32 );
	pProp->SetNullable(false);
	FdoPropertiesP(pAddrClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pAddrClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Street", L"" );
	pProp->SetDataType( FdoDataType_String );
    pProp->SetLength(100);
	pProp->SetNullable(false);
	FdoPropertiesP(pAddrClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pAddrClass );

	FdoPtr<FdoClass> pEmpClass = FdoClass::Create( L"Employee", L"" );
	pEmpClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( L"First Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength( 30 );
	pProp->SetNullable(false);
	FdoPropertiesP(pEmpClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pEmpClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Last Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength( 30 );
	pProp->SetNullable(false);
	FdoPropertiesP(pEmpClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pEmpClass->GetIdentityProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pEmpClass );

    // Create customer plus sub-classes, used later on for id property modification
    // tests

	FdoPtr<FdoClass> pCustClass = FdoClass::Create( L"Customer", L"" );
	pCustClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( L"First Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength( 30 );
	pProp->SetNullable(false);
	FdoPropertiesP(pCustClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pCustClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Last Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength( 30 );
	pProp->SetNullable(false);
	FdoPropertiesP(pCustClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pCustClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Credit Rating", L"" );
	pProp->SetDataType( FdoDataType_Byte );
	pProp->SetNullable(true);
    pProp->SetReadOnly(true);
	FdoPropertiesP(pCustClass->GetProperties())->Add( pProp );

    FdoClassesP(pSchema->GetClasses())->Add( pCustClass );

	FdoPtr<FdoClass> pResClass = FdoClass::Create( L"Customer - Residential", L"" );
	pResClass->SetIsAbstract(false);
    pResClass->SetBaseClass(pCustClass);
	FdoClassesP(pSchema->GetClasses())->Add( pResClass );

	FdoPtr<FdoClass> pBusClass = FdoClass::Create( L"Customer - Business", L"" );
	pBusClass->SetIsAbstract(false);
    pBusClass->SetBaseClass(pCustClass);
	FdoClassesP(pSchema->GetClasses())->Add( pBusClass );
}

#if 0
void ApplySchemaTest::ModLandSchema( FdoIConnection* connection )
{
	/* 
     * Test deleting a geometric property that is the geometry property of a 
     * subclass
     */

	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	pDescCmd->SetSchemaName( L"Land" );
	FdoFeatureSchemasP pSchemas = pDescCmd->Execute();

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Land" );

	FdoPtr<FdoFeatureClass> pClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"Driveway"));
	pClass->SetGeometryProperty(NULL);

    pCmd->SetFeatureSchema( pSchema );
  	pCmd->Execute();
}
#endif
void ApplySchemaTest::RedefineGeometry( FdoIConnection* connection )
{
	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
//	pDescCmd->SetSchemaName( L"Electric'l" );
	pDescCmd->SetSchemaName( L"Acad" );
	FdoPtr<FdoFeatureSchemaCollection> pSchemas = pDescCmd->Execute();

//	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Electric'l" );
	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Acad" );

	FdoPtr<FdoFeatureClass> pClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Conductor" );

	/* Try to redefine inherited geometry property */

	FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = FdoGeometricPropertyDefinition::Create( L"Conduit", L"Pipe Geometry" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Curve );
	FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );
	pClass->SetGeometryProperty( pGeomProp );

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
	pCmd->SetFeatureSchema( pSchema );

	bool succeeded = false;
	try {
		pCmd->Execute();
		succeeded = true;
	}
	catch ( FdoSchemaException* e )
	{
		UnitTestUtil::PrintException(e, "apply_schema_err7.txt", true);
		FDO_SAFE_RELEASE(e);
	}

	if ( succeeded ) 
		CPPUNIT_FAIL( "Invalid modifications were supposed to fail" );
}
#if 0
void ApplySchemaTest::DelPropertyError( FdoIConnection* connection )
{
	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	pDescCmd->SetSchemaName( L"Land" );
	FdoPtr<FdoFeatureSchemaCollection> pSchemas = pDescCmd->Execute();

	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Land" );

	FdoPtr<FdoFeatureClass> pClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Build'g" );
	pClass->SetGeometryProperty(NULL);

	FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = (FdoGeometricPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->GetItem( L"Geometry" );
	pGeomProp->Delete();

	FdoPtr<FdoDataPropertyDefinition> pDataProp = (FdoDataPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->GetItem( L"# Rooms" );
	pDataProp->Delete();

	pClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Driveway" );
	pClass->SetGeometryProperty(NULL);

	pGeomProp = (FdoGeometricPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->GetItem( L"Geometry'" );
	pGeomProp->Delete();

	pDataProp = (FdoDataPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->GetItem( L"Pav'd" );
	pDataProp->Delete();

    // Set geometry property to deleted geometric property.

	pClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Parcel" );

    pGeomProp = (FdoGeometricPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->GetItem( L"Grading" );
	pGeomProp->Delete();
    pClass->SetGeometryProperty(pGeomProp);

    FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
	pCmd->SetFeatureSchema( pSchema );

	// Try to delete an object property that has data.
	pClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Parcel" );
	FdoPtr<FdoObjectPropertyDefinition> pObjProp = (FdoObjectPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->GetItem( L"owner" );
	pObjProp->Delete();

	bool succeeded = false;
	try {
		pCmd->Execute();
		succeeded = true;
	}
	catch ( FdoSchemaException* e )
	{
		UnitTestUtil::PrintException(e, SchemaTestErrFile(8,false), true);
		FDO_SAFE_RELEASE(e);
	}

	if ( succeeded ) 
		CPPUNIT_FAIL( "Invalid modifications were supposed to fail" );

   	UnitTestUtil::Sql2Db( L"delete from a1_8_school", connection );

    // Retest with empty school table. This time we shouldn't get the
    // "school has rows" error message when Build'g.Geometry is deleted.
    // We should just get the message that it is the geometry property
    // for 1-8 Schools.

	succeeded = false;
	try {
		pCmd->Execute();
		succeeded = true;
	}
	catch ( FdoSchemaException* e )
	{
		UnitTestUtil::PrintException(e, SchemaTestErrFile(9,false), true);
		FDO_SAFE_RELEASE(e);
	}

	if ( succeeded ) 
		CPPUNIT_FAIL( "Invalid modifications were supposed to fail" );

}

void ApplySchemaTest::ModDelSchemas( FdoIConnection* connection )
{
	/* Test some more modifications plus deletions. */

	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	FdoFeatureSchemasP pSchemas = pDescCmd->Execute();

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Electric'l" );
    ModDelElectricSchema( pSchemas );

	pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();

	pSchema = pSchemas->GetItem( L"Acad" );

    ModDelAcadSchema( pSchemas );
	pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();
}

void ApplySchemaTest::ModDelElectricSchema( FdoFeatureSchemaCollection* pSchemas )
{
	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Electric'l" );

	// Dictionary elemetn delete

	FdoSADP(pSchema->GetAttributes())->Remove( L"'Author" );

	// Class delete

	FdoPtr<FdoFeatureClass> pClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"Pole"));
	pClass->Delete();

	// Geometric and Object Property delete. Also tests rippling to sub-classes of ElectricDevice.

	pClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"ElectricDevice"));
	FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = (FdoGeometricPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->GetItem(L"Geometry");
	pGeomProp->Delete();
    pClass->SetGeometryProperty(NULL);

	FdoPtr<FdoObjectPropertyDefinition> pObjProp = (FdoObjectPropertyDefinition*) (FdoPropertiesP(pClass->GetProperties())->GetItem(L"maintenance history"));
	pObjProp->Delete();

	// Complex deletions. Delete top class plus class for nested object property while leaving
	// class for top object property

    FdoPtr<FdoClass> pClsClass = (FdoClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"Employee"));
	pClsClass->Delete();

	pClsClass = (FdoClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"'Address"));

    pObjProp = (FdoObjectPropertyDefinition*) (FdoPropertiesP(pClsClass->GetProperties())->GetItem(L"street"));
	pObjProp->Delete();

	pClsClass = (FdoClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"Street"));
	pClsClass->Delete();

	pClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"Transformer"));

    FdoPtr<FdoDataPropertyDefinition> pProp = (FdoDataPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->GetItem( L"Volume" );
	pProp->Delete();

	pProp = FdoDataPropertyDefinition::Create( L"Temperature", L"" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(true);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
}

void ApplySchemaTest::ModDelAcadSchema( FdoFeatureSchemaCollection* pSchemas )
{
	FdoFeatureSchemaP pSchema = pSchemas->GetItem( L"Acad" );
	pSchema->SetDescription( L"A'CAD Entity Schema" );

	FdoFeatureClassP pClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"AcDbEntity"));
	FdoPtr<FdoClass> pRefClass = (FdoClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"Entity"));

	// Test delete of property that has values but all values are null.

    FdoDataPropertyP pProp = (FdoDataPropertyDefinition*) (FdoPropertiesP(pClass->GetProperties())->GetItem(L"ColourIndex"));
	pProp->Delete();

	pProp = (FdoDataPropertyDefinition*) (FdoPropertiesP(pRefClass->GetProperties())->GetItem(L"ColourIndex"));
	pProp->Delete();

	// Test adding data property to existing class.

    pProp = FdoDataPropertyDefinition::Create( L"Plot Style", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(20);
	pProp->SetNullable(true);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Plot Style", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(20);
	pProp->SetNullable(true);
	FdoPropertiesP(pRefClass->GetProperties())->Add( pProp );

	// Modify data property

	pProp = (FdoDataPropertyDefinition*) (FdoPropertiesP(pClass->GetProperties())->GetItem(L"Layer"));
	pProp->SetDescription( L"Entity's Classification" );

	pProp = (FdoDataPropertyDefinition*) (FdoPropertiesP(pRefClass->GetProperties())->GetItem(L"Layer"));
	pProp->SetDescription( L"Entity's Classification" );

	// Modify object property
	FdoObjectPropertyP pObjProp = (FdoObjectPropertyDefinition*) (FdoPropertiesP(pClass->GetProperties())->GetItem(L"xdata"));
	pObjProp->SetDescription( L"new xdata description" );

	pObjProp = (FdoObjectPropertyDefinition*) (FdoPropertiesP(pRefClass->GetProperties())->GetItem(L"xdata"));
	pObjProp->SetDescription( L"new xdata description" );

	// Modify class description

	FdoPtr<FdoClass> pXData = (FdoClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"AcXData"));
	pXData->SetDescription( L"Application's Data" );

	// The following verifies that the unique table name generator does not generated a table name
	// for an existing table, or one referenced by the metaschema.

	FdoPtr<FdoClass> pCoordVal = (FdoClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"AcDbVertexCoordinateValue"));
	FdoPtr<FdoClass> pVertex = (FdoClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"AcDbVertexData"));
	FdoPtr<FdoDataPropertyDefinition> pCoordValSeq = (FdoDataPropertyDefinition*) (FdoPropertiesP(pCoordVal->GetProperties())->GetItem(L"Seq"));

	pObjProp = FdoObjectPropertyDefinition::Create( L"normal", L"" );
	pObjProp->SetClass( pCoordVal );
	pObjProp->SetIdentityProperty( pCoordValSeq );
	pObjProp->SetObjectType( FdoObjectType_Collection );
	FdoPropertiesP(pVertex->GetProperties())->Add( pObjProp );

	// Add an id-less class with same spelling ( but different case ) than
	// an existing id-less class. This class should get a different table
	// name than the other class.

    FdoStringP provider = UnitTestUtil::GetEnv("provider","Oracle");

    // TODO: the SqlServer instance on seconds is case insensitive, so skip the 
    // case sensitive class name test since it will fail with duplicate index error
    // on f_classdefinition. 
    if ( provider != L"SqlServer" ) {
	    FdoPtr<FdoClass> pCaseClass = FdoClass::Create( L"aCxdATA", L"Xdata" );
	    pCaseClass->SetIsAbstract(false);

	    pProp = FdoDataPropertyDefinition::Create( L"Seq", L"seq" );
	    pProp->SetDataType( FdoDataType_Int32 );
	    pProp->SetNullable(false);
	    FdoPropertiesP(pCaseClass->GetProperties())->Add( pProp  );

	    FdoClassesP(pSchema->GetClasses())->Add( pCaseClass );
    }
}

void ApplySchemaTest::ReAddElements( FdoIConnection* connection )
{
    FdoStringP provider = UnitTestUtil::GetEnv("provider","Oracle");

    FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	pDescCmd->SetSchemaName( L"Electric'l" );
	FdoPtr<FdoFeatureSchemaCollection> pSchemas = pDescCmd->Execute();

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Electric'l" );

	// Re-add a feature class that was previously deleted. This tests that the Pole class delete actually
	// did remove the Pole table.

	FdoPtr<FdoFeatureClass> pClass = FdoFeatureClass::Create( L"Pole", L"" );
	pClass->SetIsAbstract(false);

	FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"" );
	pProp->SetDataType( FdoDataType_Int32 );
	pProp->SetIsAutoGenerated(true);
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Height", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	pProp->SetLength(30);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

	// re-add a non-feature class.

	FdoPtr<FdoClass> pStClass = FdoClass::Create( L"Street", L"" );
	pStClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( L"Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength( 30 );
	pProp->SetNullable(false);
	FdoPropertiesP(pStClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Type", L"" );
	pProp->SetDataType( FdoDataType_Int32 );
	pProp->SetNullable(false);
	FdoPropertiesP(pStClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pStClass );

	pClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Transformer" );

	// Re-add deleted geometry

	FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"location and shape" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Surface );
	FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );
	pClass->SetGeometryProperty( pGeomProp );

    // Try geometry with similar name but different case (should get different column.
    // Skip sqlserver since seconds is data case-insensitive.
    if ( provider != L"SqlServer" ) {
	    pGeomProp = FdoGeometricPropertyDefinition::Create( L"GEOMETRY", L"location and shape" );
	    pGeomProp->SetGeometryTypes( FdoGeometricType_Surface );
	    FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );
    }

	// Re-add deleted data property. Give it different type and nullibility than before. 
	// Note that "volume" column is still around ( the provider doesn't yet support column deletes )
	// so the following creates a "volume1" column.
/*
	pProp = FdoDataPropertyDefinition::Create( L"Volume", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(true);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
*/
	// Re-add a nested object property. Since added to base class, this also tests inheritance.

	FdoPtr<FdoClassDefinition> pMaintHist = FdoClassesP(pSchema->GetClasses())->GetItem( L"Maint History" );
	FdoPtr<FdoDataPropertyDefinition> pHistId = (FdoDataPropertyDefinition*) FdoPropertiesP(pMaintHist->GetProperties())->GetItem( L"Date" );
	pClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"ElectricDevice" );

	FdoPtr<FdoObjectPropertyDefinition> pObjProp = FdoObjectPropertyDefinition::Create( L"maintenance history", L"maintenance history for this device" );
	pObjProp->SetClass( pMaintHist );
	pObjProp->SetIdentityProperty( pHistId );
	pObjProp->SetObjectType( FdoObjectType_Collection );
	FdoPropertiesP(pClass->GetProperties())->Add( pObjProp );

	pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();
}
#endif
void ApplySchemaTest::ModErrors( FdoIConnection* connection )
{
	/* Test some bad modifications */

	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	FdoPtr<FdoFeatureSchemaCollection> pSchemas = pDescCmd->Execute();

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

//	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Electric'l" );
	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Acad" );

    /* Try to change a property type */
	FdoFeatureClassP pClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Pole" );
	FdoDataPropertyP pProp = (FdoDataPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->GetItem( L"Height" );
	FdoPropertiesP(pClass->GetProperties())->Remove(pProp);
	FdoGeometricPropertyP pGeomProp = FdoGeometricPropertyDefinition::Create( L"Height", L"" );
	FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );

  	FdoDataPropertyP pMatProp = FdoDataPropertyDefinition::Create( L"Material", L"" );
	pMatProp->SetDataType( FdoDataType_String );
	pMatProp->SetNullable(false);
	pMatProp->SetLength(30);
	FdoPropertiesP(pClass->GetProperties())->Add( pMatProp );
    
	FdoPtr<FdoFeatureClass> pNoexistClass = FdoFeatureClass::Create( L"NoExists", L"" );
	pNoexistClass->SetIsAbstract(true);

	pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"id" );
	pProp->SetDataType( FdoDataType_Int32 );
	pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
	FdoPropertiesP(pNoexistClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pNoexistClass->GetIdentityProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pNoexistClass );


    pSchema->AcceptChanges();
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point );
    pMatProp->SetDescription( L"modified" );
    pNoexistClass->SetDescription( L"modified" );

	pClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"ElectricDevice" );
	pProp = (FdoDataPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->GetItem( L"FeatId" );
    pProp->SetIsAutoGenerated( false );

    pGeomProp = pClass->GetGeometryProperty();
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point );
    pGeomProp->SetHasElevation(false);
    pGeomProp->SetHasMeasure(true);

	/* Various class and property mods. Currently, only descriptions can be changed */

	FdoPtr<FdoFeatureClass> pTransClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Transformer" );
	pTransClass->SetBaseClass( NULL );
	pTransClass->SetIsAbstract(true);
	
    pProp = (FdoDataPropertyDefinition*) FdoPropertiesP(pTransClass->GetProperties())->GetItem( L"Voltage" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetPrecision( 45 );
	pProp->SetScale( 8 );
	pProp->SetNullable( true );

	pProp = (FdoDataPropertyDefinition*) FdoPropertiesP(pTransClass->GetProperties())->GetItem( L"Phase" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength( 3 );
    pProp->SetDefaultValue( L"A" );

	pProp = (FdoDataPropertyDefinition*) FdoPropertiesP(pTransClass->GetProperties())->GetItem( L"PartNum" );
	pProp->SetNullable(false);

	pProp = (FdoDataPropertyDefinition*) FdoPropertiesP(pTransClass->GetProperties())->GetItem( L"SerialNum" );
    pProp->SetIsAutoGenerated( true );

	pProp = (FdoDataPropertyDefinition*) FdoPropertiesP(pTransClass->GetProperties())->GetItem( L"LastRepairDate" );
    FdoPropertiesP(pTransClass->GetProperties())->Remove( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"LastRepairDate", L"" );
	pProp->SetDataType( FdoDataType_DateTime );
	pProp->SetNullable(true);
	FdoPropertiesP(pTransClass->GetProperties())->Add( pProp );

	FdoPtr<FdoFeatureClass> pDevClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"ElectricDevice" );
	pDevClass->SetIsAbstract(false);

	FdoPtr<FdoClass> pCustClass = (FdoClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Customer" );

	pProp = (FdoDataPropertyDefinition*) FdoPropertiesP(pCustClass->GetProperties())->GetItem( L"Credit Rating" );
    pProp->SetReadOnly( false );

	/* Try to delete a class that has data */

//    InsertObject( connection, false, L"Electric'l", L"Conductor", L"underground", L"0", NULL );
	pClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Conductor" );
	pClass->Delete();

	/* Try to redefine geometry inherited from base class */

	pClass = FdoFeatureClass::Create( L"Fuse", L"redefining geometry" );
	pClass->SetIsAbstract(false);
	pClass->SetBaseClass( pDevClass );
	// Redefine geometry for fuse
	pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"location and shape" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point );
	FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );
	pClass->SetGeometryProperty(pGeomProp);

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

	/* Try to redefine inherited properties */

	pClass = FdoFeatureClass::Create( L"Dry Transformer", L"redefining properties" );
	pClass->SetIsAbstract(false);
	pClass->SetBaseClass( pTransClass );

	// redefine data type

	pProp = FdoDataPropertyDefinition::Create( L"InstallDate", L"redefined" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(true);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	// redefine nullability

	pProp = FdoDataPropertyDefinition::Create( L"PartNum", L"redefined" );
	pProp->SetDataType( FdoDataType_Int16 );
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	// redefine string length

	pProp = FdoDataPropertyDefinition::Create( L"Phase", L"redefined" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(2);
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	// redefine number precision

	pProp = FdoDataPropertyDefinition::Create( L"Voltage", L"redefined" );
	pProp->SetDataType( FdoDataType_Decimal );
	pProp->SetPrecision(20);
	pProp->SetScale(-2);
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	pClass = FdoFeatureClass::Create( L"Liquid Transformer", L"redefining properties" );
	pClass->SetIsAbstract(false);
	pClass->SetBaseClass( pTransClass );

	// redefine numeric scale

	pProp = FdoDataPropertyDefinition::Create( L"Voltage", L"redefined" );
	pProp->SetDataType( FdoDataType_Decimal );
	pProp->SetPrecision(10);
	pProp->SetScale(-3);
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

	/* Try to redefine a property's type */

	pClass = FdoFeatureClass::Create( L"Gas Transformer", L"redefining properties" );
	pClass->SetIsAbstract(false);
	pClass->SetBaseClass( pTransClass );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

	pCmd->SetFeatureSchema( pSchema );

	bool succeeded = false;
	try {
 		pCmd->Execute();
		succeeded = true;
	}
	catch ( FdoSchemaException* e )
	{
		UnitTestUtil::PrintException(e, "apply_schema_err5.txt", true);
		FDO_SAFE_RELEASE(e);
	}

	if ( succeeded ) 
		CPPUNIT_FAIL( "Invalid modifications were supposed to fail" );
}

void ApplySchemaTest::ModErrors2( FdoIConnection* connection )
{
	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	FdoPtr<FdoFeatureSchemaCollection> pSchemas = pDescCmd->Execute();

//	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Electric'l" );
	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Acad" );

	/* Try to change the type of a class */

	FdoPtr<FdoClassDefinition> pClassDef = FdoClassesP(pSchema->GetClasses())->GetItem( L"'Address" );
	FdoClassesP(pSchema->GetClasses())->Remove( pClassDef );

	FdoPtr<FdoFeatureClass> pClass = FdoFeatureClass::Create( L"'Address", L"" );
	pClass->SetIsAbstract(false);
	FdoClassesP(pSchema->GetClasses())->Add( pClass );

	pSchema->AcceptChanges();
	pClass->SetDescription( L"class type change" );

	/* Try to delete a property that has non-null values */

	pClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Conductor" );
	FdoPtr<FdoPropertyDefinition> pProp = FdoPropertiesP(pClass->GetProperties())->GetItem( L"underground" );
	pProp->Delete();

	/* Try to add a not null property to class with objects */

	FdoPtr<FdoDataPropertyDefinition> pDataProp = FdoDataPropertyDefinition::Create( L"Conductivity", L"" );
	pDataProp->SetDataType( FdoDataType_Double );
	pDataProp->SetNullable( false );
	FdoPropertiesP(pClass->GetProperties())->Add(pDataProp);

  	FdoPtr<FdoFeatureClass> pTransClass = (FdoFeatureClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Transformer" );

	/* Try to add a not null property to class without objects.
     * Should only fail for SqlServer
     */

	pDataProp = FdoDataPropertyDefinition::Create( L"Altitude", L"" );
	pDataProp->SetDataType( FdoDataType_Double );
	pDataProp->SetNullable( false );
	FdoPropertiesP(pTransClass->GetProperties())->Add(pDataProp);

	/* Try to redefine inherited properties */

    pClass = FdoFeatureClass::Create( L"Dry Transformer", L"redefining properties" );
	pClass->SetIsAbstract(false);
	pClass->SetBaseClass( pTransClass );

	// Redefine geometry types

	FdoGeometricPropertyP pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"location and shape" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point );
	FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );
	pClass->SetGeometryProperty( pGeomProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

    /* Try to modify identity properties */

    FdoClassP pCustClass = (FdoClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Customer" );
    FdoDataPropertiesP idProps = pCustClass->GetIdentityProperties();
    idProps->RemoveAt(0);

    FdoClassP pBusClass = (FdoClass*) FdoClassesP(pSchema->GetClasses())->GetItem( L"Customer - Business" );
    pBusClass->SetDescription( L"Should not generate error message" );

    FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
	pCmd->SetFeatureSchema( pSchema );

	bool succeeded = false;
	try {
		pCmd->Execute();
		succeeded = true;
	}
	catch ( FdoSchemaException* e )
	{
		UnitTestUtil::PrintException(e, "apply_schema_err6.txt", true);
		FDO_SAFE_RELEASE(e);
	}

	if ( succeeded ) 
		CPPUNIT_FAIL( "Invalid modifications were supposed to fail" );
}
#if 0
void ApplySchemaTest::GetClassCapabilities( FdoIConnection* connection )
{
	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	pDescCmd->SetSchemaName( L"Acad" );
	FdoFeatureSchemasP pSchemas = pDescCmd->Execute();
    FdoFeatureSchemaP pSchema = pSchemas->GetItem( L"Acad" );

    FdoClassDefinitionP pClass = FdoClassesP( pSchema->GetClasses() )->GetItem( L"AcDbEntity" );
    VldClassCapabilities( 0, 0, pClass );

    pClass = FdoClassesP( pSchema->GetClasses() )->GetItem( L"AcXData" );
    VldClassCapabilities( 0, 0, pClass );

}

void ApplySchemaTest::CheckBaseProperties( FdoIConnection* connection )
{
	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	pDescCmd->SetSchemaName( L"Land" );
	FdoFeatureSchemasP pSchemas = pDescCmd->Execute();
    FdoFeatureSchemaP pSchema = pSchemas->GetItem( L"Land" );
    FdoClassesP pClasses = pSchema->GetClasses();
    
    FdoClassP pClass = (FdoClass*) pClasses->GetItem( L"Person" );
    FdoPtr<FdoReadOnlyPropertyDefinitionCollection> pBaseProps = pClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 0 );

    FdoFeatureClassP pFeatClass = (FdoFeatureClass*) pClasses->GetItem( L"Driveway" );
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 2);
    CPPUNIT_ASSERT( UnitTestUtil::ContainsRdOnlyProp(pBaseProps,L"ClassId") );
    CPPUNIT_ASSERT( UnitTestUtil::ContainsRdOnlyProp(pBaseProps,L"RevisionNumber") );

    // Make sure that system property doesn't reference a 
    // class from F_MetaSchema. F_MetaSchema is not exposed to FDO.

    FdoSchemaElementP systemProp = pBaseProps->GetItem( L"RevisionNumber" );
    CPPUNIT_ASSERT( systemProp->GetParent() == NULL );


    FdoFeatureClassP pParcelClass = (FdoFeatureClass*) pClasses->GetItem( L"Parcel" );
    pBaseProps = pParcelClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 3 );
    CPPUNIT_ASSERT( UnitTestUtil::ContainsRdOnlyProp(pBaseProps,L"ClassId") );
    CPPUNIT_ASSERT( UnitTestUtil::ContainsRdOnlyProp(pBaseProps,L"RevisionNumber") );
    CPPUNIT_ASSERT( UnitTestUtil::ContainsRdOnlyProp(pBaseProps,L"FeatId") );

    FdoFeatureClassP pBldgClass = (FdoFeatureClass*) pClasses->GetItem( L"Build'g" );
    pBaseProps = pBldgClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 2 );

    pFeatClass = (FdoFeatureClass*) pClasses->GetItem( L"1-8 School" );
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 7 );
    pFeatClass->SetBaseClass(NULL);
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 2 );
    pFeatClass->SetBaseClass(NULL);
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 2 );
    pFeatClass->SetBaseClass(pParcelClass);
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 8 );
    pFeatClass->SetBaseClass(pBldgClass);
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 7 );

    FdoFeatureClassP pNewClass = FdoFeatureClass::Create( L"NewClass", L"" );
    FdoDataPropertyP pProp = FdoDataPropertyDefinition::Create( L"Feature_Id", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
	FdoPropertiesP(pNewClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pNewClass->GetIdentityProperties())->Add( pProp );
    pClasses->Add(pNewClass);

    pFeatClass->SetBaseClass(pNewClass);
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 1 );
    pFeatClass->SetBaseClass(pParcelClass);
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 8 );
    pFeatClass->SetBaseClass(NULL);
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 3 );
    pFeatClass->SetBaseClass(pNewClass);
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 1 );

    FdoDataPropertiesP(pNewClass->GetIdentityProperties())->Clear();
    pNewClass->SetBaseClass(pParcelClass);
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 9 );
    pNewClass->SetBaseClass(pBldgClass);
    pBaseProps = pFeatClass->GetBaseProperties();
    CPPUNIT_ASSERT( pBaseProps->GetCount() == 8 );
    
    CPPUNIT_ASSERT( FdoPropertyP(pBaseProps->GetItem(L"# Rooms"))->GetQualifiedName() == L"Land:Build'g.# Rooms" );
    CPPUNIT_ASSERT( FdoPropertyP(pBaseProps->GetItem(L"RevisionNumber"))->GetQualifiedName() == L"RevisionNumber" );
}
#endif
void ApplySchemaTest::CopySchemas( 
    FdoFeatureSchemaCollection* pSchemas,
    FdoFeatureSchemaCollection* pSchemas2
)
{
    /* Test ApplySchema with ignoreStates=true */

    printf( "Initializing Copy Connection ... \n" );
	FdoPtr<FdoIConnection> connection = UnitTestUtil::OpenConnection( APPLY_SCHEMA_COPY_FILE, true );

    // Test1 applies into an empty datastore and then does an apply 
    // into a non-empty datastore (tests the additive merge).

	printf( "Schema Copy Test1 ... \n" );

	FdoPtr<FdoIApplySchema> pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
    pCmd->SetIgnoreStates(true);
    pCmd->SetFeatureSchema( FdoFeatureSchemaP(pSchemas->GetItem(L"Acad")) );
    pCmd->Execute();

    pCmd->SetFeatureSchema( FdoFeatureSchemaP(pSchemas2->GetItem(L"Acad")) );
    pCmd->Execute();

    FdoIoFileStreamP outFile = FdoIoFileStream::Create( L"apply_schema_test4.xml", L"wt" );
    UnitTestUtil::ExportDb( connection, (FdoIoFileStream*) outFile );

    printf( "Closing Copy Connection ... \n" );
    connection->Close();

    printf( "Initializing Copy Connection ... \n" );
	connection = UnitTestUtil::OpenConnection( APPLY_SCHEMA_COPY_FILE, true );

    // Test2 applies into an empty datastore. The FDO Feature Schema elements have
    // a variety of states, all of which should be ignored.

	printf( "Schema Copy Test2 ... \n" );
	pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
    pCmd->SetFeatureSchema( FdoFeatureSchemaP(pSchemas->GetItem(L"Acad")) );

    // Set up a mixture of element states.
    ModElectricSchema( pSchemas );

    pCmd->SetIgnoreStates(true);
    pCmd->Execute();

    outFile = FdoIoFileStream::Create( L"apply_schema_test5.xml", L"wt" );
    UnitTestUtil::ExportDb( connection, (FdoIoFileStream*) outFile );

    printf( "Closing Copy Connection ... \n" );
    connection->Close();

}
#if 0
void ApplySchemaTest::VldClassCapabilities( int ltMode, int lckMode, FdoClassDefinition* pClass )
{
    FdoInt32 expLockCount;
    
    switch ( lckMode ) {
        case 2:
            expLockCount = 5;
            break;
        case 1:
            expLockCount = 3;
            break;
        default:
            expLockCount = 1;
            break;
    }

    if ( FdoDataPropertiesP(pClass->GetIdentityProperties())->GetCount() == 0 &&
         FdoPtr<FdoReadOnlyDataPropertyDefinitionCollection>(pClass->GetBaseIdentityProperties())->GetCount() == 0)
         expLockCount = 0;

    // The following verifies that the AcDbEntity class capabilities are valid.
    FdoClassCapabilitiesP cc = pClass->GetCapabilities();
    CPPUNIT_ASSERT( cc != NULL );
    CPPUNIT_ASSERT( (lckMode != 0) == (cc->SupportsLocking()) );
    CPPUNIT_ASSERT( (ltMode != 0) == (cc->SupportsLongTransactions()) );

    FdoInt32 lockTypeCount;
    bool lockArray[5];
    FdoInt32 i;
    FdoLockType* lockTypes = cc->GetLockTypes( lockTypeCount );

    CPPUNIT_ASSERT( lockTypeCount == expLockCount );

    for ( i = 0; i < 5; i++ ) 
        lockArray[i] = false;

    for ( i = 0; i < lockTypeCount; i++ ) {
        switch ( lockTypes[i] ) {
        case FdoLockType_Transaction:
            lockArray[0] = true;
            break;
        case FdoLockType_Exclusive:
            lockArray[1] = true;
            break;
        case FdoLockType_LongTransactionExclusive:
            lockArray[2] = true;
            break;
        case FdoLockType_Shared:
            lockArray[3] = true;
            break;
        case FdoLockType_AllLongTransactionExclusive:
            lockArray[4] = true;
            break;
        default:
			CPPUNIT_FAIL( "Unexpected lock type" );
            break;
        }
    }

    for ( i = 0; i < expLockCount; i++ )
        CPPUNIT_ASSERT( lockArray[i] );
}

void ApplySchemaTest::InsertObject( FdoIConnection* connection, bool conditional, FdoStringP schemaName, FdoString* className, ... )
{
    va_list arguments;
    FdoString* arg;

    va_start(arguments, className);

    arg = va_arg(arguments,FdoString*);
/*
    if ( conditional ) 
        StartLongTransaction( connection, schemaName );
*/
    FdoITransaction* trans = (FdoITransaction *) connection->BeginTransaction();
    FdoPtr<FdoIInsert> insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
    FdoPtr<FdoPropertyValueCollection> propertyValues;
    FdoPtr<FdoDataValue> dataValue;
    FdoPtr<FdoPropertyValue> propertyValue;

    insertCommand->SetFeatureClassName(schemaName + L":" + FdoStringP(className));
    propertyValues = insertCommand->GetPropertyValues();

    while ( arg != NULL ) {

        propertyValue = AddNewProperty( propertyValues, arg);
        arg = va_arg(arguments,FdoString*);
        
        if ( arg != NULL ) {
            dataValue = FdoDataValue::Create(arg);
            propertyValue->SetValue(dataValue);
            arg = va_arg(arguments,FdoString*);
        }
    }
        
	insertCommand->Execute();

    trans->Commit();
    FDO_SAFE_RELEASE(trans);
/*
    if ( conditional ) 
        EndLongTransaction( connection );
*/
    va_end(arguments);
}

void ApplySchemaTest::DeleteObjects( FdoIConnection* connection, FdoStringP schemaName, FdoStringP className )
{
/*
    StartLongTransaction( connection, schemaName );
*/
    FdoPtr<FdoIDelete> deleteCommand = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);
    deleteCommand->SetFeatureClassName(schemaName + L":" + className);
	deleteCommand->Execute();
/*
    EndLongTransaction( connection );
*/
}

#endif
