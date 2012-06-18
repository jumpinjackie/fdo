#ifndef MYSQL_CONNECTIONUTIL_H
#define MYSQL_CONNECTIONUTIL_H

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

#include "ConnectionUtil.h"
#include "StringPropertiesDictionary.h"

class MySqlStaticConnection : public StaticConnection
{
public:
    int do_rdbi_init (rdbi_context_def** rdbi_context);
    int do_rdbi_connect (rdbi_context_def* rdbi_context, int& id);
    virtual FdoSchemaManagerP CreateSchemaManager();
    virtual const char* GetServiceName();
};


class MySqlConnectionUtil : public ConnectionUtil
{
private:
	FdoPtr<StringPropertiesDictionary> m_SetupValues;
	FdoStringP m_IdTest;
public:
	MySqlConnectionUtil(void);
	virtual ~MySqlConnectionUtil(void);
	void LoadInitializeFile();

	virtual void SetProvider( const char *providerName );
	virtual StaticConnection* NewStaticConnection ();
    virtual SchemaOverrideUtilP NewSchemaOverrideUtil();
	virtual FdoInsertTest* GetInsertTest ();
	virtual FdoStringP GetOutputFileName (FdoString* pBaseFileName);
};

#endif // MYSQL_CONNECTIONUTIL_H
