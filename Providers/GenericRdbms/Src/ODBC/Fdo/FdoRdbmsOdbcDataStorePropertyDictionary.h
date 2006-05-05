#ifndef _FDORDBMSODBCDATASTOREPROPERTYDICTIONARY_H_
#define _FDORDBMSODBCDATASTOREPROPERTYDICTIONARY_H_
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
#include "../../Fdo/DataStore/FdoRdbmsDataStorePropertyDictionary.h"

/// <summary>The FdoIDataStorePropertyDictionary exposes a dictionary style interface
/// to the datastore properties. The properties required to create or destroy a datastore
/// can be determined and their values can be set through this interface.</summary>
class FdoRdbmsOdbcDataStorePropertyDictionary : public FdoRdbmsDataStorePropertyDictionary
{
public:

	FdoRdbmsOdbcDataStorePropertyDictionary( FdoRdbmsConnection *connection, int action ) :
	  FdoRdbmsDataStorePropertyDictionary( connection, action )
	{
		static const wchar_t *LtModeValues[]	= { L"FDO", L"NONE" };
		static const wchar_t *LockModeValues[] = { L"FDO", L"NONE" };

		if ( action == FDO_RDBMS_DATASTORE_FOR_READ )
		{
			AddItem(FDO_RDBMS_CONNECTION_DATASTORE, NlsMsgGet(FDORDBMS_117, "DataStore"), false, L"", false, false, false, 0, NULL, false, false, true);
			AddItem(FDO_RDBMS_DATASTORE_DESCRIPTION, NlsMsgGet(FDORDBMS_448, "Description"), false, L"", false, false, false, 0, NULL, false, false, false);
			AddItem(FDO_RDBMS_DATASTORE_LTMODE, NlsMsgGet(FDORDBMS_449, "LtMode"), false, L"FDO", false, false, true, 2, LtModeValues, false, false, false);
			AddItem(FDO_RDBMS_DATASTORE_LOCKMODE, NlsMsgGet(FDORDBMS_450, "LockMode"), false, L"FDO", false, false, true, 2, LockModeValues, false, false, false);
		}
		else if ( action == FDO_RDBMS_DATASTORE_FOR_CREATE )
		{
			AddItem(FDO_RDBMS_CONNECTION_DATASTORE, NlsMsgGet(FDORDBMS_117, "DataStore"), false, L"", true, false, false, 0, NULL, false, false, true);
			AddItem(FDO_RDBMS_CONNECTION_PASSWORD, NlsMsgGet(FDORDBMS_119, "Password"), false, L"", true, true, false, 0, NULL, false, false, false);
			AddItem(FDO_RDBMS_DATASTORE_DESCRIPTION, NlsMsgGet(FDORDBMS_448, "Description"), false, L"", false, false, false, 0, NULL, false, false, false);
			AddItem(FDO_RDBMS_DATASTORE_LTMODE, NlsMsgGet(FDORDBMS_449, "LtMode"), false, L"FDO", false, false, true, 2, LtModeValues, false, false, false);
			AddItem(FDO_RDBMS_DATASTORE_LOCKMODE, NlsMsgGet(FDORDBMS_450, "LockMode"), false, L"FDO", false, false, true, 2, LockModeValues, false, false, false);
		}
		else if ( action == FDO_RDBMS_DATASTORE_FOR_DELETE )
		{
			AddItem(FDO_RDBMS_CONNECTION_DATASTORE, NlsMsgGet(FDORDBMS_117, "DataStore"), false, L"", true, false, false, 0, NULL, false, false, true);
			AddItem(FDO_RDBMS_CONNECTION_PASSWORD, NlsMsgGet(FDORDBMS_119, "Password"), false, L"", true, true, false, 0, NULL, false, false, false);
		}
	}
};

#endif
