// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


#include "stdafx.h"
#include <Common/Ptr.h>
#include "FdoRdbmsConnectionPropertyDictionary.h"
#include "FdoRdbmsConnectionInfo.h"
#include "../DataStore/FdoRdbmsGetDataStores.h"
#include "Fdo/Commands/CommandType.h"
#include <Inc/Nls/fdordbms_msg.h>
#include "GdbiException.h"
#include "FdoRdbmsConnection.h"

typedef  FdoArray<FdoString *> FdoRdbmsDataStoreNames;

const wchar_t **FdoRdbmsConnectionPropertyDictionary::EnumeratePropertyValues(const wchar_t* name, int& count)
{
    FdoPtr<ConnectionProperty> pProp = FindProperty (name);
    wchar_t** pRet = NULL;
    count = 0;

    if (_wcsnicmp(name, FDO_RDBMS_CONNECTION_DATASTORE, wcslen(FDO_RDBMS_CONNECTION_DATASTORE)) == 0)
    {
        if ( (mConnection == NULL) || (mConnection->GetConnectionState() == FdoConnectionState_Closed) )
            throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

        if (((FdoRdbmsConnection*)mConnection)->GetDbiConnection() != NULL)
        {
            FdoPtr<FdoRdbmsGetDataStores> dataStores = (FdoRdbmsGetDataStores *)mConnection->CreateCommand(FdoCommandType_ListDataStores);
            FdoPtr<FdoRdbmsDataStoreNames> mNames = dataStores->GetDataStoresNames();
            FdoString **dbnames = mNames->GetData();
            count = mNames->GetCount();
            if (count)
            {
                pRet = new wchar_t* [count];
                for (int i = 0; i < count; i++)
                {
                    if (dbnames[i])
                    {
                        pRet[i] = new wchar_t[wcslen(dbnames[i]) + 1];
                        wcscpy (pRet[i], dbnames[i]);
                        delete[] dbnames[i];
                    }
                    else
                        pRet[i] = NULL;
                }
            }
        }
        pProp->UpdateEnumerableProperties (count, (const wchar_t**)pRet);
    }
    else
    {
        count = pProp->GetCountEnumerableProperties();
        pRet = (wchar_t**)pProp->GetEnumerableProperties();
    }
    return (const wchar_t**)pRet;
}
