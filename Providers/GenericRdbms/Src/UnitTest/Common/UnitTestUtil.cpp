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
#include "UnitTestUtil.h"
#include "ConnectionUtil.h"
#include "FdoInsertTest.h"
#include "XmlFormatter.h"
//#include "../Fdo/DataStore/FdoRdbmsDeleteDataStore.h"
//#include "../Fdo/DataStore/FdoRdbmsCreateDatastore.h"
#include <Sm/Ph/Rd/DbObjectReader.h>
#include <TestCommon.h>

char *UnitTestUtil::CommandFilesDirectory = NULL;
// UnitTestUtil::InfoUtilConnection and UnitTestUtil::ActiveProviderName MUST be declared 
// and initialized in provider unit test
// (setup by each provider)

UnitTestUtil::UnitTestUtil(void)
{
}

UnitTestUtil::~UnitTestUtil(void)
{
    if (NULL != CommandFilesDirectory)
        delete[] CommandFilesDirectory;
}

// The following stylesheet sorts the output XML files so that the Spatial 
// Contexts, Schemas and Schema Mappings are always in the same order. This
// allows the XML files to be compared against corresponding masters, without
// element ordering differences triggering false test failures. 
/*
*/

char* UnitTestUtil::pSortLpSheet = 
"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\
<xsl:stylesheet version=\"1.0\" \
xmlns=\"http:/www.autodesk.com/isd/fdo/GenericLogicalPhysical\" \
xmlns:lp=\"http:/www.autodesk.com/isd/fdo/GenericLogicalPhysical\" \
xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" \
xmlns:gml=\"http://www.opengis.net/gml\" \
xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" \
xmlns:fdo=\"http://www.autodesk.com/isd/fdo\" \
>\
<xsl:template match=\"lp:schemas\">\
    <xsl:copy>\
        <xsl:apply-templates select=\"@*\"/>\
        <xsl:apply-templates select=\"lp:schema\">\
            <xsl:sort select=\"@name\" />\
        </xsl:apply-templates>\
    </xsl:copy>\
</xsl:template>\
<xsl:template match=\"lp:schema\">\
    <xsl:copy>\
        <xsl:apply-templates select=\"@*\"/>\
        <xsl:apply-templates select=\"lp:class\">\
            <xsl:sort select=\"@name\" />\
        </xsl:apply-templates>\
        <xsl:apply-templates select=\"lp:SAD\"/>\
    </xsl:copy>\
</xsl:template>\
<xsl:template match=\"lp:properties\">\
    <xsl:copy>\
        <xsl:apply-templates select=\"@*\"/>\
        <xsl:apply-templates select=\"lp:property\"  >\
            <xsl:sort select=\"@name\" />\
        </xsl:apply-templates>\
    </xsl:copy>\
</xsl:template>\
<xsl:template match=\"lp:uniqueConstraints\">\
  <xsl:copy>\
    <xsl:apply-templates select=\"lp:uniqueConstraint\">\
		<xsl:sort select=\"count(lp:property)\"/>\
    </xsl:apply-templates>\
  </xsl:copy>\
</xsl:template>\
<xsl:template match=\"lp:uniqueConstraint\">\
  <xsl:copy>\
    <xsl:apply-templates select=\"lp:property\">\
		<xsl:sort select=\"@name\"/>\
    </xsl:apply-templates>\
  </xsl:copy>\
</xsl:template>\
<xsl:template match=\"lp:table\">\
    <xsl:copy>\
        <xsl:apply-templates select=\"@*\"/>\
        <xsl:apply-templates select=\"lp:property\">\
            <xsl:sort select=\"@name\" />\
        </xsl:apply-templates>\
        <xsl:apply-templates select=\"lp:column\">\
            <xsl:sort select=\"@name\" />\
        </xsl:apply-templates>\
    </xsl:copy>\
</xsl:template>\
<xsl:template match=\"@*|node()\">\
  <xsl:copy>\
    <xsl:apply-templates select=\"@*|node()\"/>\
  </xsl:copy>\
</xsl:template>\
</xsl:stylesheet>";

void UnitTestUtil::Sql2Db(const wchar_t** sCommands, FdoIConnection* connection)
{

    wchar_t sqlBuffer[5000];
    wchar_t lineBuffer[5000];

    const wchar_t** pCmd;

    sqlBuffer[0] = (wchar_t) 0;

    // for each element in the commands array.
    // (Stop when the NULL element is reached.
    for ( pCmd = sCommands; (*pCmd); pCmd++ ) {
        wcscpy( lineBuffer, *pCmd );
        size_t len = wcslen(lineBuffer);

        // knock off trailing spaces or returns.
        for ( size_t i = (len - 1); (lineBuffer[i] == ' ' || lineBuffer[i] == '\n' ) && (i >= 0); i-- )
            lineBuffer[i] = (wchar_t) 0;

        if ( wcslen(lineBuffer) > 0 ) {
            // Add the element string to the current command.
            wcscat( sqlBuffer, lineBuffer );

            if ( lineBuffer[wcslen(lineBuffer)-1] == ';' ) {
                // ; found so execute the current command.
                sqlBuffer[wcslen(sqlBuffer)-1] = (wchar_t) 0;
                Sql2Db( sqlBuffer, connection );
                sqlBuffer[0] = (wchar_t) 0;
            }
        }
    }

    if ( wcslen(sqlBuffer) > 0 )
        Sql2Db( sqlBuffer, connection );

}

void UnitTestUtil::Sql2Db(const wchar_t* sCommand, FdoIConnection* connection)
{
    FdoPtr<FdoISQLCommand> sqlCmd = (FdoISQLCommand*)connection->CreateCommand( FdoCommandType_SQLCommand );
    sqlCmd->SetSQLStatement(sCommand);
    sqlCmd->ExecuteNonQuery();
}

void UnitTestUtil::CreateAcadSchema( FdoIConnection* connection, bool useBaseMapping )
{
    FdoIApplySchema*  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
    FdoPtr<FdoRdbmsOvPhysicalSchemaMapping>pOverrides;
    FdoPtr<FdoRdbmsOvClassDefinition>pOvClass;
    SchemaOverrideUtilP overrideUtil = NewSchemaOverrideUtil();

    if( useBaseMapping )
    {
        pOverrides =  (FdoRdbmsOvPhysicalSchemaMapping*) connection->CreateSchemaMapping();
        pOverrides->SetTableMapping( FdoSmOvTableMappingType_BaseTable  );
        pCmd->SetPhysicalMapping( pOverrides );
    }
    FdoPtr<FdoFeatureSchemaCollection> pSchemas = FdoFeatureSchemaCollection::Create(NULL);

    FdoFeatureSchema* pSchema = FdoFeatureSchema::Create( L"Acad", L"AutoCAD schema" );

    pSchemas->Add( pSchema );

// Create non-feature class
    FdoClass* pObjectClass = FdoClass::Create( L"ObjectClass", L"ObjectClass" );
    pObjectClass->SetIsAbstract(false);

    FdoDataPropertyDefinition* pObjectClassWeight = FdoDataPropertyDefinition::Create( L"ObjectWeight", L"weight" );
    pObjectClassWeight->SetDataType( FdoDataType_Int32 );
    pObjectClassWeight->SetNullable(false);
    pObjectClass->GetProperties()->Add( pObjectClassWeight );
    FDO_SAFE_RELEASE(pObjectClassWeight);

    pSchema->GetClasses()->Add( pObjectClass );

    FdoClass* pObjectClass2 = FdoClass::Create( L"ObjectClass2", L"ObjectClass2" );
    pObjectClass2->SetIsAbstract(false);

    FdoDataPropertyDefinition* pObjectClassColour2 = FdoDataPropertyDefinition::Create( L"Colours", L"colours" );
    //pObjectClassColour2->SetDataType( FdoDataType_String );
    //pObjectClassColour2->SetLength(32);
    pObjectClassColour2->SetDataType(FdoDataType_Int32);
    pObjectClassColour2->SetNullable(false);
    pObjectClass2->GetProperties()->Add( pObjectClassColour2 );
//  FDO_SAFE_RELEASE(pObjectClassColour2);

    pSchema->GetClasses()->Add( pObjectClass2 );

    FdoClass* testClass = FdoClass::Create(L"testClass", L"Test class");
    testClass->SetIsAbstract(false);
    FdoDataPropertyDefinition *last = FdoDataPropertyDefinition::Create(L"LastName", L"last name");
    last->SetDataType(FdoDataType_String);
    last->SetLength(32);
    last->SetNullable(false);
    testClass->GetProperties()->Add(last);
    testClass->GetIdentityProperties()->Add(last);
    FDO_SAFE_RELEASE(last);

    FdoDataPropertyDefinition *first = FdoDataPropertyDefinition::Create(L"FirstName", L"first name");
    first->SetDataType(FdoDataType_String);
    first->SetLength(32);
    first->SetNullable(false);
    testClass->GetProperties()->Add(first);
    testClass->GetIdentityProperties()->Add(first);
    FDO_SAFE_RELEASE(first);

    FdoDataPropertyDefinition *age = FdoDataPropertyDefinition::Create(L"Age", L"age");
    age->SetDataType(FdoDataType_Int32);
    age->SetNullable(false);
    testClass->GetProperties()->Add(age);
    FDO_SAFE_RELEASE(age);

    FdoObjectPropertyDefinition* pObjPropData = FdoObjectPropertyDefinition::Create( L"Object", L"object property" );
    pObjPropData->SetClass( pObjectClass );
    pObjPropData->SetObjectType( FdoObjectType_Value );
    testClass->GetProperties()->Add( pObjPropData );
    FDO_SAFE_RELEASE(pObjPropData);

    if( useBaseMapping && overrideUtil )
    {
        // Object Property overrides
        pOvClass = overrideUtil->CreateOvClassDefinition( L"testClass" );
        pOvClass->SetTableMapping( FdoSmOvTableMappingType_BaseTable );
        overrideUtil->ClassesOvAdd(pOverrides, pOvClass);
        FdoRdbmsOvObjectPropertyP pObProp = overrideUtil->CreateOvObjectPropertyDefinition( L"Object" );
        overrideUtil->PropertiesOvAdd(pOvClass, pObProp);
        FdoRdbmsOvPropertyMappingSingleP mapping = overrideUtil->CreateOvPropertyMappingSingle();
        overrideUtil->ObjectPropertyOvSetMappingDefinition(pObProp, mapping);
        mapping->SetPrefix( L"OvTest" );
    }

    FdoPtr<FdoObjectPropertyDefinition>pObjPropData2 = FdoObjectPropertyDefinition::Create( L"Object2", L"object property" );
    pObjPropData2->SetClass( pObjectClass2 );
    pObjPropData2->SetIdentityProperty(pObjectClassColour2);
    pObjPropData2->SetObjectType( FdoObjectType_OrderedCollection );
    testClass->GetProperties()->Add( pObjPropData2 );

    FDO_SAFE_RELEASE(pObjectClassColour2);

    pSchema->GetClasses()->Add( testClass );



    // Create a class to be used for Xdata class object property
    FdoClass* pXObjLc = FdoClass::Create( L"AcXObj", L"class uned to create an obj prop for xdata class" );
    pXObjLc->SetIsAbstract(false);

    FdoPtr<FdoDataPropertyDefinition>pXProp = FdoDataPropertyDefinition::Create( L"intdata", L"int data prop" );
    pXProp ->SetDataType( FdoDataType_Int32 );
    pXProp ->SetNullable(true);
    pXObjLc->GetProperties()->Add( pXProp  );

    pXProp = FdoDataPropertyDefinition::Create( L"strdata", L"str data prop" );
    pXProp->SetDataType( FdoDataType_String );
    pXProp->SetLength(4000);
    pXProp->SetNullable(true);
    pXObjLc->GetProperties()->Add( pXProp );

    pXProp = FdoDataPropertyDefinition::Create( L"AutoGen1", L"autogenerated property" );
    pXProp->SetDataType( FdoDataType_Int64 );
    pXProp->SetNullable(true);
    pXProp->SetReadOnly(true);
    pXProp->SetIsAutoGenerated(true);
    pXObjLc->GetProperties()->Add( pXProp );

    pXProp = FdoDataPropertyDefinition::Create( L"AutoGen2", L"autogenerated property" );
    pXProp->SetDataType( FdoDataType_Int64 );
    pXProp->SetNullable(true);
    pXProp->SetReadOnly(true);
    pXProp->SetIsAutoGenerated(true);
    pXObjLc->GetProperties()->Add( pXProp );

    pSchema->GetClasses()->Add( pXObjLc );

    // Create the XData class that will be se for an object property
    FdoClass* pXData = FdoClass::Create( L"AcXData", L"Xdata" );
    pXData->SetIsAbstract(false);

    FdoDataPropertyDefinition* pXDataSeq = FdoDataPropertyDefinition::Create( L"seq", L"seq" );
    pXDataSeq ->SetDataType( FdoDataType_Int32 );
    pXDataSeq ->SetNullable(false);
    pXData->GetProperties()->Add( pXDataSeq  );

    FdoDataPropertyDefinition* pProp = FdoDataPropertyDefinition::Create( L"DataValue", L"datavalue" );
    pProp->SetDataType( FdoDataType_String );
    pProp->SetLength(4000);
    pProp->SetNullable(false);
    pXData->GetProperties()->Add( pProp );
    FDO_SAFE_RELEASE(pProp);

    // Add an object property
    pObjPropData2 = FdoObjectPropertyDefinition::Create( L"AcXObj", L"object property" );
    pObjPropData2->SetClass( pXObjLc );
    pObjPropData2->SetObjectType( FdoObjectType_Value );
    pXData->GetProperties()->Add( pObjPropData2 );

    if( useBaseMapping && overrideUtil )
    {
        pOvClass = overrideUtil->CreateOvClassDefinition( L"AcXData" );
        overrideUtil->ClassesOvAdd(pOverrides, pOvClass);
        FdoRdbmsOvObjectPropertyP pObProp = overrideUtil->CreateOvObjectPropertyDefinition( L"AcXObj" );
        overrideUtil->PropertiesOvAdd(pOvClass, pObProp);
        FdoRdbmsOvPropertyMappingSingleP mapping = overrideUtil->CreateOvPropertyMappingSingle();
        overrideUtil->ObjectPropertyOvSetMappingDefinition(pObProp, mapping);
        mapping->SetPrefix( L"OvTest2" );
    }
    pSchema->GetClasses()->Add( pXData );

    FdoClass* pXData2 = FdoClass::Create( L"AcXData2", L"Xdata2" );
    pXData2->SetIsAbstract(false);

    FdoDataPropertyDefinition* pXDataSeq2 = FdoDataPropertyDefinition::Create( L"seq", L"seq" );
    pXDataSeq2->SetDataType( FdoDataType_Int32 );
    pXDataSeq2->SetNullable(false);
    pXData2->GetProperties()->Add( pXDataSeq2  );

    FdoDataPropertyDefinition* pProp2 = FdoDataPropertyDefinition::Create( L"DataValue", L"datavalue" );
    pProp2->SetDataType( FdoDataType_String );
    pProp2->SetLength(4000);
    pProp2->SetNullable(false);
    pXData2->GetProperties()->Add( pProp2 );
    FDO_SAFE_RELEASE(pProp2);

    pSchema->GetClasses()->Add( pXData2 );

    FdoFeatureClass* pEntClass = FdoFeatureClass::Create( L"AcDbEntity", L"AutoCAD entity base class" );
    pEntClass->SetIsAbstract(false);

    pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"id" );
    pProp->SetDataType( FdoDataType_Int64 );
    pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
    pEntClass->GetProperties()->Add( pProp );
    pEntClass->GetIdentityProperties()->Add( pProp );
    FDO_SAFE_RELEASE(pProp);

    pProp = FdoDataPropertyDefinition::Create( L"layer", L"Acad layer" );
    pProp->SetDataType( FdoDataType_String );
    pProp->SetLength(10);
    pProp->SetNullable(true);
    pEntClass->GetProperties()->Add( pProp );
    FDO_SAFE_RELEASE(pProp);

    pProp = FdoDataPropertyDefinition::Create( L"color", L"Acad Color" );
    pProp->SetDataType( FdoDataType_String );
    pProp->SetLength(32);
    pProp->SetNullable(true);
    pEntClass->GetProperties()->Add( pProp );
    FDO_SAFE_RELEASE(pProp);

    pProp = FdoDataPropertyDefinition::Create( L"segcount", L"number of points" );
    pProp->SetDataType( FdoDataType_Int32 );
    pProp->SetNullable(true);
    pEntClass->GetProperties()->Add( pProp );
    FDO_SAFE_RELEASE(pProp);

