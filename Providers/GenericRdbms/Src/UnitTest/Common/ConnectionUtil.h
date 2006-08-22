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

#ifndef CPP_UNIT_CONNECTIONUTIL_H
#define CPP_UNIT_CONNECTIONUTIL_H

#include <Sm/SchemaManager.h>
#include "../Gdbi/GdbiConnection.h"
#include <Inc/Rdbi/proto.h>
#include "UnitTestUtil.h"

#define UNITTEST_VERSION_NAME L"Granite"

class FdoInsertTest;

class StaticConnection
{
public:
    StaticConnection (void);
    virtual ~StaticConnection (void);

    void connect();
    void disconnect();

    virtual FdoSchemaManagerP CreateSchemaManager()
    {
        return (FdoSchemaManager*)NULL;
    }

    virtual const char* GetServiceName()
    {
        return NULL;
    }

    virtual void SetSchema ( char* suffix );

protected:
    void init ();
    void name ();
    void info ();

    virtual int do_rdbi_init (rdbi_context_def** rdbi_context);
    virtual int do_rdbi_connect (rdbi_context_def* rdbi_context, int& id);

    rdbi_context_def *m_rdbi_context;
    GdbiConnection* m_gdbi_conn;
    char mDatastore[100];
};


class ConnectionUtil
{
public:
	ConnectionUtil(void);

	virtual wchar_t *GetConnectionString(StringConnTypeRequest pTypeReq = Connection_WithDatastore, const char *suffix = "");
	virtual const char *GetEnviron( const char *name, const char *suffix = "" );
	virtual StaticConnection* NewStaticConnection() = 0;
	virtual void SetProvider( const char *providerName ) = 0;
	virtual FdoInsertTest* GetInsertTest () = 0;
	virtual FdoStringP GetOutputFileName (FdoString* pBaseFileName) = 0;
	virtual const char* GetSuffixTest(){return NULL;};

public:
	virtual ~ConnectionUtil(void);
};

#endif // CPP_UNIT_CONNECTIONUTIL_H
