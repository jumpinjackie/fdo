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
#include "OdbcConnectionUtil.h"
#include "UnitTestUtil.h"
#include "FdoInsertTest.h"
#include "../ODBC/SchemaMgr/SchemaManager.h"
#include "../ODBC/SchemaMgr/Ph/Mgr.h"
#include <FdoCommonFile.h>
#include "../SchemaMgr/Ph/Owner.h"

#ifdef _WIN32
#include <odbcinst.h>
#endif

#define ODBC_INIT_FILENAME_TEST				"OdbcInit.txt"
//	IMPORTANT NOTE!!!
//	The unit test will try to load OdbcInit.txt file to get the initialization values.
//	If the file is missing next values will be taken as default.
//	You can update your file (OdbcInit.txt) with right values or if you don’t have that file (or you don't want to create it) you can fill in the default values.
//	As a priority the values from OdbcInit.txt will be used if this file exist
//	You should create/modify your own file initialization (OdbcInit.txt) in this way you are not forced to recompile the project after you changed some values.
#define ODBCMYSQL_SERVICENAME_DEFAULT		L"" // e.g. mysqlserver
#define ODBCMYSQL_USERNAME_DEFAULT			L"" // e.g. root
#define ODBCMYSQL_PASSWORD_DEFAULT			L"" // e.g. xxxx
#define ODBCMYSQL_DSN_DEFAULT				UNITTEST_VERSION_NAME L"MySqlFdoTest"

#define ODBCSQLSERVER_SERVICENAME_DEFAULT	L"" // e.g. sqlserver
// if usernameSqlServer is missing from initialization file
// the logon user will be used!!!
#define ODBCSQLSERVER_PASSWORD_DEFAULT		L"" // e.g. xxxx
#define ODBCSQLSERVER_DSN_DEFAULT			UNITTEST_VERSION_NAME L"SqlServerFdoTest"

#define ODBCORACLE_SERVICENAME_DEFAULT		L"" // e.g. oraserver
// if usernameOracle is missing from initialization file
// the logon user will be used!!!
#define ODBCORACLE_PASSWORD_DEFAULT			L"" // e.g. xxxx
#define ODBCORACLE_DSN_DEFAULT				UNITTEST_VERSION_NAME L"OracleFdoTest"

#define ODBCACCESS_DSN_DEFAULT				L"MsTest"
#define ODBCDBASE_DSN_DEFAULT				L"MsTestDbase"
#define ODBCEXCEL_DSN_DEFAULT				L"MsTestXls"
#define ODBCTEXT_DSN_DEFAULT				L"Country_Text"

#ifdef _WIN32
int odbcdr_rdbi_init( odbcdr_context_def **, rdbi_methods	methods );
#endif

int OdbcStaticConnection::do_rdbi_init (rdbi_context_def** rdbi_context)
{
#ifdef _WIN32
#ifdef RDBI_STATIC
        return (rdbi_init (rdbi_context, (initializer*)odbcdr_rdbi_init));
#else
        return (rdbi_initialize (rdbi_context, "SqlServerDriver"));
#endif
#else
	return 0;
#endif
}

int OdbcStaticConnection::do_rdbi_connect (rdbi_context_def* rdbi_context, int& id)
{
	FdoStringP csStr = UnitTestUtil::GetConnectionString(Connection_NoDatastore);
    if (rdbi_context->dispatch.capabilities.supports_unicode == 1)
        return (rdbi_connectW (rdbi_context, csStr, L"", L"", &id));
    else
        return (rdbi_connect (rdbi_context, csStr, "", "", &id));
}

FdoSchemaManagerP OdbcStaticConnection::CreateSchemaManager()
{
    FdoSchemaManagerP schMgr = new FdoOdbcSchemaManager(m_gdbi_conn, (FdoString*) FdoStringP(mDatastore));
    FdoSmPhOdbcMgrP physMgr = schMgr->GetPhysicalSchema()->SmartCast<FdoSmPhOdbcMgr>();

    // Pass down the location of the COM directory for MetaSchema creation scripts.
    physMgr->SetHomeDir( FdoStringP(L"Dbg/com") );
    physMgr->SetRdbiContext( m_rdbi_context );

    return schMgr;
}

const char* OdbcStaticConnection::GetServiceName()
{
	return "Odbc";
}

OdbcConnectionUtil::OdbcConnectionUtil(void)
{
	m_SetupValues = new StringPropertiesDictionary();

	srand( (unsigned)time( NULL ) );
	int TestID = (int)(((double)rand()/(double)100.3666) * 100.3666 + 1.3666);
	m_IdTest = FdoStringP::Format(L"_%d_", TestID);
	printf ("Test ID: %d\n", TestID);
#ifdef _WIN32
	m_SetupTextDSNdone = m_SetupAccessDSNdone = m_SetupExcelDSNdone = false;
    m_SetupSqlServerDSNdone = m_SetupMySqlDSNdone = m_SetupOracleDSNdone = false;
    m_SetupDbaseDSNdone = false;
#endif
}

