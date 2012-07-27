#ifndef _CLIENTSERVICESTEST_H_
#define _CLIENTSERVICESTEST_H_

/***************************************************************************
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
 ***************************************************************************/

#ifdef _WIN32
#pragma once
#endif

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

/* 
 * A test case a select.
 */
class ClientServicesTest : public CppUnit::TestCase
{
    class FdoCSTestData
    {
    public:
        FdoCSTestData(std::wstring name, 
                    std::wstring displayName, 
                    std::wstring description, 
                    std::wstring version, 
                    std::wstring fdoVersion, 
                    std::wstring libraryPath,
                    bool isManaged)
        {
            m_name = name;
            m_displayName = displayName;
            m_description = description;
            m_version = version;
            m_fdoVersion = fdoVersion;
            m_libraryPath = libraryPath;
            m_isManaged = isManaged;
        }

    public:
        std::wstring m_name;
        std::wstring m_displayName;
        std::wstring m_description;
        std::wstring m_version;
        std::wstring m_fdoVersion;
        std::wstring m_libraryPath;
        bool m_isManaged;
    };

    FDO_CPPUNIT_DEFINE(TestServices);

    CPPUNIT_TEST_SUITE( ClientServicesTest );
    CPPUNIT_TEST( TestServices );
    CPPUNIT_TEST_SUITE_END();

public:
    ClientServicesTest(void);
    virtual ~ClientServicesTest(void);
    void setUp ();
    void tearDown ();

protected:
    void TestServices ();

private:
    FdoCSTestData * dt1;
    FdoCSTestData * dt2;
    FdoCSTestData * dt3;
};

#endif

