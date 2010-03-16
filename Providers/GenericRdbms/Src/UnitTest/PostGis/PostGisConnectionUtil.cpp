/*
 * Copyright (C) 2006 Refractions Research, Inc.
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
#include "PostGisConnectionUtil.h"
#include "PostGisSchemaOverrideUtil.h"
#include "UnitTestUtil.h"
#include "PostGisFdoInsertTest.h"
#include "../../PostGis/SchemaMgr/SchemaManager.h"
#include "../../PostGis/SchemaMgr/Ph/Mgr.h"
#include <FdoCommonFile.h>

#include <cstdlib> // srand, rand
#include <ctime> // time

extern initializer postgis_rdbi_init;

#define POSTGIS_INIT_FILENAME_TEST	"PostGisInit.txt"

// IMPORTANT NOTE!!!
// he unit test will try to load PostGisInit.txt file to get the initialization values.
// If the file is missing next values will be taken as default.
// You can update your file (PostGisInit.txt) with right values or
// if you don’t have that file (or you don't want to create it) you can fill in the default values.
// As a priority the values from OdbcInit.txt will be used if this file exist
// You should create/modify your own file initialization (PostGisInit.txt)
// in this way you are not forced to recompile the project after you changed some values.
#define POSTGIS_SERVICE_DEFAULT		L"" // e.g. postgisserver
#define POSTGIS_USERNAME_DEFAULT	L"" // e.g. root
#define POSTGIS_PASSWORD_DEFAULT	L"" // e.g. xxxx

int PostGisStaticConnection::do_rdbi_init(rdbi_context_def** rdbi_context)
{
#ifdef RDBI_STATIC
    return(rdbi_init(rdbi_context, postgis_rdbi_init));
#else
    return (rdbi_initialize(rdbi_context, "PostGisDriver"));
#endif
}

int PostGisStaticConnection::do_rdbi_connect(rdbi_context_def* rdbi_context, int& id)
{
    return (rdbi_connect(
        rdbi_context,
        UnitTestUtil::GetEnviron("service"),
        UnitTestUtil::GetEnviron("username"),
        UnitTestUtil::GetEnviron("password"),
        &id));
}

FdoSchemaManagerP PostGisStaticConnection::CreateSchemaManager()
{
    FdoStringP datastore(mDatastore);

    FdoSchemaManagerP mgr = NULL;
    mgr = new FdoPostGisSchemaManager(m_gdbi_conn,
        static_cast<FdoString*>(datastore));

    FdoSmPhPostGisMgrP phMgr = 
        mgr->GetPhysicalSchema()->SmartCast<FdoSmPhPostGisMgr>();

    // Pass down the location of the COM directory for MetaSchema
    // creation scripts.
    phMgr->SetHomeDir(L"Dbg/com");

    phMgr->SetRdbiContext(m_rdbi_context);

    return mgr;
}

const char* PostGisStaticConnection::GetServiceName()
{
    return "PostGIS";
}

PostGisConnectionUtil::PostGisConnectionUtil(void)
    : m_SetupValues(NULL)
{
    m_SetupValues = new StringPropertiesDictionary();

    std::srand(static_cast<unsigned int>(std::time(NULL)));
    double sid = static_cast<double>(std::rand()) / 100.3666;
    int testId = static_cast<int>(sid * 100.3666 + 1.3666);
    
    m_IdTest = FdoStringP::Format(L"_%d_", testId);
	printf ("Test ID: %d\n", testId);
}

void PostGisConnectionUtil::LoadInitializeFile()
{
	try
	{
		m_SetupDone = true;
		FdoStringP fileNameCfg = getenv("initfiletest");
		if (fileNameCfg.GetLength() == 0)
			fileNameCfg = POSTGIS_INIT_FILENAME_TEST;
		else
			FdoCommonOSUtil::setenv("initfiletest", "");
		char buffer[1001];
		FdoCommonFile pFile;
		FdoCommonFile::ErrorCode err = FdoCommonFile::ERROR_NONE;
		if (!pFile.OpenFile(fileNameCfg, FdoCommonFile::IDF_OPEN_READ, err) )
		{
			printf("WARNING: PostGisConnectionUtil->GetConfigFile failed to open file '%s'\n",
                   (const char*)fileNameCfg );

			fileNameCfg = POSTGIS_INIT_FILENAME_TEST;
			if (!pFile.OpenFile(fileNameCfg, FdoCommonFile::IDF_OPEN_READ, err) )
			{
				printf("WARNING: PostGisConnectionUtil->GetConfigFile failed to open file '%s'\n",
                       (const char*)fileNameCfg );
				printf("Default values will be used\n" );
			}
		}
		if ( FdoCommonFile::ERROR_NONE == err )
		{
			long bytesRead = 0;
			pFile.ReadFile( buffer, 1000, &bytesRead);
			buffer[bytesRead] = '\0';
			pFile.CloseFile();
			m_SetupValues->ParseStringProperties((FdoStringP)buffer);
		}

		m_SetupValues->SetProperty(L"provider", L"PostGIS");
		// Setup default values in case are missing	
		if (!m_SetupValues->PropertyExist( L"service" ))
			m_SetupValues->SetProperty( L"service", POSTGIS_SERVICE_DEFAULT);

		if (!m_SetupValues->PropertyExist( L"username" ))
			m_SetupValues->SetProperty( L"username", POSTGIS_USERNAME_DEFAULT);
			
		if (!m_SetupValues->PropertyExist( L"password" ))
			m_SetupValues->SetProperty( L"password", POSTGIS_PASSWORD_DEFAULT);
		
		if (!m_SetupValues->PropertyExist( L"clean" ))
			m_SetupValues->SetProperty( L"clean", L"true");
	}
	catch(...)
    {}
}

PostGisConnectionUtil::~PostGisConnectionUtil(void)
{
	if (m_SetupValues->PropertyExist( L"clean" ))
	{
		FdoStringP pValue = m_SetupValues->GetPropertyValue( L"clean" );
		if (pValue == L"false")
			return;
	}
	FdoStringP pTypeName = L"PostGis";
	pTypeName += m_IdTest;
	FdoString* pTypeNamecst = pTypeName;
	std::vector<std::wstring> files;
	FdoCommonFile::GetAllFiles (L"", files);
	size_t lng = pTypeName.GetLength();
	size_t count = files.size();
	for (size_t i = 0; i < count; i++)
	{
		const wchar_t* name;
		if (lng < files[i].length())
		{
			name = files[i].c_str ();
			if (0 == memcmp(name, pTypeNamecst, lng*sizeof(wchar_t)))
				FdoCommonFile::Delete (name, true);
		}
	}
}

void PostGisConnectionUtil::SetProvider( const char *providerName )
{
	if(!m_SetupDone)
		LoadInitializeFile();
	
	if (NULL == getenv("USE_ENV"))
	{
        FdoCommonOSUtil::setenv("service", "");
        FdoCommonOSUtil::setenv("username", ""); 
        FdoCommonOSUtil::setenv("password", ""); 
    }

	if (0 == strcmp(providerName, "PostGIS"))
    {
		FdoStringP pValue;
		
		pValue = m_SetupValues->GetPropertyValue( L"provider" );
        FdoCommonOSUtil::setenv( "provider", pValue);
		
		pValue = m_SetupValues->GetPropertyValue( L"service" );
        FdoCommonOSUtil::setenv( "service", pValue);

		pValue = m_SetupValues->GetPropertyValue( L"username" );
        FdoCommonOSUtil::setenv( "username", pValue);
		
		pValue = m_SetupValues->GetPropertyValue( L"password" );
        FdoCommonOSUtil::setenv( "password", pValue);
    } 
    else
    {
        printf("ERROR:Unknown provider name '%s'!\n", providerName );
        throw;
    }
}

StaticConnection* PostGisConnectionUtil::NewStaticConnection()
{
	return (new PostGisStaticConnection());
}

SchemaOverrideUtilP PostGisConnectionUtil::NewSchemaOverrideUtil()
{
    return new PostGisSchemaOverrideUtil();
}

FdoInsertTest* PostGisConnectionUtil::GetInsertTest()
{
	return new PostGisFdoInsertTest();
}

FdoStringP PostGisConnectionUtil::GetOutputFileName (FdoString* pBaseFileName)
{
	FdoStringP newFileName = L"PostGIS";
	newFileName += m_IdTest;
	newFileName += pBaseFileName;
	return newFileName;
}

wchar_t PostGisConnectionUtil::GetNlsChar( int index )
{
    wchar_t ret = 0x00c1;

    // The PostgreSQL provider can handle any unicode character, regardless of the 
    // database character set. Therefore, try some Asian characters (30b0 and 30b1).
    switch ( index ) 
    {
        case 1:
            ret = 0x30b0;
            break;
        case 2:
            ret = 0x30b1;
            break;
        case 3:
            ret = 0x00e3;
            break;
        case 4:
            ret = 0x00e4;
            break;
    }

    return ret;
}
