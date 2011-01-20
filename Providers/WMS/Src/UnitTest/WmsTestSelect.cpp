/*
 * Copyright (C) 2004-2007  Autodesk, Inc.
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
#include "WmsTestSelect.h"

CPPUNIT_TEST_SUITE_REGISTRATION( WmsTestSelect );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( WmsTestSelect, "WmsTestSelect");

WmsTestSelect::WmsTestSelect ()
{
}

WmsTestSelect::~WmsTestSelect ()
{
}

void WmsTestSelect::testServer1 ()
{
//TODO: remove this when ? the bug ? on Linux is fixed
#ifdef WIN32
    bool failed = false;

    try
    {
	    FdoPtr<FdoIConnection> conn = this->GetConnection ();
	    conn->SetConnectionString (L"FeatureServer=http://www.cgkn.net/cgi-bin/cgknwms");
	    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

	    FdoPtr<FdoISelect> cmd = static_cast<FdoISelect *> (conn->CreateCommand (FdoCommandType_Select));
	    cmd->SetFeatureClassName (L"Borders_Poly");
	    FdoPtr<FdoIdentifierCollection> selProps = cmd->GetPropertyNames ();
	    CPPUNIT_ASSERT (selProps->GetCount () == 0);
	    FdoPtr<FdoIdentifier> prop = FdoIdentifier::Create (L"FeatId");
	    selProps->Add (prop);
	    FdoPtr<FdoIFeatureReader> featReader = cmd->Execute ();

	    // test GetClassDefinition method
	    FdoPtr<FdoClassDefinition> clsDef = featReader->GetClassDefinition ();
	    FdoPtr<FdoPropertyDefinitionCollection> props = clsDef->GetProperties ();	
	    CPPUNIT_ASSERT (wcscmp (clsDef->GetName (), L"Borders_Poly") == 0);
	    CPPUNIT_ASSERT (props->GetCount () == 0);
	    FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseProps = clsDef->GetBaseProperties ();
	    CPPUNIT_ASSERT (baseProps->GetCount () != 0);
    	

	    FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader;

	    // Layer "Borders_Poly"
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    CPPUNIT_ASSERT (wcscmp (featReader->GetString(L"FeatId"), L"Borders_Poly") == 0);
	    try 
	    {
		    (featReader->GetString(L"non-exist"));		
		    CPPUNIT_ASSERT (false);
	    }	
	    catch(FdoException*)
	    {
		    // Expected exception caught.
	    }

	    FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Raster");
	    raster->SetImageXSize(1000);
	    raster->SetImageYSize(1000);
	    FdoIStreamReader* streamReader = raster->GetStreamReader ();
	    byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (streamReader);
	    CPPUNIT_ASSERT (streamReader != NULL);
	    CPPUNIT_ASSERT (byteStreamReader != NULL);
	    FdoByte buff[4096];
	    FdoInt64 cntTotal = 0;
	    FdoInt32 cntRead = 0;
	    do
	    {
		    cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
		    cntTotal += cntRead;
	    }
	    while (cntRead);

	    FdoPtr<FdoIRasterPropertyDictionary> x = raster->GetAuxiliaryProperties();
	    FdoDataValue* val1 = x->GetProperty(L"Palette");
	    FdoDataValue* val2 = x->GetProperty(L"NumOfPaletteEntries");

	    struct RgbColor
	    {
		    union 
		    {
			    struct { FdoByte red; FdoByte green; FdoByte blue; FdoByte alpha; } rgba;
			    FdoInt32 packed;
		    };
	    };

	    FdoPtr<FdoInt32Value> val3 = static_cast<FdoInt32Value*>(val2);
	    FdoPtr<FdoBLOBValue> val4 = static_cast<FdoBLOBValue*>(val1);
	    FdoPtr<FdoByteArray> paletteData  = val4->GetData();

	    RgbColor*	palette = (RgbColor*)paletteData->GetData();

	    CPPUNIT_ASSERT (cntTotal == 1000000);	
	    CPPUNIT_ASSERT (!featReader->ReadNext ());	

	    // Layer "BW_Shaded_Relief_Magnetics"
	    cmd->SetFeatureClassName (L"BW_Shaded_Relief_Magnetics");	

	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());

	    // get the raster first time
	    raster = featReader->GetRaster (L"Raster");
	    streamReader = raster->GetStreamReader ();
	    byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (streamReader);
	    cntTotal = 0;
	    do
	    {
		    cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
		    cntTotal += cntRead;
	    }
	    while (cntRead);

	    // get the raster second time
	    raster = featReader->GetRaster (L"Raster");
	    streamReader = raster->GetStreamReader ();
	    byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (streamReader);
	    cntTotal = 0;
	    do
	    {
		    cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
		    cntTotal += cntRead;
	    }
	    while (cntRead);

	    CPPUNIT_ASSERT (!featReader->ReadNext ());

	    // Layer "Borders_Line"
	    cmd->SetFeatureClassName (L"Borders_Line");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());
	    streamReader = raster->GetStreamReader ();
	    byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (streamReader);

	    cntTotal = 0;
	    do
	    {
		    cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
		    cntTotal += cntRead;
	    }
	    while (cntRead);

	    // Layer "Radarsat_1000"
	    cmd->SetFeatureClassName (L"Radarsat_1000");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());	

	    // Layer "Colour_Shaded_Relief_Bouger"
	    cmd->SetFeatureClassName (L"Colour_Shaded_Relief_Bouger");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());	
	    raster = featReader->GetRaster (L"Raster");	
	    featReader->GetString (L"FeatId");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());
    }
/*
    catch (FdoException* e)
    {
        fail(e);
    }
*/
    catch ( ... )
    {
        failed = true;
    }

    CPPUNIT_ASSERT_MESSAGE("test started working again", failed);

#endif //WIN32
}

void WmsTestSelect::testNestedClip ()
{
    try
    {
	    FdoPtr<FdoIConnection> conn = this->GetConnection ();
	    conn->SetConnectionString (L"FeatureServer=http://www.cgkn.net/cgi-bin/cgknwms");
	    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

	    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect *> (conn->CreateCommand (FdoCommandType_Select));
	    cmdSelect->SetFeatureClassName (L"Borders_Poly");

	    // set up clip function: CLIP(Image, 100, 100, 1500.0, 1000.0)
	    FdoPtr<FdoExpressionCollection> funcParams = FdoExpressionCollection::Create();
	    FdoPtr<FdoIdentifier> rasterProp = FdoIdentifier::Create(L"Image");
	    funcParams->Add(rasterProp);
	    FdoPtr<FdoDataValue> minX = FdoDataValue::Create(100, FdoDataType_Double);
	    funcParams->Add(minX);
	    FdoPtr<FdoDataValue> minY = FdoDataValue::Create(100, FdoDataType_Double);
	    funcParams->Add(minY);
	    FdoPtr<FdoDataValue> maxX = FdoDataValue::Create(1500, FdoDataType_Double);
	    funcParams->Add(maxX);
	    FdoPtr<FdoDataValue> maxY = FdoDataValue::Create(1000, FdoDataType_Double);
	    funcParams->Add(maxY);

	    FdoPtr<FdoFunction> clipFunc = FdoFunction::Create(L"CLIP", funcParams);
	    FdoPtr<FdoExpressionCollection> outerFuncParams = FdoExpressionCollection::Create ();
	    minX = FdoDataValue::Create (500);
	    minY = FdoDataValue::Create (500);
	    maxX = FdoDataValue::Create (2000);
	    maxY = FdoDataValue::Create (1200);
	    outerFuncParams->Add (clipFunc);
	    outerFuncParams->Add (minX);
	    outerFuncParams->Add (minY);
	    outerFuncParams->Add (maxX);
	    outerFuncParams->Add (maxY);

	    FdoPtr<FdoFunction> outerClipFunc = FdoFunction::Create (L"CLIP", outerFuncParams);
	    FdoPtr<FdoComputedIdentifier> clipIdentifier = FdoComputedIdentifier::Create(L"clippedRaster", outerClipFunc);
    	
	    FdoPtr<FdoIdentifierCollection> propsToSelect = cmdSelect->GetPropertyNames();
    	
	    // add it to the properties to select
	    propsToSelect->Add(clipIdentifier);

	    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

	    CPPUNIT_ASSERT (featureReader->ReadNext ());
	    FdoPtr<FdoIRaster> raster = featureReader->GetRaster (L"clippedRaster");
	    CPPUNIT_ASSERT (raster.p);
	    FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();
	    CPPUNIT_ASSERT (stream.p);
	    CPPUNIT_ASSERT (!featureReader->ReadNext ());	
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

// http://CADCISDJIAB.ads.autodesk.com:8888/cgi-bin/mapserv.exe?map=wms/wms.map
void WmsTestSelect::testHttpBasicAuthentification ()
{
	try
	{
		FdoPtr<FdoIConnection> conn = this->GetConnection ();

		// use ip address directly
		conn->SetConnectionString (L"FeatureServer=http://CADCISDJIAB.ads.autodesk.com:8888/cgi-bin/mapserv.exe?map=wms/wms.map;Username=bruce;Password=brucebruce");
		CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

		FdoPtr<FdoIDescribeSchema> cmdDescribeSchema = static_cast<FdoIDescribeSchema *> (conn->CreateCommand (FdoCommandType_DescribeSchema));
		FdoPtr<FdoFeatureSchemaCollection> schemas = cmdDescribeSchema->Execute ();

		FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
		cmdSelect->SetFeatureClassName(L"lakespy2");


		FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

		while (featureReader->ReadNext ())
		{
			FdoPtr<FdoIRaster> raster = featureReader->GetRaster (L"Raster");
			FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();		
		}
	}
	catch(FdoException* ex)
	{
        fail(ex);
	}
}

void WmsTestSelect::testGetBounds ()
{
    FdoPtr<FdoIConnection> conn = this->GetConnection ();
	try
	{
		conn->SetConnectionString (L"FeatureServer=http://wms.jpl.nasa.gov/wms.cgi");
		CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

		FdoPtr<FdoISelect> cmd = static_cast<FdoISelect *> (conn->CreateCommand (FdoCommandType_Select));
		cmd->SetFeatureClassName (L"global_mosaic");	    
		FdoPtr<FdoIFeatureReader> featReader = cmd->Execute ();

		CPPUNIT_ASSERT (featReader->ReadNext ());
		FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Raster");
		FdoPtr<FdoByteArray> gba = raster->GetBounds ();
		FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance ();
		FdoPtr<FdoIGeometry> geom = factory->CreateGeometryFromFgf (gba.p);
		FdoPtr<FdoIEnvelope> enve = geom->GetEnvelope ();

		CPPUNIT_ASSERT (enve->GetMinX() == -180);
		CPPUNIT_ASSERT (enve->GetMinY() == -60);
		CPPUNIT_ASSERT (enve->GetMaxX() == 180);
		CPPUNIT_ASSERT (enve->GetMaxY() == 84);
#ifdef _DEBUG
		wprintf (L"Extent: (%f,%f %f,%f)\n", enve->GetMinX(), enve->GetMinY(), enve->GetMaxX(), enve->GetMaxY());                
#endif
	}
	catch (FdoException* e) 
    {
        fail (e);
	}
}

// http://www.bsc-eoc.org/cgi-bin/bsc_ows.asp
void WmsTestSelect::testServer2 ()
{
    try
    {
	    FdoPtr<FdoIConnection> conn = this->GetConnection ();
	    conn->SetConnectionString (L"FeatureServer=http://www.bsc-eoc.org/cgi-bin/bsc_ows.asp;DefaultImageHeight=400");
	    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

	    FdoPtr<FdoISelect> cmd = static_cast<FdoISelect *> (conn->CreateCommand (FdoCommandType_Select));
	    cmd->SetFeatureClassName (L"IBA");
	    FdoPtr<FdoIFeatureReader> featReader = cmd->Execute ();

	    // Layer "IBA"
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());

	    // Layer "CBC_PT"
	    cmd->SetFeatureClassName (L"CBC_PT");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());

	    // Layer "CBC_PY"
	    cmd->SetFeatureClassName (L"CBC_PY");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());

	    // Layer "MMP"
	    cmd->SetFeatureClassName (L"MMP");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());

	    // Layer "CLLS"
	    cmd->SetFeatureClassName (L"CLLS");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());

	    // Layer "OBBA_SQUARE"
	    cmd->SetFeatureClassName (L"OBBA_SQUARE");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());

	    // Layer "OBBA_REGION"
	    cmd->SetFeatureClassName (L"OBBA_REGION");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());

	    // Layer "OBBA_BLOCK"
	    cmd->SetFeatureClassName (L"OBBA_BLOCK");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());


	    // Layer "OWLS"
	    cmd->SetFeatureClassName (L"OWLS");

	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());

	    // Layer "BBS_PT"
	    cmd->SetFeatureClassName (L"BBS_PT");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

