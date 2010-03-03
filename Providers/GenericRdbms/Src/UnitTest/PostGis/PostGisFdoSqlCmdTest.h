/*
 * Copyright (C) 2006 Refractions Research, Inc.
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
#ifndef POSTGIS_FDOSQLCMDTEST_H
#define POSTGIS_FDOSQLCMDTEST_H

#include "FdoSqlCmdTest.h"

class PostGisFdoSqlCmdTest : public FdoSqlCmdTest
{
    CPPUNIT_TEST_SUB_SUITE(PostGisFdoSqlCmdTest, FdoSqlCmdTest);
    CPPUNIT_TEST_SUITE_END();

    void  set_provider();

    wchar_t *get_number_type() 
    { 
        return L"numeric"; 
    }
    wchar_t *get_datetime_sys()
    {
        return L"CURRENT_TIMESTAMP"; // or "NOW() or NULL"
    }
};

#endif // POSTGIS_FDOSQLCMDTEST_H
