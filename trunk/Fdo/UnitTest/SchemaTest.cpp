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
#include "SchemaTest.h"
#include "UnitTestUtil.h"
#include "XmlFormatter.h"
#ifdef _DEBUG
//#define DEBUG_DETAIL  1
#endif

#ifndef FALSE
#define FALSE   1
#endif

CPPUNIT_TEST_SUITE_REGISTRATION (SchemaTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (SchemaTest, "SchemaTest");

SchemaTest::SchemaTest(void)
{
}

SchemaTest::~SchemaTest(void)
{
}

void SchemaTest::setUp()
{
    // nothing to do at setup time... yet
}

void SchemaTest::testSchemaElement(FdoSchemaElement* pElement, FdoString* pszName, FdoString* pszDesc)
{
    // check constructor created things OK
    FDO_CPPUNIT_ASSERT(wcscmp(pElement->GetName(), pszName) == 0);
    FDO_CPPUNIT_ASSERT(wcscmp(pElement->GetDescription(), pszDesc) == 0);

    // check setting to null
    pElement->SetName(NULL);
    pElement->SetDescription(NULL);
    FDO_CPPUNIT_ASSERT(pElement->GetName() == NULL);
    FDO_CPPUNIT_ASSERT(pElement->GetDescription() == NULL);

    // check setters and getters
    pElement->SetName(pszName);
    pElement->SetDescription(pszDesc);
    FDO_CPPUNIT_ASSERT(wcscmp(pElement->GetName(), pszName) == 0);
    FDO_CPPUNIT_ASSERT(wcscmp(pElement->GetDescription(), pszDesc) == 0);

    ////////////////////////////////////////////////////////////
    // check Schema Attribute Dictionary
    //
    FdoSchemaAttributeDictionary*   pDict = pElement->GetAttributes();
    FDO_CPPUNIT_ASSERT(pDict->GetCount() == 0);

    pDict->Add(L"Test", L"test");
    FDO_CPPUNIT_ASSERT(pDict->GetCount() == 1);
    FDO_CPPUNIT_ASSERT(wcscmp(pDict->GetAttributeValue(L"Test"), L"test") == 0);

    try
    {
        pDict->Add(L"Test", L"this");
        FDO_CPPUNIT_ASSERT(false);         // should have failed and throw exception
    }
    catch (FdoException* exception)
    {
        exception->Release();
    }

    pDict->Add(L"test", L"this");     // case sensitive, so should succeed
    FDO_CPPUNIT_ASSERT(pDict->GetCount() == 2);

    try
    {
        pDict->SetAttributeValue(L"NoMatch", L"other");
        FDO_CPPUNIT_ASSERT(false);         // should have failed and throw exception
    }
    catch (FdoException* exception)
    {
        exception->Release();
    }

    pDict->SetAttributeValue(L"Test", L"me");
    pDict->SetAttributeValue(L"Test", L"other");
    FDO_CPPUNIT_ASSERT(pDict->GetCount() == 2);
    FDO_CPPUNIT_ASSERT(wcscmp(pDict->GetAttributeValue(L"Test"), L"other") == 0);

    pDict->Add(L"3", NULL);
    pDict->Add(L"4", L"");
    FDO_CPPUNIT_ASSERT(pDict->GetAttributeValue(L"3") == NULL);

    // NULL value and NULL string aren't the same.
    FDO_CPPUNIT_ASSERT(pDict->GetAttributeValue(L"4") != NULL);
    FDO_CPPUNIT_ASSERT(wcscmp(pDict->GetAttributeValue(L"4"), L"") == 0);
    pDict->SetAttributeValue(L"4", NULL);
    FDO_CPPUNIT_ASSERT(pDict->GetAttributeValue(L"4") == NULL);

    pDict->Add(L"5", L"5lab");
    pDict->Add(L"6", L"b6ab");
    pDict->Add(L"7", L"bl7b");
    pDict->Add(L"8", L"bla8");
    pDict->Add(L"9", L"9lab");
    pDict->Add(L"10", L"b10ab");
    FDO_CPPUNIT_ASSERT(pDict->GetCount() == 10);

    FDO_CPPUNIT_ASSERT(pDict->GetCount() == 10);
    pDict->Remove(L"4");
    FDO_CPPUNIT_ASSERT(pDict->GetCount() == 9);

    try
    {
        pDict->Remove(L"4");
        FDO_CPPUNIT_ASSERT(false);         // should have failed and throw exception
    }
    catch (FdoException* exception)
    {
        exception->Release();
    }

    try
    {
        pDict->Remove(L"b10ab");
        FDO_CPPUNIT_ASSERT(false);         // should have failed and throw exception
    }
    catch (FdoException* exception)
    {
        exception->Release();
    }

    FdoInt32                 cLength = 0;
    FdoString**     aNames = pDict->GetAttributeNames(cLength);
    FDO_CPPUNIT_ASSERT(cLength == 9);
    FDO_CPPUNIT_ASSERT(wcscmp(aNames[0], L"Test") == 0);
    FDO_CPPUNIT_ASSERT(wcscmp(aNames[1], L"test") == 0);
    FDO_CPPUNIT_ASSERT(wcscmp(aNames[2], L"3") == 0);
    FDO_CPPUNIT_ASSERT(wcscmp(aNames[3], L"5") == 0);
    FDO_CPPUNIT_ASSERT(wcscmp(aNames[4], L"6") == 0);
    FDO_CPPUNIT_ASSERT(wcscmp(aNames[5], L"7") == 0);
    FDO_CPPUNIT_ASSERT(wcscmp(aNames[6], L"8") == 0);
    FDO_CPPUNIT_ASSERT(wcscmp(aNames[7], L"9") == 0);
    FDO_CPPUNIT_ASSERT(wcscmp(aNames[8], L"10") == 0);

    pDict->Release();

    ////////////////////////////////////////////////////////////
    // TODO: Test Set/GetParent() and GetFeatureSchema()
}

void SchemaTest::testClassDefinition(FdoClassDefinition* pClassDef, FdoString* pszName, FdoString* pszDesc)
{
    testSchemaElement(pClassDef, pszName, pszDesc);

    pClassDef->SetIsAbstract(true);
    FDO_CPPUNIT_ASSERT(pClassDef->GetIsAbstract() == true);

    pClassDef->SetIsAbstract(false);
    FDO_CPPUNIT_ASSERT(pClassDef->GetIsAbstract() == false);

    // TODO: test GetProperties(), GetBaseProperties() and GetIdentityProperites()
}

/*TODO:R2 or later
void SchemaTest::testCoverageFeatureClass(FdoCoverageFeatureClass* pCoverageFeature, FdoString* pszName, FdoString* pszDesc)
{
    static FdoString*   pszCoordSys = L"LL84";

    testClassDefinition(pCoverageFeature, pszName, pszDesc);

    FDO_CPPUNIT_ASSERT(pCoverageFeature->GetCoordinateSystem() == NULL);

    // TODO: Should SetCoordinateSystem be private??
//    pCoverageFeature->SetCoordinateSystem(pszCoordSys);
//    FDO_CPPUNIT_ASSERT(wcscmp(pCoverageFeature->GetCoordinateSystem(), pszCoordSys) == 0);
}

void SchemaTest::testSegmentFeatureClass(FdoSegmentFeatureClass* pSegFeature, FdoString* pszName, FdoString* pszDesc)
{
    testClassDefinition(pSegFeature, pszName, pszDesc);

    // TODO: test GetMeasureUnit() - How is it set??
    // TODO: test GetFeatureClasses()
}
*/
void SchemaTest::testPropertyValue()
{
    // create some data for the test
    static FdoString*  pszIdent = L"Prop.Value.1";
    static FdoString*  pszDVal = L"DataValue1";

    FdoDataValue*   pDV = FdoStringValue::Create(pszDVal);
    FdoIdentifier*  pIdent = FdoIdentifier::Create(pszIdent);

    FdoPropertyValue*   pPropVal = FdoPropertyValue::Create(pIdent, pDV);

    // check constructor created things OK
    FdoIdentifier*  pIdentTest = pPropVal->GetName();
    FDO_CPPUNIT_ASSERT(pIdentTest == pIdent);
    pIdentTest->Release();

    FdoValueExpression* pVETest = pPropVal->GetValue();
    FDO_CPPUNIT_ASSERT(pVETest == pDV);
    pVETest->Release();

    // test getting and setting nulls
    pPropVal->SetName((FdoIdentifier*)NULL);
    pPropVal->SetValue((FdoDataValue*)NULL);
    FDO_CPPUNIT_ASSERT(pPropVal->GetName() == NULL);
    FDO_CPPUNIT_ASSERT(pPropVal->GetValue() == NULL);

    // test setters and getters
    pPropVal->SetName(pszIdent);
    wchar_t pszDValExpression[100];
    swprintf(pszDValExpression, 100, L"'%ls'", pszDVal);
    pPropVal->SetValue(pszDValExpression);
    pIdentTest = pPropVal->GetName();
    FDO_CPPUNIT_ASSERT(wcscmp(pIdentTest->GetText(), pszIdent) == 0);
    pIdentTest->Release();
    FdoStringValue*   pDVTest = dynamic_cast<FdoStringValue*>(pPropVal->GetValue());
    FDO_CPPUNIT_ASSERT(wcscmp(pDVTest->GetString(), pszDVal) == 0);
    pDVTest->Release();

    pDV->Release();
    pIdent->Release();
    pPropVal->Release();
}


void SchemaTest::testMultiSchema()
{
    FdoPtr<FdoFeatureSchemaCollection>pSchemas = FdoFeatureSchemaCollection::Create(NULL);

    FdoPtr<FdoFeatureSchema>pFSchema1 = FdoFeatureSchema::Create(L"SchemaOne", L"Schema One Desc");
    FdoPtr<FdoClassCollection>pClasses1 = pFSchema1->GetClasses();
    pSchemas->Add( pFSchema1 );
    
    FdoPtr<FdoFeatureSchema>pFSchema2 = FdoFeatureSchema::Create(L"SchemaTwo", L"Schema two Desc");
    FdoPtr<FdoClassCollection>pClasses2 = pFSchema2->GetClasses();
    pSchemas->Add( pFSchema2 );
    
    FdoPtr<FdoClass>pclass1 = FdoClass::Create(L"Class1", L"Class1 Desc");
    FdoPtr<FdoClass>pclass2 = FdoClass::Create(L"Class2", L"Class2 Desc");
    pClasses1->Add( pclass1 );
    pClasses1->Add( pclass2 );

    FdoPtr<FdoClass>pclass3 = FdoClass::Create(L"Class3", L"Class3 Desc");
    FdoPtr<FdoClass>pclass4 = FdoClass::Create(L"Class1", L"Class1 Desc");
    pClasses2->Add( pclass3 );
    pClasses2->Add( pclass4 );
    
    FdoPtr<FdoIDisposableCollection>result = pSchemas->FindClass(L"Class1");
    FDO_CPPUNIT_ASSERT( (result->GetCount() == 2 ) );
    result = pSchemas->FindClass(L"SchemaTwo:Class1");
    FDO_CPPUNIT_ASSERT( (result->GetCount() == 1 ) );
    result = pSchemas->FindClass(L"SchemaOne:Bogus");
    FDO_CPPUNIT_ASSERT( (result->GetCount() == 0 ) );
    result = pSchemas->FindClass(L"Bogus");
    FDO_CPPUNIT_ASSERT( (result->GetCount() == 0 ) );
}

void SchemaTest::testFeatureSchema()
{
    static FdoString*   pName = L"FSClassName";
    static FdoString*   pDesc = L"FeatureSchema Description";
    FdoFeatureSchema*   pFSchema = FdoFeatureSchema::Create(pName, pDesc);
    FdoClassCollection* pClasses = pFSchema->GetClasses();
    FdoPtr<FdoSchemaAttributeDictionary> pattrib;
    FdoPtr<FdoPropertyDefinitionCollection> pfeatureclassprops;
    FdoPtr<FdoDataPropertyDefinitionCollection> pfeatureclassidprops;

    // check base class functionality
    testSchemaElement(pFSchema, pName, pDesc);

    // Build a small schema with one of each element
    //
    //
    // FeatureSchema
    //  + FdoClass (Class,Class Desc)
    //      + Attrib (Attribute1, Value1)
    //      + Attrib (Attribute2, Value2)
    //  
    //  + FdoFeatureClass (Feature Class, Feature Class Desc)   
    //      + Attrib (FC Attribute1, FC Value1)
    //      + Attrib (FC Attribute2, FC Value2)
    //      Props = [pgeomprop, pobjprop]
    //      BaseProps = [pgeomprop]
    //      GeometryProp = pgeomprop
    // 
    // 
    //  pgeomprop = GeometricPropertyDefinition (Gemo Prop, Geom Prop Desc)
    //      GeometryType = Curve
    //      Readonly = True
    //      HasMeasure = True
    //      + Attrib (Geom Prop Attrib1, Value1)
    // 
    //  pdataprop = DataPropertyDefinition (Data Prop, Data Prop Desc)
    //      DataType = Boolean
    //      DefaultValue = 1
    //      Nullable = false
    //      Precision = 2
    //      ReadOnly = false
    //      Scale = 2
    //
    //  pobjprop = ObjectPropertyDefinition (Object Prop, Object Prop Desc)
    //      Class = FdoClass (Class, Class Desc)
    //      identityProperty = pdataprop

    FdoClass*           pclass = FdoClass::Create(L"Class", L"Class Desc");
    pattrib = pclass->GetAttributes();
    FDO_CPPUNIT_ASSERT(GET_REFCOUNT(pclass) == 1);
    pClasses->Add(pclass);
    FDO_CPPUNIT_ASSERT(GET_REFCOUNT(pclass) == 2);
    pattrib->Add(L"Attribute1", L"Value1");
    pattrib->Add(L"Attribute2", L"Value2");

    FdoGeometricPropertyDefinition* pgeomprop = FdoGeometricPropertyDefinition::Create(L"Geom Prop", L"Geom Prop Desc");
    FDO_CPPUNIT_ASSERT(GET_REFCOUNT(pgeomprop) == 1);
    pgeomprop->SetGeometryTypes(FdoGeometricType_Curve);
    pgeomprop->SetReadOnly(true);
    pgeomprop->SetHasMeasure(true);
    pgeomprop->SetHasElevation(true);
    pattrib = pgeomprop->GetAttributes();
    pattrib->Add(L"Geom Prop Attr1", L"Value1");

    FdoDataPropertyDefinition* pdataprop = FdoDataPropertyDefinition::Create(L"Data Prop", L"Data Prop Desc");
    FDO_CPPUNIT_ASSERT(GET_REFCOUNT(pdataprop) == 1);
    pdataprop->SetDataType(FdoDataType_Boolean);
    pdataprop->SetDefaultValue(L"1");
    pdataprop->SetNullable(false);

	// test incorrect precision:
	bool bCaughtPrecisionException = false;
	try
	{
		pdataprop->SetPrecision(-1);
	}
	catch (FdoException* ge)
	{
		bCaughtPrecisionException = true;
		pdataprop->SetPrecision(10);
        ge->Release ();
	}
	FDO_CPPUNIT_ASSERT(bCaughtPrecisionException);

	// test incorrect length:
	bool bCaughtLengthException = false;
	try
	{
		pdataprop->SetLength(-1);
	}
	catch (FdoException* ge)
	{
		bCaughtLengthException = true;
		pdataprop->SetLength(20);
        ge->Release ();
	}
	FDO_CPPUNIT_ASSERT(bCaughtLengthException);

    FdoBoolean bFailed = false;
    try {
        FdoFeatureClass*   pFClass = FdoFeatureClass::Create(L"bad:name", pDesc);
    }
    catch ( FdoSchemaException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Invalid Feature schema element name 'bad:name'; must not contain ':'. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Invalid Feature schema element name 'bad:name'; must not contain ':'. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
    }
    if ( !bFailed ) 
        CPPUNIT_FAIL( "Creating 'bad.name' feature class should have failed." );

    pdataprop->SetPrecision(2);
    pdataprop->SetReadOnly(false);
    pdataprop->SetScale(2);

    FdoDataPropertyP pAutogenProp = FdoDataPropertyDefinition::Create(L"AutoGen Prop", L"AutoGen Prop Desc");
    pAutogenProp->SetDataType(FdoDataType_Int64);
    pAutogenProp->SetNullable(false);
    pAutogenProp->SetReadOnly(false);
    pAutogenProp->SetIsAutoGenerated(true);

    // Turning on autogeneration causes readonly to flip to true.
    FDO_CPPUNIT_ASSERT( pAutogenProp->GetReadOnly() == true );

	// test making autogenerated property writable, must fail. 
	bool bCaughtAutoGenException = false;
	try
	{
		pAutogenProp->SetReadOnly(false);
	}
	catch (FdoException* ge)
	{
		bCaughtAutoGenException = true;
        ge->Release ();
	}
	FDO_CPPUNIT_ASSERT(bCaughtAutoGenException);

    // Test setting it back to non-autogenerated
    pAutogenProp->SetIsAutoGenerated(false);
    // Readonly should not change this time.
    FDO_CPPUNIT_ASSERT( pAutogenProp->GetReadOnly() == true );
    pAutogenProp->SetReadOnly(false);
    FDO_CPPUNIT_ASSERT( pAutogenProp->GetReadOnly() == false );

    FdoObjectPropertyDefinition* pobjprop = FdoObjectPropertyDefinition::Create(L"Object Prop", L"Object Prop Desc");
    pobjprop->SetClass(pclass);
    pobjprop->SetIdentityProperty(pdataprop);
    pobjprop->SetObjectType(FdoObjectType_Value);

    FdoFeatureClass*    pfeatureclass = FdoFeatureClass::Create(L"FeatureClass", L"FeatureClass Desc");
    pattrib = pfeatureclass->GetAttributes();
    FDO_CPPUNIT_ASSERT(GET_REFCOUNT(pfeatureclass) == 1);
    pClasses->Add(pfeatureclass);
    FDO_CPPUNIT_ASSERT(GET_REFCOUNT(pfeatureclass) == 2);
    pattrib->Add(L"FC Attribute1", L"FC Value1");
    pattrib->Add(L"FC Attribute2", L"FC Value2");
    pfeatureclassprops = pfeatureclass->GetProperties();
    pfeatureclassidprops = pfeatureclass->GetIdentityProperties();
    pfeatureclassprops->Add(pgeomprop);
    FDO_CPPUNIT_ASSERT(GET_REFCOUNT(pgeomprop) == 2);
    pfeatureclass->SetGeometryProperty(pgeomprop);
    FDO_CPPUNIT_ASSERT(GET_REFCOUNT(pgeomprop) == 3);
    pfeatureclassprops->Add(pobjprop);

    FDO_CPPUNIT_ASSERT( pfeatureclass->GetCapabilities() == NULL );

    try
    {
        pfeatureclassidprops->Add(pdataprop);   // should fail, not member of Properties yet
        FDO_CPPUNIT_ASSERT(FALSE);
    }
    catch (FdoException* e)
    {
        e->Release();
    }

    pfeatureclassprops->Add(pdataprop);
    pfeatureclassidprops->Add(pdataprop);

    FdoPropertyDefinitionCollection*   baseProps = FdoPropertyDefinitionCollection::Create(NULL);
    baseProps->Add(pgeomprop);
    pfeatureclass->SetBaseProperties(baseProps);
    baseProps->Release();
    baseProps = NULL;

    // Test modifying/accepting/changing schema
    FDO_CPPUNIT_ASSERT(pclass->GetElementState() == FdoSchemaElementState_Added);
    FDO_CPPUNIT_ASSERT(pfeatureclass->GetElementState() == FdoSchemaElementState_Added);
    FDO_CPPUNIT_ASSERT(pgeomprop->GetElementState() == FdoSchemaElementState_Added);

    // initially reject all changes
    pFSchema->RejectChanges();
    FDO_CPPUNIT_ASSERT(pFSchema->GetElementState() == FdoSchemaElementState_Unchanged);

    ///////////////////////////////////////////////////////
    // now redo and accept all changes

    pClasses->Add(pclass);
    FDO_CPPUNIT_ASSERT(pFSchema->GetElementState() == FdoSchemaElementState_Modified);
    pattrib = pclass->GetAttributes();
    pattrib->Add(L"Attribute1", L"Value1");
    pattrib->Add(L"Attribute2", L"Value2");

    pgeomprop->SetGeometryTypes(FdoGeometricType_Curve);
    pgeomprop->SetReadOnly(true);
    pgeomprop->SetHasMeasure(true);
    pgeomprop->SetHasElevation(true);
    pattrib = pgeomprop->GetAttributes();
    pattrib->Add(L"Geom Prop Attr1", L"Value1");

    pdataprop->SetDataType(FdoDataType_Boolean);
    pdataprop->SetDefaultValue(L"1");
    pdataprop->SetNullable(false);
    pdataprop->SetPrecision(2);
    pdataprop->SetReadOnly(false);
    pdataprop->SetScale(2);

    pobjprop->SetClass(pclass);
    pobjprop->SetIdentityProperty(pdataprop);
    pobjprop->SetObjectType(FdoObjectType_Value);

    pattrib = pfeatureclass->GetAttributes();
    pClasses->Add(pfeatureclass);
    pattrib->Add(L"FC Attribute1", L"FC Value1");
    pattrib->Add(L"FC Attribute2", L"FC Value2");
    pfeatureclassprops = pfeatureclass->GetProperties();
    pfeatureclassidprops = pfeatureclass->GetIdentityProperties();
    pfeatureclassprops->Add(pgeomprop);
    pfeatureclass->SetGeometryProperty(pgeomprop);
    pfeatureclassprops->Add(pobjprop);

    pfeatureclassprops->Add(pdataprop);
    pfeatureclassidprops->Add(pdataprop);

    baseProps = FdoPropertyDefinitionCollection::Create(NULL);
    baseProps->Add(pgeomprop);
    pfeatureclass->SetBaseProperties(baseProps);
    baseProps->Release();
    baseProps = NULL;

    FDO_CPPUNIT_ASSERT(GET_REFCOUNT(pclass) == 3);
    FDO_CPPUNIT_ASSERT(GET_REFCOUNT(pfeatureclass) == 2);
    FDO_CPPUNIT_ASSERT(GET_REFCOUNT(pgeomprop) == 4);     // self + featureClass baseProp + featureClass prop + featureClass GeometryProperty
    pFSchema->AcceptChanges();
    FDO_CPPUNIT_ASSERT(pFSchema->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pclass->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pfeatureclass->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pgeomprop->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pobjprop->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pdataprop->GetElementState() == FdoSchemaElementState_Unchanged);

    ///////////////////////////////////////////////////////////////////////////
    // Test changing and rejecting change to a class
    //
    pclass->SetName(L"New Name");
    FDO_CPPUNIT_ASSERT(pFSchema->GetElementState() == FdoSchemaElementState_Modified);
    pclass->SetDescription(L"New Description");

    pFSchema->RejectChanges();
    FDO_CPPUNIT_ASSERT(pFSchema->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pclass->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pfeatureclass->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pgeomprop->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pobjprop->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pdataprop->GetElementState() == FdoSchemaElementState_Unchanged);

    ///////////////////////////////////////////////////////////////////////////
    // Test deleting class from a collection via class->Delete()
    //
    FDO_CPPUNIT_ASSERT(pClasses->GetCount() == 2);
    pclass->Delete();
    FDO_CPPUNIT_ASSERT(pFSchema->GetElementState() == FdoSchemaElementState_Modified);
    FDO_CPPUNIT_ASSERT(pClasses->GetCount() == 2);
    pFSchema->AcceptChanges();
    FDO_CPPUNIT_ASSERT(pClasses->GetCount() == 1);
    FDO_CPPUNIT_ASSERT(pFSchema->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pclass->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pfeatureclass->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pgeomprop->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pobjprop->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pdataprop->GetElementState() == FdoSchemaElementState_Unchanged);

    ///////////////////////////////////////////////////////////////////////////
    // Test rejecting simple Description change in a property
    //
    pgeomprop->SetDescription(L"Something else");
    FDO_CPPUNIT_ASSERT(wcscmp(pgeomprop->GetDescription(), L"Something else") == 0);
    FDO_CPPUNIT_ASSERT(pfeatureclass->GetElementState() == FdoSchemaElementState_Modified);
    FDO_CPPUNIT_ASSERT(pFSchema->GetElementState() == FdoSchemaElementState_Modified);
    pFSchema->RejectChanges();
    FDO_CPPUNIT_ASSERT(wcscmp(pgeomprop->GetDescription(), L"Geom Prop Desc") == 0);
    FDO_CPPUNIT_ASSERT(pFSchema->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pclass->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pfeatureclass->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pgeomprop->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pobjprop->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pdataprop->GetElementState() == FdoSchemaElementState_Unchanged);

    ///////////////////////////////////////////////////////////////////////////
    // Test removing property (which should also remove from idProperties)
    //
    FDO_CPPUNIT_ASSERT(pfeatureclassidprops->GetCount() == 1);
    FDO_CPPUNIT_ASSERT(pfeatureclassprops->GetCount() == 3);
    pfeatureclassprops->Remove(pdataprop);
    FDO_CPPUNIT_ASSERT(pfeatureclassprops->GetCount() == 2);
    FDO_CPPUNIT_ASSERT(pfeatureclassidprops->GetCount() == 0);    // removing property should also remove from identity properties
    pFSchema->RejectChanges();
    FDO_CPPUNIT_ASSERT(pfeatureclassprops->GetCount() == 3);
    FDO_CPPUNIT_ASSERT(pfeatureclassidprops->GetCount() == 1);
    FDO_CPPUNIT_ASSERT(pFSchema->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pclass->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pfeatureclass->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pgeomprop->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pobjprop->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pdataprop->GetElementState() == FdoSchemaElementState_Unchanged);

    FDO_CPPUNIT_ASSERT(FdoPtr<FdoReadOnlyPropertyDefinitionCollection>(pfeatureclass->GetBaseProperties())->GetCount() == 1);
    FDO_CPPUNIT_ASSERT(FdoPtr<FdoSchemaElement>(FdoPtr<FdoReadOnlyPropertyDefinitionCollection>(pfeatureclass->GetBaseProperties())->GetItem(0)) == pgeomprop);
    FDO_CPPUNIT_ASSERT(FdoPtr<FdoSchemaElement>(FdoPtr<FdoReadOnlyPropertyDefinitionCollection>(pfeatureclass->GetBaseProperties())->GetItem(pgeomprop->GetName())) == pgeomprop);
    FDO_CPPUNIT_ASSERT(FdoPtr<FdoReadOnlyPropertyDefinitionCollection>(pfeatureclass->GetBaseProperties())->IndexOf(pgeomprop) == 0);
    FDO_CPPUNIT_ASSERT(FdoPtr<FdoReadOnlyPropertyDefinitionCollection>(pfeatureclass->GetBaseProperties())->Contains(pgeomprop));
    FDO_CPPUNIT_ASSERT(!FdoPtr<FdoReadOnlyPropertyDefinitionCollection>(pfeatureclass->GetBaseProperties())->Contains(pobjprop));

    ///////////////////////////////////////////////////////////////////////////
    // Test inserting a property -- make sure ElementState is propogated up properly
    //
    FDO_CPPUNIT_ASSERT(pFSchema->GetElementState() == FdoSchemaElementState_Unchanged);
    FdoDataPropertyDefinition* pdataprop2 = FdoDataPropertyDefinition::Create(L"Data Prop 2", L"Data Prop 2 Desc");
    FDO_CPPUNIT_ASSERT(GET_REFCOUNT(pdataprop2) == 1);
    pdataprop2->SetDataType(FdoDataType_Boolean);
    pdataprop2->SetDefaultValue(L"1");
    pdataprop2->SetNullable(false);
    pdataprop2->SetPrecision(2);
    pdataprop2->SetReadOnly(false);
    pdataprop2->SetScale(2);
    pfeatureclassprops->Add(pdataprop2);
    FDO_CPPUNIT_ASSERT(pFSchema->GetElementState() == FdoSchemaElementState_Modified);
    pdataprop2->Release();      // NOTE: Still in props collection
    pFSchema->AcceptChanges();
    FDO_CPPUNIT_ASSERT(pFSchema->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pclass->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pfeatureclass->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pgeomprop->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pobjprop->GetElementState() == FdoSchemaElementState_Unchanged);
    FDO_CPPUNIT_ASSERT(pdataprop->GetElementState() == FdoSchemaElementState_Unchanged);

    ///////////////////////////////////////////////////////////////////////////
    // try inserting a Class -- make sure ElementState is propogated up properly
    //
    FdoClass*           pclass2 = FdoClass::Create(L"Class 2", L"Class 2 Desc");
    FDO_CPPUNIT_ASSERT(GET_REFCOUNT(pclass2) == 1);
    pClasses->Add(pclass2);
    FDO_CPPUNIT_ASSERT(GET_REFCOUNT(pclass2) == 2);
    FDO_CPPUNIT_ASSERT(pclass2->GetElementState() == FdoSchemaElementState_Added);
    FDO_CPPUNIT_ASSERT(pFSchema->GetElementState() == FdoSchemaElementState_Modified);
    pclass2->Release();
    pFSchema->AcceptChanges();

    ///////////////////////////////////////////////////////////////////////////
    // Test reject change for feature class
    //
    FdoGeometricPropertyDefinition* pgeomprop2 = FdoGeometricPropertyDefinition::Create(L"Geom Prop", L"Geom Prop Desc");
    pfeatureclass->SetGeometryProperty(pgeomprop2);
    pFSchema->RejectChanges();
    pgeomprop2->Release();

    ///////////////////////////////////////////////////////////////////////////
    // Try to create set base to class definition of different type.
    //
    try
    {
        pclass->SetBaseClass(pfeatureclass);
        FDO_CPPUNIT_ASSERT(FALSE);  // should never reach this, an exception should be thrown because pclass would become its own grandparent
    }
    catch (FdoSchemaException* e)
    {
        e->Release();
    }

    ///////////////////////////////////////////////////////////////////////////
    // Try to create circular base-class references (infinite loop)
    //
    pFSchema->AcceptChanges();

    FdoFeatureClassP    pfeatureclass2 = FdoFeatureClass::Create(L"FeatureClass2", L"FeatureClass2 Desc");

    // NOTE: This is semi-circular because pfeatureclass contains pobjprop, and pobjprop refers to pclass
    pfeatureclass2->SetBaseClass(pfeatureclass);
    try
    {
        pfeatureclass->SetBaseClass(pfeatureclass2);
        FDO_CPPUNIT_ASSERT(FALSE);  // should never reach this, an exception should be thrown because pclass would become its own grandparent
    }
    catch (FdoSchemaException* e)
    {
        e->Release();
    }
    pFSchema->AcceptChanges();

    // TODO: If we leave the semi-circular reference, some memory doesn't get released
    //       when we release the pFSchema.  Do we need a way to break this reference count deadlock?
    pfeatureclass2->SetBaseClass(NULL);
    pFSchema->AcceptChanges();

    ///////////////////////////////////////////////////////////////////////////
    // TODO: test Relation methods

    ///////////////////////////////////////////////////////////////////////////
    // Cleanup
    ///////////////////////////////////////////////////////////////////////////


    // Force smart pointers to release
    pfeatureclassprops = NULL;
    pfeatureclassidprops = NULL;
    pattrib = NULL;

    pClasses->Release();
    pFSchema->Release();

    FDO_CPPUNIT_ASSERT(pfeatureclass->GetParent() == NULL);
    FDO_CPPUNIT_ASSERT(GET_REFCOUNT(pfeatureclass) == 1);
    pfeatureclass->Release();
    FDO_CPPUNIT_ASSERT(GET_REFCOUNT(pgeomprop) == 1);
    pgeomprop->Release();
    FDO_CPPUNIT_ASSERT(GET_REFCOUNT(pdataprop) == 3);   // once for self, twice for pobjprop (m_identityProperty and m_identityPropertyCHANGED)
    FDO_CPPUNIT_ASSERT(GET_REFCOUNT(pclass) == 3);      // once for self, twice for pobjprop (m_class and m_classCHANGED)
    FDO_CPPUNIT_ASSERT(GET_REFCOUNT(pobjprop) == 1);
    pobjprop->Release();
    FDO_CPPUNIT_ASSERT(GET_REFCOUNT(pdataprop) == 1);
    pdataprop->Release();
    FDO_CPPUNIT_ASSERT(GET_REFCOUNT(pclass) == 1);
    pclass->Release();

    bFailed = false;
    try {
        pFSchema = FdoFeatureSchema::Create(L"bad.name", pDesc);
    }
    catch ( FdoSchemaException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Invalid Feature schema element name 'bad.name'; must not contain '.'. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Invalid Feature schema element name 'bad.name'; must not contain '.'. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
    }
    if ( !bFailed ) 
        CPPUNIT_FAIL( "Creating 'bad.name' feature schema should have failed." );

    bFailed = false;
    try {
        pdataprop = FdoDataPropertyDefinition::Create(L"bad.name", pDesc);
    }
    catch ( FdoSchemaException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Invalid Feature schema element name 'bad.name'; must not contain '.'. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Invalid Feature schema element name 'bad.name'; must not contain '.'. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
    }
    if ( !bFailed ) 
        CPPUNIT_FAIL( "Creating 'bad.name' property should have failed." );

}

/*TODO:R2 or later
void SchemaTest::testAssociationFeatureClass()
{
    static FdoString*   pName = L"AFClassName";
    static FdoString*   pDesc = L"AssociationFeatureClass Description";

    FdoAssociationFeatureClass* pAFClass = FdoAssociationFeatureClass::Create(pName, pDesc);

    // check constructor created things OK
    FDO_CPPUNIT_ASSERT(pAFClass->GetClassType() == FdoClassType_AssociationFeatureClass);

    // check base class functionality
    testClassDefinition(pAFClass, pName, pDesc);

    pAFClass->Release();
}
*/

void SchemaTest::testClass()
{
    static FdoString*   pName = L"ClassName";
    static FdoString*   pDesc = L"Class Description";

    FdoClass*   pClass = FdoClass::Create(pName, pDesc);

    // check constructor created things OK
    FDO_CPPUNIT_ASSERT(pClass->GetClassType() == FdoClassType_Class);

    // check base class functionality
    testClassDefinition(pClass, pName, pDesc);

    pClass->Release();
}

void SchemaTest::testFeatureClass()
{
    static FdoString*   pName = L"FClassName";
    static FdoString*   pDesc = L"FeatureClass Description";

    FdoFeatureClass*   pFClass = FdoFeatureClass::Create(pName, pDesc);

    // check constructor created things OK
    FDO_CPPUNIT_ASSERT(pFClass->GetClassType() == FdoClassType_FeatureClass);

    FdoBoolean bFailed = false;
    try {
        FdoFeatureClass*   pFClass = FdoFeatureClass::Create(L"bad:name", pDesc);
    }
    catch ( FdoSchemaException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Invalid Feature schema element name 'bad:name'; must not contain ':'. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Invalid Feature schema element name 'bad:name'; must not contain ':'. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
    }
    if ( !bFailed ) 
        CPPUNIT_FAIL( "Creating 'bad:name' feature class should have failed." );

    // check base class functionality
    testClassDefinition(pFClass, pName, pDesc);

    // TODO: test Get/SetGeometryProperty()

    pFClass->Release();

    bFailed = false;
    try {
        FdoFeatureClass*   pFClass = FdoFeatureClass::Create(L"bad.name", pDesc);
    }
    catch ( FdoSchemaException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Invalid Feature schema element name 'bad.name'; must not contain '.'. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Invalid Feature schema element name 'bad.name'; must not contain '.'. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
    }
    if ( !bFailed ) 
        CPPUNIT_FAIL( "Creating 'bad.name' feature class should have failed." );
}

void SchemaTest::testSubClass()
{
	FdoPtr<FdoFeatureSchema> pSchema = FdoFeatureSchema::Create( L"TestSchema", L"" );

	// A non-abstract base class

	FdoPtr<FdoFeatureClass> pBaseClass = FdoFeatureClass::Create( L"Base Class", L"" );
	pBaseClass->SetIsAbstract(false);

	FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(pBaseClass->GetProperties())->Add( pProp );
	FdoPtr<FdoDataPropertyDefinitionCollection>(pBaseClass->GetIdentityProperties())->Add( pProp );

	FdoPtr<FdoClassCollection>(pSchema->GetClasses())->Add( pBaseClass );

	// Try to define an identity property on a subclass

	FdoPtr<FdoFeatureClass> pClass = FdoFeatureClass::Create( L"ClassA", L"" );
	pClass->SetIsAbstract(false);
	pClass->SetBaseClass( pBaseClass );

	pProp = FdoDataPropertyDefinition::Create( L"Id", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(pClass->GetProperties())->Add( pProp );
    try
    {
		FdoPtr<FdoDataPropertyDefinitionCollection>(pClass->GetIdentityProperties())->Add( pProp );
        FDO_CPPUNIT_ASSERT(FALSE);  // should never reach this, an exception should be thrown because tried to add an id property to class with base class
    }
    catch (FdoSchemaException* e)
    {
        e->Release();
    }

	// Try to set the base class for a class with identity properties

	pClass = FdoFeatureClass::Create( L"ClassB", L"" );
	pClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( L"Id", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(pClass->GetProperties())->Add( pProp );
	FdoPtr<FdoDataPropertyDefinitionCollection>(pClass->GetIdentityProperties())->Add( pProp );
    try
    {
		pClass->SetBaseClass( pBaseClass );
        FDO_CPPUNIT_ASSERT(FALSE);  // should never reach this, an exception should be thrown because tried to base class for class with id properties
    }
    catch (FdoSchemaException* e)
    {
        e->Release();
    }

	// Create a valid subclass.

	pClass = FdoFeatureClass::Create( L"ClassC", L"" );
	pClass->SetIsAbstract(false);
	pClass->SetBaseClass( pBaseClass );

	pProp = FdoDataPropertyDefinition::Create( L"Id", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(pClass->GetProperties())->Add( pProp );

	FdoPtr<FdoClassCollection>(pSchema->GetClasses())->Add( pClass );
}

void SchemaTest::testRasterClass ()
{
    static FdoString* class_name = L"Raster Class Name";
    static FdoString* class_description = L"Raster Class Description";
    static FdoString* raster_property_name = L"Raster Property Name";
    static FdoString* raster_property_description = L"Raster Property Description";

    FdoPtr<FdoFeatureClass> cls = FdoFeatureClass::Create (class_name, class_description);
    FDO_CPPUNIT_ASSERT (FdoClassType_FeatureClass == cls->GetClassType ());
    FdoPtr<FdoPropertyDefinitionCollection> properties = cls->GetProperties ();
    FdoPtr<FdoRasterPropertyDefinition> raster = FdoRasterPropertyDefinition::Create (raster_property_name, raster_property_description);
    properties->Add (raster);
}

/*TODO:R2 or later
void SchemaTest::testLineSegmentClass()
{
    static FdoString*   pName = L"LSClassName";
    static FdoString*   pDesc = L"LineSegmentClass Description";

    FdoLineSegmentClass*   pLSClass = FdoLineSegmentClass::Create(pName, pDesc);

    // check constructor created things OK
    FDO_CPPUNIT_ASSERT(pLSClass->GetClassType() == FdoClassType_LineSegmentClass);

    // check base class functionality
    testClassDefinition(pLSClass, pName, pDesc);

    // TODO: test Get/SetGeometryProperty()

    pLSClass->Release();
}

void SchemaTest::testTinEdgeClass()
{
    static FdoString*   pName = L"TEClassName";
    static FdoString*   pDesc = L"TinEdgeClass Description";

    FdoTinEdgeClass*   pTEClass = FdoTinEdgeClass::Create(pName, pDesc);

    // check constructor created things OK
    FDO_CPPUNIT_ASSERT(pTEClass->GetClassType() == FdoClassType_TinEdgeClass);

    // check base class functionality
    testClassDefinition(pTEClass, pName, pDesc);

    pTEClass->Release();
}

void SchemaTest::testTinPointClass()
{
    static FdoString*   pName = L"TPClassName";
    static FdoString*   pDesc = L"TinPointClass Description";

    FdoTinPointClass*   pTPClass = FdoTinPointClass::Create(pName, pDesc);

    // check constructor created things OK
    FDO_CPPUNIT_ASSERT(pTPClass->GetClassType() == FdoClassType_TinPointClass);

    // check base class functionality
    testClassDefinition(pTPClass, pName, pDesc);

    pTPClass->Release();
}

void SchemaTest::testTopologyFeatureClass()
{
    static FdoString*   pName = L"TFClassName";
    static FdoString*   pDesc = L"TopologyFeatureClass Description";

    FdoTopologyFeatureClass*   pTFClass = FdoTopologyFeatureClass::Create(pName, pDesc);

    // check constructor created things OK
    FDO_CPPUNIT_ASSERT(pTFClass->GetClassType() == FdoClassType_TopologyFeatureClass);

    // check base class functionality
    testClassDefinition(pTFClass, pName, pDesc);

    pTFClass->Release();
}

void SchemaTest::testTinFeatureClass()
{
    static FdoString*   pName = L"TFClassName";
    static FdoString*   pDesc = L"TinFeatureClass Description";

    // TODO: pass TinEdgeClass and TinPointClass to constructor
    FdoTinFeatureClass*   pTFClass = FdoTinFeatureClass::Create(pName, pDesc, NULL, NULL);

    // check constructor created things OK
    FDO_CPPUNIT_ASSERT(pTFClass->GetClassType() == FdoClassType_TinFeatureClass);
    FDO_CPPUNIT_ASSERT(pTFClass->GetEdgeClass() == NULL);
    FDO_CPPUNIT_ASSERT(pTFClass->GetPointClass() == NULL);

    // check base class functionality
    testCoverageFeatureClass(pTFClass, pName, pDesc);

    // TODO: test Get/SetEdgeClass() and Get/SetPointClass()
    // TODO: Test GetCoverageProperties()

    pTFClass->Release();
}

void SchemaTest::testLineSegmentFeatureClass()
{
    static FdoString*   pName = L"LSFClassName";
    static FdoString*   pDesc = L"LineSegmentFeatureClass Description";

    // TODO: pass LineSegmentClass to constructor
    FdoLineSegmentFeatureClass*   pLSFClass = FdoLineSegmentFeatureClass::Create(pName, pDesc, NULL);

    // check constructor created things OK
    FDO_CPPUNIT_ASSERT(pLSFClass->GetClassType() == FdoClassType_LineSegmentFeatureClass);
    FDO_CPPUNIT_ASSERT(pLSFClass->GetSegmentClass() == NULL);

    // check base class functionality
    testSegmentFeatureClass(pLSFClass, pName, pDesc);

    // TODO: test Set/GetSegmentClass

    pLSFClass->Release();
}

void SchemaTest::testPointSegmentFeatureClass()
{
    static FdoString*   pName = L"PSFClassName";
    static FdoString*   pDesc = L"PointSegmentFeatureClass Description";

    FdoPointSegmentFeatureClass*   pPSFClass = FdoPointSegmentFeatureClass::Create(pName, pDesc);

    // check constructor created things OK
    FDO_CPPUNIT_ASSERT(pPSFClass->GetClassType() == FdoClassType_PointSegmentFeatureClass);

    // check base class functionality
    testSegmentFeatureClass(pPSFClass, pName, pDesc);

    pPSFClass->Release();
}
*/
void SchemaTest::testAssociationOK()
{
    static FdoString*   pName = L"FSClassName";
    static FdoString*   pDesc = L"FeatureSchema Description";
    FdoPtr<FdoFeatureSchema>   pFSchema = FdoFeatureSchema::Create(pName, pDesc);
    FdoPtr<FdoClassCollection> pClasses = pFSchema->GetClasses();
    FdoPtr<FdoDataPropertyDefinition> pProp;

    // Create a feature class
    FdoPtr<FdoFeatureClass>    pfeatureclass = FdoFeatureClass::Create(L"FeatureClass", L"FeatureClass Desc");
    // Add identity property
    pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"Id Prop" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
    FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( pProp );
	FdoPtr<FdoDataPropertyDefinitionCollection>(pfeatureclass->GetIdentityProperties())->Add( pProp );
    // Add first name and last name properties
    pProp = FdoDataPropertyDefinition::Create( L"FirstName", L"First Name" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( pProp );
    pProp = FdoDataPropertyDefinition::Create( L"LastName", L"Last Name" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( pProp );

    // Create a class
    FdoPtr<FdoClass>  pclass = FdoClass::Create(L"Class", L"Class Desc");
    // Add identity property
    pProp = FdoDataPropertyDefinition::Create( L"Id", L"Id Prop" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
    FdoPtr<FdoPropertyDefinitionCollection>(pclass->GetProperties())->Add( pProp );
	FdoPtr<FdoDataPropertyDefinitionCollection>(pclass->GetIdentityProperties())->Add( pProp );
    // Add name one and name two properties
    pProp = FdoDataPropertyDefinition::Create( L"NameOne", L"Name One" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(pclass->GetProperties())->Add( pProp );
    pProp = FdoDataPropertyDefinition::Create( L"NameTwo", L"Name Two" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(pclass->GetProperties())->Add( pProp );

    // Add the classes to the schema class collection
    pClasses->Add( pfeatureclass );
    pClasses->Add( pclass );

    // Create a valid association property
    FdoPtr<FdoAssociationPropertyDefinition> passprop = FdoAssociationPropertyDefinition::Create(L"Association Prop1", L"Association Prop Desc");
    passprop->SetAssociatedClass(pclass);
    passprop->SetLockCascade( true );
    passprop->SetDeleteRule( FdoDeleteRule_Prevent );
    passprop->SetReverseName( L"Reverse Name" );

    FdoPtr<FdoDataPropertyDefinition> pprop = (FdoDataPropertyDefinition*)
        FdoPtr<FdoPropertyDefinitionCollection>(pclass->GetProperties())->GetItem( L"NameOne" );
    FdoPtr<FdoDataPropertyDefinitionCollection>(passprop->GetIdentityProperties())->Add( pprop );
   
    pProp = (FdoDataPropertyDefinition*)FdoPtr<FdoPropertyDefinitionCollection>(pclass->GetProperties())->GetItem( L"NameTwo" );
    FdoPtr<FdoDataPropertyDefinitionCollection>(passprop->GetIdentityProperties())->Add( pProp );
        
    pProp = (FdoDataPropertyDefinition*)FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->GetItem( L"FirstName" );
    FdoPtr<FdoDataPropertyDefinitionCollection>(passprop->GetReverseIdentityProperties())->Add( pProp );
        
    pProp = (FdoDataPropertyDefinition*)FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->GetItem( L"LastName" );
    FdoPtr<FdoDataPropertyDefinitionCollection>(passprop->GetReverseIdentityProperties())->Add(  pProp );
        
    // Add the association property to the feature class property collection
    FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( passprop );

    pFSchema->AcceptChanges();
}

void SchemaTest::testAssociationFail1()
{
    static FdoString*   pName = L"FSClassName";
    static FdoString*   pDesc = L"FeatureSchema Description";
    FdoPtr<FdoFeatureSchema>   pFSchema = FdoFeatureSchema::Create(pName, pDesc);
    FdoPtr<FdoClassCollection> pClasses = pFSchema->GetClasses();
    
    // Create a feature class
    FdoPtr<FdoFeatureClass>    pfeatureclass = FdoFeatureClass::Create(L"FeatureClass", L"FeatureClass Desc");
    // Add identity property
    FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"Id Prop" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
    FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( pProp );
	FdoPtr<FdoDataPropertyDefinitionCollection>(pfeatureclass->GetIdentityProperties())->Add( pProp );
    // Add first name and last name properties
    pProp = FdoDataPropertyDefinition::Create( L"FirstName", L"First Name" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( pProp );
    pProp = FdoDataPropertyDefinition::Create( L"LastName", L"Last Name" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( pProp );


    // Create a class
    FdoPtr<FdoClass> pclass = FdoClass::Create(L"Class", L"Class Desc");
    // Add identity property
    pProp = FdoDataPropertyDefinition::Create( L"Id", L"Id Prop" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
    FdoPtr<FdoPropertyDefinitionCollection>(pclass->GetProperties())->Add( pProp );
	FdoPtr<FdoDataPropertyDefinitionCollection>(pclass->GetIdentityProperties())->Add( pProp );
    // Add name one and name two properties
    pProp = FdoDataPropertyDefinition::Create( L"NameOne", L"Name One" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(pclass->GetProperties())->Add( pProp );
    pProp = FdoDataPropertyDefinition::Create( L"NameTwo", L"Name Two" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(pclass->GetProperties())->Add( pProp );

    // Add the classes to the schema class collection
    pClasses->Add( pfeatureclass );
    pClasses->Add( pclass );

    // Create an invalid association property( identity and reverse identity are not of the same types
    FdoPtr<FdoAssociationPropertyDefinition> passprop = FdoAssociationPropertyDefinition::Create(L"Association Prop2", L"Association Prop 2 Desc");
    passprop->SetAssociatedClass(pclass);
    passprop->SetReverseName(L"Reverse");
    FdoPtr<FdoDataPropertyDefinitionCollection>(passprop->GetIdentityProperties())->Add( 
        FdoPtr<FdoDataPropertyDefinition>( (FdoDataPropertyDefinition*)
            FdoPtr<FdoPropertyDefinitionCollection>(pclass->GetProperties())->GetItem( L"NameOne" )
        )
    );

    FdoPtr<FdoDataPropertyDefinitionCollection>(passprop->GetIdentityProperties())->Add( 
        FdoPtr<FdoDataPropertyDefinition>( (FdoDataPropertyDefinition*)
            FdoPtr<FdoPropertyDefinitionCollection>(pclass->GetProperties())->GetItem( L"NameTwo" )
        )
    );
    FdoPtr<FdoDataPropertyDefinitionCollection>(passprop->GetReverseIdentityProperties())->Add( 
        FdoPtr<FdoDataPropertyDefinition>( (FdoDataPropertyDefinition*)
            FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->GetItem( L"FirstName" )
        )
    );
    FdoPtr<FdoDataPropertyDefinitionCollection>(passprop->GetReverseIdentityProperties())->Add( 
        FdoPtr<FdoDataPropertyDefinition>( (FdoDataPropertyDefinition*)
            FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->GetItem( L"FeatId" )
        )
    );
    // Add the invalid association property to the feature class property collection
    FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( passprop );
    bool failed = false;
    try
    {
        pFSchema->AcceptChanges();
    }
    catch( FdoSchemaException *exp )
    {
        // Expected exception
        pFSchema->RejectChanges();
        exp->Release();
        failed = true;
    }
    if( ! failed )
        CPPUNIT_FAIL("testAssociationFail1 did not fail");
}
void SchemaTest::testAssociationFail2()
{
    static FdoString*   pName = L"FSClassName";
    static FdoString*   pDesc = L"FeatureSchema Description";
    FdoPtr<FdoFeatureSchema>   pFSchema = FdoFeatureSchema::Create(pName, pDesc);
    FdoPtr<FdoClassCollection> pClasses = pFSchema->GetClasses();
    
    // Create a feature class
    FdoPtr<FdoFeatureClass>    pfeatureclass = FdoFeatureClass::Create(L"FeatureClass", L"FeatureClass Desc");
    // Add identity property
    FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"Id Prop" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
    FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( pProp );
	FdoPtr<FdoDataPropertyDefinitionCollection>(pfeatureclass->GetIdentityProperties())->Add( pProp );
    // Add first name and last name properties
    pProp = FdoDataPropertyDefinition::Create( L"FirstName", L"First Name" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( pProp );
    pProp = FdoDataPropertyDefinition::Create( L"LastName", L"Last Name" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( pProp );


    // Create a class
    FdoPtr<FdoClass> pclass = FdoClass::Create(L"Class", L"Class Desc");
    // Add identity property
    pProp = FdoDataPropertyDefinition::Create( L"Id", L"Id Prop" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
    FdoPtr<FdoPropertyDefinitionCollection>(pclass->GetProperties())->Add( pProp );
	FdoPtr<FdoDataPropertyDefinitionCollection>(pclass->GetIdentityProperties())->Add( pProp );
    // Add name one and name two properties
    pProp = FdoDataPropertyDefinition::Create( L"NameOne", L"Name One" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(pclass->GetProperties())->Add( pProp );
    pProp = FdoDataPropertyDefinition::Create( L"NameTwo", L"Name Two" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(pclass->GetProperties())->Add( pProp );

    // Add the classes to the schema class collection
    pClasses->Add( pfeatureclass );
    pClasses->Add( pclass );

    // Create an invalid association by not setting the associated class
    FdoPtr<FdoAssociationPropertyDefinition> passprop = FdoAssociationPropertyDefinition::Create(L"Association Prop3", L"Association Prop 3 Desc");
    //passprop->SetAssociatedClass(pclass);
    FdoPtr<FdoDataPropertyDefinitionCollection>(passprop->GetIdentityProperties())->Add( 
        FdoPtr<FdoDataPropertyDefinition>( (FdoDataPropertyDefinition*)
            FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->GetItem( L"FirstName" )
        )
    );
    FdoPtr<FdoDataPropertyDefinitionCollection>(passprop->GetReverseIdentityProperties())->Add( 
        FdoPtr<FdoDataPropertyDefinition>( (FdoDataPropertyDefinition*)
            FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->GetItem( L"FirstName" )
        )
    );
    // Add the invalid association property to the feature class property collection
    FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( passprop );
    bool failed = false;
    try
    {
        pFSchema->AcceptChanges();
    }
    catch( FdoSchemaException *exp )
    {
        // Expected exception
        pFSchema->RejectChanges();
        exp->Release();
        failed = true;
    }
    if( ! failed )
        CPPUNIT_FAIL("testAssociationFail2 did not fail");
}

void SchemaTest::testAssociationFail3()
{
    static FdoString*   pName = L"FSClassName";
    static FdoString*   pDesc = L"FeatureSchema Description";
    FdoPtr<FdoFeatureSchema>   pFSchema = FdoFeatureSchema::Create(pName, pDesc);
    FdoPtr<FdoClassCollection> pClasses = pFSchema->GetClasses();
    
    // Create a feature class
    FdoPtr<FdoFeatureClass>    pfeatureclass = FdoFeatureClass::Create(L"FeatureClass", L"FeatureClass Desc");
    // Add identity property
    FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"Id Prop" );
	pProp->SetDataType( FdoDataType_Int64 );
    pProp->SetIsAutoGenerated( true );
	pProp->SetNullable(false);
    FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( pProp );
	FdoPtr<FdoDataPropertyDefinitionCollection>(pfeatureclass->GetIdentityProperties())->Add( pProp );
    // Add first name and last name properties
    pProp = FdoDataPropertyDefinition::Create( L"FirstName", L"First Name" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( pProp );
    pProp = FdoDataPropertyDefinition::Create( L"LastName", L"Last Name" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( pProp );


    // Create a class
    FdoPtr<FdoClass> pclass = FdoClass::Create(L"Class", L"Class Desc");
    // Add identity property
    pProp = FdoDataPropertyDefinition::Create( L"Id", L"Id Prop" );
	pProp->SetDataType( FdoDataType_Int64 );
    pProp->SetIsAutoGenerated( true );
	pProp->SetNullable(false);
    FdoPtr<FdoPropertyDefinitionCollection>(pclass->GetProperties())->Add( pProp );
	FdoPtr<FdoDataPropertyDefinitionCollection>(pclass->GetIdentityProperties())->Add( pProp );
    // Add name one and name two properties
    pProp = FdoDataPropertyDefinition::Create( L"NameOne", L"Name One" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(pclass->GetProperties())->Add( pProp );
    pProp = FdoDataPropertyDefinition::Create( L"NameTwo", L"Name Two" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(pclass->GetProperties())->Add( pProp );

    // Add the classes to the schema class collection
    pClasses->Add( pfeatureclass );
    pClasses->Add( pclass );

    // Create an invalid a reverse idenitity property with an autogenerated property(FeatId)(should fail)
    FdoPtr<FdoAssociationPropertyDefinition> passprop = FdoAssociationPropertyDefinition::Create(L"Association Prop3", L"Association Prop 3 Desc");
    passprop->SetAssociatedClass(pclass);
    FdoPtr<FdoDataPropertyDefinitionCollection>(passprop->GetIdentityProperties())->Add( 
        FdoPtr<FdoDataPropertyDefinition>( (FdoDataPropertyDefinition*)
            FdoPtr<FdoPropertyDefinitionCollection>(pclass->GetProperties())->GetItem( L"Id" )
        )
    );
    FdoPtr<FdoDataPropertyDefinitionCollection>(passprop->GetReverseIdentityProperties())->Add( 
        FdoPtr<FdoDataPropertyDefinition>( (FdoDataPropertyDefinition*)
            FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->GetItem( L"FeatId" )
        )
    );
    
    // Add the invalid association property to the feature class property collection
    FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( passprop );
    bool failed = false;
    try
    {
        pFSchema->AcceptChanges();
    }
    catch( FdoSchemaException *exp )
    {
        // Expected exception
        pFSchema->RejectChanges();
        exp->Release();
        failed = true;
    }
    if( ! failed )
        CPPUNIT_FAIL("testAssociationFail2 did not fail");
}

void SchemaTest::testNetworkOK()
{
    FdoPtr<FdoFeatureSchema>   pFSchema = FdoFeatureSchema::Create(L"FSClassName", L"FeatureSchema Description");
    FdoPtr<FdoClassCollection>pClasses = pFSchema->GetClasses();

    // Create a network layer class
    FdoPtr<FdoNetworkLayerClass>layer = FdoNetworkLayerClass::Create(L"TestLayer",L"A network test Layer class");
    // Add property to the layer class
    FdoPtr<FdoDataPropertyDefinition>pProp = FdoDataPropertyDefinition::Create( L"LayerPropertyOne", L"A layer test property" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(layer->GetProperties())->Add( pProp );


    // Create a network class
    FdoPtr<FdoNetworkClass>network = FdoNetworkClass::Create(L"TestNetwork",L"A network test class");
    // Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"NetworkPropertyOne", L"A network test property" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(network->GetProperties())->Add( pProp );
    // Set the layer class
    network->SetLayerClass( layer );

    // Create a parent network node class
    FdoPtr<FdoNetworkNodeFeatureClass>parentNetworkNode = FdoNetworkNodeFeatureClass::Create(L"TestNetworkParentNode",L"A parent network node test class");
    // Set the layer, network and cost properties
    FdoPtr<FdoAssociationPropertyDefinition> layerAssoc = FdoAssociationPropertyDefinition::Create(L"Layer Association Prop", L"The layer association prop");
    layerAssoc->SetAssociatedClass(layer);
    FdoPtr<FdoAssociationPropertyDefinition> networkAssoc = FdoAssociationPropertyDefinition::Create(L"Network Association Prop", L"The network association prop");
    networkAssoc->SetAssociatedClass(network);
    parentNetworkNode->SetLayerProperty( layerAssoc );
    parentNetworkNode->SetNetworkProperty( networkAssoc );

    // Create a network node class
    FdoPtr<FdoNetworkNodeFeatureClass>networkNode = FdoNetworkNodeFeatureClass::Create(L"TestNetworkNode",L"A network node test class");
    // Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"Speed", L"A speed property" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(networkNode->GetProperties())->Add( pProp );
    // Set the layer, network and cost properties
    layerAssoc = FdoAssociationPropertyDefinition::Create(L"Layer Association Prop", L"The layer association prop");
    layerAssoc->SetAssociatedClass(layer);
    networkAssoc = FdoAssociationPropertyDefinition::Create(L"Network Association Prop", L"The network association prop");
    networkAssoc->SetAssociatedClass(network);
    networkNode->SetLayerProperty( layerAssoc );
    networkNode->SetNetworkProperty( networkAssoc );
    networkNode->SetCostProperty( pProp );
    FdoPtr<FdoAssociationPropertyDefinition> parentAssoc = FdoAssociationPropertyDefinition::Create(L"Parent Association Prop", L"The parent association prop");
    parentAssoc->SetAssociatedClass(parentNetworkNode);
    networkNode->SetParentNetworkFeatureProperty( parentAssoc );

    // Create a network link class
    FdoPtr<FdoNetworkLinkFeatureClass>networkLink = FdoNetworkLinkFeatureClass::Create(L"TestNetworkLink",L"A network link test class");
	// Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"Speed", L"A speed property" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(false);
    FdoPtr<FdoPropertyDefinitionCollection>(networkLink->GetProperties())->Add( pProp );
    // Set the network and cost properties
    networkLink->SetNetworkProperty( networkAssoc );
    networkLink->SetCostProperty( pProp );
    // Set the start and end nodes
    FdoPtr<FdoAssociationPropertyDefinition> nodeAssoc = FdoAssociationPropertyDefinition::Create(L"Network node Association Prop", L"The network node association prop");
    nodeAssoc->SetAssociatedClass(networkNode);
    networkLink->SetStartNodeProperty( nodeAssoc );
    networkLink->SetEndNodeProperty( nodeAssoc );

    // Add the classes to the schema class collection
    pClasses->Add( layer );
    pClasses->Add( network );
    pClasses->Add( networkNode );
    pClasses->Add( networkLink );
    pClasses->Add( parentNetworkNode );

    try
    {
        pFSchema->AcceptChanges();
    }
    catch( FdoSchemaException *exp )
    {
        exp->Release();
    }
}

//
// This test will attemp to create a link and set the start node and end node from different network
// than the one used to create the link.
void SchemaTest::testNetworkFail1()
{
    // Create a network layer class
    FdoPtr<FdoNetworkLayerClass>layer = FdoNetworkLayerClass::Create(L"TestLayer",L"A network test Layer class");
    // Add property to the layer class
    FdoPtr<FdoDataPropertyDefinition>pProp = FdoDataPropertyDefinition::Create( L"LayerPropertyOne", L"A layer test property" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(layer->GetProperties())->Add( pProp );

    // Create the first network class
    FdoPtr<FdoNetworkClass>network1 = FdoNetworkClass::Create(L"TestNetwork1",L"A network test class");
    // Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"NetworkPropertyOne", L"A network test property" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(network1->GetProperties())->Add( pProp );
    // Set the layer class
    network1->SetLayerClass( layer );

    // Create a second network class
    FdoPtr<FdoNetworkClass>network2 = FdoNetworkClass::Create(L"TestNetwork2",L"A network test class");
    // Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"NetworkPropertyOne", L"A network test property" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(network2->GetProperties())->Add( pProp );
    // Set the layer class
    network2->SetLayerClass( layer );


    // Create the first network node class
    FdoPtr<FdoNetworkNodeFeatureClass>networkNode1 = FdoNetworkNodeFeatureClass::Create(L"TestNetworkNode",L"A network node test class");
    // Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"Speed", L"A speed property" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(networkNode1->GetProperties())->Add( pProp );
    // Set the layer, network and cost properties
    FdoPtr<FdoAssociationPropertyDefinition> layerAssoc = FdoAssociationPropertyDefinition::Create(L"Layer Association Prop", L"The layer association prop");
    layerAssoc->SetAssociatedClass(layer);
    FdoPtr<FdoAssociationPropertyDefinition> networkAssoc = FdoAssociationPropertyDefinition::Create(L"Network Association Prop", L"The network association prop");
    networkAssoc->SetAssociatedClass(network1);
    networkNode1->SetLayerProperty( layerAssoc );
    networkNode1->SetNetworkProperty( networkAssoc );
    networkNode1->SetCostProperty( pProp );

    // Create the second network node class
    FdoPtr<FdoNetworkNodeFeatureClass>networkNode2 = FdoNetworkNodeFeatureClass::Create(L"TestNetworkNode",L"A network node test class");
    // Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"Speed", L"A speed property" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(networkNode2->GetProperties())->Add( pProp );
    // Set the layer, network and cost properties
    layerAssoc = FdoAssociationPropertyDefinition::Create(L"Layer Association Prop", L"The layer association prop");
    layerAssoc->SetAssociatedClass(layer);
    networkAssoc = FdoAssociationPropertyDefinition::Create(L"Network Association Prop", L"The network association prop");
    networkAssoc->SetAssociatedClass(network2);
    networkNode2->SetLayerProperty( layerAssoc );
    networkNode2->SetNetworkProperty( networkAssoc );
    networkNode2->SetCostProperty( pProp );

    // Create a network link class
    FdoPtr<FdoNetworkLinkFeatureClass>networkLink = FdoNetworkLinkFeatureClass::Create(L"TestNetworkLink",L"A network link test class");
	// Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"Speed", L"A speed property" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(false);
    FdoPtr<FdoPropertyDefinitionCollection>(networkLink->GetProperties())->Add( pProp );
    // Set the network and cost properties
    networkLink->SetNetworkProperty( networkAssoc );
    networkLink->SetCostProperty( pProp );
    // Set the start and end nodes
    FdoPtr<FdoAssociationPropertyDefinition> nodeAssoc = FdoAssociationPropertyDefinition::Create(L"Network node Association Prop", L"The network node association prop");
    nodeAssoc->SetAssociatedClass(networkNode1);
    bool failed = false;
    try
    {
        networkLink->SetStartNodeProperty( nodeAssoc );
        networkLink->SetEndNodeProperty( nodeAssoc );
    }
    catch( FdoSchemaException *exp )
    {
        // Excepted exception
        exp->Release();
        failed = true;
    }
    if( !failed )
        CPPUNIT_FAIL("testNetworkFail1 failed to fail!!"); 
}

void SchemaTest::testNetworkFail2()
{
    // Create a network layer class
    FdoPtr<FdoNetworkLayerClass>layer = FdoNetworkLayerClass::Create(L"TestLayer",L"A network test Layer class");
    // Add property to the layer class
    FdoPtr<FdoDataPropertyDefinition>pProp = FdoDataPropertyDefinition::Create( L"LayerPropertyOne", L"A layer test property" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(layer->GetProperties())->Add( pProp );


    // Create a network class
    FdoPtr<FdoNetworkClass>network = FdoNetworkClass::Create(L"TestNetwork",L"A network test class");
    // Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"NetworkPropertyOne", L"A network test property" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(network->GetProperties())->Add( pProp );
    // Set the layer class
    network->SetLayerClass( layer );

    // Create a network node class
    FdoPtr<FdoNetworkNodeFeatureClass>networkNode = FdoNetworkNodeFeatureClass::Create(L"TestNetworkNode",L"A network node test class");
    // Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"Speed", L"A speed property" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(false);
    // Set the layer, network and cost properties
    FdoPtr<FdoAssociationPropertyDefinition> layerAssoc = FdoAssociationPropertyDefinition::Create(L"Layer Association Prop", L"The layer association prop");
    layerAssoc->SetAssociatedClass(layer);
    FdoPtr<FdoAssociationPropertyDefinition> networkAssoc = FdoAssociationPropertyDefinition::Create(L"Network Association Prop", L"The network association prop");
    networkAssoc->SetAssociatedClass(network);
    networkNode->SetLayerProperty( layerAssoc );
    networkNode->SetNetworkProperty( networkAssoc );
    bool failed = false;
    try
    {
        networkNode->SetCostProperty( pProp );
    }
    catch( FdoSchemaException *exp )
    {
        // Excepted exception
        exp->Release();
        failed = true;
    }
    if( !failed )
        CPPUNIT_FAIL("testNetworkFail1 failed to fail!!"); 
}

void SchemaTest::testXmlV2()
{
    FdoFeatureSchemasP schemas;

    try {

        // Read them back into an empty collection.
        schemas = FdoFeatureSchemaCollection::Create(NULL);
        schemas->ReadXml( 
            L"schema_all_v2.xml"
        );

        // Write them out again to check for loss of data on round-trip
        schemas->WriteXml( L"schema_all_v2_out.xml", FdoXmlFlagsP(FdoXmlFlags::Create(L"www.autodesk.com")) );

        // Format output files for comparison. Adds carriage returns.
        FdoIoFileStreamP reader = FdoIoFileStream::Create( L"schema_all_v2_out.xml", L"r" );
        FdoIoFileStreamP writer = FdoIoFileStream::Create( L"schema_all_v2_out.txt", L"w+t");
        XmlFormatter formatter2(reader, writer);
        formatter2.Format();

//Todo: get this working on linux
#ifdef _WIN32
        // Compare output and expected results.
        UnitTestUtil::CheckOutput( "schema_all_v2_out_master.txt", "schema_all_v2_out.txt" );
#endif
    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void SchemaTest::testXmlAll()
{
    FdoFeatureSchemasP schemas = FdoFeatureSchemaCollection::Create(NULL);
    FdoFeatureSchemasP schemas2;

    try {

        // Create 2 schemas: electric references acad.
        FdoFeatureSchemaP acadSchema = createAcadSchema();
        schemas->Add( acadSchema );
        schemas->Add( FdoFeatureSchemaP(createElectricSchema(acadSchema)) );

        // Write both schemas to an XML document.
        schemas->WriteXml( L"schema_all1.xml", FdoXmlFlagsP(FdoXmlFlags::Create(L"www.autodesk.com")) );

        // Read them back into an empty collection.
        schemas2 = FdoFeatureSchemaCollection::Create(NULL);
        schemas2->ReadXml( 
            L"schema_all1.xml", 
            FdoXmlFlagsP(FdoXmlFlags::Create(
                L"www.autodesk.com",
                FdoXmlFlags::ErrorLevel_High
            ))
        );

        // Write them out again to check for loss of data on round-trip
        schemas2->WriteXml( L"schema_all2.xml", FdoXmlFlagsP(FdoXmlFlags::Create(L"www.autodesk.com")) );

        // Format output files for comparison. Adds carriage returns.
        FdoIoFileStreamP reader = FdoIoFileStream::Create( L"schema_all1.xml", L"r" );
        FdoIoFileStreamP writer = FdoIoFileStream::Create( L"schema_all1.txt", L"w+t");
        XmlFormatter formatter1(reader, writer);
        formatter1.Format();

        reader = FdoIoFileStream::Create( L"schema_all2.xml", L"r" );
        writer = FdoIoFileStream::Create( L"schema_all2.txt", L"w+t");
        XmlFormatter formatter2(reader, writer);
        formatter2.Format();

//Todo: get this working on linux
#ifdef _WIN32
        // Compare output and expected results.
        UnitTestUtil::CheckOutput( "schema_all1_master.txt", "schema_all1.txt" );
        UnitTestUtil::CheckOutput( "schema_all1_master.txt", "schema_all2.txt" );
#endif
    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void SchemaTest::testXmlRefs()
{
    FdoFeatureSchemasP schemas = FdoFeatureSchemaCollection::Create(NULL);
    FdoFeatureSchemasP schemas2;

    try {
        // Create a number of schemas. Each schema has base class and
        // object property class references to the previous schema
        FdoFeatureSchemaP schema1 = createRefSchema(1);
        FdoFeatureSchemaP schema2 = createRefSchema(2, schema1);
        FdoFeatureSchemaP schema3 = createRefSchema(3, schema2);
        FdoFeatureSchemaP schema4 = createRefSchema(4, schema3);
        FdoFeatureSchemaP schema5 = createRefSchema(5, schema4);

        // Write them out to 3 XML documents in memory
        FdoIoMemoryStreamP schemaStreamA = FdoIoMemoryStream::Create();
        FdoIoMemoryStreamP schemaStreamB = FdoIoMemoryStream::Create();
        FdoIoMemoryStreamP schemaStreamC = FdoIoMemoryStream::Create();
        FdoIoMemoryStreamP schemaStream2 = FdoIoMemoryStream::Create();

        // 4 and 5 go into the 3rd document
        // This creates an XML document since it contains multiple schemas
        schemas->Add( schema5 );
        schemas->Add( schema4);
        schemas->WriteXml( schemaStreamC );

        // 3 into the 2nd document
        // This creates an XML Schema document since there is only one schema.
        schema3->WriteXml( schemaStreamB );

        // 1 and 2 into the first document.
        // This creates an XML document since it contains multiple schemas
        schemas = FdoFeatureSchemaCollection::Create(NULL);
        schemas->Add( schema2 );
        schemas->Add( schema1);
        schemas->WriteXml( schemaStreamA );
    
        schemaStreamA->Reset();
        schemaStreamB->Reset();
        schemaStreamC->Reset();

        schemas = FdoFeatureSchemaCollection::Create(NULL);
        // Read 1st document into empty collection.
        // This tests the resolving of references between schemas in same document.
        schemas->ReadXml( schemaStreamA ); 
        // Read 2nd document into collection.
        // This tests the resolving of references from a schema in an XML document
        // to schemas outside the document, but in the collection being read into.
        schemas->ReadXml( schemaStreamB );

        // Read 3rd document. Tests both types of references, cross-schema and
        // cross-document.
        schemas->ReadXml( schemaStreamC );

        // Write them all back out again. Tests round-tripping
        schemas->WriteXml( schemaStream2 );

        // Write the documents to files, with carriage return formatting.
        FdoIoFileStreamP f1a = FdoIoFileStream::Create( L"schema_refs1a.xml", L"w+t" );
        FdoIoFileStreamP f1b = FdoIoFileStream::Create( L"schema_refs1b.xsd", L"w+t" );
        FdoIoFileStreamP f1c = FdoIoFileStream::Create( L"schema_refs1c.xml", L"w+t" );
        FdoIoFileStreamP f2 = FdoIoFileStream::Create( L"schema_refs2.xml", L"w+t" );

        schemaStreamA->Reset();
        XmlFormatter fmt1a(schemaStreamA, f1a);
        fmt1a.Format();

        schemaStreamB->Reset();
        XmlFormatter fmt1b(schemaStreamB, f1b);
        fmt1b.Format();

        schemaStreamC->Reset();
        XmlFormatter fmt1c(schemaStreamC, f1c);
        fmt1c.Format();

        schemaStream2->Reset();
        XmlFormatter fmt2(schemaStream2, f2);
        fmt2.Format();


        schemaStreamC->Reset();
        schemas = FdoFeatureSchemaCollection::Create(NULL);

        // The following tests reading a schema with cross-document
        // references into an empty collection. This test will fail 
        // because these references will be unresolvable.
        FdoBoolean bFailed = false;
        try {
            schemas->ReadXml( schemaStreamC );
        }
        catch ( FdoException* e ) {
            UnitTestUtil::PrintException( e, "schema_refs_err.txt", true );
            e->Release ();
            bFailed = true;
        }

        if ( !bFailed ) 
            CPPUNIT_FAIL( "Loading of schemas 4 and 5 into empty feature schema collection should have failed" );

//Todo: get this working on linux
#ifdef _WIN32
        // Compare output against expected results.
        UnitTestUtil::CheckOutput( "schema_refs1a_master.txt", "schema_refs1a.xml" );
        UnitTestUtil::CheckOutput( "schema_refs1b_master.txt", "schema_refs1b.xsd" );
        UnitTestUtil::CheckOutput( "schema_refs1c_master.txt", "schema_refs1c.xml" );
        UnitTestUtil::CheckOutput( "schema_refs2_master.txt", "schema_refs2.xml" );
        // Todo: get this working on linux. Might need message catalogue.
        UnitTestUtil::CheckOutput( "schema_refs_err_master.txt", "schema_refs_err.txt" );
#endif
    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void SchemaTest::testXmlMerge()
{
    FdoFeatureSchemasP schemas = FdoFeatureSchemaCollection::Create(NULL);

    try {   
        // Initialize a schema collection with 2 schemas.
        FdoFeatureSchemaP acadSchema = createAcadSchema();
        schemas->Add( acadSchema );
        schemas->Add( FdoFeatureSchemaP(createElectricSchema(acadSchema)) );

        // schema_merge1a.xml contains the above schemas, with some modifications.
        // The following tests merging the modifications into the schema collection.
        schemas->ReadXml( 
            L"schema_merge1a.xml", 
            FdoXmlFlagsP(FdoXmlFlags::Create(
                L"www.autodesk.com",
                FdoXmlFlags::ErrorLevel_Normal
            ))
        );

        // Write out the schemas for verification
        schemas->WriteXml( L"schema_merge2a.xml", FdoXmlFlagsP(FdoXmlFlags::Create(L"www.autodesk.com")) );

        // Try merging into schemas with lots of references.
        // Create schema collection with 5 schema, each schema has
        // base class and object property class references to previous
        // schema.
        FdoFeatureSchemaP schema1 = createRefSchema(1);
        FdoFeatureSchemaP schema2 = createRefSchema(2, schema1);
        FdoFeatureSchemaP schema3 = createRefSchema(3, schema2);
        FdoFeatureSchemaP schema4 = createRefSchema(4, schema3);
        FdoFeatureSchemaP schema5 = createRefSchema(5, schema4);

        schemas = FdoFeatureSchemaCollection::Create(NULL);
        schemas->Add(schema1);
        schemas->Add(schema2);
        schemas->Add(schema3);
        schemas->Add(schema4);
        schemas->Add(schema5);

        // schema_merge1b.xml contains modifications to the 5 schemas.
        // The following tests the merging of these mods into the schema collection.
        schemas->ReadXml( 
            L"schema_merge1b.xml", 
            FdoXmlFlagsP(FdoXmlFlags::Create(
                L"fdo.osgeo.org/schemas/feature",
                FdoXmlFlags::ErrorLevel_Normal
            ))
        );

        // Write out the schemas for verification
        schemas->WriteXml( L"schema_merge2b.xml", FdoXmlFlagsP(FdoXmlFlags::Create(L"www.autodesk.com")) );

        // Test merging invalid modifications (changes to class or property type).
        FdoBoolean bFailed = false;
        try {
            schemas->ReadXml( 
                L"schema_merge_err.xml", 
                FdoXmlFlagsP(FdoXmlFlags::Create(
                    L"www.autodesk.com",
                    FdoXmlFlags::ErrorLevel_High
                ))
            );
        }
        catch ( FdoException* e ) {
            UnitTestUtil::PrintException( e, "schema_merge_err.txt", true );
            bFailed = true;
            e->Release ();
        }

        if ( !bFailed ) 
            CPPUNIT_FAIL( "Loading of schema_merge_err.xml should have failed" );

        // Write out the schemas for verification. In this case the schemas
        // should not have changed, since the last read had errors.
        schemas->WriteXml( L"schema_merge3b.xml", FdoXmlFlagsP(FdoXmlFlags::Create(L"www.autodesk.com")) );

        // Format the output files with carriage returns so they can 
        // be compared with masters.
        FdoIoFileStreamP reader = FdoIoFileStream::Create( L"schema_merge2a.xml", L"r" );
        FdoIoFileStreamP writer = FdoIoFileStream::Create( L"schema_merge2a.txt", L"w+t");
        XmlFormatter formatter1(reader, writer);
        formatter1.Format();

        reader = FdoIoFileStream::Create( L"schema_merge2b.xml", L"r" );
        writer = FdoIoFileStream::Create( L"schema_merge2b.txt", L"w+t");
        XmlFormatter formatter2(reader, writer);
        formatter2.Format();

        reader = FdoIoFileStream::Create( L"schema_merge3b.xml", L"r" );
        writer = FdoIoFileStream::Create( L"schema_merge3b.txt", L"w+t");
        XmlFormatter formatter3(reader, writer);
        formatter3.Format();

//Todo: get this working on linux
#ifdef _WIN32
        // Compare output against expected results.
        UnitTestUtil::CheckOutput( "schema_merge2a_master.txt", "schema_merge2a.txt" );
        UnitTestUtil::CheckOutput( "schema_merge2b_master.txt", "schema_merge2b.txt" );
        // Todo: get this working on linux. Might need message catalogue.
        UnitTestUtil::CheckOutput( "schema_merge_err_master.txt", "schema_merge_err.txt" );
        UnitTestUtil::CheckOutput( "schema_merge2b_master.txt", "schema_merge3b.txt" );
#endif
    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void SchemaTest::testXmlMultiDoc()
{/*
    FdoFeatureSchemasP schemas = FdoFeatureSchemaCollection::Create(NULL);

    try {
        // Create a number of schemas. Each schema has base class and
        // object property class references to the previous schema
        FdoFeatureSchemaP schema1 = createRefSchema(1);
        FdoFeatureSchemaP schema2 = createRefSchema(2, schema1);
        FdoFeatureSchemaP schema3 = createRefSchema(3, schema2);
        FdoFeatureSchemaP schema4 = createRefSchema(4, schema3);
        FdoFeatureSchemaP schema5 = createRefSchema(5, schema4);

        // Write them out to as separate documents in the same stream
        FdoIoMemoryStreamP schemaStream = FdoIoMemoryStream::Create();

        schema1->WriteXml(schemaStream);
        schema2->WriteXml(schemaStream);
        schema3->WriteXml(schemaStream);
        schema4->WriteXml(schemaStream);
        schema5->WriteXml(schemaStream);

        schemaStream->Reset();

        while ( schemaStream->GetIndex() < schemaStream->GetLength() )
            schemas->ReadXml(schemaStream);

        FdoIoMemoryStreamP schemaStream2 = FdoIoMemoryStream::Create();

        schemas->WriteXml(schemaStream2);

        schemaStream->Reset();
        XmlFormatter fmt(schemaStream2, FdoIoFileStreamP(FdoIoFileStream::Create(L"multidoc.xml", L"w+t")) );
        fmt.Format();
//Todo: get this working on linux
#ifdef _WIN32
        // Compare output against expected results.
        UnitTestUtil::CheckOutput( "multidoc_master.txt", "multidoc.xml" );
#endif
    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
*/
}

void SchemaTest::testXmlUnsupported()
{
    try {
        // The following reads in a schema that has GML elements not supported by FDO.
        // A number of errors are generated. The errors depend on the 
        // error level. All 4 error levels are tested.
        eachUnsupported( FdoXmlFlags::ErrorLevel_High, "schema_uns_high_master.txt", "schema_uns_high.txt" );
        eachUnsupported( FdoXmlFlags::ErrorLevel_Normal, "schema_uns_norm_master.txt", "schema_uns_norm.txt" );
        eachUnsupported( FdoXmlFlags::ErrorLevel_Low, "schema_uns_low_master.txt", "schema_uns_low.txt" );
        eachUnsupported( FdoXmlFlags::ErrorLevel_VeryLow, "schema_uns_vlow_master.txt", "schema_uns_vlow.txt" );
    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void SchemaTest::testRefErrors()
{
    FdoFeatureSchemasP schemas = FdoFeatureSchemaCollection::Create(NULL);
    FdoFeatureSchemasP schemas2;
    FdoInt32 i;

    try {
        // Create classes that reference classes in other schemas.
        FdoFeatureSchemaP schema1 = createRefSchema(1);
        FdoFeatureSchemaP schema2 = createRefSchema(2, schema1);
        FdoFeatureSchemaP schema3 = createRefSchema(3, schema2);
        FdoFeatureSchemaP schema4 = createRefSchema(4, schema3);

        schemas = FdoFeatureSchemaCollection::Create(NULL);
        schemas->Add(schema1);
        schemas->Add(schema2);
        schemas->Add(schema3);
        schemas->Add(schema4);

        // Detach some referenced classes from their schemas.

        FdoClassesP classes = schema1->GetClasses();
        for ( i = (classes->GetCount() - 1); i >= 0; i-- ) {
            classes->RemoveAt(i);
        }

        classes = schema3->GetClasses();
        for ( i = (classes->GetCount() - 1); i >= 0; i-- ) {
            classes->RemoveAt(i);
        }

        // Trap the bad references by writing to XML
        FdoIoMemoryStreamP schemaStream = FdoIoMemoryStream::Create();

        bool bFailed = false;

        // Try just one schema
        try {
            schema2->WriteXml( schemaStream );
        }
        catch ( FdoException* e ) {
            UnitTestUtil::PrintException( e, "schema_ref_err1.txt", true );
            bFailed = true;
            e->Release ();
        }

        if ( !bFailed  ) 
            CPPUNIT_FAIL( "WriteXml of schema should have failed" );

        schemaStream->Reset();

        // try all schemas.
        try {
            schemas->WriteXml( schemaStream );
        }
        catch ( FdoException* e ) {
            UnitTestUtil::PrintException( e, "schema_ref_err2.txt", true );
            bFailed = true;
            e->Release ();
        }

        if ( !bFailed  ) 
            CPPUNIT_FAIL( "WriteXml of schema collection should have failed" );

//Todo: get this working on linux
#ifdef _WIN32
        // Compare output against expected results.
        UnitTestUtil::CheckOutput( "schema_ref_err1_master.txt", "schema_ref_err1.txt" );
        UnitTestUtil::CheckOutput( "schema_ref_err2_master.txt", "schema_ref_err2.txt" );
#endif
    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void SchemaTest::testXmlExternal()
{
    FdoFeatureSchemasP schemas = FdoFeatureSchemaCollection::Create(NULL);
    FdoFeatureSchemasP schemas2 = FdoFeatureSchemaCollection::Create(NULL);

    try {

        FdoXmlFlagsP flags = FdoXmlFlags::Create(
            L"www.autodesk.com",
            FdoXmlFlags::ErrorLevel_VeryLow
        );

        /* Read the "externally created" schema */
        schemas->ReadXml( L"schema_ext1.xml", flags ); 

        // Write the schema back to an XML document.
        schemas->WriteXml( L"schema_ext2a.xml", flags );

        // Format output files for comparison. Adds carriage returns.
        FdoIoFileStreamP reader = FdoIoFileStream::Create( L"schema_ext2a.xml", L"r" );
        FdoIoFileStreamP writer = FdoIoFileStream::Create( L"schema_ext2a.txt", L"w+t");
        XmlFormatter formatter1(reader, writer);
        formatter1.Format();
//Todo: get this working on linux
#ifdef _WIN32
        // Compare output and expected results.
        UnitTestUtil::CheckOutput( "schema_ext_a_master.txt", "schema_ext2a.txt" );
#endif

        // The following tests an Association Property on gml:FeatureAssociationType
        // where gml and external schemas are read separately.

        flags->SetUseGmlId(true);
        schemas = FdoFeatureSchemaCollection::Create(NULL);
        FdoXmlReaderP gmlReader = FdoGml212Schema::CreateReader();
        schemas->ReadXml( gmlReader, flags );

        /* Read the "externally created" schema */
        schemas->ReadXml( L"schema_ext1b.xml", flags );

        FdoFeatureSchemaP schema = schemas->GetItem(L"External");
        // Write the schema back to an XML document.
        schema->WriteXml( L"schema_ext2b.xsd", FdoXmlFlagsP(FdoXmlFlags::Create(L"www.autodesk.com")) );

        // Format output files for comparison. Adds carriage returns.
        reader = FdoIoFileStream::Create( L"schema_ext2b.xsd", L"r" );
        writer = FdoIoFileStream::Create( L"schema_ext2b.txt", L"w+t");
        XmlFormatter formatter2(reader, writer);
        formatter2.Format();
//Todo: get this working on linux
#ifdef _WIN32
        // Compare output and expected results.
        UnitTestUtil::CheckOutput( "schema_ext_b_master.txt", "schema_ext2b.txt" );
#endif

        // The following tests an Association Property on gml:FeatureAssociationType
        // where gml and external schemas are read together.

        // Merge the gml and external schemas into one document
        schemas = FdoFeatureSchemaCollection::Create(NULL);
        gmlReader = FdoGml212Schema::CreateReader();
        FdoXmlReaderP extReader = FdoXmlReader::Create( L"schema_ext1b.xml" );
        FdoIoMemoryStreamP mergedStream = FdoIoMemoryStream::Create();
        FdoXmlCopyHandlerP copier = FdoXmlCopyHandler::Create( FdoXmlWriterP(FdoXmlWriter::Create(mergedStream)) );
        gmlReader->Parse( copier );
        extReader->Parse( copier );
        copier = NULL;
        mergedStream->Reset();

        // Read the merged document.
        schemas->ReadXml( mergedStream, flags );
        schema = schemas->GetItem(L"External");
        // Write the schema back to an XML document.
        schema->WriteXml( L"schema_ext2c.xsd", FdoXmlFlagsP(FdoXmlFlags::Create(L"www.autodesk.com")) );

        // Format output files for comparison. Adds carriage returns.
        reader = FdoIoFileStream::Create( L"schema_ext2c.xsd", L"r" );
        writer = FdoIoFileStream::Create( L"schema_ext2c.txt", L"w+t");
        XmlFormatter formatter3(reader, writer);
        formatter3.Format();
//Todo: get this working on linux
#ifdef _WIN32
        // Compare output and expected results.
        UnitTestUtil::CheckOutput( "schema_ext_c_master.txt", "schema_ext2c.txt" );
#endif
    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void SchemaTest::testXmlSchemaMappings()
{
    int i;
    FdoFeatureSchemasP schemas = FdoFeatureSchemaCollection::Create(NULL);

    try {
        FdoXmlFlagsP flags = FdoXmlFlags::Create(
            L"fdo.osgeo.org/schemas/feature", 
            FdoXmlFlags::ErrorLevel_VeryLow
        );

        flags->SetSchemaNameAsPrefix(true);
        flags->SetUseGmlId(true);

        FdoXmlReaderP gmlReader = FdoGml212Schema::CreateReader();

        FdoXmlReaderP extReader = FdoXmlReader::Create( L"schema_ext1.xml" );
        FdoIoMemoryStreamP mergedStream = FdoIoMemoryStream::Create();
        FdoXmlWriterP mergedWriter = FdoXmlWriter::Create( mergedStream );

        // RefHandler performs copy and accumulates any include or import
        // references.
        FdoXmlCopyHandlerP copier = FdoXmlCopyHandler::Create( mergedWriter );

        gmlReader->Parse(copier);
        extReader->Parse(copier);

        copier = NULL;
        mergedWriter = NULL;
        mergedStream->Reset();
        FdoXmlReaderP mergedReader = FdoXmlReader::Create(mergedStream);

        schemas->ReadXml(mergedReader, flags);
        FdoSchemaMappingsP mappings = schemas->GetXmlSchemaMappings();

        FdoXmlWriterP writer = FdoXmlWriter::Create( L"schema_mappings_212.xml", true, FdoXmlWriter::LineFormat_Indent, 50);
        mappings->WriteXml( writer );
        writer = NULL;

//Todo: get this working on linux
#ifdef _WIN32
        // Compare output and expected results.
        UnitTestUtil::CheckOutput( "schema_mappings_212_master.txt", "schema_mappings_212.xml" );
#endif
        FdoXmlSchemaMappingP mapping;
        for ( i = 0; i < mappings->GetCount(); i++ ) {
            mapping = (FdoXmlSchemaMapping*) mappings->GetItem(i);
            if ( FdoStringP(L"http://www.autodesk.com/External") == mapping->GetTargetNamespace() )
                break;
            mapping = NULL;
        }

        FDO_CPPUNIT_ASSERT(mapping);

        FdoXmlElementMappingP elemMapping = 
            FdoXmlElementMappingsP(mapping->GetElementMappings())->FindItem( L"TestFeat1" );
        FDO_CPPUNIT_ASSERT(elemMapping);

        FDO_CPPUNIT_ASSERT( wcscmp(elemMapping->GetGmlUri(),L"http://www.autodesk.com/External") == 0 );
        FDO_CPPUNIT_ASSERT( wcscmp(elemMapping->GetGmlLocalName(),L"TestFeat1") == 0 );

        FdoXmlClassMappingP classMapping = elemMapping->GetClassMapping();
        FDO_CPPUNIT_ASSERT(classMapping);

        FdoXmlElementMappingP subElemMapping = 
            FdoXmlElementMappingsP(classMapping->GetElementMappings())->FindItem(L"ext/LineProperty");
        FDO_CPPUNIT_ASSERT(subElemMapping);

        FDO_CPPUNIT_ASSERT( wcscmp(subElemMapping->GetGmlUri(),L"http://www.autodesk.com/External") == 0 );
        FDO_CPPUNIT_ASSERT( wcscmp(subElemMapping->GetGmlLocalName(),L"LineProperty") == 0 );

        FdoXmlClassMappingP propClassMapping = subElemMapping->GetClassMapping();
        FDO_CPPUNIT_ASSERT(classMapping);

        FDO_CPPUNIT_ASSERT(wcscmp(propClassMapping->GetWkBaseName(), L"CurveProperty") == 0);

        subElemMapping = 
            FdoXmlElementMappingsP(classMapping->GetElementMappings())->FindItem(L"GrpDouble");
        FDO_CPPUNIT_ASSERT(subElemMapping);

        FDO_CPPUNIT_ASSERT( wcscmp(subElemMapping->GetGmlUri(),L"http://www.autodesk.com/External") == 0 );
        FDO_CPPUNIT_ASSERT( wcscmp(subElemMapping->GetGmlLocalName(),L"GrpDouble") == 0 );
    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void SchemaTest::eachUnsupported( FdoXmlFlags::ErrorLevel level, char* masterFile, char* outFile)
{
    FdoFeatureSchemasP schemas = FdoFeatureSchemaCollection::Create(NULL);
    FdoBoolean bFailed = false;

    try {
        schemas->ReadXml( 
            L"schema_unsupported.xsd", 
            FdoXmlFlagsP(FdoXmlFlags::Create(
                L"fdo.osgeo.org/schemas/feature",
                level
            ))
        );
    }
    catch ( FdoException* e ) {

        if ( level == FdoXmlFlags::ErrorLevel_VeryLow )
            throw;

        UnitTestUtil::PrintException( e, outFile, true );
        bFailed = true;
        e->Release ();
    }

    if ( level != FdoXmlFlags::ErrorLevel_VeryLow ) { 
        if ( !bFailed  ) 
            CPPUNIT_FAIL( "Loading of schema_unsupported.xsd should have failed" );

#ifdef _WIN32
        UnitTestUtil::CheckOutput( masterFile, outFile );
#endif
    }
}

FdoFeatureSchema* SchemaTest::createAcadSchema()
{
	FdoFeatureSchema* pSchema = FdoFeatureSchema::Create( L"Acad", L"AutoCAD schema" );

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

	// More Id'less classes to test nested object properties

	FdoClassP pCoordVal = FdoClass::Create( L"AcDbVertexCoordinateValue", L"" );
	pCoordVal->SetIsAbstract(false);

	FdoDataPropertyP pCoordValSeq = FdoDataPropertyDefinition::Create( L"Seq", L"seq" );
	pCoordValSeq->SetDataType( FdoDataType_Int32 );
	pCoordValSeq->SetNullable(false);
	FdoPropertiesP(pCoordVal->GetProperties())->Add( pCoordValSeq  );

	pProp = FdoDataPropertyDefinition::Create( L"Value", L"" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(false);
	FdoPropertiesP(pCoordVal->GetProperties())->Add( pProp  );

	FdoClassesP(pSchema->GetClasses())->Add( pCoordVal );

	FdoClassP pVertex = FdoClass::Create( L"AcDbVertexData", L"" );
	pVertex->SetIsAbstract(false);

	FdoDataPropertyP pVertexSeq = FdoDataPropertyDefinition::Create( L"Seq", L"seq");
    pVertexSeq->SetIsAutoGenerated(true);
	pVertexSeq->SetDataType( FdoDataType_Int32 );
	pVertexSeq->SetNullable(false);
	FdoPropertiesP(pVertex->GetProperties())->Add( pVertexSeq  );

	FdoObjectPropertyP pObjProp = FdoObjectPropertyDefinition::Create( L"point", L"" );
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
    pProp->SetReadOnly(true);
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

    FdoPtr<FdoDataValue> minVal = FdoDataValue::Create( (FdoByte) 16 );
    FdoPtr<FdoDataValue> maxVal = FdoDataValue::Create( (FdoByte) 128 );
    FdoPtr<FdoPropertyValueConstraintRange> constraint = FdoPropertyValueConstraintRange::Create( minVal, maxVal );
    pProp->SetValueConstraint( constraint );

	pObjProp = FdoObjectPropertyDefinition::Create( L"xdata", L"xdata" );
	pObjProp->SetClass( pXData );
	pObjProp->SetIdentityProperty( pXDataSeq );
	pObjProp->SetObjectType( FdoObjectType_OrderedCollection );
	pObjProp->SetOrderType( FdoOrderType_Ascending );
	FdoPropertiesP(pEntClass->GetProperties())->Add( pObjProp );

	FdoClassesP(pSchema->GetClasses())->Add( pEntClass );

	// A sub-class to test Object and Data property inheritance.

	FdoFeatureClassP pPlineClass = FdoFeatureClass::Create( L"AcDb3dPolyline", L"AutoCAD 3d polyline" );
	pPlineClass->SetIsAbstract(false);
	pPlineClass->SetBaseClass( pEntClass );
	FdoClassesP(pSchema->GetClasses())->Add( pPlineClass );

	pProp = FdoDataPropertyDefinition::Create( L"Width", L"line width" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetPrecision(10);
	pProp->SetScale(5);
	pProp->SetNullable(false);
	FdoPropertiesP(pPlineClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Closed", L"is first and last points the same" );
	pProp->SetDataType( FdoDataType_Boolean );
	pProp->SetNullable(false);
	FdoPropertiesP(pPlineClass->GetProperties())->Add( pProp );

    pObjProp = FdoObjectPropertyDefinition::Create( L"vertices", L"" );
	pObjProp->SetClass( pVertex );
	pObjProp->SetIdentityProperty( pVertexSeq );
	pObjProp->SetObjectType( FdoObjectType_OrderedCollection );
	pObjProp->SetOrderType( FdoOrderType_Descending );
	FdoPropertiesP(pPlineClass->GetProperties())->Add( pObjProp );

	// The following tests object property nesting to 3 levels.

	FdoPtr<FdoFeatureClass> pHatchClass = FdoFeatureClass::Create( L"AcDbHatch", L"AutoCAD hatched polygon" );
	pHatchClass->SetIsAbstract(false);
	pHatchClass->SetBaseClass( pEntClass );

	pObjProp = FdoObjectPropertyDefinition::Create( L"edges", L"" );
	pObjProp->SetClass( pPlineClass );
	pObjProp->SetObjectType( FdoObjectType_Collection );
	FdoPropertiesP(pHatchClass->GetProperties())->Add( pObjProp );

	FdoClassesP(pSchema->GetClasses())->Add( pHatchClass );

    /* Create a number of interrelated network classes, that have no base classes */

    // Create a network layer class
    FdoPtr<FdoNetworkLayerClass>layer = FdoNetworkLayerClass::Create(L"TestLayer",L"A network test Layer class");
    // Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"LayerPropertyOne", L"A layer test property" );
	pProp->SetDataType( FdoDataType_String );
    pProp->SetLength(50);
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(layer->GetProperties())->Add( pProp );
	FdoClassesP(pSchema->GetClasses())->Add( layer );

    // Create a network class
    FdoPtr<FdoNetworkClass>network = FdoNetworkClass::Create(L"TestNetwork",L"A network test class");
    // Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"NetworkPropertyOne", L"A network test property" );
	pProp->SetDataType( FdoDataType_String );
    pProp->SetLength(100);
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(network->GetProperties())->Add( pProp );
    // Set the layer class
    network->SetLayerClass( layer );
	FdoClassesP(pSchema->GetClasses())->Add( network );

    // Create a parent network node class
    FdoPtr<FdoNetworkNodeFeatureClass>parentNetworkNode = FdoNetworkNodeFeatureClass::Create(L"TestNetworkParentNode",L"A parent network node test class");
    // Set the layer, network and cost properties
    FdoPtr<FdoAssociationPropertyDefinition> layerAssoc = FdoAssociationPropertyDefinition::Create(L"Layer Association Prop", L"The layer association prop");
    layerAssoc->SetAssociatedClass(layer);
    FdoPtr<FdoAssociationPropertyDefinition> networkAssoc = FdoAssociationPropertyDefinition::Create(L"Network Association Prop", L"The network association prop");
    networkAssoc->SetAssociatedClass(network);
    parentNetworkNode->SetLayerProperty( layerAssoc );
    parentNetworkNode->SetNetworkProperty( networkAssoc );
	FdoClassesP(pSchema->GetClasses())->Add( parentNetworkNode );

    // Create a network node class
    FdoPtr<FdoNetworkNodeFeatureClass>networkNode = FdoNetworkNodeFeatureClass::Create(L"TestNetworkNode",L"A network node test class");
    // Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"Speed", L"A speed property" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(networkNode->GetProperties())->Add( pProp );
    // Set the layer, network and cost properties
    layerAssoc = FdoAssociationPropertyDefinition::Create(L"Layer Association Prop", L"The layer association prop");
    layerAssoc->SetAssociatedClass(layer);
    networkAssoc = FdoAssociationPropertyDefinition::Create(L"Network Association Prop", L"The network association prop");
    networkAssoc->SetAssociatedClass(network);
    networkNode->SetLayerProperty( layerAssoc );
    networkNode->SetNetworkProperty( networkAssoc );
    networkNode->SetCostProperty( pProp );
    FdoPtr<FdoAssociationPropertyDefinition> parentAssoc = FdoAssociationPropertyDefinition::Create(L"Parent Association Prop", L"The parent association prop");
    parentAssoc->SetAssociatedClass(parentNetworkNode);
    networkNode->SetParentNetworkFeatureProperty( parentAssoc );
    FdoPtr<FdoAssociationPropertyDefinition> nodeFeatAssoc = FdoAssociationPropertyDefinition::Create(L"Feature Association Prop", L"The feature association prop");
    nodeFeatAssoc->SetAssociatedClass(pHatchClass);
    networkNode->SetReferencedFeatureProperty(nodeFeatAssoc);
	FdoClassesP(pSchema->GetClasses())->Add( networkNode );

    // Create a network link class
    FdoPtr<FdoNetworkLinkFeatureClass>networkLink = FdoNetworkLinkFeatureClass::Create(L"TestNetworkLink",L"A network link test class");
	// Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"Speed", L"A speed property" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(false);
    FdoPtr<FdoPropertyDefinitionCollection>(networkLink->GetProperties())->Add( pProp );
    // Set the network and cost properties
    networkLink->SetNetworkProperty( networkAssoc );
    networkLink->SetCostProperty( pProp );
    // Set the start and end nodes
    FdoPtr<FdoAssociationPropertyDefinition> nodeAssoc = FdoAssociationPropertyDefinition::Create(L"Network node Association Prop", L"The network node association prop");
    nodeAssoc->SetAssociatedClass(networkNode);
    networkLink->SetStartNodeProperty( nodeAssoc );
    networkLink->SetEndNodeProperty( nodeAssoc );
    FdoPtr<FdoAssociationPropertyDefinition> linkFeatAssoc = FdoAssociationPropertyDefinition::Create(L"Feature Association Prop", L"The feature association prop");
    linkFeatAssoc->SetAssociatedClass(pPlineClass);
    networkLink->SetReferencedFeatureProperty(linkFeatAssoc);
	FdoClassesP(pSchema->GetClasses())->Add( networkLink );

    /* Create a number of non-interrelated network classes, that have base classes */

    // Create a network layer class
    FdoPtr<FdoNetworkLayerClass>layer2 = FdoNetworkLayerClass::Create(L"TestLayer2",L"A network test Layer class");
    layer2->SetBaseClass(layer);
    // Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"LayerPropertyTwo", L"A layer test property" );
	pProp->SetDataType( FdoDataType_String );
    pProp->SetLength(52);
	pProp->SetNullable(true);
	FdoPtr<FdoPropertyDefinitionCollection>(layer2->GetProperties())->Add( pProp );
	FdoClassesP(pSchema->GetClasses())->Add( layer2 );

    // Create a network class
    FdoPtr<FdoNetworkClass>network2 = FdoNetworkClass::Create(L"TestNetwork2",L"A network test class");
    network2->SetBaseClass(network);
    // Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"NetworkPropertyTwo", L"A network test property" );
	pProp->SetDataType( FdoDataType_String );
    pProp->SetLength(102);
	pProp->SetNullable(true);
	FdoPtr<FdoPropertyDefinitionCollection>(network2->GetProperties())->Add( pProp );
	FdoClassesP(pSchema->GetClasses())->Add( network2 );

    // Create a parent network node class
    FdoPtr<FdoNetworkNodeFeatureClass>parentNetworkNode2 = FdoNetworkNodeFeatureClass::Create(L"TestNetworkParentNode2",L"A parent network node test class");
	parentNetworkNode2->SetBaseClass(parentNetworkNode);
    FdoClassesP(pSchema->GetClasses())->Add( parentNetworkNode2 );

    // Create a network node class
    FdoPtr<FdoNetworkNodeFeatureClass>networkNode2 = FdoNetworkNodeFeatureClass::Create(L"TestNetworkNode2",L"A network node test class");
    networkNode2->SetBaseClass(networkNode);
    // Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"Closed", L"" );
	pProp->SetDataType( FdoDataType_Boolean );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(networkNode2->GetProperties())->Add( pProp );
	FdoClassesP(pSchema->GetClasses())->Add( networkNode2 );

    // Create a network link class
    FdoPtr<FdoNetworkLinkFeatureClass>networkLink2 = FdoNetworkLinkFeatureClass::Create(L"TestNetworkLink2",L"A network link test class");
	networkLink2->SetBaseClass(networkLink);
    // Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"Material", L"" );
	pProp->SetDataType( FdoDataType_String );
    pProp->SetLength(52);
	pProp->SetNullable(false);
    FdoPtr<FdoPropertyDefinitionCollection>(networkLink2->GetProperties())->Add( pProp );
	FdoClassesP(pSchema->GetClasses())->Add( networkLink2 );

    return(pSchema);
}

FdoFeatureSchema*  SchemaTest::createElectricSchema( FdoFeatureSchema* pAcadSchema )
{
	FdoClassDefinitionP pEntClass = FdoClassesP(pAcadSchema->GetClasses())->GetItem( L"AcDbEntity" );
	FdoClassDefinitionP pPlineClass = FdoClassesP(pAcadSchema->GetClasses())->GetItem( L"AcDb3dPolyline" );

	FdoFeatureSchema* pSchema = FdoFeatureSchema::Create( L"Electrical", L"Electrical schema" );

	FdoSADP(pSchema->GetAttributes())->Add( L"Author", L"Thomas Edison" );

	/* An abstract base class */

	FdoFeatureClassP pDevClass = FdoFeatureClass::Create( L"Electric Device", L"electic base class" );
	pDevClass->SetIsAbstract(true);
    pDevClass->SetBaseClass(pEntClass);

	// Test nested object properties (ElectricDevice.furniture.xdata) where graphic's class has an id.

	FdoObjectPropertyP pObjProp = FdoObjectPropertyDefinition::Create( L"furniture", L"Acad entity" );
	pObjProp->SetObjectType( FdoObjectType_Value );
	pObjProp->SetClass( pEntClass );
	FdoPropertiesP(pDevClass->GetProperties())->Add( pObjProp );

	// Test geometry property

	FdoGeometricPropertyP pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"location and shape" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve | FdoGeometricType_Surface);
    pGeomProp->SetHasMeasure(true);
    pGeomProp->SetReadOnly(true);
    pGeomProp->SetSpatialContextAssociation(L"SC_0");
	FdoSADP(pGeomProp->GetAttributes())->Add( L"Measure Units", L"metres" );
	FdoPropertiesP(pDevClass->GetProperties())->Add( pGeomProp );

	pDevClass->SetGeometryProperty( pGeomProp );

	FdoClassesP(pSchema->GetClasses())->Add( pDevClass );

	/* Subclass with dictionary */

	FdoFeatureClassP pTransClass = FdoFeatureClass::Create( L"Transformer", L"" );
	pTransClass->SetIsAbstract(false);
	pTransClass->SetBaseClass( pDevClass );
	FdoSADP(pTransClass->GetAttributes())->Add( L"Rules DLL", L"transformer.dll" );
	FdoSADP(pTransClass->GetAttributes())->Add( L"Entrypoint", L"start_transformer" );

	// Add data properties of various types

    FdoDataPropertyP pProp = FdoDataPropertyDefinition::Create( L"Voltage", L"voltage" );
	FdoSADP(pProp->GetAttributes())->Add( L"Calculable", L"yes" );
	pProp->SetDataType( FdoDataType_Decimal );
	pProp->SetPrecision(10);
	pProp->SetScale(-2);
	pProp->SetNullable(false);
	FdoPropertiesP(pTransClass->GetProperties())->Add( pProp );

	FdoDataPropertyP pTransPhase = FdoDataPropertyDefinition::Create( L"Phase", L"A, B or C" );
	pTransPhase->SetDataType( FdoDataType_String );
	pTransPhase->SetLength(1);
	pTransPhase->SetNullable(false);
	FdoPropertiesP(pTransClass->GetProperties())->Add( pTransPhase );

    FdoPtr<FdoPropertyValueConstraintList> constraint = FdoPropertyValueConstraintList::Create();
    FdoPtr<FdoDataValueCollection> values = constraint->GetConstraintList();
    values->Add( FdoStringValue::Create(L"A") );
    values->Add( FdoStringValue::Create(L"B") );
    values->Add( FdoStringValue::Create(L"C") );
    pTransPhase->SetValueConstraint( constraint );

	FdoDataPropertyP pTransDate = FdoDataPropertyDefinition::Create( L"InstallDate", L"" );
	pTransDate->SetDataType( FdoDataType_DateTime );
	pTransDate->SetNullable(true);
	FdoPropertiesP(pTransClass->GetProperties())->Add( pTransDate );

	FdoDataPropertyP pTransPartNum = FdoDataPropertyDefinition::Create( L"PartNum", L"" );
	pTransPartNum->SetDataType( FdoDataType_Int16 );
	pTransPartNum->SetNullable(true);
	FdoPropertiesP(pTransClass->GetProperties())->Add( pTransPartNum );

	FdoDataPropertyP pTransVolume = FdoDataPropertyDefinition::Create( L"Volume", L"" );
	pTransVolume->SetDataType( FdoDataType_Single );
	pTransVolume->SetNullable(false);
	FdoPropertiesP(pTransClass->GetProperties())->Add( pTransVolume );

	pProp = FdoDataPropertyDefinition::Create( L"Picture", L"" );
	pProp->SetDataType( FdoDataType_BLOB );
	pProp->SetNullable(true);
	FdoPropertiesP(pTransClass->GetProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Specifications", L"" );
	pProp->SetDataType( FdoDataType_CLOB );
	pProp->SetNullable(false);
	FdoPropertiesP(pTransClass->GetProperties())->Add( pProp );

    FdoUniqueConstraintCollectionP uniCons = pTransClass->GetUniqueConstraints();

    FdoUniqueConstraintP uniCon = FdoUniqueConstraint::Create();
    FdoDataPropertiesP uniConProps = uniCon->GetProperties();
    uniConProps->Add( pTransPartNum);
    uniCons->Add( uniCon );

    uniCon = FdoUniqueConstraint::Create();
    uniConProps = uniCon->GetProperties();
    uniConProps->Add( pTransPhase);
    uniConProps->Add( pTransDate);
    uniCons->Add( uniCon );

    uniCon = FdoUniqueConstraint::Create();
    uniConProps = uniCon->GetProperties();
    uniConProps->Add( pTransVolume);
    uniCons->Add( uniCon );

    // Raster property (all defaults)

    FdoRasterPropertyP pRasterProp = FdoRasterPropertyDefinition::Create( L"Maintenance Notes", L"" );
    FdoPropertiesP(pTransClass->GetProperties())->Add( pRasterProp );

	FdoClassesP(pSchema->GetClasses())->Add( pTransClass );

    // Test class that is already suffixed by "Type". GML ComplexType should end up
    // as TransformerTypeType
	FdoClassP pTransTypeClass = FdoClass::Create( L"TransformerType", L"" );
	pTransTypeClass->SetIsAbstract(false);

    pProp = FdoDataPropertyDefinition::Create( L"Name", L"type name" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(30);
	pProp->SetNullable(false);
	FdoPropertiesP(pTransTypeClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pTransTypeClass->GetIdentityProperties())->Add( pProp );
	FdoClassesP(pSchema->GetClasses())->Add( pTransTypeClass );

    // Test class that is called "Type". GML ComplexType should end up
    // as TypeType
	FdoClassP pTypeClass = FdoClass::Create( L"Type", L"" );
	pTypeClass->SetIsAbstract(false);

    pProp = FdoDataPropertyDefinition::Create( L"Name", L"type name" );
	pProp->SetDataType( FdoDataType_String );
	pProp->SetLength(30);
	pProp->SetNullable(false);
	FdoPropertiesP(pTypeClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pTypeClass->GetIdentityProperties())->Add( pProp );
	FdoClassesP(pSchema->GetClasses())->Add( pTypeClass );

  	// Add a new non-inherited class that will later be removed.

	FdoFeatureClassP pPoleClass = FdoFeatureClass::Create( L"PoleTypeStructure", L"" );
	pPoleClass->SetIsAbstract(false);

	pProp = FdoDataPropertyDefinition::Create( L"Street", L"" );
	pProp->SetDataType( FdoDataType_String );
    pProp->SetLength(20);
	pProp->SetNullable(false);
	FdoPropertiesP(pPoleClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pPoleClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"ID", L"" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(false);
	FdoPropertiesP(pPoleClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pPoleClass->GetIdentityProperties())->Add( pProp );

	pProp = FdoDataPropertyDefinition::Create( L"Height", L"" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(false);
    pProp->SetDefaultValue( L"15" );
	FdoPropertiesP(pPoleClass->GetProperties())->Add( pProp );

	pGeomProp = FdoGeometricPropertyDefinition::Create( L"Location", L"point location" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point );
    pGeomProp->SetHasElevation(false);
	FdoPropertiesP(pPoleClass->GetProperties())->Add( pGeomProp );
	pPoleClass->SetGeometryProperty( pGeomProp );

	pGeomProp = FdoGeometricPropertyDefinition::Create( L"Shape", L"pole shape" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Curve );
    pGeomProp->SetSpatialContextAssociation( L"SC_1" );
	FdoPropertiesP(pPoleClass->GetProperties())->Add( pGeomProp );

    // Raster property (no defaults)
    pRasterProp = FdoRasterPropertyDefinition::Create( L"Picture", L"" );
	pRasterProp->SetNullable(true);
    pRasterProp->SetReadOnly(true);
    pRasterProp->SetDefaultImageXSize(4000);
    pRasterProp->SetDefaultImageYSize(2000);
    pRasterProp->SetSpatialContextAssociation(L"SC_0");

    FdoPtr<FdoRasterDataModel> model = FdoRasterDataModel::Create();
    model->SetDataModelType( FdoRasterDataModelType_Bitonal );
    model->SetOrganization( FdoRasterDataOrganization_Pixel );
    model->SetBitsPerPixel(24);
    model->SetTileSizeX(500);
    model->SetTileSizeY(250);
    model->SetDataType(FdoRasterDataType_Float);
    pRasterProp->SetDefaultDataModel(model);

    FdoPropertiesP(pPoleClass->GetProperties())->Add( pRasterProp );

    // Association property (all defaults)
    FdoAssociationPropertyP pAssocProp = FdoAssociationPropertyDefinition::Create( L"Mounted Transformer", L"sits on pole" );
    pAssocProp->SetAssociatedClass(pTransClass);

    FdoPropertiesP(pPoleClass->GetProperties())->Add( pAssocProp );

    // Raster property (all defaults except one model attribute).
    pRasterProp = FdoRasterPropertyDefinition::Create( L"Infrared", L"" );
	pRasterProp->SetNullable(false);

    model = FdoRasterDataModel::Create();
    model->SetBitsPerPixel(16);
    model->SetDataType(FdoRasterDataType_Integer);
    pRasterProp->SetDefaultDataModel(model);

    FdoPropertiesP(pPoleClass->GetProperties())->Add( pRasterProp );
    FdoClassesP(pSchema->GetClasses())->Add( pPoleClass );

    // The rest of the classes test various types of association properties

	FdoClassP pMaintClass = FdoClass::Create( L"Maint Hist", L"" );
	pMaintClass->SetIsAbstract(false);

    pProp = FdoDataPropertyDefinition::Create( L"id", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
	FdoPropertiesP(pMaintClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pMaintClass->GetIdentityProperties())->Add( pProp );

    FdoDataPropertyP pMaintPhase = FdoDataPropertyDefinition::Create( L"Phase", L"" );
	pMaintPhase->SetDataType( FdoDataType_String );
	pMaintPhase->SetLength(1);
	pMaintPhase->SetNullable(false);
	FdoPropertiesP(pMaintClass->GetProperties())->Add( pMaintPhase );

	FdoDataPropertyP pMaintPartNum = FdoDataPropertyDefinition::Create( L"PartNum", L"" );
	pMaintPartNum->SetDataType( FdoDataType_Int16 );
	pMaintPartNum->SetNullable(true);
	FdoPropertiesP(pMaintClass->GetProperties())->Add( pMaintPartNum );

    // Association property with multiple explicit identity properties and
    // reverse name.
    pAssocProp = FdoAssociationPropertyDefinition::Create( L"Transformer", L"" );
    pAssocProp->SetAssociatedClass(pTransClass);
    pAssocProp->SetReverseName( L"Maintenance" );
    pAssocProp->SetMultiplicity( L"1" );

    FdoDataPropertiesP(pAssocProp->GetIdentityProperties())->Add(pTransPhase);
    FdoDataPropertiesP(pAssocProp->GetIdentityProperties())->Add(pTransPartNum);
    FdoDataPropertiesP(pAssocProp->GetReverseIdentityProperties())->Add(pMaintPhase);
    FdoDataPropertiesP(pAssocProp->GetReverseIdentityProperties())->Add(pMaintPartNum);

    uniCons = pMaintClass->GetUniqueConstraints();

    uniCon = FdoUniqueConstraint::Create();
    uniConProps = uniCon->GetProperties();
    uniConProps->Add( pMaintPartNum);
    uniCons->Add( uniCon );

    FdoPropertiesP(pMaintClass->GetProperties())->Add( pAssocProp );
	FdoClassesP(pSchema->GetClasses())->Add( pMaintClass );

	FdoClassP pOutClass = FdoClass::Create( L"Outage Hist", L"" );
	pOutClass->SetIsAbstract(false);

    pProp = FdoDataPropertyDefinition::Create( L"id", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
	FdoPropertiesP(pOutClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pOutClass->GetIdentityProperties())->Add( pProp );

    FdoDataPropertyP pOutPhase = FdoDataPropertyDefinition::Create( L"Phase", L"" );
	pOutPhase->SetDataType( FdoDataType_String );
	pOutPhase->SetLength(1);
	pOutPhase->SetNullable(false);
	FdoPropertiesP(pOutClass->GetProperties())->Add( pOutPhase );

	FdoDataPropertyP pOutPartNum = FdoDataPropertyDefinition::Create( L"PartNum", L"" );
	pOutPartNum->SetDataType( FdoDataType_Int16 );
	pOutPartNum->SetNullable(true);
	FdoPropertiesP(pOutClass->GetProperties())->Add( pOutPartNum );

    // Association property with multiple explicit identity properties, but no
    // reverse name.
    pAssocProp = FdoAssociationPropertyDefinition::Create( L"Transformer", L"" );
    pAssocProp->SetAssociatedClass(pTransClass);
    pAssocProp->SetMultiplicity( L"1" );

    FdoDataPropertiesP(pAssocProp->GetIdentityProperties())->Add(pTransPhase);
    FdoDataPropertiesP(pAssocProp->GetIdentityProperties())->Add(pTransPartNum);
    FdoDataPropertiesP(pAssocProp->GetReverseIdentityProperties())->Add(pOutPhase);
    FdoDataPropertiesP(pAssocProp->GetReverseIdentityProperties())->Add(pOutPartNum);

    FdoPropertiesP(pOutClass->GetProperties())->Add( pAssocProp );
	FdoClassesP(pSchema->GetClasses())->Add( pOutClass );

	FdoClassP pTarClass = FdoClass::Create( L"Tarrings", L"" );
	pOutClass->SetIsAbstract(false);

    pProp = FdoDataPropertyDefinition::Create( L"id", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
	FdoPropertiesP(pTarClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pTarClass->GetIdentityProperties())->Add( pProp );

    // Association property with multiple implicit identity properties.
    pAssocProp = FdoAssociationPropertyDefinition::Create( L"Pole", L"" );
    pAssocProp->SetAssociatedClass(pPoleClass);
    pAssocProp->SetReverseName( L"Tar Applications" );

    FdoPropertiesP(pTarClass->GetProperties())->Add( pAssocProp );
	FdoClassesP(pSchema->GetClasses())->Add( pTarClass );

    /* Create a number of non-interrelated network classes, that have no base classes */

    // Create a network layer class
    FdoPtr<FdoNetworkLayerClass>layer = FdoNetworkLayerClass::Create(L"TestLayer",L"A network test Layer class");
    // Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"LayerPropertyOne", L"A layer test property" );
	pProp->SetDataType( FdoDataType_String );
    pProp->SetLength(50);
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(layer->GetProperties())->Add( pProp );
	FdoClassesP(pSchema->GetClasses())->Add( layer );

    // Create a network class
    FdoPtr<FdoNetworkClass>network = FdoNetworkClass::Create(L"TestNetwork",L"A network test class");
    // Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"NetworkPropertyOne", L"A network test property" );
	pProp->SetDataType( FdoDataType_String );
    pProp->SetLength(100);
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(network->GetProperties())->Add( pProp );
	FdoClassesP(pSchema->GetClasses())->Add( network );

    // Create a parent network node class
    FdoPtr<FdoNetworkNodeFeatureClass>parentNetworkNode = FdoNetworkNodeFeatureClass::Create(L"TestNetworkParentNode",L"A parent network node test class");
	FdoClassesP(pSchema->GetClasses())->Add( parentNetworkNode );

    // Create a network node class
    FdoPtr<FdoNetworkNodeFeatureClass>networkNode = FdoNetworkNodeFeatureClass::Create(L"TestNetworkNode",L"A network node test class");
    // Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"Speed", L"A speed property" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(networkNode->GetProperties())->Add( pProp );
    // Set the layer, network and cost properties
	FdoClassesP(pSchema->GetClasses())->Add( networkNode );

    // Create a network link class
    FdoPtr<FdoNetworkLinkFeatureClass>networkLink = FdoNetworkLinkFeatureClass::Create(L"TestNetworkLink",L"A network link test class");
	// Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"Speed", L"A speed property" );
	pProp->SetDataType( FdoDataType_Double );
	pProp->SetNullable(false);
    FdoPtr<FdoPropertyDefinitionCollection>(networkLink->GetProperties())->Add( pProp );
	FdoClassesP(pSchema->GetClasses())->Add( networkLink );

    /* Create a number of interrelated network classes, that have base classes 
     * The setting of non-default network association property settings is also tested.
     */

    // Create a network layer class
    FdoPtr<FdoNetworkLayerClass>layer2 = FdoNetworkLayerClass::Create(L"TestLayer2",L"A network test Layer class");
    layer2->SetBaseClass(layer);
    // Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"LayerPropertyTwo", L"A layer test property" );
	pProp->SetDataType( FdoDataType_String );
    pProp->SetLength(52);
	pProp->SetNullable(true);
	FdoPtr<FdoPropertyDefinitionCollection>(layer2->GetProperties())->Add( pProp );
	FdoClassesP(pSchema->GetClasses())->Add( layer2 );

    // Create a network class
    FdoPtr<FdoNetworkClass>network2 = FdoNetworkClass::Create(L"TestNetwork2",L"A network test class");
    network2->SetBaseClass(network);
    // Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"NetworkPropertyTwo", L"A network test property" );
	pProp->SetDataType( FdoDataType_String );
    pProp->SetLength(102);
	pProp->SetNullable(true);
	FdoPtr<FdoPropertyDefinitionCollection>(network2->GetProperties())->Add( pProp );
    // Set the layer class
    network2->SetLayerClass( layer2 );
	FdoClassesP(pSchema->GetClasses())->Add( network2 );

    // Create a parent network node class
    FdoPtr<FdoNetworkNodeFeatureClass>parentNetworkNode2 = FdoNetworkNodeFeatureClass::Create(L"TestNetworkParentNode2",L"A parent network node test class");
    parentNetworkNode2->SetBaseClass(parentNetworkNode);
    // Set the layer, network and cost properties
    FdoPtr<FdoAssociationPropertyDefinition> layerAssoc = FdoAssociationPropertyDefinition::Create(L"Layer Association Prop", L"The layer association prop");
    layerAssoc->SetAssociatedClass(layer2);
    layerAssoc->SetDeleteRule( FdoDeleteRule_Break );
    FdoPtr<FdoAssociationPropertyDefinition> networkAssoc = FdoAssociationPropertyDefinition::Create(L"Network Association Prop", L"The network association prop");
    networkAssoc->SetAssociatedClass(network2);
    networkAssoc->SetReverseName( L"Primitive" );
    parentNetworkNode2->SetLayerProperty( layerAssoc );
    parentNetworkNode2->SetNetworkProperty( networkAssoc );
    FdoPtr<FdoAssociationPropertyDefinition> parentNodeFeatAssoc = FdoAssociationPropertyDefinition::Create(L"Feature Association Prop", L"The feature association prop");
    parentNodeFeatAssoc->SetAssociatedClass(pPoleClass);
    parentNetworkNode2->SetReferencedFeatureProperty(parentNodeFeatAssoc);
	FdoClassesP(pSchema->GetClasses())->Add( parentNetworkNode2 );

    // Create a network node class
    FdoPtr<FdoNetworkNodeFeatureClass>networkNode2 = FdoNetworkNodeFeatureClass::Create(L"TestNetworkNode2",L"A network node test class");
    networkNode2->SetBaseClass(networkNode);
    // Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"Closed", L"" );
	pProp->SetDataType( FdoDataType_Boolean );
	pProp->SetNullable(false);
	FdoPtr<FdoPropertyDefinitionCollection>(networkNode2->GetProperties())->Add( pProp );
    // Set the layer, network and cost properties
    layerAssoc = FdoAssociationPropertyDefinition::Create(L"Layer Association Prop", L"The layer association prop");
    layerAssoc->SetAssociatedClass(layer2);
    networkAssoc = FdoAssociationPropertyDefinition::Create(L"Network Association Prop", L"The network association prop");
    networkAssoc->SetAssociatedClass(network2);
    networkAssoc->SetLockCascade(true);
    networkNode2->SetLayerProperty( layerAssoc );
    networkNode2->SetNetworkProperty( networkAssoc );
    FdoPtr<FdoAssociationPropertyDefinition> parentAssoc = FdoAssociationPropertyDefinition::Create(L"Parent Association Prop", L"The parent association prop");
    parentAssoc->SetAssociatedClass(parentNetworkNode2);
    parentAssoc->SetDeleteRule( FdoDeleteRule_Break );
    networkNode2->SetParentNetworkFeatureProperty( parentAssoc );
    FdoPtr<FdoAssociationPropertyDefinition> nodeFeatAssoc = FdoAssociationPropertyDefinition::Create(L"Feature Association Prop", L"The feature association prop");
    nodeFeatAssoc->SetAssociatedClass(pTransClass);
    networkNode2->SetReferencedFeatureProperty(nodeFeatAssoc);
	FdoClassesP(pSchema->GetClasses())->Add( networkNode2 );

    // Create a network link class
    FdoPtr<FdoNetworkLinkFeatureClass>networkLink2 = FdoNetworkLinkFeatureClass::Create(L"TestNetworkLink2",L"A network link test class");
	networkLink2->SetBaseClass(networkLink);
    // Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"Material", L"" );
	pProp->SetDataType( FdoDataType_String );
    pProp->SetLength(52);
	pProp->SetNullable(false);
    FdoPtr<FdoPropertyDefinitionCollection>(networkLink2->GetProperties())->Add( pProp );
    // Set the network and cost properties
    networkLink2->SetNetworkProperty( networkAssoc );
    // Set the start and end nodes
    FdoPtr<FdoAssociationPropertyDefinition> nodeAssocA = FdoAssociationPropertyDefinition::Create(L"Network node Association Prop A", L"The network node association prop");
    nodeAssocA->SetAssociatedClass(networkNode2);
    nodeAssocA->SetDeleteRule( FdoDeleteRule_Prevent );
    networkLink2->SetStartNodeProperty( nodeAssocA );
    FdoPtr<FdoAssociationPropertyDefinition> nodeAssocB = FdoAssociationPropertyDefinition::Create(L"Network node Association Prop B", L"The network node association prop");
    nodeAssocB->SetAssociatedClass(parentNetworkNode2);
    nodeAssocB->SetLockCascade(true);
    nodeAssocB->SetReverseName( L"FromLink" );
    networkLink2->SetEndNodeProperty( nodeAssocB );
    FdoPtr<FdoAssociationPropertyDefinition> linkFeatAssoc = FdoAssociationPropertyDefinition::Create(L"Feature Association Prop", L"The feature association prop");
    linkFeatAssoc->SetAssociatedClass(pPlineClass);
    linkFeatAssoc->SetDeleteRule( FdoDeleteRule_Break );
    linkFeatAssoc->SetLockCascade(true);
    linkFeatAssoc->SetReverseName( L"ElecNet2" );
    networkLink2->SetReferencedFeatureProperty(linkFeatAssoc);
	FdoClassesP(pSchema->GetClasses())->Add( networkLink2 );

    FdoPtr<FdoNetworkClass>network3 = FdoNetworkClass::Create(L"TestNetwork3",L"A network test class");
    network3->SetBaseClass(network2);
    // Add property to the layer class
    pProp = FdoDataPropertyDefinition::Create( L"NetworkPropertyThree", L"A network test property" );
	pProp->SetDataType( FdoDataType_String );
    pProp->SetLength(103);
	pProp->SetNullable(true);
	FdoPtr<FdoPropertyDefinitionCollection>(network3->GetProperties())->Add( pProp );
    // Set the layer class
    network3->SetLayerClass( layer2 );
	FdoClassesP(pSchema->GetClasses())->Add( network3 );

    return( pSchema );
}

FdoFeatureSchema* SchemaTest::createRefSchema( FdoInt32 idx, FdoFeatureSchema* pBaseSchema )
{
	FdoClassDefinitionP pBaseClass;
	FdoClassDefinitionP pBaseOpClass;
	FdoFeatureClassP pClass;
	FdoClassP pOpClass;
    FdoDataPropertyP pBaseOpId;
	FdoNetworkLayerClassP baseLayer;
	FdoNetworkClassP baseNetwork;
	FdoNetworkNodeFeatureClassP baseParentNetworkNode;
	FdoNetworkNodeFeatureClassP baseNetworkNode;
	FdoNetworkLinkFeatureClassP baseNetworkLink;
    
    if ( pBaseSchema ) {
        pBaseClass = FdoClassesP(pBaseSchema->GetClasses())->GetItem( L"Reference Class" );
        pBaseOpClass = FdoClassesP(pBaseSchema->GetClasses())->GetItem( L"Object Property Class" );
        pBaseOpId = (FdoDataPropertyDefinition*)(FdoPropertyDefinition*)(FdoPropertiesP(pBaseOpClass->GetProperties())->GetItem(0));   
        baseLayer = (FdoNetworkLayerClass*) FdoClassesP(pBaseSchema->GetClasses())->GetItem( L"TestLayer" );
        baseNetwork = (FdoNetworkClass*) FdoClassesP(pBaseSchema->GetClasses())->GetItem( L"TestNetwork" );
        baseParentNetworkNode = (FdoNetworkNodeFeatureClass*) FdoClassesP(pBaseSchema->GetClasses())->GetItem( L"TestNetworkParentNode" );
        baseNetworkNode = (FdoNetworkNodeFeatureClass*) FdoClassesP(pBaseSchema->GetClasses())->GetItem( L"TestNetworkNode" );
        baseNetworkLink = (FdoNetworkLinkFeatureClass*)FdoClassesP(pBaseSchema->GetClasses())->GetItem( L"TestNetworkLink" );
    }

	FdoFeatureSchema* pSchema = FdoFeatureSchema::Create( 
        (FdoString*) FdoStringP::Format( L"Reference Schema %d", idx ), 
        L"" 
    );

	/* An object property class */

	pOpClass = FdoClass::Create( L"Object Property Class", L"" );
	pOpClass->SetIsAbstract(false);

    FdoDataPropertyP pProp = FdoDataPropertyDefinition::Create( L"Prop 1", L"" );
	pProp->SetDataType( FdoDataType_Decimal );
	pProp->SetPrecision(10);
	pProp->SetScale(2);
	pProp->SetNullable(false);
	FdoPropertiesP(pOpClass->GetProperties())->Add( pProp );

	FdoClassesP(pSchema->GetClasses())->Add( pOpClass );

	pClass = FdoFeatureClass::Create( L"Reference Class", L"" );
	pClass->SetIsAbstract(false);
    if ( pBaseClass != NULL ) 
        pClass->SetBaseClass( pBaseClass );

    pProp = FdoDataPropertyDefinition::Create(
        FdoStringP::Format(L"Prop %d", idx), 
        L"" 
    );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );

    if ( pBaseClass == NULL ) 
    	FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

    if ( pBaseOpClass != NULL ) {
	    FdoObjectPropertyP pObjProp = FdoObjectPropertyDefinition::Create( 
            FdoStringP::Format(L"Object Property %d", idx), 
            L"" 
        );

	    pObjProp->SetObjectType( FdoObjectType_Collection );
        pObjProp->SetIdentityProperty( pBaseOpId );
	    pObjProp->SetClass( pBaseOpClass );
	    FdoPropertiesP(pClass->GetProperties())->Add( pObjProp );
    }

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

	pClass = FdoFeatureClass::Create( L"Concrete Class", L"" );
	pClass->SetIsAbstract(false);

    pProp = FdoDataPropertyDefinition::Create(
        FdoStringP::Format(L"Prop %d", idx), 
        L"" 
    );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
  	FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

    if ( pBaseOpClass != NULL ) {
	    FdoObjectPropertyP pObjProp = FdoObjectPropertyDefinition::Create( 
            FdoStringP::Format(L"Object Property %d", idx), 
            L"" 
        );

	    pObjProp->SetObjectType( FdoObjectType_Collection );
        pObjProp->SetIdentityProperty( pBaseOpId );
	    pObjProp->SetClass( pBaseOpClass );
	    FdoPropertiesP(pClass->GetProperties())->Add( pObjProp );

        FdoAssociationPropertyP pAssocProp = FdoAssociationPropertyDefinition::Create(
            FdoStringP::Format(L"Association Property %d", idx), 
            L"" 
        );

        pAssocProp->SetAssociatedClass( pBaseClass );
	    FdoPropertiesP(pClass->GetProperties())->Add( pAssocProp );

    }

	FdoClassesP(pSchema->GetClasses())->Add( pClass );

    /* The following tests cross-schema references for all references between
     * network type classes (including base class and associations).
     */

    // Create a network layer class
    FdoPtr<FdoNetworkLayerClass>layer = FdoNetworkLayerClass::Create(L"TestLayer",L"A network test Layer class");
    if ( pBaseSchema ) {
        layer->SetBaseClass( baseLayer );
    }
    else {
        // Add property to the layer class
        pProp = FdoDataPropertyDefinition::Create( 
            FdoStringP::Format(L"LayerProperty%d", idx), 
                L"" 
        );
	    pProp->SetDataType( FdoDataType_String );
        pProp->SetLength(50);
	    pProp->SetNullable(false);
	    FdoPtr<FdoPropertyDefinitionCollection>(layer->GetProperties())->Add( pProp );
	    FdoPtr<FdoDataPropertyDefinitionCollection>(layer->GetIdentityProperties())->Add( pProp );
    }
	FdoClassesP(pSchema->GetClasses())->Add( layer );

    // Create a network class
    FdoPtr<FdoNetworkClass>network = FdoNetworkClass::Create(L"TestNetwork",L"A network test class");
    if ( pBaseSchema ) {
        network->SetBaseClass( baseNetwork );
        network->SetLayerClass( baseLayer );
    }
    else {
        // Add property to the layer class
        pProp = FdoDataPropertyDefinition::Create( 
            FdoStringP::Format(L"NetworkProperty%d", idx), 
                L"" 
        );
	    pProp->SetDataType( FdoDataType_String );
        pProp->SetLength(100);
	    pProp->SetNullable(false);
	    FdoPtr<FdoPropertyDefinitionCollection>(network->GetProperties())->Add( pProp );
	    FdoPtr<FdoDataPropertyDefinitionCollection>(network->GetIdentityProperties())->Add( pProp );
    }
	FdoClassesP(pSchema->GetClasses())->Add( network );

    FdoPtr<FdoAssociationPropertyDefinition> layerAssoc;
    FdoPtr<FdoAssociationPropertyDefinition> networkAssoc;
    // Create a parent network node class
    FdoPtr<FdoNetworkNodeFeatureClass>parentNetworkNode = FdoNetworkNodeFeatureClass::Create(L"TestNetworkParentNode",L"A parent network node test class");
    if ( pBaseSchema ) {
        parentNetworkNode->SetBaseClass(baseParentNetworkNode);
        // Set the layer, network and cost properties
        layerAssoc = FdoAssociationPropertyDefinition::Create(L"Layer Association Prop", L"The layer association prop");
        layerAssoc->SetAssociatedClass(baseLayer);
        networkAssoc = FdoAssociationPropertyDefinition::Create(L"Network Association Prop", L"The network association prop");
        networkAssoc->SetAssociatedClass(baseNetwork);
        parentNetworkNode->SetLayerProperty( layerAssoc );
        parentNetworkNode->SetNetworkProperty( networkAssoc );
    }
	FdoClassesP(pSchema->GetClasses())->Add( parentNetworkNode );

    // Create a network node class
    FdoPtr<FdoNetworkNodeFeatureClass>networkNode = FdoNetworkNodeFeatureClass::Create(L"TestNetworkNode",L"A network node test class");
    if ( pBaseSchema ) {
        networkNode->SetBaseClass( baseNetworkNode );
        // Set the layer, network 
        layerAssoc = FdoAssociationPropertyDefinition::Create(L"Layer Association Prop", L"The layer association prop");
        layerAssoc->SetAssociatedClass(baseLayer);
        networkAssoc = FdoAssociationPropertyDefinition::Create(L"Network Association Prop", L"The network association prop");
        networkAssoc->SetAssociatedClass(baseNetwork);
        networkNode->SetLayerProperty( layerAssoc );
        networkNode->SetNetworkProperty( networkAssoc );
        FdoPtr<FdoAssociationPropertyDefinition> parentAssoc = FdoAssociationPropertyDefinition::Create(L"Parent Association Prop", L"The parent association prop");
        parentAssoc->SetAssociatedClass(baseParentNetworkNode);
        networkNode->SetParentNetworkFeatureProperty( parentAssoc );
        FdoPtr<FdoAssociationPropertyDefinition> nodeFeatAssoc = FdoAssociationPropertyDefinition::Create(L"Feature Association Prop", L"The feature association prop");
        nodeFeatAssoc->SetAssociatedClass(pBaseClass);
        networkNode->SetReferencedFeatureProperty(nodeFeatAssoc);
    }
    else {
    // Add property to the layer class
        pProp = FdoDataPropertyDefinition::Create( 
            FdoStringP::Format(L"NodeProperty%d", idx), 
                L"" 
        );
    	pProp->SetDataType( FdoDataType_Double );
	    pProp->SetNullable(false);
	    FdoPtr<FdoPropertyDefinitionCollection>(networkNode->GetProperties())->Add( pProp );
	    FdoPtr<FdoDataPropertyDefinitionCollection>(networkNode->GetIdentityProperties())->Add( pProp );
    }
	FdoClassesP(pSchema->GetClasses())->Add( networkNode );

    // Create a network link class
    FdoPtr<FdoNetworkLinkFeatureClass>networkLink = FdoNetworkLinkFeatureClass::Create(L"TestNetworkLink",L"A network link test class");
    if ( pBaseSchema ) {
        networkLink->SetBaseClass(baseNetworkLink);
        // Set the network and cost properties
        networkLink->SetNetworkProperty( networkAssoc );
        // Set the start and end nodes
        FdoPtr<FdoAssociationPropertyDefinition> nodeAssoc = FdoAssociationPropertyDefinition::Create(L"Network node Association Prop", L"The network node association prop");
        nodeAssoc->SetAssociatedClass(baseNetworkNode);
        networkLink->SetStartNodeProperty( nodeAssoc );
        networkLink->SetEndNodeProperty( nodeAssoc );
        FdoPtr<FdoAssociationPropertyDefinition> linkFeatAssoc = FdoAssociationPropertyDefinition::Create(L"Feature Association Prop", L"The feature association prop");
        linkFeatAssoc->SetAssociatedClass(pBaseClass);
        networkLink->SetReferencedFeatureProperty(linkFeatAssoc);
    }
    else {
    // Add property to the layer class
        pProp = FdoDataPropertyDefinition::Create( 
            FdoStringP::Format(L"LinkProperty%d", idx), 
                L"" 
        );
    	pProp->SetDataType( FdoDataType_Double );
	    pProp->SetNullable(false);
	    FdoPtr<FdoPropertyDefinitionCollection>(networkLink->GetProperties())->Add( pProp );
	    FdoPtr<FdoDataPropertyDefinitionCollection>(networkLink->GetIdentityProperties())->Add( pProp );
    }
	FdoClassesP(pSchema->GetClasses())->Add( networkLink );

    return( pSchema );
}