//test 1.3.0 version and 1.1.0 version server with axis order reverse or not
void WmsTestSelect::testServer3 ()
{
    try
    {
	    FdoPtr<FdoIConnection> conn = this->GetConnection ();
	    conn->SetConnectionString (L"FeatureServer=http://demo.cubewerx.com/demo/cubeserv/cubeserv.cgi?version=1.3.0");
	    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

	    FdoPtr<FdoISelect> cmd = static_cast<FdoISelect *> (conn->CreateCommand (FdoCommandType_Select));
		cmd->SetFeatureClassName (L"Foundation BNDTXT_1M");
	    FdoPtr<FdoIFeatureReader> featReader = cmd->Execute ();

		CPPUNIT_ASSERT (featReader->ReadNext ());
		FdoPtr<FdoIRaster> raster = featReader->GetRaster(L"Raster");

        // Test the bounds
        FdoPtr<FdoByteArray> gba = raster->GetBounds ();
        FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance ();
        FdoPtr<FdoIGeometry> geom = factory->CreateGeometryFromFgf (gba.p);
        FdoPtr<FdoIEnvelope> enve = geom->GetEnvelope ();

        CPPUNIT_ASSERT (enve->GetMinX() == -179.909469604492);
        CPPUNIT_ASSERT (enve->GetMinY() == -82.9724044799805);
        CPPUNIT_ASSERT (enve->GetMaxX() == 179.91194152832);
        CPPUNIT_ASSERT (enve->GetMaxY() == 83.61899566650389);

		conn->Close();

		// connect the same server with 1.1.0 version
	    conn->SetConnectionString (L"FeatureServer=http://demo.cubewerx.com/demo/cubeserv/cubeserv.cgi?version=1.1.0");
	    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

	    cmd = static_cast<FdoISelect *> (conn->CreateCommand (FdoCommandType_Select));
		cmd->SetFeatureClassName (L"Foundation BNDTXT_1M");
	    featReader = cmd->Execute ();

		CPPUNIT_ASSERT (featReader->ReadNext ());
		raster = featReader->GetRaster(L"Raster");

        // Test the bounds
        gba = raster->GetBounds ();
        factory = FdoFgfGeometryFactory::GetInstance ();
        geom = factory->CreateGeometryFromFgf (gba.p);
        enve = geom->GetEnvelope ();

        CPPUNIT_ASSERT (enve->GetMinX() == -179.909469604492);
        CPPUNIT_ASSERT (enve->GetMinY() == -82.9724044799805);
        CPPUNIT_ASSERT (enve->GetMaxX() == 179.91194152832);
        CPPUNIT_ASSERT (enve->GetMaxY() == 83.61899566650389);
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

//test 1.3.0 version server with axis order reverse
void WmsTestSelect::testServer4 ()
{
    try
    {
	    FdoPtr<FdoIConnection> conn = this->GetConnection ();
	    conn->SetConnectionString (L"FeatureServer=http://sampleserver1.arcgisonline.com/ArcGIS/services/Specialty/ESRI_StatesCitiesRivers_USA/MapServer/WMSServer?version=1.3.0");
	    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

	    FdoPtr<FdoISelect> cmd = static_cast<FdoISelect *> (conn->CreateCommand (FdoCommandType_Select));
		cmd->SetFeatureClassName (L"0");
	    FdoPtr<FdoIFeatureReader> featReader = cmd->Execute ();

		CPPUNIT_ASSERT (featReader->ReadNext ());
		FdoPtr<FdoIRaster> raster = featReader->GetRaster(L"Raster");

        // Test the bounds
        FdoPtr<FdoByteArray> gba = raster->GetBounds ();
        FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance ();
        FdoPtr<FdoIGeometry> geom = factory->CreateGeometryFromFgf (gba.p);
        FdoPtr<FdoIEnvelope> enve = geom->GetEnvelope ();

        CPPUNIT_ASSERT (enve->GetMinX() == -178.217598);
        CPPUNIT_ASSERT (enve->GetMinY() == 18.924782);
        CPPUNIT_ASSERT (enve->GetMaxX() == -66.969271);
        CPPUNIT_ASSERT (enve->GetMaxY() == 71.406235);

		conn->Close();
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

void WmsTestSelect::testResample ()
{
    try
    {
	    FdoPtr<FdoIConnection> conn = this->GetConnection ();
	    conn->SetConnectionString (L"FeatureServer=http://www.bsc-eoc.org/cgi-bin/bsc_ows.asp;DefaultImageHeight=400");
	    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

	    FdoICommand* cmd = conn->CreateCommand(FdoCommandType_Select);
	    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);
	    cmdSelect->SetFeatureClassName(L"IBA");
    	
	    // set up clip function: CLIP(Image, 100, 100, 700.0, 500.0)
	    FdoPtr<FdoExpressionCollection> funcParams = FdoExpressionCollection::Create();
	    FdoPtr<FdoIdentifier> rasterProp = FdoIdentifier::Create(L"Image");
	    funcParams->Add(rasterProp);
	    FdoPtr<FdoDataValue> minX = FdoDataValue::Create(100, FdoDataType_Double);
	    funcParams->Add(minX);
	    FdoPtr<FdoDataValue> minY = FdoDataValue::Create(100, FdoDataType_Double);
	    funcParams->Add(minY);
	    FdoPtr<FdoDataValue> maxX = FdoDataValue::Create((FdoInt32(700)));
	    funcParams->Add(maxX);
	    FdoPtr<FdoDataValue> maxY = FdoDataValue::Create(500, FdoDataType_Double);
	    funcParams->Add(maxY);
	    FdoPtr<FdoDataValue> height = FdoDataValue::Create(800, FdoDataType_Double);
	    funcParams->Add(height);
	    FdoPtr<FdoDataValue> width = FdoDataValue::Create(600, FdoDataType_Double);
	    funcParams->Add(width);

	    FdoPtr<FdoFunction> resampleFunc = FdoFunction::Create(L"RESAMPLE", funcParams);
	    FdoPtr<FdoComputedIdentifier> resampleIdentifier = FdoComputedIdentifier::Create(L"resampledRaster", resampleFunc);
	    FdoPtr<FdoIdentifierCollection> propsToSelect = cmdSelect->GetPropertyNames();
    	
	    // add it to the properties to select
	    propsToSelect->Add(resampleIdentifier);

	    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

	    while (featureReader->ReadNext ())
	    {
		    FdoPtr<FdoIRaster> raster = featureReader->GetRaster (L"resampledRaster");
		    FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();		
	    }
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

void WmsTestSelect::testClip ()
{
    try
    {
	    FdoPtr<FdoIConnection> conn = this->GetConnection ();
	    conn->SetConnectionString (L"FeatureServer=http://www.bsc-eoc.org/cgi-bin/bsc_ows.asp");
	    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

	    FdoICommand* cmd = conn->CreateCommand(FdoCommandType_Select);
	    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);
	    cmdSelect->SetFeatureClassName(L"IBA");
    	
	    // set up clip function: CLIP(Image, 100, 100, 700.0, 500.0)
	    FdoPtr<FdoExpressionCollection> funcParams = FdoExpressionCollection::Create();
	    FdoPtr<FdoIdentifier> rasterProp = FdoIdentifier::Create(L"Image");
	    funcParams->Add(rasterProp);
	    FdoPtr<FdoDataValue> minX = FdoDataValue::Create(100, FdoDataType_Double);
	    funcParams->Add(minX);
	    FdoPtr<FdoDataValue> minY = FdoDataValue::Create(100, FdoDataType_Double);
	    funcParams->Add(minY);
	    FdoPtr<FdoDataValue> maxX = FdoDataValue::Create((FdoInt32(700)));
	    funcParams->Add(maxX);
	    FdoPtr<FdoDataValue> maxY = FdoDataValue::Create(500, FdoDataType_Double);
	    funcParams->Add(maxY);

	    FdoPtr<FdoFunction> clipFunc = FdoFunction::Create(L"CLIP", funcParams);
	    FdoPtr<FdoComputedIdentifier> clipIdentifier = FdoComputedIdentifier::Create(L"clippedRaster", clipFunc);
	    FdoPtr<FdoIdentifierCollection> propsToSelect = cmdSelect->GetPropertyNames();
    	
	    // add it to the properties to select
	    propsToSelect->Add(clipIdentifier);

	    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

	    while (featureReader->ReadNext ())
	    {
		    FdoPtr<FdoIRaster> raster = featureReader->GetRaster (L"clippedRaster");
		    FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();		
	    }
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

void WmsTestSelect::testDefaultHeight()
{
    try
    {
	    FdoPtr<FdoIConnection> conn = this->GetConnection ();
	    conn->SetConnectionString (L"FeatureServer=http://www.bsc-eoc.org/cgi-bin/bsc_ows.asp;DefaultImageHeight=300");
	    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

	    FdoICommand* cmd = conn->CreateCommand(FdoCommandType_Select);
	    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);
        cmdSelect->SetFeatureClassName(L"IBA");


	    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

	    while (featureReader->ReadNext ())
	    {
		    FdoPtr<FdoIRaster> raster = featureReader->GetRaster (L"Raster");
		    FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();		
	    }
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

//http://wms.jpl.nasa.gov/wms.cgi?
void WmsTestSelect::testNASAServer ()
{	
    bool failed = false;

    try
    {
	    FdoPtr<FdoIConnection> conn = this->GetConnection ();
	    FdoIoFileStreamP fileStream = FdoIoFileStream::Create(L"NASA_WMS_Config.xml", L"r");
	    conn->SetConnectionString (L"FeatureServer=http://wms.jpl.nasa.gov/wms.cgi?");
	    conn->SetConfiguration (fileStream);
	    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

	    FdoPtr<FdoIDescribeSchema> cmdDescribeSchema = static_cast<FdoIDescribeSchema *> (conn->CreateCommand (FdoCommandType_DescribeSchema));
	    FdoPtr<FdoFeatureSchemaCollection> schemas = cmdDescribeSchema->Execute ();

	    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
	    cmdSelect->SetFeatureClassName(L"global_mosaic");

	    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

	    while (featureReader->ReadNext ())
	    {
		    FdoPtr<FdoIRaster> raster = featureReader->GetRaster (L"Image");
		    FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();
		    FdoIStreamReader* streamReader = raster->GetStreamReader ();
		    FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (streamReader);
    		
		    CPPUNIT_ASSERT (streamReader != NULL);
		    CPPUNIT_ASSERT (byteStreamReader != NULL);
    		
		    FdoByte buff[4096];
		    FdoInt64 cntTotal = 0;
		    FdoInt32 cntRead = 0;
		    do
		    {
			    cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
			    cntTotal += cntRead;
		    }
		    while (cntRead);
	    }
	}
/*
    catch(FdoException* ex)
	{
        fail(ex);
	}
*/
    catch ( ... )
    {
        failed = true;
    }

    CPPUNIT_ASSERT_MESSAGE("test started working again", failed);
}

//http://wms.jpl.nasa.gov/wms.cgi?
void WmsTestSelect::testNASAServerDefaultOverrides ()
{	
    try
    {
	    FdoPtr<FdoIConnection> conn = this->GetConnection ();
	    conn->SetConnectionString (L"FeatureServer=http://wms.jpl.nasa.gov/wms.cgi?");
	    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

	    FdoPtr<FdoIDescribeSchemaMapping> cmdDescribeSchemaMapping = static_cast<FdoIDescribeSchemaMapping *> (conn->CreateCommand (FdoCommandType_DescribeSchemaMapping));
        cmdDescribeSchemaMapping->SetIncludeDefaults(false);
	    FdoSchemaMappingsP schemaMappings = cmdDescribeSchemaMapping->Execute ();
        CPPUNIT_ASSERT (schemaMappings == NULL);

        cmdDescribeSchemaMapping->SetIncludeDefaults(true);
	    schemaMappings = cmdDescribeSchemaMapping->Execute ();
        FdoInt32 numSchemas = schemaMappings->GetCount();
        CPPUNIT_ASSERT (numSchemas == 1);

        for (FdoInt32 i=0; i<1; i++)
        {
            FdoPhysicalSchemaMappingP schemaMapping = schemaMappings->GetItem(i);
            FdoWmsOvPhysicalSchemaMapping* ovMapping = static_cast<FdoWmsOvPhysicalSchemaMapping*>(schemaMapping.p);
            CPPUNIT_ASSERT (FdoStringP(ovMapping->GetName()) == L"WMS_Schema");
            
            FdoWmsOvClassesP ovClasses = ovMapping->GetClasses();
            FdoInt32 numClasses = ovClasses->GetCount();
            CPPUNIT_ASSERT (numClasses == 15);

            FdoWmsOvClassDefinitionP ovClass = ovClasses->GetItem(0);
            CPPUNIT_ASSERT (FdoStringP(ovClass->GetName()) == L"global_mosaic");

            FdoWmsOvRasterDefinitionP ovRasterDef = ovClass->GetRasterDefinition();
            CPPUNIT_ASSERT (FdoStringP(ovRasterDef->GetName()) == L"Raster");

            CPPUNIT_ASSERT (FdoStringP(ovRasterDef->GetBackgroundColor()) == L"0xFFFFFF");
            CPPUNIT_ASSERT (FdoStringP(ovRasterDef->GetImageFormat()) == L"image/png");
            CPPUNIT_ASSERT (FdoStringP(ovRasterDef->GetSpatialContextName()) == L"EPSG:4326");
            CPPUNIT_ASSERT (FdoStringP(ovRasterDef->GetTimeDimension()) == L"");
            CPPUNIT_ASSERT (FdoStringP(ovRasterDef->GetElevationDimension()) == L"");
            CPPUNIT_ASSERT (ovRasterDef->GetTransparent() == false);
            
            FdoWmsOvLayersP ovLayers = ovRasterDef->GetLayers();
            FdoInt32 numLayers = ovLayers->GetCount();
            CPPUNIT_ASSERT (numLayers == 1);

            FdoWmsOvLayerDefinitionP ovLayer = ovLayers->GetItem(0);
            CPPUNIT_ASSERT (FdoStringP(ovLayer->GetName()) == L"global_mosaic");

            ovClass = ovClasses->GetItem(1);
            CPPUNIT_ASSERT (FdoStringP(ovClass->GetName()) == L"global_mosaic_base");

            ovRasterDef = ovClass->GetRasterDefinition();
            CPPUNIT_ASSERT (FdoStringP(ovRasterDef->GetName()) == L"Raster");

            CPPUNIT_ASSERT (FdoStringP(ovRasterDef->GetBackgroundColor()) == L"0xFFFFFF");
            CPPUNIT_ASSERT (FdoStringP(ovRasterDef->GetImageFormat()) == L"image/png");
            CPPUNIT_ASSERT (FdoStringP(ovRasterDef->GetSpatialContextName()) == L"EPSG:4326");
            CPPUNIT_ASSERT (FdoStringP(ovRasterDef->GetTimeDimension()) == L"");
            CPPUNIT_ASSERT (FdoStringP(ovRasterDef->GetElevationDimension()) == L"");
            CPPUNIT_ASSERT (ovRasterDef->GetTransparent() == false);
            
            ovLayers = ovRasterDef->GetLayers();
            numLayers = ovLayers->GetCount();
            CPPUNIT_ASSERT (numLayers == 1);

            ovLayer = ovLayers->GetItem(0);
            CPPUNIT_ASSERT (FdoStringP(ovLayer->GetName()) == L"global_mosaic_base");
        }
	}
	catch(FdoException* ex)
	{
        fail(ex);
	}
}

//http://wms.jpl.nasa.gov/wms.cgi?
void WmsTestSelect::testNASAServer2 ()
{	
    bool failed = false;

    try
    {
	    FdoPtr<FdoIConnection> conn = this->GetConnection ();
	    conn->SetConnectionString (L"FeatureServer=http://wms.jpl.nasa.gov/wms.cgi?");
	    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

	    FdoPtr<FdoIDescribeSchema> cmdDescribeSchema = static_cast<FdoIDescribeSchema *> (conn->CreateCommand (FdoCommandType_DescribeSchema));
	    FdoPtr<FdoFeatureSchemaCollection> schemas = cmdDescribeSchema->Execute ();

	    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
	    cmdSelect->SetFeatureClassName(L"us_landsat_wgs84");

	    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

	    while (featureReader->ReadNext ())
	    {
		    FdoPtr<FdoIRaster> raster = featureReader->GetRaster (L"Raster");
		    FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();

		    CPPUNIT_ASSERT (stream->GetType() == FdoStreamReaderType_Byte);
		    FdoIoByteStreamReader* byteStreamReader = static_cast<FdoIoByteStreamReader*> (stream.p);
		    CPPUNIT_ASSERT (byteStreamReader != NULL);
    		
		    FdoByte buff[4096];
		    FdoInt64 cntTotal = 0;
		    FdoInt32 cntRead = 0;
		    do
		    {
			    cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
			    cntTotal += cntRead;
		    }
		    while (cntRead);
	    }
	}
/*
    catch(FdoException* ex)
	{
        fail(ex);
	}
*/
    catch ( ... )
    {
        failed = true;
    }

    CPPUNIT_ASSERT_MESSAGE("test started working again", failed);
}

//http://maps1.intergraph.com/wms/world/request.asp
void WmsTestSelect::testIntegraphWorld ()
{	
    try
    {
        FdoPtr<FdoIConnection> conn = this->GetConnection ();
        FdoPtr<FdoIConnectionInfo> info = conn->GetConnectionInfo();
        FdoPtr<FdoIConnectionPropertyDictionary> props = info->GetConnectionProperties();
        props->SetProperty(L"FeatureServer", L"http://maps1.intergraph.com/wms/world/request.asp");
        CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

        FdoPtr<FdoIDescribeSchema> cmdDescribeSchema = static_cast<FdoIDescribeSchema *> (conn->CreateCommand (FdoCommandType_DescribeSchema));
        FdoPtr<FdoFeatureSchemaCollection> schemas = cmdDescribeSchema->Execute ();

        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0);
        CPPUNIT_ASSERT (FdoStringP(schema->GetName()) == L"WMS_Schema");

        FdoPtr<FdoClassCollection> classes = schema->GetClasses();
        FdoPtr<FdoClassDefinition> classDef = classes->GetItem(L"COUNTRY");

        FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
        cmdSelect->SetFeatureClassName(classDef->GetName());

        FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();
        FdoPtr<FdoClassDefinition> classDef2 = featureReader->GetClassDefinition();
        CPPUNIT_ASSERT (FdoStringP(classDef2->GetName()) == L"COUNTRY");

        while (featureReader->ReadNext ())
        {
            FdoPtr<FdoIRaster> raster = featureReader->GetRaster (L"Raster");
            FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();

            CPPUNIT_ASSERT (stream->GetType() == FdoStreamReaderType_Byte);
            FdoIoByteStreamReader* byteStreamReader = static_cast<FdoIoByteStreamReader*> (stream.p);
            CPPUNIT_ASSERT (byteStreamReader != NULL);
        	
            FdoByte buff[4096];
            FdoInt64 cntTotal = 0;
            FdoInt32 cntRead = 0;
            do
            {
                cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
                cntTotal += cntRead;
            }
            while (cntRead);
        }
    }
    catch(FdoException* ex)
    {
        fail(ex);
    }
}

//http://demo.cubewerx.com/demo/cubeserv/cubeserv.cgi?CONFIG=main
void WmsTestSelect::testCubeServer ()
{
    try
    {
        FdoPtr<FdoIConnection> conn = this->GetConnection ();
        FdoPtr<FdoIConnectionInfo> info = conn->GetConnectionInfo();
        FdoPtr<FdoIConnectionPropertyDictionary> props = info->GetConnectionProperties();
        props->SetProperty(L"FeatureServer", L"http://demo.cubewerx.com/demo/cubeserv/cubeserv.cgi?CONFIG=main");
        CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

        FdoPtr<FdoIDescribeSchema> cmdDescribeSchema = static_cast<FdoIDescribeSchema *> (conn->CreateCommand (FdoCommandType_DescribeSchema));
        FdoPtr<FdoFeatureSchemaCollection> schemas = cmdDescribeSchema->Execute ();

        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0);

#ifdef _DEBUG
        FdoPtr<FdoClassCollection> classes = schema->GetClasses();
        FdoInt32 cntClass = classes->GetCount ();
        for (int i=0; i<cntClass; i++)
        {
            FdoPtr<FdoClassDefinition> classDef = classes->GetItem(i);
            printf ("Class: %s\n", (const char*)FdoStringP(classDef->GetName()));
        }
#endif//_DEBUG

        FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
        cmdSelect->SetFeatureClassName(L"Foundation LANDICEA_1M");

        FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();
        FdoPtr<FdoClassDefinition> classDef2 = featureReader->GetClassDefinition();
#ifdef _DEBUG
        printf ("ClassDefinition: %s\n", (const char*)FdoStringP(classDef2->GetName()));
#endif
        while (featureReader->ReadNext ())
        {
            FdoPtr<FdoIRaster> raster = featureReader->GetRaster (L"Raster");
            FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();

            CPPUNIT_ASSERT (stream->GetType() == FdoStreamReaderType_Byte);
            FdoIoByteStreamReader* byteStreamReader = static_cast<FdoIoByteStreamReader*> (stream.p);
            CPPUNIT_ASSERT (byteStreamReader != NULL);
        	
            FdoByte buff[4096];
            FdoInt64 cntTotal = 0;
            FdoInt32 cntRead = 0;
            do
            {
                cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
                cntTotal += cntRead;
            }
            while (cntRead);
        }

        conn->Close ();

        // set configuration file 
        FdoIoFileStreamP fileStream = FdoIoFileStream::Create(L"CubeServer_WMS_Config.xml", L"r");
        conn->SetConfiguration (fileStream);

        CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());
        cmdSelect = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
        cmdSelect->SetFeatureClassName(L"OceanSeas");
        featureReader = cmdSelect->Execute();
        while (featureReader->ReadNext ())
        {
            FdoPtr<FdoIRaster> raster = featureReader->GetRaster (L"Image");
            FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();

            CPPUNIT_ASSERT (stream->GetType() == FdoStreamReaderType_Byte);
            FdoIoByteStreamReader* byteStreamReader = static_cast<FdoIoByteStreamReader*> (stream.p);
            CPPUNIT_ASSERT (byteStreamReader != NULL);
        	
            FdoByte buff[4096];
            FdoInt64 cntTotal = 0;
            FdoInt32 cntRead = 0;
            do
            {
                cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
                cntTotal += cntRead;
            }
            while (cntRead);
        }

        conn->Close ();
    }
    catch(FdoException* ex)
    {
        fail(ex);
    }
}

//
// Test against NASA server with multi-layers.
// SRS: EPSG:4326
// Layers: srtm_mag, global_mosaic
void WmsTestSelect::testMultiLayers ()
{
    bool failed = false;

    try
    {
	    FdoPtr<FdoIConnection> conn = this->GetConnection ();
	    FdoIoFileStreamP fileStream = FdoIoFileStream::Create(L"MultiLayers_Config.xml", L"r");
	    conn->SetConnectionString (L"FeatureServer=http://wms.jpl.nasa.gov/wms.cgi?");
	    conn->SetConfiguration (fileStream);
	    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

	    FdoPtr<FdoIDescribeSchema> cmdDescribeSchema = static_cast<FdoIDescribeSchema *> (conn->CreateCommand (FdoCommandType_DescribeSchema));
	    FdoPtr<FdoFeatureSchemaCollection> schemas = cmdDescribeSchema->Execute ();

	    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
	    cmdSelect->SetFeatureClassName(L"global_mosaic");

	    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

	    CPPUNIT_ASSERT (featureReader->ReadNext ());	    
        FdoPtr<FdoIRaster> raster = featureReader->GetRaster (L"Image");

        // Test the bounds
        FdoPtr<FdoByteArray> gba = raster->GetBounds ();
        FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance ();
        FdoPtr<FdoIGeometry> geom = factory->CreateGeometryFromFgf (gba.p);
        FdoPtr<FdoIEnvelope> enve = geom->GetEnvelope ();

        CPPUNIT_ASSERT (enve->GetMinX() == -180.00);
        CPPUNIT_ASSERT (enve->GetMinY() == -60.00);
        CPPUNIT_ASSERT (enve->GetMaxX() == 180.00);
        CPPUNIT_ASSERT (enve->GetMaxY() == 84.00);
#ifdef _DEBUG
        wprintf (L"Extent: (%f,%f %f,%f)\n", enve->GetMinX(), enve->GetMinY(), enve->GetMaxX(), enve->GetMaxY());                
#endif

        FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();
        FdoIStreamReader* streamReader = raster->GetStreamReader ();
        FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (streamReader);

        CPPUNIT_ASSERT (streamReader != NULL);
        CPPUNIT_ASSERT (byteStreamReader != NULL);

        FdoByte buff[4096];
        FdoInt64 cntTotal = 0;
        FdoInt32 cntRead = 0;
        do
        {
            cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
            cntTotal += cntRead;
        }
        while (cntRead);

	    CPPUNIT_ASSERT (!featureReader->ReadNext ());
	}
/*
    catch(FdoException* ex)
	{
        fail(ex);
	}
*/
    catch ( ... )
    {
        failed = true;
    }

    CPPUNIT_ASSERT_MESSAGE("test started working again", failed);
}

// test against http://terraservice.net/ogccapabilities.ashx. This site uses
// different URLs for "GetCapabilities" and "GetMap" operations.
void WmsTestSelect::testTerraService ()
{
    try
    {
        ExecuteSelectServer(L"http://www.terraservice.net/ogccapabilities.ashx", L"DOQ");
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

// http://maps.york.ca/wmsconnector/com.esri.wms.Esrimap/WMS_RegOrtho?
void WmsTestSelect::testDefect786029()
{
    try
    {
        ExecuteSelectServer(L"http://maps.york.ca/wmsconnector/com.esri.wms.Esrimap/WMS_RegOrtho?;DefaultImageHeight=1024", L"Ortho"); // or "Roads"
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

// http://eusoils.jrc.it/wmsconnector/com.esri.wms.Esrimap/OCTOP_WMS?
void WmsTestSelect::testEusoils()
{
    try
    {
        ExecuteSelectServer(L"http://eusoils.jrc.it/wmsconnector/com.esri.wms.Esrimap/OCTOP_WMS?", L"3");
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

// http://eusoils.jrc.it:80/wmsconnector/com.esri.wms.Esrimap/PESERA?
void WmsTestSelect::testEusoilsPesera()
{
    try
    {
        ExecuteSelectServer(L"http://eusoils.jrc.it:80/wmsconnector/com.esri.wms.Esrimap/PESERA?", L"1");
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

// http://eusoils.jrc.it/wmsconnector/com.esri.wms.Esrimap/Meusis_alps?
void WmsTestSelect::testEusoilsMeusis()
{
    try
    {
        ExecuteSelectServer(L"http://eusoils.jrc.it/wmsconnector/com.esri.wms.Esrimap/Meusis_alps?", L"2");
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}


void WmsTestSelect::ExecuteSelectServer(FdoString* server, FdoString* classname)
{
    FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

    FdoStringP sServer = FdoStringP::Format(L"FeatureServer=%ls", server);
    connection->SetConnectionString((FdoString*)sServer);
    FdoConnectionState state = connection->Open ();

    FdoPtr<FdoIDescribeSchema> cmdDS = static_cast<FdoIDescribeSchema *> (connection->CreateCommand (FdoCommandType_DescribeSchema));
    FdoPtr<FdoFeatureSchemaCollection> schemas = cmdDS->Execute ();
    FdoInt32 cntSchemas = schemas->GetCount ();
    CPPUNIT_ASSERT (cntSchemas > 0);
    FdoPtr<FdoFeatureSchema> schema = schemas->GetItem (0);
    FdoPtr<FdoClassCollection> classes = schema->GetClasses ();
    FdoInt32 cntClasses = classes->GetCount ();
    CPPUNIT_ASSERT (cntClasses > 0);

    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*> (connection->CreateCommand (FdoCommandType_Select));
    cmdSelect->SetFeatureClassName (classname);
    FdoPtr<FdoIFeatureReader> featReader = cmdSelect->Execute ();
    CPPUNIT_ASSERT (featReader->ReadNext ());	    
    FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Raster");

    FdoPtr<FdoByteArray> gba = raster->GetBounds ();
    FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance ();
    FdoPtr<FdoIGeometry> geom = factory->CreateGeometryFromFgf (gba.p);
    FdoPtr<FdoIEnvelope> enve = geom->GetEnvelope ();

#ifdef _DEBUG
    wprintf (L"Extent: (%f,%f %f,%f)\n", enve->GetMinX(), enve->GetMinY(), enve->GetMaxX(), enve->GetMaxY());                
#endif

    FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();
    FdoIStreamReader* streamReader = raster->GetStreamReader ();
    FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (streamReader);

    CPPUNIT_ASSERT (streamReader != NULL);
    CPPUNIT_ASSERT (byteStreamReader != NULL);

    FdoByte buff[4096];
    FdoInt64 cntTotal = 0;
    FdoInt32 cntRead = 0;
    do
    {
        cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
        cntTotal += cntRead;
    }
    while (cntRead);

    CPPUNIT_ASSERT (cntTotal > 0);
    CPPUNIT_ASSERT (!featReader->ReadNext ());

    connection->Close ();
}

void WmsTestSelect::testSelectSpatialExtents()
{
	try 
	{
        FdoPtr<FdoISelectAggregates> selAggr;
        FdoPtr<FdoIDataReader> rdr;
        FdoPtr<FdoIdentifierCollection> ids;
        FdoPtr<FdoExpression> expr;
        FdoPtr<FdoComputedIdentifier> cid;
        int count = 0;

		FdoPtr<FdoIConnection> conn = WmsTests::GetConnection();
		conn->SetConnectionString(L"FeatureServer=http://wms.jpl.nasa.gov/wms.cgi?");
		CPPUNIT_ASSERT(conn->Open() == FdoConnectionState_Open);

        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
		
        FdoPtr<FdoISelectAggregates> advsel = (FdoISelectAggregates*)(conn->CreateCommand(FdoCommandType_SelectAggregates));
	    advsel->SetFeatureClassName(L"us_landsat_wgs84");
	    ids = advsel->GetPropertyNames();
	    expr = FdoExpression::Parse(L"SpatialExtents(Raster)");
	    cid = FdoComputedIdentifier::Create(L"MBR", expr);
	    ids->Add(cid);
	    rdr = advsel->Execute();

	    count = 0;
	    FdoPtr<FdoIEnvelope> envelopeAllWithoutFilter;
	    while (rdr->ReadNext())
	    {
		    if ( rdr->IsNull(L"MBR") )
			    continue;

		    FdoPtr<FdoByteArray> geomBytes = rdr->GetGeometry(L"MBR");
		    FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromFgf(geomBytes);
		    envelopeAllWithoutFilter = geom->GetEnvelope();

		    if (envelopeAllWithoutFilter->GetIsEmpty())
			    CPPUNIT_FAIL("Expected non-empty envelope for SpatialExtents() result");

		    count++;
	    }
	    CPPUNIT_ASSERT_MESSAGE("Expected exactly one row of aggregate data", count==1);

	    rdr->Close();

	    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MinX)", fabs(envelopeAllWithoutFilter->GetMinX() - (-127.00)) < 0.0000001);
	    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MinY)", fabs(envelopeAllWithoutFilter->GetMinY() - (23.00)) < 0.0000001);
	    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MaxX)", fabs(envelopeAllWithoutFilter->GetMaxX() - (-66.00)) < 0.0000001);
	    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MaxY)", fabs(envelopeAllWithoutFilter->GetMaxY() - (50.00)) < 0.0000001);

        conn->Close();

        /////////////////////////

	    FdoIoFileStreamP fileStream = FdoIoFileStream::Create(L"NASA_WMS_Config.xml", L"r");
	    conn->SetConnectionString (L"FeatureServer=http://wms.jpl.nasa.gov/wms.cgi?");
	    conn->SetConfiguration (fileStream);
		CPPUNIT_ASSERT(conn->Open() == FdoConnectionState_Open);

        advsel = (FdoISelectAggregates*)(conn->CreateCommand(FdoCommandType_SelectAggregates));
	    advsel->SetFeatureClassName(L"global_mosaic");
	    ids = advsel->GetPropertyNames();
	    expr = FdoExpression::Parse(L"SpatialExtents(Image)");
	    cid = FdoComputedIdentifier::Create(L"MBR", expr);
	    ids->Add(cid);
	    rdr = advsel->Execute();

	    count = 0;
	    while (rdr->ReadNext())
	    {
		    if ( rdr->IsNull(L"MBR") )
			    continue;

		    FdoPtr<FdoByteArray> geomBytes = rdr->GetGeometry(L"MBR");
		    FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromFgf(geomBytes);
		    envelopeAllWithoutFilter = geom->GetEnvelope();

		    if (envelopeAllWithoutFilter->GetIsEmpty())
			    CPPUNIT_FAIL("Expected non-empty envelope for SpatialExtents() result");

		    count++;
	    }
	    CPPUNIT_ASSERT_MESSAGE("Expected exactly one row of aggregate data", count==1);

	    rdr->Close();

	    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MinX)", fabs(envelopeAllWithoutFilter->GetMinX() - (-180.00)) < 0.0000001);
	    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MinY)", fabs(envelopeAllWithoutFilter->GetMinY() - (-60.00)) < 0.0000001);
	    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MaxX)", fabs(envelopeAllWithoutFilter->GetMaxX() - (180.00)) < 0.0000001);
	    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MaxY)", fabs(envelopeAllWithoutFilter->GetMaxY() - (84.00)) < 0.0000001);


        //////////////////////////

	    advsel->SetFeatureClassName(L"Itasca_Demo:global_mosaic");
	    rdr = advsel->Execute();

	    count = 0;
	    while (rdr->ReadNext())
	    {
		    if ( rdr->IsNull(L"MBR") )
			    continue;

		    FdoPtr<FdoByteArray> geomBytes = rdr->GetGeometry(L"MBR");
		    FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromFgf(geomBytes);
		    envelopeAllWithoutFilter = geom->GetEnvelope();

		    if (envelopeAllWithoutFilter->GetIsEmpty())
			    CPPUNIT_FAIL("Expected non-empty envelope for SpatialExtents() result");

		    count++;
	    }
	    CPPUNIT_ASSERT_MESSAGE("Expected exactly one row of aggregate data", count==1);

	    rdr->Close();

	    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MinX)", fabs(envelopeAllWithoutFilter->GetMinX() - (-180.00)) < 0.0000001);
	    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MinY)", fabs(envelopeAllWithoutFilter->GetMinY() - (-60.00)) < 0.0000001);
	    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MaxX)", fabs(envelopeAllWithoutFilter->GetMaxX() - (180.00)) < 0.0000001);
	    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MaxY)", fabs(envelopeAllWithoutFilter->GetMaxY() - (84.00)) < 0.0000001);

        //////////////////////////

        bool failed = false;
        try
        {
            advsel = (FdoISelectAggregates*)(conn->CreateCommand(FdoCommandType_SelectAggregates));
            advsel->SetFeatureClassName(L"Itasca_Demo:foo");
	        ids = advsel->GetPropertyNames();
	        expr = FdoExpression::Parse(L"SpatialExtents(Image)");
	        cid = FdoComputedIdentifier::Create(L"MBR", expr);
	        ids->Add(cid);
	        rdr = advsel->Execute();
        }
        catch ( FdoException *ex )
        {
            ex->Release();
            failed = true;
        }

        if (!failed) {
            CPPUNIT_FAIL("FAILED - SelectAggregates allowed accessing an invalid class\n");
        }

       //////////////////////////

        failed = false;
        try
        {
            advsel = (FdoISelectAggregates*)(conn->CreateCommand(FdoCommandType_SelectAggregates));
            advsel->SetFeatureClassName(L"foo:global_mosaic");
	        ids = advsel->GetPropertyNames();
	        expr = FdoExpression::Parse(L"SpatialExtents(Image)");
	        cid = FdoComputedIdentifier::Create(L"MBR", expr);
	        ids->Add(cid);
	        rdr = advsel->Execute();
        }
        catch ( FdoException *ex )
        {
            ex->Release();
            failed = true;
        }

        if (!failed) {
            CPPUNIT_FAIL("FAILED - SelectAggregates allowed accessing an invalid schema\n");
        }

        //////////////////////////

        failed = false;
        try
        {
            advsel = (FdoISelectAggregates*)(conn->CreateCommand(FdoCommandType_SelectAggregates));
            advsel->SetFeatureClassName(L"Itasca_Demo:global_mosaic");
	        ids = advsel->GetPropertyNames();
	        expr = FdoExpression::Parse(L"SpatialExtents(foo)");
	        cid = FdoComputedIdentifier::Create(L"MBR", expr);
	        ids->Add(cid);
	        rdr = advsel->Execute();
        }
        catch ( FdoException *ex )
        {
            ex->Release();
            failed = true;
        }

        if (!failed) {
            CPPUNIT_FAIL("FAILED - SelectAggregates allowed accessing an invalid property\n");
        }

        //////////////////////////

        failed = false;
        try
        {
            advsel = (FdoISelectAggregates*)(conn->CreateCommand(FdoCommandType_SelectAggregates));
            advsel->SetFeatureClassName(L"Itasca_Demo:global_mosaic");
	        ids = advsel->GetPropertyNames();
	        expr = FdoExpression::Parse(L"foo(Image)");
	        cid = FdoComputedIdentifier::Create(L"MBR", expr);
	        ids->Add(cid);
	        rdr = advsel->Execute();
        }
        catch ( FdoException *ex )
        {
            ex->Release();
            failed = true;
        }

        if (!failed) {
            CPPUNIT_FAIL("FAILED - SelectAggregates allowed accessing an invalid function\n");
        }

        //////////////////////////

        failed = false;
        try
        {
            advsel = (FdoISelectAggregates*)(conn->CreateCommand(FdoCommandType_SelectAggregates));
            advsel->SetFeatureClassName(L"Itasca_Demo:global_mosaic");
	        ids = advsel->GetPropertyNames();
	        expr = FdoExpression::Parse(L"SpatialExtents(Image)");
            advsel->SetFilter(L"FeatId > -1");
	        cid = FdoComputedIdentifier::Create(L"MBR", expr);
	        ids->Add(cid);
	        rdr = advsel->Execute();
        }
        catch ( FdoException *ex )
        {
            ex->Release();
            failed = true;
        }

        if (!failed) {
            CPPUNIT_FAIL("FAILED - SelectAggregates allowed the use of a conditional filter\n");
        }
    }
	catch( FdoException *ex )
    {
        fail(ex);
    }
}

// test against http://terraservice.net/ogccapabilities.ashx. This site uses
// different URLs for "GetCapabilities" and "GetMap" operations.
void WmsTestSelect::testTerraServiceThumbnail ()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        FdoStringP sServer = FdoStringP::Format(L"FeatureServer=http://www.terraservice.net/ogccapabilities.ashx");
        connection->SetConnectionString((FdoString*)sServer);
        FdoConnectionState state = connection->Open ();

        FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*> (connection->CreateCommand (FdoCommandType_Select));
        cmdSelect->SetFeatureClassName (L"DOQ");
        FdoPtr<FdoIFeatureReader> featReader = cmdSelect->Execute ();
        CPPUNIT_ASSERT (featReader->ReadNext ());	    
        FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Raster");

        raster->SetImageXSize(16);
        raster->SetImageYSize(16);

        FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (raster->GetStreamReader ());

        FdoByte buff[4096];
        FdoInt64 cntTotal = 0;
        FdoInt32 cntRead = 0;
        do
        {
            cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
            cntTotal += cntRead;
        }
        while (cntRead);

        CPPUNIT_ASSERT (cntTotal > 0);
        CPPUNIT_ASSERT (!featReader->ReadNext ());

        connection->Close ();
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

//http://142.176.62.108/cgi-bin/mapserv.exe?map=D:\ms441oci\maps\NS_TOPO_1000.map
void WmsTestSelect::testNS_TOPO_1000 ()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        FdoStringP sServer = FdoStringP::Format(L"FeatureServer=http://142.176.62.108/cgi-bin/mapserv.exe?map=D:\\ms441oci\\maps\\NS_TOPO_1000.map");
        connection->SetConnectionString((FdoString*)sServer);
        FdoConnectionState state = connection->Open ();

        FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*> (connection->CreateCommand (FdoCommandType_Select));
        cmdSelect->SetFeatureClassName (L"k1_BL_LINE");
        FdoPtr<FdoIFeatureReader> featReader = cmdSelect->Execute ();
        CPPUNIT_ASSERT (featReader->ReadNext ());	    
        FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Raster");

        raster->SetImageXSize(1024);
        raster->SetImageYSize(1024);

        FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (raster->GetStreamReader ());

        FdoByte buff[4096];
        FdoInt64 cntTotal = 0;
        FdoInt32 cntRead = 0;
        do
        {
            cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
            cntTotal += cntRead;
        }
        while (cntRead);

        CPPUNIT_ASSERT (cntTotal > 0);
        CPPUNIT_ASSERT (!featReader->ReadNext ());

        connection->Close ();
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

//http://142.176.62.108/cgi-bin/mapserv.exe?map=D:\ms441oci\maps\NS_TOPO_5000.map
void WmsTestSelect::testNS_TOPO_5000 ()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        FdoStringP sServer = FdoStringP::Format(L"FeatureServer=http://142.176.62.108/cgi-bin/mapserv.exe?map=D:\\ms441oci\\maps\\NS_TOPO_5000.map");
        connection->SetConnectionString((FdoString*)sServer);
        FdoConnectionState state = connection->Open ();

        FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*> (connection->CreateCommand (FdoCommandType_Select));
        cmdSelect->SetFeatureClassName (L"NS_TOPO_5000.K5_DL_LINE");
        FdoPtr<FdoIFeatureReader> featReader = cmdSelect->Execute ();
        CPPUNIT_ASSERT (featReader->ReadNext ());	    
        FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Raster");

        raster->SetImageXSize(1024);
        raster->SetImageYSize(1024);

        FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (raster->GetStreamReader ());

        FdoByte buff[4096];
        FdoInt64 cntTotal = 0;
        FdoInt32 cntRead = 0;
        do
        {
            cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
            cntTotal += cntRead;
        }
        while (cntRead);

        CPPUNIT_ASSERT (cntTotal > 0);
        CPPUNIT_ASSERT (!featReader->ReadNext ());

        connection->Close ();
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

//http://142.176.62.108/cgi-bin/mapserv.exe?map=D:\\ms441oci\\maps\\NS_CRS.map
void WmsTestSelect::testNS_CRS ()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        FdoStringP sServer = FdoStringP::Format(L"FeatureServer=http://142.176.62.108/cgi-bin/mapserv.exe?map=D:\\ms441oci\\maps\\NS_CRS.map&");
        connection->SetConnectionString((FdoString*)sServer);
        FdoConnectionState state = connection->Open ();

        FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*> (connection->CreateCommand (FdoCommandType_Select));
        cmdSelect->SetFeatureClassName (L"NSHPN_TEXT");
        FdoPtr<FdoIFeatureReader> featReader = cmdSelect->Execute ();
        CPPUNIT_ASSERT (featReader->ReadNext ());	    
        FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Raster");

        raster->SetImageXSize(1024);
        raster->SetImageYSize(1024);

        FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (raster->GetStreamReader ());

        FdoByte buff[4096];
        FdoInt64 cntTotal = 0;
        FdoInt32 cntRead = 0;
        do
        {
            cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
            cntTotal += cntRead;
        }
        while (cntRead);

        CPPUNIT_ASSERT (cntTotal > 0);
        CPPUNIT_ASSERT (!featReader->ReadNext ());

        connection->Close ();
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

//http://mapconnect.ga.gov.au/wmsconnector/com.esri.wms.Esrimap?Servicename=GDA94_MapConnect_SDE_250kmap_WMS
void WmsTestSelect::testMapConnect_SDE ()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        FdoStringP sServer = FdoStringP::Format(L"FeatureServer=http://mapconnect.ga.gov.au/wmsconnector/com.esri.wms.Esrimap?Servicename=GDA94_MapConnect_SDE_250kmap_WMS");
        connection->SetConnectionString((FdoString*)sServer);
        FdoConnectionState state = connection->Open ();

        FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*> (connection->CreateCommand (FdoCommandType_Select));
        cmdSelect->SetFeatureClassName (L"Mainlands");
        FdoPtr<FdoIFeatureReader> featReader = cmdSelect->Execute ();
        CPPUNIT_ASSERT (featReader->ReadNext ());	    
        FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Raster");

        raster->SetImageXSize(1024);
        raster->SetImageYSize(1024);

        FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (raster->GetStreamReader ());

        FdoByte buff[4096];
        FdoInt64 cntTotal = 0;
        FdoInt32 cntRead = 0;
        do
        {
            cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
            cntTotal += cntRead;
        }
        while (cntRead);

        CPPUNIT_ASSERT (cntTotal > 0);
        CPPUNIT_ASSERT (!featReader->ReadNext ());

        connection->Close ();
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

//http://ceoware2.ccrs.nrcan.gc.ca/cubewerx/cubeserv/cubeserv.cgi 
void WmsTestSelect::testCeoware2 ()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        FdoStringP sServer = FdoStringP::Format(L"FeatureServer=http://ceoware2.ccrs.nrcan.gc.ca/cubewerx/cubeserv/cubeserv.cgi");
        connection->SetConnectionString((FdoString*)sServer);
        FdoConnectionState state = connection->Open ();

        FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*> (connection->CreateCommand (FdoCommandType_Select));
        cmdSelect->SetFeatureClassName (L"GRFM_AFR_1C CEOWARE2");
        FdoPtr<FdoIFeatureReader> featReader = cmdSelect->Execute ();
        CPPUNIT_ASSERT (featReader->ReadNext ());	    
        FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Raster");

        raster->SetImageXSize(1024);
        raster->SetImageYSize(1024);

        FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (raster->GetStreamReader ());

        FdoByte buff[4096];
        FdoInt64 cntTotal = 0;
        FdoInt32 cntRead = 0;
        do
        {
            cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
            cntTotal += cntRead;
        }
        while (cntRead);

        CPPUNIT_ASSERT (cntTotal > 0);
        CPPUNIT_ASSERT (!featReader->ReadNext ());

        connection->Close ();
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

//http://lioib.lio.mnr.gov.on.ca/lioogc/default.asp?
void WmsTestSelect::testLioib ()
{
    bool failed = true;
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        FdoStringP sServer = FdoStringP::Format(L"FeatureServer=http://lioib.lio.mnr.gov.on.ca/lioogc/default.asp?");
        connection->SetConnectionString((FdoString*)sServer);
        FdoConnectionState state = connection->Open ();

        FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*> (connection->CreateCommand (FdoCommandType_Select));
        cmdSelect->SetFeatureClassName (L"OGC_Settlements");
        FdoPtr<FdoIFeatureReader> featReader = cmdSelect->Execute ();
        CPPUNIT_ASSERT (featReader->ReadNext ());	    
        FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Raster");

        raster->SetImageXSize(1024);
        raster->SetImageYSize(1024);

        FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (raster->GetStreamReader ());

        connection->Close ();

        failed = false;
    }
    catch (FdoException* e)
    {
        e->Release();
    }

    CPPUNIT_ASSERT (failed == true);	    
}

