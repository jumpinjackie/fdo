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

#include "Pch.h"
#include "ConnectionUtil.h"
#include <FdoCommonFile.h>

#ifndef RDBI_DEF_SA_ORA
extern    void ut_thread_mutex_init ();
extern    void ut_thread_mutex_destroy ();

/// <summary>This class initializes the mutecies for nls_msg_get.</summary>
/// <remark>It is only needed if this initialization is not done in the DLL initialization code.</remark>
class FdoMsgInitializerSingleton
{
public:
    FdoMsgInitializerSingleton ()
    {
        ut_thread_mutex_init ();
    }

    ~FdoMsgInitializerSingleton ()
    {
        ut_thread_mutex_destroy ();
    }
};

static FdoMsgInitializerSingleton Singleton;
#endif

StaticConnection::StaticConnection (void) :
    m_rdbi_context(NULL),
    m_gdbi_conn(NULL)
{
}

StaticConnection::~StaticConnection (void)
{
    disconnect();
}

void StaticConnection::SetSchema ( FdoString* suffix ) 
{
    mDatastore = UnitTestUtil::GetEnviron("datastore", suffix );
    FdoStringP strDatastore = mDatastore;
#ifndef RDBI_DEF_SA_ORA
    if (m_rdbi_context->dispatch.capabilities.supports_unicode == 1)
        ::rdbi_set_schemaW( m_rdbi_context, strDatastore );
    else
        ::rdbi_set_schema( m_rdbi_context, strDatastore );
#else
        ::rdbi_set_schema( m_rdbi_context, (char*)(const char*)strDatastore );
#endif
}

int StaticConnection::do_rdbi_init (rdbi_context_def** rdbi_context)
{
    CPPUNIT_FAIL ("naked do_rdbi_init");
    return (0);
}

int StaticConnection::do_rdbi_connect (rdbi_context_def* rdbi_context, int& id)
{
    CPPUNIT_FAIL ("naked do_rdbi_connect");
    return (0);
}

void StaticConnection::init ()
{
    rdbi_context_def* rdbi_context;

    try
    {
        CPPUNIT_ASSERT_MESSAGE ("rdbi_initialize failed", RDBI_SUCCESS == do_rdbi_init (&rdbi_context));
#ifndef RDBI_DEF_SA_ORA
        CPPUNIT_ASSERT_MESSAGE ("rdbi_term failed", RDBI_SUCCESS == rdbi_term (&rdbi_context));
#else
        CPPUNIT_ASSERT_MESSAGE ("rdbi_term failed", RDBI_SUCCESS == rdbi_term (rdbi_context));
#endif
    }
    catch (CppUnit::Exception exception)
    {
        throw exception;
    }
    catch (...)
    {
        CPPUNIT_FAIL ("unexpected exception encountered");
    }
}

void StaticConnection::connect ()
{
    int id;

    try
    {
        CPPUNIT_ASSERT_MESSAGE ("rdbi_initialize failed", RDBI_SUCCESS == do_rdbi_init (&m_rdbi_context));
        try
        {
            if (! (RDBI_SUCCESS == do_rdbi_connect (m_rdbi_context, id)) ) {
                rdbi_get_msg( m_rdbi_context );
                throw FdoException::Create( m_rdbi_context->last_error_msg );
            }
    
            m_gdbi_conn = new GdbiConnection(m_rdbi_context);

        }
        catch (CppUnit::Exception exception)
        {
#ifndef RDBI_DEF_SA_ORA
            rdbi_term (&m_rdbi_context);
#else
            rdbi_term (m_rdbi_context);
#endif
            throw exception;
        }
    }
    catch (FdoException* e ) 
    {
        UnitTestUtil::FailOnException(e);
    }
    catch (CppUnit::Exception exception)
    {
        throw exception;
    }
    catch (...)
    {
        CPPUNIT_FAIL ("unexpected exception encountered");
    }
}

void StaticConnection::disconnect ()
{
    try
    {
        if ( m_gdbi_conn ) {
            delete m_gdbi_conn;
            m_gdbi_conn = NULL;
        }

        if ( m_rdbi_context ) {
            CPPUNIT_ASSERT_MESSAGE ("rdbi_disconnect failed", RDBI_SUCCESS == rdbi_disconnect (m_rdbi_context));
#ifndef RDBI_DEF_SA_ORA
            CPPUNIT_ASSERT_MESSAGE ("rdbi_term failed", RDBI_SUCCESS == rdbi_term (&m_rdbi_context));
#else
            if ( m_rdbi_context->rdbi_initialized )
                CPPUNIT_ASSERT_MESSAGE ("rdbi_term failed", RDBI_SUCCESS == rdbi_term (m_rdbi_context));
#endif
        }

    }
    catch (FdoException* e ) 
    {
        if ( m_rdbi_context ) {
            delete m_rdbi_context;
            m_rdbi_context = NULL;
        }
        if ( m_gdbi_conn ) {
            delete m_gdbi_conn;
            m_gdbi_conn = NULL;
        }
        UnitTestUtil::FailOnException(e);
    }
    catch (CppUnit::Exception exception)
    {
        if ( m_rdbi_context ) {
            delete m_rdbi_context;
            m_rdbi_context = NULL;
        }
        if ( m_gdbi_conn ) {
            delete m_gdbi_conn;
            m_gdbi_conn = NULL;
        }
        throw exception;
    }
    catch (...)
    {
        if ( m_rdbi_context ) {
            delete m_rdbi_context;
            m_rdbi_context = NULL;
        }
        if ( m_gdbi_conn ) {
            delete m_gdbi_conn;
            m_gdbi_conn = NULL;
        }
        CPPUNIT_FAIL ("unexpected exception encountered");
    }
}

