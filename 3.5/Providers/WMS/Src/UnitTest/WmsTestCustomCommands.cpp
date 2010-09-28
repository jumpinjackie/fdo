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
#include "WmsTestCustomCommands.h"
#include "WMS/FdoWmsCommandType.h"
#include "WMS/IGetImageFormats.h"
#include "WMS/IGetFeatureClassStyles.h"
#include "WMS/IGetFeatureClassCRSNames.h"

CPPUNIT_TEST_SUITE_REGISTRATION( WmsTestCustomCommands );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( WmsTestCustomCommands, "WmsTestCustomCommands");

WmsTestCustomCommands::WmsTestCustomCommands ()
{
}

WmsTestCustomCommands::~WmsTestCustomCommands ()
{
}

//http://www2.dmsolutions.ca/cgi-bin/mswms_gmap
void WmsTestCustomCommands::testGetImageFormats ()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        FdoStringP sServer = FdoStringP::Format(L"FeatureServer=http://www2.dmsolutions.ca/cgi-bin/mswms_gmap");
        connection->SetConnectionString((FdoString*)sServer);
        FdoConnectionState state = connection->Open ();
        
        //test for formats command
        FdoPtr<FdoWmsIGetImagefromats> cmdGIF = static_cast<FdoWmsIGetImagefromats *> (connection->CreateCommand(FdoWmsCommandType_GetImageFormats));
        FdoStringsP formats = cmdGIF->Execute ();
        CPPUNIT_ASSERT(formats->GetCount() == 7);
#ifdef _DEBUG
		for (int i = 0; i < formats->GetCount(); ++i)
		{
			wprintf (L"image format: %ls\n", formats->GetString(i));
		}
#endif
        connection->Close ();
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

void WmsTestCustomCommands::testGetFeatureStyles ()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        FdoStringP sServer = FdoStringP::Format(L"FeatureServer=http://www2.dmsolutions.ca/cgi-bin/mswms_gmap");
        connection->SetConnectionString((FdoString*)sServer);
        FdoConnectionState state = connection->Open ();
        
        //test for get styles command
        FdoPtr<FdoWmsIGetFeatureClassStyles> cmdGLS = static_cast<FdoWmsIGetFeatureClassStyles *> (connection->CreateCommand(FdoWmsCommandType_GetFeatureClassStyles));
        cmdGLS->SetFeatureClassName(L"drainage");
        FdoStringsP styles = cmdGLS->Execute();
        CPPUNIT_ASSERT(styles->GetCount() == 1);
#ifdef _DEBUG
		for (int i = 0; i < styles->GetCount(); ++i)
		{
			wprintf (L"Layer Style: %ls\n", styles->GetString(i));
		}
#endif
		
        connection->Close ();
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

void WmsTestCustomCommands::testSpacialFeatureClassName ()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        FdoStringP sServer = FdoStringP::Format(L"FeatureServer=http://demo.cubewerx.com/demo/cubeserv/cubeserv.cgi?version=1.1.0");
        connection->SetConnectionString((FdoString*)sServer);
        FdoConnectionState state = connection->Open ();
        
        //test for get styles command
        FdoPtr<FdoWmsIGetFeatureClassStyles> cmdGLS = static_cast<FdoWmsIGetFeatureClassStyles *> (connection->CreateCommand(FdoWmsCommandType_GetFeatureClassStyles));
        cmdGLS->SetFeatureClassName(L"BARRIERL_1M Foundation"); // layer name is BARRIERL_1M:Foundation
        FdoStringsP styles = cmdGLS->Execute();
        CPPUNIT_ASSERT(styles->GetCount() == 1);
#ifdef _DEBUG
		for (int i = 0; i < styles->GetCount(); ++i)
		{
			wprintf (L"Layer Style: %ls\n", styles->GetString(i));
		}
