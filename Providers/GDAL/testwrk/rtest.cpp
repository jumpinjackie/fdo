/*
 * (C) Copyright 2004 by Autodesk, Inc. All Rights Reserved.
 *
 * By using this code, you are agreeing to the terms and conditions of
 * the License Agreement included in the documentation for this code.
 *
 * AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
 * CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
 * IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
 * DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
 * DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
 *
 * Use, duplication, or disclosure by the U.S. Government is subject
 * to restrictions set forth in FAR 52.227-19 (Commercial Computer
 * Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
 * (Rights in Technical Data and Computer Software), as applicable.
 *
 * Revision Control Modification History
 *
 *         $Id: //providers_rubicon/Shp/src/UnitTest/ConnectTests.cpp#6 $
 *     $Author: derrick $
 *   $DateTime: 2005/08/25 16:57:09 $
 *     $Change: 8196 $
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
#include <Fdo/Raster/RasterDataModel.h>

#include <gdal.h>
#include <ogr_api.h>
#include <cpl_conv.h>
#include <cpl_string.h>

static void TranslateTo( FdoPtr<FdoIRaster> raster, 
                         const char *pszOutFilename );

/************************************************************************/
/*                                main()                                */
/************************************************************************/

int main( int argc, char ** argv )
{

/* -------------------------------------------------------------------- */
/*      Setup connection                                                */
/* -------------------------------------------------------------------- */
    FdoPtr<IConnectionManager> manager;
    manager = FdoFeatureAccessManager::GetConnectionManager();
    if (0 == manager)
    {
        printf("Connection manager is NULL\n");
        return 0;
    }

    FdoPtr<FdoIConnection> conn;
    conn = manager->CreateConnection (L"Autodesk.Gdal.3.0");
    if (0 == conn)
    {
        printf("Connection is NULL\n");
        return 0;
    }


    printf( "conn = %p\n", (void *) conn );

/* -------------------------------------------------------------------- */
/*      Setup connection configuration if we don't have a filename      */
/*      supplied on the commandline.                                    */
/* -------------------------------------------------------------------- */
    if( argc < 2 )
    {
        try
        {
            FdoStringP configFile(L"RfpConfigExample.xml");
            FdoStringP configMode(L"rb");
            FdoIoFileStreamP configStream = 
                FdoIoFileStream::Create( configFile, configMode );
        
            conn->SetConfiguration( configStream );
        }
        catch (FdoException* ge) 
        {
            printf( "Trapped exception: %ls\n", ge->GetExceptionMessage() );
        }
    }

/* -------------------------------------------------------------------- */
/*      Open specific shapefile.                                        */
/* -------------------------------------------------------------------- */
    try
    {
        if( argc == 2 )
        {
            FdoStringP defaultLocation;

            defaultLocation = L"DefaultRasterFileLocation=";
            defaultLocation += argv[1];
            conn->SetConnectionString (defaultLocation);
        }

        FdoConnectionState state = conn->Open ();

        if( state != FdoConnectionState_Open )
        {
            printf( "State = %d, not open.\n", (int) state );
            conn->Close();
            exit( 1 );
        }
    }
    catch (FdoException* ge) 
    {
        printf( "Trapped exception: %ls\n", ge->GetExceptionMessage() );
    }

/* -------------------------------------------------------------------- */
/*      Describe layer.                                                 */
/* -------------------------------------------------------------------- */
    try
    {
        FdoPtr<FdoIDescribeSchema> describe = 
            (FdoIDescribeSchema*)conn->CreateCommand(
                FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();

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

            FdoPtr<FdoClassCapabilities> classCapabilities = cls->GetCapabilities();
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
                    printf ("        Property: %ls\n", definition->GetName ());

                    if (!identity->Contains (data_definition))
                    {
                        if ((data_definition->GetDescription () != NULL) && (0 != wcscmp (data_definition->GetDescription (), L"")))
                            printf ("            Description: %ls\n", data_definition->GetDescription ());
                        printf ("            Type: %d Length: %d Precision: %d %ls\n",
                                (int) data_definition->GetDataType (),
                                data_definition->GetLength (),
                                data_definition->GetPrecision (),
                                data_definition->GetNullable () ? L"Nullable" : L"NotNull");
                    }
                    else
                        printf( "            (no data definition)\n" );
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
                else if (definition->GetPropertyType () == FdoPropertyType_RasterProperty)
                {
                    FdoRasterPropertyDefinition *r_definition = (FdoRasterPropertyDefinition*) definition;
                    FdoStringP srs;
                    FdoRasterDataModel *data_model;

                    printf ("        Raster: %ls\n", definition->GetName ());

                    srs = r_definition->GetSpatialContextAssociation();
                    if( srs != NULL )
                        printf( "          SRS = %p\n", (const char *) srs );
                    else
                        printf( "          SRS = NULL\n" );

                    data_model = r_definition->GetDefaultDataModel();
                    printf( "          data_model = %p\n", data_model );
                    if( data_model != NULL )
                    {
                        printf( "            ModelType = %d\n", 
                                (int) data_model->GetDataModelType() );
                        printf( "            BitsPerPixel = %d\n", 
                                data_model->GetBitsPerPixel() );
                        printf( "            Organization = %d\n", 
                                (int) data_model->GetOrganization() );
                        printf( "            Data Type = %d\n", 
                                (int) data_model->GetDataType() );
                    }

                    printf( "            SRS = %ls\n", 
                            r_definition->GetSpatialContextAssociation() );
                }
                else
                {
                    printf ("        Unsupported Property: %ls\n", definition->GetName ());
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
    catch (FdoException* ge) 
    {
        printf( "Trapped exception: %ls\n", ge->GetExceptionMessage() );
    }

/* -------------------------------------------------------------------- */
/*      Setup query.                                                    */
/* -------------------------------------------------------------------- */
    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand (FdoCommandType_Select);
//        select->SetFeatureClassName (L"Photo");
        select->SetFeatureClassName (L"default");
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        int iCounter = 0;
    
        while (reader->ReadNext ())
        {
            FdoPtr<FdoIRaster>  raster;

            printf( "Feature:\n" );

//            printf( "  Id = %ls\n", reader->GetString( L"Id" ) );
            printf( "  Id = %ls\n", reader->GetString( L"FeatId" ) );

//            raster = reader->GetRaster( L"Image" );
            raster = reader->GetRaster( L"Raster" );

            if( raster != NULL || !raster->IsNull() )
            {
                FdoPtr<FdoRasterDataModel> data_model;

                printf( "  Raster:\n" );
                printf( "    bands = %d, current = %d\n", 
                        raster->GetNumberOfBands(),
                        raster->GetCurrentBand() );

                printf( "    size = %dx%d\n", 
                        raster->GetImageXSize(), 
                        raster->GetImageYSize() );

                data_model = raster->GetDataModel();
                if( data_model != NULL )
                {
                    printf( "    ModelType = %d\n", 
                            (int) data_model->GetDataModelType() );
                    printf( "    BitsPerPixel = %d\n", 
                            data_model->GetBitsPerPixel() );
                    printf( "    Organization = %d\n", 
                            (int) data_model->GetOrganization() );
                    printf( "    Data Type = %d\n", 
                            (int) data_model->GetDataType() );
                }

                // Null (NODATA)
                FdoDataValue* nullVal = raster->GetNullPixelValue();
                if( nullVal != NULL && !nullVal->IsNull() )
                {
                    if( nullVal->GetDataType() == FdoDataType_Byte )
                    {
                        FdoPtr<FdoByteValue> val = static_cast<FdoByteValue*>(nullVal);
                        printf( "    NULL (Byte) = %d\n", val->GetByte() );
                    }
                    else if( nullVal->GetDataType() == FdoDataType_Int16 )
                    {
                        FdoPtr<FdoInt16Value> val = static_cast<FdoInt16Value*>(nullVal);
                        printf( "    NULL (Int16) = %d\n", val->GetInt16() );
                    }
                    else if( nullVal->GetDataType() == FdoDataType_Int32 )
                    {
                        FdoPtr<FdoInt32Value> val = static_cast<FdoInt32Value*>(nullVal);
                        printf( "    NULL (Int32) = %d\n", val->GetInt32() );
                    }
                    else if( nullVal->GetDataType() == FdoDataType_Single )
                    {
                        FdoPtr<FdoSingleValue> val = static_cast<FdoSingleValue*>(nullVal);
                        printf( "    NULL (float) = %g\n", val->GetSingle() );
                    }
                    else if( nullVal->GetDataType() == FdoDataType_Double )
                    {
                        FdoPtr<FdoDoubleValue> val = static_cast<FdoDoubleValue*>(nullVal);
                        printf( "    NULL (double) = %g\n", val->GetDouble() );
                    }
                }

                // Report boundary.
                FdoPtr<FdoFgfGeometryFactory> geomFactory = 
                    FdoFgfGeometryFactory::GetInstance();
                FdoPtr<FdoByteArray> ba = raster->GetBounds();
                FdoPtr<FdoIGeometry> geometry = geomFactory->CreateGeometryFromFgf(ba);
                FdoStringP wkt = geometry->GetText();

                printf( "    Bounds = %s\n", (const char *) wkt );

                try 
                {
                    FdoPtr<FdoIRasterPropertyDictionary> propDict = raster->GetAuxiliaryProperties();
                    FdoPtr<FdoDataValue> pal = propDict->GetProperty(L"Palette");
                    FdoLOBValue* palLOB = static_cast<FdoLOBValue*>(pal.p);
                    FdoByteArray *palArray = palLOB->GetData();
                    int iColor;

                    for( iColor = 0; iColor < palArray->GetCount(); iColor++ )
                    {
                        printf( "    Palette[%d] = %d,%d,%d,%d\n",
                                iColor,
                                (*palArray)[ iColor*4 + 0 ],
                                (*palArray)[ iColor*4 + 1 ],
                                (*palArray)[ iColor*4 + 2 ],
                                (*palArray)[ iColor*4 + 3 ] );
                    }
                }
                catch(...)
                {
                }

                // For now, only translate the first file. 
                if( iCounter == 0 )
                    TranslateTo( raster, 
                                 CPLString().Printf( "out_%d.tif", iCounter++) );
            }
        }
    }
    catch (FdoException* ge) 
    {
        printf( "Trapped exception: %ls\n", ge->GetExceptionMessage() );
    }


    conn->Close();
}

/************************************************************************/
/*                        FdoStringToReadable()                         */
/*                                                                      */
/*      This function is to support debugging.                          */
/************************************************************************/

const char *FdoStringToReadable( FdoString *pInput )

{
    static FdoStringP oStringPerm;
    FdoStringP oStringTemp( pInput );

    oStringPerm = oStringTemp;

    return (const char *) oStringPerm;
}

/************************************************************************/
/*                            TranslateTo()                             */
/*                                                                      */
/*      Translate an IRaster to a GeoTIFF file.                         */
/************************************************************************/

static void TranslateTo( FdoPtr<FdoIRaster> raster, 
                         const char *pszOutFilename )

{
    FdoPtr<FdoRasterDataModel> data_model = raster->GetDataModel();
    GDALDriverH hOutDriver = GDALGetDriverByName( "GTiff" );
    GDALDataType eOutType;

    if( data_model == NULL )
        return;

    GDALAllRegister();

/* -------------------------------------------------------------------- */
/*      Set alternate render size.                                      */
/* -------------------------------------------------------------------- */
//    raster->SetImageXSize(200);
//    raster->SetImageYSize(200);

/* -------------------------------------------------------------------- */
/*      Set alternate bounds.                                           */
/* -------------------------------------------------------------------- */
#ifdef notdef
    FdoStringP wkt = L"POLYGON ((440720 3720600, 471440 3720600, 471440 3741320, 440720 3741320, 440720 3720600))";

    FdoPtr<FdoAgfGeometryFactory> geomFactory = 
        FdoAgfGeometryFactory::GetInstance();
    FdoPtr<FdoIGeometry> geometry = 
        geomFactory->CreateGeometry(wkt);
    FdoPtr<FdoByteArray> ba = geomFactory->GetAgf( geometry );

    raster->SetBounds( ba );

    ba = raster->GetBounds();
    geometry = geomFactory->CreateGeometryFromAgf(ba);
    wkt = geometry->GetText();

    printf( "    Bounds = %s\n", (const char *) wkt );
#endif

/* -------------------------------------------------------------------- */
/*      Determine number of bands.                                      */
/* -------------------------------------------------------------------- */
    int nBandCount;

    if( data_model->GetDataModelType() == FdoRasterDataModelType_Gray )
        nBandCount = 1;
    else if( data_model->GetDataModelType() == FdoRasterDataModelType_RGB )
        nBandCount = 3;
    else if( data_model->GetDataModelType() == FdoRasterDataModelType_RGBA )
        nBandCount = 4;
    else if( data_model->GetDataModelType() == FdoRasterDataModelType_Palette )
        nBandCount = 1;
    else if( data_model->GetDataModelType() == FdoRasterDataModelType_Bitonal )
    {
        fprintf( stderr, 
                 "Bitonal RasterDataModelType not supported by rtest.\n" );
        return;
    }
    else
    {
        fprintf( stderr, "Unrecognised RasterDataModelType\n" );
        return;
    }
        
/* -------------------------------------------------------------------- */
/*      Determine target data type.                                     */
/* -------------------------------------------------------------------- */
    if( data_model->GetBitsPerPixel() == 8 * nBandCount )
        eOutType = GDT_Byte;
    else if( data_model->GetBitsPerPixel() == 16 * nBandCount
             && data_model->GetDataType() == FdoRasterDataType_Integer )
        eOutType = GDT_Int16;
    else if( data_model->GetBitsPerPixel() == 16 * nBandCount
             && data_model->GetDataType() == FdoRasterDataType_UnsignedInteger)
        eOutType = GDT_UInt16;
    else if( data_model->GetBitsPerPixel() == 32 * nBandCount
             && data_model->GetDataType() == FdoRasterDataType_Integer)
        eOutType = GDT_Int32;
    else if( data_model->GetBitsPerPixel() == 32 * nBandCount
             && data_model->GetDataType() == FdoRasterDataType_UnsignedInteger)
        eOutType = GDT_UInt32;
    else if( data_model->GetBitsPerPixel() == 32 * nBandCount
             && data_model->GetDataType() == FdoRasterDataType_Float)
        eOutType = GDT_Float32;
    else if( data_model->GetBitsPerPixel() == 64 * nBandCount
             && data_model->GetDataType() == FdoRasterDataType_Float)
        eOutType = GDT_Float64;
    else
    {
        fprintf( stderr, 
                 "Unable to determine GDAL data type from data model.\n" );
        return;
    }

/* -------------------------------------------------------------------- */
/*      Create the output file.                                         */
/* -------------------------------------------------------------------- */
    GDALDatasetH hOutDS;

    hOutDS = GDALCreate( hOutDriver, pszOutFilename, 
                         raster->GetImageXSize(), raster->GetImageYSize(), 
                         nBandCount, eOutType, NULL );

    if( hOutDS == NULL )
        return;

/* -------------------------------------------------------------------- */
/*      Create the data stream to read from.                            */
/* -------------------------------------------------------------------- */
    FdoPtr<FdoIStreamReaderTmpl<FdoByte> > stream
     = dynamic_cast<FdoIStreamReaderTmpl<FdoByte>*>(raster->GetStreamReader());

/* -------------------------------------------------------------------- */
/*      Create buffer for one scanline of data.                         */
/* -------------------------------------------------------------------- */
    FdoByte *pabyLineData;
    int nComponentSize = (GDALGetDataTypeSize(eOutType) / 8);
    int   nLineData = nBandCount * raster->GetImageXSize() * nComponentSize; 

    pabyLineData = (FdoByte *) CPLMalloc(nLineData);

/* -------------------------------------------------------------------- */
/*      Process all scanlines.                                          */
/* -------------------------------------------------------------------- */
    int iLine;

    stream->Reset();
    for( iLine = 0; iLine < raster->GetImageYSize(); iLine++ )
    {
        if( stream->ReadNext( pabyLineData, 0, nLineData )
            != nLineData )
        {
            fprintf( stderr, "Got an error or short buffer in ReadNext()\n" );
            break;
        }

        if( GDALDatasetRasterIO( hOutDS, GF_Write, 
                                 0, iLine, raster->GetImageXSize(), 1, 
                                 pabyLineData, raster->GetImageXSize(), 1, 
                                 eOutType, nBandCount, NULL, 
                                 nComponentSize * nBandCount, 
                                 nLineData, 
                                 nComponentSize )
            != CE_None )
        {
            break;
        }
    }

/* -------------------------------------------------------------------- */
/*      Close and cleanup.                                              */
/* -------------------------------------------------------------------- */
    GDALClose( hOutDS );
}