void StaticConnection::name ()
{
    rdbi_context_def *rdbi_context;
    char* name;

    try
    {
        CPPUNIT_ASSERT_MESSAGE ("rdbi_initialize failed", RDBI_SUCCESS == do_rdbi_init (&rdbi_context));
        name = rdbi_vndr_name (rdbi_context);
        printf ("%s\n", name);
#ifndef RDBI_DEF_SA_ORA
        CPPUNIT_ASSERT_MESSAGE ("rdbi_term failed", RDBI_SUCCESS == rdbi_term (&rdbi_context));
#else
        CPPUNIT_ASSERT_MESSAGE ("rdbi_term failed", RDBI_SUCCESS == rdbi_term (rdbi_context));
#endif
    }
    catch (CppUnit::Exception exception)
    {
        throw exception;
    }
    catch (...)
    {
        CPPUNIT_FAIL ("unexpected exception encountered");
    }
}

void StaticConnection::info ()
{
    rdbi_context_def *rdbi_context;
    int id;
    rdbi_vndr_info_def info;

    try
    {
        CPPUNIT_ASSERT_MESSAGE ("rdbi_initialize failed", RDBI_SUCCESS == do_rdbi_init (&rdbi_context));
        try
        {
            CPPUNIT_ASSERT_MESSAGE ("rdbi_connect failed", RDBI_SUCCESS == do_rdbi_connect (rdbi_context, id));
            CPPUNIT_ASSERT_MESSAGE ("rdbi_vndr_info failed", RDBI_SUCCESS == rdbi_vndr_info (rdbi_context, &info));
            printf ("%s version %ld, precision(%ld,%ld), scale(%ld,%ld), string(%ld,%ld)\n",
                info.name, info.dbversion,
                info.minPrecision, /* Minimum allowed precision on numeric columns */
                info.maxPrecision, /* Maximum allowed precision on numeric columns */
                info.minScale, /* Minimum allowed scale on numeric columns */
                info.maxScale, /* Maximum allowed scale on numeric columns */
                info.minStrLen, /* Minimum length for string columns */
                info.maxStrLen); /* Maximum length for string columns */
            CPPUNIT_ASSERT_MESSAGE ("rdbi_disconnect failed", RDBI_SUCCESS == rdbi_disconnect (rdbi_context));
        }
        catch (CppUnit::Exception exception)
        {
#ifndef RDBI_DEF_SA_ORA
            rdbi_term (&rdbi_context);
#else
            rdbi_term (rdbi_context);
#endif
            throw exception;
        }
#ifndef RDBI_DEF_SA_ORA
        CPPUNIT_ASSERT_MESSAGE ("rdbi_term failed", RDBI_SUCCESS == rdbi_term (&rdbi_context));
#else
        CPPUNIT_ASSERT_MESSAGE ("rdbi_term failed", RDBI_SUCCESS == rdbi_term (rdbi_context));
#endif
    }
    catch (CppUnit::Exception exception)
    {
        throw exception;
    }
    catch (...)
    {
        CPPUNIT_FAIL ("unexpected exception encountered");
    }
}

char *ConnectionUtil::CommandFilesDirectory;

ConnectionUtil::ConnectionUtil(void)
{
	m_SetupDone = false;
    CommandFilesDirectory = NULL;
}

ConnectionUtil::~ConnectionUtil(void)
{
    if (NULL != CommandFilesDirectory)
        delete[] CommandFilesDirectory;
}