//http://kort.plandk.dk/scripts/mapserv.pl?service=wms
void WmsTestSelect::testKortPlandk ()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        FdoStringP sServer = FdoStringP::Format(L"FeatureServer=http://kort.plandk.dk/scripts/mapserv.pl?service=wms");
        connection->SetConnectionString((FdoString*)sServer);
        FdoConnectionState state = connection->Open ();

        FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*> (connection->CreateCommand (FdoCommandType_Select));
        cmdSelect->SetFeatureClassName (L"plandk_r");
        FdoPtr<FdoIFeatureReader> featReader = cmdSelect->Execute ();
        CPPUNIT_ASSERT (featReader->ReadNext ());	    
        FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Raster");

        raster->SetImageXSize(1024);
        raster->SetImageYSize(1024);

        FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (raster->GetStreamReader ());

        FdoByte buff[4096];
        FdoInt64 cntTotal = 0;
        FdoInt32 cntRead = 0;
        do
        {
            cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
            cntTotal += cntRead;
        }
        while (cntRead);

        CPPUNIT_ASSERT (cntTotal > 0);
        CPPUNIT_ASSERT (!featReader->ReadNext ());

        connection->Close ();
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

//http://libcwms.gov.bc.ca/wmsconnector/com.esri.wsit.WMSServlet/ogc_layer_service?version=1.1.1
void WmsTestSelect::testLibcwms ()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        FdoStringP sServer = FdoStringP::Format(L"FeatureServer=http://libcwms.gov.bc.ca/wmsconnector/com.esri.wsit.WMSServlet/ogc_layer_service?version=1.1.1");
        connection->SetConnectionString((FdoString*)sServer);
        FdoConnectionState state = connection->Open ();

        FdoPtr<FdoIDescribeSchema> cmdDS = static_cast<FdoIDescribeSchema *> (connection->CreateCommand (FdoCommandType_DescribeSchema));
        FdoPtr<FdoFeatureSchemaCollection> schemas = cmdDS->Execute ();
        FdoInt32 cntSchemas = schemas->GetCount ();
        CPPUNIT_ASSERT (cntSchemas == 1);

        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem (0);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();
        FdoPtr<FdoClassDefinition> clsDef = classes->GetItem (L"DBM_7H_MIL_BOUNDARIES_LINE");
        FdoFeatureClass* featClsDef = static_cast<FdoFeatureClass *> (clsDef.p);
        FdoPtr<FdoReadOnlyPropertyDefinitionCollection> props = clsDef->GetBaseProperties ();
        FdoPtr<FdoPropertyDefinition> prop = props->GetItem (L"Raster");
        FdoRasterPropertyDefinition* rasterProp = (FdoRasterPropertyDefinition*)(prop.p);
        FdoStringP rasterAssoc = rasterProp->GetSpatialContextAssociation();
        CPPUNIT_ASSERT (rasterAssoc == L"EPSG:4326");

        FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*> (connection->CreateCommand (FdoCommandType_Select));
        cmdSelect->SetFeatureClassName (L"DBM_7H_MIL_BOUNDARIES_LINE");
        FdoPtr<FdoIFeatureReader> featReader = cmdSelect->Execute ();
        CPPUNIT_ASSERT (featReader->ReadNext ());	    
        FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Raster");

        raster->SetImageXSize(1024);
        raster->SetImageYSize(1024);

        FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (raster->GetStreamReader ());

        FdoByte buff[4096];
        FdoInt64 cntTotal = 0;
        FdoInt32 cntRead = 0;
        do
        {
            cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
            cntTotal += cntRead;
        }
        while (cntRead);

        CPPUNIT_ASSERT (cntTotal > 0);
        CPPUNIT_ASSERT (!featReader->ReadNext ());

        connection->Close ();
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

