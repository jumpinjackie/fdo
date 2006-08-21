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
#include "OdbcBaseSetup.h"
#include "UnitTestUtil.h"

bool OdbcBaseSetup::OracleDataSupportCreated = false;

void OdbcBaseSetup::DestroyDataStore(FdoIConnection* pConnection, const char *suffix)
{
	const char *pDatastore = UnitTestUtil::GetEnviron("datastore", suffix);

	FdoStringP pQueryStringDrp;
	if (DataBaseType_Oracle == m_typeDB)
	{
		pQueryStringDrp = L"DROP USER ";
		pQueryStringDrp += pDatastore;
		pQueryStringDrp = pQueryStringDrp.Upper();
		pQueryStringDrp += L" CASCADE";
	}
	else
	{
		pQueryStringDrp = L"DROP DATABASE ";
		pQueryStringDrp += pDatastore;
	}
	pQueryStringDrp += L";";
	try
	{
		UnitTestUtil::Sql2Db( (const wchar_t*) pQueryStringDrp, pConnection );
	}
	catch( FdoException *ex )
	{
		// In case of exception ignore it.
		ex->Release();
	}
}

void OdbcBaseSetup::CreateDataStore(FdoIConnection* pConnection, const char *suffix)
{
	if (DataBaseType_Oracle == m_typeDB && OdbcBaseSetup::OracleDataSupportCreated == true)
		return;
	const char *pDatastore = UnitTestUtil::GetEnviron("datastore", suffix);
	// clean the before start
	DestroyDataStore(pConnection, suffix);

	FdoStringP pQueryStringDrp;
	if (DataBaseType_Oracle == m_typeDB)
	{
		OdbcBaseSetup::OracleDataSupportCreated = true;
		FdoStringP pUDatastore = pDatastore;
		pUDatastore = pUDatastore.Upper();
		pQueryStringDrp = (FdoStringP)L"CREATE USER \"" + pUDatastore + L"\"  PROFILE \"DEFAULT\" IDENTIFIED BY \"";
		pQueryStringDrp += UnitTestUtil::GetEnviron("password");
		pQueryStringDrp += L"\" DEFAULT TABLESPACE \"USERS\" TEMPORARY TABLESPACE \"TEMP\" ACCOUNT UNLOCK;";
		UnitTestUtil::Sql2Db( (const wchar_t*) pQueryStringDrp, pConnection );
		pQueryStringDrp = (FdoStringP)L"GRANT ALTER SESSION TO " + pUDatastore;
		UnitTestUtil::Sql2Db( (const wchar_t*) pQueryStringDrp, pConnection );
		pQueryStringDrp = (FdoStringP)L"GRANT CREATE PROCEDURE TO " + pUDatastore;
		UnitTestUtil::Sql2Db( (const wchar_t*) pQueryStringDrp, pConnection );
		pQueryStringDrp = (FdoStringP)L"GRANT CREATE SEQUENCE TO " + pUDatastore;
		UnitTestUtil::Sql2Db( (const wchar_t*) pQueryStringDrp, pConnection );
		pQueryStringDrp = (FdoStringP)L"GRANT CREATE SESSION TO " + pUDatastore;
		UnitTestUtil::Sql2Db( (const wchar_t*) pQueryStringDrp, pConnection );
		pQueryStringDrp = (FdoStringP)L"GRANT CREATE SYNONYM TO " + pUDatastore;
		UnitTestUtil::Sql2Db( (const wchar_t*) pQueryStringDrp, pConnection );
		pQueryStringDrp = (FdoStringP)L"GRANT CREATE TABLE TO " + pUDatastore;
		UnitTestUtil::Sql2Db( (const wchar_t*) pQueryStringDrp, pConnection );
		pQueryStringDrp = (FdoStringP)L"GRANT CREATE VIEW TO " + pUDatastore;
		UnitTestUtil::Sql2Db( (const wchar_t*) pQueryStringDrp, pConnection );
		pQueryStringDrp = (FdoStringP)L"GRANT UNLIMITED TABLESPACE TO " + pUDatastore;
		UnitTestUtil::Sql2Db( (const wchar_t*) pQueryStringDrp, pConnection );
		pQueryStringDrp = (FdoStringP)L"GRANT \"WM_ADMIN_ROLE\" TO " + pUDatastore;
		UnitTestUtil::Sql2Db( (const wchar_t*) pQueryStringDrp, pConnection );
		pQueryStringDrp = (FdoStringP)L"GRANT DELETE ON \"MDSYS\".\"SDO_GEOM_METADATA_TABLE\" TO " + pUDatastore;
		UnitTestUtil::Sql2Db( (const wchar_t*) pQueryStringDrp, pConnection );
		pQueryStringDrp = (FdoStringP)L"GRANT INSERT ON \"MDSYS\".\"SDO_GEOM_METADATA_TABLE\" TO " + pUDatastore;
		UnitTestUtil::Sql2Db( (const wchar_t*) pQueryStringDrp, pConnection );
		pQueryStringDrp = (FdoStringP)L"GRANT SELECT ON \"MDSYS\".\"SDO_GEOM_METADATA_TABLE\" TO " + pUDatastore;
		UnitTestUtil::Sql2Db( (const wchar_t*) pQueryStringDrp, pConnection );
		pQueryStringDrp = (FdoStringP)L"GRANT UPDATE ON \"MDSYS\".\"SDO_GEOM_METADATA_TABLE\" TO " + pUDatastore;
		UnitTestUtil::Sql2Db( (const wchar_t*) pQueryStringDrp, pConnection );

		pConnection->Close();
		pConnection->SetConnectionString ( UnitTestUtil::GetConnectionString(Connection_NoDatastore, "") );
		pConnection->Open();
		UnitTestUtil::Sql2Db( (const wchar_t**) mOracleAcadTestData, pConnection );
	}
	else
	{
		pQueryStringDrp = L"CREATE DATABASE ";
		pQueryStringDrp += pDatastore;
		UnitTestUtil::Sql2Db( (const wchar_t*) pQueryStringDrp, pConnection );
	}
	
	pConnection->Close();
	pConnection->SetConnectionString ( UnitTestUtil::GetConnectionString(Connection_WithDatastore, suffix) );
	pConnection->Open();
}

