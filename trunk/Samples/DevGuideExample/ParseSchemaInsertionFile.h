// ParseSchemaInsertionFile.h


#ifndef _PARSESCHEMAINSERTIONFILE_H_
#define _PARSESCHEMAINSERTIONFILE_H_
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

#include "Logger.h"
#include "conversions.h"
#include "ExerciseFdoUtilities.h"
#include <wchar.h>
#include <istream>
#include <fstream>
#include <string>
#include <cstring>
#include <iostream>
#include "ParseSchemaOperationFile.h"

class ParseSchemaInsertionFile  : public ParseSchemaOperationFile {
public:
	ParseSchemaInsertionFile(const char * schemaFileName,
		Logger * log,
		Conversions * convert,
		ExerciseFdoUtilities * utilities);
	~ParseSchemaInsertionFile();

	int getNameValue();
	int GetNextSchema();
	int GetNextClass();
	int GetNextProperty();
	int GetNextGeometricType();
	int GetNextPolygon();
	int GetNextPoint();
	int GetNextInteriorRing();
	int GetNextLineString();
	int GetClassInstanceCount();
	int GetPropertyCount();
	const wchar_t * GetPropertyName();
	const wchar_t * GetPropertyNameUpperCase();
	const char * GetPropertyValue();
	const char * GetPropertyDataType();
	const char * GetPropertyType();
	const char * GetPropertyGeometryType();
	double * GetExteriorRing();
	double * GetInteriorRing();
	int GetInteriorRingCount();
	double * GetLineString();
	int GetLineStringCount();
	int GetPolygonCount();
	int GetPointCount();
	double * GetPoint();
	int GetOrdinateCount();

private:
	ExerciseFdoUtilities * utilities;

	int classInstanceCount;
	int propertyCount;
	wstring * propertyName;
	string * propertyValue;
	string * propertyType;
	string * propertyDataType;
	string * propertyGeometryType;
	double * exteriorRing;
	int interiorRingCount;
	double * interiorRing;
	double * lineString;
	double * point;
	int lineStringCount;
	int polygonCount;
	int pointCount;
	int ordinateCount;

	void initSchema();
	void initClass();
	void initProperty();
	void initPolygon();
	void initClassOtherProperties();
	void constructionInit();
	void deconstructionDelete();
	int storeNameValue(char * name, char * value);
};

#endif
