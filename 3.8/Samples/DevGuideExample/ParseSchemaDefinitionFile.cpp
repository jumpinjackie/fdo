// ParseSchemaDefinitionFile.cpp
// 
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
#include "ParseSchemaDefinitionFile.h"

// this parses a file which has the following structure:
// schemaCount=<n>
// schemaBegin=
// schemaName=<name>
// classCount=<n>
// classBegin=
// className=<name>

ParseSchemaDefinitionFile::ParseSchemaDefinitionFile(const char * schemaFileName,
	Logger * log, Conversions * convert) : ParseSchemaOperationFile(schemaFileName, log, convert) {
		constructionInit();
}
													 
ParseSchemaDefinitionFile::~ParseSchemaDefinitionFile() {
	deconstructionDelete();
}


void
ParseSchemaDefinitionFile::constructionInit() {
	schemaDescription = NULL;
	classDescription = NULL;
	classType = NULL;
	propertyName = NULL;
	propertyDescription = NULL;
	propertyType = NULL;
	propertyGeometricTypes = NULL;
	propertyDataType = NULL;
}

void
ParseSchemaDefinitionFile::deconstructionDelete() {
	delete schemaDescription;
	delete classDescription;
	delete classType;
	delete propertyName;
	delete propertyDescription;
	delete propertyType;
	delete propertyGeometricTypes;
	delete propertyDataType;
}


