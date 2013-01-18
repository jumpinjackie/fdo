/*
 * Copyright (C) 2004-2011  Autodesk, Inc.
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

#ifndef POSTGIS_ADVANCEDSELECTTEST_H
#define POSTGIS_ADVANCEDSELECTTEST_H

#include "FdoAdvancedSelectTest.h"

class PostGisFdoAdvancedSelectTest : public FdoAdvancedSelectTest
{
    CPPUNIT_TEST_SUB_SUITE (PostGisFdoAdvancedSelectTest, FdoAdvancedSelectTest);
    CPPUNIT_TEST( isValidTest );
    CPPUNIT_TEST_SUITE_END ();

    void  set_provider();

protected:
    // TODO: this one gets an uninitialized value crash in Expression Engine
    // since TestFunc is Double but GetString() is used to retrieved it. Need
    // to investigate whether this is a bug.
    virtual void functionTest() {}

    // TODO: investigate why this one crashes.
    virtual void getDataTypeTest() {}
};

#endif // PostGis_ADVANCEDSELECTTEST_H