#if 1
    pProp = FdoDataPropertyDefinition::Create( L"boolean", L"" );
    pProp->SetDataType( FdoDataType_Boolean );
    pProp->SetNullable(true);
    pEntClass->GetProperties()->Add( pProp );
    FDO_SAFE_RELEASE(pProp);

    pProp = FdoDataPropertyDefinition::Create( L"byte", L"" );
    pProp->SetDataType( FdoDataType_Byte );
    pProp->SetNullable(true);
    pEntClass->GetProperties()->Add( pProp );
    FDO_SAFE_RELEASE(pProp);

    pProp = FdoDataPropertyDefinition::Create( L"datetime", L"" );
    pProp->SetDataType( FdoDataType_DateTime );
    pProp->SetNullable(true);
    pEntClass->GetProperties()->Add( pProp );
    FDO_SAFE_RELEASE(pProp);

    pProp = FdoDataPropertyDefinition::Create( L"decimal", L"" );
    pProp->SetDataType( FdoDataType_Decimal );
    pProp->SetPrecision(10);
    pProp->SetScale(2);
    pProp->SetNullable(true);
    pEntClass->GetProperties()->Add( pProp );
    FDO_SAFE_RELEASE(pProp);

    pProp = FdoDataPropertyDefinition::Create( L"double", L"" );
    pProp->SetDataType( FdoDataType_Double );
    pProp->SetNullable(true);
    pEntClass->GetProperties()->Add( pProp );
    FDO_SAFE_RELEASE(pProp);

    pProp = FdoDataPropertyDefinition::Create( L"int16", L"" );
    pProp->SetDataType( FdoDataType_Int16 );
    pProp->SetNullable(true);
    pEntClass->GetProperties()->Add( pProp );
    FDO_SAFE_RELEASE(pProp);

    pProp = FdoDataPropertyDefinition::Create( L"int32", L"" );
    pProp->SetDataType( FdoDataType_Int32 );
    pProp->SetNullable(true);
    pEntClass->GetProperties()->Add( pProp );
    FDO_SAFE_RELEASE(pProp);


    pProp = FdoDataPropertyDefinition::Create( L"int64", L"" );
    pProp->SetDataType( FdoDataType_Int64 );
    pProp->SetNullable(true);
    pEntClass->GetProperties()->Add( pProp );
    FDO_SAFE_RELEASE(pProp);

    pProp = FdoDataPropertyDefinition::Create( L"single", L"" );
    pProp->SetDataType( FdoDataType_Single );
    pProp->SetNullable(true);
    pEntClass->GetProperties()->Add( pProp );
    FDO_SAFE_RELEASE(pProp);

#endif

    FdoObjectPropertyDefinition* pObjProp = FdoObjectPropertyDefinition::Create( L"xdata", L"xdata" );
    pObjProp->SetClass( pXData );
    pObjProp->SetObjectType( FdoObjectType_Value );
	//pObjProp->SetIdentityProperty( pXDataSeq );
    pEntClass->GetProperties()->Add( pObjProp );
    FDO_SAFE_RELEASE(pProp);

    FdoObjectPropertyDefinition* pObjProp2 = FdoObjectPropertyDefinition::Create( L"xdata2", L"xdata" );
    pObjProp2->SetClass( pXData2 );
    pObjProp2->SetIdentityProperty( pXDataSeq2 );
    pObjProp2->SetObjectType( FdoObjectType_OrderedCollection );
    pEntClass->GetProperties()->Add( pObjProp2 );
    FDO_SAFE_RELEASE(pProp2);

    pSchema->GetClasses()->Add( pEntClass );

    FdoFeatureClass* pClass = FdoFeatureClass::Create( L"AcDb3dPolyline", L"AutoCAD 3d polyline" );
    pClass->SetIsAbstract(false);
    pClass->SetBaseClass( pEntClass );


    FdoGeometricPropertyDefinition* pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"location and shape" );
    pGeomProp->SetGeometryTypes( FdoGeometricType_Curve );
    pGeomProp->SetHasElevation( true );
    pClass->GetProperties()->Add( pGeomProp );

    pClass->SetGeometryProperty( pGeomProp );
    FDO_SAFE_RELEASE(pGeomProp);

#if 0
    pProp = FdoDataPropertyDefinition::Create( L"F_SI1", L"Spatial Index 1" );
    pProp->SetDataType( FdoDataType_String );
    pProp->SetLength(50);
    pProp->SetNullable(true);
    pClass->GetProperties()->Add( pProp );
    FDO_SAFE_RELEASE(pProp);

    pProp = FdoDataPropertyDefinition::Create( L"F_SI2", L"Spatial Index 2" );
    pProp->SetDataType( FdoDataType_String );
    pProp->SetLength(50);
    pProp->SetNullable(true);
    pClass->GetProperties()->Add( pProp );
    FDO_SAFE_RELEASE(pProp);
#endif

    pSchema->GetClasses()->Add( pClass );
    FDO_SAFE_RELEASE(pClass);

    FDO_SAFE_RELEASE(pXDataSeq);
    FDO_SAFE_RELEASE(pXData);
    FDO_SAFE_RELEASE(pEntClass);
    pCmd->SetFeatureSchema( pSchema );

    pCmd->Execute();

    FDO_SAFE_RELEASE(pSchema);
    FDO_SAFE_RELEASE(pCmd);
}

void UnitTestUtil::CreateLandSchema( FdoIConnection* connection )
{
    // Version must be incremented each time the following Land schema is updated.
    // This forces a re-create of the Land Schema in existing datastores.
    static wchar_t* currVersion = L"1.9";
    FdoPtr<FdoISchemaCapabilities>	schemaCap = connection->GetSchemaCapabilities();    

    FdoPtr<FdoIDescribeSchema> pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);

    FdoPtr<FdoFeatureSchemaCollection> pSchemas = pDescCmd->Execute();

    // Check if land schema already exists
    FdoPtr<FdoFeatureSchema> pSchema = pSchemas->FindItem( L"L\x00e4nd" );

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
            pCmd->SetSchemaName(L"L\x00e4nd");
            pCmd->Execute();
            pSchema = NULL;
        }
    }

    if ( !pSchema )
    {
        FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

        /* Create a schema to test successful schema deletion */

        FdoPtr<FdoFeatureSchema> pSchema = FdoFeatureSchema::Create( L"L\x00e4nd", L"Property schema" );
        FdoSADP(pSchema->GetAttributes())->Add(L"version", currVersion);

        FdoPtr<FdoClass> pPersonClass = FdoClass::Create( L"Person", L"" );
        pPersonClass->SetIsAbstract(false);

        FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"First N\x00e4me", L"" );
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

        /* Test feature class with 2 id properties not FeatId */

        FdoPtr<FdoFeatureClass> pParcelClass = FdoFeatureClass::Create( L"Parcel", L"land parcel" );
        pParcelClass->SetIsAbstract(false);

        pProp = FdoDataPropertyDefinition::Create( L"Pr\x00f6vince", L"" );
        pProp->SetDataType( FdoDataType_String );
        pProp->SetLength(20);
        pProp->SetNullable(false);
        FdoPropertiesP(pParcelClass->GetProperties())->Add( pProp );
        FdoDataPropertiesP(pParcelClass->GetIdentityProperties())->Add( pProp );

