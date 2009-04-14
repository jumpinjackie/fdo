// ParseSchemaFilterFile.cpp
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
#include "ParseSchemaFilterFile.h"

ParseSchemaFilterFile::ParseSchemaFilterFile(const char * schemaFileName,
	Logger * log, Conversions * convert) : ParseSchemaOperationFile(schemaFileName, log, convert) {
		constructionInit();
}
													 
ParseSchemaFilterFile::~ParseSchemaFilterFile() {
	deconstructionDelete();
}

void
ParseSchemaFilterFile::constructionInit() {
	filterCount = 0;
	filterString = NULL;
}

void
ParseSchemaFilterFile::deconstructionDelete() {
	delete filterString;
}

// assummes that getNameValue() has been called to retrieve the schema count
// which is the first line in the schema file
// the next line, the one this method looks for is schemaBegin=
// returns 0 if lines are parsed successfully and last line is classCount
// returns 1 if first line is not schemaBegin
// returns 2 if a line is not read successfully
int
ParseSchemaFilterFile::GetNextSchema() {
	int retval = 0;
	initSchema();
	retval = getNameValue();
	if (retval) return 2;
	if (strcmp(GetCurrentName(), "schemaBegin") != 0) return 1;
	while (strcmp(GetCurrentName(), "filterCount") != 0) {
		retval = getNameValue();
		if (retval) return 2;
	}
	return retval;
}

// returns 0 if lines parsed successfully and last line read is propertyCount
// returns 1 if first line is not classBegin
// returns 2 if a line is not read successfully
int
ParseSchemaFilterFile::GetNextFilter() {
	int retval = 0;
	initFilter();
	retval = getNameValue();
	if (retval) return 2;
	if (strcmp(GetCurrentName(), "filterBegin") != 0) return 1;
	while (strcmp(GetCurrentName(), "filterEnd") != 0) {
		retval = getNameValue();
		if (retval) return 2;
	}
	return retval;
}


int
ParseSchemaFilterFile::GetFilterCount() {
	return filterCount;
}

const wchar_t *
ParseSchemaFilterFile::GetFilterString() {
	return filterString->c_str();
}

const wchar_t *
ParseSchemaFilterFile::GetFilterStringUpperCase() {
	return convert->wchar_tStarToUpperCase(filterString->c_str());
}

void
ParseSchemaFilterFile::initSchema() {
	if (schemaName != NULL) {
		delete schemaName;
		schemaName = NULL;
	}
	filterCount = 0;
	initFilter();
}

void
ParseSchemaFilterFile::initFilter() {
	if (className != NULL) {
		delete className;
		className = NULL;
	}
	if (filterString != NULL) {
		delete filterString;
		filterString = NULL;
	}
}

int
ParseSchemaFilterFile::getNameValue() {
	int retval = 0;
	char key[256];
	char val[256];
	if (retval = getline(key, val)) return retval;
	if (retval = storeNameValue(key, val)) {
		log->timestr();
		log->str(errString);
		log->str(key);
		log->str(val);
		log->endline();
	}
   return retval;
}

int
ParseSchemaFilterFile::storeNameValue(char * name, char * value) {
	int retval = 1;
	if (strcmp(name, "schemaBegin") == 0) {
		retval = 0;
	} else if (strcmp(name, "schemaName") == 0) {
		this->schemaName = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "schemaCount") == 0) {
		this->schemaCount = atoi(value);
		retval = 0;
	} else if (strcmp(name, "filterCount") == 0) {
		this->filterCount = atoi(value);
		retval = 0;
	} else if (strcmp(name, "className") == 0) {
		this->className = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "filterBegin") == 0) {
		retval = 0;
	} else if (strcmp(name, "filterEnd") == 0) {
		retval = 0;
	} else if (strcmp(name, "filterString") == 0) {
		this->filterString = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else {
		sprintf(errString, "ERROR: %s not a valid schema filter file key name", name);
		retval = 1;
	}
	delete currentName;
	currentName = new string(name);
	delete currentValue;
	currentValue = new string(value);
	return retval;
}


