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
#include "DescribeSchemaTest.h"
#include "UnitTestUtil.h"

DescribeSchemaTest::DescribeSchemaTest(void)
{
}

DescribeSchemaTest::~DescribeSchemaTest(void)
{
}

void DescribeSchemaTest::setUp ()
{
    set_provider();
}

void DescribeSchemaTest::dropDb ()
{
    try
    {
        UnitTestUtil::DropDb();
    }
    catch( FdoException *ex )
    {
        printf("DropDb: %ls\n", ex->GetExceptionMessage());
        throw;
    }
}

void DescribeSchemaTest::describe2()
{
    FdoPtr<FdoIConnection> connection;
    try
    {
        FdoIConnection* connection  = UnitTestUtil::GetConnection(L"", true);
        FdoPtr<FdoIDescribeSchema>  pDescSchemaCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
        FdoFeatureSchemasP          fsc = pDescSchemaCmd->Execute();

        fsc->WriteXml( UnitTestUtil::GetOutputFileName( L"describeSchemas.xml" ) );
    }
    catch ( FdoException* e )
    {
        if (connection)
            connection->Close();
        UnitTestUtil::FailOnException( e );
    }
    catch (...)
    {
        if (connection)
            connection->Close();
        throw;
    }
}


void DescribeSchemaTest::describe()
{
    FdoPtr<FdoIConnection> connection;
    try
    {
        printf( "Initializing Connection ... \n" );
        connection = UnitTestUtil::CreateConnection(
            true,
            true,
            L"_describeschema",
            NULL,
            NULL,
            0
        );

        printf( "Loading test data ... \n" );

        // Load data into newly created datastore
//      FdoSchemaManagerP sm = connection->GetSchemaManager();
        LoadTestData(connection /*, sm*/ );

        printf( "Closing Connection ... \n" );
        UnitTestUtil::CloseConnection(
            connection,
            false,
            L"_describeschema"
        );

        printf( "Re-opening Connection ... \n" );
        connection = UnitTestUtil::CreateConnection(
            false,
            false,
            L"_describeschema",
            NULL,
            NULL,
            0
        );

        FdoPtr<FdoIConnectionCapabilities> conCap = connection->GetConnectionCapabilities();

        printf( "Performing Describe ... \n" );
        FdoPtr<FdoIDescribeSchema>  pDescSchemaCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
        FdoFeatureSchemasP                     fsc = pDescSchemaCmd->Execute();

        fsc->WriteXml( UnitTestUtil::GetOutputFileName( L"describeSchemas.xml" ) );

        // TODO: Do more looking through pfsc. Currently, only class capabilities are checked.

        //if (UnitTestUtil::ProviderUnderstandsMetaschema())  // I.E. dont run the remaining tests for ODBC
        {
            CPPUNIT_ASSERT( fsc->GetCount() == 5 );

            CPPUNIT_ASSERT( !FdoFeatureSchemaP(fsc->FindItem(L"F_MetaClass")) );

            FdoFeatureSchemaP fs = fsc->GetItem( L"Acad" );
            CPPUNIT_ASSERT( fs != NULL );
            CPPUNIT_ASSERT( FdoClassesP(fs->GetClasses())->GetCount() == 9 );

            FdoClassDefinitionP fc = FdoClassesP(fs->GetClasses())->GetItem( L"AcDbEntity" );
            CPPUNIT_ASSERT( fc != NULL );

            FdoClassCapabilitiesP cc = fc->GetCapabilities();
            CPPUNIT_ASSERT( cc != NULL );

            // This test currently does not create OWM enabled tables so the
            // following capabilities are false.
            // ApplySchemaTest verifies that these are true when tables are OWM enabled.
            CPPUNIT_ASSERT( !cc->SupportsLocking() );
            CPPUNIT_ASSERT( !cc->SupportsLongTransactions() );
            CPPUNIT_ASSERT( FdoStringP(fc->GetName()) == FdoClassDefinitionP(cc->GetParent())->GetName() );

            FdoInt32 lockTypeCount = 0;
            FdoLockType* lockTypes = cc->GetLockTypes( lockTypeCount );

            CPPUNIT_ASSERT( lockTypeCount == GetLockTypeCount() );
            if ( GetLockTypeCount() > 0 ) {
                CPPUNIT_ASSERT( lockTypes[0] == FdoLockType_Transaction );
            }

            // Verify that the feature class has the right system properties
            // There are actually 5 system properties, but FeatId is omitted
            // because AcDbEntity.ID gets grafted onto it.

            FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseProps = fc->GetBaseProperties();

            CPPUNIT_ASSERT( baseProps->GetCount() == 0 );

            // Check the base properties for a class with base class

            fc = FdoClassesP(fs->GetClasses())->GetItem( L"AcDb3dPolyline" );
            baseProps = fc->GetBaseProperties();
    /* TODO
            // 0 system + 3 AcDbEntity properties
            CPPUNIT_ASSERT( baseProps->GetCount() == 3 );
    */
        }

        printf( "Closing Connection ... \n" );
        UnitTestUtil::CloseConnection(
            connection,
            false,
            L"_describeschema"
        );

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

void DescribeSchemaTest::getSchemaNames()
{
    FdoPtr<FdoIConnection> connection;
    try
    {
        printf( "Initializing Connection ... \n" );
        connection = UnitTestUtil::CreateConnection(
            true,
            true,
            L"_getSchemaNames",
            NULL,
            NULL,
            0
        );

        printf( "Loading test data ... \n" );

        // Load data into newly created datastore
//      FdoSchemaManagerP sm = connection->GetSchemaManager();
        LoadTestData(connection /*, sm*/ );

        printf( "Closing Connection ... \n" );
        UnitTestUtil::CloseConnection(
            connection,
            false,
            L"_getSchemaNames"
        );

        printf( "Re-opening Connection ... \n" );
        connection = UnitTestUtil::CreateConnection(
            false,
            false,
            L"_getSchemaNames",
            NULL,
            NULL,
            0
        );

        FdoPtr<FdoIConnectionCapabilities> conCap = connection->GetConnectionCapabilities();

        printf( "Performing GetSchemaNames ... \n" );
        FdoPtr<FdoIGetSchemaNames>  pCmd = (FdoIGetSchemaNames*) connection->CreateCommand(FdoCommandType_GetSchemaNames);
        FdoPtr<FdoStringCollection> schemaNames = pCmd->Execute();

        CPPUNIT_ASSERT( schemaNames->GetCount() == 5 );

        for (int i = 0; i < schemaNames->GetCount(); i++)
        {
            printf("SchemaName = %ls\n", (FdoString*) schemaNames->GetItem(i)->GetString());
        }

        printf( "Closing Connection ... \n" );
        UnitTestUtil::CloseConnection(
            connection,
            false,
            L"_getSchemaNames"
        );

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

void DescribeSchemaTest::getClassNames()
{
    FdoPtr<FdoIConnection> connection;
    try
    {
        printf( "Initializing Connection ... \n" );
        connection = UnitTestUtil::CreateConnection(
            true,
            true,
            L"_getClassNames",
            NULL,
            NULL,
            0
        );

        printf( "Loading test data ... \n" );

        // Load data into newly created datastore
//      FdoSchemaManagerP sm = connection->GetSchemaManager();
        LoadTestData(connection /*, sm*/ );

        printf( "Closing Connection ... \n" );
        UnitTestUtil::CloseConnection(
            connection,
            false,
            L"_getClassNames"
        );

        printf( "Re-opening Connection ... \n" );
        connection = UnitTestUtil::CreateConnection(
            false,
            false,
            L"_getClassNames",
            NULL,
            NULL,
            0
        );

        FdoPtr<FdoIConnectionCapabilities> conCap = connection->GetConnectionCapabilities();

        printf( "Performing GetClassNames ... \n" );
        FdoPtr<FdoIGetClassNames>  pCmd = (FdoIGetClassNames*) connection->CreateCommand(FdoCommandType_GetClassNames);
        FdoPtr<FdoStringCollection> classNames = pCmd->Execute();

        CPPUNIT_ASSERT( classNames->GetCount() == 25 );

        for (int i = 0; i < classNames->GetCount(); i++)
        {
            printf("ClassName = %ls\n", (FdoString *) classNames->GetItem(i)->GetString());
        }

        printf( "Closing Connection ... \n" );
        UnitTestUtil::CloseConnection(
            connection,
            false,
            L"_getClassNames"
        );

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

void DescribeSchemaTest::describeWithClassNames()
{
    FdoPtr<FdoIConnection> connection;
    try
    {
        printf( "Initializing Connection ... \n" );
        connection = UnitTestUtil::CreateConnection(
            true,
            true,
            L"_describeclasses",
            NULL,
            NULL,
            0
        );

        printf( "Loading test data ... \n" );

        // Load data into newly created datastore
//      FdoSchemaManagerP sm = connection->GetSchemaManager();
        LoadTestData(connection /*, sm*/ );

        printf( "Closing Connection ... \n" );
        UnitTestUtil::CloseConnection(
            connection,
            false,
            L"_describeclasses"
        );

        printf( "Re-opening Connection ... \n" );
        connection = UnitTestUtil::CreateConnection(
            false,
            false,
            L"_describeclasses",
            NULL,
            NULL,
            0
        );

        FdoPtr<FdoIConnectionCapabilities> conCap = connection->GetConnectionCapabilities();

        printf( "Performing Describe with class names... \n" );
        FdoPtr<FdoIDescribeSchema>  pDescSchemaCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
        FdoPtr<FdoStringCollection> classNames = FdoStringCollection::Create();
        classNames->Add(L"Acad:AcDbLine");
        classNames->Add(L"Acad:AcDbHatch");
        classNames->Add(L"Electric:Transformer");
        classNames->Add(L"Postal:Customer");
        pDescSchemaCmd->SetClassNames(classNames);
        FdoFeatureSchemasP                     fsc = pDescSchemaCmd->Execute();

        CPPUNIT_ASSERT( fsc->GetCount() == 3 );

        for (FdoInt32 i = 0; i < fsc->GetCount(); i++)
        {
            FdoPtr<FdoFeatureSchema> schema = fsc->GetItem(i);
            FdoPtr<FdoClassCollection> classes = schema->GetClasses();
            for (FdoInt32 j = 0; j < classes->GetCount(); j++)
            {
                FdoPtr<FdoClassDefinition> classDef = classes->GetItem(j);
                printf("ClassName = %ls\n", (FdoString *) classDef->GetQualifiedName());
                FdoPtr<FdoClassDefinition> baseClass = classDef->GetBaseClass();
                if (baseClass)
                {
                    printf("BaseClassName = %ls\n", (FdoString *) baseClass->GetQualifiedName());
                }
            }
        }

        printf( "Closing Connection ... \n" );
        UnitTestUtil::CloseConnection(
            connection,
            false,
            L"_describeclasses"
        );

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

FdoInt32 DescribeSchemaTest::GetLockTypeCount()
{
    return 1;
}

void DescribeSchemaTest::LoadTestData(FdoIConnection* connection/*, FdoRdbmsSchemaManager* sm*/)
{
    static char* id_cols[] = { "ID" };
    static char* featid_cols[] = { "FEATID" };
    static char* addrid_cols[] = { "NUM", "STREET", "CITY" };
    static char* addraddid_cols[] = { "NUM", "STREET", "CITY", "PROVINCE" };
    static char* addrinhid_cols[] = { "NUM", "CITY" };
    static char* custid_cols[] = { "NAME", "PROVINCE" };

    // Create schema
    UnitTestUtil::Sql2Db( GetSchema(), connection );

    // Load data
    UnitTestUtil::Sql2Db( (const wchar_t**) mInputData, connection );
/* TODO:
    sm->dbi_key_add( "PK_ACDBENTITY", "ACDBENTITY", id_cols, 1 );
    sm->dbi_key_add( "PK_ACDBLINE", "ACDBLINE", id_cols, 1 );
    sm->dbi_key_add( "PK_CONDUCTOR", "CONDUCTOR", id_cols, 1 );
    sm->dbi_key_add( "PK_TRANSFORMER", "TRANSFORMER", featid_cols, 1 );
    sm->dbi_key_add( "PK_ELECTRICDEVICE_ACDBENTITY", "ELECTRICDEVICE_ACDBENTITY", id_cols, 1 );
    sm->dbi_key_add( "PK_ADDRESS", "ADDRESS", addrid_cols, 3 );
    sm->dbi_key_add( "PK_ADDRESSADDID", "ADDRESSADDID", addraddid_cols, 4 );
    sm->dbi_key_add( "PK_ADDRESSBADID", "ADDRESSBADID", addrid_cols, 3 );
    sm->dbi_key_add( "PK_ADDRESSINHID", "ADDRESSINHID", addrinhid_cols, 2 );
    sm->dbi_key_add( "PK_CUSTOMER", "CUSTOMER", custid_cols, 2 );
    sm->dbi_key_add( "PK_CUSTOMER_HQADDRESSES", "CUSTOMER_HQADDRESSES", addrid_cols, 3 );
*/
}

bool DescribeSchemaTest::Find_ROP( FdoReadOnlyPropertyDefinitionCollection* props, FdoString* propName )
{
    try {
        FdoSchemaElementP( props->GetItem(propName) );
    }
    catch ( FdoException* e ) {
        FDO_SAFE_RELEASE(e);
        return false;
    }

    return true;
}

FdoString** DescribeSchemaTest::GetSchema()
{
    static FdoString* theSchema[] = {
    L"create table acdbentity (",
    L"    ID       INT NOT NULL,",
    L"    LAYER    VARCHAR(255)",
    L");",
    L"",
    L"",
    L"create table acdbline (",
    L"    ID       INT NOT NULL,",
    L"    LAYER    VARCHAR(255)",
    L");",
    L"",
    L"create table acdb3dpolyline (",
    L"    ID       INT NOT NULL,",
    L"    LAYER    VARCHAR(255)",
    L");",
    L"",
    L"create table acdbblockattribute (",
    L"    ID       INT NOT NULL",
    L");",
    L"",
    L"create table acdbentity_acxdata (",
    L"    ACDBENTITY_ID       INT NOT NULL,",
    L"    SEQ                 INT NOT NULL,",
    L"    DATAVALUE           VARCHAR(4000) NOT NULL",
    L");",
    L"",
    L"create table acdbhatch (",
    L"    ID       INT NOT NULL",
    L");",
    L"",
    L"create table acdbtext (",
    L"    ID       INT NOT NULL",
    L");",
    L"",
    L"create table aeccdbalignment (",
    L"    ID       INT NOT NULL",
    L");",
    L"",
    L"create table aeccdbsurface (",
    L"    ID       INT NOT NULL",
    L");",
    L"",
    L"create table conductor (",
    L"    ID       INT NOT NULL,",
    L"    COST     DECIMAL(10,2) NOT NULL",
    L");",
    L"",
    L"create table transformer (",
    L"    FEATID     BIGINT NOT NULL,",
    L"    PHASE        CHAR(1),",
    L"    VOLTAGE      DECIMAL(10, 2)",
    L");",
    L"",
    L"create table electricdevice_acdbentity (",
    L"    ELECTRICDEVICE_ID       INT NOT NULL,",
    L"    ID                      INT NOT NULL,",
    L"    LAYER                   VARCHAR(255)",
    L");",
    L"",
    L"create table address (",
    L"    NUM                     SMALLINT NOT NULL, ",
    L"    STREET                  VARCHAR(50) NOT NULL, ",
    L"    CITY                    VARCHAR(50) NOT NULL,",
    L"    OWNER                   VARCHAR(100)",
    L");",
    L"",
    L"create table addressaddid (",
    L"    NUM                     SMALLINT NOT NULL, ",
    L"    STREET                  VARCHAR(50) NOT NULL, ",
    L"    CITY                    VARCHAR(50) NOT NULL,",
    L"    OWNER                   VARCHAR(100),",
    L"    PROVINCE                VARCHAR(50) NOT NULL",
    L");",
    L"",
    L"create table addressbadid (",
    L"    NUM                     SMALLINT NOT NULL, ",
    L"    STREET                  VARCHAR(50) NOT NULL, ",
    L"    CITY                    VARCHAR(50) NOT NULL,",
    L"    OWNER                   VARCHAR(100)",
    L");",
    L"",
    L"create table addressinhid (",
    L"    NUM                     SMALLINT NOT NULL, ",
    L"    CITY                    VARCHAR(50) NOT NULL,",
    L"    OWNER                   VARCHAR(50)",
    L");",
    L"",
    L"create table customer (",
    L"    NAME                    VARCHAR(100) NOT NULL,",
    L"    PROVINCE                VARCHAR(50) NOT NULL",
    L");",
    L"",
    L"create table customer_hqaddresses (",
    L"    NUM                     SMALLINT NOT NULL, ",
    L"    STREET                  VARCHAR(50) NOT NULL, ",
    L"    CITY                    VARCHAR(50) NOT NULL,",
    L"    OWNER                   VARCHAR(100),",
    L"    CUSTOMER_NAME           VARCHAR(100) NOT NULL,",
    L"    CUSTOMER_PROVINCE       VARCHAR(50) NOT NULL",
    L");",
    L"",
    L"create table customer_retailaddresses (",
    L"    NUM                     SMALLINT NOT NULL, ",
    L"    STREET                  VARCHAR(50) NOT NULL, ",
    L"    CITY                    VARCHAR(50) NOT NULL,",
    L"    OWNER                   VARCHAR(100),",
    L"    CUSTOMER_NAME           VARCHAR(100) NOT NULL,",
    L"    CUSTOMER_PROVINCE       VARCHAR(50) NOT NULL",
    L");",
    L"",
    L"create table customer_factoryaddresses (",
    L"    NUM                     SMALLINT NOT NULL, ",
    L"    STREET                  VARCHAR(50) NOT NULL, ",
    L"    CITY                    VARCHAR(50) NOT NULL,",
    L"    OWNER                   VARCHAR(100),",
    L"    CUSTOMER_NAME           VARCHAR(100) NOT NULL,",
    L"    SEQ                     INT NOT NULL,",
    L"    CUSTOMER_PROVINCE       VARCHAR(50) NOT NULL",
    L");",
    NULL
    };

    return theSchema;
}

const wchar_t* DescribeSchemaTest::mData[] = {
    L"",
    L"delete from f_schemainfo where schemaname = 'Acad';",
    L"",
    L"insert into f_schemainfo ( schemaname, description, creationdate, owner, ",
    L"schemaversionid ) values ( 'Acad', null, '03-10-07', 'br', 1 );                          ",
    L"                                                                                ",
    L"insert into f_schemainfo ( schemaname, description, creationdate, owner, ",
    L"schemaversionid ) values ( 'Electric', 'Electric Utility Model', '03-10-07', 'br', 1 );   ",
    L"                                                                                                                                                                ",
    L"insert into f_schemainfo ( schemaname, description, creationdate, owner, ",
    L"schemaversionid ) values ( 'Vine', 'Vine schema', '03-10-16', 'br', 1 );               ",
    L"                                                                                ",
    L"insert into f_schemainfo ( schemaname, description, creationdate, owner, ",
    L"schemaversionid ) values ( 'Error', 'Test schema errors', '03-10-16', 'br', 1 );       ",
    L"                                                                                ",
    L"insert into f_schemainfo ( schemaname, description, creationdate, owner, ",
    L"schemaversionid ) values ( 'Postal', 'more error tests', '03-10-17', 'br', 1 ); ",
    L"",
    L"delete from f_classdefinition where schemaname = 'Acad';",
    L"",
    L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (101, 'AcDbEntity', ",
    L"'Acad', 'AcDbEntity', 2, null, 0, null );                                                             ",
    L"                                                                                ",
    L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (102, 'AcDbLine', ",
    L"'Acad', 'AcDbLine', 2, null, 0, 'AcDbEntity' );                                                     ",
    L"                                                                                ",
    L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (103, 'ElectricDevice', ",
    L"'Electric', 'ElectricDevice', 2, null, 1, null );                                                     ",
    L"",
    L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (104, 'Conductor', ",
    L"'Electric', 'Conductor', 2, 'Electrical Conductor', 0, 'ElectricDevice' );                        ",
    L"                                                                                ",
    L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (105, 'Transformer', ",
    L"'Electric', 'Transformer', 2, null, 0, 'ElectricDevice' );                                          ",
    L"                                                                                ",
    L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (106, 'AcXData', ",
    L"'Acad', 'AcXData', 1, null, 0, null );                                                                ",
    L"                                                                                ",
    L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (107, 'AbstractText', ",
    L"'Acad', 'AcDbText', 2, 'text base class', 1, 'AcDbEntity' );                                  ",
    L"                                                                                ",
    L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (108, 'AcDb3dPolyline', ",
    L"'Acad', 'AcDb3dPolyline', 2, '3d polyline', 0, 'Acad:AcDbEntity' );                               ",
    L"                                                                                ",
    L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (109, ",
    L"'AcDbBlockAttribute', 'Acad', 'AcDbBlockAttribute', 2, null, 0, 'AcDbEntity' );                                           ",
    L"                                                                                ",
    L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (110, 'AcDbHatch', ",
    L"'Acad', 'AcDbHatch', 2, 'a hatched polygon', 0, 'AcDbEntity' );                                  ",
    L"                                                                                ",
    L"insert into f_classdefinition ( classid,  classname,schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (111, 'AcDbMText', ",
    L"'Acad', 'AcDbText', 2, 'multiline text', 0, 'AbstractText' );                                   ",
    L"                                                                                ",
    L"",
    L"insert into f_classdefinition ( classid,  classname,schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (112, 'AcDbText', ",
    L"'Acad', 'AcDbText', 2, 'single line text', 0, 'AbstractText' );                                  ",
    L"                                                                                ",
    L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (113, ",
    L"'AeccDbAlignment', 'Vine', 'AeccDbAlignment', 2, null, 0, 'Acad:AcDbEntity' );                                        ",
    L"                                                                                ",
    L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (114, 'AeccDbSurface', ",
    L"'Vine', 'AeccDbSurface', 2, null, 0, 'Acad:AcDbEntity' );                                          ",
    L"",
    L"                                                                                ",
    L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (115, 'BadTable', ",
    L"'Error', 'BadTable', 1, 'Table does not exist', 0, null );                                         ",
    L"                                                                                ",
    L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (116, 'LoopA', 'Error', ",
    L"'LoopA', 1, null, 0, 'LoopC' );                                                           ",
    L"                                                                                ",
    L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (117, 'LoopB', 'Error', ",
    L"'LoopB', 1, null, 0, 'LoopA' );                                                           ",
    L"                                                                                ",
    L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (118, 'LoopC', ",
    L"'Error', 'LoopC', 1, null, 0, 'LoopB' );                                                           ",
    L"                                                                                ",
    L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (119, 'LoopD', ",
    L"'Error', 'LoopD', 1, null, 0, 'LoopC' );                                                           ",
    L"                                                                                ",
    L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (120, 'BadParent', ",
    L"'Error', 'BadParent', 1, 'parent class does not exist', 0, 'NoClass' );                          ",
    L"                                                                                ",
    L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (122, 'AddressAddId', ",
    L"'Postal', 'AddressAddId', 1, null, 0, 'Address' );                                                 ",
    L"                                                                                ",
    L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (121, 'Address', ",
    L"'Postal', 'Address', 1, null, 1, null );                                                             ",
    L"                                                                                ",
    L"",
    L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (123, 'AddressBadId', ",
    L"'Postal', 'AddressBadId', 1, null, 0, null );                                                        ",
    L"                                                                                ",
    L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (124, 'AddressInhId', ",
    L"'Postal', 'AddressInhId', 1, null, 0, 'Address' );                                                 ",
    L"                                                                                ",
    L"insert into f_classdefinition ( classid, classname, schemaname, tablename, ",
    L"classtype, description, isabstract, parentclassname ) values (125, 'Customer', ",
    L"'Postal', 'Customer', 1, null, 0, null );                                                            ",
    L"",
    L"delete from f_attributedefinition where classid >= 100;       ",
    L"                                                                         ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, issystem, ",
    L"isreadonly, sequencename, owner, description ) values ( 'AcDbEntity', 101, 'layer', ",
    L"'layer', 'string', 255, 0, 'string', 1, 0, 0, 0, null, null, 'Autocad layer' );      ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description ) values ( 'Transformer', ",
    L"105, 'PHASE', 'phase', 'string', 1, 0, 'string', 1, 0, 0, 0, null, null, ",
    L"'an electrical term' );  ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, ",
    L"isnullable, isfeatid, issystem, isreadonly, sequencename, owner, description ) values ",
    L"( 'Transformer', 105, 'VOLTAGE', 'voltage', 'double', 0, 0, 'double', 1, 0, 0, 0, ",
    L"null, null, 'number of volts' ); ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description ) values ( 'Conductor', ",
    L"104, 'Cost', 'Cost', 'NUMBER', 10, 2, 'decimal', 1, 0, 0, 0, null, null, ",
    L"'Installation Cost' );     ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description ) values ( ",
    L"'ElectricDevice_AcDbEntity', 103, 'graphic', 'graphic', 'Acad:AcDbEntity', 0, 0, ",
    L"'Acad:AcDbEntity', 1, 0, 0, 0, null, null, null );                                                                ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, issystem, ",
    L"isreadonly, sequencename, owner, description ) values ( 'AcXData', 106, 'datavalue', ",
    L"'DataValue', 'VARCHAR2', 4000, 0, 'string', 1, 0, 0, 0, null, null, null );           ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description ) values ( 'AcDb3dPolyline', ",
    L"108, 'layer', 'layer', 'string', 255, 0, 'string', 1, 0, 0, 0, null, null, ",
    L"'Autocad layer' );  ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, issystem,",
    L"isreadonly, sequencename, owner, description ) values ( 'AcDbEntity_AcXData', 101, ",
    L"'xdata', 'xdata', 'AcXData', 0, 0, 'AcXData', 1, 0, 0, 0, null, null, null );           ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description ) values ( 'AcDbEntity_AcXData', ",
    L"102, 'xdata', 'xdata', 'AcXData', 0, 0, 'AcXData', 1, 0, 0, 0, null, null, null );           ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description ) values ( ",
    L"'AcDbEntity_AcXData', 102, 'acdbentity_id', 'xdata.acdbentity_id', 'NUMBER', 10, 0, ",
    L"'int32', 1, 1, 0, 0, null, null, null );                                                                       ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, issystem, ",
    L"isreadonly, sequencename, owner, description ) values ( 'AcDbEntity_AcXData', 102, ",
    L"'seq', 'xdata.seq', 'NUMBER', 10, 0, 'int32', 1, 0, 0, 0, null, null, null );           ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description ) values ( 'AcDbEntity_AcXData',",
    L"102, 'datavalue', 'xdata.datavalue', 'VARCHAR2', 4000, 0, 'string', 1, 0, 0, 0, ",
    L"null, null, null );                                                                          ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description ) values ( 'AcDbEntity_AcXData', ",
    L"101, 'seq', 'xdata.seq', 'NUMBER', 10, 0, 'int32', 1, 0, 0, 0, null, null, null );           ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description, idposition ) values ( 'AcDbEntity', 101, ",
    L"'id', 'Id', 'NUMBER', 10, 0, 'int32', 1, 1, 0, 0, null, null, null, 1 );                           ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, attributename",
    L", columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, issystem, isread",
    L"only, sequencename, owner, description ) values ( 'AcDbXData', 106, 'seq', 'seq', ",
    L"'NUMBER', 10, 0, 'int32', 1, 0, 0, 0, null, null, null );                          ",
    L"",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description, idposition ) values ( 'Address', 121, ",
    L"'num', 'Number', 'NUMBER', 5, 0, 'int16', 1, 0, 0, 0, null, null, null, 1 );                         ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description, idposition ) values ( 'Address', 121, ",
    L"'street', 'Street', 'VARCHAR2', 50, 0, 'string', 1, 0, 0, 0, null, null, null, 2 );                  ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description, idposition ) values ( 'Address', 121, ",
    L"'city', 'City', 'VARCHAR2', 50, 0, 'string', 1, 0, 0, 0, null, null, null, 3 );                      ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description, idposition ) values ( 'AddressAddId',",
    L" 122, 'city', 'City', 'VARCHAR2', 50, 0, 'string', 1, 0, 0, 0, null, null, null, 3 );                 ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description, idposition ) values ( 'AddressAddId', 122, ",
    L"'num', 'Number', 'NUMBER', 5, 0, 'int16', 1, 0, 0, 0, null, null, null, 1 );                    ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, attributename",
    L", columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, issystem, isread",
    L"only, sequencename, owner, description, idposition ) values ( 'AddressAddId', 122, 'street', ",
    L"'Street', 'VARCHAR2', 50, 0, 'string', 1, 0, 0, 0, null, null, null, 2 );             ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, attributename",
    L", columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, issystem, isread",
    L"only, sequencename, owner, description, idposition ) values ( 'AddressBadId', 123, 'city', 'C",
    L"ity', 'VARCHAR2', 50, 0, 'string', 1, 0, 0, 0, null, null, null, 3 );                 ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description, idposition ) values ( 'AddressBadId', ",
    L"123, 'num', 'Number', 'NUMBER', 5, 0, 'int16', 1, 0, 0, 0, null, null, null, 1 );                    ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description, idposition ) values ( 'Customer', 125, ",
    L"'Province', 'Province', 'VARCHAR2', 50, 0, 'string', 1, 0, 0, 0, null, null, null, 2 );             ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description, idposition ) values ( 'AddressAddId', ",
    L"122, 'province', 'Province', 'VARCHAR2', 50, 0, 'string', 1, 0, 0, 0, null, null, null, 4 );         ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description ) values ( 'AddressBadId', ",
    L"123, 'AddressType', 'AddressType', 'CHAR', 1, 0, 'string', 1, 0, 0, 1, null, null, null );        ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, issystem, ",
    L"isreadonly, sequencename, owner, description ) values ( 'Transformer_AcXData', 105, ",
    L"'xdata', 'xdata', 'AcXData', 0, 0, 'AcXData', 1, 0, 0, 0, null, null, null );          ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description, idposition ) values ( 'Customer', ",
    L"125, 'Name', 'Name', 'VARCHAR2', 100, 0, 'string', 1, 0, 0, 0, null, null, null, 1 );                    ",
    L"",
    L"insert into f_attributedefinition ( tablename, classid, columnname, attributename",
    L", columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, issystem, isread",
    L"only, sequencename, owner, description ) values ( 'Customer_HqAddresses', 125, 'H",
    L"qAddresses', 'HqAddresses', 'Address', 0, 0, 'Address', 1, 0, 0, 0, null, null, '",
    L"' );                                                                            ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, issystem, ",
    L"isreadonly, sequencename, owner, description ) values ( 'Customer_RetailAddresses', ",
    L"125, 'RetailAddresses', 'RetailAddresses', 'Address', 0, 0, 'Address', 1, 0, ",
    L"0, 0, null, null, null );                                                                ",
    L"",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description ) values ( ",
    L"'Customer_FactoryAddresses', 125, 'FactoryAddresses', 'FactoryAddresses', 'Address', ",
    L"0, 0, 'Address', 1, 0, 0, 0, null, null, null );                                                             ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description ) values ( ",
    L"'Customer_StorageAddresses', 125, 'storageAddresses', 'storageAddresses', 'Address', ",
    L"0, 0, 'Address', 1, 0, 0, 0, null, null, null );                                                             ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description ) values ( 'Transformer', ",
    L"105, 'EntityId', 'EntityId', 'NUMBER', 38, 8, 'double', 1, 0, 0, 0, null, null, null );             ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description ) values ( 'F_GEOMETRY_0', ",
    L"103, 'RDBMS_GEOM', 'Geometry', 'SDO_GEOMETRY', 0, 0, '2', 1, 0, 0, ",
    L"0, null, null, '' );                                                                            ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, ",
    L"isnullable, isfeatid, issystem, isreadonly, sequencename, owner, description ) values ",
    L"( 'F_GEOMETRY_0', 109, 'RDBMS_GEOM', 'InsertPoint', 'SDO_GEOMETRY', 0, 0, '1', 1, ",
    L"0, 0, 0, null, null, null );   ",
    L"",
    L"insert into f_attributedependencies ( pktablename, pkcolumnnames, fktablename, fkcolumnnames, fkcardinality, identitycolumn, ordertype ) values ( 'Conductor', ' ', 'ElectricDevice_AcDbEntity', ' ', 1, '', 'a');                                                                                                           ",
    L"insert into f_attributedependencies ( pktablename, pkcolumnnames, fktablename, fkcolumnnames, fkcardinality, identitycolumn, ordertype ) values ( 'AcDbEntity', 'Id', 'AcDbEntity_AcXData', 'AcDbEntity_id', -1, 'seq', 'a');                                                                                                ",
    L"insert into f_attributedependencies ( pktablename, pkcolumnnames, fktablename, fkcolumnnames, fkcardinality, identitycolumn, ordertype ) values ( 'AcDbLine', 'Id', 'AcDbEntity_AcXData', 'AcDbEntity_id', -1, 'seq', 'a');                                                                                                  ",
    L"insert into f_attributedependencies ( pktablename, pkcolumnnames, fktablename, fkcolumnnames, fkcardinality, identitycolumn, ordertype ) values ( 'Customer', 'Name, Province', 'Customer_HqAddresses', 'Customer_Name, Customer_Province', -1, '', 'a');                                                                    ",
    L"insert into f_attributedependencies ( pktablename, pkcolumnnames, fktablename, fkcolumnnames, fkcardinality, identitycolumn, ordertype ) values ( 'Customer', 'NoName', 'Customer_RetailAddresses', 'Customer_Name', -1, 'seq', 'a');                                                                                        ",
    L"insert into f_attributedependencies ( pktablename, pkcolumnnames, fktablename, fkcolumnnames, fkcardinality, identitycolumn, ordertype ) values ( 'Customer', 'Name', 'Customer_FactoryAddresses', 'Customer_NoName', -1, 'seq', 'a');                                                                                       ",
    L"",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description ) values ( 'F_GEOMETRY_0',",
    L"107, 'RDBMS_GEOM', 'InsertPoint', 'SDO_GEOMETRY', 0, 0, '1', 1, 0, 0, 0, null,",
    L"null, null );   ",
    L"                                                                                ",
    L"insert into f_attributedefinition ( tablename, classid, columnname, ",
    L"attributename, columntype, columnsize, columnscale, attributetype, isnullable, isfeatid, ",
    L"issystem, isreadonly, sequencename, owner, description ) values ( 'F_GEOMETRY_0', ",
    L"107, 'C_1', 'Position', 'SDO_GEOMETRY', 0, 0, '3', 1, 0, 0, 0, null, null, null );",
    L"",
    L"delete from f_sad; ",
    L"                                                                                ",
    L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
    L" values ( 'Electric', 'Electric', 'schema', 'Author', 'Thomas Edison' ); ",
    L"",
    L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
    L" values ( 'Electric', 'Electric', 'schema', 'Sharable', 'Yes' ); ",
    L"",
    L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
    L" values ( 'Vine', 'Vine', 'schema', 'Author', 'Tarzan' ); ",
    L"",
    L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
    L" values ( 'Acad', 'AbstractText', 'class', 'Entity', 'Yes' ); ",
    L"",
    L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
    L" values ( 'Acad', 'AbstractText', 'class', 'Text', 'Yes' ); ",
    L"",
    L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
    L" values ( 'Acad', 'AbstractText', 'class', 'Encoding', 'ASCII' ); ",
    L"",
    L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
    L" values ( 'Acad', 'AcDbEntity', 'class', 'Entity', 'Yes' ); ",
    L"",
    L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
    L" values ( 'Electric', 'Conductor', 'class', 'Construction Template', 'linear' ); ",
    L"",
    L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
    L" values ( 'Electric', 'ElectricDevice', 'class', 'Construction Template', 'general' ); ",
    L"",
    L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
    L" values ( 'Electric', 'ElectricDevice', 'class', 'Maintenance Template', 'standard' ); ",
    L"",
    L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
    L" values ( 'Electric', 'Transformer', 'class', 'Construction Template', 'point' ); ",
    L"",
    L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
    L" values ( 'Acad:AcDbEntity', 'layer', 'property', 'Classified', 'Yes' ); ",
    L"",
    L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
    L" values ( 'Acad:AcDbEntity', 'xdata', 'property', 'LinkList', 'Yes' ); ",
    L"",
    L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
    L" values ( 'Acad:AcDbEntity', 'xdata', 'property', 'Ordered', 'No' ); ",
    L"",
    L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
    L" values ( 'Electric:Conductor', 'Cost', 'property', 'Copper', '5' ); ",
    L"",
    L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
    L" values ( 'Electric:Conductor', 'Cost', 'property', 'Aluminum', '3' ); ",
    L"",
    L"insert into f_sad ( ownername, elementname, elementtype, name, value ) ",
    L" values ( 'Electric:Transformer', 'voltage', 'property', 'Formula', 'Ohms Law' ); ",
    L"",
    NULL
};
