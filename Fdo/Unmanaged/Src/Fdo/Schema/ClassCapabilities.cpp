// 
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
//  
#include <cstring>
#include <Fdo/Schema/ClassDefinition.h>

FdoClassCapabilities::FdoClassCapabilities()
{
    m_supportsLocking = false;
    m_supportsLongTransactions = false;
    m_supportsWrite = false;
    m_parent = NULL;
    m_lockTypeCount = 0;
    m_lockTypes = NULL;
}

FdoClassCapabilities::FdoClassCapabilities(FdoClassDefinition& parent)
{
    m_supportsLocking = false;
    m_supportsLongTransactions = false;
    m_supportsWrite = false;
    m_parent = &parent;    // NOTE: Not addref'ed to avoid circular reference issues.
    m_lockTypeCount = 0;
    m_lockTypes = NULL;
}

FdoClassCapabilities::~FdoClassCapabilities()
{
    if(m_lockTypes)
        delete[] m_lockTypes;
}

void FdoClassCapabilities::Dispose()
{
    delete this;
}

FdoBoolean FdoClassCapabilities::SupportsLocking()
{
    return m_supportsLocking;
}

FdoBoolean FdoClassCapabilities::SupportsLongTransactions()
{
    return m_supportsLongTransactions;
}

FdoClassDefinition* FdoClassCapabilities::GetParent()
{
    return FDO_SAFE_ADDREF(m_parent);
}

FdoClassCapabilities* FdoClassCapabilities::Create(FdoClassDefinition& parent)
{
    return new FdoClassCapabilities(parent);
}

void FdoClassCapabilities::SetSupportsLocking(FdoBoolean value)
{
    m_supportsLocking = value;
}

void FdoClassCapabilities::SetSupportsLongTransactions(FdoBoolean value)
{
    m_supportsLongTransactions = value;
}

FdoLockType* FdoClassCapabilities::GetLockTypes(FdoInt32& size)
{
    size =  m_lockTypeCount;
    return m_lockTypes;
}

void FdoClassCapabilities::SetLockTypes(const FdoLockType* types, FdoInt32 size)
{
    if(m_lockTypes)
        delete[] m_lockTypes;
    m_lockTypes = NULL;
    m_lockTypeCount = 0;
    
    if(types != NULL && size > 0)
    {
        m_lockTypes = new FdoLockType[size];
        memcpy(m_lockTypes,types,size*sizeof(FdoLockType));
        m_lockTypeCount = size;
    }
}

FdoBoolean FdoClassCapabilities::SupportsWrite()
{
    return m_supportsWrite;
}

void FdoClassCapabilities::SetSupportsWrite(FdoBoolean value)
{
    m_supportsWrite = value;
}



