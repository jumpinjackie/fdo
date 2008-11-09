// ProviderConfigInfo.cpp
//
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
//

#include "stdafx.h"
#include "ProviderConfigInfo.h"

#ifndef _WIN32
#define _wcsicmp	wcscasecmp
#endif

// if parseFile can't open file or the file contents are incorrect,
// the constructor should throw an exception
ProviderConfigInfo::ProviderConfigInfo(char * filename, Logger * log, Conversions * convert) {
	int retval = 0;
	this->filename = new string(filename);
	this->log = log;
	this->convert = convert;
	initStore();
	retval = parseFile();
	if (retval) throw (errString);
	dumpGetOutputToLog();
}

void
ProviderConfigInfo::initStore() {
	name = NULL;
	server = NULL;
	service = NULL;
	instance = NULL;
	username = NULL;
	password = NULL;
	datastore = NULL;
	schemaDefFileName = NULL;
	schemaInsertFileName = NULL;
	schemaDeleteFileName = NULL;
	generatedXmlSchemaFileName = NULL;
	editedXmlSchemaFileName = NULL;
	editedXmlSchemaName = NULL;
	File = NULL;
	ReadOnly = NULL;
	spatialContextName = NULL;
	spatialContextDesc = NULL;
	srsWkt = NULL;
	DefaultFileLocation = NULL;
	TemporaryFileLocation = NULL;
	XYTolerance = 0.0;
	ZTolerance = 0.0;
	DataSourceName = NULL;
	UserId = NULL;
	ConnectionString = NULL;
}

ProviderConfigInfo::~ProviderConfigInfo() {
	delete filename;
	delete name;
	delete server;
	delete service;
	delete instance;
	delete username;
	delete password;
	delete datastore;
	delete schemaDefFileName;
	delete schemaInsertFileName;
	delete schemaDeleteFileName;
	delete generatedXmlSchemaFileName;
	delete editedXmlSchemaFileName;
	delete editedXmlSchemaName;
	delete File;
	delete ReadOnly;
	delete spatialContextName;
	delete spatialContextDesc;
	delete srsWkt;
	delete DefaultFileLocation;
	delete TemporaryFileLocation;
	delete DataSourceName;
	delete UserId;
	delete ConnectionString;
}

int
ProviderConfigInfo::parseFile() {
	char line[256];
	char *name;
	char *value;
	int retval = 0;
	ifstream myFile;
	myFile.open(filename->c_str());
   if( myFile.is_open() ) {
	   log->write("Parsing", filename->c_str());
       while ( myFile.good( ) ) {
			myFile.getline(line, 256);
			if (strlen(line)) {
				log->write(line);
				name = strtok(line, "=");
				log->write("name =", name);
				value = strtok((char*)NULL, "\n");
 				log->write("value =", value);
				retval = storeNameValue(name, value);
				if (retval) {
					log->timestr();
					log->str(errString);
					log->str(name);
					log->str(value);
					log->endline();
					return retval;
				}
			}
	   }
		if (retval) {
			log->timestr();
			log->str(errString);
			log->endline();
			return retval;
		}
   } else {
	   sprintf(errString, "ERROR: Cannot open file: %s", filename->c_str());
	   log->write("ERROR: Cannot open file:", filename->c_str());
	   return 1;
   }
   return retval;
}

const wchar_t *
ProviderConfigInfo::getName() {
	return name->c_str();
}

const wchar_t *
ProviderConfigInfo::getServer() {
	return server->c_str();
}

const wchar_t *
ProviderConfigInfo::getService() {
	return service->c_str();
}

const wchar_t *
ProviderConfigInfo::getInstance() {
	return instance->c_str();
}

const wchar_t *
ProviderConfigInfo::getUsername() {
	return username->c_str();
}

const wchar_t *
ProviderConfigInfo::getPassword() {
	return password->c_str();
}

const wchar_t *
ProviderConfigInfo::getDatastore() {
	return datastore->c_str();
}

const char *
ProviderConfigInfo::getSchemaDefFileName() {
	return schemaDefFileName->c_str();
}

const wchar_t *
ProviderConfigInfo::getGeneratedXmlSchemaFileName() {
	return generatedXmlSchemaFileName->c_str();
}

const wchar_t *
ProviderConfigInfo::getEditedXmlSchemaName() {
	return editedXmlSchemaName->c_str();
}

const wchar_t *
ProviderConfigInfo::getEditedXmlSchemaFileName() {
	return editedXmlSchemaFileName->c_str();
}

const char *
ProviderConfigInfo::getSchemaInsertFileName() {
	return schemaInsertFileName->c_str();
}

const char *
ProviderConfigInfo::getSchemaDeleteFileName() {
	return schemaDeleteFileName->c_str();
}

const wchar_t *
ProviderConfigInfo::getFile() {
	return File->c_str();
}