/*
 	    if ( schemaCap->SupportsExclusiveValueRangeConstraints() && schemaCap->SupportsInclusiveValueRangeConstraints())
	    {
		    GisPtr<FdoPropertyValueConstraintRange>  newRangeConstr1 =  FdoPropertyValueConstraintRange::Create();
		    newRangeConstr1->SetMinInclusive(true);

		    GisPtr<FdoDataValue>   val1 = FdoDataValue::Create( L"A" );
		    newRangeConstr1->SetMinValue( val1 );

		    newRangeConstr1->SetMaxInclusive(true);
		    GisPtr<FdoDataValue>   val2 = FdoDataValue::Create( L"z" );
		    newRangeConstr1->SetMaxValue( val2 );
		    pProp->SetValueConstraint(newRangeConstr1);
	    }
*/

        pProp = FdoDataPropertyDefinition::Create( L"PIN", L"parcel id" );
        pProp->SetDataType( FdoDataType_String );
        pProp->SetLength(15);
        pProp->SetNullable(false);
        FdoPropertiesP(pParcelClass->GetProperties())->Add( pProp );
        FdoDataPropertiesP(pParcelClass->GetIdentityProperties())->Add( pProp );

        pProp = FdoDataPropertyDefinition::Create( L"Value", L"" );
        pProp->SetDataType( FdoDataType_Decimal );
        pProp->SetNullable(true);
        pProp->SetPrecision( 8 );
        pProp->SetScale( 0 );
        FdoPropertiesP(pParcelClass->GetProperties())->Add( pProp );

 	    if ( schemaCap->SupportsExclusiveValueRangeConstraints() && schemaCap->SupportsInclusiveValueRangeConstraints())
	    {
		    FdoPtr<FdoPropertyValueConstraintRange>  newRangeConstr1 =  FdoPropertyValueConstraintRange::Create();
		    newRangeConstr1->SetMinInclusive(true);

		    FdoPtr<FdoDataValue>   val1 = FdoDataValue::Create( (double) 0, FdoDataType_Decimal );
		    newRangeConstr1->SetMinValue( val1 );

		    newRangeConstr1->SetMaxInclusive(true);
		    FdoPtr<FdoDataValue>   val2 = FdoDataValue::Create( 100000000, FdoDataType_Decimal );
		    newRangeConstr1->SetMaxValue( val2 );
		    pProp->SetValueConstraint(newRangeConstr1);
	    }

        pProp = FdoDataPropertyDefinition::Create( L"AUTOGEN1", L"1st autogenerated property" );
        pProp->SetDataType( FdoDataType_Int64 );
        pProp->SetNullable(true);
        pProp->SetReadOnly(true);
        pProp->SetIsAutoGenerated(true);
        FdoPropertiesP(pParcelClass->GetProperties())->Add( pProp );

        pProp = FdoDataPropertyDefinition::Create( L"AUTOGEN2", L"2nd autogenerated property" );
        pProp->SetDataType( FdoDataType_Int64 );
        pProp->SetNullable(true);
        pProp->SetReadOnly(true);
        pProp->SetIsAutoGenerated(true);
        FdoPropertiesP(pParcelClass->GetProperties())->Add( pProp );

        FdoPtr<FdoObjectPropertyDefinition> pObjProp = FdoObjectPropertyDefinition::Create( L"owner", L"" );
        pObjProp->SetObjectType( FdoObjectType_Value );
        pObjProp->SetClass( pPersonClass );
        FdoPropertiesP(pParcelClass->GetProperties())->Add( pObjProp );

        FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = FdoGeometricPropertyDefinition::Create( 
            UnitTestUtil::GetNlsObjectName(L"Geometry"),
            L"" 
        );
        pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve);
        FdoPropertiesP(pParcelClass->GetProperties())->Add( pGeomProp );
        pGeomProp->SetHasElevation( true );
        FdoClassesP(pSchema->GetClasses())->Add( pParcelClass );

		FdoPtr<FdoFeatureClass> pParcelAClass = FdoFeatureClass::Create( L"Parcel_A", L"land parcel" );
		pParcelAClass->SetIsAbstract(false);

		pProp = FdoDataPropertyDefinition::Create( L"Name", L"" );
		pProp->SetDataType( FdoDataType_String );
		pProp->SetLength(20);
		pProp->SetNullable(false);
		FdoPropertiesP(pParcelAClass->GetProperties())->Add( pProp );

		pProp = FdoDataPropertyDefinition::Create( L"PIN", L"parcel id" );
		pProp->SetDataType( FdoDataType_String );
		pProp->SetLength(15);
		pProp->SetNullable(false);
		FdoPropertiesP(pParcelAClass->GetProperties())->Add( pProp );
		FdoDataPropertiesP(pParcelAClass->GetIdentityProperties())->Add( pProp );

		pProp = FdoDataPropertyDefinition::Create( L"Value", L"" );
		pProp->SetDataType( FdoDataType_Decimal );
		pProp->SetNullable(true);
		pProp->SetPrecision( 8 );
		pProp->SetScale( 0 );
		FdoPropertiesP(pParcelAClass->GetProperties())->Add( pProp );

 	    if ( schemaCap->SupportsExclusiveValueRangeConstraints() && schemaCap->SupportsInclusiveValueRangeConstraints())
	    {
		    FdoPtr<FdoPropertyValueConstraintRange>  newRangeConstr1 =  FdoPropertyValueConstraintRange::Create();
		    newRangeConstr1->SetMinInclusive(true);

		    FdoPtr<FdoDataValue>   val1 = FdoDataValue::Create( (double) 0, FdoDataType_Decimal );
		    newRangeConstr1->SetMinValue( val1 );

		    newRangeConstr1->SetMaxInclusive(true);
		    FdoPtr<FdoDataValue>   val2 = FdoDataValue::Create( 100000000, FdoDataType_Decimal );
		    newRangeConstr1->SetMaxValue( val2 );
		    pProp->SetValueConstraint(newRangeConstr1);
	    }

		FdoClassesP(pSchema->GetClasses())->Add( pParcelAClass );

        FdoPtr<FdoFeatureClass> pClass = FdoFeatureClass::Create( GetNlsObjectName(L"Industrial Parcel"), L"" );
        pClass->SetIsAbstract(false);
        pClass->SetBaseClass( pParcelClass );

        pProp = FdoDataPropertyDefinition::Create( L"# employees", L"" );
        pProp->SetDataType( FdoDataType_Int32 );
        pProp->SetNullable(false);
        FdoPropertiesP(pClass->GetProperties())->Add( pProp );

        FdoClassesP(pSchema->GetClasses())->Add( pClass );

        /* Test feature class with 1 id property not FeatId */

        FdoPtr<FdoFeatureClass> pCityClass = FdoFeatureClass::Create( L"Municipality", L"City" );
        pParcelClass->SetIsAbstract(false);

        pProp = FdoDataPropertyDefinition::Create( L"Name", L"" );
        pProp->SetDataType( FdoDataType_String );
        pProp->SetLength(50);
        pProp->SetNullable(false);
        FdoPropertiesP(pCityClass->GetProperties())->Add( pProp );
        FdoDataPropertiesP(pCityClass->GetIdentityProperties())->Add( pProp );

        pProp = FdoDataPropertyDefinition::Create( L"Population", L"" );
        pProp->SetDataType( FdoDataType_Int32 );
        pProp->SetNullable(false);
        FdoPropertiesP(pCityClass->GetProperties())->Add( pProp );

 	    if ( schemaCap->SupportsExclusiveValueRangeConstraints() && schemaCap->SupportsInclusiveValueRangeConstraints())
	    {
		    FdoPtr<FdoPropertyValueConstraintRange>  newRangeConstr1 =  FdoPropertyValueConstraintRange::Create();
		    newRangeConstr1->SetMinInclusive(true);

		    FdoPtr<FdoDataValue>   val1 = FdoDataValue::Create( 0 );
		    newRangeConstr1->SetMinValue( val1 );

		    newRangeConstr1->SetMaxInclusive(true);
		    FdoPtr<FdoDataValue>   val2 = FdoDataValue::Create( 100000000 );
		    newRangeConstr1->SetMaxValue( val2 );
		    pProp->SetValueConstraint(newRangeConstr1);
	    }

        pObjProp = FdoObjectPropertyDefinition::Create( L"mayor", L"" );
        pObjProp->SetObjectType( FdoObjectType_Value );
        pObjProp->SetClass( pPersonClass );
        FdoPropertiesP(pCityClass->GetProperties())->Add( pObjProp );

        pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"" );
        pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve );
        pGeomProp->SetHasElevation( true );
        FdoPropertiesP(pCityClass->GetProperties())->Add( pGeomProp );
        pCityClass->SetGeometryProperty( pGeomProp );

        FdoClassesP(pSchema->GetClasses())->Add( pCityClass );

	    FdoFeatureClassP pZoningClass = FdoFeatureClass::Create( L"Zoning", L"land use zone" );
	    pZoningClass->SetIsAbstract(false);

 	    pProp = FdoDataPropertyDefinition::Create( L"ByLaw", L"" );
	    pProp->SetDataType( FdoDataType_String );
	    pProp->SetLength(20);
	    pProp->SetNullable(false);
	    FdoPropertiesP(pZoningClass->GetProperties())->Add( pProp );
	    FdoDataPropertiesP(pZoningClass->GetIdentityProperties())->Add( pProp );

	    pProp = FdoDataPropertyDefinition::Create( L"ZoningType", L"" );
	    pProp->SetDataType( FdoDataType_Int32 );
	    pProp->SetNullable(false);
	    FdoPropertiesP(pZoningClass->GetProperties())->Add( pProp );

 	    if ( schemaCap->SupportsExclusiveValueRangeConstraints() && schemaCap->SupportsInclusiveValueRangeConstraints())
	    {
		    FdoPtr<FdoPropertyValueConstraintRange>  newRangeConstr1 =  FdoPropertyValueConstraintRange::Create();
		    newRangeConstr1->SetMinInclusive(true);

		    FdoPtr<FdoDataValue>   val1 = FdoDataValue::Create( 1 );
		    newRangeConstr1->SetMinValue( val1 );

		    newRangeConstr1->SetMaxInclusive(true);
		    FdoPtr<FdoDataValue>   val2 = FdoDataValue::Create( 20 );
    	    newRangeConstr1->SetMaxValue( val2 );
		    pProp->SetValueConstraint(newRangeConstr1);
	    }

	    FdoClassesP(pSchema->GetClasses())->Add( pZoningClass );

	    pZoningClass = FdoFeatureClass::Create( L"Zoning_A", L"land use zone" );
	    pZoningClass->SetIsAbstract(false);

	    pProp = FdoDataPropertyDefinition::Create( L"ByLaw", L"" );
	    pProp->SetDataType( FdoDataType_String );
	    pProp->SetLength(20);
	    pProp->SetNullable(false);
	    FdoPropertiesP(pZoningClass->GetProperties())->Add( pProp );
	    FdoDataPropertiesP(pZoningClass->GetIdentityProperties())->Add( pProp );

	    pProp = FdoDataPropertyDefinition::Create( L"ZoningType", L"" );
	    pProp->SetDataType( FdoDataType_Int32 );
	    pProp->SetNullable(false);
	    FdoPropertiesP(pZoningClass->GetProperties())->Add( pProp );

 	    if ( schemaCap->SupportsExclusiveValueRangeConstraints() && schemaCap->SupportsInclusiveValueRangeConstraints())
	    {
		    FdoPtr<FdoPropertyValueConstraintRange>  newRangeConstr1 =  FdoPropertyValueConstraintRange::Create();
		    newRangeConstr1->SetMinInclusive(true);

		    FdoPtr<FdoDataValue>   val1 = FdoDataValue::Create( 21 );
		    newRangeConstr1->SetMinValue( val1 );

		    newRangeConstr1->SetMaxInclusive(true);
		    FdoPtr<FdoDataValue>   val2 = FdoDataValue::Create( 40 );
		    newRangeConstr1->SetMaxValue( val2 );
		    pProp->SetValueConstraint(newRangeConstr1);
	    }

	    FdoFeatureClassP pBldgClass = FdoFeatureClass::Create( L"Building", L"" );
	    FdoClassesP(pSchema->GetClasses())->Add( pBldgClass );
	    pBldgClass->SetIsAbstract(false);

	    pProp = FdoDataPropertyDefinition::Create( L"ID\x00e4", L"" );
	    pProp->SetDataType( FdoDataType_Int64 );
	    pProp->SetNullable(false);
        pProp->SetIsAutoGenerated(true);
        FdoPropertiesP(pBldgClass->GetProperties())->Add( pProp );
	    FdoDataPropertiesP(pBldgClass->GetIdentityProperties())->Add( pProp );

	    pProp = FdoDataPropertyDefinition::Create( L"Value", L"" );
	    pProp->SetDataType( FdoDataType_String );
	    pProp->SetNullable(true);
        pProp->SetLength(10);
        FdoPropertiesP(pBldgClass->GetProperties())->Add( pProp );

        pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"" );
        pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve );
        pGeomProp->SetHasElevation( true );
        FdoPropertiesP(pBldgClass->GetProperties())->Add( pGeomProp );
        pBldgClass->SetGeometryProperty( pGeomProp );

        pCmd->SetFeatureSchema( pSchema );
        pCmd->Execute();
    }
}


