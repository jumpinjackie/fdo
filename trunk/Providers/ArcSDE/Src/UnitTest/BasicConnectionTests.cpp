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
#include "BasicConnectionTests.h"

CPPUNIT_TEST_SUITE_REGISTRATION (BasicConnectionTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (BasicConnectionTests, "BasicConnectionTests");

BasicConnectionTests::BasicConnectionTests (void)
{
    mConnection = ArcSDETests::GetConnection ();
}

BasicConnectionTests::~BasicConnectionTests (void)
{
    mConnection->Release ();
}

void BasicConnectionTests::setUp ()
{
    mConnection->SetConnectionString (ArcSDETestConfig::ConnStringOzzie());
}

void BasicConnectionTests::tearDown ()
{
}

void BasicConnectionTests::info ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        FdoIConnectionInfo* info;
        
        info = mConnection->GetConnectionInfo ();
        printf ("Name: %ls\n", info->GetProviderName ());
        printf ("Description: %ls\n", info->GetProviderDescription ());
        printf ("Version: %ls\n", info->GetProviderVersion ());
        printf ("FDO: %ls\n", info->GetFeatureDataObjectsVersion ());
        info->Release ();
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}

void BasicConnectionTests::get_properties ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        FdoIConnectionInfo* info;
        FdoIConnectionPropertyDictionary *dictionary;
        int count;
        const wchar_t** strings;
        int number;
        const wchar_t** values;
        int flags;

        mConnection->SetConnectionString (L"Server=vulcan;Instance=enterprise;Datastore=Default Datastore;Username=spock;Password=logical");
        info = mConnection->GetConnectionInfo ();
        dictionary = info->GetConnectionProperties ();
        strings = dictionary->GetPropertyNames (count);
        flags = 0;
        for (int i = 0; i < count; i++)
        {
            printf ("%ls=\"%ls\"", strings[i], dictionary->GetProperty (strings[i]));
            printf (" Default: \"%ls\"", dictionary->GetPropertyDefault (strings[i]));
            printf (dictionary->IsPropertyRequired (strings[i]) ? " required" : " optional");
            printf (dictionary->IsPropertyProtected (strings[i]) ? " protected" : " non-protected");
            printf (" (LOCALIZED NAME='%ls')", dictionary->GetLocalizedName (strings[i]));

            if (dictionary->IsPropertyEnumerable (strings[i]))
            {
                values = dictionary->EnumeratePropertyValues (strings[i], number);
                printf ("[");
                for (int i = 0; i < number; i++)
                {
                    if (0 != i)
                        printf (", ");
                    printf ("\"%ls\"", values[i]);
                }
                printf ("]");
            }
            printf ("\n");
            
            if (0 == wcscmp (strings[i], L"Server"))
                CPPUNIT_ASSERT_MESSAGE ("server wrong", 0 == wcscmp (L"vulcan", dictionary->GetProperty (strings[i])));
            else if (0 == wcscmp (strings[i], L"Instance"))
            {
                CPPUNIT_ASSERT_MESSAGE ("instance wrong", 0 == wcscmp (L"enterprise", dictionary->GetProperty (strings[i])));
                flags |= 1;
            }
            else if (0 == wcscmp (strings[i], L"Username"))
            {
                CPPUNIT_ASSERT_MESSAGE ("user name wrong", 0 == wcscmp (L"spock", dictionary->GetProperty (strings[i])));
                flags |= 2;
            }
            else if (0 == wcscmp (strings[i], L"Password"))
            {
                CPPUNIT_ASSERT_MESSAGE ("password wrong", 0 == wcscmp (L"logical", dictionary->GetProperty (strings[i])));
                flags |= 4;
            }
            else if (0 == wcscmp (strings[i], L"Datastore"))
            {
                CPPUNIT_ASSERT_MESSAGE ("data store wrong", 0 == wcscmp (L"Default Datastore", dictionary->GetProperty (strings[i])));
                CPPUNIT_ASSERT_MESSAGE ("datastore property found before required one", 7 == flags);
            }

        }
        dictionary->Release ();
        info->Release ();
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}

