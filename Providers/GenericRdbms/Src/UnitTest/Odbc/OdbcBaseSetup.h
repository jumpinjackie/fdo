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

#ifndef ODBC_BASESETUP_H
#define ODBC_BASESETUP_H

enum DataBaseType {
    DataBaseType_MySQL,
    DataBaseType_SqlServer,
    DataBaseType_Oracle
};


class OdbcBaseSetup
{
	DataBaseType m_typeDB;
public:
	// 0 - MySQL, 1 - SqlServer
	OdbcBaseSetup(DataBaseType typeDB = DataBaseType_MySQL)
	{
		m_typeDB = typeDB;
	}
	void SetTypeDB(DataBaseType typeDB = DataBaseType_MySQL)
	{
		m_typeDB = typeDB;
	}
	DataBaseType GetTypeDB()
	{
		return m_typeDB;
	}

	virtual void CreateDataStore(FdoIConnection* pConnection, const char *suffix);
	virtual void DestroyDataStore(FdoIConnection* pConnection, const char *suffix);
	virtual void CreateAcadSchema(FdoIConnection* pConnection);

	static const wchar_t* mSqlServerAcadTest[];
	static const wchar_t* mMySqlAcadTest[];
	static const wchar_t* mOracleAcadTestData[];

	static bool OracleDataSupportCreated;
};

#endif // ODBC_BASESETUP_H
