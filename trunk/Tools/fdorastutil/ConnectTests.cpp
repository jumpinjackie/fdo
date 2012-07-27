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

#include <malloc.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <dlfcn.h>

#include <Fdo.h>
#include <FdoCommonOSUtil.h>
#include <FdoCommonStringUtil.h>
#include <FdoCommonMiscUtil.h>
#include <FdoCommonFile.h>

int main()

{
/* -------------------------------------------------------------------- */
/*      Setup connection                                                */
/* -------------------------------------------------------------------- */
    GisPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
    GisPtr<FdoIConnection> conn;

    conn = manager->CreateConnection (L"Autodesk.Shp.1.0");

    printf( "conn = %p\n", (void *) conn );

/* -------------------------------------------------------------------- */
/*      Open specific shapefile.                                        */
/* -------------------------------------------------------------------- */
    conn->SetConnectionString (L"DefaultFileLocation=polygon.shp");

    FdoConnectionState state = conn->Open ();

    if( state != FdoConnectionState_Open )
    {
        printf( "State = %d, not open.\n", (int) state );
        conn->Close();
        exit( 1 );
    }

/* -------------------------------------------------------------------- */
/*      Describe layer.                                                 */
/* -------------------------------------------------------------------- */
    try
    {
        GisPtr<FdoIDescribeSchema> describe = 
            (FdoIDescribeSchema*)conn->CreateCommand(
                FdoCommandType_DescribeSchema);
        GisPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();

        FdoFeatureSchema* schema = schemas->GetItem (0);

        printf ("Schema: %ls\n", schema->GetName());

        if ((schema->GetDescription () != NULL) 
            && (0 != wcscmp (schema->GetDescription (), L"")))
            printf ("    Description: %ls\n", schema->GetDescription ());

        FdoClassCollection* classes = schema->GetClasses ();
        for (int j = 0; j < classes->GetCount (); j++)
        {
            FdoClassDefinition* cls = classes->GetItem (j);

            // Output basic class info:
            if (FdoClassType_FeatureClass == cls->GetClassType ())
                printf ("    Feature Class: %ls\n", cls->GetName ());
            else
                printf ("    Class: %ls\n", cls->GetName ());
            if ((cls->GetDescription () != NULL) && (0 != wcscmp (cls->GetDescription (), L"")))
                printf ("        Description: %ls\n", cls->GetDescription ());

            GisPtr<FdoClassCapabilities> classCapabilities = cls->GetCapabilities();
            printf ("        Class Capabilities:\n");
            if (classCapabilities == NULL)
                printf ("            (Not available).\n");
            else
            {
                printf ("            Supports locking: %s\n", classCapabilities->SupportsLocking() ? "yes" : "no");
                printf ("            Supports long transactions: %s\n", classCapabilities->SupportsLongTransactions() ? "yes" : "no");
            }


            // Output identity properties:
            FdoDataPropertyDefinitionCollection* identity = cls->GetIdentityProperties ();
            for (int k = 0; k < identity->GetCount (); k++)
            {
                FdoDataPropertyDefinition* definition = identity->GetItem (k);
                printf ("        Id: %ls\n", definition->GetName ());
                if ((definition->GetDescription () != NULL) && (0 != wcscmp (definition->GetDescription (), L"")))
                    printf ("            Description: %ls\n", definition->GetDescription ());
                printf ("            Type: %d Length: %d Precision: %d %ls\n",
                        (int) definition->GetDataType (),
                        definition->GetLength (),
                        definition->GetPrecision (),
                        definition->GetNullable () ? L"Nullable" : L"NotNull");
                definition->Release ();
            }

            // Output regular properties:
            FdoPropertyDefinitionCollection* properties = cls->GetProperties ();
            for (int k = 0; k < properties->GetCount (); k++)
            {
                FdoPropertyDefinition* definition = properties->GetItem (k);
                if (definition->GetPropertyType () == FdoPropertyType_DataProperty)
                {
                    FdoDataPropertyDefinition* data_definition = (FdoDataPropertyDefinition*)definition;
                    if (!identity->Contains (data_definition))
                    {
                        printf ("        Property: %ls\n", definition->GetName ());
                        if ((data_definition->GetDescription () != NULL) && (0 != wcscmp (data_definition->GetDescription (), L"")))
                            printf ("            Description: %ls\n", data_definition->GetDescription ());
                        printf ("            Type: %d Length: %d Precision: %d %ls\n",
                                (int) data_definition->GetDataType (),
                                data_definition->GetLength (),
                                data_definition->GetPrecision (),
                                data_definition->GetNullable () ? L"Nullable" : L"NotNull");
                    }
                }
                else if (definition->GetPropertyType () == FdoPropertyType_ObjectProperty)
                {
                    printf ("       Object Property: %ls\n", definition->GetName ());
                    if ((definition->GetDescription () != NULL) && (0 != wcscmp (definition->GetDescription (), L"")))
                        printf ("            Description: %ls\n", definition->GetDescription ());
                }
                else if (definition->GetPropertyType () == FdoPropertyType_GeometricProperty)
                {
                    FdoGeometricPropertyDefinition* geometry_definition = (FdoGeometricPropertyDefinition*)definition;
                    printf ("        Geometric Property: %ls\n", geometry_definition->GetName ());
                    if ((geometry_definition->GetDescription () != NULL) && (0 != wcscmp (geometry_definition->GetDescription (), L"")))
                        printf ("            Description: %ls\n", geometry_definition->GetDescription ());
                    int types = geometry_definition->GetGeometryTypes ();
                    if (0 != (types & FdoGeometricType_Point))
                        printf ("            FdoGeometricType_Point types allowed\n");
                    if (0 != (types & FdoGeometricType_Curve))
                        printf ("            FdoGeometricType_Curve types allowed\n");
                    if (0 != (types & FdoGeometricType_Surface))
                        printf ("            FdoGeometricType_Surface types allowed\n");
                    if (0 != (types & FdoGeometricType_Solid))
                        printf ("            FdoGeometricType_Solid types allowed\n");
                }
                definition->Release ();
            }
            identity->Release ();
            properties->Release ();

            // Output schema attribute dictionary:
            FdoSchemaAttributeDictionary* dictionary = cls->GetAttributes ();
            int count;
            const wchar_t **names = dictionary->GetAttributeNames (count);
            if ((0 != count) && (NULL != names))
            {
                printf ("        MetaData:");
                const wchar_t *name = *names;
                for (int i = 0; i < count; i++)
                {
                    if (0 != i)
                        printf (",");
                    const wchar_t* value = dictionary->GetAttributeValue (name);
                    printf (" %ls=%ls", name, value);
                    name++;
                }
                printf ("\n");
            }
            dictionary->Release ();

            cls->Release ();
        }
        classes->Release ();
        schema->Release ();
    }
    catch (GisException* ge) 
    {
        printf( "Trapped exception: %ls\n", ge->GetExceptionMessage() );
    }

/* -------------------------------------------------------------------- */
/*      Setup query.                                                    */
/* -------------------------------------------------------------------- */
    try
    {
        GisPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"polygon");
        GisPtr<FdoIFeatureReader> reader = select->Execute ();
    
        while (reader->ReadNext ())
        {
            printf( "Feature:\n" );
            printf( "  AREA = %.15g\n", reader->GetDouble( L"AREA" ) );
            printf( "  LIFESTYLES = %g\n", reader->GetDouble( L"LIFESTYLES" ) );
            printf( "  PRFEDEA = %ls\n", reader->GetString( L"PRFEDEA" ) );
        

//        GisPtr<GisByteArray> geometry = reader->GetGeometry (L"Geometry");

//        AnalyzeGeometry( L"ontario", count, geometry, length, area);
        }
    }
    catch (GisException* ge) 
    {
        printf( "Trapped exception: %ls\n", ge->GetExceptionMessage() );
    }


    conn->Close();
}

