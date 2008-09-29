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
#include "ConnectTests.h"

CPPUNIT_TEST_SUITE_REGISTRATION (ConnectTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (ConnectTests, "ConnectTests");


ConnectTests::ConnectTests (void)
{
}

ConnectTests::~ConnectTests (void)
{
}

void ConnectTests::TestConnection(FdoString* connString, bool expectedExc)
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        connection->SetConnectionString(connString);
        FdoConnectionState state = connection->Open ();
        connection->Close ();
    }
    catch (FdoException* e)
    {
        if (!expectedExc)
            fail(e);
        else
        {
            FdoStringP txt = e->GetExceptionMessage();
            e->Release();
            printf ("\n Expected exception: %s \n", (const char*)txt);
        }
    }
}

void ConnectTests::TestConnection1 ()
{
    TestConnection(L"FeatureServer=http://www.terraservice.net/ogccapabilities.ashx");
}

void ConnectTests::TestConnection2 ()
{
    TestConnection(L"FeatureServer=http://wms.jpl.nasa.gov/wms.cgi");
}

void ConnectTests::TestConnection3 ()
{
    TestConnection(L"FeatureServer=http://globe.digitalearth.gov/viz-bin/wmt.cgi");
}

void ConnectTests::TestConnection4 ()
{
    TestConnection(L"FeatureServer=http://demo.cubewerx.com/demo/cubeserv/cubeserv.cgi");
}

void ConnectTests::TestConnection5 ()
{
    TestConnection(L"FeatureServer=http://www.geographynetwork.com/servlet/com.esri.wms.Esrimap", true);
}

void ConnectTests::TestConnection6 ()
{
    TestConnection(L"FeatureServer=http://www.geographynetwork.ca/wmsconnector/com.esri.wms.Esrimap/Geobase_NRN_NewfoundlandAndLabrador_I_Detail");
}

void ConnectTests::TestConnection7 ()
{
    TestConnection(L"FeatureServer=http://maps1.intergraph.com/wms/world/request.asp");
}

// the following sites sometimes response very slow and they are 
// used to test "timeout" situations.
// http://terraservice.net/ogccapabilities.ashx?
// http://gis2.gov.ns.ca/servlet/com.esri.wms.Esrimap?
void ConnectTests::TestConnectionTimeout ()
{    
    TestConnection(L"FeatureServer=http://terraservice.net/ogccapabilities.ashx?");
    TestConnection(L"FeatureServer=http://gis2.gov.ns.ca/servlet/com.esri.wms.Esrimap?");
}

void ConnectTests::TestConnection8 ()
{
    TestConnection(L"FeatureServer=http://atlas.walis.wa.gov.au/servlet/com.esri.wms.Esrimap", true);
}

void ConnectTests::TestConnection9 ()
{
    TestConnection(L"FeatureServer=http://www.dottedeyes.com/wms");
}

void ConnectTests::TestConnection10 ()
{
    TestConnection(L"FeatureServer=http://data.mapguide.com/mapguide/mapagent/mapagent.fcgi");
}

void ConnectTests::TestConnection11 ()
{
    TestConnection(L"FeatureServer=http://cgdi-dev.geoconnections.org/cgi-bin/tomatlasmapper");
}

void ConnectTests::TestConnection12 ()
{
    TestConnection(L"FeatureServer=http://map.genimap.com/GenimapWMS/v1/GenimapWMS?cid=demo");
}

void ConnectTests::TestConnection13 ()
{
    TestConnection(L"FeatureServer=http://www.mapguide.com/mapguide/mapagent/mapagent.fcgi");
}

void ConnectTests::TestConnection14 ()
{
    TestConnection(L"FeatureServer=http://ceoware2.ccrs.nrcan.gc.ca/cuberwerx/cubeserv/cuberserv.cgi");
}