#endif
        //test for get CRS command
        FdoPtr<FdoWmsIGetFeatureClassCRSNames> cmdGLCRS = static_cast<FdoWmsIGetFeatureClassCRSNames *> (connection->CreateCommand(FdoWmsCommandType_GetFeatureClassCRSNames));
        cmdGLCRS->SetFeatureClassName(L"BARRIERL_1M Foundation");//layer name is BARRIERL_1M:Foundation
        FdoStringsP crsNames = cmdGLCRS->Execute();
        CPPUNIT_ASSERT(crsNames->GetCount() == 1);
#ifdef _DEBUG
		for (int i = 0; i < crsNames->GetCount(); ++i)
		{
			wprintf (L"CRS Name: %ls\n", crsNames->GetString(i));
		}
#endif		

        connection->Close ();
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

void WmsTestCustomCommands::testGetFeatureCRSNames ()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        FdoStringP sServer = FdoStringP::Format(L"FeatureServer=http://www2.dmsolutions.ca/cgi-bin/mswms_gmap");
        connection->SetConnectionString((FdoString*)sServer);
        FdoConnectionState state = connection->Open ();
        
        //test for get CRS command
        FdoPtr<FdoWmsIGetFeatureClassCRSNames> cmdGLCRS = static_cast<FdoWmsIGetFeatureClassCRSNames *> (connection->CreateCommand(FdoWmsCommandType_GetFeatureClassCRSNames));
        cmdGLCRS->SetFeatureClassName(L"drainage");
        FdoStringsP crsNames = cmdGLCRS->Execute();
        CPPUNIT_ASSERT(crsNames->GetCount() == 2);
#ifdef _DEBUG
		for (int i = 0; i < crsNames->GetCount(); ++i)
		{
			wprintf (L"CRS Name: %ls\n", crsNames->GetString(i));
		}
#endif		
        connection->Close ();
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

void WmsTestCustomCommands::testInvalidParamForStyle ()
{
    FdoPtr<FdoIConnection> connection;
    try
    {
		connection = WmsTests::GetConnection ();

        FdoStringP sServer = FdoStringP::Format(L"FeatureServer=http://www2.dmsolutions.ca/cgi-bin/mswms_gmap");
        connection->SetConnectionString((FdoString*)sServer);
        FdoConnectionState state = connection->Open ();
        
        //test for get styles command
        FdoPtr<FdoWmsIGetFeatureClassStyles> cmdGLS = static_cast<FdoWmsIGetFeatureClassStyles *> (connection->CreateCommand(FdoWmsCommandType_GetFeatureClassStyles));
        FdoStringsP styles = cmdGLS->Execute();
		
        connection->Close ();

		std::string errorMsg;
		errorMsg.append("Fail to catch none feature class input error!");
		CPPUNIT_FAIL(errorMsg);
    }
    catch (FdoException* e)
    {
        e->Release();
        connection->Close ();
    }
}

void WmsTestCustomCommands::testInvalidParamForCRS ()
{
    FdoPtr<FdoIConnection> connection;
    try
    {
		connection = WmsTests::GetConnection ();
        FdoStringP sServer = FdoStringP::Format(L"FeatureServer=http://www2.dmsolutions.ca/cgi-bin/mswms_gmap");
        connection->SetConnectionString((FdoString*)sServer);
        FdoConnectionState state = connection->Open ();
        
        //test for get CRS command
        FdoPtr<FdoWmsIGetFeatureClassCRSNames> cmdGLCRS = static_cast<FdoWmsIGetFeatureClassCRSNames *> (connection->CreateCommand(FdoWmsCommandType_GetFeatureClassCRSNames));
        FdoStringsP crsNames = cmdGLCRS->Execute();
		
        connection->Close ();

		std::string errorMsg;
		errorMsg.append("Fail to catch none feature class input error!");
		CPPUNIT_FAIL(errorMsg);
    }
    catch (FdoException* e)
    {
        e->Release();
		connection->Close ();
    }
}
