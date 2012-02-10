// ExerciseFdoSchema.h

#ifndef _EXERCISEFDOSCHEMA_H_
#define _EXERCISEFDOSCHEMA_H_
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

#include "ParseSchemaDefinitionFile.h"
#include "strToFdoType.h"
#include "ExerciseFdoUtilities.h"
#include "ProviderConfigInfo.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

extern int errno;

class ExerciseFdoSchema {
public:
	ExerciseFdoSchema(StrToFdoType * str2FdoType,
		ExerciseFdoUtilities * utilities,
		FdoIConnection * connection);
	void AddClassDefinitionProperties(
		FdoClassDefinition * classDef,
		ParseSchemaDefinitionFile * schemaDefinitionFile);
	void AddProperties(
		FdoClassDefinition * classDef,
		ParseSchemaDefinitionFile * schemaDefinitionFile);

private:
	char errString[256];
	char charbuf[1024];
	FdoInt32 convGeoTypesToInt(const char * geometricTypes);
	StrToFdoType * str2FdoType;
	ExerciseFdoUtilities * utilities;
	FdoIConnection * connection;
	FdoPtr<FdoIApplySchema> applySchema;
	FdoPtr<FdoIDescribeSchema> describeSchema;
	FdoPtr<FdoIDestroySchema> destroySchema;
	FdoPtr<FdoIConnectionInfo> connectionInfo;
};

#endif