//http://www.gis2.nrw.de/wmsconnector/wms/stobo?version=1.1.1
void WmsTestSelect::testStoboWms ()
{
    bool failed = false;

    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        FdoStringP sServer = FdoStringP::Format(L"FeatureServer=http://www.gis2.nrw.de/wmsconnector/wms/stobo?version=1.1.1");
        connection->SetConnectionString((FdoString*)sServer);
        FdoConnectionState state = connection->Open ();

        FdoPtr<FdoIDescribeSchema> cmdDS = static_cast<FdoIDescribeSchema *> (connection->CreateCommand (FdoCommandType_DescribeSchema));
        FdoPtr<FdoFeatureSchemaCollection> schemas = cmdDS->Execute ();
        FdoInt32 cntSchemas = schemas->GetCount ();
        CPPUNIT_ASSERT (cntSchemas == 1);

        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem (0);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();
        FdoPtr<FdoClassDefinition> clsDef = classes->GetItem (L"Bleibelastung der Ackerboeden in mg pro kg");
        FdoFeatureClass* featClsDef = static_cast<FdoFeatureClass *> (clsDef.p);
        FdoPtr<FdoReadOnlyPropertyDefinitionCollection> props = clsDef->GetBaseProperties ();
        FdoPtr<FdoPropertyDefinition> prop = props->GetItem (L"Raster");
        FdoRasterPropertyDefinition* rasterProp = (FdoRasterPropertyDefinition*)(prop.p);
        FdoStringP rasterAssoc = rasterProp->GetSpatialContextAssociation();
        CPPUNIT_ASSERT (rasterAssoc == L"EPSG:4326");

        FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*> (connection->CreateCommand (FdoCommandType_Select));
        cmdSelect->SetFeatureClassName (L"Bleibelastung der Ackerboeden in mg pro kg");
        FdoPtr<FdoIFeatureReader> featReader = cmdSelect->Execute ();
        CPPUNIT_ASSERT (featReader->ReadNext ());	    
        FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Raster");

        raster->SetImageXSize(1024);
        raster->SetImageYSize(1024);

        FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (raster->GetStreamReader ());

        FdoByte buff[4096];
        FdoInt64 cntTotal = 0;
        FdoInt32 cntRead = 0;
        do
        {
            cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
            cntTotal += cntRead;
        }
        while (cntRead);

        CPPUNIT_ASSERT (cntTotal > 0);
        CPPUNIT_ASSERT (!featReader->ReadNext ());

        connection->Close ();
    }
