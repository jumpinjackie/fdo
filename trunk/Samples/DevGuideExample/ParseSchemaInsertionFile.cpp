// ParseSchemaInsertionFile.cpp
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
#include "ParseSchemaInsertionFile.h"

ParseSchemaInsertionFile::ParseSchemaInsertionFile(const char * schemaFileName,
	Logger * log, Conversions * convert, ExerciseFdoUtilities * utilities) : ParseSchemaOperationFile(schemaFileName, log, convert) {
		constructionInit();
}
													 
ParseSchemaInsertionFile::~ParseSchemaInsertionFile() {
	deconstructionDelete();
}

void
ParseSchemaInsertionFile::constructionInit() {
	this->utilities = utilities;
	classInstanceCount = 0;
	propertyCount = 0;
	propertyName = NULL;
	propertyValue = NULL;
	propertyType = NULL;
	propertyDataType = NULL;
	propertyGeometryType = NULL;
	exteriorRing = NULL;
	interiorRing = NULL;
	interiorRingCount = 0;
	lineString = NULL;
	point = NULL;
	lineStringCount = 0;
	polygonCount = 0;
	pointCount = 0;
	ordinateCount = 0;
}

void
ParseSchemaInsertionFile::deconstructionDelete() {
	delete propertyName;
	delete propertyValue;
	delete propertyType;
	delete propertyDataType;
	delete propertyGeometryType;
	delete exteriorRing;
	delete interiorRing;
	delete lineString;
	delete point;
}

// assummes that getNameValue() has been called to retrieve the schema count
// which is the first line in the schema file
// the next line, the one this method looks for is schemaBegin=
// returns 0 if lines are parsed successfully and last line is classCount
// returns 1 if first line is not schemaBegin
// returns 2 if a line is not read successfully
int
ParseSchemaInsertionFile::GetNextSchema() {
	int retval = 0;
	initSchema();
	retval = getNameValue();
	if (retval) return 2;
	if (strcmp(GetCurrentName(), "schemaBegin") != 0) return 1;
	while (strcmp(GetCurrentName(), "classCount") != 0) {
		retval = getNameValue();
		if (retval) return 2;
	}
	return retval;
}

// returns 0 if lines parsed successfully and last line read is propertyCount
// returns 1 if first line is not classBegin
// returns 2 if a line is not read successfully
int
ParseSchemaInsertionFile::GetNextClass() {
	int retval = 0;
	initClass();
	retval = getNameValue();
	if (retval) return 2;
	if (strcmp(GetCurrentName(), "classBegin") != 0) return 1;
	while (strcmp(GetCurrentName(), "propertyCount") != 0) {
		retval = getNameValue();
		if (retval) return 2;
	}
	return retval;
}

// returns 0 if lines parsed successfully and last line read is propertyEnd
// returns 1 if first line is not propertyBegin
// returns 2 if a line is not read successfully
int
ParseSchemaInsertionFile::GetNextProperty() {
	int retval = 0;
	initProperty();
	retval = getNameValue();
	if (retval) return 2;
	if (strcmp(GetCurrentName(), "propertyBegin") != 0) return 1;
	while (strcmp(GetCurrentName(), "propertyEnd") != 0) {
		retval = getNameValue();
		if (retval) return 2;
	}
	return retval;
}

// returns 0 if line parsed successfully
// returns 1 if line is not interiorring
// returns 2 if a line is not read successfully
int
ParseSchemaInsertionFile::GetNextInteriorRing() {
	int retval = 0;
	if (interiorRing != NULL) {
		delete interiorRing;
		interiorRing = NULL;
		ordinateCount = 0;
	}
	retval = getNameValue();
	if (retval) return 2;
	if (strcmp(GetCurrentName(), "interiorRing") != 0) retval = 1;
	return retval;
}

// returns 0 if line parsed successfully
// returns 1 if line is not interiorring
// returns 2 if a line is not read successfully
int
ParseSchemaInsertionFile::GetNextLineString() {
	int retval = 0;
	if (lineString != NULL) {
		delete lineString;
		lineString = NULL;
		ordinateCount = 0;
	}
	retval = getNameValue();
	if (retval) return 2;
	if (strcmp(GetCurrentName(), "lineString") != 0) retval = 1;
	return retval;
}

// returns 0 if line parsed successfully
// returns 1 if line is not interiorring
// returns 2 if a line is not read successfully
int
ParseSchemaInsertionFile::GetNextPoint() {
	int retval = 0;
	if (point != NULL) {
		delete point;
		point = NULL;
		ordinateCount = 0;
	}
	retval = getNameValue();
	if (retval) return 2;
	if (strcmp(GetCurrentName(), "point") != 0) retval = 1;
	return retval;
}

// returns 0 if line parsed successfully
// returns 1 if line is not interiorring
// returns 2 if a line is not read successfully
int
ParseSchemaInsertionFile::GetNextPolygon() {
	int retval = 0;
	initPolygon();
	retval = getNameValue();
	if (retval) return 2;
	if (strcmp(GetCurrentName(), "polygonBegin") != 0) return 1;
	while (strcmp(GetCurrentName(), "polygonEnd") != 0) {
		retval = getNameValue();
		if (retval) return 2;
	}
	return retval;
}

int
ParseSchemaInsertionFile::GetClassInstanceCount() {
	return classInstanceCount;
}

int
ParseSchemaInsertionFile::GetPropertyCount() {
	return propertyCount;
}

const wchar_t *
ParseSchemaInsertionFile::GetPropertyName() {
	return propertyName->c_str();
}

const wchar_t *
ParseSchemaInsertionFile::GetPropertyNameUpperCase() {
	return convert->wchar_tStarToUpperCase(propertyName->c_str());
}

