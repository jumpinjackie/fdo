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
#include "WmsTestDescribeSchema.h"

CPPUNIT_TEST_SUITE_REGISTRATION( WmsTestDescribeSchema );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( WmsTestDescribeSchema, "WmsTestDescribeSchema");

WmsTestDescribeSchema::WmsTestDescribeSchema(void)
{
}

WmsTestDescribeSchema::~WmsTestDescribeSchema(void)
{
}

void WmsTestDescribeSchema::testServer1 ()
{
    try 
    {
	    TestServer(L"http://CADCISDJIAB.ads.autodesk.com/cgi-bin/mapserv.exe?map=wms/wms.map&");
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

void WmsTestDescribeSchema::testInheritance1 ()
{
    try 
    {
	    TestInheritance(L"http://CADCISDJIAB.ads.autodesk.com/cgi-bin/mapserv.exe?map=wms/wms.map&");
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

void WmsTestDescribeSchema::testServer2 ()
{
    try 
    {
	    TestServer(L"http://wms.jpl.nasa.gov/wms.cgi");
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

void WmsTestDescribeSchema::testServer3 ()
{
    try 
    {
	    TestServer(L"http://globe.digitalearth.gov/viz-bin/wmt.cgi");
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

void WmsTestDescribeSchema::testServer4 ()
{
    try 
    {
        FdoPtr<FdoIConnection> conn = this->GetConnection ();
        FdoStringP connString = FdoStringP(L"FeatureServer=http://fbinter.stadt-berlin.de/fb/wms/oma_ogc_capabilitiesrequest.jsp");
        conn->SetConnectionString (connString);
        CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

        FdoPtr<FdoIDescribeSchema> cmdDS = static_cast<FdoIDescribeSchema *> (conn->CreateCommand (FdoCommandType_DescribeSchema));
        FdoPtr<FdoFeatureSchemaCollection> schemas = cmdDS->Execute ();
        FdoInt32 cntSchemas = schemas->GetCount ();
        CPPUNIT_ASSERT (cntSchemas == 1);

        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem (0);

        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();
        FdoInt32 cntClasses = classes->GetCount ();

        for (FdoInt32 j=0; j<cntClasses; j++)
        {
            FdoPtr<FdoClassDefinition> clsDef = classes->GetItem (j);
            FdoStringP clsName = clsDef->GetName ();
            FdoFeatureClass* featClsDef = static_cast<FdoFeatureClass *> (clsDef.p);
            CPPUNIT_ASSERT (featClsDef != NULL);
            CPPUNIT_ASSERT (featClsDef->GetGeometryProperty() == NULL);
            FdoBoolean bAbstract = featClsDef->GetIsAbstract();

            FdoPtr<FdoPropertyDefinitionCollection> props = clsDef->GetProperties ();
            FdoInt32 cntProps = props->GetCount ();
            for (FdoInt32 k=0; k<cntProps; k++)
            {
                FdoPtr<FdoPropertyDefinition> prop = props->GetItem (k);
                FdoStringP propName = prop->GetName ();
                FdoPropertyType propType = prop->GetPropertyType ();
            }		
        }

		CPPUNIT_ASSERT (cntClasses == 24);
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

void WmsTestDescribeSchema::TestServer (FdoString* featureServer)
{
    FdoPtr<FdoIConnection> conn = this->GetConnection ();
    FdoStringP connString = FdoStringP(L"FeatureServer=") + featureServer;
    conn->SetConnectionString (connString);
    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

    FdoPtr<FdoIDescribeSchema> cmdDS = static_cast<FdoIDescribeSchema *> (conn->CreateCommand (FdoCommandType_DescribeSchema));
    FdoPtr<FdoFeatureSchemaCollection> schemas = cmdDS->Execute ();
    FdoInt32 cntSchemas = schemas->GetCount ();
    CPPUNIT_ASSERT (cntSchemas == 1);

    FdoPtr<FdoFeatureSchema> schema = schemas->GetItem (0);

#ifdef _DEBUG
    wprintf (L"Schema: %ls\n", schema->GetName ());
#endif

    FdoPtr<FdoClassCollection> classes = schema->GetClasses ();
    FdoInt32 cntClasses = classes->GetCount ();

    for (FdoInt32 j=0; j<cntClasses; j++)
    {
        FdoPtr<FdoClassDefinition> clsDef = classes->GetItem (j);
        FdoStringP clsName = clsDef->GetName ();
        FdoFeatureClass* featClsDef = static_cast<FdoFeatureClass *> (clsDef.p);
        CPPUNIT_ASSERT (featClsDef != NULL);
        CPPUNIT_ASSERT (featClsDef->GetGeometryProperty() == NULL);
        FdoBoolean bAbstract = featClsDef->GetIsAbstract();

#ifdef _DEBUG
        wprintf (L"  Class: %ls\n", (FdoString*)clsName);
        wprintf (L"    Abstract: %ls\n", bAbstract ? L"True" : L"False");
#endif
        FdoPtr<FdoPropertyDefinitionCollection> props = clsDef->GetProperties ();
        FdoInt32 cntProps = props->GetCount ();
        for (FdoInt32 k=0; k<cntProps; k++)
        {
            FdoPtr<FdoPropertyDefinition> prop = props->GetItem (k);
            FdoStringP propName = prop->GetName ();
            FdoPropertyType propType = prop->GetPropertyType ();
#ifdef _DEBUG
            wprintf (L"    Property: %ls\t", (FdoString*)propName);
            wprintf (L"    Type: %ls\n", PropertyTypeToString(propType));
#endif
        }		
    }
}

void WmsTestDescribeSchema::TestInheritance (FdoString* featureServer)
{
    FdoPtr<FdoIConnection> conn = this->GetConnection ();
	FdoStringP connString = FdoStringP(L"FeatureServer=") + featureServer;
    conn->SetConnectionString (connString);
	CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

	FdoPtr<FdoIDescribeSchema> cmdDS = static_cast<FdoIDescribeSchema *> (conn->CreateCommand (FdoCommandType_DescribeSchema));
	FdoPtr<FdoFeatureSchemaCollection> schemas = cmdDS->Execute ();
	FdoInt32 cntSchemas = schemas->GetCount ();
	CPPUNIT_ASSERT (cntSchemas == 1);

	FdoPtr<FdoFeatureSchema> schema = schemas->GetItem (0);
#ifdef _DEBUG
	wprintf (L"Schema: %ls\n", schema->GetName ());
#endif

	FdoPtr<FdoClassCollection> classes = schema->GetClasses ();
	FdoInt32 cntClasses = classes->GetCount ();
	CPPUNIT_ASSERT (cntClasses == 15);

	// test the inheritance realationship
	for (FdoInt32 j=0; j<cntClasses; j++)
	{
		FdoPtr<FdoClassDefinition> clsDef = classes->GetItem (j);
		FdoStringP clsName = clsDef->GetName ();
		FdoFeatureClass* featClsDef = static_cast<FdoFeatureClass *> (clsDef.p);
		CPPUNIT_ASSERT (featClsDef != NULL);
		CPPUNIT_ASSERT (featClsDef->GetGeometryProperty() == NULL);

		FdoFeatureClass* base = static_cast<FdoFeatureClass *> (featClsDef->GetBaseClass ());		

#ifdef _DEBUG		
		wprintf (L"  Class: %ls\n", (FdoString*)clsName);
		if (base != NULL)
            wprintf (L"  Base:  %ls\n", base->GetName ());
		else
			wprintf (L"  Base:  (null)\n");

#endif
		FdoPtr<FdoPropertyDefinitionCollection> props = clsDef->GetProperties ();
		FdoInt32 cntProps = props->GetCount ();
		for (FdoInt32 k=0; k<cntProps; k++)
		{
			FdoPtr<FdoPropertyDefinition> prop = props->GetItem (k);
			FdoStringP propName = prop->GetName ();
			FdoPropertyType propType = prop->GetPropertyType ();
#ifdef _DEBUG
			wprintf (L"    Property: %ls\t", (FdoString*)propName);
			wprintf (L"    Type: %ls\n", PropertyTypeToString(propType));
#endif
		}		
	}
}


FdoString* WmsTestDescribeSchema::PropertyTypeToString (FdoPropertyType propType)
{
	switch (propType)
	{
	case FdoPropertyType_DataProperty:
		return L"Data";
	case FdoPropertyType_RasterProperty:
		return L"Raster";
	case FdoPropertyType_GeometricProperty:
		return L"Geometry";
	}

	return NULL;
}

// test "FdoIDescribeSchemaMapping" command
void WmsTestDescribeSchema::testSchemaMapping ()
{
    try
    {
		FdoPtr<FdoIConnection> conn = this->GetConnection ();
		conn->SetConnectionString(L"FeatureServer=http://wms.jpl.nasa.gov/wms.cgi");
		FdoIoFileStreamP fileStream = FdoIoFileStream::Create(L"WmsSchemaConfig.xml", L"r");
		conn->SetConfiguration(fileStream);
		CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

		FdoPtr<FdoIDescribeSchemaMapping> cmd = static_cast<FdoIDescribeSchemaMapping *> (conn->CreateCommand (FdoCommandType_DescribeSchemaMapping));
		FdoPtr<FdoPhysicalSchemaMappingCollection> mappings = cmd->Execute ();

		for (FdoInt32 i=0; i<mappings->GetCount(); i++)
		{
			FdoPtr<FdoPhysicalSchemaMapping> mapping = mappings->GetItem (i);
			FdoString* mappingName = mapping->GetName ();
			FdoString* providerName = mapping->GetProvider ();
	#ifdef _DEBUG
			wprintf (L"Provider: %ls\nSchemaMapping: %ls\n", providerName, mappingName);
	#endif
		}
    }
    catch(FdoException* ex)
    {
        fail(ex);
    }
}