void BasicConnectionTests::set_properties ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        FdoIConnectionInfo* info;
        FdoIConnectionPropertyDictionary *dictionary;
        const wchar_t* connection_string;
        
        info = mConnection->GetConnectionInfo ();
        dictionary = info->GetConnectionProperties ();
        dictionary->SetProperty (L"Server", L"dauntless");
        dictionary->SetProperty (L"Datastore", L"myDatastore");
        dictionary->SetProperty (L"Instance", L"gis");
        dictionary->SetProperty (L"Username", L"poindexter");
        dictionary->SetProperty (L"Password", L"secret");
        connection_string = mConnection->GetConnectionString ();
        CPPUNIT_ASSERT_MESSAGE ("connection string wrong", 0 == wcscmp (L"Server=dauntless;Instance=gis;Username=poindexter;Password=secret;Datastore=myDatastore;", connection_string));
        dictionary->Release ();
        info->Release ();
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}

void BasicConnectionTests::connection_capabilities ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        FdoIConnectionCapabilities* connection_capabilities;
        FdoThreadCapability threads;
        FdoSpatialContextExtentType* contexts;
        wchar_t* s;
        int length;

        // display the connection capabilities
        connection_capabilities = mConnection->GetConnectionCapabilities ();
        threads = connection_capabilities->GetThreadCapability();
        switch (threads)
        {
            case FdoThreadCapability_SingleThreaded:
                s = L"The provider is not thread safe.";
                break;
            case FdoThreadCapability_PerConnectionThreaded:
                s = L"The provider supports a single thread per connection.";
                break;
            case FdoThreadCapability_PerCommandThreaded:
                s = L"The provider supports a single thread per command.";
                break;
            case FdoThreadCapability_MultiThreaded:
                s = L"The provider is fully multi-threaded.";
                break;
            default:
                s = L"Unknown thread capability type.";
                break;
        }
        wprintf (L"    %ls\n", s);
        contexts = connection_capabilities->GetSpatialContextTypes (length);
        for (int i = 0; i < length; i++)
        {
            switch (*contexts)
            {
                case FdoSpatialContextExtentType_Static:
                    s = L"static";
                    break;
                case FdoSpatialContextExtentType_Dynamic:
                    s = L"dynamic";
                    break;
                default:
                    s = L"Unknown spatial context type.";
                    break;
            }
            contexts++;
            wprintf (L"    The provider spatial context extent is %ls.\n", s);
        }
        wprintf (L"    The provider does %lssupport locking.\n", (connection_capabilities->SupportsLocking () ? L"" : L"not "));
        wprintf (L"    The provider does %lssupport connection timeout.\n", (connection_capabilities->SupportsTimeout () ? L"" : L"not "));
        wprintf (L"    The provider does %lssupport transactions.\n", (connection_capabilities->SupportsTransactions () ? L"" : L"not "));
        wprintf (L"    The provider does %lssupport long transactions.\n", (connection_capabilities->SupportsLongTransactions () ? L"" : L"not "));
        wprintf (L"    The provider does %lssupport SQL commands.\n", (connection_capabilities->SupportsSQL () ? L"" : L"not "));
        connection_capabilities->Release ();
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}