// assummes that getNameValue() has been called to retrieve the schema count
// which is the first line in the schema file
// the next line, the one this method looks for is schemaBegin=
// returns 0 if lines are parsed successfully and last line is classCount
// returns 1 if first line is not schemaBegin
// returns 2 if a line is not read successfully
int
ParseSchemaDefinitionFile::GetNextSchema() {
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
ParseSchemaDefinitionFile::GetNextClass() {
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
ParseSchemaDefinitionFile::GetNextProperty() {
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

int
ParseSchemaDefinitionFile::getNameValue() {
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

const wchar_t *
ParseSchemaDefinitionFile::GetSchemaDescription() {
	return schemaDescription->c_str();
}

const wchar_t *
ParseSchemaDefinitionFile::GetClassDescription() {
	return classDescription->c_str();
}

const char *
ParseSchemaDefinitionFile::GetClassType() {
	return classType->c_str();
}

bool
ParseSchemaDefinitionFile::GetClassAbstract() {
	return classAbstract;
}

bool
ParseSchemaDefinitionFile::GetClassComputed() {
	return classComputed;
}

const wchar_t *
ParseSchemaDefinitionFile::GetPropertyName() {
	return propertyName->c_str();
}

const wchar_t *
ParseSchemaDefinitionFile::GetPropertyDescription() {
	return propertyDescription->c_str();
}

const char *
ParseSchemaDefinitionFile::GetPropertyType() {
	return propertyType->c_str();
}

const char *
ParseSchemaDefinitionFile::GetPropertyDataType() {
	return propertyDataType->c_str();
}

const char *
ParseSchemaDefinitionFile::GetPropertyGeometricTypes() {
	return propertyGeometricTypes->c_str();
}

bool
ParseSchemaDefinitionFile::GetPropertyReadOnly(){
	return propertyReadOnly;
}

bool
ParseSchemaDefinitionFile::GetPropertyNullable(){
	return propertyNullable;
}


bool
ParseSchemaDefinitionFile::GetPropertyAutoGenerated() {
	return propertyAutoGenerated;
}

bool
ParseSchemaDefinitionFile::GetPropertyIdentity() {
	return propertyIdentity;
}

bool
ParseSchemaDefinitionFile::GetPropertyHasElevation() {
	return propertyHasElevation;
}

bool
ParseSchemaDefinitionFile::GetPropertyHasMeasure() {
	return propertyHasElevation;
}

int
ParseSchemaDefinitionFile::GetPropertyLength() {
	return propertyLength;
}

int
ParseSchemaDefinitionFile::GetPropertyCount() {
	return propertyCount;
}

void
ParseSchemaDefinitionFile::initSchema() {
	if (schemaName != NULL) {
		delete schemaName;
		schemaName = NULL;
	}
	initSchemaOtherProperties();
	classCount = 0;
	initClass();
}

void
ParseSchemaDefinitionFile::initClass() {
	if (className != NULL) {
		delete className;
		className = NULL;
	}
	initClassOtherProperties();
}

void
ParseSchemaDefinitionFile::initSchemaOtherProperties() {
	if (schemaDescription != NULL) {
		delete schemaDescription;
		schemaDescription = NULL;
	}
}

void
ParseSchemaDefinitionFile::initClassOtherProperties() {
	if (classDescription != NULL) {
		delete classDescription;
		classDescription = NULL;
	}
	if (classType != NULL) {
		delete classType;
		classType = NULL;
	}
	classAbstract = false;
	classComputed = false;
	propertyCount = 0;
	initProperty();
}

void
ParseSchemaDefinitionFile::initProperty() {
	if (propertyName != NULL) {
		delete propertyName;
		propertyName = NULL;
	}
	if (propertyDescription != NULL) {
		delete propertyDescription;
		propertyDescription = NULL;
	}
	if (propertyType != NULL) {
		delete propertyType;
		propertyType = NULL;
	}
	if (propertyGeometricTypes != NULL) {
		delete propertyGeometricTypes;
		propertyGeometricTypes = NULL;
	}
	if (propertyDataType != NULL) {
		delete propertyDataType;
		propertyDataType = NULL;
	}
	propertyReadOnly = false;
	propertyNullable = false;
	propertyAutoGenerated = false;
	propertyIdentity = false;
	propertyHasElevation = false;
	propertyHasMeasure = false;
	propertyLength = 0;
}

int
ParseSchemaDefinitionFile::storeNameValue(char * name, char * value) {
	int retval = 1;
	if (strcmp(name, "schemaBegin") == 0) {
		retval = 0;
	} else if (strcmp(name, "schemaName") == 0) {
		this->schemaName = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "schemaCount") == 0) {
		this->schemaCount = atoi(value);
		retval = 0;
	} else if (strcmp(name, "classBegin") == 0) {
		retval = 0;
	} else if (strcmp(name, "className") == 0) {
		this->className = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "classCount") == 0) {
		this->classCount = atoi(value);
		retval = 0;
	} else if (strcmp(name, "schemaDescription") == 0) {
		this->schemaDescription = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "classDescription") == 0) {
		this->classDescription = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "classType") == 0) {
		this->classType = new string(value);
		retval = 0;
	} else if (strcmp(name, "classAbstract") == 0) {
		if (strcmp(value, "false") == 0) {
			this->classAbstract = false;
			retval = 0;
		} else if (strcmp(value, "true") == 0) {
			this->classAbstract = true;
			retval = 0;
		} else {
			strcpy(errString, "ERROR parsing provider config info file: classAbstract value is not \"true\" or \"false\"");
		}
	} else if (strcmp(name, "classComputed") == 0) {
		if (strcmp(value, "false") == 0) {
			this->classComputed = false;
			retval = 0;
		} else if (strcmp(value, "true") == 0) {
			this->classComputed = true;
			retval = 0;
		} else {
			strcpy(errString, "ERROR parsing provider config info file: classComputed value is not \"true\" or \"false\"");
		}
	} else if (strcmp(name, "propertyBegin") == 0) {
		retval = 0;
	} else if (strcmp(name, "propertyEnd") == 0) {
		retval = 0;
	} else if (strcmp(name, "propertyName") == 0) {
		this->propertyName = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "propertyDescription") == 0) {
		this->propertyDescription = new wstring(convert->charStarToWchar_tStar(value));
		retval = 0;
	} else if (strcmp(name, "propertyType") == 0) {
		this->propertyType = new string(value);
		retval = 0;
	} else if (strcmp(name, "propertyDataType") == 0) {
		this->propertyDataType = new string(value);
		retval = 0;
	} else if (strcmp(name, "propertyGeometricTypes") == 0) {
		this->propertyGeometricTypes = new string(value);
		retval = 0;
	} else if (strcmp(name, "propertyReadOnly") == 0) {
		if (strcmp(value, "false") == 0) {
			this->propertyReadOnly = false;
			retval = 0;
		} else if (strcmp(value, "true") == 0) {
			this->propertyReadOnly = true;
			retval = 0;
		} else {
			strcpy(errString, "ERROR parsing provider config info file: propertyReadOnly value is not \"true\" or \"false\"");
		}
	} else if (strcmp(name, "propertyNullable") == 0) {
		if (strcmp(value, "false") == 0) {
			this->propertyNullable = false;
			retval = 0;
		} else if (strcmp(value, "true") == 0) {
			this->propertyNullable = true;
			retval = 0;
		} else {
			strcpy(errString, "ERROR parsing provider config info file: propertyNullable value is not \"true\" or \"false\"");
		}
	} else if (strcmp(name, "propertyAutoGenerated") == 0) {
		if (strcmp(value, "false") == 0) {
			this->propertyAutoGenerated = false;
			retval = 0;
		} else if (strcmp(value, "true") == 0) {
			this->propertyAutoGenerated = true;
			retval = 0;
		} else {
			strcpy(errString, "ERROR parsing provider config info file: propertyAutoGenerated value is not \"true\" or \"false\"");
		}
	} else if (strcmp(name, "propertyIdentity") == 0) {
		if (strcmp(value, "false") == 0) {
			this->propertyIdentity = false;
			retval = 0;
		} else if (strcmp(value, "true") == 0) {
			this->propertyIdentity = true;
			retval = 0;
		} else {
			strcpy(errString, "ERROR parsing provider config info file: propertyIdentity value is not \"true\" or \"false\"");
		}
	} else if (strcmp(name, "propertyHasElevation") == 0) {
		if (strcmp(value, "false") == 0) {
			this->propertyHasElevation = false;
			retval = 0;
		} else if (strcmp(value, "true") == 0) {
			this->propertyHasElevation = true;
			retval = 0;
		} else {
			strcpy(errString, "ERROR parsing provider config info file: propertyHasElevation value is not \"true\" or \"false\"");
		}
	} else if (strcmp(name, "propertyHasMeasure") == 0) {
		if (strcmp(value, "false") == 0) {
			this->propertyHasMeasure = false;
			retval = 0;
		} else if (strcmp(value, "true") == 0) {
			this->propertyHasMeasure = true;
			retval = 0;
		} else {
			strcpy(errString, "ERROR parsing provider config info file: propertyHasMeasure value is not \"true\" or \"false\"");
		}
	} else if (strcmp(name, "propertyLength") == 0) {
		this->propertyLength = atoi(value);
		retval = 0;
	} else if (strcmp(name, "propertyCount") == 0) {
		this->propertyCount = atoi(value);
		retval = 0;
	} else {
		sprintf(errString, "ERROR: %s not a valid schema definition file key name", name);
		retval = 1;
	}
	delete currentName;
	currentName = new string(name);
	delete currentValue;
	currentValue = new string(value);
	return retval;
}