void UnitTestUtil::CreateNonUniqueSchema( FdoIConnection* connection )
{
    // Version must be incremented each time the following Land schema is updated.
    // This forces a re-create of the NonUnique Schema in existing datastores.
    static wchar_t* currVersion = L"1.4";

    FdoPtr<FdoIDescribeSchema> pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);

    FdoPtr<FdoFeatureSchemaCollection> pSchemas = pDescCmd->Execute();

    // Check if NonUnique schema exists.
    FdoPtr<FdoFeatureSchema> pSchema = pSchemas->FindItem( L"NonUnique" );

    // If it does, check if it has the current version. The version is kept in
    // the Schema Attribute dictionary.

    if ( pSchema ) {
        if ( (!FdoSADP(pSchema->GetAttributes())->ContainsAttribute(L"version")) ||
             (wcscmp(FdoSADP(pSchema->GetAttributes())->GetAttributeValue(L"version"), currVersion) != 0) ) {

            // No version or wrong version, destroy and re-create the schema.

            FdoPtr<FdoIDestroySchema>  pCmd = (FdoIDestroySchema*) connection->CreateCommand(FdoCommandType_DestroySchema);
            pCmd->SetSchemaName(L"NonUnique");
            pCmd->Execute();
            pSchema = NULL;
        }
    }

    if ( !pSchema )
    {
        FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

        /* Create a schema to test successful schema deletion */

        FdoPtr<FdoFeatureSchema> pSchema = FdoFeatureSchema::Create( L"NonUnique", L"has classes with same names as classes in other schemas" );
        FdoSADP(pSchema->GetAttributes())->Add(L"version", currVersion);

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

        /* Test feature class with 2 id properties not FeatId */

        FdoPtr<FdoFeatureClass> pParcelClass = FdoFeatureClass::Create( L"Parcel", L"land parcel" );
        pParcelClass->SetIsAbstract(false);

        pProp = FdoDataPropertyDefinition::Create( L"Pr\x00f6vince", L"" );
        pProp->SetDataType( FdoDataType_String );
        pProp->SetLength(20);
        pProp->SetNullable(false);
        FdoPropertiesP(pParcelClass->GetProperties())->Add( pProp );
        FdoDataPropertiesP(pParcelClass->GetIdentityProperties())->Add( pProp );

        pProp = FdoDataPropertyDefinition::Create( L"PIN", L"parcel id" );
        pProp->SetDataType( FdoDataType_String );
        pProp->SetLength(15);
        pProp->SetNullable(false);
        FdoPropertiesP(pParcelClass->GetProperties())->Add( pProp );
        FdoDataPropertiesP(pParcelClass->GetIdentityProperties())->Add( pProp );

        pProp = FdoDataPropertyDefinition::Create( L"Value", L"" );
        pProp->SetDataType( FdoDataType_Decimal );
        pProp->SetNullable(true);
        pProp->SetPrecision( 8 );
        pProp->SetScale( 0 );
        FdoPropertiesP(pParcelClass->GetProperties())->Add( pProp );

        pProp = FdoDataPropertyDefinition::Create( L"AUTOGEN1", L"1st autogenerated property" );
        pProp->SetDataType( FdoDataType_Int64 );
        pProp->SetNullable(true);
        pProp->SetReadOnly(true);
        pProp->SetIsAutoGenerated(true);
        FdoPropertiesP(pParcelClass->GetProperties())->Add( pProp );

        pProp = FdoDataPropertyDefinition::Create( L"AUTOGEN2", L"2nd autogenerated property" );
        pProp->SetDataType( FdoDataType_Int64 );
        pProp->SetNullable(true);
        pProp->SetReadOnly(true);
        pProp->SetIsAutoGenerated(true);
        FdoPropertiesP(pParcelClass->GetProperties())->Add( pProp );

        FdoPtr<FdoObjectPropertyDefinition> pObjProp = FdoObjectPropertyDefinition::Create( L"owner", L"" );
        pObjProp->SetObjectType( FdoObjectType_Value );
        pObjProp->SetClass( pPersonClass );
        FdoPropertiesP(pParcelClass->GetProperties())->Add( pObjProp );

        FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"" );
        pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve);
        FdoPropertiesP(pParcelClass->GetProperties())->Add( pGeomProp );
        pGeomProp->SetHasElevation( true );
        FdoClassesP(pSchema->GetClasses())->Add( pParcelClass );

        FdoPtr<FdoFeatureClass> pClass = FdoFeatureClass::Create( GetNlsObjectName(L"Industrial Parcel"), L"" );
        pClass->SetIsAbstract(false);
        pClass->SetBaseClass( pParcelClass );

        pProp = FdoDataPropertyDefinition::Create( L"# employees", L"" );
        pProp->SetDataType( FdoDataType_Int32 );
        pProp->SetNullable(false);
        FdoPropertiesP(pClass->GetProperties())->Add( pProp );

        FdoClassesP(pSchema->GetClasses())->Add( pClass );

        /* Test feature class with 1 id property not FeatId */

        FdoPtr<FdoFeatureClass> pPlineClass = FdoFeatureClass::Create( L"AcDb3dPolyline", L"City" );
        pParcelClass->SetIsAbstract(false);

        pProp = FdoDataPropertyDefinition::Create( L"Name", L"" );
        pProp->SetDataType( FdoDataType_String );
        pProp->SetLength(50);
        pProp->SetNullable(false);
        FdoPropertiesP(pPlineClass->GetProperties())->Add( pProp );
        FdoDataPropertiesP(pPlineClass->GetIdentityProperties())->Add( pProp );

        pProp = FdoDataPropertyDefinition::Create( L"Population", L"" );
        pProp->SetDataType( FdoDataType_Int32 );
        pProp->SetNullable(false);
        FdoPropertiesP(pPlineClass->GetProperties())->Add( pProp );

        pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"" );
        pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve );
        pGeomProp->SetHasElevation( true );
        FdoPropertiesP(pPlineClass->GetProperties())->Add( pGeomProp );
        pPlineClass->SetGeometryProperty( pGeomProp );

        FdoClassesP(pSchema->GetClasses())->Add( pPlineClass );

        pCmd->SetFeatureSchema( pSchema );
        pCmd->Execute();
    }
}


void UnitTestUtil::CreateCityTable( FdoIConnection* connection )
{
    try
    {
        FdoPtr<FdoISQLCommand> sqlCmd = (FdoISQLCommand *) connection->CreateCommand(FdoCommandType_SQLCommand);

        // This statement is valid only for Oracle (others vary slightly).
        sqlCmd->SetSQLStatement(L"create table CITIES ( CITYID NUMBER(10) primary key, NAME VARCHAR(50), CITY VARCHAR(50) )");

        (void) sqlCmd->ExecuteNonQuery();

        sqlCmd->SetSQLStatement(L"insert into CITIES ( CITYID, NAME, CITY ) values ( 1, 'Marin', 'San Rafael' )");
        (void) sqlCmd->ExecuteNonQuery();
    }
    catch(FdoException *ex)
    {
        // An error probably means that the table already exists.
        ex->Release();
    }
}

void UnitTestUtil::CreateTable1( FdoIConnection* connection )
{
    try
    {
        FdoPtr<FdoISQLCommand> sqlCmd = (FdoISQLCommand *) connection->CreateCommand(FdoCommandType_SQLCommand);

        // This statement is valid only for Oracle (others vary slightly).
        sqlCmd->SetSQLStatement(L"create table TABLE1 ( FEATID1 NUMBER(10) primary key, NAME VARCHAR(50), X NUMBER, Y NUMBER )");

        (void) sqlCmd->ExecuteNonQuery();

        sqlCmd->SetSQLStatement(L"insert into TABLE1 ( FEATID1, NAME, X, Y ) values ( 1, 'MyName', 10, 15 )");
        (void) sqlCmd->ExecuteNonQuery();

        sqlCmd->SetSQLStatement(L"insert into TABLE1 ( FEATID1, NAME, X, Y ) values ( 2, 'His''Name', 20, 25 )");
        (void) sqlCmd->ExecuteNonQuery();
    }
    catch(FdoException *ex)
    {
        // An error probably means that the table already exists.
        ex->Release();
    }
}

void UnitTestUtil::GetRealUserName(char * user_name, unsigned size )
{
#ifdef _WIN32 
	DWORD intsize = size;
    GetUserName(user_name, &intsize);
#else
	struct passwd   *passwd_ptr;
    static int   real_uid;
    real_uid   = getuid();
    passwd_ptr = getpwuid( real_uid );
    strncpy( user_name,  passwd_ptr->pw_name, size );
    user_name[size-1] = 0;
#endif
}

char *UnitTestUtil::w2a(const wchar_t* wString)
{
    static char string[2048];

    FdoCommonThreadMutex mutex;
    mutex.Enter();
    wcstombs(string, wString, sizeof(string));
    mutex.Leave();
    return string;
}

