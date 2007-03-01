/*
 * Copyright (C) 2006 Frank Warmerdam
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
 **************************************************************************
 * 
 * Purpose: 
 *
 * This is a commandline utility to interogate information about a raster
 * connection, and to dump read rasters to TIFF files. 
 *
 */

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
static void ReportSchemas( FdoPtr<FdoIConnection> conn, FdoStringP &FeatureClassName,
                           FdoStringP &FeatureIdName, FdoStringP &RasterName,
                           int bActuallyReport );
static void ReportSpatialContexts( FdoPtr<FdoIConnection> connection );

/************************************************************************/
/*                               Usage()                                */
/************************************************************************/

static void Usage()

{
    printf( "\n"
            "Usage: fdorastutil [-schema] [-sc] [-dump] [-nofeatures]\n"
            "                   [-p <provider>] [-fc <featureclass>]\n"
            "                   [-config <file>] [<target>]\n"
            "\n"
            "  -schema: Report schema\n"
            "  -sc: Report spatial contexts.\n"
            "  -dump: Dump rasters to TIFF files.\n"
            "  -nofeatures: Don't report feature records.\n"
            "  -p <provider>: Provider.  ie. OSGeo.Gdal.3.3\n"
            "  -fc <featureclass>: feature class to report/dump (default is first)\n"
            "  -config <file>: Use XML configuration file for connection.\n"
            "  <target>: Use file or directory as DefaultRasterFileLocation for\n"
            "            connection.\n" );
    exit( 1 );
}

/************************************************************************/
/*                                main()                                */
/************************************************************************/

