// ParseSchemaFilterFile.h


#ifndef _PARSESCHEMAFILTERFILE_H_
#define _PARSESCHEMAFILTERFILE_H_
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
#include "ParseSchemaOperationFile.h"

class ParseSchemaFilterFile  : public ParseSchemaOperationFile {
public:
	ParseSchemaFilterFile(const char * schemaFileName,
		Logger * log,
		Conversions * convert);
	~ParseSchemaFilterFile();

	int getNameValue();
	int GetNextSchema();
	int GetNextFilter();
	int GetFilterCount();
	const wchar_t * GetFilterString();
	const wchar_t * GetFilterStringUpperCase();

private:
	int filterCount;
	wstring * filterString;

	void initSchema();
	void initFilter();
	void constructionInit();
	void deconstructionDelete();
	int storeNameValue(char * name, char * value);
};

#endif