void OdbcBaseSetup::CreateAcadSchema(FdoIConnection* pConnection)
{
	if (DataBaseType_MySQL == m_typeDB)
		UnitTestUtil::Sql2Db( (const wchar_t**) mMySqlAcadTest, pConnection );
	else if (DataBaseType_SqlServer == m_typeDB)
		UnitTestUtil::Sql2Db( (const wchar_t**) mSqlServerAcadTest, pConnection );
}

const wchar_t* OdbcBaseSetup::mOracleAcadTestData[] = {
    L"CREATE TABLE ACDB3DPOLYLINE_LT (",
    L"LAYER VARCHAR2(10), ",
    L"COLOR VARCHAR2(32), ",
    L"SEGCOUNT NUMBER(10), ",
    L"BOOLEAN NUMBER(1), ",
    L"BYTE NUMBER(3), ",
    L"DATETIME DATE, ",
    L"DECIMAL1 NUMBER(10, 2), ",
    L"DOUBLE NUMBER, ",
    L"INT16 NUMBER(5), ",
    L"INT32 NUMBER(10), ",
    L"INT64 NUMBER(20), ",
    L"SINGLE NUMBER, ",
    L"CLASSID NUMBER(20) NOT NULL, ",
    L"REVISIONNUMBER NUMBER NOT NULL,", 
    L"GEOMETRY MDSYS.SDO_GEOMETRY, ",
    L"FEATID NUMBER(20) NOT NULL, ",
    L"VERSION NUMBER, ",
    L"NEXTVER VARCHAR2(500), ",
	L"DELSTATUS NUMBER, ",
    L"LTLOCK VARCHAR2(100), ",
    L"CONSTRAINT PK_ACDB3DPOLYLINE PRIMARY KEY(VERSION, FEATID));",
	L"",
	L"CREATE TABLE TESTCLASS_LT (",
	L"AGE NUMBER(10) NOT NULL, ",
	L"FIRSTNAME VARCHAR2(32) NOT NULL,",
	L"LASTNAME VARCHAR2(32) NOT NULL, ",
	L"VERSION NUMBER, ",
	L"NEXTVER VARCHAR2(500), ",
	L"DELSTATUS NUMBER, ",
	L"LTLOCK VARCHAR2(100), ",
	L"CONSTRAINT PK_TESTCLASS PRIMARY KEY(VERSION, LASTNAME, FIRSTNAME));",
	L"",
	L"insert into TESTCLASS_LT (AGE, FIRSTNAME, LASTNAME, VERSION, NEXTVER, DELSTATUS ) values (36, 'UserFN', 'UserLN', 0, -1, 10 );",
	L"",
    L"insert into ACDB3DPOLYLINE_LT (LAYER, COLOR, SEGCOUNT, CLASSID, REVISIONNUMBER, FEATID, VERSION, NEXTVER, DELSTATUS ) values (200, 256, 7, 11, 0, 2, 0, -1, 10);",
	L"",
    L"insert into ACDB3DPOLYLINE_LT (LAYER, COLOR, SEGCOUNT, CLASSID, REVISIONNUMBER, FEATID, VERSION, NEXTVER, DELSTATUS ) values (200, 256, 5, 11, 0, 8, 0, -1, 10);",
	L"",
	NULL
};

