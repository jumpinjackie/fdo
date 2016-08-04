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

#ifndef CPP_UNIT_CONNECTTESTS_H
#define CPP_UNIT_CONNECTTESTS_H

#include <Inc/Rdbi/proto.h>

class ConnectTests : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE (ConnectTests);
    CPPUNIT_TEST (init);
    CPPUNIT_TEST (name);
    CPPUNIT_TEST (connect);
    CPPUNIT_TEST (info);
    CPPUNIT_TEST_SUITE_END ();

public:
    ConnectTests (void);
    virtual ~ConnectTests (void);
    void setUp ();
    void tearDown ();

protected:
    virtual void set_provider() {  CPPUNIT_FAIL ("naked set_provider() called"); };
    void init ();
    void name ();
    void connect ();
    void info ();

    virtual int do_rdbi_init (rdbi_context_def** rdbi_context);
    virtual int do_rdbi_connect (rdbi_context_def* rdbi_context, int& id);
};

#endif // CPP_UNIT_CONNECTTESTS_H
