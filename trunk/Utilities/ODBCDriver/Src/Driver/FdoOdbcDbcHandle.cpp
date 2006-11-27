// 
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include "StdHdr.h"
#include "FdoOdbcDbcHandle.h"

FdoOdbcDbcHandle::FdoOdbcDbcHandle(void)
    :m_Handle(NULL)
{
    FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
    m_Handle = manager->CreateConnection (L"OSGeo.SDF.3.2");
}

FdoOdbcDbcHandle::FdoOdbcDbcHandle(FdoIConnection* handle)
    :m_Handle(handle)
{
}

FdoOdbcDbcHandle::~FdoOdbcDbcHandle(void)
{
}

FdoOdbcDbcHandle* FdoOdbcDbcHandle::Create(void)
{
    return new FdoOdbcDbcHandle();
}

FdoOdbcDbcHandle* FdoOdbcDbcHandle::Create(FdoIConnection* handle)
{
    return new FdoOdbcDbcHandle(handle);
}

FdoIConnection* FdoOdbcDbcHandle::GetHandle()
{
    return m_Handle;
}

void FdoOdbcDbcHandle::SetHandle(FdoIConnection* handle)
{
    m_Handle = handle;
}