const wchar_t* OdbcBaseSetup::mMySqlAcadTest[] = {
	L"create table acdb3dpolyline (",
	L"layer varchar(10) default NULL,",
	L"color varchar(32) default NULL,",
	L"segcount int default NULL,",
	L"boolean tinyint default NULL,",
	L"byte tinyint unsigned default NULL,",
	L"datetime1 datetime default NULL,",
	L"decimal1 decimal default NULL,",
	L"double1 double default NULL,",
	L"int16 smallint default NULL,",
	L"int32 int default NULL,",
	L"int64 bigint default NULL,",
	L"single1 double default NULL,",
	L"classid bigint NOT NULL,",
	L"revisionnumber double NOT NULL,",
	L"geometry geometry default NULL,",
	L"featid bigint NOT NULL auto_increment,",
	L"PRIMARY KEY (featid)",
	L") ENGINE=MyISAM;",
    L"",
	L"create table acdbentity (",
	L"classid bigint NOT NULL,",
	L"revisionnumber double NOT NULL,",
	L"single1 double default NULL,",
	L"int64 bigint default NULL,",
	L"int32 int default NULL,",
	L"int16 smallint default NULL,",
	L"double1 double default NULL,",
	L"decimal1 decimal default NULL,",
	L"datetime1 datetime default NULL,",
	L"byte tinyint unsigned default NULL,",
	L"boolean tinyint default NULL,",
	L"segcount int default NULL,",
	L"color varchar(32) default NULL,",
	L"layer varchar(10) default NULL,",
	L"featid bigint NOT NULL auto_increment,",
	L"PRIMARY KEY (featid)",
	L") ENGINE=MyISAM;",
    L"",
	L"create table acdbentity_acxdata (",
	L"seq int NOT NULL,",
	L"datavalue text NOT NULL,",
	L"acdbentity_featid bigint NOT NULL,",
	L"PRIMARY KEY (acdbentity_featid)",
	L") ENGINE=MyISAM;",
    L"",
	L"create table acdbentity_acxdata_acxobj (",
	L"intdata int default NULL,",
	L"strdata text,",
	L"autogen1 bigint default NULL,",
	L"autogen2 bigint default NULL,",
	L"acdbentity_acxdata_acdbentity_featid bigint NOT NULL,",
	L"PRIMARY KEY (acdbentity_acxdata_acdbentity_featid)",
	L") ENGINE=MyISAM;",
    L"",
	L"create table acdbentity_acxdata2 (",
	L"seq int NOT NULL,",
	L"datavalue text NOT NULL,",
	L"acdbentity_featid bigint NOT NULL,",
	L"PRIMARY KEY (acdbentity_featid,seq)",
	L") ENGINE=MyISAM;",
    L"",
	L"create table testClass (",
	L"age int NOT NULL,",
	L"firstname varchar(32) NOT NULL,",
	L"lastname varchar(32) NOT NULL,",
	L"PRIMARY KEY (lastname,firstname)",
	L") ENGINE=MyISAM;",
    L"",
	L"create table testClass_objectclass (",
	L"objectweight int NOT NULL,",
	L"testClass_lastname varchar(32) NOT NULL,",
	L"testClass_firstname varchar(32) NOT NULL,",
	L"PRIMARY KEY (testClass_lastname,testClass_firstname)",
	L") ENGINE=MyISAM;",
    L"",
	L"create table testClass_objectclass2 (",
	L"colours int NOT NULL,",
	L"testClass_lastname varchar(32) NOT NULL,",
	L"testClass_firstname varchar(32) NOT NULL,",
	L"PRIMARY KEY (testClass_lastname,testClass_firstname,colours)",
	L") ENGINE=MyISAM;",
    NULL
};