void BasicConnectionTests::schema_capabilities ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        FdoISchemaCapabilities* schema_capabilities;
        FdoClassType* class_types;
        int length;
        wchar_t* s;
        FdoDataType* data_types;

        schema_capabilities = mConnection->GetSchemaCapabilities ();
        class_types = schema_capabilities->GetClassTypes (length);
        for (int i = 0; i < length; i++)
        {
            switch (*class_types)
            {
                case FdoClassType_Class: // Represents the concrete class type Class.
                    s = L"FdoClassType_Class";
                    break;
                case FdoClassType_FeatureClass: // Represents the concrete class type FeatureClass.
                    s = L"FdoClassType_FeatureClass";
                    break;
            }
            class_types++;
            wprintf (L"    The provider schema class types include %ls.\n", s);
        }

        data_types = schema_capabilities->GetDataTypes (length);
        for (int i = 0; i < length; i++)
        {
            wprintf (L"    The provider data types include %ls.\n", ArcSDETests::GetDataTypeString (*data_types));
            data_types++;
        }
        wprintf (L"    The provider does %lssupport inheritance.\n", (schema_capabilities->SupportsInheritance () ? L"" : L"not "));
        wprintf (L"    The provider does %lssupport multiple schemas.\n", (schema_capabilities->SupportsMultipleSchemas () ? L"" : L"not "));
        wprintf (L"    The provider does %lssupport object properties.\n", (schema_capabilities->SupportsObjectProperties () ? L"" : L"not "));
        schema_capabilities->Release ();
    }
    catch (FdoException *e)
    {
        fail (e);
    }
}


void BasicConnectionTests::command_capabilities ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        FdoICommandCapabilities* command_capabilities;
        int length;
        int* commands;

        command_capabilities = mConnection->GetCommandCapabilities ();
        commands = command_capabilities->GetCommands (length);
        for (int i = 0; i < length; i++)
        {
            wprintf (L"    The provider supports command %ls.\n", (FdoString *)FdoCommonMiscUtil::FdoCommandTypeToString(*commands));
            commands++;
        }
        wprintf (L"    The provider commands do %lssupport parameterization.\n", (command_capabilities->SupportsParameters () ? L"" : L"not "));
        wprintf (L"    The provider commands do %lssupport timeout.\n", (command_capabilities->SupportsTimeout () ? L"" : L"not "));
        command_capabilities->Release ();
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}