void OdbcConnectionUtil::LoadInitializeFile()
{
	try
	{
		m_SetupDone = true;
		FdoStringP fileNameCfg = getenv("initfiletest");
		if (fileNameCfg.GetLength() == 0)
			fileNameCfg = ODBC_INIT_FILENAME_TEST;
		else
			FdoCommonOSUtil::setenv("initfiletest", "");
		char buffer[1001];
		FdoCommonFile pFile;
		FdoCommonFile::ErrorCode err = FdoCommonFile::ERROR_NONE;
		if (!pFile.OpenFile(fileNameCfg, FdoCommonFile::IDF_OPEN_READ, err) )
		{
			printf( "WARNING: OdbcConnectionUtil->LoadInitializeFile failed to open file '%s'\n", (const char*)fileNameCfg );
			fileNameCfg = ODBC_INIT_FILENAME_TEST;
			if (!pFile.OpenFile(fileNameCfg, FdoCommonFile::IDF_OPEN_READ, err) )
			{
				printf( "WARNING: OdbcConnectionUtil->LoadInitializeFile failed to open file '%s'\n", (const char*)fileNameCfg );
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

		m_SetupValues->SetProperty(L"provider", L"Odbc");

		// Setup default values in case are missing	
		// MySql
		if (!m_SetupValues->PropertyExist( L"serviceMySql" ))
			m_SetupValues->SetProperty( L"serviceMySql", ODBCMYSQL_SERVICENAME_DEFAULT);

		if (!m_SetupValues->PropertyExist( L"usernameMySql" ))
			m_SetupValues->SetProperty( L"usernameMySql", ODBCMYSQL_USERNAME_DEFAULT);
			
		if (!m_SetupValues->PropertyExist( L"passwordMySql" ))
			m_SetupValues->SetProperty( L"passwordMySql", ODBCMYSQL_PASSWORD_DEFAULT);

		if (!m_SetupValues->PropertyExist( L"DSNMySql" ))
			m_SetupValues->SetProperty( L"DSNMySql", ODBCMYSQL_DSN_DEFAULT);

		// SqlServer
		if (!m_SetupValues->PropertyExist( L"serviceSqlServer" ))
			m_SetupValues->SetProperty( L"serviceSqlServer", ODBCSQLSERVER_SERVICENAME_DEFAULT);

		if (!m_SetupValues->PropertyExist( L"usernameSqlServer" ))
		{
			char uname[1024];
			int   size=1024;
			UnitTestUtil::GetRealUserName(uname, size);
			m_SetupValues->SetProperty( L"usernameSqlServer", (FdoStringP)uname);
		}
		if (!m_SetupValues->PropertyExist( L"passwordSqlServer" ))
			m_SetupValues->SetProperty( L"passwordSqlServer", ODBCSQLSERVER_PASSWORD_DEFAULT);

		if (!m_SetupValues->PropertyExist( L"DSNSqlServer" ))
			m_SetupValues->SetProperty( L"DSNSqlServer", ODBCSQLSERVER_DSN_DEFAULT);

		// Oracle
		if (!m_SetupValues->PropertyExist( L"serviceOracle" ))
			m_SetupValues->SetProperty( L"serviceOracle", ODBCORACLE_SERVICENAME_DEFAULT);

		if (!m_SetupValues->PropertyExist( L"usernameOracle" ))
		{
			char uname[1024];
			int   size=1024;
			UnitTestUtil::GetRealUserName(uname, size);
			m_SetupValues->SetProperty( L"usernameOracle", (FdoStringP)uname);
		}	
			
		if (!m_SetupValues->PropertyExist( L"passwordOracle" ))
			m_SetupValues->SetProperty( L"passwordOracle", ODBCORACLE_PASSWORD_DEFAULT);

		if (!m_SetupValues->PropertyExist( L"DSNOracle" ))
			m_SetupValues->SetProperty( L"DSNOracle", ODBCORACLE_DSN_DEFAULT);

		if (!m_SetupValues->PropertyExist( L"enableOracleSetup" ))
			m_SetupValues->SetProperty( L"enableOracleSetup", L"true");
		
        // Access
		if (!m_SetupValues->PropertyExist( L"DSNAccess" ))
			m_SetupValues->SetProperty( L"DSNAccess", ODBCACCESS_DSN_DEFAULT);

        // dBASE
		if (!m_SetupValues->PropertyExist( L"DSNDbase" ))
			m_SetupValues->SetProperty( L"DSNDbase", ODBCDBASE_DSN_DEFAULT);

		// Excel
		if (!m_SetupValues->PropertyExist( L"DSNExcel" ))
			m_SetupValues->SetProperty( L"DSNExcel", ODBCEXCEL_DSN_DEFAULT);
		
		// Text
		if (!m_SetupValues->PropertyExist( L"DSNText" ))
			m_SetupValues->SetProperty( L"DSNText", ODBCTEXT_DSN_DEFAULT);

		if (!m_SetupValues->PropertyExist( L"clean" ))
			m_SetupValues->SetProperty( L"clean", L"true");
	}
	catch(...){}
}

void OdbcConnectionUtil::CleanFiles(std::vector<std::wstring>& files, FdoStringP& pTypeName)
{
	try
	{
		FdoString* pTypeNamecst = pTypeName;
		size_t lng = pTypeName.GetLength();
		size_t count = files.size();
		for (size_t i = 0; i < count; i++)
		{
			FdoStringP name;
			if (lng != files[i].length())
			{
				name = files[i].c_str ();
                if (name.Contains(pTypeName))
					FdoCommonFile::Delete (name, true);
			}
		}
	}
	catch(...){}
}

OdbcConnectionUtil::~OdbcConnectionUtil(void)
{
	if (m_SetupValues->PropertyExist( L"clean" ))
	{
		FdoStringP pValue = m_SetupValues->GetPropertyValue( L"clean" );
		if (pValue == L"false")
			return;
	}
	std::vector<std::wstring> files;
	FdoCommonFile::GetAllFiles (L"", files);

	FdoStringP pTypeName = L"OdbcMySql";
	pTypeName += m_IdTest;
	CleanFiles(files, pTypeName);

	pTypeName = L"OdbcSqlServer";
	pTypeName += m_IdTest;
	CleanFiles(files, pTypeName);

	pTypeName = L"OdbcOracle";
	pTypeName += m_IdTest;
	CleanFiles(files, pTypeName);

	pTypeName = L"OdbcAccess";
	pTypeName += m_IdTest;
	CleanFiles(files, pTypeName);

	pTypeName = L"OdbcExcel";
	pTypeName += m_IdTest;
	CleanFiles(files, pTypeName);

#ifdef _WIN32
    if (m_SetupMySqlDSNdone)
        TeardownMySqlDSN();
    if (m_SetupSqlServerDSNdone)
        TeardownSqlServerDSN();
    if (m_SetupOracleDSNdone)
        TeardownOracleDSN();
    if (m_SetupAccessDSNdone)
        TeardownAccessDSN();
    if (m_SetupDbaseDSNdone)
        TeardownDbaseDSN();
    if (m_SetupExcelDSNdone)
        TeardownExcelDSN();
    if (m_SetupTextDSNdone)
        TeardownTextDSN();
#endif
}

void OdbcConnectionUtil::SetProvider( const char *providerName )
{
	if(!m_SetupDone)
		LoadInitializeFile();

	if (getenv("USE_ENV") == NULL)
	{
        FdoCommonOSUtil::setenv("service", "");
        FdoCommonOSUtil::setenv("username", ""); 
        FdoCommonOSUtil::setenv("password", ""); 
    }

	FdoStringP pValue;
    FdoCommonOSUtil::setenv( "provider", "Odbc");
	if (strcmp(providerName, "OdbcMySql") == 0 )
    {
		m_ProviderActive = L"OdbcMySql";
#ifdef _WIN32
        if (!m_SetupMySqlDSNdone)
            SetupMySqlDSN();
#endif
		pValue = m_SetupValues->GetPropertyValue( L"serviceMySql" );
        FdoCommonOSUtil::setenv( "service", pValue);

		pValue = m_SetupValues->GetPropertyValue( L"usernameMySql" );
        FdoCommonOSUtil::setenv( "username", pValue);
		
		pValue = m_SetupValues->GetPropertyValue( L"passwordMySql" );
        FdoCommonOSUtil::setenv( "password", pValue);

		pValue = m_SetupValues->GetPropertyValue( L"DSNMySql" );
        FdoCommonOSUtil::setenv( "dsnname", pValue);
    } 
    else if (strcmp(providerName, "OdbcSqlServer") == 0 )
    {
		m_ProviderActive = L"OdbcSqlServer";
#ifdef _WIN32
        if (!m_SetupSqlServerDSNdone)
		    SetupSqlServerDSN();
#endif
		pValue = m_SetupValues->GetPropertyValue( L"serviceSqlServer" );
        FdoCommonOSUtil::setenv( "service", pValue);

		pValue = m_SetupValues->GetPropertyValue( L"usernameSqlServer" );
        FdoCommonOSUtil::setenv( "username", pValue);
		
		pValue = m_SetupValues->GetPropertyValue( L"passwordSqlServer" );
        FdoCommonOSUtil::setenv( "password", pValue);
		
		pValue = m_SetupValues->GetPropertyValue( L"DSNSqlServer" );
        FdoCommonOSUtil::setenv( "dsnname", pValue);
    } 
    else if (strcmp(providerName, "OdbcOracle") == 0 )
    {
		m_ProviderActive = L"OdbcOracle";
#ifdef _WIN32
		if (!m_SetupOracleDSNdone)
		    SetupOracleDSN();
#else
        strcpy(theOracleDriverName, "Oracle 10g ODBC Driver");
#endif
		pValue = m_SetupValues->GetPropertyValue( L"serviceOracle" );
        FdoCommonOSUtil::setenv( "service", pValue);

		pValue = m_SetupValues->GetPropertyValue( L"usernameOracle" );
        FdoCommonOSUtil::setenv( "username", pValue);
		
		pValue = m_SetupValues->GetPropertyValue( L"passwordOracle" );
        FdoCommonOSUtil::setenv( "password", pValue);

		pValue = m_SetupValues->GetPropertyValue( L"DSNOracle" );
        FdoCommonOSUtil::setenv( "dsnname", pValue);
	}
    else if (strcmp(providerName, "OdbcSybase") == 0 )
    {
		m_ProviderActive = L"OdbcSybase";
		pValue = m_SetupValues->GetPropertyValue( L"serviceSybase" );
        FdoCommonOSUtil::setenv( "service", pValue);

		pValue = m_SetupValues->GetPropertyValue( L"usernameSybase" );
        FdoCommonOSUtil::setenv( "username", pValue);
		
		pValue = m_SetupValues->GetPropertyValue( L"passwordSybase" );
        FdoCommonOSUtil::setenv( "password", pValue);

		pValue = m_SetupValues->GetPropertyValue( L"DSNSybase" );
        FdoCommonOSUtil::setenv( "dsnname", pValue);
	}
    else if (strcmp(providerName, "OdbcAccess") == 0 )
    {
		m_ProviderActive = L"OdbcAccess";
#ifdef _WIN32
		if (!m_SetupAccessDSNdone)
		    SetupAccessDSN();
#endif
		pValue = m_SetupValues->GetPropertyValue( L"DSNAccess" );
        FdoCommonOSUtil::setenv( "dsnname", pValue);
	}
    else if (strcmp(providerName, "OdbcDbase") == 0 )
    {
		m_ProviderActive = L"OdbcDbase";
#ifdef _WIN32
		if (!m_SetupDbaseDSNdone)
		    SetupDbaseDSN();
#endif
		pValue = m_SetupValues->GetPropertyValue( L"DSNDbase" );
        FdoCommonOSUtil::setenv( "dsnname", pValue);
	}
    else if (strcmp(providerName, "OdbcExcel") == 0 )
    {
		m_ProviderActive = L"OdbcExcel";
#ifdef _WIN32
        if (!m_SetupExcelDSNdone)
		    SetupExcelDSN();
#endif
		pValue = m_SetupValues->GetPropertyValue( L"DSNExcel" );
        FdoCommonOSUtil::setenv( "dsnname", pValue);
	}
    else if (strcmp(providerName, "OdbcText") == 0 )
    {
		m_ProviderActive = L"OdbcText";
#ifdef _WIN32
        if (!m_SetupTextDSNdone)
            SetupTextDSN();
#endif
		pValue = m_SetupValues->GetPropertyValue( L"DSNText" );
        FdoCommonOSUtil::setenv( "dsnname", pValue);
	}
    else
    {
        printf("ERROR:Unknown provider name '%s'!\n", providerName );
        throw;
    }
}

wchar_t *OdbcConnectionUtil::GetConnectionString(StringConnTypeRequest pTypeReq, FdoString *suffix, bool bAddExtraneousSpaces)
{
    FdoStringP service = UnitTestUtil::GetEnviron("service");
    FdoStringP username = UnitTestUtil::GetEnviron("username");
    FdoStringP password = UnitTestUtil::GetEnviron("password");
    FdoStringP datastore = UnitTestUtil::GetEnviron("datastore", suffix);
    FdoStringP dsnname = UnitTestUtil::GetEnviron("dsnname");
	
	FdoStringP OracleDriverName = theOracleDriverName;

	static wchar_t connectString[200];
	connectString[0] = L'\0';

	switch (pTypeReq)
	{
		case Connection_WithDatastore:
			if (m_ProviderActive == L"OdbcMySql")
				swprintf( 
                    connectString, 
                    sizeof(connectString)/sizeof(wchar_t), 
                    L"ConnectionString=\"DRIVER={MySQL ODBC 3.51 Driver};SERVER=%ls;DATABASE=%ls;USER=%ls;PASSWORD=%ls;OPTION=3;\"", 
                    (FdoString*) service, 
                    (FdoString*) datastore, 
                    (FdoString*) username, 
                    (FdoString*) password
                );
			else if (m_ProviderActive == L"OdbcSqlServer")
				swprintf( 
                    connectString, 
                    sizeof(connectString)/sizeof(wchar_t), 
                    L"ConnectionString=\"DRIVER={SQL Server};SERVER=%ls;DATABASE=%ls;UID=%ls;PWD=%ls;\"", 
                    (FdoString*) service, 
                    (FdoString*) datastore, 
                    (FdoString*) username, 
                    (FdoString*) password
                );
			else if (m_ProviderActive == L"OdbcSybase")
				swprintf( 
                    connectString, 
                    sizeof(connectString)/sizeof(wchar_t), 
                    L"ConnectionString=\"DRIVER={Adaptive Server Enterprise};SERVER=%ls;PORT=5000;DATABASE=%ls;UID=%ls;PWD=%ls;\"", 
                    (FdoString*) service, 
                    (FdoString*) datastore, 
                    (FdoString*) username, 
                    (FdoString*) password
                );
			else if (m_ProviderActive == L"OdbcOracle")
			{
				FdoStringP pDatastore = datastore;
				pDatastore = pDatastore.Upper();
				swprintf( 
                    connectString, 
                    sizeof(connectString)/sizeof(wchar_t), 
                    L"ConnectionString=\"DRIVER={%ls};UID=%ls;PWD=%ls;DBQ=%ls;XSM=%ls;\"", 
                    (FdoString*)OracleDriverName, 
                    (FdoString*)pDatastore, 
                    (FdoString*)password, 
                    (FdoString*)service, 
                    (FdoString*)pDatastore);
			}
			else if (m_ProviderActive == L"OdbcAccess")
				swprintf( 
                    connectString, 
                    sizeof(connectString)/sizeof(wchar_t), 
                    L"DataSourceName=%ls;UserId=;Password=;", 
                    (FdoString*)dsnname
                );
			else if (m_ProviderActive == L"OdbcExcel")
				swprintf( 
                    connectString, 
                    sizeof(connectString)/sizeof(wchar_t), 
                    L"DataSourceName=%ls;UserId=;Password=;", 
                    (FdoString*) dsnname
                );
			else if (m_ProviderActive == L"OdbcText")
				swprintf( 
                    connectString, 
                    sizeof(connectString)/sizeof(wchar_t), 
                    L"DataSourceName=%ls;UserId=;Password=;", 
                    (FdoString*) dsnname
                );
		break;

		case Connection_NoDatastore:
			if (m_ProviderActive == L"OdbcMySql")
				swprintf( 
                    connectString, 
                    sizeof(connectString)/sizeof(wchar_t), 
                    L"ConnectionString=\"DRIVER={MySQL ODBC 3.51 Driver};SERVER=%ls;USER=%ls;PASSWORD=%ls;OPTION=3;\"", 
                    (FdoString*)service, 
                    (FdoString*)username, 
                    (FdoString*)password
                );
			else if (m_ProviderActive == L"OdbcSqlServer")
				swprintf( 
                    connectString, 
                    sizeof(connectString)/sizeof(wchar_t), 
                    L"ConnectionString=\"DRIVER={SQL Server};SERVER=%ls;UID=%ls;PWD=%ls;\"", 
                    (FdoString*)service, 
                    (FdoString*)username, 
                    (FdoString*)password
                );
			else if (m_ProviderActive == L"OdbcSybase")
				swprintf( 
                    connectString, 
                    sizeof(connectString)/sizeof(wchar_t), 
                    L"ConnectionString=\"DRIVER={Adaptive Server Enterprise};SERVER=%ls;PORT=5000;UID=%ls;PWD=%ls;\"", 
                    (FdoString*)service, 
                    (FdoString*)username, 
                    (FdoString*)password
                );
			else if (m_ProviderActive == L"OdbcOracle")
			{
				FdoStringP pDatastore = datastore;
				pDatastore = pDatastore.Upper();
				swprintf( 
                    connectString, 
                    sizeof(connectString)/sizeof(wchar_t), 
                    L"ConnectionString=\"DRIVER={%ls};UID=%ls;PWD=%ls;DBQ=%ls;\"", 
                    (FdoString*)OracleDriverName, 
                    (FdoString*)pDatastore /*username*/, 
                    (FdoString*)password, 
                    (FdoString*)service
                );
			}
			else if (m_ProviderActive == L"OdbcAccess")
				swprintf( 
                    connectString, 
                    sizeof(connectString)/sizeof(wchar_t), 
                    L"DataSourceName=%ls;UserId=;Password=;", 
                    (FdoString*)dsnname
                );
			else if (m_ProviderActive == L"OdbcExcel")
				swprintf( 
                    connectString, 
                    sizeof(connectString)/sizeof(wchar_t), 
                    L"DataSourceName=%ls;UserId=;Password=;", 
                    (FdoString*)dsnname
                );
			else if (m_ProviderActive == L"OdbcText")
				swprintf( 
                    connectString, 
                    sizeof(connectString)/sizeof(wchar_t), 
                    L"DataSourceName=%ls;UserId=;Password=;", 
                    (FdoString*)dsnname
                );
		break;
		
		case Connection_WithDSN:
			if (m_ProviderActive == L"OdbcMySql")
				swprintf( 
                    connectString, 
                    sizeof(connectString)/sizeof(wchar_t), 
                    L"DataSourceName=%ls;UserId=%ls;Password=%ls;", 
                    (FdoString*)dsnname, 
                    (FdoString*)username, 
                    (FdoString*)password
                );
			else if (m_ProviderActive == L"OdbcSqlServer")
				swprintf( 
                    connectString, 
                    sizeof(connectString)/sizeof(wchar_t), 
                    L"DataSourceName=%ls;UserId=%ls;Password=%ls;", 
                    (FdoString*)dsnname, 
                    (FdoString*)username, 
                    (FdoString*)password
                );
			else if (m_ProviderActive == L"OdbcOracle")
			{
			FdoStringP pDatastore = datastore;
			pDatastore = pDatastore.Upper();
            swprintf( 
                connectString, 
                sizeof(connectString)/sizeof(wchar_t), 
                L"DataSourceName=%ls;UserId=%ls;Password=%ls;", 
                (FdoString*)dsnname, 
                (FdoString*)pDatastore, 
                (FdoString*)password
            );
			}
			else if (m_ProviderActive == L"OdbcSybase")
			{
                swprintf( 
                    connectString, 
                    sizeof(connectString)/sizeof(wchar_t), 
                    L"DataSourceName=%ls;UserId=%ls;Password=%ls;", 
                    (FdoString*)dsnname, 
                    (FdoString*)username, 
                    (FdoString*)password
                );
			}
			else if (m_ProviderActive == L"OdbcAccess")
				swprintf( 
                    connectString, 
                    sizeof(connectString)/sizeof(wchar_t), 
                    L"DataSourceName=%ls;UserId=;Password=;", 
                    (FdoString*)dsnname
                );
			else if (m_ProviderActive == L"OdbcDbase")
				swprintf( 
                    connectString, 
                    sizeof(connectString)/sizeof(wchar_t), 
                    L"DataSourceName=%ls;UserId=;Password=;", 
                    (FdoString*)dsnname
                );
			else if (m_ProviderActive == L"OdbcExcel")
				swprintf( 
                    connectString, 
                    sizeof(connectString)/sizeof(wchar_t), 
                    L"DataSourceName=%ls;UserId=;Password=;", 
                    (FdoString*)dsnname
                );
			else if (m_ProviderActive == L"OdbcText")
				swprintf( 
                    connectString, 
                    sizeof(connectString)/sizeof(wchar_t), 
                    L"DataSourceName=%ls;UserId=;Password=;", 
                    (FdoString*)dsnname
                );
		break;
		case Connection_OraSetup:
			swprintf( 
                connectString, 
                sizeof(connectString)/sizeof(wchar_t), 
                L"ConnectionString=\"DRIVER={%ls};UID=%ls;PWD=%ls;DBQ=%ls;;\"", 
                (FdoString*)OracleDriverName, 
                (FdoString*)username, 
                (FdoString*)password, 
                (FdoString*)service
            );
		break;
	}
	return connectString;
}

StaticConnection* OdbcConnectionUtil::NewStaticConnection()
{
	return new OdbcStaticConnection();
}

FdoInsertTest* OdbcConnectionUtil::GetInsertTest ()
{
	return NULL;
}

FdoStringP OdbcConnectionUtil::GetOutputFileName (FdoString* pBaseFileName)
{
	FdoStringP newFileName = m_ProviderActive + m_IdTest;
	newFileName += pBaseFileName;
	return newFileName;
}

#ifdef _WIN32
void OdbcConnectionUtil::SetupTextDSN()
{
    char module[MAX_PATH];
    char teststr[1024];
    DWORD nchars;
    char* last;
    m_SetupTextDSNdone = true;
    nchars = GetModuleFileName (NULL, module, MAX_PATH);
    if (0 != nchars)
    {   
        // scan the string for the last occurrence of a slash
        last = strrchr (module, '\\');
        if (NULL != last)
        {
            last++; // move past the slash
            *last = '\0'; // null terminate it there
			sprintf (teststr, "DSN=%s%cDescription=Test Text datastore for FDO ODBC provider%cDefaultDir=%s%s%c%c", (const char*)(FdoStringP)m_SetupValues->GetPropertyValue( L"DSNText" )
				, '\0', '\0', module, "/Odbc", '\0', '\0');
            if (!SQLConfigDataSource (NULL, ODBC_ADD_DSN, "Microsoft Text Driver (*.txt; *.csv)", teststr))
			{
                DWORD error;
                WORD count;
                SQLInstallerError (1, &error, teststr, sizeof (teststr), &count);
                printf (teststr);
                throw FdoException::Create (L"Text DSN setup failed");
			}
		}
	}
}

void OdbcConnectionUtil::SetupAccessDSN()
{
    char module[MAX_PATH];
    char teststr[1024];
    DWORD nchars;
    char* last;
    m_SetupAccessDSNdone = true;
    nchars = GetModuleFileName (NULL, module, MAX_PATH);
    if (0 != nchars)
    {   
        // scan the string for the last occurrence of a slash
        last = strrchr (module, '\\');
        if (NULL != last)
        {
            last++; // move past the slash
            *last = '\0'; // null terminate it there
            sprintf (teststr, "DSN=%s%cDescription=Test Access datastore for FDO ODBC provider%cDBQ=%sMSTest.mdb%c%c", (const char*)(FdoStringP)m_SetupValues->GetPropertyValue( L"DSNAccess" ), 
				'\0', '\0', module, '\0', '\0');
            if (!SQLConfigDataSource (NULL, ODBC_ADD_DSN, "Microsoft Access Driver (*.mdb)", teststr))
			{
                DWORD error;
                WORD count;
                SQLInstallerError (1, &error, teststr, sizeof (teststr), &count);
                printf (teststr);
                throw FdoException::Create (L"Access DSN setup failed");
			}
		}
	}
}

void OdbcConnectionUtil::SetupDbaseDSN()
{
    char module[MAX_PATH];
    char teststr[1024];
    DWORD nchars;
    char* last;
    m_SetupDbaseDSNdone = true;
    nchars = GetModuleFileName (NULL, module, MAX_PATH);
    if (0 != nchars)
    {   
        // scan the string for the last occurrence of a slash
        last = strrchr (module, '\\');
        if (NULL != last)
        {
            *last = '\0'; // null terminate it there
            FdoStringP dsnNameP = m_SetupValues->GetPropertyValue( L"DSNDbase" );
            const char * dsnName = (const char*)dsnNameP;
            sprintf (teststr, "DSN=%s%cDescription=Test dBASE datastore for FDO ODBC provider%cDefaultDir=%s%c%c", dsnName, 
				'\0', '\0', module, '\0', '\0');
            if (!SQLConfigDataSource (NULL, ODBC_ADD_DSN, "Microsoft dBase Driver (*.dbf)", teststr))
			{
                DWORD error;
                WORD count;
                SQLInstallerError (1, &error, teststr, sizeof (teststr), &count);
                printf (teststr);
                throw FdoException::Create (L"dBASE DSN setup failed");
			}
		}
	}
}

void OdbcConnectionUtil::SetupExcelDSN()
{
    char module[MAX_PATH];
    char teststr[1024];
    DWORD nchars;
    char* last;
    m_SetupExcelDSNdone = true;
    nchars = GetModuleFileName (NULL, module, MAX_PATH);
    if (0 != nchars)
    {   
        // scan the string for the last occurrence of a slash
        last = strrchr (module, '\\');
        if (NULL != last)
        {
            last++; // move past the slash
            *last = '\0'; // null terminate it there
            sprintf (teststr, "DSN=%s%cDescription=Test Excel datastore for FDO ODBC provider%cDBQ=%sMSTest.xls%c%c", (const char*)(FdoStringP)m_SetupValues->GetPropertyValue( L"DSNExcel" ),
				'\0', '\0', module, '\0', '\0');
            if (!SQLConfigDataSource (NULL, ODBC_ADD_DSN, "Microsoft Excel Driver (*.xls)", teststr))
			{
                DWORD error;
                WORD count;
                SQLInstallerError (1, &error, teststr, sizeof (teststr), &count);
                printf (teststr);
                throw FdoException::Create (L"Excel DSN setup failed");
			}
		}
	}
}

void OdbcConnectionUtil::SetupSqlServerDSN()
{
    char teststr[1024];
    sprintf (teststr, "DSN=%s%cDescription=Test SqlServer DSN for FDO ODBC provider%cSERVER=%s%cDATABASE=%ls%c%c", (const char*)(FdoStringP)m_SetupValues->GetPropertyValue( L"DSNSqlServer" ), 
		'\0', '\0', (const char*)(FdoStringP)m_SetupValues->GetPropertyValue( L"serviceSqlServer" ), '\0', (FdoString*)(UnitTestUtil::GetEnviron("datastore", L"")), '\0', '\0', '\0');
    m_SetupSqlServerDSNdone = true;
    if (!SQLConfigDataSource (NULL, ODBC_ADD_DSN, "SQL Server", teststr))
    {
        DWORD error;
        WORD count;
        SQLInstallerError (1, &error, teststr, sizeof (teststr), &count);
        printf (teststr);
        throw FdoException::Create (L"SqlServer DSN setup failed");
    }
}

void OdbcConnectionUtil::SetupMySqlDSN()
{
    char teststr[1024];
    sprintf (teststr, "DSN=%s%cDescription=Test MySql DSN for FDO ODBC provider%cSERVER=%s%cDATABASE=%ls%cOPTION=3%c%c", (const char*)(FdoStringP)m_SetupValues->GetPropertyValue( L"DSNMySql" ), 
		'\0','\0', (const char*)(FdoStringP)m_SetupValues->GetPropertyValue( L"serviceMySql" ), '\0', (FdoString*)(UnitTestUtil::GetEnviron("datastore", L"")), '\0', '\0', '\0');
    m_SetupMySqlDSNdone = true;
    if (!SQLConfigDataSource (NULL, ODBC_ADD_DSN, "MySQL ODBC 3.51 Driver", teststr))
    {
        DWORD error;
        WORD count;
        SQLInstallerError (1, &error, teststr, sizeof (teststr), &count);
        printf (teststr);
        throw FdoException::Create (L"MySql DSN setup failed");
    }

}

#define SQLRETURN_OK(rc)    (SQL_SUCCESS == rc || SQL_SUCCESS_WITH_INFO == rc)
// Establish an Oracle DSN.  Oracle does not have a constant name, but rather
// a formulated one based on installed instance.  We'll look for the substrings
// "Oracle" and "10g", unless the user has overridden this pattern using
// the "odbcoracledriver" environment variable.
void OdbcConnectionUtil::SetupOracleDSN()
{
    char driverDesc[1024];
    char driverAttrs[1024];
    theOracleDriverName[0] = '\0';
	char teststr[1024];
	BOOL ret = false;
	SQLRETURN rc = SQL_ERROR;
    m_SetupOracleDSNdone = true;

    SQLHENV sqlenv = SQL_NULL_HENV;
    rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HENV, &sqlenv);
    if ( SQLRETURN_OK(rc) )
        rc = SQLSetEnvAttr(sqlenv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);

    if ( SQLRETURN_OK(rc) )
    {
        SQLUSMALLINT direction = SQL_FETCH_FIRST;
        SQLSMALLINT driverDescLength = 0;
        SQLSMALLINT driverAttrsLength = 0;
        do
        {
            driverDescLength = 0;
            driverAttrsLength = 0;
            rc = SQLDrivers(sqlenv, direction, (SQLCHAR *) driverDesc, (SQLSMALLINT) sizeof(driverDesc), &driverDescLength,
                (SQLCHAR *) driverAttrs, (SQLSMALLINT) sizeof(driverAttrs), &driverAttrsLength);
            if (SQLRETURN_OK(rc))
            {
                #ifdef WIN32
                #pragma message("TODO: update this with each Oracle version update")
                #endif
                if (NULL != strstr(driverDesc, "Oracle") && (NULL != strstr(driverDesc, "10") || NULL != strstr(driverDesc, "11")))
                    strcpy(theOracleDriverName, driverDesc);
            }
            direction = SQL_FETCH_NEXT;
        }
        while ( SQLRETURN_OK(rc) && SQL_NO_DATA != rc && '\0' == theOracleDriverName[0] );
	    
        if (m_SetupValues->PropertyExist( L"enableOracleSetup" ))
	    {
		    FdoStringP pValue = m_SetupValues->GetPropertyValue( L"enableOracleSetup" );
		    if (pValue != L"true")
			    return;
	    }
        if (SQL_NO_DATA == rc)
            rc = SQL_SUCCESS;
		direction = SQL_FETCH_FIRST;
		FdoStringP pDSNOracle = m_SetupValues->GetPropertyValue( L"DSNOracle");
		while(SQLRETURN_OK(SQLDataSources(sqlenv, direction, (SQLCHAR *)teststr, sizeof(teststr), &driverAttrsLength, (SQLCHAR *)driverDesc, sizeof(driverDesc), &driverDescLength)))
		{
			direction = SQL_FETCH_NEXT;
			if (pDSNOracle == (FdoStringP)teststr)
			{
				SQLFreeHandle(SQL_HANDLE_ENV, sqlenv);
				return;
			}
		}
    }
    if (SQLRETURN_OK(rc) && '\0' != theOracleDriverName[0])
    {
        char* datastoreUpper = driverDesc;
        sprintf(datastoreUpper, "%ls", (FdoString*) UnitTestUtil::GetEnviron("datastore", L""));
        (void) _strupr_s(datastoreUpper, 1024);

        sprintf ( teststr, "DSN=%s%cDescription=Oracle DSN for FDO ODBC provider%cServerName=%s%cUserID=%s%c%c", (const char*)(FdoStringP)m_SetupValues->GetPropertyValue( L"DSNOracle"), '\0',
            '\0', (const char*)(FdoStringP)m_SetupValues->GetPropertyValue( L"serviceOracle" ), '\0', datastoreUpper, '\0', '\0', '\0');
		ret = SQLConfigDataSource (NULL, ODBC_ADD_DSN, theOracleDriverName, teststr);
    }

    if (!SQLRETURN_OK(rc))
    {
    	SQLSMALLINT cRecNmbr = 1;
	    UCHAR		szSqlState[MAX_PATH] = "";
	    UCHAR 	 	szErrorMsg[MAX_PATH] = "";
	    SDWORD		pfNativeError = 0L;
	    SWORD	 	pcbErrorMsg = 0;

		rc = SQLGetDiagRec(SQL_HANDLE_ENV, sqlenv, 1, szSqlState, &pfNativeError, szErrorMsg, MAX_PATH-1, &pcbErrorMsg);
        printf("%.200s\n", (char *)szErrorMsg);
        throw FdoException::Create (L"Oracle DSN setup failed");
    }
	if (sqlenv != SQL_NULL_HENV)
		SQLFreeHandle(SQL_HANDLE_ENV, sqlenv);
    if (!ret )
    {
        DWORD error;
        WORD count;
        SQLInstallerError (1, &error, teststr, sizeof (teststr), &count);
        printf (teststr);
        throw FdoException::Create (L"Oracle DSN setup failed");
    }

    if ('\0' == theOracleDriverName[0])
        throw FdoException::Create (L"Oracle DSN setup failed");
}

void OdbcConnectionUtil::TeardownAccessDSN()
{
    char pString[SQL_MAX_MESSAGE_LENGTH];
    DWORD error;
    WORD count;
	sprintf( pString, "DSN=%s%c%c", (const char*)(FdoStringP)m_SetupValues->GetPropertyValue( L"DSNAccess" ), '\0', '\0');
    if (!SQLConfigDataSource (NULL, ODBC_REMOVE_DSN, "Microsoft Access Driver (*.mdb)", pString))
    {
        SQLInstallerError (1, &error, pString, sizeof (pString), &count);
		printf ("\nAccess DSN teardown failed:\n");
        printf (pString);
    }
}

void OdbcConnectionUtil::TeardownDbaseDSN()
{
    char pString[SQL_MAX_MESSAGE_LENGTH];
    DWORD error;
    WORD count;
    FdoStringP dsnNameP = m_SetupValues->GetPropertyValue( L"DSNDbase" );
    const char * dsnName = (const char*)dsnNameP;
	sprintf( pString, "DSN=%s%c%c", dsnName, '\0', '\0');
    if (!SQLConfigDataSource (NULL, ODBC_REMOVE_DSN, "Microsoft dBase Driver (*.dbf)", pString))
    {
        SQLInstallerError (1, &error, pString, sizeof (pString), &count);
		printf ("\ndBASE DSN teardown failed:\n");
        printf (pString);
    }
}

void OdbcConnectionUtil::TeardownExcelDSN()
{
    char pString[SQL_MAX_MESSAGE_LENGTH];
    DWORD error;
    WORD count;
	sprintf( pString, "DSN=%s%c%c", (const char*)(FdoStringP)m_SetupValues->GetPropertyValue( L"DSNExcel" ), '\0', '\0' );
    if (!SQLConfigDataSource (NULL, ODBC_REMOVE_DSN, "Microsoft Excel Driver (*.xls)", pString))
    {
        SQLInstallerError (1, &error, pString, sizeof (pString), &count);
		printf ("\nExcel DSN teardown failed:\n");
        printf (pString);
    }
}

void OdbcConnectionUtil::TeardownTextDSN()
{
    char pString[SQL_MAX_MESSAGE_LENGTH];
    DWORD error;
    WORD count;
	sprintf( pString, "DSN=%s%c%c", (const char*)(FdoStringP)m_SetupValues->GetPropertyValue( L"DSNText" ), '\0', '\0');
    if (!SQLConfigDataSource (NULL, ODBC_REMOVE_DSN, "Microsoft Text Driver (*.txt; *.csv)", pString))
    {
        SQLInstallerError (1, &error, pString, sizeof (pString), &count);
		printf ("\nText DSN teardown failed:\n");
        printf (pString);
    }
}

void OdbcConnectionUtil::TeardownSqlServerDSN()
{
    char pString[SQL_MAX_MESSAGE_LENGTH];
    DWORD error;
    WORD count;
	sprintf( pString, "DSN=%s%c%c", (const char*)(FdoStringP)m_SetupValues->GetPropertyValue( L"DSNSqlServer" ), '\0', '\0');
    if (!SQLConfigDataSource (NULL, ODBC_REMOVE_DSN, "SQL Server", pString))
    {
        SQLInstallerError (1, &error, pString, sizeof (pString), &count);
		printf ("\nSqlServer DSN teardown failed:\n");
        printf (pString);
    }
}

void OdbcConnectionUtil::TeardownMySqlDSN()
{
    char pString[SQL_MAX_MESSAGE_LENGTH];
    DWORD error;
    WORD count;
	sprintf( pString, "DSN=%s%c%c", (const char*)(FdoStringP)m_SetupValues->GetPropertyValue( L"DSNMySql" ), '\0', '\0');
    if (!SQLConfigDataSource (NULL, ODBC_REMOVE_DSN, "MySQL ODBC 3.51 Driver", pString))
    {
        SQLInstallerError (1, &error, pString, sizeof (pString), &count);
		printf ("\nMySql DSN teardown failed:\n");
        printf (pString);
    }
}

void OdbcConnectionUtil::TeardownOracleDSN()
{
	if (m_SetupValues->PropertyExist( L"enableOracleSetup" ))
	{
		FdoStringP pValue = m_SetupValues->GetPropertyValue( L"enableOracleSetup" );
		if (pValue != L"true")
			return;
	}
    char pString[SQL_MAX_MESSAGE_LENGTH];
    DWORD error;
    WORD count;
	sprintf( pString, "DSN=%s%c%c", (const char*)(FdoStringP)m_SetupValues->GetPropertyValue( L"DSNOracle" ), '\0', '\0');
    if (!SQLConfigDataSource (NULL, ODBC_REMOVE_DSN, theOracleDriverName, pString))
    {
        SQLInstallerError (1, &error, pString, sizeof (pString), &count);
		printf ("\nOracle DSN teardown failed:\n");
        printf (pString);
    }
}
#endif
