// ProviderConfigInfo.h
// objects of this class take a configuration filename as a constructor arg
// the file is parsed and string values returned via methods whose name includes
// property names which are significant for registering a provider and connecting to it

#ifndef _PROVIDERCONFIGINFO_H_
#define _PROVIDERCONFIGINFO_H_

//
// Copyright (C) 2008  Autodesk, Inc.
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#include "Logger.h"
#include "conversions.h"
#include <wchar.h>
#include <istream>
#include <fstream>
#include <string>
#include <cstring>
#include <iostream>

using namespace std;

class ProviderConfigInfo {
public:
	ProviderConfigInfo(char * filename, Logger *log, Conversions * convert);
	~ProviderConfigInfo();

	const wchar_t * getName();
	const wchar_t * getServer();
	const wchar_t * getService();
	const wchar_t * getInstance();
	const wchar_t * getUsername();
	const wchar_t * getPassword();
	const wchar_t * getDatastore();
	const char * getSchemaDefFileName();
	const char * getSchemaInsertFileName();
	const char * getSchemaDeleteFileName();
	const wchar_t * getGeneratedXmlSchemaFileName();
	const wchar_t * getEditedXmlSchemaFileName();
	const wchar_t * getEditedXmlSchemaName();
	const wchar_t * getValue(const wchar_t * name);
	const wchar_t * getFile();
	const wchar_t * getReadOnly();
	const wchar_t * getSpatialContextName();
	const wchar_t * getSpatialContextDesc();
	const wchar_t * getSrsWkt();
	double getXYTolerance();
	double getZTolerance();
	const wchar_t * getDefaultFileLocation();
	const wchar_t * getTemporaryFileLocation();
	const wchar_t * getDataSourceName(); // ODBC
	const wchar_t * getUserId(); // ODBC
	const wchar_t * getConnectionString(); // ODBC

private:
	Logger * log;
	Conversions * convert;
	string * filename;
	wstring * name;
	wstring * server;
	wstring * service;
	wstring * instance;
	wstring * username;
	wstring * password;
	wstring * datastore;
	string * schemaDefFileName;
	string * schemaInsertFileName;
	string * schemaDeleteFileName;
	wstring * generatedXmlSchemaFileName;
	wstring * editedXmlSchemaName;
	wstring * editedXmlSchemaFileName;
	wstring * File;
	wstring * ReadOnly;
	wstring * spatialContextName;
	wstring * spatialContextDesc;
	wstring * srsWkt;
	double XYTolerance;
	double ZTolerance;
	//SHP connection attributes
	wstring * DefaultFileLocation;
	wstring * TemporaryFileLocation;
	wstring * DataSourceName;
	wstring * UserId;
	wstring * ConnectionString;

	char errString[256];
	wchar_t formatWstringBuf[512];
	char formatStringBuf[256];

	void initStore();
	int parseFile();
	void dumpGetOutputToLog();
	int storeNameValue(char * name, char * value);
};

#endif

