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
#include "PostGisConnectionUtil.h"
#include "PostGisConnectTests.h"
#include "PostGisFdoApplySchemaTest.h"
#include "PostGisFdoConnectTest.h"
#include "PostGisFdoConnectionInfoTest.h"
#include "PostGisFdoDatastoreTest.h"
#include "PostGisFdoDeleteTest.h"
#include "PostGisFdoImportExportTest.h"
#include "PostGisFdoInsertTest.h"
#include "PostGisFdoMultiThreadTest.h"
#include "PostGisFdoSelectTest.h"
#include "PostGisFdoSqlCmdTest.h"
#include "PostGisFdoUpdateTest.h"
#include "PostGisSchemaMgrTests.h"
#include "UnitTestUtil.h"
#include "MessageTest.h"

static PostGisConnectionUtil pPostGisConnectionUtil;
ConnectionUtil* UnitTestUtil::InfoUtilConnection = &pPostGisConnectionUtil;
FdoString *UnitTestUtil::ActiveProviderName = L"OSGeo.PostgreSQL.3.6";

void ForceLinuxLink()
{
  PostGisConnectTests cl2;
  PostGisFdoApplySchemaTest cl3;
  PostGisFdoConnectTest cl4;
  PostGisFdoConnectionInfoTest cl5;
  PostGisFdoDatastoreTest cl6;
  PostGisFdoDeleteTest cl7;
  PostGisFdoImportExportTest cl8;
  PostGisFdoInsertTest cl9;
  PostGisFdoMultiThreadTest cl10;
  PostGisFdoSelectTest cl11;
  PostGisFdoSqlCmdTest cl12;
  PostGisFdoUpdateTest cl13;
  PostGisSchemaMgrTests cl14;
}

