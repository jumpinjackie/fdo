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

void ConnectTests::TestConnection1 ()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        connection->SetConnectionString(L"FeatureServer=http://www.terraservice.net/ogccapabilities.ashx");
        FdoConnectionState state = connection->Open ();
        connection->Close ();
    }
    catch (FdoException* e)
    {
        CPPUNIT_FAIL((const char*)FdoStringP(e->GetExceptionMessage()));
    }
}

void ConnectTests::TestConnection2 ()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        connection->SetConnectionString(L"FeatureServer=http://wms.jpl.nasa.gov/wms.cgi");
        FdoConnectionState state = connection->Open ();
        connection->Close ();
    }
    catch (FdoException* e)
    {
        CPPUNIT_FAIL((const char*)FdoStringP(e->GetExceptionMessage()));
    }
}

void ConnectTests::TestConnection3 ()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        connection->SetConnectionString(L"FeatureServer=http://globe.digitalearth.gov/viz-bin/wmt.cgi");
        FdoConnectionState state = connection->Open ();
        connection->Close ();
    }
    catch (FdoException* e)
    {
        CPPUNIT_FAIL((const char*)FdoStringP(e->GetExceptionMessage()));
    }
}

void ConnectTests::TestConnection4 ()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        connection->SetConnectionString(L"FeatureServer=http://demo.cubewerx.com/demo/cubeserv/cubeserv.cgi");
        FdoConnectionState state = connection->Open ();
        connection->Close ();
    }
    catch (FdoException* e)
    {
        CPPUNIT_FAIL((const char*)FdoStringP(e->GetExceptionMessage()));
    }
}

void ConnectTests::TestConnection5 ()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        connection->SetConnectionString(L"FeatureServer=http://www.geographynetwork.com/servlet/com.esri.wms.Esrimap");
        FdoConnectionState state = connection->Open ();
        connection->Close ();
    }
    catch (FdoException* e)
    {
        CPPUNIT_FAIL((const char*)FdoStringP(e->GetExceptionMessage()));
    }
}

void ConnectTests::TestConnection6 ()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        connection->SetConnectionString(L"FeatureServer=http://www.geographynetwork.ca/wmsconnector/com.esri.wms.Esrimap/Geobase_NRN_NewfoundlandAndLabrador_I_Detail");
        FdoConnectionState state = connection->Open ();
        connection->Close ();
    }
    catch (FdoException* e)
    {
        CPPUNIT_FAIL((const char*)FdoStringP(e->GetExceptionMessage()));
    }
}

void ConnectTests::TestConnection7 ()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        connection->SetConnectionString(L"FeatureServer=http://maps1.intergraph.com/wms/world/request.asp");
        FdoConnectionState state = connection->Open ();
        connection->Close ();
    }
    catch (FdoException* e)
    {
        CPPUNIT_FAIL((const char*)FdoStringP(e->GetExceptionMessage()));
    }
}

// the following sites sometimes response very slow and they are 
// used to test "timeout" situations.
// http://terraservice.net/ogccapabilities.ashx?
// http://gis2.gov.ns.ca/servlet/com.esri.wms.Esrimap?
void ConnectTests::TestConnectionTimeout ()
{    
    FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();
    try
    {
        connection->SetConnectionString(L"FeatureServer=http://terraservice.net/ogccapabilities.ashx?");
        FdoConnectionState state = connection->Open ();
        connection->Close ();
    }
    catch (FdoException* e)
    {
#ifdef _DEBUG
        wprintf(L"Error: %ls\n", e->GetExceptionMessage());
#endif
        e->Release ();        
    }

    try
    {
        connection->SetConnectionString(L"FeatureServer=http://gis2.gov.ns.ca/servlet/com.esri.wms.Esrimap?");
        FdoConnectionState state = connection->Open ();
        connection->Close ();
    }
    catch (FdoException* e)
    {
#ifdef _DEBUG
        wprintf(L"Error: %ls\n", e->GetExceptionMessage());
#endif
        e->Release ();        
    }
}

