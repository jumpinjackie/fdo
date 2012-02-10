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
#include "ConnectTests.h"


void ut_thread_mutex_init ();
void ut_thread_mutex_destroy ();


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

ConnectTests::ConnectTests (void)
{
}

ConnectTests::~ConnectTests (void)
{
}

void ConnectTests::setUp ()
{
    set_provider();
}

void ConnectTests::tearDown ()
{
}

int ConnectTests::do_rdbi_init (rdbi_context_def** rdbi_context)
{
    CPPUNIT_FAIL ("naked do_rdbi_init");
    return (0);
}

int ConnectTests::do_rdbi_connect (rdbi_context_def* rdbi_context, int& id)
{
    CPPUNIT_FAIL ("naked do_rdbi_connect");
    return (0);
}

void ConnectTests::init ()
{
    rdbi_context_def* rdbi_context;

    try
    {
        CPPUNIT_ASSERT_MESSAGE ("rdbi_initialize failed", RDBI_SUCCESS == do_rdbi_init (&rdbi_context));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_term failed", RDBI_SUCCESS == rdbi_term (&rdbi_context));
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

void ConnectTests::name ()
{
    rdbi_context_def *rdbi_context;
    char* name;

    try
    {
        CPPUNIT_ASSERT_MESSAGE ("rdbi_initialize failed", RDBI_SUCCESS == do_rdbi_init (&rdbi_context));
        name = rdbi_vndr_name (rdbi_context);
        printf ("%s\n", name);
        CPPUNIT_ASSERT_MESSAGE ("rdbi_term failed", RDBI_SUCCESS == rdbi_term (&rdbi_context));
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

void ConnectTests::connect ()
{
    rdbi_context_def *rdbi_context;
    int id;

    try
    {
        CPPUNIT_ASSERT_MESSAGE ("rdbi_initialize failed", RDBI_SUCCESS == do_rdbi_init (&rdbi_context));
        try
        {
            CPPUNIT_ASSERT_MESSAGE ("rdbi_connect failed", RDBI_SUCCESS == do_rdbi_connect (rdbi_context, id));
            CPPUNIT_ASSERT_MESSAGE ("rdbi_disconnect failed", RDBI_SUCCESS == rdbi_disconnect (rdbi_context));
        }
        catch (CppUnit::Exception exception)
        {
            rdbi_term (&rdbi_context);
            throw exception;
        }
        CPPUNIT_ASSERT_MESSAGE ("rdbi_term failed", RDBI_SUCCESS == rdbi_term (&rdbi_context));
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

void ConnectTests::info ()
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
            rdbi_term (&rdbi_context);
            throw exception;
        }
        CPPUNIT_ASSERT_MESSAGE ("rdbi_term failed", RDBI_SUCCESS == rdbi_term (&rdbi_context));
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
