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
#include "FdoSchemaRollbackTest.h"
#include "UnitTestUtil.h"
#include "ConnectionUtil.h"

#define   DB_NAME_SUFFIX            L"_Rollback_schema"

FdoSchemaRollbackTest::FdoSchemaRollbackTest(void) :
    mCanAddNotNullCol(true)
{
}

FdoSchemaRollbackTest::~FdoSchemaRollbackTest(void)
{
}

void FdoSchemaRollbackTest::setUp ()
{
    set_provider();
}

// no rollback cache pre-clear (therefore 
// synchronize all schemas
// synchronize everything       ( therefore same results as test_ynnn )
// no commit before synchronize
void FdoSchemaRollbackTest::Test_nnnn ( )
{
	DoTest( false, L"", false, false );
}

// no rollback cache pre-clear (therefore same results as test_nnnn )
// synchronize all schemas
// synchronize rollbacks only 
// no commit before synchronize
void FdoSchemaRollbackTest::Test_nnyn ( )
{
	DoTest( false, L"", true, false );
}

// no rollback cache pre-clear 
// synchronize only one schema
// synchronize everything in the schema
// no commit before synchronize
void FdoSchemaRollbackTest::Test_nynn ( )
{
	DoTest( false, L"Acad", false, false );
}

// no rollback cache pre-clear 
// synchronize only one schema
// synchronize rollbacks only 
// no commit before synchronize
void FdoSchemaRollbackTest::Test_nyyn ( )
{
	DoTest( false, L"Electric'l", true, false );
}

// rollback cache pre-clear 
// synchronize all schemas
// synchronize everything (therefore same as test_nnnn, rollback cache pre-clear has no effect) 
// no commit before synchronize
void FdoSchemaRollbackTest::Test_ynnn ( )
{
	DoTest( true, L"", false, false );
}

// rollback cache pre-clear 
// synchronize all schemas
// synchronize rollbacks only 
// no commit before synchronize
void FdoSchemaRollbackTest::Test_ynyn ( )
{
	DoTest( true, L"", true, false );
}

// rollback cache pre-clear 
// synchronize one schema
// synchronize everything (therefore rollback cache pre-clear has no effect) 
// no commit before synchronize
void FdoSchemaRollbackTest::Test_yynn ( )
{
	DoTest( true, L"Acad", false, false );
}

// rollback cache pre-clear 
// synchronize one schema
// synchronize rollbacks only
// no commit before synchronize
void FdoSchemaRollbackTest::Test_yyyn ( )
{
	DoTest( true, L"Acad", true, false );
}

// rollback cache pre-clear 
// synchronize all schemas
// synchronize everything
// commit before synchronize (therefore, synchronize should do nothing since nothing got 
//                              rolled back ).
void FdoSchemaRollbackTest::Test_ynny ( )
{
	DoTest( true, L"", false, true );
}