wchar_t *ConnectionUtil::GetConnectionString(StringConnTypeRequest pTypeReq, FdoString *suffix, bool bAddExtraneousSpaces)
{
    FdoStringP service = UnitTestUtil::GetEnviron("service");
    FdoStringP username = UnitTestUtil::GetEnviron("username");
    FdoStringP password = UnitTestUtil::GetEnviron("password");
    FdoStringP datastore = UnitTestUtil::GetEnviron("datastore", suffix);
	
    FdoStringP     connectString;
	static wchar_t retConnectString[200];

    if (Connection_WithDatastore == pTypeReq)
        connectString = FdoStringP::Format(
            L"service=%ls;username=%ls;password=%ls;datastore=%ls", 
            (FdoString*) service, 
            (FdoString*) username, 
            (FdoString*) password, 
            (FdoString*) datastore
        );
	else
        connectString = FdoStringP::Format(
            L"service=%ls;username=%ls;password=%ls;", 
            (FdoString*) service, 
            (FdoString*) username, 
            (FdoString*) password
        );

    if ( bAddExtraneousSpaces ) 
        connectString = FdoStringP(L"  ") + connectString.Replace(L"=",L"  =  ").Replace(L";",L"  ;  ") + L"  ";

    wcscpy(retConnectString, connectString);

	return retConnectString;
}

FdoStringP ConnectionUtil::GetEnviron(const char *name, FdoString *suffix)
{
    static char    uname [1024];
    int   size=1024;

    UnitTestUtil::GetRealUserName(uname, size);

    if (_stricmp(name, "service") == 0)
    {
        return GetEnv("service");
    }
    else if (_stricmp(name, "username") == 0)
    {
        return GetEnv("username");
    }
    else if (_stricmp(name, "password") == 0)
    {
        return GetEnv("password");
    }
    else if (_stricmp(name, "datastore") == 0)
    {
        FdoStringP source;
        FdoStringP datastore = GetEnv("datastore");
        if (datastore == NULL)
            source = FdoStringP::Format( L"fdo_%hs", uname);
        else
            source = datastore;
        source = source + suffix;
		if (GetSuffixTest() != NULL)
			source = source + GetSuffixTest();
        return source;
    }
    else if (_stricmp(name, "provider") == 0)
    {
        FdoStringP provider = GetEnv("provider");
        return provider;
    }
    else if (_stricmp(name, "dsnname") == 0)
    {
        return GetEnv("dsnname");
    }
    else if (_stricmp(name, "comdir") == 0)
    {
#ifdef _WIN32
        char com_dir[MAX_PATH];
        wchar_t wcom_dir[MAX_PATH];
        DWORD result;
        char* last;
        char buffer[1024];

        if (NULL == CommandFilesDirectory)
        {
            strcpy (com_dir, ".\\");
            result = GetModuleFileName (
                NULL, // handle to module
                com_dir, // path buffer
                MAX_PATH // size of buffer
                );
            if (0 != result)
            {
                // scan the string for the last occurrence of a slash
                last = strrchr (com_dir, '\\');
                if (NULL != last)
                {
                    last++; // move past the slash
                    *last = '\0'; // null terminate it there
                }
            }
            strcat (com_dir, "com\\");

            swprintf (wcom_dir, MAX_PATH - 1, L"%hs", com_dir ); 

            if ( (!FdoCommonFile::FileExists(wcom_dir)) || (!FdoCommonFile::IsDirectory(wcom_dir)) )
            {
                sprintf (buffer, "Command files directory '%s' doesn't exist.", com_dir);
                CPPUNIT_FAIL (buffer);
            }
            else
            {
                CommandFilesDirectory = new char[strlen (com_dir) + 1];
                strcpy (CommandFilesDirectory, com_dir);
            }
        }
        return (CommandFilesDirectory);
#else
	char* getComDir ();
	return getComDir();
#endif
    }
    return L"";
}

FdoStringP ConnectionUtil::GetEnv(const char* name)
{
    const char* env = getenv( name );

    return env;
}

SchemaOverrideUtilP ConnectionUtil::NewSchemaOverrideUtil()
{
    return new SchemaOverrideUtil();
}

FdoStringP ConnectionUtil::GetNlsObjectName( FdoStringP inName )
{
    if ( inName == L"Industrial Parcel" ) 
        return FdoStringP::Format(
            L"Ind%lcstri%lc P%lcrcel",
            GetNlsChar(1),
            GetNlsChar(2),
            GetNlsChar(3)
        );

    if ( inName == L"Geometry" ) 
        return FdoStringP::Format(
            L"G%lc%lcm%lctry",
            GetNlsChar(1),
            GetNlsChar(2),
            GetNlsChar(3)
        );

    return inName;
}

wchar_t ConnectionUtil::GetNlsChar( int index )
{
    wchar_t ret = 0x00c1;

    switch ( index ) 
    {
        case 1:
            ret = 0x00c2;
            break;
        case 2:
            ret = 0x00c3;
            break;
        case 3:
            ret = 0x00e3;
            break;
        case 4:
            ret = 0x00e4;
            break;
    }

    return ret;
}

FdoStringP ConnectionUtil::FormatGrant( FdoString* objName, FdoString* grantee, FdoString* privileges )
{
    return L"";
}

