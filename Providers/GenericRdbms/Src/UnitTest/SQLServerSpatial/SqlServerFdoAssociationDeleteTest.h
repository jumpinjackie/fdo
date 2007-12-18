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

#ifndef SQLSERVER_ASSOCIATIONDELETETEST_H
#define SQLSERVER_ASSOCIATIONDELETETEST_H

#include "FdoAssociationDeleteTest.h"
#include "SqlServerFdoAssociationInsertTest.h"


class SqlServerFdoAssociationDeleteTest : public FdoAssociationDeleteTest
{
    CPPUNIT_TEST_SUB_SUITE (SqlServerFdoAssociationDeleteTest, FdoAssociationDeleteTest);
    CPPUNIT_TEST_SUITE_END ();

	SqlServerFdoAssociationInsertTest   insertUtil;
	virtual FdoAssociationInsertTest* GetInsertUtil() { return &insertUtil; }

    void  set_provider();
};

#endif // SQLSERVER_ASSOCIATIONDELETETEST_H