/*
    catch (FdoException* e)
    {
        fail(e);
    }
*/
    catch ( ... )
    {
        failed = true;
    }

    CPPUNIT_ASSERT_MESSAGE("test started working again", failed);
}

//http://linuxgurrl.agr.ca/cgi-bin/mapeco
void WmsTestSelect::testLinuxgurrl ()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        FdoStringP sServer = L"FeatureServer=http://linuxgurrl.agr.ca/cgi-bin/mapeco?version=1.1.1";
        connection->SetConnectionString((FdoString*)sServer);
        FdoConnectionState state = connection->Open ();

        FdoPtr<FdoIDescribeSchema> cmdDS = static_cast<FdoIDescribeSchema *> (connection->CreateCommand (FdoCommandType_DescribeSchema));
        FdoPtr<FdoFeatureSchemaCollection> schemas = cmdDS->Execute ();
        FdoInt32 cntSchemas = schemas->GetCount ();
        CPPUNIT_ASSERT (cntSchemas == 1);

        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem (0);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();
        FdoPtr<FdoClassDefinition> clsDef = classes->GetItem (L"ecodistricts");
        FdoFeatureClass* featClsDef = static_cast<FdoFeatureClass *> (clsDef.p);
        FdoPtr<FdoReadOnlyPropertyDefinitionCollection> props = clsDef->GetBaseProperties ();
        FdoPtr<FdoPropertyDefinition> prop = props->GetItem (L"Raster");
        FdoRasterPropertyDefinition* rasterProp = (FdoRasterPropertyDefinition*)(prop.p);
        FdoStringP rasterAssoc = rasterProp->GetSpatialContextAssociation();
        CPPUNIT_ASSERT (rasterAssoc == L"EPSG:4326");

        FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*> (connection->CreateCommand (FdoCommandType_Select));
        cmdSelect->SetFeatureClassName (L"ecodistricts-labels"); 
        FdoPtr<FdoIFeatureReader> featReader = cmdSelect->Execute ();
        CPPUNIT_ASSERT (featReader->ReadNext ());	    
        FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Raster");

        raster->SetImageXSize(1024);
        raster->SetImageYSize(1024);

        FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (raster->GetStreamReader ());

        FdoByte buff[4096];
        FdoInt64 cntTotal = 0;
        FdoInt32 cntRead = 0;
        do
        {
            cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
            cntTotal += cntRead;
        }
        while (cntRead);

        CPPUNIT_ASSERT (cntTotal > 0);
        CPPUNIT_ASSERT (!featReader->ReadNext ());

        connection->Close ();
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