const wchar_t *
ProviderConfigInfo::getReadOnly() {
	return ReadOnly->c_str();
}

const wchar_t *
ProviderConfigInfo::getSpatialContextName() {
	return spatialContextName->c_str();
}

const wchar_t *
ProviderConfigInfo::getSpatialContextDesc() {
	return spatialContextDesc->c_str();
}

const wchar_t *
ProviderConfigInfo::getSrsWkt() {
	return srsWkt->c_str();
}

double
ProviderConfigInfo::getXYTolerance() {
	return XYTolerance;
}

double
ProviderConfigInfo::getZTolerance() {
	return ZTolerance;
}

const wchar_t *
ProviderConfigInfo::getDefaultFileLocation() {
	return DefaultFileLocation->c_str();
}

const wchar_t *
ProviderConfigInfo::getTemporaryFileLocation() {
	return TemporaryFileLocation->c_str();
}

const wchar_t *
ProviderConfigInfo::getDataSourceName() {
	return DataSourceName->c_str();
}

const wchar_t *
ProviderConfigInfo::getUserId() {
	return UserId->c_str();
}

const wchar_t *
ProviderConfigInfo::getConnectionString() {
	return ConnectionString->c_str();
}

const wchar_t *
ProviderConfigInfo::getValue(const wchar_t * name) {
#ifdef _WIN32
	if (_wcsicmp(name, L"datastore") == 0) {
#else
	if (_wcscasecmp(name, L"datastore") == 0) {
#endif
		return getDatastore();
#ifdef _WIN32
	} else if (_wcsicmp(name, L"server") == 0) {
#else
	} else if (_wcscasecmp(name, L"server") == 0) {
#endif
		return getServer();
#ifdef _WIN32
	} else if (_wcsicmp(name, L"instance") == 0) {
#else
	} else if (_wcscasecmp(name, L"instance") == 0) {
#endif
		return getInstance();
#ifdef _WIN32
	} else if (_wcsicmp(name, L"username") == 0) {
#else
	} else if (_wcscasecmp(name, L"username") == 0) {
#endif
		return getUsername();
#ifdef _WIN32
	} else if (_wcsicmp(name, L"password") == 0) {
#else
	} else if (_wcscasecmp(name, L"password") == 0) {
#endif
		return getPassword();
#ifdef _WIN32
	} else if (_wcsicmp(name, L"service") == 0) {
#else
	} else if (_wcscasecmp(name, L"service") == 0) {
#endif
		return getService();
#ifdef _WIN32
	} else if (_wcsicmp(name, L"file") == 0) {
#else
	} else if (_wcscasecmp(name, L"file") == 0) {
#endif
		return getFile();
#ifdef _WIN32
	} else if (_wcsicmp(name, L"readonly") == 0) {
#else
	} else if (_wcscasecmp(name, L"readonly") == 0) {
#endif
		return getReadOnly();
#ifdef _WIN32
	} else if (_wcsicmp(name, L"defaultfilelocation") == 0) {
#else
	} else if (_wcscasecmp(name, L"defaultfilelocation") == 0) {
#endif
		return getDefaultFileLocation();
#ifdef _WIN32
	} else if (_wcsicmp(name, L"temporaryfilelocation") == 0) {
#else
	} else if (_wcscasecmp(name, L"temporaryfilelocation") == 0) {
#endif
		return getTemporaryFileLocation();
#ifdef _WIN32
	} else if (_wcsicmp(name, L"datasourcename") == 0) {
#else
	} else if (_wcscasecmp(name, L"datasourcename") == 0) {
#endif
		return getDataSourceName();
#ifdef _WIN32
	} else if (_wcsicmp(name, L"userid") == 0) {
#else
	} else if (_wcscasecmp(name, L"userid") == 0) {
#endif
		return getUserId();
#ifdef _WIN32
	} else if (_wcsicmp(name, L"connectionstring") == 0) {
#else
	} else if (_wcscasecmp(name, L"connectionstring") == 0) {
#endif
		return getConnectionString();
	} else {
		return NULL;
	}
}


int
ProviderConfigInfo::storeNameValue(char * name, char * value) {
	int retval = 1;
	if (strcmp(name, "Name") == 0) {
		this->name = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "Server") == 0) {
		this->server = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "Service") == 0) {
		this->service = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "Instance") == 0) {
		this->instance = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "Username") == 0) {
		this->username = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "Password") == 0) {
		this->password = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "Datastore") == 0) {
		this->datastore = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "SchemaDefFileName") == 0) {
		this->schemaDefFileName = new string(value);
		retval = 0;
	}  else if (strcmp(name, "SchemaInsertFileName") == 0) {
		this->schemaInsertFileName = new string(value);
		retval = 0;
	}  else if (strcmp(name, "SchemaDeleteFileName") == 0) {
		this->schemaDeleteFileName = new string(value);
		retval = 0;
	}  else if (strcmp(name, "GeneratedXmlSchemaFileName") == 0) {
		this->generatedXmlSchemaFileName = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "EditedXmlSchemaName") == 0) {
		this->editedXmlSchemaName = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "EditedXmlSchemaFileName") == 0) {
		this->editedXmlSchemaFileName = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "File") == 0) {
		this->File = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "ReadOnly") == 0) {
		this->ReadOnly = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "SpatialContextName") == 0) {
		this->spatialContextName = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "SpatialContextDesc") == 0) {
		this->spatialContextDesc = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "SrsWkt") == 0) {
		this->srsWkt = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "XYTolerance") == 0) {
		this->XYTolerance = atof(value);
		retval = 0;
	} else if (strcmp(name, "ZTolerance") == 0) {
		this->ZTolerance = atof(value);
		retval = 0;
	} else if (strcmp(name, "DefaultFileLocation") == 0) {
		this->DefaultFileLocation = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "TemporaryFileLocation") == 0) {
		this->TemporaryFileLocation = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "DataSourceName") == 0) {
		this->DataSourceName = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "UserId") == 0) {
		this->UserId = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "ConnectionString") == 0) {
		this->ConnectionString = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else {
		sprintf(errString, "ProviderConfigInfo ERROR: %s not a valid provider configuration property name", name);
		retval = 1;
	}
	return retval;
}

