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
#include "MySqlConnectionUtil.h"
#include "MySqlConnectTests.h"
#include "MySqlDataTypeTests.h"
#include "MySqlDescribeSchemaTest.h"
#include "MySqlFdoAdvancedSelectTest.h"
#include "MySqlFdoApplySchemaTest.h"
#include "MySqlFdoAssociationDeleteTest.h"
#include "MySqlFdoAssociationInsertTest.h"
#include "MySqlFdoAssociationSchemaTest.h"
#include "MySqlFdoAssociationSelectTest.h"
#include "MySqlFdoAssociationUpdateTest.h"
#include "MySqlFdoCapabilityTest.h"
#include "MySqlFdoConnectTest.h"
#include "MySqlFdoConstraintsTest.h"
#include "MySqlFdoDatastoreTest.h"
#include "MySqlFdoDeleteTest.h"
#include "MySqlFdoImportExportTest.h"
#include "MySqlFdoInsertTest.h"
#include "MySqlFdoMultiThreadTest.h"
#include "MySqlFdoSchemaRollbackTest.h"
#include "MySqlFdoSchemaTest.h"
#include "MySqlFdoSelectTest.h"
#include "MySqlFdoSqlCmdTest.h"
#include "MySqlGeometryTests.h"
#include "MySqlSchemaMgrTests.h"
#include "MySqlSelectTests.h"
#include "MessageTest.h"
#include "MySqlFdoUpdateTest.h"
#include "UnitTestUtil.h"

static MySqlConnectionUtil pMySqlConnectionUtil;
ConnectionUtil* UnitTestUtil::InfoUtilConnection = &pMySqlConnectionUtil;
FdoString *UnitTestUtil::ActiveProviderName = L"OSGeo.MySQL.3.2";

void ForceLinuxLink()
{
	MySqlConnectTests cl2;
	MySqlDataTypeTests cl3;
	MySqlDescribeSchemaTest cl4;
	MySqlFdoAdvancedSelectTest cl5;
	MySqlFdoApplySchemaTest cl6;
	MySqlFdoAssociationDeleteTest cl7;
	MySqlFdoAssociationInsertTest cl8;
	MySqlFdoAssociationSchemaTest cl9;
	MySqlFdoAssociationSelectTest cl10;
	MySqlFdoAssociationUpdateTest cl11;
	MySqlFdoCapabilityTest cl12;
	MySqlFdoConnectTest cl13;
	MySqlFdoConstraintsTest cl14;
	MySqlFdoDatastoreTest cl15;
	MySqlFdoDeleteTest cl16;
	MySqlFdoImportExportTest cl17;
	MySqlFdoInsertTest cl18;
	MySqlFdoMultiThreadTest cl19;
	MySqlFdoSchemaRollbackTest cl20;
	MySqlFdoSchemaTest cl21;
	MySqlFdoSelectTest cl22;
	MySqlFdoSqlCmdTest cl23;
	MySqlFdoUpdateTest cl24;
	MySqlGeometryTests cl25;
	MySqlSchemaMgrTests cl26;
	MySqlSelectTests cl27;
	MessageTest c28;
}

