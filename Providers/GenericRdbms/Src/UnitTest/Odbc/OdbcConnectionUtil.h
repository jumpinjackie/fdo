#ifndef ODBC_CONNECTIONUTIL_H
#define ODBC_CONNECTIONUTIL_H

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

#include "ConnectionUtil.h"
#include "StringPropertiesDictionary.h"

#ifdef _WIN32
#include "../../ODBCDriver/context.h"
#endif

class OdbcStaticConnection : public StaticConnection
{
public:
    int do_rdbi_init (rdbi_context_def** rdbi_context);
    int do_rdbi_connect (rdbi_context_def* rdbi_context, int& id);
    virtual FdoSchemaManagerP CreateSchemaManager();
    virtual const char* GetServiceName();
};


class OdbcConnectionUtil : public ConnectionUtil
{
private:
    FdoPtr<StringPropertiesDictionary> m_SetupValues;
    FdoStringP m_IdTest;
    FdoStringP m_ProviderActive;
    char theOracleDriverName[1024];
    FdoStringP ACCESS_ODBC_DRIVER_NAME;
    FdoStringP EXCEL_ODBC_DRIVER_NAME;
    FdoStringP TEXT_ODBC_DRIVER_NAME;
    FdoStringP DBASE_ODBC_DRIVER_NAME;
    FdoStringP MYSQL_ODBC_DRIVER_NAME;

public:
	OdbcConnectionUtil(void);
	virtual ~OdbcConnectionUtil(void);
	void LoadInitializeFile();

	virtual void SetProvider( const char *providerName );
	virtual StaticConnection* NewStaticConnection ();
	virtual FdoInsertTest* GetInsertTest ();
	virtual FdoStringP GetOutputFileName (FdoString* pBaseFileName);
	virtual wchar_t* GetConnectionString(StringConnTypeRequest pTypeReq = Connection_WithDatastore, FdoString *suffix = L"", bool bAddExtraneousSpaces = false);
	virtual const char* GetSuffixTest(){return "_odbc";};

public:
    FdoString* GetAccessODBCDriverName() {return ACCESS_ODBC_DRIVER_NAME;};
    FdoString* GetExcelODBCDriverName() {return EXCEL_ODBC_DRIVER_NAME;};
    FdoString* GetTextODBCDriverName() {return TEXT_ODBC_DRIVER_NAME;};
    FdoString* GetDbaseODBCDriverName() {return DBASE_ODBC_DRIVER_NAME;};
    FdoString* GetMySQLODBCDriverName() {return MYSQL_ODBC_DRIVER_NAME;};

#ifdef _WIN32
    const char* GetTestDataPath();
#endif

protected:
	void CleanFiles(std::vector<std::wstring>& files, FdoStringP& pTypeName);
#ifdef _WIN32
	bool m_SetupTextDSNdone;
	bool m_SetupAccessDSNdone;
	bool m_SetupDbaseDSNdone;
	bool m_SetupExcelDSNdone;
	bool m_SetupSqlServerDSNdone;
	bool m_SetupMySqlDSNdone;
	bool m_SetupOracleDSNdone;
	char m_testDataPath[MAX_PATH];

	void SetupTextDSN();
	void SetupAccessDSN();
	void SetupDbaseDSN();
	void SetupExcelDSN();
	void SetupSqlServerDSN();
	void SetupMySqlDSN();
	void SetupOracleDSN();

	void TeardownAccessDSN();
	void TeardownDbaseDSN();
	void TeardownExcelDSN();
	void TeardownTextDSN();
	void TeardownSqlServerDSN();
	void TeardownMySqlDSN();
	void TeardownOracleDSN();
#endif
};

#endif // ODBC_CONNECTIONUTIL_H