void FdoSchemaRollbackTest::DoTest ( bool bPreClear, const wchar_t* schemaName, bool bRollbackOnly, bool bCommit )
{
	FdoPtr<FdoIConnection> connection;
    StaticConnection* staticConn = NULL;
    FdoSchemaManagerP mgr;
    const FdoSmLpSchemaCollection* lp = NULL;

    // All of these tests exercise the Schema Manager but only one test
    // exercises sometime that can currently happen in FDO. Therefore,
    // to save time ...
    // mode. 
    if ( bPreClear || (wcslen(schemaName) > 0) || (!bRollbackOnly) || bCommit ) {
        // these other tests are skipped by default.
        if ( !UnitTestUtil::DoThisTest( "rollback" ) )
            return;
    }

    try {
		// delete, re-create and open the datastore
		printf( "Initializing Connection ... \n" );
		connection = UnitTestUtil::CreateConnection(
			true,
			true,
            DB_NAME_SUFFIX,
            NULL,
            NULL,
            0
		);

        staticConn = UnitTestUtil::NewStaticConnection();
        staticConn->connect();
        staticConn->SetSchema( DB_NAME_SUFFIX );

	    // The following tests must be run in the following order to get the expected results.

	    printf( "Creating Acad Schema ... \n" );
	    CreateAcadSchema( connection );

	    printf( "Creating Electric Schema ... \n" );
	    CreateElectricSchema( connection );

        // At this point all schema elements are in the rollback cache. The following 
        // conditionally does a rollback, which clears the rollback cache. This means
        // that only subsequently modified elements will be later rolled back  when 
        // bRollbackOnly is true.
        if ( bPreClear ) {
            FdoPtr<FdoITransaction> clearTrans = connection->BeginTransaction();
		    clearTrans->Rollback();
		}

        // Grab snapshot of original MetaSchema ( for debugging ).
		printf( "Writing Original Schema ... \n" );
        FdoSchemaManagerP mgr = staticConn->CreateSchemaManager();
        const FdoSmLpSchemaCollection* lp = mgr->RefLogicalPhysicalSchemas();
        lp->XMLSerialize( UnitTestUtil::GetOutputFileName( L"schema_rollback_orig.xml" ) );

		printf( "Deleting some tables ... \n" );
        // Do some deletes outside of the schema manager. These tables will not be re-created
        // bPreClear and bRollbackOnly are true, since these tables won't be in the 
        // rollback cache.
        FdoSmPhMgrP phMgr = mgr->GetPhysicalSchema();
        FdoSmPhDbObjectP pObject = phMgr->FindDbObject( phMgr->GetDcDbObjectName(L"Conductor"), L"", L"", false );
        pObject->SetElementState( FdoSchemaElementState_Deleted );
        pObject->Commit();
        pObject = phMgr->FindDbObject( phMgr->GetDcDbObjectName(L"AcDbEntity_AcXData"), L"", L"", false );
        pObject->SetElementState( FdoSchemaElementState_Deleted );
        pObject->Commit();
    	
		FdoPtr<FdoITransaction> trans1 = connection->BeginTransaction();

		printf( "Modifying Acad Schema ... \n" );
		ModAcadSchema( connection );
		printf( "Modifying Electric Schema ... \n" );
        ModElectricSchema( connection );

		printf( "Performing rollback ... \n" );
 		if ( bCommit ) {
            // Commit before rollback if asked to. In this case, rollback will not 
            // have any physical schema synchronizations to perform.
		    trans1->Commit();
		    trans1 = connection->BeginTransaction();
		}

		if ( (wcslen(schemaName) == 0) && bRollbackOnly ) {
            // Transaction rollback invokes SynchPhysical on all schemas ( synchronize 
            // rollbacks only ) so exercise it in this case.
		    trans1->Rollback();
		}
		else {
            // For other cases, call SynchPhysical directly, after low-level rollback.
		    UnitTestUtil::Sql2Db( L"rollback", connection );
            mgr = staticConn->CreateSchemaManager();
		    mgr->SynchPhysical( schemaName, bRollbackOnly );
		}

        // Write out the resulting metaschema
		printf( "Writing Schema after Rollback ... \n" );
        mgr = staticConn->CreateSchemaManager();
        lp = mgr->RefLogicalPhysicalSchemas();
        lp->XMLSerialize( OutFileName(bPreClear, schemaName, bRollbackOnly, bCommit) );

		try {
		    trans1 = NULL;
		}
		catch ( ... ) {
		}

		printf( "Closing Connection ... \n" );
		UnitTestUtil::CloseConnection( connection, false, DB_NAME_SUFFIX );

		// Compare results with expected results.
        // Todo: check SqlServer and MySql results.

#ifdef RDBI_DEF_ORA
        UnitTestUtil::CheckOutput( 
	        MasterFileName(bPreClear, schemaName, bRollbackOnly, bCommit), 
	        OutFileName(bPreClear, schemaName, bRollbackOnly, bCommit)
        );
#endif
	}
	catch ( FdoException* e ) 
	{
		try {
			connection->Close(); 
		}
		catch ( ... ) 
		{
		}
		UnitTestUtil::FailOnException( e );
	}
	catch ( CppUnit::Exception e ) 
	{
		connection->Close(); 
		throw;
	}
   	catch (...)
   	{
		connection->Close(); 
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}
		
	printf( "Done\n" );

}

