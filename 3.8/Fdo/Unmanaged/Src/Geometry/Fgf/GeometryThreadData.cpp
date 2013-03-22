/*
 * 
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

#include "GeometryThreadData.h"

static FdoGeometryThreadDataKey threadDataKey_S = 0;

#ifdef _WIN32
#define GET_VALUE(key)          ((FdoGeometryThreadData *) TlsGetValue(key))
#define SET_VALUE(key, value)   (TlsSetValue((key), (value)))
#else
#define GET_VALUE(key)          ((FdoGeometryThreadData *) pthread_getspecific(key))
#define SET_VALUE(key, value)   (pthread_setspecific((key), (value)))
#endif

FdoGeometryThreadData *
FdoGeometryThreadData::GetValue()
{
    FdoGeometryThreadData *data = GET_VALUE(threadDataKey_S);

	if (NULL == data)
	{
        data = new FdoGeometryThreadData;
        if (NULL == data)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
        else
            data->geometryPools = new FdoFgfGeometryPools();
        SET_VALUE(threadDataKey_S, data);
	}
	return data;
}

void FdoGeometryThreadData::ReleaseValue()
{
    FdoGeometryThreadData *data = GET_VALUE(threadDataKey_S);

    if (NULL != data)
    {
        // in case we set thread value to null before delete thread object
        // the thread object will be recreated and all geometries will readded to new pools
        delete data;
        SET_VALUE(threadDataKey_S, NULL);
    }
}

FdoGeometryThreadDataKey
FdoGeometryThreadData::GetKey()
{
    return threadDataKey_S;
}

void FdoGeometryThreadData::SetKey(FdoGeometryThreadDataKey key)
{
    threadDataKey_S = key;
}

