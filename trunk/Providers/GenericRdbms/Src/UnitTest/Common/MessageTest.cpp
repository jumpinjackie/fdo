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
#include "MessageTest.h"
#include "FdoCommonNlsUtil.h"
#include <Inc/Nls/rdbi_msg.h>
#ifndef _WIN32
extern char *rdbi_cat;
#else
extern char *rdbi_cat;
#endif

CPPUNIT_TEST_SUITE_REGISTRATION( MessageTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( MessageTest, "MessageTest");

MessageTest::MessageTest(void)
{
   
}


MessageTest::~MessageTest(void)
{
}

void MessageTest::RunTest ()
{
	wchar_t *name = L"\x00e6"L"aOP\x00e7\x00c6";
    wchar_t firstMsg[] = L"Property '\x00e6"L"aOP\x00e7\x00c6' not found ";
    wchar_t secondMsg[] = L"No current database is open; cannot begin transaction '1234' . ";
    
	const wchar_t *msg = FdoCommonNlsUtil::NLSGetMessage( FDORDBMS_56, "Property '%1$ls' not found",rdbi_cat, name );
    // eliminate a space from the end if is the case
    if (msg[wcslen(msg) - 1] != L' ')
        firstMsg[wcslen(firstMsg)-1] = L'\0';
    CPPUNIT_ASSERT( wcscmp(msg, firstMsg)==0 );

	msg = FdoCommonNlsUtil::NLSGetMessage( RDBI_21, "No current database is open; cannot begin transaction '%1$ls' .",rdbi_cat, L"1234");
    // eliminate a space from the end if is the case
    if (msg[wcslen(msg) - 1] != L' ')
        secondMsg[wcslen(secondMsg)-1] = L'\0';
	CPPUNIT_ASSERT( wcscmp(msg, secondMsg)==0 );
}