void FdoSchemaRollbackTest::CreateAcadSchema( FdoIConnection* connection )
{
	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	FdoFeatureSchemasP pSchemas = FdoFeatureSchemaCollection::Create(NULL);

	FdoPtr<FdoFeatureSchema> pSchema = FdoFeatureSchema::Create( L"Acad", L"AutoCAD schema" );
	pSchemas->Add( pSchema );

	// Id'less class for Object Properties only

	FdoPtr<FdoClass> pXData = FdoClass::Create( L"AcXData", L"Xdata" );
	pXData->SetIsAbstract(false);

	FdoPtr<FdoDataPropertyDefinition> pXDataSeq = FdoDataPropertyDefinition::Create( L"Seq", L"seq" );
	pXDataSeq->SetDataType( FdoDataType_Int32 );
	pXDataSeq->SetNullable(false);
	FdoPropertiesP(pXData->GetProperties())->Add( pXDataSeq  );

	FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"Datavalue", L"datavalue" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(4000);
	pProp->SetNullable(false);
	FdoPropertiesP(pXData->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pXData );

	// More Id'less classes used to test unique table name generation

	FdoPtr<FdoClass> pCoordVal = FdoClass::Create( L"AcDbVertexCoordinateValue", L"" );
	pCoordVal->SetIsAbstract(false);

	FdoPtr<FdoDataPropertyDefinition> pCoordValSeq = FdoDataPropertyDefinition::Create( L"Seq", L"seq" );
	pCoordValSeq->SetDataType( FdoDataType_Int32 );
	pCoordValSeq->SetNullable(false);
	FdoPropertiesP(pCoordVal->GetProperties())->Add( pCoordValSeq  );

	pProp = FdoDataPropertyDefinition::Create( L"Value", L"" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(false);
	FdoPropertiesP(pCoordVal->GetProperties())->Add( pProp  );

	FdoClassesP(pSchema->GetClasses())->Add( pCoordVal );

	FdoPtr<FdoClass> pVertex = FdoClass::Create( L"AcDbVertexData", L"" );
	pVertex->SetIsAbstract(false);

	FdoPtr<FdoDataPropertyDefinition> pVertexSeq = FdoDataPropertyDefinition::Create( L"Seq", L"seq" );
	pVertexSeq->SetDataType( FdoDataType_Int32 );
	pVertexSeq->SetNullable(false);
	FdoPropertiesP(pVertex->GetProperties())->Add( pVertexSeq  );

	FdoPtr<FdoObjectPropertyDefinition> pObjProp = FdoObjectPropertyDefinition::Create( L"point", L"" );
	pObjProp->SetClass( pCoordVal );
	pObjProp->SetIdentityProperty( pCoordValSeq );
	pObjProp->SetObjectType( FdoObjectType_Collection );
	FdoPropertiesP(pVertex->GetProperties())->Add( pObjProp );

	FdoClassesP(pSchema->GetClasses())->Add( pVertex );

	// A non-abstract base class

	FdoPtr<FdoFeatureClass> pEntClass = FdoFeatureClass::Create( L"AcDbEntity", L"AutoCAD entity base class" );
	pEntClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"id" );
	pProp->SetDataType( FdoDataType_Int64 );
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
	pProp->SetNullable(false);
	FdoPropertiesP(pEntClass->GetProperties())->Add( pProp );

	pObjProp = FdoObjectPropertyDefinition::Create( L"xdata", L"xdata" );
	pObjProp->SetClass( pXData );
	pObjProp->SetIdentityProperty( pXDataSeq );
	pObjProp->SetObjectType( FdoObjectType_OrderedCollection );
	pObjProp->SetOrderType( FdoOrderType_Ascending );
	FdoPropertiesP(pEntClass->GetProperties())->Add( pObjProp );

	FdoClassesP(pSchema->GetClasses())->Add( pEntClass );

	FdoPtr<FdoClass> pEntRefClass = FdoClass::Create( L"Entity", L"Embedded entity base class" );
	pEntRefClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"id" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
	FdoPropertiesP(pEntRefClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pEntRefClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Layer", L"Acad layer" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(10);
	pProp->SetNullable(true);
	FdoPropertiesP(pEntRefClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"ColourIndex", L"Acad Colour" );
	pProp->SetDataType( FdoDataType_Byte );
	pProp->SetNullable(true);
	FdoPropertiesP(pEntRefClass->GetProperties())->Add( pProp );

	pObjProp = FdoObjectPropertyDefinition::Create( L"xdata", L"xdata" );
	pObjProp->SetClass( pXData );
	pObjProp->SetIdentityProperty( pXDataSeq );
	pObjProp->SetObjectType( FdoObjectType_OrderedCollection );
	pObjProp->SetOrderType( FdoOrderType_Ascending );
	FdoPropertiesP(pEntRefClass->GetProperties())->Add( pObjProp );

	FdoClassesP(pSchema->GetClasses())->Add( pEntRefClass );

	// A sub-class to test Object and Data property inheritance.

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

	pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();

	pProp = FdoDataPropertyDefinition::Create( L"Closed", L"is first and last points the same" );
	pProp->SetDataType( FdoDataType_Boolean );
	pProp->SetNullable(!mCanAddNotNullCol);
	FdoPropertiesP(pPlineClass->GetProperties())->Add( pProp );

	pObjProp = FdoObjectPropertyDefinition::Create( L"vertices", L"" );
	pObjProp->SetClass( pVertex );
	pObjProp->SetIdentityProperty( pVertexSeq );
	pObjProp->SetObjectType( FdoObjectType_Collection );
	FdoPropertiesP(pPlineClass->GetProperties())->Add( pObjProp );

	FdoPtr<FdoClass> pPlineRefClass = FdoClass::Create( L"Polyline", L"Embedded 3d polyline" );
	pPlineRefClass->SetIsAbstract(false);
	pPlineRefClass->SetBaseClass( pEntRefClass );
	FdoClassesP(pSchema->GetClasses())->Add( pPlineRefClass );

	pProp = FdoDataPropertyDefinition::Create( L"Width", L"line width" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetPrecision(10);
	pProp->SetScale(5);
	pProp->SetNullable(false);
	FdoPropertiesP(pPlineRefClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Closed", L"is first and last points the same" );
	pProp->SetDataType( FdoDataType_Boolean );
	pProp->SetNullable(false);
	FdoPropertiesP(pPlineRefClass->GetProperties())->Add( pProp );

	pObjProp = FdoObjectPropertyDefinition::Create( L"vertices", L"" );
	pObjProp->SetClass( pVertex );
	pObjProp->SetIdentityProperty( pVertexSeq );
	pObjProp->SetObjectType( FdoObjectType_Collection );
	FdoPropertiesP(pPlineRefClass->GetProperties())->Add( pObjProp );

	// The following tests object property nesting to 3 levels.

	FdoPtr<FdoFeatureClass> pHatchClass = FdoFeatureClass::Create( L"AcDbHatch", L"AutoCAD hatched polygon" );
	pHatchClass->SetIsAbstract(false);
	pHatchClass->SetBaseClass( pEntClass );

	pObjProp = FdoObjectPropertyDefinition::Create( L"edges", L"" );
	pObjProp->SetClass( pPlineRefClass );
	pObjProp->SetObjectType( FdoObjectType_Collection );
	FdoPropertiesP(pHatchClass->GetProperties())->Add( pObjProp );

	FdoClassesP(pSchema->GetClasses())->Add( pHatchClass );

	pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();
}

void FdoSchemaRollbackTest::CreateElectricSchema( FdoIConnection* connection )
{
	FdoPtr<FdoIDescribeSchema> pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);

	pDescCmd->SetSchemaName( L"Acad" );

	FdoPtr<FdoFeatureSchemaCollection> pAcadSchema = pDescCmd->Execute();

	FdoPtr<FdoClassDefinition> pEntClass = FdoClassesP(FdoFeatureSchemaP(pAcadSchema->GetItem( L"Acad" ))->GetClasses())->GetItem( L"Entity" );

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	/* A schema with dictionary */

	FdoPtr<FdoFeatureSchema> pSchema = FdoFeatureSchema::Create( L"Electric'l", L"Electrical '' schema'" );
	pAcadSchema->Add( pSchema );

	FdoSADP(pSchema->GetAttributes())->Add( L"'Author", L"Thomas O'Edison" );

	/* An abstract base class */

	FdoPtr<FdoFeatureClass> pDevClass = FdoFeatureClass::Create( L"ElectricDevice", L"electic base class" );
	pDevClass->SetIsAbstract(true);

	FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"id" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
	FdoPropertiesP(pDevClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pDevClass->GetIdentityProperties())->Add( pProp );

	// Test nested object properties (ElectricDevice.graphic.xdata) where graphic's class has an id.

	FdoPtr<FdoObjectPropertyDefinition> pObjProp = FdoObjectPropertyDefinition::Create( L"graphic", L"Acad entity" );
	pObjProp->SetObjectType( FdoObjectType_Value );
	pObjProp->SetClass( pEntClass );
	FdoPropertiesP(pDevClass->GetProperties())->Add( pObjProp );

	// Test geometry property

	FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"location and shape" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve );
	FdoPropertiesP(pDevClass->GetProperties())->Add( pGeomProp );

	pDevClass->SetGeometryProperty( pGeomProp );

	FdoClassesP(pSchema->GetClasses())->Add( pDevClass );

	/* Subclass with dictionary */

	FdoPtr<FdoFeatureClass> pClass = FdoFeatureClass::Create( L"Transformer", L"" );
	pClass->SetIsAbstract(false);
	pClass->SetBaseClass( pDevClass );
	FdoSADP(pClass->GetAttributes())->Add( L"Rules' DLL", L"transformer.dll" );
	FdoSADP(pClass->GetAttributes())->Add( L"Entrypoint", L"start_transformer" );

	// Add data properties of various types

	pProp = FdoDataPropertyDefinition::Create( L"Voltage", L"voltage" );
	FdoSADP(pProp->GetAttributes())->Add( L"Calculable", L"yes" );
	pProp->SetDataType( FdoDataType_Decimal );
	pProp->SetPrecision(10);
	pProp->SetScale(2);
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

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

	pClass = FdoFeatureClass::Create( L"Conductor", L"" );
	pClass->SetIsAbstract(false);
	pClass->SetBaseClass( pDevClass );

	pProp = FdoDataPropertyDefinition::Create( L"underground", L"" );
	pProp->SetDataType( FdoDataType_Boolean );
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

	pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();
}

void FdoSchemaRollbackTest::ModElectricSchema( FdoIConnection* connection )
{
	/* Test modifying an existing schema */

	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	pDescCmd->SetSchemaName( L"Electric'l" );
	FdoFeatureSchemasP pSchemas = pDescCmd->Execute();

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Electric'l" );

	FdoPtr<FdoFeatureClass> pClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"Transformer"));
	pClass->Delete();

	// Add a new non-inherited class that will later be removed.

	pClass = FdoFeatureClass::Create( L"Pole", L"" );
	pClass->SetIsAbstract(false);

	FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"FeatureId", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetIsAutoGenerated(true);
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Height", L"" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

	/* The following 3 classes tests adding nested object properties to an FdoClass,
	 * instead of an FdoFeatureClass.
	 */

	// class for nested object property

	FdoPtr<FdoClass> pStClass = FdoClass::Create( L"Street", L"" );
	pStClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( L"Name", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength( 30 );
	pProp->SetNullable(false);
	FdoPropertiesP(pStClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Type", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength( 30 );
	pProp->SetNullable(false);
	FdoPropertiesP(pStClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pStClass );

	// Class for top object property ( includes nested property )

	FdoPtr<FdoClass> pAddrClass = FdoClass::Create( L"'Address", L"" );
	pAddrClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( L"Number", L"" );
	pProp->SetDataType( FdoDataType_Int32 );
	pProp->SetNullable(false);
	FdoPropertiesP(pAddrClass->GetProperties())->Add( pProp );

	FdoPtr<FdoObjectPropertyDefinition> pObjProp = FdoObjectPropertyDefinition::Create( L"street", L"" );
	pObjProp->SetClass( pStClass );
	pObjProp->SetObjectType( FdoObjectType_Value );
	FdoPropertiesP(pAddrClass->GetProperties())->Add( pObjProp );

	FdoClassesP(pSchema->GetClasses())->Add( pAddrClass );

	// Class that contains top object property

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

	pObjProp = FdoObjectPropertyDefinition::Create( L"home' address", L"" );
	pObjProp->SetClass( pAddrClass );
	pObjProp->SetObjectType( FdoObjectType_Value );
	FdoPropertiesP(pEmpClass->GetProperties())->Add( pObjProp );

	FdoClassesP(pSchema->GetClasses())->Add( pEmpClass );

	pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();
}

void FdoSchemaRollbackTest::ModAcadSchema( FdoIConnection* connection )
{
	/* Test modifying an existing schema */

	FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	pDescCmd->SetSchemaName( L"Electric'l" );
	FdoFeatureSchemasP pSchemas = pDescCmd->Execute();

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	FdoPtr<FdoFeatureSchema> pSchema = pSchemas->GetItem( L"Acad" );

	FdoPtr<FdoFeatureClass> pClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"AcDbEntity"));
	FdoPtr<FdoClass> pRefClass = (FdoClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"Entity"));

	// Test delete of property 

	FdoPtr<FdoDataPropertyDefinition> pProp = (FdoDataPropertyDefinition*) (FdoPropertiesP(pClass->GetProperties())->GetItem(L"ColourIndex"));
	pProp->Delete();

	pProp = (FdoDataPropertyDefinition*) (FdoPropertiesP(pRefClass->GetProperties())->GetItem(L"ColourIndex"));
	pProp->Delete();

	// Test adding data property to existing class.

	pProp = FdoDataPropertyDefinition::Create( L"Plot Style", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(20);
	pProp->SetNullable(!mCanAddNotNullCol);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Plot Style", L"" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(20);
	pProp->SetNullable(!mCanAddNotNullCol);
	FdoPropertiesP(pRefClass->GetProperties())->Add( pProp );

	pClass = (FdoFeatureClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"AcDbHatch"));
	FdoPtr<FdoObjectPropertyDefinition> pObjProp = (FdoObjectPropertyDefinition*) FdoPropertiesP(pClass->GetProperties())->GetItem(L"edges");
	pObjProp->Delete();

	pRefClass = (FdoClass*) (FdoClassesP(pSchema->GetClasses())->GetItem(L"AcDbVertexData"));
	pObjProp = (FdoObjectPropertyDefinition*) FdoPropertiesP(pRefClass->GetProperties())->GetItem(L"point");
	pObjProp->Delete();

	pCmd->SetFeatureSchema( pSchema );
	pCmd->Execute();
}

FdoStringP FdoSchemaRollbackTest::OutFileName(bool bPreClear, const wchar_t* schemaName, bool bRollbackOnly, bool bCommit )
{
	FdoStringP fileName = 
		FdoStringP::Format(
			L"schema_rb_%hs%hs%hs%hs.xml",
			bPreClear ? "y" : "n",
			(wcslen(schemaName) == 0) ? "n" : "y",
			bRollbackOnly ? "y" : "n",	
			bCommit ? "y" : "n"
		);

	return(UnitTestUtil::GetOutputFileName( fileName ) );
}

FdoStringP FdoSchemaRollbackTest::MasterFileName(bool bPreClear, const wchar_t* schemaName, bool bRollbackOnly, bool bCommit )
{
	FdoStringP fileName = 
		FdoStringP::Format(
			L"schema_rb_%hs%hs%hs%hs_master.txt",
			bPreClear ? "y" : "n",
			(wcslen(schemaName) == 0) ? "n" : "y",
			bRollbackOnly ? "y" : "n",
			bCommit ? "y" : "n"	
		);

	return( fileName );
}

