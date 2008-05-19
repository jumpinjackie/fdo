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
 */

#ifndef FIDELITYTESTS_H
#define FIDELITYTESTS_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ShpTests.h"

class FidelityTests :
    public ShpTests
{
    CPPUNIT_TEST_SUITE (FidelityTests);
    CPPUNIT_TEST (ontario);
    CPPUNIT_TEST (lakes);
    CPPUNIT_TEST (nanboku);
    CPPUNIT_TEST (ignore_states);
    CPPUNIT_TEST_SUITE_END ();

    static FdoPtr<FdoIConnection> mConnection;

public:
    FidelityTests (void);
    virtual ~FidelityTests (void);
	void setUp ();
	void tearDown ();

protected:
    void ontario ();
    void lakes ();
    void nanboku ();
    void ignore_states ();

private:
    void compare (const wchar_t* target_class_name, const wchar_t* file_name, const wchar_t* extension, int skip);
};

#endif // FIDELITYTESTS_H

