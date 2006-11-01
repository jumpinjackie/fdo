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
    DataBaseType_None,
    DataBaseType_MySQL,
    DataBaseType_SqlServer,
    DataBaseType_Oracle
};


class OdbcBaseSetup
{
	DataBaseType m_typeDB;
    bool         m_ReverseEngineersToUppercase;
public:
	OdbcBaseSetup()
        : m_typeDB(DataBaseType_None), m_ReverseEngineersToUppercase(true)
	{
	}
	OdbcBaseSetup(DataBaseType typeDB)
        : m_typeDB(typeDB), m_ReverseEngineersToUppercase(true)
	{
        if (DataBaseType_MySQL==m_typeDB || DataBaseType_SqlServer==m_typeDB)
            m_ReverseEngineersToUppercase = false;
	}
	void SetTypeDB(DataBaseType typeDB)
	{
		m_typeDB = typeDB;
        if (DataBaseType_MySQL==m_typeDB || DataBaseType_SqlServer==m_typeDB)
            m_ReverseEngineersToUppercase = false;
	}
	DataBaseType GetTypeDB()
	{
		return m_typeDB;
	}
    bool LikesUC()
    {
        return m_ReverseEngineersToUppercase;
    }

    FdoString * GetClassNameCities()            { return LikesUC() ? L"CITIES" : L"cities"; }
    FdoString * GetPropertyNameCitiesCityId()   { return LikesUC() ? L"CITYID" : L"cityid"; }
    FdoString * GetPropertyNameCitiesName()     { return LikesUC() ? L"NAME"   : L"name"; }
    FdoString * GetPropertyNameCitiesCity()     { return LikesUC() ? L"CITY"   : L"city"; }
	FdoString * GetPropertyNameCitiesDate()     { return LikesUC() ? L"CDATE"   : L"cdate"; }

    FdoString * GetClassNameTable1()            { return LikesUC() ? L"TABLE1"  : L"table1"; }
    FdoString * GetPropertyNameTable1FeatId()   { return LikesUC() ? L"FEATID1" : L"featid1"; }
    FdoString * GetPropertyNameTable1Name()     { return LikesUC() ? L"NAME"    : L"name"; }

    FdoString * GetClassNameAcdb3dpolyline()    { return LikesUC() ? L"ACDB3DPOLYLINE" : L"acdb3dpolyline"; }
    FdoString * GetPropertyNameAcdb3dpolylineFeatId()   { return LikesUC() ? L"FEATID"        : L"featid"; }
    FdoString * GetPropertyNameAcdb3dpolylineClassId()   { return LikesUC() ? L"CLASSID" : L"classid"; }
    FdoString * GetPropertyNameAcdb3dpolylineRevision()  { return LikesUC() ? L"REVISIONNUMBER" : L"revisionnumber"; }
	FdoString * GetPropertyNameAcdb3dpolylineDateTime()  { return LikesUC() ? L"DATETIME" : L"datetime1"; }

	virtual void CreateDataStore(FdoIConnection* pConnection, const char *suffix);
	virtual void DestroyDataStore(FdoIConnection* pConnection, const char *suffix);
	virtual void CreateAcadSchema(FdoIConnection* pConnection);
	virtual void CreateNonAcadSchema(FdoIConnection* pConnection);

	static const wchar_t* mSqlServerAcadTest[];
    static const wchar_t* mSqlServerNonAcadTest[];
	static const wchar_t* mMySqlAcadTest[];
	static const wchar_t* mMySqlNonAcadTest[];
	static const wchar_t* mOracleAcadTestData[];
	static const wchar_t* mOracleNonAcadTest[];

	static bool OracleDataSupportCreated;
};

#endif // ODBC_BASESETUP_H