void BasicConnectionTests::filter_capabilities ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        FdoIFilterCapabilities* filter_capabilities;
        int length;
        wchar_t* s;
        FdoConditionType* condition_types;
        FdoSpatialOperations* spatial_operations;
        FdoDistanceOperations* distance_operations;

        filter_capabilities = mConnection->GetFilterCapabilities ();
        condition_types = filter_capabilities->GetConditionTypes (length);
        for (int i = 0; i < length; i++)
        {
            switch (*condition_types)
            {
                case FdoConditionType_Comparison: // Tests if one expression is equal, not equal, greater than, less than, greater than or equal to, or less than or equal to another expression.
                    s = L"FdoConditionType_Comparison";
                    break;
                case FdoConditionType_Like: // Tests if the value of a specified data property is matches a specified pattern.
                    s = L"FdoConditionType_Like";
                    break;
                case FdoConditionType_In: // Tests if the value of a specified data property is within a given set of literal values.
                    s = L"FdoConditionType_In";
                    break;
                case FdoConditionType_Null: // Tests if the value of a specified data property is null.
                    s = L"FdoConditionType_Null";
                    break;
                case FdoConditionType_Spatial: // Tests whether the value of a geometric property and a literal geometric value satisfy the spatial relationship implied by the operation.
                    s = L"FdoConditionType_Spatial";
                    break;
                case FdoConditionType_Distance: // Tests whether the value of a geometric property is within or beyond a specified distance of a literal geometric value.
                    s = L"FdoConditionType_Distance";
                    break;
                default:
                    s = L"Unknown condition type.";
                    break;
            }
            condition_types++;
            wprintf (L"    The provider filter conditions include %ls.\n", s);
        }
        spatial_operations = filter_capabilities->GetSpatialOperations (length);
        for (int i = 0; i < length; i++)
        {
            switch (*spatial_operations)
            {
                case FdoSpatialOperations_Contains: // Test to see if the geometric property value spatially contains the literal geometric value.
                    s = L"FdoSpatialOperations_Contains";
                    break;
                case FdoSpatialOperations_Crosses: // Test to see if the geometric property value spatially crosses the given geometry.
                    s = L"FdoSpatialOperations_Crosses";
                    break;
                case FdoSpatialOperations_Disjoint: // Test to see if the geometric property value spatially is spatially disjoint from the given geometry.
                    s = L"FdoSpatialOperations_Disjoint";
                    break;
                case FdoSpatialOperations_Equals: // Test to see if the geometric property value is spatially equal to the given geometry.
                    s = L"FdoSpatialOperations_Equals";
                    break;
                case FdoSpatialOperations_Intersects: // Test to see if the geometric property value spatially intersects the given geometry.
                    s = L"FdoSpatialOperations_Intersects";
                    break;
                case FdoSpatialOperations_Overlaps: // Test to see if the geometric property value spatially overlaps the given geometry.
                    s = L"FdoSpatialOperations_Overlaps";
                    break;
                case FdoSpatialOperations_Touches: // Test to see if the geometric property value spatially touches the given geometry.
                    s = L"FdoSpatialOperations_Touches";
                    break;
                case FdoSpatialOperations_Within: // Test to see if the geometric property value is spatially within the given geometry.
                    s = L"FdoSpatialOperations_Within";
                    break;
                case FdoSpatialOperations_CoveredBy: // Test to see if the geometric property value is covered by the interior or boundary of the given geometry.
                    s = L"FdoSpatialOperations_CoveredBy";
                    break;
                case FdoSpatialOperations_EnvelopeIntersects: // Test to see if the geometric property value's envelope overlaps the given geometry.
                    s = L"FdoSpatialOperations_EnvelopeIntersects";
                    break;
                default:
                    s = L"Unknown spatial operation";
                    break;
            }
            spatial_operations++;
            wprintf (L"    The provider spatial operations include %ls.\n", s);
        }

        distance_operations = filter_capabilities->GetDistanceOperations (length);
        for (int i = 0; i < length; i++)
        {
            switch (*distance_operations)
            {
                case FdoDistanceOperations_Beyond: // Test to see if the geometric property value lies beyond a specified distance of a literal geometric value.
                    s = L"FdoDistanceOperations_Beyond";
                    break;
                case FdoDistanceOperations_Within: // Test to see if the geometric property value lies within a specified distance of a literal geometric value.
                    s = L"FdoDistanceOperations_Within";
                    break;
                default:
                    s = L"Unknown distance operation.";
                    break;
            }
            distance_operations++;
            wprintf (L"    The provider distance operations include %ls.\n", s);
        }

        wprintf (L"    The provider does %lssupport geodesic distance measurement.\n", (filter_capabilities->SupportsGeodesicDistance () ? L"" : L"not "));
        wprintf (L"    The provider does %lssupport inter-property spatial operations.\n", (filter_capabilities->SupportsNonLiteralGeometricOperations () ? L"" : L"not "));
        filter_capabilities->Release ();
    }
    catch (FdoException *e)
    {
        fail (e);
    }
}

void BasicConnectionTests::expression_capabilities ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        int length;
        wchar_t* s;
        FdoIExpressionCapabilities* expression_capabilities;
        FdoExpressionType* expression_types;
        FdoFunctionDefinitionCollection* definitions;
        FdoReadOnlyArgumentDefinitionCollection* args;
        FdoArgumentDefinition* arg;

        expression_capabilities = mConnection->GetExpressionCapabilities ();
        expression_types = expression_capabilities->GetExpressionTypes (length);
        for (int i = 0; i < length; i++)
        {
            switch (*expression_types)
            {
                case FdoExpressionType_Basic:
                    s = L"basic arithmetic expressions";
                    break;
                case FdoExpressionType_Function:
                    s = L"function evaluations";
                    break;
                case FdoExpressionType_Parameter:
                    s = L"parameter substitutions";
                    break;
                default:
                    s = L"Unknown expression type";
                    break;
            }
            expression_types++;
            wprintf (L"    The provider expressions support %ls.\n", s);
        }
        definitions = expression_capabilities->GetFunctions();
        if (0 != definitions->GetCount())
            wprintf (L"    Functions:\n", s);
        for (int i = 0; i < definitions->GetCount(); i++)
        {
            FdoFunctionDefinition*  definition;

            definition = definitions->GetItem(i);
            wprintf (L"        // %ls\n", definition->GetDescription ());
            wprintf (L"        %ls ", ArcSDETests::GetDataTypeString (definition->GetReturnType ()));
            wprintf (L" %ls(", definition->GetName ());
            args = definition->GetArguments ();
            for (int j = 0; j < args->GetCount (); j++)
            {
                arg = args->GetItem (j);
                wprintf (L"%ls %ls  /* %ls */", ArcSDETests::GetDataTypeString(arg->GetDataType ()), arg->GetName(), arg->GetDescription());
                arg->Release ();
            }
            wprintf (L");\n");
            args->Release ();
            definition->Release();
        }
        definitions->Release();
        expression_capabilities->Release ();
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}


