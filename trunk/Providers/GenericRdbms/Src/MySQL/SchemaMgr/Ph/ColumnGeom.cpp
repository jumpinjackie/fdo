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

#include "stdafx.h"
#include "Mgr.h"
#include "ColumnGeom.h"

FdoInt64 FdoSmPhMySqlColumnGeom::GetSRID()
{
	if (mSRID == -1)
	{
		FdoSmPhDbObjectP dbObject = this->GetContainingDbObject();
		FdoStringP sqlStmt = FdoStringP::Format(
			L"select SRID(%ls) as srid from %ls", (FdoString*)this->GetDbName(), (FdoString*)dbObject->GetDbName());
		FdoSmPhMySqlMgrP mgr = this->GetManager()->SmartCast<FdoSmPhMySqlMgr>();
		GdbiConnection* gdbiConn = mgr->GetGdbiConnection();
		GdbiQueryResult *gdbiResult = gdbiConn->ExecuteQuery((const char*)sqlStmt);
		// SRID is 0 if table is empty or geometry column has not been populated yet
		mSRID = 0;
		if (gdbiResult->ReadNext())
		{
			if (!gdbiResult->GetIsNull(L"srid"))
				mSRID = gdbiResult->GetInt64(L"srid", NULL, NULL);
		}
		gdbiResult->End();
        delete gdbiResult;
	}
	return mSRID;
}
