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

#ifndef SQLSERVER_ASSOCIATIONUPDATETEST_H
#define SQLSERVER_ASSOCIATIONUPDATETEST_H

#include "FdoAssociationUpdateTest.h"
#include "SqlServerFdoAssociationSchemaTest.h"

class SqlServerFdoAssociationUpdateTest : public FdoAssociationUpdateTest
{
    CPPUNIT_TEST_SUB_SUITE (SqlServerFdoAssociationUpdateTest, FdoAssociationUpdateTest);
    CPPUNIT_TEST_SUITE_END ();

	SqlServerFdoAssociationSchemaTest   schemaUtil;
	virtual FdoAssociationSchemaTest* GetSchemaUtil() { return &schemaUtil; }
    void  set_provider();
};

#endif // SQLSERVER_ASSOCIATIONUPDATETEST_H
