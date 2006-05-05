#ifndef _FDORDBMSMYSQLCONNECTIONPROPERTYDICTIONARY_H_
#define _FDORDBMSMYSQLCONNECTIONPROPERTYDICTIONARY_H_

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
#ifdef _WIN32
#pragma once
#endif
#include "../../Fdo/Connection/FdoRdbmsConnection.h"
#include "../../Fdo/Connection/FdoRdbmsConnectionPropertyDictionary.h"

class FdoRdbmsMySqlConnectionPropertyDictionary : public FdoRdbmsPropertyDictionary
{
public:

	FdoRdbmsMySqlConnectionPropertyDictionary(void)
	{
	}
	FdoRdbmsMySqlConnectionPropertyDictionary(FdoRdbmsConnection *connection):
		FdoRdbmsPropertyDictionary(connection)
	{
		AddItem(FDO_RDBMS_CONNECTION_USERNAME, NlsMsgGet(FDORDBMS_118, "Username"), false, L"", true, false, false, 0, NULL, false, false, false);
		AddItem(FDO_RDBMS_CONNECTION_PASSWORD, NlsMsgGet(FDORDBMS_119, "Password"), false, L"", true, true, false, 0, NULL, false, false, false);
		AddItem(FDO_RDBMS_CONNECTION_SERVICE,  NlsMsgGet(FDORDBMS_120, "Service"), false, L"", true, false, false, 0, NULL, false, false, false);
		AddItem(FDO_RDBMS_CONNECTION_DATASTORE, NlsMsgGet(FDORDBMS_117, "DataStore"), false, L"", false, false, true, 0, NULL, false, false, true);
	}

	~FdoRdbmsMySqlConnectionPropertyDictionary(void)
	{
	}
};
#endif