const char *
ParseSchemaInsertionFile::GetPropertyValue() {
	return propertyValue->c_str();
}

const char *
ParseSchemaInsertionFile::GetPropertyDataType() {
	return propertyDataType->c_str();
}

const char *
ParseSchemaInsertionFile::GetPropertyType() {
	return propertyType->c_str();
}

const char *
ParseSchemaInsertionFile::GetPropertyGeometryType() {
	return propertyGeometryType->c_str();
}

double *
ParseSchemaInsertionFile::GetExteriorRing() {
	return exteriorRing;
}

double *
ParseSchemaInsertionFile::GetInteriorRing() {
	return interiorRing;
}

double *
ParseSchemaInsertionFile::GetLineString() {
	return lineString;
}

double *
ParseSchemaInsertionFile::GetPoint() {
	return point;
}

int
ParseSchemaInsertionFile::GetOrdinateCount(){
	return ordinateCount;
}

int
ParseSchemaInsertionFile::GetInteriorRingCount() {
	return interiorRingCount;
}

int
ParseSchemaInsertionFile::GetLineStringCount() {
	return lineStringCount;
}

int
ParseSchemaInsertionFile::GetPolygonCount() {
	return polygonCount;
}

int
ParseSchemaInsertionFile::GetPointCount() {
	return pointCount;
}

void
ParseSchemaInsertionFile::initSchema() {
	if (schemaName != NULL) {
		delete schemaName;
		schemaName = NULL;
	}
	classCount = 0;
	initClass();
}

void
ParseSchemaInsertionFile::initClass() {
	if (className != NULL) {
		delete className;
		className = NULL;
	}
	classInstanceCount = 0;
	propertyCount = 0;
	initProperty();
}

void
ParseSchemaInsertionFile::initProperty() {
	if (propertyName != NULL) {
		delete propertyName;
		propertyName = NULL;
	}
	if (propertyValue != NULL) {
		delete propertyValue;
		propertyValue = NULL;
	}
	if (propertyType != NULL) {
		delete propertyType;
		propertyType = NULL;
	}
	if (propertyGeometryType != NULL) {
		delete propertyGeometryType;
		propertyGeometryType = NULL;
	}
	lineStringCount = 0;
	polygonCount = 0;
	pointCount = 0;
}
void
ParseSchemaInsertionFile::initPolygon() {
	if (exteriorRing != NULL) {
		delete exteriorRing;
		exteriorRing = NULL;
		ordinateCount = 0;
	}
	interiorRingCount = 0;
}

int
ParseSchemaInsertionFile::getNameValue() {
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
ParseSchemaInsertionFile::storeNameValue(char * name, char * value) {
	int retval = 1;
	if (strcmp(name, "schemaBegin") == 0) {
		retval = 0;
	} else if (strcmp(name, "schemaName") == 0) {
		this->schemaName = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "schemaCount") == 0) {
		this->schemaCount = atoi(value);
		retval = 0;
	} else if (strcmp(name, "classCount") == 0) {
		this->classCount = atoi(value);
		retval = 0;
	} else if (strcmp(name, "classBegin") == 0) {
		retval = 0;
	} else if (strcmp(name, "classInstanceCount") == 0) {
		this->classInstanceCount = atoi(value);
		retval = 0;
	} else if (strcmp(name, "className") == 0) {
		this->className = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "propertyCount") == 0) {
		this->propertyCount = atoi(value);
		retval = 0;
	} else if (strcmp(name, "propertyBegin") == 0) {
		retval = 0;
	} else if (strcmp(name, "propertyEnd") == 0) {
		retval = 0;
	} else if (strcmp(name, "polygonBegin") == 0) {
		retval = 0;
	} else if (strcmp(name, "polygonEnd") == 0) {
		retval = 0;
	} else if (strcmp(name, "propertyName") == 0) {
		this->propertyName = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "propertyType") == 0) {
		this->propertyType = new string(value);
		retval = 0;
	} else if (strcmp(name, "propertyDataType") == 0) {
		this->propertyDataType = new string(value);
		retval = 0;
	} else if (strcmp(name, "propertyValue") == 0) {
		this->propertyValue = new string(value);
		retval = 0;
	} else if (strcmp(name, "propertyGeometryType") == 0) {
		this->propertyGeometryType = new string(value);
		retval = 0;
	} else if (strcmp(name, "exteriorRing") == 0) {
		this->exteriorRing = utilities->stringToDoubleArray(value, &ordinateCount);
		retval = 0;
	} else if (strcmp(name, "interiorRing") == 0) {
		this->interiorRing = utilities->stringToDoubleArray(value, &ordinateCount);
		retval = 0;
	} else if (strcmp(name, "interiorRingCount") == 0) {
		this->interiorRingCount = atoi(value);
		retval = 0;
	} else if (strcmp(name, "polygonCount") == 0) {
		this->polygonCount = atoi(value);
		retval = 0;
	} else if (strcmp(name, "pointCount") == 0) {
		this->pointCount = atoi(value);
		retval = 0;
	} else if (strcmp(name, "point") == 0) {
		this->point = utilities->stringToDoubleArray(value, &ordinateCount);
		retval = 0;
	} else if (strcmp(name, "lineString") == 0) {
		this->lineString = utilities->stringToDoubleArray(value, &ordinateCount);
		retval = 0;
	} else if (strcmp(name, "lineStringCount") == 0) {
		this->lineStringCount = atoi(value);
		retval = 0;
	} else {
		sprintf(errString, "ERROR: %s not a valid schema data file value name", name);
		retval = 1;
	}
	delete currentName;
	currentName = new string(name);
	delete currentValue;
	currentValue = new string(value);
	return retval;
}


