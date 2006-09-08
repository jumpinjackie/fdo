#ifndef _FDORDBMSCONNECTIONPROPERTYDICTIONARY_H_
#define _FDORDBMSCONNECTIONPROPERTYDICTIONARY_H_

/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "Fdo.h"
#include "../Other/Collection.h"
#include <Common/Ptr.h>
#include "Fdo/Commands/CommandType.h"
#include <FdoCommonConnPropDictionary.h>

#define FDO_RDBMS_CONNECTION_USERNAME           L"Username"
#define FDO_RDBMS_CONNECTION_PASSWORD           L"Password"
#define FDO_RDBMS_CONNECTION_SERVICE            L"Service"
#define FDO_RDBMS_CONNECTION_DATASTORE          L"DataStore"
#define FDO_RDBMS_CONNECTION_DSN    	        L"DataSourceName"
#define FDO_RDBMS_CONNECTION_USERID    	        L"UserId"
#define FDO_RDBMS_CONNECTION_CONNSTRING         L"ConnectionString"
#define FDO_RDBMS_CONNECTION_GENDEFGEOMETRYPROP L"GenerateDefaultGeometryProperty"

#define FDO_RDBMS_DATASTORE_DESCRIPTION	 L"Description"
#define FDO_RDBMS_DATASTORE_LTMODE		 L"LtMode"
#define FDO_RDBMS_DATASTORE_LOCKMODE	 L"LockMode"
#define FDO_RDBMS_DATASTORE_TABLESPACE	 L"TableSpace"

#define FDO_RDBMS_DATASTORE_FOR_READ	0
#define FDO_RDBMS_DATASTORE_FOR_CREATE	1
#define FDO_RDBMS_DATASTORE_FOR_DELETE	2

class FdoRdbmsConnectionPropertyDictionary : public FdoCommonConnPropDictionary
{
public:
    FdoRdbmsConnectionPropertyDictionary(FdoIConnection* conn) : FdoCommonConnPropDictionary(conn) {};
    FdoRdbmsConnectionPropertyDictionary() : FdoCommonConnPropDictionary() {};

    // Returns an array of possible values for the specified property.
    virtual const wchar_t** EnumeratePropertyValues(FdoString* name, int& length);

};

#endif