void
ProviderConfigInfo::dumpGetOutputToLog() {
	log->write("Dump provider config info");
	if (name != NULL) log->write("Name:", convert->wchar_tStarToCharStar(this->getName()));
	if (server != NULL) log->write("Server:", convert->wchar_tStarToCharStar(getServer()));
	if (service != NULL) log->write("Service:", convert->wchar_tStarToCharStar(getService()));
	if (instance != NULL) log->write("Instance:", convert->wchar_tStarToCharStar(this->getInstance()));
	if (username != NULL) log->write("Username:", convert->wchar_tStarToCharStar(this->getUsername()));
	if (password != NULL) log->write("Password:", convert->wchar_tStarToCharStar(this->getPassword()));
	if (datastore != NULL) log->write("Datastore:", convert->wchar_tStarToCharStar(this->getDatastore()));
	if (generatedXmlSchemaFileName != NULL) log->write("GeneratedXmlSchemaFileName:", convert->wchar_tStarToCharStar(this->getGeneratedXmlSchemaFileName()));
	if (editedXmlSchemaName != NULL) log->write("EditedXmlSchemaName:", convert->wchar_tStarToCharStar(this->getEditedXmlSchemaName()));
	if (editedXmlSchemaFileName != NULL) log->write("EditedXmlSchemaFileName:", convert->wchar_tStarToCharStar(this->getEditedXmlSchemaFileName()));
	if (schemaDefFileName != NULL) log->write("Schema file name:", this->getSchemaDefFileName());
	if (schemaInsertFileName != NULL) log->write("Schema insert file name:", this->getSchemaInsertFileName());
	if (schemaDeleteFileName != NULL) log->write("Schema delete file name:", this->getSchemaDeleteFileName());
	if (File != NULL) log->write("File :", convert->wchar_tStarToCharStar(this->getFile()));
	if (ReadOnly != NULL) log->write("Is Readonly:", convert->wchar_tStarToCharStar(this->getReadOnly()));
	if (spatialContextName != NULL) log->write("SpatialContextName:", convert->wchar_tStarToCharStar(this->getSpatialContextName()));
	if (spatialContextDesc != NULL) log->write("SpatialContextDesc:", convert->wchar_tStarToCharStar(this->getSpatialContextDesc()));
	if (srsWkt != NULL) log->write("SRSWKT:", convert->wchar_tStarToCharStar(this->getSrsWkt()));
	sprintf(formatStringBuf, "%f", this->getXYTolerance());
	log->write("XYTolerance:", formatStringBuf);
	sprintf(formatStringBuf, "%f", this->getZTolerance());
	log->write("ZTolerance:", formatStringBuf);
	if (DefaultFileLocation != NULL) log->write("DefaultFileLocation:", convert->wchar_tStarToCharStar(this->getDefaultFileLocation()));
	if (TemporaryFileLocation != NULL) log->write("TemporaryFileLocation:", convert->wchar_tStarToCharStar(this->getTemporaryFileLocation()));
	if (DataSourceName != NULL) log->write("DataSourceName:", convert->wchar_tStarToCharStar(this->getDataSourceName()));
	if (UserId != NULL) log->write("UserId:", convert->wchar_tStarToCharStar(this->getUserId()));
	if (ConnectionString != NULL) log->write("ConnectionString:", convert->wchar_tStarToCharStar(this->getConnectionString()));
}




