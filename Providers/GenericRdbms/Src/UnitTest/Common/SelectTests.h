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

#ifndef SELECTTESTS_H
#define SELECTTESTS_H

#include <Inc/Rdbi/proto.h>

class SelectTests : public CppUnit::TestCase
{
protected:
    rdbi_context_def* mRdbiContext;
    int mId;

private:
    CPPUNIT_TEST_SUITE (SelectTests);
    CPPUNIT_TEST (ddl);
    CPPUNIT_TEST (define);
    CPPUNIT_TEST (bind);
    CPPUNIT_TEST (describe);
    CPPUNIT_TEST_SUITE_END ();

public:
    SelectTests (void);
    virtual ~SelectTests (void);
    void setUp ();
    void tearDown ();

protected:
    void ddl ();
    void define ();
    void bind ();
    void describe ();

    virtual void set_provider() {  CPPUNIT_FAIL ("naked set_provider() called"); };
    virtual int do_rdbi_init ();
    virtual int do_rdbi_connect (const char* dataStoreName, const char* userName, const char* userPassword);
    virtual char *get_bind_var (int n);
    virtual char *get_date_time (const struct tm *when);
    virtual void  set_timestamp_nnull ( char *nullind ) { rdbi_set_nnull( mRdbiContext, nullind, 0, 0); }
    virtual bool allow_timestamp_notnull() { return true;};
    virtual int get_char_size() { return 1; };
};

#endif // SELECTTESTS_H

