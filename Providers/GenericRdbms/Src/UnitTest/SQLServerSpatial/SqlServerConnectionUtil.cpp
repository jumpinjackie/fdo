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
#include "SqlServerConnectionUtil.h"
#include "SqlServerSchemaOverrideUtil.h"
#include "UnitTestUtil.h"
#include "SqlServerFdoInsertTest.h"
#include "../../SQLServerSpatial/SchemaMgr/SchemaManager.h"
#include "../../SQLServerSpatial/SchemaMgr/Ph/Mgr.h"
#include <FdoCommonFile.h>
#include "../../ODBCDriver/context.h"
int odbcdr_rdbi_init( odbcdr_context_def **, rdbi_methods	methods );

#define SQLSERVER_INIT_FILENAME_TEST	"SqlServerSpatialInit.txt"
#define SQLSERVER_SERVICE_DEFAULT		L"seconds"
#define SQLSERVER_PASSWORD_DEFAULT		L"test"

int SqlServerStaticConnection::do_rdbi_init (rdbi_context_def** rdbi_context)
{
#ifdef RDBI_STATIC
    return (rdbi_init (rdbi_context, (initializer*)odbcdr_rdbi_init));
#else
    return (rdbi_initialize (rdbi_context, "SqlServerDriver"));
#endif
}

int SqlServerStaticConnection::do_rdbi_connect (rdbi_context_def* rdbi_context, int& id)
{
	FdoStringP pService = UnitTestUtil::GetEnviron("service");
    FdoStringP connectString = FdoStringP::Format(L"DRIVER={SQL Server}; SERVER=%ls", (FdoString*)pService);

    if (rdbi_context->dispatch.capabilities.supports_unicode == 1)
    {
        return (rdbi_connectW (
            rdbi_context,
            connectString,
            UnitTestUtil::GetEnviron( "username" ),
            UnitTestUtil::GetEnviron( "password" ),
            &id));
    }
    else
    {
        return (rdbi_connect (
            rdbi_context,
            connectString,
            UnitTestUtil::GetEnviron( "username" ),
            UnitTestUtil::GetEnviron( "password" ),
            &id));
    }
}

FdoSchemaManagerP SqlServerStaticConnection::CreateSchemaManager()
{
    FdoSchemaManagerP schMgr = new FdoSqsSchemaManager( m_gdbi_conn, (FdoString*) FdoStringP(mDatastore) );

    FdoSmPhSqsMgrP physMgr = schMgr->GetPhysicalSchema()->SmartCast<FdoSmPhSqsMgr>();

    // Pass down the location of the COM directory for MetaSchema creation scripts.
    physMgr->SetHomeDir( L"Dbg/com" );

    physMgr->SetRdbiContext( m_rdbi_context );

    return schMgr;
}

const char* SqlServerStaticConnection::GetServiceName()
{
    return "SQLServerSpatial";
}

void SqlServerStaticConnection::SetSchema (FdoString* suffix) 
{
    mDatastore = UnitTestUtil::GetEnviron( "datastore", suffix );
    if ( m_rdbi_context->dispatch.capabilities.supports_unicode == 1 )
        rdbi_set_schemaW( m_rdbi_context, mDatastore );
    else
        rdbi_set_schema( m_rdbi_context, mDatastore );
}

SqlServerConnectionUtil::SqlServerConnectionUtil(void)
{
	m_SetupValues = new StringPropertiesDictionary();

	srand( (unsigned)time( NULL ) );
	int TestID = (int)(((double)rand()/(double)100.3666) * 100.3666 + 1.3666);
	m_IdTest = FdoStringP::Format(L"_%d_", TestID);
	printf ("Test ID: %d\n", TestID);
}