FdoIConnection* UnitTestUtil::GetProviderConnectionObject()
{
    FdoIConnection* ret = NULL;
    try
    {
		if (NULL != ActiveProviderName)
		{
			FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
			ret = static_cast<FdoIConnection*>(manager->CreateConnection (ActiveProviderName));
		}
        else
        {
            printf("ERROR:Unknown provider name!\n");
            throw;
        }
    }
    catch (FdoException* ge)
    {
        TestCommonFail (ge);
    }

    return (ret);
}

wchar_t *UnitTestUtil::GetConnectionString(StringConnTypeRequest pTypeReq, FdoString *suffix, bool bAddExtraneousSpaces)
{
	return UnitTestUtil::InfoUtilConnection->GetConnectionString(pTypeReq, suffix, bAddExtraneousSpaces);
}

void UnitTestUtil::CreateDB(bool addSchema, bool useBaseMapping, FdoString *suffix, int lt_method, bool lt_method_fixed)
{
    FdoStringP service = UnitTestUtil::GetEnviron("service");
    FdoStringP username = UnitTestUtil::GetEnviron("username");
    FdoStringP password = UnitTestUtil::GetEnviron("password");
    FdoStringP datastore = UnitTestUtil::GetEnviron("datastore", suffix);
    FdoStringP schemaType = UnitTestUtil::GetEnviron("schematype");
    FdoStringP description = FdoStringP::Format( L"%hs", UnitTestUtil::GetEnv("description",(char*) NULL) );

     // Long Transaction method defaults to the one passed in.
    // If it is not fixed, it can be overridden by ltmethod from
    // command line.
    const char *lt_string = UnitTestUtil::GetEnv("ltmethod",(char*) NULL);
    int local_lt_method = lt_method;
    if ( lt_string && !lt_method_fixed)
        local_lt_method = atoi(lt_string);

    bool bCreated = false;
    wchar_t *connectString = UnitTestUtil::GetConnectionString(Connection_NoDatastore, suffix);
    FdoPtr<FdoIConnection> connection = GetProviderConnectionObject();
    connection->SetConnectionString ( connectString);

	// Optimization: This has been already checked in GetConnection(). Make sure the caller 
	// checks the existence explictly.
	bool bExists = DatastoreExists(suffix);
	//bool bExists = false;

    if (bExists == false)
    {
        connection->Open ();

		CreateDB( connection, datastore, description, password, (char*)(const char*)schemaType, local_lt_method );
        connection->Close();
        connectString = UnitTestUtil::GetConnectionString(Connection_WithDatastore, suffix);
        connection->SetConnectionString ( connectString);
        connection->Open ();

        if( addSchema )
        {
            try
            {
                UnitTestUtil::CreateAcadSchema( connection, useBaseMapping );
                UnitTestUtil::CreateLandSchema( connection );
            }
            catch( FdoException *e)
            {
                PrintException(e,"schema_error.txt");
                throw e;
            }
            UnitTestUtil::CreateNonUniqueSchema( connection );
        }

    }

    connection->Close ();
}

void UnitTestUtil::CreateDB( FdoIConnection* connection, FdoString *datastore, FdoString *description, FdoString *password, char *schemaType, int local_lt_method )
{
    FdoPtr<FdoICreateDataStore> createCmd = (FdoICreateDataStore*)connection->CreateCommand( FdoCommandType_CreateDataStore );
	
	FdoPtr<FdoIDataStorePropertyDictionary> dictionary = createCmd->GetDataStoreProperties();

	int count;
	FdoString **names = dictionary->GetPropertyNames(count);
	for ( int i = 0; i < count; i++)
	{
		FdoString  *name = names[i];
		
		if ( wcscmp( name, L"DataStore" ) == 0 )
		{
            dictionary->SetProperty( name, datastore );
		} 
		else if ( wcscmp( name, L"Password" ) == 0 )
		{
			dictionary->SetProperty( name, password );
		}
		else if ( wcscmp( name, L"Description" ) == 0 )
		{
			dictionary->SetProperty( name, description );
		}
		else if ( (wcscmp( name, L"LtMode" ) == 0 )|| (wcscmp( name, L"LockMode" ) == 0))
		{
			if ( local_lt_method == 0 )
				dictionary->SetProperty( name, L"NONE" );
			else if ( local_lt_method == 1 || local_lt_method == 2)
                dictionary->SetProperty( name, (local_lt_method == 2) ? L"OWM" : L"FDO" );
		}
		else if ( wcscmp( name, L"TableSpace" ) == 0 )
		{
			FdoStringP value(L""); // TODO
			dictionary->SetProperty( name, value );
		}			
	}

	if( createCmd )
		createCmd->Execute();// datastore, password, service, false, local_lt_method, local_lt_method );

}

void UnitTestUtil::FailOnException( FdoException* e )
{
    char buffer[5000];

    PrintException( e );
    Exception2String( e, buffer );
    e->Release ();
    CPPUNIT_FAIL( buffer );
}


void UnitTestUtil::Exception2String( FdoException* e, char* buffer )
{
    FdoPtr<FdoException> innerE = e;
    // Add ref to prevent smart pointer from destroying exception.
    FDO_SAFE_ADDREF(e);

    while ( innerE->GetCause() )
        innerE = innerE->GetCause();

    if ( innerE == e )
        sprintf( buffer, "%ls", e->GetExceptionMessage() );
    else
        sprintf( buffer, "%ls ... %ls", innerE->GetExceptionMessage(), e->GetExceptionMessage() );
}

void UnitTestUtil::PrintException( FdoException* e, FILE* fp, FdoBoolean stripLineNo )
{
    FdoStringP userDSPref = UnitTestUtil::GetEnviron("datastore", L"");

    FdoPtr<FdoException> currE = e;
    // Add ref to prevent smart pointer from destroying exception.
    FDO_SAFE_ADDREF(e);

    while ( currE ) {
        FdoStringP message = FdoStringP(currE->GetExceptionMessage()).Replace( userDSPref, L"{USER_DS}" ).Replace( userDSPref.Upper(), L"{USER_DS}" );
        FdoString* pMessage = message;
        if ( stripLineNo ) {
#ifdef _WIN32
            // The following removes the function and line number info from error messages.
            FdoString* nextPos = pMessage;
            FdoString* nextPos2;
            FdoString* nextPos3;
            FdoString* nextPos4;
            FdoStringP subStr;
            int state = 0;
            bool done = false;

            while ( !done ) {
                switch (state) {

                case 0: // Initial state
                    // Look for cpp file name enclosed in brackets
                    nextPos = wcschr( pMessage, '(' );
                    nextPos2 = nextPos ? wcsstr( nextPos, L".cpp" ) : NULL;
                    nextPos3 = nextPos ? wcschr( nextPos, ')' ) : NULL;
                    state = 1;
                    break;

                case 1: // process next cpp file in brackets
                    if ( (nextPos == NULL) || (nextPos2 == NULL) || (nextPos3 == NULL) ) { 
                        // no more cpp file in brackets
                        state = 2;
                    }
                    else {
                        if ( nextPos2 < nextPos3 ) {
                            // theres another one.
                            
                            // It might be in nested '()' some move ahead
                            // to innermost '(' before file name.
                            while (true) {
                                nextPos4 = wcschr( nextPos + 1, '(' );
                                if ( nextPos4 && (nextPos4 < nextPos2) )
                                    nextPos = nextPos4;
                                else
                                    break;
                            }
                            state = 3;
                        }
                        else {
                            // next ')' is before '.cpp', skip current '()'
                            state = 4;
                        }
                    }
                    break;

                case 2: // end state
                    // print rest of message
                    fprintf( fp, "%ls\n", pMessage );
                    done = true;
                    break;

                case 3: // found '( ... .cpp ... )'
                    // Get part to the left of '( ... .cpp ... )' and print it
                    subStr = FdoStringP(pMessage).Mid(0, nextPos - pMessage);
                    fprintf( fp, "%ls", (FdoString*) subStr );

                    // Move current ptr to right of '( ... .cpp ... )'
                    pMessage = nextPos3 + 1;
                    state = 0;
                    break;

                case 4: // Move on to next '( ... .cpp ... )'..
                    nextPos = wcschr( nextPos3, '(' );
                    nextPos2 = nextPos ? wcsstr( nextPos, L".cpp" ) : NULL;
                    nextPos3 = nextPos ? wcschr( nextPos, ')' ) : NULL;
                    state = 1;
                    break;
                }
            }
#else
		fprintf( fp, " %ls \n", pMessage );
#endif
        }
        else {
            fprintf( fp, "%ls\n", pMessage );
        }

        currE = currE->GetCause();
    }
}

void UnitTestUtil::PrintException( FdoException* e, const char* fileName, FdoBoolean stripLineNo )
{
    FILE* fp = fopen( fileName, "w" );

    if ( !fp ) {
        char buffer[500];
        sprintf( buffer, "Failed to open file %s", fileName );
        CPPUNIT_FAIL( buffer );
    }

    PrintException( e, fp, stripLineNo );

    fclose( fp );
}

FdoStringP UnitTestUtil::GetEnviron(const char *name, FdoString *suffix)
{
    FdoStringP pRet = UnitTestUtil::InfoUtilConnection->GetEnviron(name, suffix);
    return pRet;
}

const char* UnitTestUtil::GetEnv( const char* pVar, const char* pDefault )
{
    const char* pRet = getenv( pVar );
    if ( !pRet || (strcmp(pRet,"(null)") == 0) )
        pRet = pDefault;

    return(pRet);
}

void  UnitTestUtil::SetProvider( const char *providerName )
{
	UnitTestUtil::InfoUtilConnection->SetProvider(providerName);
}

void UnitTestUtil::CloseConnection( FdoIConnection* connection, bool bDelete, FdoString *suffix )
{
    FdoStringP pDatastore = UnitTestUtil::GetEnviron("datastore", suffix);
    FdoStringP pPassword = UnitTestUtil::GetEnviron("password");
    FdoStringP pService = UnitTestUtil::GetEnviron("service");

    if ( bDelete ) {
         DropDb( connection, pDatastore, pPassword, pService );
    }
	connection->Close();
}

FdoIConnection* UnitTestUtil::CreateConnection(
    bool bPredelete,
    bool bCreate,
    FdoString *suffix,
    const char *schemaType,
    bool* bCreated,
    int   lt_method,
    bool  lt_method_fixed
)
{
    bool bExists = DatastoreExists(suffix);

    if ( bCreated )
        (*bCreated) = false;

    FdoStringP pDatastore = UnitTestUtil::GetEnviron("datastore", suffix);
    FdoStringP pPassword = UnitTestUtil::GetEnviron("password");
    FdoStringP pService = UnitTestUtil::GetEnviron("service");
    FdoStringP pSchemaType = schemaType;

    // Long Transaction method defaults to the one passed in.
    // If it is not fixed, it can be overridden by ltmethod from
    // command line.
    const char *lt_string = UnitTestUtil::GetEnv("ltmethod",(char*) NULL);
    int local_lt_method = lt_method;
    if ( lt_string && !lt_method_fixed)
        local_lt_method = atoi(lt_string);

    FdoStringP strSchemaType = schemaType;
    if ( strSchemaType.GetLength() == 0 )
        pSchemaType = UnitTestUtil::GetEnviron("schematype");

    FdoStringP userConnectString = UnitTestUtil::GetConnectionString(Connection_NoDatastore, suffix);
    FdoStringP userConnectStringMetadata;
    FdoStringP dbConnectString = UnitTestUtil::GetConnectionString(Connection_WithDatastore, suffix);
    FdoStringP dbConnectStringMetadata;

    // Create/open primary connection:
    FdoIConnection* connection = GetProviderConnectionObject();

    connection->SetConnectionString ( userConnectString);
    connection->Open();

    FdoPtr<FdoIConnection> connectionMetadata;
    
	dbConnectStringMetadata = UnitTestUtil::GetConnectionString(Connection_WithDatastore, suffix);
    userConnectStringMetadata = UnitTestUtil::GetConnectionString(Connection_NoDatastore, suffix);
    connectionMetadata = FDO_SAFE_ADDREF(connection);

    if ( bPredelete && bExists ) {
        connectionMetadata->SetConnectionString ( dbConnectStringMetadata);

		DropDb( connectionMetadata, pDatastore, pPassword, pService );

		bExists = false;
    }

    if ( bCreate && !bExists ) {
  
        CreateDB( connectionMetadata, pDatastore, L"", pPassword, (char *)(const char*)pSchemaType, local_lt_method );

        if ( bCreated )
            (*bCreated) = true;
    }

    connection->Close();
    connection->SetConnectionString ( dbConnectString);
    connection->Open();
   // connection->GetDbiConnection()->dbi_set_lt_method(local_lt_method);

    return(connection);
}

