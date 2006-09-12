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
#include "MySqlConnectionUtil.h"
#include "UnitTestUtil.h"
#include "MySqlFdoInsertTest.h"
#include "../../MySQL/SchemaMgr/SchemaManager.h"
#include "../../MySQL/SchemaMgr/Ph/Mgr.h"
#include <FdoCommonFile.h>

extern initializer mysql_rdbi_init;

#define MYSQL_INIT_FILENAME_TEST	"MySqlInit.txt"
//	IMPORTANT NOTE!!!
//	The unit test will try to load MySqlInit.txt file to get the initialization values.
//	If the file is missing next values will be taken as default.
//	You can update your file (MySqlInit.txt) with right values or if you don’t have that file (or you don't want to create it) you can fill in the default values.
//	As a priority the values from OdbcInit.txt will be used if this file exist
//	You should create/modify your own file initialization (MySqlInit.txt) in this way you are not forced to recompile the project after you changed some values.
#define MYSQL_SERVICE_DEFAULT		L"" // e.g. mysqlserver
#define MYSQL_USERNAME_DEFAULT		L"" // e.g. root
#define MYSQL_PASSWORD_DEFAULT		L"" // e.g. xxxx

int MySqlStaticConnection::do_rdbi_init (rdbi_context_def** rdbi_context)
{
#ifdef RDBI_STATIC
    return (rdbi_init (rdbi_context, mysql_rdbi_init));
#else
    return (rdbi_initialize (rdbi_context, "MySqlDriver"));
#endif
}

int MySqlStaticConnection::do_rdbi_connect (rdbi_context_def* rdbi_context, int& id)
{
    return (rdbi_connect (
        rdbi_context,
        UnitTestUtil::GetEnviron("service"),
        UnitTestUtil::GetEnviron("username"),
        UnitTestUtil::GetEnviron("password"),
        &id));
}

FdoSchemaManagerP MySqlStaticConnection::CreateSchemaManager()
{
    FdoSchemaManagerP schMgr = new FdoMySqlSchemaManager(m_gdbi_conn, (FdoString*) FdoStringP(mDatastore));

    FdoSmPhMySqlMgrP physMgr = schMgr->GetPhysicalSchema()->SmartCast<FdoSmPhMySqlMgr>();

    // Pass down the location of the COM directory for MetaSchema creation scripts.
    physMgr->SetHomeDir( L"Dbg/com" );

    physMgr->SetRdbiContext( m_rdbi_context );

    return schMgr;
}

const char* MySqlStaticConnection::GetServiceName()
{
    return "MySql";
}

MySqlConnectionUtil::MySqlConnectionUtil(void)
{
	m_SetupValues = new StringPropertiesDictionary();
	srand( (unsigned)time( NULL ) );
	int TestID = (int)(((double)rand()/(double)100.3666) * 100.3666 + 1.3666);
	m_IdTest = FdoStringP::Format(L"_%d_", TestID);
	printf ("Test ID: %d\n", TestID);
}

void MySqlConnectionUtil::LoadInitializeFile()
{
	try
	{
		m_SetupDone = true;
		FdoStringP fileNameCfg = getenv("initfiletest");
		if (fileNameCfg.GetLength() == 0)
			fileNameCfg = MYSQL_INIT_FILENAME_TEST;
		else
			FdoCommonOSUtil::setenv("initfiletest", "");
		char buffer[1001];
		FdoCommonFile pFile;
		FdoCommonFile::ErrorCode err = FdoCommonFile::ERROR_NONE;
		if (!pFile.OpenFile(fileNameCfg, FdoCommonFile::IDF_OPEN_READ, err) )
		{
			printf( "WARNING: MySqlConnectionUtil->GetConfigFile failed to open file '%s'\n", (const char*)fileNameCfg );
			fileNameCfg = MYSQL_INIT_FILENAME_TEST;
			if (!pFile.OpenFile(fileNameCfg, FdoCommonFile::IDF_OPEN_READ, err) )
			{
				printf( "WARNING: MySqlConnectionUtil->GetConfigFile failed to open file '%s'\n", (const char*)fileNameCfg );
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

		m_SetupValues->SetProperty(L"provider", L"MySql");
		// Setup default values in case are missing	
		if (!m_SetupValues->PropertyExist( L"service" ))
			m_SetupValues->SetProperty( L"service", MYSQL_SERVICE_DEFAULT);

		if (!m_SetupValues->PropertyExist( L"username" ))
			m_SetupValues->SetProperty( L"username", MYSQL_USERNAME_DEFAULT);
			
		if (!m_SetupValues->PropertyExist( L"password" ))
			m_SetupValues->SetProperty( L"password", MYSQL_PASSWORD_DEFAULT);
		
		if (!m_SetupValues->PropertyExist( L"clean" ))
			m_SetupValues->SetProperty( L"clean", L"true");
	}
	catch(...){}
}

MySqlConnectionUtil::~MySqlConnectionUtil(void)
{
	if (m_SetupValues->PropertyExist( L"clean" ))
	{
		FdoStringP pValue = m_SetupValues->GetPropertyValue( L"clean" );
		if (pValue == L"false")
			return;
	}
	FdoStringP pTypeName = L"MySql";
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

void MySqlConnectionUtil::SetProvider( const char *providerName )
{
	if(!m_SetupDone)
		LoadInitializeFile();
	
	if (getenv("USE_ENV") == NULL)
	{
        FdoCommonOSUtil::setenv("service", "");
        FdoCommonOSUtil::setenv("username", ""); 
        FdoCommonOSUtil::setenv("password", ""); 
    }

	if (strcmp(providerName, "MySql") == 0 )
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

StaticConnection* MySqlConnectionUtil::NewStaticConnection()
{
	return new MySqlStaticConnection();
}

FdoInsertTest* MySqlConnectionUtil::GetInsertTest ()
{
	return new MySqlFdoInsertTest();
}

FdoStringP MySqlConnectionUtil::GetOutputFileName (FdoString* pBaseFileName)
{
	FdoStringP newFileName = L"MySql";
	newFileName += m_IdTest;
	newFileName += pBaseFileName;
	return newFileName;
}