void SqlServerConnectionUtil::LoadInitializeFile()
{
	try
	{
		m_SetupDone = true;
		FdoStringP fileNameCfg = getenv("initfiletest");
		if (fileNameCfg.GetLength() == 0)
			fileNameCfg = SQLSERVER_INIT_FILENAME_TEST;
		else
			FdoCommonOSUtil::setenv("initfiletest", "");
		char buffer[1001];
		FdoCommonFile pFile;
		FdoCommonFile::ErrorCode err = FdoCommonFile::ERROR_NONE;
		if (!pFile.OpenFile(fileNameCfg, FdoCommonFile::IDF_OPEN_READ, err) )
		{
			printf( "WARNING: SqlServerConnectionUtil->GetConfigFile failed to open file '%s'\n", (const char*)fileNameCfg );
			fileNameCfg = SQLSERVER_INIT_FILENAME_TEST;
			if (!pFile.OpenFile(fileNameCfg, FdoCommonFile::IDF_OPEN_READ, err) )
			{
				printf( "WARNING: SqlServerConnectionUtil->GetConfigFile failed to open file '%s'\n", (const char*)fileNameCfg );
				printf( "Default values will be used\n" );
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

		m_SetupValues->SetProperty(L"provider", L"SQLServerSpatial");
		
		// Setup default values in case are missing	
		if (!m_SetupValues->PropertyExist( L"service" ))
			m_SetupValues->SetProperty( L"service", SQLSERVER_SERVICE_DEFAULT);

		if (!m_SetupValues->PropertyExist( L"username" ))
		{
			char uname[1024];
			int   size=1024;
			UnitTestUtil::GetRealUserName(uname, size);
			m_SetupValues->SetProperty( L"username", (FdoStringP)uname);
		}	
		if (!m_SetupValues->PropertyExist( L"password" ))
			m_SetupValues->SetProperty( L"password", SQLSERVER_PASSWORD_DEFAULT);
		
		if (!m_SetupValues->PropertyExist( L"clean" ))
			m_SetupValues->SetProperty( L"clean", L"true");
	}
	catch(...){}
}

SqlServerConnectionUtil::~SqlServerConnectionUtil(void)
{
	if (m_SetupValues->PropertyExist( L"clean" ))
	{
		FdoStringP pValue = m_SetupValues->GetPropertyValue( L"clean" );
		if (pValue == L"false")
			return;
	}
	FdoStringP pTypeName = L"SQLServerSpatial";
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

void SqlServerConnectionUtil::SetProvider( const char *providerName )
{
	if(!m_SetupDone)
		LoadInitializeFile();

	if (getenv("USE_ENV") == NULL)
	{
        FdoCommonOSUtil::setenv("service", "");
        FdoCommonOSUtil::setenv("username", ""); 
        FdoCommonOSUtil::setenv("password", ""); 
    }

    if (strcmp(providerName, "SQLServerSpatial") == 0)
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

StaticConnection* SqlServerConnectionUtil::NewStaticConnection()
{
	return new SqlServerStaticConnection();
}

SchemaOverrideUtilP SqlServerConnectionUtil::NewSchemaOverrideUtil()
{
    return new SqlServerSchemaOverrideUtil();
}

FdoInsertTest* SqlServerConnectionUtil::GetInsertTest ()
{
	return new SqlServerFdoInsertTest();
}

FdoStringP SqlServerConnectionUtil::GetOutputFileName (FdoString* pBaseFileName)
{
	FdoStringP newFileName = L"SQLServerSpatial";
	newFileName += m_IdTest;
	newFileName += pBaseFileName;
	return newFileName;
}

FdoStringP SqlServerConnectionUtil::GetNlsObjectName( FdoStringP inName )
{
    if ( inName == L"Geometry") 
        return inName;

    if ( inName == L"Industrial Parcel" ) 
        return L"Industrial_Parcel";

    return ConnectionUtil::GetNlsObjectName(inName);
}

wchar_t SqlServerConnectionUtil::GetNlsChar( int index )
{
    wchar_t ret = 0x00c1;

    // The SqlServer provider can handle any unicode character, regardless of the 
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
