// ParseSchemaOperationFile.cpp
// 
//  
//  Copyright (C) 2008  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  

#include "stdafx.h"
#include "ParseSchemaOperationFile.h"

// this parses a file which has the following structure:
// schemaCount=<n>
// schemaBegin=
// schemaName=<name>
// classCount=<n>
// classBegin=
// className=<name>

ParseSchemaOperationFile::ParseSchemaOperationFile(const char * schemaFileName, Logger * log, Conversions * convert) {
	this->schemaFileName = new string(schemaFileName);
	this->log = log;
	this->convert = convert;
	schemaFile.open(schemaFileName);
	if (!schemaFile.is_open()) {
		sprintf(errString, "ERROR: cannot open file: %s", schemaFileName);
		throw(errString);
	}
	currentName = NULL;
	currentValue = NULL;
	schemaName = NULL;
	className = NULL;
	schemaCount = 0;
	classCount = 0;
}

ParseSchemaOperationFile::~ParseSchemaOperationFile() {
	delete schemaFileName;
	delete schemaName;
	delete className;
	delete currentName;
	delete currentValue;
	schemaFile.close();
}


int
ParseSchemaOperationFile::getline(char * key, char * val) {
	int retval = 0;
	char * name = NULL;
	char * value = NULL;
   if( schemaFile.is_open() ) {
		schemaFile.getline(line,512);
		if (!schemaFile.good()) {
			strcpy(errString, "ERROR: reading schema file");
			log->write("ERROR: reading file:", schemaFileName->c_str());
			return 1;
		}
		if (strlen(line)) {
			log->write(line);
			name = strtok(line, "=");
			strcpy(key, name);
			log->write("name =", name);
			value = strtok((char*)NULL, "\n");
			if (value == NULL) value = "";
			strcpy(val, value);
 			log->write("value =", value);
		}
   } else {
	   sprintf(errString, "ERROR: file not open: %s", schemaFileName->c_str());
	   log->write("ERROR: file not open:", schemaFileName->c_str());
	   retval = 1;
   }
   return retval;
}

const wchar_t *
ParseSchemaOperationFile::GetSchemaName() {
	return schemaName->c_str();
}

const wchar_t *
ParseSchemaOperationFile::GetSchemaNameUpperCase() {
	return convert->wchar_tStarToUpperCase(schemaName->c_str());
}

int
ParseSchemaOperationFile::GetSchemaCount() {
	return schemaCount;
}

const wchar_t *
ParseSchemaOperationFile::GetClassName() {
	return className->c_str();
}

const wchar_t *
ParseSchemaOperationFile::GetClassNameUpperCase() {
	return convert->wchar_tStarToUpperCase(className->c_str());
}

const wchar_t *
ParseSchemaOperationFile::GetQualifiedClassName() {
#ifdef _WIN32
	swprintf(wcharBuf, L"%s:%s", schemaName->c_str(), className->c_str());
	return wcharBuf;
#else
	swprintf(wcharBuf, sizeof(wcharBuf)/sizeof(wchar_t), L"%ls:%ls", schemaName->c_str(), className->c_str());
	return wcharBuf;
#endif
}

const wchar_t *
ParseSchemaOperationFile::GetQualifiedClassNameUpperCase() {
#ifdef _WIN32
	swprintf(wcharBuf, L"%s:%s", schemaName->c_str(), className->c_str());
	return convert->wchar_tStarToUpperCase(wcharBuf);
#else
	swprintf(wcharBuf, sizeof(wcharBuf)/sizeof(wchar_t), L"%ls:%ls", schemaName->c_str(), className->c_str());
	return convert->wchar_tStarToUpperCase(wcharBuf);
#endif
}

int
ParseSchemaOperationFile::GetClassCount() {
	return classCount;
}

const char *
ParseSchemaOperationFile::GetCurrentName() {
	return currentName->c_str();
}

const char *
ParseSchemaOperationFile::GetCurrentValue() {
	return currentValue->c_str();
}