void UnitTestUtil::DropDb(FdoString *suffix)
{
    FdoStringP service = UnitTestUtil::GetEnviron("service");
    FdoStringP username = UnitTestUtil::GetEnviron("username");
    FdoStringP password = UnitTestUtil::GetEnviron("password");
    FdoStringP datastore = UnitTestUtil::GetEnviron("datastore", suffix);
    bool bExists = DatastoreExists(suffix);

    if (bExists)
    {
        //FdoRdbmsConnection::DeleteDb(context, datastore, password, service);

        FdoStringP userConnectString = UnitTestUtil::GetConnectionString(Connection_NoDatastore);

        FdoPtr<FdoIConnection> connection = GetProviderConnectionObject();
        connection->SetConnectionString( userConnectString );
        connection->Open();
        FdoPtr<FdoIDestroyDataStore> delCmd = (FdoIDestroyDataStore*)connection->CreateCommand( FdoCommandType_DestroyDataStore );

        if( delCmd )
            DropDb( connection, datastore, password, service );

		connection->Close();
    }

}

void UnitTestUtil::DropDb( FdoIConnection *connection, FdoString* pDatastore, FdoString* pPassword, FdoString* pService )
{
    FdoPtr<FdoIDestroyDataStore> pDelCmd = (FdoIDestroyDataStore*)connection->CreateCommand( FdoCommandType_DestroyDataStore );

	FdoPtr<FdoIDataStorePropertyDictionary> dictionary = pDelCmd->GetDataStoreProperties();
		
	dictionary->SetProperty( L"DataStore", pDatastore );

	// NO provider requires the password 
//	dictionary->SetProperty( L"Password", pPassword );

	// What happens with the service? is it really necessay?
	pDelCmd->Execute();
}

bool UnitTestUtil::DatastoreExists(FdoString *suffix)
{
    StaticConnection* staticConn;

    staticConn = UnitTestUtil::NewStaticConnection();
    staticConn->connect();

    FdoStringP datastore = GetEnviron("datastore", suffix);
    bool found = false;

    {
        FdoSchemaManagerP mgr = staticConn->CreateSchemaManager();
        FdoSmPhMgrP ph = mgr->GetPhysicalSchema();

        if ( FdoSmPhOwnerP(ph->FindOwner(datastore, L"", false)) )
            found = true;
    }

    staticConn->disconnect();
    delete staticConn;

    return(found);
}

FdoIConnection* UnitTestUtil::GetConnection(FdoString *suffix, bool bCreate, bool bRecreateData, StringConnTypeRequest pTypeReq, int lt_method, bool lt_method_fixed)
{
    FdoIConnection* connection = GetProviderConnectionObject();

    try {

        bool bCreated = false;
        if (bCreate)
        {
            bool bExists = UnitTestUtil::DatastoreExists(suffix);
            if (!bExists)
            {
				UnitTestUtil::CreateDB(true, false, suffix, lt_method, lt_method_fixed);
                bCreated = true;
            }
        }
        wchar_t *connectionString = UnitTestUtil::GetConnectionString(Connection_WithDatastore, suffix);

        FdoConnectionState state = connection->GetConnectionState();
      	connection->SetConnectionString(connectionString);

        connection->Open();
        if ( (!bCreated) && bRecreateData ) {
            TestCommonMiscUtil::DeleteObjects( connection, L"Acad", L"AcDb3dPolyline", NULL );
            TestCommonMiscUtil::DeleteObjects( connection, L"L\x00e4nd", L"Parcel", NULL );
            TestCommonMiscUtil::DeleteObjects( connection, L"Acad", L"testClass", NULL );
        }

        if ( bCreated || bRecreateData ) {
            FdoInsertTest *insert = new FdoInsertTest(suffix);

            insert->setUp();
            insert->insert();
            delete insert;
            insert = new FdoInsertTest(suffix);
            insert->setUp();
            insert->insert2();
            delete insert;
        }
    }
    catch ( FdoException* e )
    {
		if( connection )
			connection->Release();
		connection = NULL;
        throw e;
    }
    catch ( ... ) 
    {
		if( connection )
			connection->Release();
        throw;
    }
    return connection;

}

StaticConnection* UnitTestUtil::NewStaticConnection()
{
	return UnitTestUtil::InfoUtilConnection->NewStaticConnection();
}

SchemaOverrideUtilP UnitTestUtil::NewSchemaOverrideUtil()
{
	return UnitTestUtil::InfoUtilConnection->NewSchemaOverrideUtil();
}

bool UnitTestUtil::fuzzyEqual (const double d1, const double d2)
{
    if ((d1==0.0) || (d2==0.0))
        return 1e-5 > fabs(d1 - d2);
    else
        return 1e-5 > fabs(1.0 - (d2 / d1));
}

void UnitTestUtil::CheckOutput( const char* masterFileName, const char* outFileName )
{
    if ( CompareFiles( masterFileName, outFileName ) != 0 ) {
        char buffer[5000];
        sprintf( buffer, "Output file %s differs from expected output file %s", outFileName, masterFileName );
        CPPUNIT_FAIL( buffer );
    }
}

int UnitTestUtil::CompareFiles( const char* file1Name, const char* file2Name )
{
    char buffer[500];
    char buffer1[5000];
    char buffer2[5000];

    int retcode = -1;

    FILE* fp1 = fopen( file1Name, "r" );
    FILE* fp2 = fopen( file2Name, "r" );

    if ( fp1 == NULL ) {
        sprintf( buffer, "UnitTestUtil::CompareFiles: failed to open file %s", file1Name );
        CPPUNIT_FAIL( buffer );
    }

    if ( fp2 == NULL ) {
        sprintf( buffer, "UnitTestUtil::CompareFiles: failed to open file %s", file2Name );
        CPPUNIT_FAIL( buffer );
    }

    while ( fgets( buffer1, 4999, fp1 ) != NULL ) {
        if ( !fgets( buffer2, 4999, fp2  ) )
            // different: file2 has fewer lines.
            goto the_exit;

        if ( strcmp( buffer1, buffer2 ) )
            // different: a line is different
            goto the_exit;
    }

    if ( fgets( buffer2, 4999, fp2 ) )
        // different: file2 has more lines.
        goto the_exit;

    retcode = 0;

the_exit:
    fclose(fp1);
    fclose(fp2);

    return( retcode );
}

void UnitTestUtil::GrantDatastore( FdoIConnection* connection, FdoSchemaManagerP mgr, FdoStringP privileges, FdoStringP grantee )
{
    FdoSmPhMgrP ph = mgr->GetPhysicalSchema();

    FdoSmPhOwnerP owner = ph->GetOwner();
    FdoSmPhRdDbObjectReaderP rdr = owner->CreateDbObjectReader();

    while ( rdr->ReadNext() ) {
        if ( ((rdr->GetType() == FdoSmPhDbObjType_Table) || (rdr->GetType() == FdoSmPhDbObjType_View)) &&
            !rdr->GetString(L"",L"name").Contains(L"$") ) {

            FdoSmPhDbObjectP dbObject = owner->CacheDbObject( rdr );

            UnitTestUtil::Sql2Db( 
                FdoStringP::Format( 
                    L"grant %ls on %ls to %ls",
                    (FdoString*) privileges,
                    (FdoString*) dbObject->GetDbQName(),
                    (FdoString*) grantee
                ),
                connection
            );
        }
    }
}

void UnitTestUtil::WriteSortedLp( const FdoSmLpSchemaCollection* lp, FdoString* fileName, char* styleSheetString )
{
    lp->XMLSerialize( UnitTestUtil::GetOutputFileName( L"lp_temp.xml" ) );

    FdoIoFileStreamP fileStream1 = FdoIoFileStream::Create(UnitTestUtil::GetOutputFileName( L"lp_temp.xml" ),L"rt");
    FdoIoMemoryStreamP outStream = FdoIoMemoryStream::Create();
    UnitTestUtil::SortLpSchemas( fileStream1, outStream, styleSheetString );
    outStream->Reset();
    fileStream1 = NULL;

    FdoIoFileStreamP fileStream2 = FdoIoFileStream::Create(fileName, L"wt");
    XmlFormatter formatter( 
        outStream, 
        fileStream2
    );
    formatter.Format();
    fileStream2 = NULL;
}

// Exports spatial contexts and feature schemas from datastore to XML.
void UnitTestUtil::ExportDb( 
    FdoIConnection* connection, 
    FdoIoStream* stream, 
    FdoXmlSpatialContextFlags* flags, 
    bool includeDefaultMappings, 
    FdoString* oldDsSchemaName,
    FdoString* newDsSchemaName
)
{
    FdoInt32 i;

    stream->Reset();
    FdoXmlWriterP writer = FdoXmlWriter::Create(stream);

    // Serialize the spatial contexts
    FdoXmlSpatialContextSerializer::XmlSerialize( 
        connection,
        FdoXmlSpatialContextWriterP(
            FdoXmlSpatialContextWriter::Create(writer,flags)
        ),
        flags
    );

    // Get the feature schemas from the Datastore
    FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
    FdoFeatureSchemasP schemas = pDescCmd->Execute();

    if ( oldDsSchemaName ) {
        for ( i = 0; i < schemas->GetCount(); i++ ) {
            FdoFeatureSchemaP schema = schemas->GetItem( i );
            if ( FdoStringP(schema->GetName()).ICompare(oldDsSchemaName) == 0 ) 
                schema->SetName( newDsSchemaName );
        }
    }

    // Serialize the feature schemas.
    schemas->WriteXml( writer );

    // Get the feature schema overrides from the Datastore
    FdoPtr<FdoIDescribeSchemaMapping>  pDescMappingsCmd = (FdoIDescribeSchemaMapping*) connection->CreateCommand(FdoCommandType_DescribeSchemaMapping);
    pDescMappingsCmd->SetIncludeDefaults( includeDefaultMappings );
    FdoSchemaMappingsP mappings = pDescMappingsCmd->Execute();

    if ( oldDsSchemaName ) {
        for ( i = 0; i < mappings->GetCount(); i++ ) {
            FdoPhysicalSchemaMappingP mapping = mappings->GetItem(i);
            if ( FdoStringP(mapping->GetName()).ICompare(oldDsSchemaName) == 0 ) 
                mapping->SetName( newDsSchemaName );
        }
    }

    // Serialize the feature schemas.
    mappings->WriteXml( writer );
}

