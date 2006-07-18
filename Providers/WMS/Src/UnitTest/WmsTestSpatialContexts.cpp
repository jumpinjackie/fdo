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
#include "WmsTestSpatialContexts.h"
#include <iostream>
#include <iomanip>

CPPUNIT_TEST_SUITE_REGISTRATION (WmsTestSpatialContext );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (WmsTestSpatialContext, "WmsTestSpatialContext");

WmsTestSpatialContext::WmsTestSpatialContext ()
{
}

WmsTestSpatialContext::~WmsTestSpatialContext ()
{
}

#ifdef _FDO_RUN_INTERNAL_TEST
void WmsTestSpatialContext::testServer1 ()
{
	try 
	{
		testServer (L"http://cadc-isd-jake.ads.autodesk.com/cgi-bin/mapserv.exe?map=wms/wms.map&", 2);
	}
    catch (FdoException* e)
    {
        CPPUNIT_FAIL((const char*)FdoStringP(e->GetExceptionMessage()));
    }
}
#endif

void WmsTestSpatialContext::testServer2 ()
{
	try 
	{
		testServer (L"http://www.bsc-eoc.org/cgi-bin/bsc_ows.asp", 1);
	}
    catch (FdoException* e)
    {
        CPPUNIT_FAIL((const char*)FdoStringP(e->GetExceptionMessage()));
    }
}

void WmsTestSpatialContext::testServer (FdoString* featureServer, FdoInt32 cntSpatialContexts)
{
	FdoPtr<FdoIConnection> conn = this->GetConnection ();
	FdoStringP connString = FdoStringP(L"FeatureServer=") + featureServer;
	conn->SetConnectionString (connString);
	CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

	FdoPtr<FdoIGetSpatialContexts> cmd = static_cast<FdoIGetSpatialContexts *> (conn->CreateCommand (FdoCommandType_GetSpatialContexts));
	FdoPtr<FdoISpatialContextReader> reader = cmd->Execute ();
	FdoInt32 cntRead = 0;

	while (reader->ReadNext ())
	{
#ifdef _DEBUG
		wprintf (L"Name: %ls\n", reader->GetName ());
		wprintf (L"CRS:  %ls\n", reader->GetCoordinateSystem ());
		reader->GetExtent ();

		FdoPtr<FdoByteArray> gba = reader->GetExtent ();
		FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
		FdoPtr<FdoIGeometry> polygon = gf->CreateGeometryFromFgf(gba);
		FdoPtr<FdoIEnvelope> extent = polygon->GetEnvelope();

		std::wcout << std::setprecision (16);
		std::wcout << L"MinX: " << extent->GetMinX() <<std::endl 
					<< L"MinY: " << extent->GetMinY() << std::endl
					<< L"MaxX: " << extent->GetMaxX() << std::endl
					<< L"MaxY: " << extent->GetMaxY() << std::endl
					<< std::endl;

#endif

		cntRead++;
	}

	CPPUNIT_ASSERT (cntRead == cntSpatialContexts);
}

