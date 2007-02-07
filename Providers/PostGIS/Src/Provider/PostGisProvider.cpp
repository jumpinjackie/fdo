//
// Copyright (C) 2007 Refractions Research, Inc. 
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#include "stdafx.h"
#include "PostGisProvider.h"

// TODO: Implement 'home dir' features

///////////////////////////////////////////////////////////////////////////////
// PostGisProvider DLL Entry Point
///////////////////////////////////////////////////////////////////////////////

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// Definition of various properties used by PostGIS Provier
///////////////////////////////////////////////////////////////////////////////

namespace fdo { namespace postgis {

//
// General descriptive information about PostGIS Provier
//
FdoString* ProviderName = L"OSGeo.PostGIS.1.0";
FdoString* ProviderVersion = L"1.0.0.0";
FdoString* FdoVersion = L"3.2.0.0";

char const* ProviderDisplayName = "OSGeo FDO Provider for PostGIS";
char const* ProviderDescription = "Read/write access to PostGIS-based data store. Supports spatial data types and spatial query operations.";

//
// Connection properties defined for PostGIS Provier
//

FdoString* PropertyUsername = L"Username";
FdoString* PropertyPassword = L"Password";
FdoString* PropertyService = L"Service";
FdoString* PropertyDatastore = L"Datastore";

FdoString* PropertyDefaultHostname = L"localhost";
FdoString* PropertyDefaultPort = L"5432"; // DEF_PGPORT_STR

}} // namespace fdo::postgis