// The following stylesheet sorts the output XML files so that the Spatial 
// Contexts, Schemas and Schema Mappings are always in the same order. This
// allows the XML files to be compared against corresponding masters, without
// element ordering differences triggering false test failures. 

char* UnitTestUtil::pSortConfigSheet = 
"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\
<stylesheet version=\"1.0\" \
xmlns=\"http://www.w3.org/1999/XSL/Transform\" \
xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" \
xmlns:gml=\"http://www.opengis.net/gml\" \
xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" \
xmlns:fdo=\"http://fdo.osgeo.org/schemas\" \
xmlns:ora=\"http://www.autodesk.com/isd/fdo/OracleProvider\" \
xmlns:mql=\"http://fdomysql.osgeo.org/schemas\" \
xmlns:sqs=\"http://www.autodesk.com/isd/fdo/SQLServerProvider\">\
<xsl:template match=\"fdo:DataStore\">\
    <xsl:copy>\
        <xsl:apply-templates select=\"@*\"/>\
        <xsl:apply-templates select=\"gml:DerivedCRS\">\
            <xsl:sort select=\"@gml:id\" />\
        </xsl:apply-templates>\
        <xsl:apply-templates select=\"xs:schema\">\
            <xsl:sort select=\"@targetNamespace\" />\
        </xsl:apply-templates>\
        <xsl:apply-templates select=\"node()[local-name()='SchemaMapping']\">\
            <xsl:sort select=\"@name\" />\
        </xsl:apply-templates>\
        <xsl:apply-templates select=\"node()[local-name()='FeatureCollection']\"/>\
    </xsl:copy>\
</xsl:template>\
<xsl:template match=\"xs:schema\">\
    <xsl:copy>\
        <xsl:apply-templates select=\"@*\"/>\
        <xsl:apply-templates select=\"xs:annotation\"/>\
        <xsl:apply-templates select=\"xs:element\">\
            <xsl:sort select=\"@name\" />\
        </xsl:apply-templates>\
        <xsl:apply-templates select=\"xs:complexType\">\
            <xsl:sort select=\"@name\" />\
        </xsl:apply-templates>\
    </xsl:copy>\
</xsl:template>\
<xsl:template match=\"xs:sequence\">\
    <xsl:copy>\
        <xsl:apply-templates select=\"@*\"/>\
        <xsl:apply-templates select=\"xs:element\">\
            <xsl:sort select=\"@name\" />\
        </xsl:apply-templates>\
    </xsl:copy>\
</xsl:template>\
<xsl:template match=\"node()[local-name() = 'SchemaMapping']\">\
    <xsl:element name=\"SchemaMapping\" namespace=\"{namespace::node()[name()='']}\">\
		<xsl:for-each select=\"namespace::node()[not(name()='')]\">\
			<xsl:copy/>\
		</xsl:for-each>\
        <xsl:apply-templates select=\"@*|node()\"/>\
    </xsl:element>\
</xsl:template>\
<xsl:template match=\"gml:FeatureColection\">\
    <xsl:copy>\
        <xsl:apply-templates select=\"gml:featureMember\">\
            <xsl:sort select=\"ClassB1/Prop1\" />\
        </xsl:apply-templates>\
    </xsl:copy>\
</xsl:template>\
<xsl:template match=\"gml:coordinates\">\
    <xsl:element name=\"gml:coordinates\" >\
        <xsl:variable name=\"coords\" select=\"text()\"/>\
        <xsl:value-of select=\"$coords\"/>\
        <xsl:if test=\"not(contains(substring-after($coords,','),','))\" >,0.000000</xsl:if>\
    </xsl:element>\
</xsl:template>\
<xsl:template match=\"@*|node()\">\
  <xsl:copy>\
    <xsl:apply-templates select=\"@*|node()\"/>\
  </xsl:copy>\
  </xsl:template>\
</stylesheet>";

// Write a config document to a file. Applies an XSL transformation to sort the 
// elements in the file.
void UnitTestUtil::Config2SortedFile( FdoIoStream* stream, FdoString* fileName, char* styleSheetString )
{
    FdoIoMemoryStreamP sortedStream = FdoIoMemoryStream::Create();

    SortXml( stream, styleSheetString ? styleSheetString : pSortConfigSheet, sortedStream );
    
    sortedStream->Reset();

    UnitTestUtil::Stream2File( sortedStream, fileName );
}


// Write a stream out to a file.
void UnitTestUtil::Stream2File( FdoIoStream* stream, FdoString* fileName )
{
    stream->Reset();

    FdoIoFileStreamP fileStream = FdoIoFileStream::Create( fileName, L"w+t" );
    XmlFormatter formatter(stream, fileStream);
    formatter.Format();
}

void UnitTestUtil::LogicalPhysicalFormat(FdoIoStream* stream1, FdoIoStream* stream2, FdoStringP providerName)
{
    FdoIoMemoryStreamP tempStream = FdoIoMemoryStream::Create();

    stream1->Reset();
    FdoXmlReaderP stylesheet = FdoXmlReader::Create( L"LogicalPhysicalFormatter.xslt" );

    FdoXslTransformerP transformer = FdoXslTransformer::Create( 
        FdoXmlReaderP(FdoXmlReader::Create(stream1)), 
        stylesheet, 
        FdoXmlWriterP(FdoXmlWriter::Create(tempStream,false))
    );

    FdoDictionaryP params = transformer->GetParameters();
    params->Add( 
        FdoDictionaryElementP( 
            FdoDictionaryElement::Create( 
                L"providerName", 
                FdoStringP(L"'") + providerName + L"'"
            ) 
        ) 
    );

    transformer->Transform();
    transformer = NULL;

    LogicalPhysicalSort( tempStream, stream2 );
}

void UnitTestUtil::LogicalPhysicalBend( FdoIoStream* stream1, FdoIoStream* stream2, FdoStringP providerName )
{
    FdoIoMemoryStreamP tempStream = FdoIoMemoryStream::Create();

    stream1->Reset();
    FdoXmlReaderP stylesheet = FdoXmlReader::Create( L"LogicalPhysicalBender.xslt" );

    FdoXslTransformerP transformer = FdoXslTransformer::Create( 
        FdoXmlReaderP(FdoXmlReader::Create(stream1)), 
        stylesheet, 
        FdoXmlWriterP(FdoXmlWriter::Create(tempStream,false))
    );

    FdoDictionaryP params = transformer->GetParameters();
    params->Add( 
        FdoDictionaryElementP( 
            FdoDictionaryElement::Create( 
                L"providerName", 
                FdoStringP(L"'") + providerName + L"'"
            ) 
        ) 
    );

    transformer->Transform();
    transformer = NULL;

    LogicalPhysicalSort( tempStream, stream2 );
}

void UnitTestUtil::LogicalPhysicalSort( FdoIoStream* stream1, FdoIoStream* stream2 )
{

    stream1->Reset();
    FdoXmlReaderP stylesheet = FdoXmlReader::Create( L"LogicalPhysicalSorter.xslt" );

    FdoXslTransformerP transformer = FdoXslTransformer::Create( 
        FdoXmlReaderP(FdoXmlReader::Create(stream1)), 
        stylesheet, 
        FdoXmlWriterP(FdoXmlWriter::Create(stream2,false))
    );

    transformer->Transform();
    transformer = NULL;

    stream2->Reset();
}

bool UnitTestUtil::ContainsRdOnlyProp( FdoReadOnlyPropertyDefinitionCollection* props, FdoString* propName )
{
    FdoPropertyP prop = UnitTestUtil::FindRdOnlyProp( props, propName );

    return (prop != NULL);
}

FdoPropertyP UnitTestUtil::FindRdOnlyProp( FdoReadOnlyPropertyDefinitionCollection* props, FdoString* propName )
{
    FdoInt32 idx;
    FdoPropertyP prop;

    for ( idx = 0; idx < props->GetCount(); idx++ ) {
        prop = props->GetItem(idx);

        if ( wcscmp(prop->GetName(), propName) == 0 ) 
            return prop;
    }

    return (FdoPropertyDefinition*) NULL;
}

void UnitTestUtil::SortLpSchemas( FdoIoStream* inStream, FdoIoStream* outStream, char* styleSheetString )
{
    UnitTestUtil::SortXml( inStream, styleSheetString ? styleSheetString : pSortLpSheet, outStream );
}

void UnitTestUtil::SortXml( FdoIoStream* inStream, char* styleSheetString, FdoIoStream* outStream )
{
    FdoIoMemoryStreamP stylesheetStream = FdoIoMemoryStream::Create();

    stylesheetStream->Write( (FdoByte*) styleSheetString, strlen(styleSheetString) );
    stylesheetStream->Reset();

    inStream->Reset();

    FdoXslTransformerP tfmr = FdoXslTransformer::Create(
        FdoXmlReaderP( FdoXmlReader::Create(inStream) ),
        FdoXmlReaderP( FdoXmlReader::Create(stylesheetStream) ),
        FdoXmlWriterP( FdoXmlWriter::Create(outStream, false) )
    );

    tfmr->Transform();

}

void UnitTestUtil::OverrideBend( FdoIoStream* stream1, FdoIoStream* stream2, FdoStringP providerName, 
								FdoStringP providerUri, FdoStringP oldOwnerPrefix, FdoStringP newOwnerPrefix,
                                FdoStringP tablespace)
{
    stream1->Reset();
    FdoXmlReaderP stylesheet = FdoXmlReader::Create( L"OverrideBender.xslt" );

    FdoXslTransformerP transformer = FdoXslTransformer::Create( 
        FdoXmlReaderP(FdoXmlReader::Create(stream1)), 
        stylesheet, 
        FdoXmlWriterP(FdoXmlWriter::Create(stream2,false))
    );

    FdoDictionaryP params = transformer->GetParameters();
    params->Add( 
        FdoDictionaryElementP( 
            FdoDictionaryElement::Create( 
                L"providerName", 
                FdoStringP(L"'") + providerName + L"'"
            ) 
        ) 
    );
    params->Add( 
        FdoDictionaryElementP( 
            FdoDictionaryElement::Create( 
                L"providerUri", 
                FdoStringP(L"'") + providerUri + L"'"
            ) 
        ) 
    );
    params->Add( 
        FdoDictionaryElementP( 
            FdoDictionaryElement::Create( 
                L"oldOwnerPrefix", 
                FdoStringP(L"'") + oldOwnerPrefix + L"'"
            ) 
        ) 
    );
    params->Add( 
        FdoDictionaryElementP( 
            FdoDictionaryElement::Create( 
                L"newOwnerPrefix", 
                FdoStringP(L"'") + newOwnerPrefix + L"'"
            ) 
        ) 
    );

    if ( tablespace != L"" ) {
        params->Add( 
            FdoDictionaryElementP( 
                FdoDictionaryElement::Create( 
                    L"tablespace", 
                    FdoStringP(L"'") + tablespace + L"'"
                ) 
            ) 
        );
    }

    transformer->Transform();
    transformer = NULL;

    stream2->Reset();
}

bool UnitTestUtil::DoThisTest( char *testGroupName )
{
    const char *testGroups = UnitTestUtil::GetEnv("testgroups", "");

    FdoStringsP parsedGroups = FdoStringCollection::Create( testGroups, L"," );
    FdoStringP pGroupName(testGroupName);

    int i;

    for ( i = 0; i < parsedGroups->GetCount(); i++ ) {
        if ( FdoStringP(L"all") == parsedGroups->GetString(i) ||
             pGroupName == parsedGroups->GetString(i) )
            return true;
    }

    printf( "Test is in group '%s', skipping test\n", testGroupName );
    return false;
}

