// ParseSchemaOperationFile.h


#ifndef _PARSESCHEMAOPERATIONFILE_H_
#define _PARSESCHEMAOPERATIONFILE_H_
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
#include <wchar.h>
#include <istream>
#include <fstream>
#include <string>
#include <cstring>
#include <iostream>
#include <stdio.h>

class ParseSchemaOperationFile {
public:
	ParseSchemaOperationFile(const char * schemaFileName, Logger * log, Conversions * convert);
	~ParseSchemaOperationFile();

	int getline(char * key, char * val);
	const wchar_t * GetSchemaName();
	const wchar_t * GetSchemaNameUpperCase();
	const wchar_t * GetClassName();
	const wchar_t * GetClassNameUpperCase();
	const wchar_t * GetQualifiedClassName();
	const wchar_t * GetQualifiedClassNameUpperCase();
	int GetSchemaCount();
	int GetClassCount();
	const char * GetCurrentName();
	const char * GetCurrentValue();

protected:
	string * schemaFileName;
	ifstream schemaFile;
	Conversions * convert;
	Logger * log;

	char line[512];
	char charbuf[256];
	char schemaNameBuf[128];
	char classNameBuf[128];
	wchar_t wcharBuf[512];
	char errString[256];

	string * currentName;
	string * currentValue;
	wstring * schemaName;
	wstring * className;
	int schemaCount;
	int classCount;

};

#endif