int main( int argc, char ** argv )
{
    const char *targetConfig = NULL;
    const char *targetFileDir = NULL;
    int         bReportSchema = FALSE;
    int         bReportSpatialContexts = FALSE;
    int         bReportFeatures = TRUE;
    int         bTranslateFeatures = FALSE;
    int         iArg;
    FdoStringP  provider = "OSGeo.Gdal.3.3";
    FdoStringP  FeatureClassName;

/* -------------------------------------------------------------------- */
/*      Process arguments.                                              */
/* -------------------------------------------------------------------- */
    for( iArg = 1; iArg < argc; iArg++ )
    {
        if( EQUAL(argv[iArg],"-schema") )
            bReportSchema = TRUE;
        else if( EQUAL(argv[iArg],"-sc") )
            bReportSpatialContexts = TRUE;
        else if( EQUAL(argv[iArg],"-dump") )
            bTranslateFeatures = TRUE;
        else if( EQUAL(argv[iArg],"-nofeatures") )
            bReportFeatures = FALSE;
        else if( iArg < argc-1 && EQUAL(argv[iArg],"-p") )
            provider = argv[++iArg];
        else if( iArg < argc-1 && EQUAL(argv[iArg],"-fc") )
            FeatureClassName = argv[++iArg];
        else if( iArg < argc-1 && EQUAL(argv[iArg],"-config") )
            targetConfig = argv[++iArg];
        else if( argv[iArg][0] == '-' )
            Usage();
        else if( targetFileDir == NULL )
            targetFileDir = argv[iArg];
        else
            Usage();
    }

    if( targetConfig == NULL && targetFileDir == NULL )
        Usage();

/* -------------------------------------------------------------------- */
/*      Setup connection                                                */
/* -------------------------------------------------------------------- */

    // Get instance of the connection manager
    FdoPtr<IConnectionManager> manager;
    manager = FdoFeatureAccessManager::GetConnectionManager();
    if (0 == manager)
    {
        printf("GetConnectionManager returned nullptr.\n");
        return 0;
    }

    // Call the manager’s CreateConnection() method using the provider
    // internal name as an argument to obtain a connection object.
    FdoPtr<FdoIConnection> conn;
    conn = manager->CreateConnection (provider);
    if (0 == conn)
    {
        printf("CreateConnection returned nullptr.\n");
        return 0;
    }

/* -------------------------------------------------------------------- */
/*      Setup for a configuration file.                                 */
/* -------------------------------------------------------------------- */
    if( targetConfig != NULL )
    {
        printf("Setting up connection config file: %s\n", targetConfig );

        try
        {
            FdoStringP configFile(targetConfig);
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
/*      Setup for a file for directory.                                 */
/* -------------------------------------------------------------------- */
    else
    {
        try
        {
            FdoStringP defaultLocation;

            defaultLocation = L"DefaultRasterFileLocation=";
            defaultLocation += targetFileDir;
            conn->SetConnectionString (defaultLocation);
        }
        catch (FdoException* ge) 
        {
            printf( "Trapped exception: %ls\n", ge->GetExceptionMessage() );
            exit( 1 );
        }
    }

/* -------------------------------------------------------------------- */
/*      Open the connection.                                            */
/* -------------------------------------------------------------------- */
    try
    {
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
        exit( 1 );
    }

/* -------------------------------------------------------------------- */
/*      Report on spatial contexts if requested.                        */
/* -------------------------------------------------------------------- */
    if( bReportSpatialContexts )
        ReportSpatialContexts( conn );

/* -------------------------------------------------------------------- */
/*      Describe layer.                                                 */
/* -------------------------------------------------------------------- */
    FdoStringP  FeatureIdName;
    FdoStringP  RasterName;

    ReportSchemas( conn, FeatureClassName, FeatureIdName, RasterName, 
                   bReportSchema );

/* -------------------------------------------------------------------- */
/*      Actually scan raster records, reporting on them.                */
/* -------------------------------------------------------------------- */
    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (FeatureClassName);
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        int iCounter = 0;
    
        while (reader->ReadNext () && bReportFeatures )
        {
            FdoPtr<FdoIRaster>  raster;

            printf( "Feature:\n" );

            printf( "  Id = %ls\n", reader->GetString( FeatureIdName ) );

            raster = reader->GetRaster( RasterName );
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
                    printf( "    No Palette\n" );
                }

                // For now, only translate the first file. 
                if( iCounter == 0 && bTranslateFeatures )
                    TranslateTo( raster, 
                                 CPLString().Printf( "out_%d.tif", iCounter) );

                iCounter++;
            }
        }

        if( iCounter == 0 && bReportFeatures )
        {
            printf( "No features found for class %s.\n", 
                    (const char *) FeatureClassName );
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

/************************************************************************/
/*                       ReportSpatialContexts()                        */
/************************************************************************/

static void ReportSpatialContexts( FdoPtr<FdoIConnection> connection )

{
    FdoICommand* cmd = connection->CreateCommand(FdoCommandType_GetSpatialContexts);
    FdoPtr<FdoIGetSpatialContexts> getSpatialContexts = static_cast<FdoIGetSpatialContexts*>(cmd);
    FdoPtr<FdoISpatialContextReader> spatialContextsReader = getSpatialContexts->Execute();
	
    //test the first spatial context
    while( spatialContextsReader->ReadNext() )
    {
        printf( "Spatial Context\n"
                "  Name:              %ls\n"
                "  Description:       %ls\n"
                "  Coordinate System: %ls\n"
                "  CS WKT:            %ls\n",
                spatialContextsReader->GetName(),
                spatialContextsReader->GetDescription(),
                spatialContextsReader->GetCoordinateSystem(),
                spatialContextsReader->GetCoordinateSystemWkt() );

        FdoSpatialContextExtentType type = spatialContextsReader->GetExtentType() ;
	
        FdoByteArray *byteArray = spatialContextsReader->GetExtent() ;
        FdoPtr<FdoFgfGeometryFactory> agfFactory = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoIGeometry> geometry = agfFactory->CreateGeometryFromFgf(byteArray);
        FdoPtr<FdoIEnvelope> envelope = geometry->GetEnvelope();

        printf( "  Extent: (%g,%g)-(%g,%g)\n",
                envelope->GetMinX(), envelope->GetMinY(),
                envelope->GetMaxX(), envelope->GetMaxY() );
    }
}

/************************************************************************/
/*                           ReportSchemas()                            */
/************************************************************************/

static void ReportSchemas( FdoPtr<FdoIConnection> conn, 
                           FdoStringP &FeatureClassName,
                           FdoStringP &FeatureIdName,
                           FdoStringP &RasterName,
                           int bActuallyReport )

{
    try
    {
        FdoPtr<FdoIDescribeSchema> describe = 
            (FdoIDescribeSchema*)conn->CreateCommand(
                FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();

        FdoFeatureSchema* schema = schemas->GetItem (0);

        if( bActuallyReport )
            printf ("Schema: %ls\n", schema->GetName());

        if ((schema->GetDescription () != NULL) 
            && bActuallyReport
            && (0 != wcscmp (schema->GetDescription (), L"")))
            printf ("    Description: %ls\n", schema->GetDescription ());

        FdoClassCollection* classes = schema->GetClasses ();
        for (int j = 0; j < classes->GetCount (); j++)
        {
            FdoClassDefinition* cls = classes->GetItem (j);

            // Output basic class info:
            if( bActuallyReport )
            {
                if (FdoClassType_FeatureClass == cls->GetClassType ())
                    printf ("    Feature Class: %ls\n", cls->GetName ());
                else
                    printf ("    Class: %ls\n", cls->GetName ());
            }

            // pick first feature class encountered.
            if( wcscmp(FeatureClassName,L"") == 0 )
                FeatureClassName = cls->GetName();

            if ((cls->GetDescription () != NULL) 
                && (0 != wcscmp (cls->GetDescription (), L""))
                && bActuallyReport )
                printf ("        Description: %ls\n", cls->GetDescription ());

            FdoPtr<FdoClassCapabilities> classCapabilities = cls->GetCapabilities();
            if( bActuallyReport )
            {
                printf ("        Class Capabilities:\n");
                if (classCapabilities == NULL)
                    printf ("            (Not available).\n");
                else
                {
                    printf ("            Supports locking: %s\n", classCapabilities->SupportsLocking() ? "yes" : "no");
                    printf ("            Supports long transactions: %s\n", classCapabilities->SupportsLongTransactions() ? "yes" : "no");
                }
            }
                
            // Output identity properties:
            FdoDataPropertyDefinitionCollection* identity = cls->GetIdentityProperties ();
            for (int k = 0; k < identity->GetCount (); k++)
            {
                FdoDataPropertyDefinition* definition = identity->GetItem (k);
                FeatureIdName = definition->GetName();

                if( bActuallyReport )
                {
                    printf ("        Id: %ls\n", definition->GetName ());

                    if ((definition->GetDescription () != NULL) 
                        && (0 != wcscmp (definition->GetDescription (), L"")))
                        printf ("            Description: %ls\n", definition->GetDescription ());
                    printf ("            Type: %d Length: %d Precision: %d %ls\n",
                            (int) definition->GetDataType (),
                            definition->GetLength (),
                            definition->GetPrecision (),
                            definition->GetNullable () ? L"Nullable" : L"NotNull");
                }
                definition->Release ();
            }

            // Output regular properties:
            FdoPropertyDefinitionCollection* properties = cls->GetProperties ();
            for (int k = 0; k < properties->GetCount (); k++)
            {
                FdoPropertyDefinition* definition = properties->GetItem (k);

                if (definition->GetPropertyType () == FdoPropertyType_RasterProperty)
                {
                    FdoRasterPropertyDefinition *r_definition = 
                        (FdoRasterPropertyDefinition*) definition;
                    RasterName = definition->GetName();
                }

                if( !bActuallyReport )
                    continue;

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
                    RasterName = definition->GetName();

                    srs = r_definition->GetSpatialContextAssociation();
                    if( srs != NULL )
                        printf( "          SRS = %s\n", (const char *) srs );
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
            if ((0 != count) && (NULL != names) && bActuallyReport )
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
        exit( 1 );
    }
}