// Accesses the data on the current feature.
void UnitTestUtil::ProcessFeature(FdoIFeatureReader* featureReader)
{
    FdoPtr<FdoClassDefinition> classDef = featureReader->GetClassDefinition();
    FdoPtr<FdoPropertyDefinitionCollection> propColl = classDef->GetProperties();

    bool        boolValue   = false;
    FdoByte     byteValue   = 13;
    FdoDateTime dateTimeValue;
    double      doubleValue = 13.0;
    FdoInt16    int16Value  = 13;
    FdoInt32    int32Value  = 13;
    FdoInt64    int64Value  = 13;
    float       singleValue = 13.0;
    FdoStringP  stringValue = L"Unset";

    // access all the data properties
    for (int i=0; i<propColl->GetCount(); i++)
    {
        FdoPtr<FdoPropertyDefinition> prop = propColl->GetItem(i);
        FdoPropertyType propertyType = prop->GetPropertyType();
        switch (propertyType)
        {
        case FdoPropertyType_DataProperty:
            {
                FdoDataPropertyDefinition* dataProp = static_cast<FdoDataPropertyDefinition*>(prop.p);
                FdoString* propName = dataProp->GetName();
                FdoDataType propDataType = dataProp->GetDataType();
                if (!featureReader->IsNull(propName))
                {
                    switch (propDataType)
                    {
                    case FdoDataType_Boolean:
                        boolValue = featureReader->GetBoolean(propName);
                        break;
                    case FdoDataType_Byte:
                        byteValue = featureReader->GetByte(propName);
                        break;
                    case FdoDataType_DateTime:
                        dateTimeValue = featureReader->GetDateTime(propName);
                        break;
                    case FdoDataType_Decimal:
                        doubleValue = featureReader->GetDouble(propName);     // handle as double or single
                        break;
                    case FdoDataType_Double:
                        doubleValue = featureReader->GetDouble(propName);
                        break;
                    case FdoDataType_Int16:
                        int16Value = featureReader->GetInt16(propName);
                        break;
                    case FdoDataType_Int32:
                        int32Value = featureReader->GetInt32(propName);
                        break;
                    case FdoDataType_Int64:
                        int64Value = featureReader->GetInt64(propName);
                        break;
                    case FdoDataType_Single:
                        singleValue = featureReader->GetSingle(propName);
                        break;
                    case FdoDataType_String:
                        stringValue = featureReader->GetString(propName);
                        break;
                    case FdoDataType_BLOB:
                    case FdoDataType_CLOB:
                        {
                            FdoLOBValue* value = featureReader->GetLOB(propName);
                            FDO_SAFE_RELEASE(value);
                        }
                        break;
                    }
                }
            } // end case FdoPropertyType_DataProperty
            break;

        case FdoPropertyType_GeometricProperty:
            {
                FdoGeometricPropertyDefinition* geomProp = static_cast<FdoGeometricPropertyDefinition*>(prop.p);
                FdoString* propName = geomProp->GetName();
                FdoInt32 geometryTypes = geomProp->GetGeometryTypes();
                bool hasElevation = geomProp->GetHasElevation();
                bool hasMeasure = geomProp->GetHasMeasure();
                FdoString * scName = geomProp->GetSpatialContextAssociation();
                FdoString * className = classDef->GetName();
                bool scNameOk =
                    ( NULL != scName &&
                      (   wcscmp(scName, L"Default") == 0 ||
                        ( wcscmp(scName, L"survey1") == 0 && wcscmp(className, L"Sounding") == 0 ) ||
                        ( wcscmp(scName, L"AcdbSc") == 0 && wcscmp(className, L"Table1") == 0 ) ||
                        ( wcscmp(scName, L"AcdbSc") == 0 && wcscmp(className, L"Polyline") == 0 ) ) );
                CPPUNIT_ASSERT_MESSAGE("Wrong SpatialContextAssociation name", scNameOk);
                if (!featureReader->IsNull(propName))
                {
                    FdoPtr<FdoByteArray> fgf = featureReader->GetGeometry(propName);
                    FdoInt32 numBytes = fgf->GetCount();
                    CPPUNIT_ASSERT_MESSAGE("Got 0-byte FGF geometry from feature reader", numBytes > 0);

                    // make a geometry
                    FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance();
                    FdoPtr<FdoIGeometry> geom = factory->CreateGeometryFromFgf(fgf);
                    FdoPtr<FdoIEnvelope> bbox = geom->GetEnvelope();
                }
            }
            break;
        }
    }
}

FdoStringP UnitTestUtil::GetOutputFileName (FdoString* pBaseFileName)
{
	return InfoUtilConnection->GetOutputFileName( pBaseFileName );
}

// Combines the input strings into one string.  The caller must call
// "delete" on the returned string when done.
wchar_t* UnitTestUtil::MakeString(FdoString* str1,
                                    FdoString* str2,
                                    FdoString* str3,
                                    FdoString* str4,
                                    FdoString* str5,
                                    FdoString* str6,
                                    FdoString* str7,
                                    FdoString* str8,
                                    FdoString* str9,
                                    FdoString* str10)
{
    // return NULL if all strings are NULL
    if (str1 == NULL && str2 == NULL && str3 == NULL && str4 == NULL && str5 == NULL && str6 == NULL && str7 == NULL && str8 == NULL && str9 == NULL && str10 == NULL)
        return NULL;

    size_t len = 1;     // 1 for EOS
    if (str1 != NULL)
        len += wcslen(str1);
    if (str2 != NULL)
        len += wcslen(str2);
    if (str3 != NULL)
        len += wcslen(str3);
    if (str4 != NULL)
        len += wcslen(str4);
    if (str5 != NULL)
        len += wcslen(str5);
    if (str6 != NULL)
        len += wcslen(str6);
    if (str7 != NULL)
        len += wcslen(str7);
    if (str8 != NULL)
        len += wcslen(str8);
    if (str9 != NULL)
        len += wcslen(str9);
    if (str10 != NULL)
        len += wcslen(str10);

    wchar_t* strRet = new wchar_t[len];
    strRet[0] = L'\0';

    if (str1 != NULL)
        wcscat(strRet, str1);
    if (str2 != NULL)
        wcscat(strRet, str2);
    if (str3 != NULL)
        wcscat(strRet, str3);
    if (str4 != NULL)
        wcscat(strRet, str4);
    if (str5 != NULL)
        wcscat(strRet, str5);
    if (str6 != NULL)
        wcscat(strRet, str6);
    if (str7 != NULL)
        wcscat(strRet, str7);
    if (str8 != NULL)
        wcscat(strRet, str8);
    if (str9 != NULL)
        wcscat(strRet, str9);
    if (str10 != NULL)
        wcscat(strRet, str10);

    return strRet;
}

// Accesses the data on the current row of data.
void UnitTestUtil::ProcessData(FdoIDataReader* reader)
{
    bool        boolValue   = false;
    FdoByte     byteValue   = 13;
    FdoDateTime dateTimeValue;
    double      doubleValue = 13.0;
    FdoInt16    int16Value  = 13;
    FdoInt32    int32Value  = 13;
    FdoInt64    int64Value  = 13;
    float       singleValue = 13.0;
    FdoStringP  stringValue = L"Unset";

    // access all the data properties
    FdoInt32 propertyCount = reader->GetPropertyCount();
    for (int i=0; i<propertyCount; i++)
    {
        FdoString * propName = reader->GetPropertyName(i);
        FdoPropertyType propertyType = reader->GetPropertyType(propName);
        CPPUNIT_ASSERT_MESSAGE("Wrong property type (must always be Data or Geometry).",
            propertyType==FdoPropertyType_DataProperty ||
            propertyType==FdoPropertyType_GeometricProperty);
 
        switch (propertyType)
        {
        case FdoPropertyType_DataProperty:
            {
                FdoDataType propDataType = reader->GetDataType(propName);
                if (!reader->IsNull(propName))
                {
                    switch (propDataType)
                    {
                    case FdoDataType_Boolean:
                        boolValue = reader->GetBoolean(propName);
                        break;
                    case FdoDataType_Byte:
                        byteValue = reader->GetByte(propName);
                        break;
                    case FdoDataType_DateTime:
                        dateTimeValue = reader->GetDateTime(propName);
                        break;
                    case FdoDataType_Decimal:
                        doubleValue = reader->GetDouble(propName);     // handle as double or single
                        break;
                    case FdoDataType_Double:
                        doubleValue = reader->GetDouble(propName);
                        break;
                    case FdoDataType_Int16:
                        int16Value = reader->GetInt16(propName);
                        break;
                    case FdoDataType_Int32:
                        int32Value = reader->GetInt32(propName);
                        break;
                    case FdoDataType_Int64:
                        int64Value = reader->GetInt64(propName);
                        break;
                    case FdoDataType_Single:
                        singleValue = reader->GetSingle(propName);
                        break;
                    case FdoDataType_String:
                        stringValue = reader->GetString(propName);
                        break;
                    case FdoDataType_BLOB:
                    case FdoDataType_CLOB:
                        {
                            FdoLOBValue* value = reader->GetLOB(propName);
                            FDO_SAFE_RELEASE(value);
                        }
                        break;
                    }
                }
            } // end case FdoPropertyType_DataProperty
            break;

        case FdoPropertyType_GeometricProperty:
            {
                if (!reader->IsNull(propName))
                {
                    FdoPtr<FdoByteArray> awkb = reader->GetGeometry(propName);
                    FdoInt32 numBytes = awkb->GetCount();
                    CPPUNIT_ASSERT_MESSAGE("Got 0-byte AGF geometry from feature reader", numBytes > 0);

                    // make a geometry
                    FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance();
                    FdoPtr<FdoIGeometry> geom = factory->CreateGeometryFromFgf(awkb);
                    FdoPtr<FdoIEnvelope> bbox = geom->GetEnvelope();
                }
            }
            break;
        }
    }
}

wchar_t UnitTestUtil::GetNlsChar( int index )
{
	return UnitTestUtil::InfoUtilConnection->GetNlsChar( index );
}

FdoStringP UnitTestUtil::GetNlsValue( int index )
{
    FdoStringP val = FdoStringP::Format( 
        L"Val%lc%lc%lc",
        GetNlsChar(1),
        GetNlsChar(2),
        GetNlsChar(3)
    );

    switch ( index ) {
        case 1:
            val = FdoStringP::Format( 
                L"%lc",
                GetNlsChar(1)
            );
            break;
        case 2:
            val = FdoStringP::Format( 
                L"%lc%lc",
                GetNlsChar(1),
                GetNlsChar(3)
            );
            break;
        case 3:
            val = FdoStringP::Format( 
                L"%lc%lc%lc",
                GetNlsChar(1),
                GetNlsChar(2),
                GetNlsChar(3)
            );
            break;
        case 4:
            val = FdoStringP::Format( 
                L"%lc%lc%lc%lc",
                GetNlsChar(4),
                GetNlsChar(3),
                GetNlsChar(2),
                GetNlsChar(1)
            );
            break;
    }

    return val;
}

FdoStringP UnitTestUtil::GetNlsObjectName( FdoStringP inName )
{
    if ( inName == L"Industrial Parcel" ) 
        return FdoStringP::Format(
            L"Ind%lcstri%lc P%lcrcel",
            GetNlsChar(1),
            GetNlsChar(2),
            GetNlsChar(3)
        );

    if ( inName == L"Geometry" ) 
        return FdoStringP::Format(
            L"G%lc%lcm%lctry",
            GetNlsChar(1),
            GetNlsChar(2),
            GetNlsChar(3)
        );

    return inName;
}