const wchar_t* OdbcBaseSetup::mSqlServerAcadTest[] = {
	L"create table acdb3dpolyline (",
	L"layer varchar(10) NULL,",
	L"color varchar(32) NULL,",
	L"segcount int NULL,",
	L"boolean tinyint NULL,",
	L"byte tinyint NULL,",
	L"datetime1 datetime NULL,",
	L"decimal1 decimal NULL,",
	L"double1 real NULL,",
	L"int16 smallint NULL,",
	L"int32 int NULL,",
	L"int64 bigint NULL,",
	L"single1 real NULL,",
	L"classid bigint NOT NULL,",
	L"revisionnumber real NOT NULL,",
	L"geometry image NULL,",
	L"GEOMETRY_SI_1 varchar (255) NULL,",
	L"GEOMETRY_SI_2 varchar (255) NULL,",
	L"featid bigint IDENTITY NOT NULL",
	L");",
    L"",
	L"create table acdbentity (",
	L"classid bigint NOT NULL,",
	L"revisionnumber real NOT NULL,",
	L"single1 real NULL,",
	L"int64 bigint NULL,",
	L"int32 int NULL,",
	L"int16 smallint NULL,",
	L"double1 real NULL,",
	L"decimal1 decimal NULL,",
	L"datetime1 datetime NULL,",
	L"byte tinyint NULL,",
	L"boolean tinyint NULL,",
	L"segcount int NULL,",
	L"color varchar(32) NULL,",
	L"layer varchar(10) NULL,",
	L"featid bigint IDENTITY NOT NULL",
	L");",
    L"",
	L"create table acdbentity_acxdata (",
	L"seq int NOT NULL,",
	L"datavalue text NOT NULL,",
	L"acdbentity_featid bigint NOT NULL",
	L");",
    L"",
	L"create table acdbentity_acxdata_acxobj (",
	L"intdata int NULL,",
	L"strdata text,",
	L"autogen1 bigint NULL,",
	L"autogen2 bigint NULL,",
	L"acdbentity_acxdata_acdbentity_featid bigint NOT NULL",
	L");",
    L"",
	L"create table acdbentity_acxdata2 (",
	L"seq int NOT NULL,",
	L"datavalue text NOT NULL,",
	L"acdbentity_featid bigint NOT NULL",
	L");",
    L"",
	L"create table testClass (",
	L"age int NOT NULL,",
	L"firstname varchar(32) NOT NULL,",
	L"lastname varchar(32) NOT NULL",
	L");",
    L"",
	L"create table testClass_objectclass (",
	L"objectweight int NOT NULL,",
	L"testClass_lastname varchar(32) NOT NULL,",
	L"testClass_firstname varchar(32) NOT NULL",
	L");",
    L"",
	L"create table testClass_objectclass2 (",
	L"colours int NOT NULL,",
	L"testClass_lastname varchar(32) NOT NULL,",
	L"testClass_firstname varchar(32) NOT NULL",
	L");",
    NULL
};