//http://openmaps.gov.bc.ca/images/base.xml
void WmsTestSelect::testOpenmaps ()
{
    bool failed = false;

    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        FdoStringP sServer = L"FeatureServer=http://openmaps.gov.bc.ca/images/base.xml";
        connection->SetConnectionString((FdoString*)sServer);
        FdoConnectionState state = connection->Open ();

        FdoPtr<FdoIDescribeSchema> cmdDS = static_cast<FdoIDescribeSchema *> (connection->CreateCommand (FdoCommandType_DescribeSchema));
        FdoPtr<FdoFeatureSchemaCollection> schemas = cmdDS->Execute ();
        FdoInt32 cntSchemas = schemas->GetCount ();
        CPPUNIT_ASSERT (cntSchemas == 1);

        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem (0);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();
        FdoPtr<FdoClassDefinition> clsDef = classes->GetItem (L"BC_BC_BC_XC100M_BCALB_20070115");
        FdoFeatureClass* featClsDef = static_cast<FdoFeatureClass *> (clsDef.p);
        FdoPtr<FdoReadOnlyPropertyDefinitionCollection> props = clsDef->GetBaseProperties ();
        FdoPtr<FdoPropertyDefinition> prop = props->GetItem (L"Raster");
        FdoRasterPropertyDefinition* rasterProp = (FdoRasterPropertyDefinition*)(prop.p);
        FdoStringP rasterAssoc = rasterProp->GetSpatialContextAssociation();
        CPPUNIT_ASSERT (rasterAssoc == L"EPSG:4326");

        FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*> (connection->CreateCommand (FdoCommandType_Select));
        cmdSelect->SetFeatureClassName (L"BC_BC_BC_XC100M_BCALB_20070115");
        FdoPtr<FdoIFeatureReader> featReader = cmdSelect->Execute ();
        CPPUNIT_ASSERT (featReader->ReadNext ());	    
        FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Raster");

        raster->SetImageXSize(1024);
        raster->SetImageYSize(1024);

        FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (raster->GetStreamReader ());

        FdoByte buff[4096];
        FdoInt64 cntTotal = 0;
        FdoInt32 cntRead = 0;
        do
        {
            cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
            cntTotal += cntRead;
        }
        while (cntRead);

        CPPUNIT_ASSERT (cntTotal > 0);
        CPPUNIT_ASSERT (!featReader->ReadNext ());

        connection->Close ();
    }