void BasicConnectionTests::geometry_capabilities ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        FdoPtr<FdoIGeometryCapabilities> geometry_capabilities = mConnection->GetGeometryCapabilities ();

        // output geometry dimensionalities supported:
        FdoInt32 geomDimensionalities = geometry_capabilities->GetDimensionalities();
        wprintf(L"    Geometry dimensionalities supported:\n");
        if ((geomDimensionalities | FdoDimensionality_XY) > 0)
            wprintf(L"        XY\n");
        if ((geomDimensionalities | FdoDimensionality_Z) > 0)
            wprintf(L"        Z\n");
        if ((geomDimensionalities | FdoDimensionality_M) > 0)
            wprintf(L"        M\n");

        // output geometry component types supported:
        FdoInt32 length = 0;
        FdoGeometryComponentType *geomComponentTypes = geometry_capabilities->GetGeometryComponentTypes(length);
        wprintf(L"    Geometry components types supported:\n");
        for (int i=0; i<length; i++)
        {
            switch (geomComponentTypes[i])
            {
            case FdoGeometryComponentType_LinearRing:
                wprintf(L"        FdoGeometryComponentType_LinearRing\n");
            break;
            case FdoGeometryComponentType_CircularArcSegment:
                wprintf(L"        FdoGeometryComponentType_CircularArcSegment\n");
            break;
            case FdoGeometryComponentType_LineStringSegment:
                wprintf(L"        FdoGeometryComponentType_LineStringSegment\n");
            break;
            case FdoGeometryComponentType_Ring:
                wprintf(L"        FdoGeometryComponentType_Ring\n");
            break;
            default:
                wprintf(L"        <unknown geometry component type>\n");
            break;
            }
        }


        // output geometry types supported:
        length = 0;
        FdoGeometryType *geomTypes = geometry_capabilities->GetGeometryTypes(length);
        wprintf(L"    Geometry types supported:\n");
        for (int i=0; i<length; i++)
        {
            switch (geomTypes[i])
            {
            case FdoGeometryType_Point:
                wprintf(L"        FdoGeometryType_Point\n");
            break;
            case FdoGeometryType_LineString:
                wprintf(L"        FdoGeometryType_LineString\n");
            break;
            case FdoGeometryType_Polygon:
                wprintf(L"        FdoGeometryType_Polygon\n");
            break;
            case FdoGeometryType_MultiPoint:
                wprintf(L"        FdoGeometryType_MultiPoint\n");
            break;
            case FdoGeometryType_MultiLineString:
                wprintf(L"        FdoGeometryType_MultiLineString\n");
            break;
            case FdoGeometryType_MultiPolygon:
                wprintf(L"        FdoGeometryType_MultiPolygon\n");
            break;
            case FdoGeometryType_MultiGeometry:
                wprintf(L"        FdoGeometryType_MultiGeometry\n");
            break;
            default:
                wprintf(L"        <unknown geometry type>\n");
            break;
            }
        }
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}
