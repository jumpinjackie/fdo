#ifndef _FDORDBMSMYSQLCONNECTIONCAPABILITIES_H_
#define _FDORDBMSMYSQLCONNECTIONCAPABILITIES_H_
#ifdef _WIN32
#pragma once
#endif
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


#include "Fdo.h"
#include "../../Fdo/Capability/FdoRdbmsConnectionCapabilities.h"


class FdoRdbmsMySqlConnectionCapabilities : public FdoRdbmsConnectionCapabilities
{
public:
    FdoRdbmsMySqlConnectionCapabilities(void);

    // Returns true if the feature provider supports feature locking.
    virtual bool SupportsLocking();

    /// <summary>Gets an array of the FdoLockType values supported by the feature provider.</summary>
    /// <param name="size">Output the number of lock types</param>
    /// <returns>Returns the list of lock types</returns>
    virtual FdoLockType* GetLockTypes(FdoInt32& size);

    // Returns true if the feature provider supports long transactions.
    virtual bool SupportsLongTransactions();

    // Returns true if the provider supports specifying the coordinate system by name or ID without specifying the WKT
    // when creating a new spatial context.
    virtual bool SupportsCSysWKTFromCSysName();

protected:
    ~FdoRdbmsMySqlConnectionCapabilities(void);
    virtual void Dispose();
};
#endif