/*
    catch (FdoException* e)
    {
        fail(e);
    }
*/
    catch ( ... )
    {
        failed = true;
    }

    CPPUNIT_ASSERT_MESSAGE("test started working again", failed);
}

void WmsTestSelect::testquestionmarkend ()
{
    bool failed = false;

    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        FdoStringP sServer = L"FeatureServer=http://www.karttakone.fi/wms/200f2fd3f687aa1f05f1fe5c47cf80af/ykj/rs06_suomi_5m?";
        connection->SetConnectionString((FdoString*)sServer);
        FdoConnectionState state = connection->Open ();

        FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(connection->CreateCommand (FdoCommandType_Select));
        cmdSelect->SetFeatureClassName(L"WMS_Schema:Karttakone");

        FdoPtr<FdoIFeatureReader> rasterReader = cmdSelect->Execute();
    }
/*
    catch (FdoException* e)
    {
        fail(e);
    }
*/
    catch ( ... )
    {
        failed = true;
    }

    CPPUNIT_ASSERT_MESSAGE("test started working again", failed);
}

//http://62.214.147.252:8080
void WmsTestSelect::testURLEcoding ()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        FdoStringP sServer = L"FeatureServer=http://62.214.147.252:8080/cgi-bin/mapserv.exe?map=C%3a%5cProgram+Files%5cms4w%5cApache%5chtdocs%5cBB2009%5cBB2009.map&version=1.1.1";
        connection->SetConnectionString((FdoString*)sServer);
        FdoConnectionState state = connection->Open ();

        FdoPtr<FdoIDescribeSchema> cmdDS = static_cast<FdoIDescribeSchema *> (connection->CreateCommand (FdoCommandType_DescribeSchema));
        FdoPtr<FdoFeatureSchemaCollection> schemas = cmdDS->Execute ();
        FdoInt32 cntSchemas = schemas->GetCount ();
        CPPUNIT_ASSERT (cntSchemas == 1);

        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem (0);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();
        FdoPtr<FdoClassDefinition> clsDef = classes->GetItem (L"Brandenburg2009");
        FdoFeatureClass* featClsDef = static_cast<FdoFeatureClass *> (clsDef.p);
        FdoPtr<FdoPropertyDefinitionCollection> props = clsDef->GetProperties ();
        FdoPtr<FdoPropertyDefinition> prop = props->GetItem (L"Raster");
        FdoRasterPropertyDefinition* rasterProp = (FdoRasterPropertyDefinition*)(prop.p);
        FdoStringP rasterAssoc = rasterProp->GetSpatialContextAssociation();
        CPPUNIT_ASSERT (rasterAssoc == L"EPSG:25833");

        FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*> (connection->CreateCommand (FdoCommandType_Select));
        cmdSelect->SetFeatureClassName (L"Brandenburg2009");
        FdoPtr<FdoIFeatureReader> featReader = cmdSelect->Execute ();
        CPPUNIT_ASSERT (featReader->ReadNext ());	    
        FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Raster");

        raster->SetImageXSize(1024);
        raster->SetImageYSize(1024);

        FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (raster->GetStreamReader ());

        FdoByte buff[4096];
        FdoInt64 cntTotal = 0;
        FdoInt32 cntRead = 0;
        do
        {
            cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
            cntTotal += cntRead;
        }
        while (cntRead);

        CPPUNIT_ASSERT (cntTotal > 0);
        CPPUNIT_ASSERT (!featReader->ReadNext ());

        connection->Close ();
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}
