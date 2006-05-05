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

#ifndef FDOWFSCONNECTIONINFO_H
#define FDOWFSCONNECTIONINFO_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

class FdoWfsConnectionInfo :
    public FdoIConnectionInfo
{
    friend class FdoWfsConnection;

private:
    /**
     * The connection that spawned us.
     */
    FdoWfsConnection* mConnection; // weak reference

    /**
     * The dictionary of connection parameters.
     */
    FdoPtr<FdoCommonConnPropDictionary> mPropertyDictionary;

public:
    FdoWfsConnectionInfo ()  { FdoWfsConnectionInfo(NULL); }  // 0-argument constructor to please FdoPtr::operator->
    FdoWfsConnectionInfo (FdoWfsConnection* connection);

protected:
    virtual ~FdoWfsConnectionInfo (void);
    virtual void Dispose ();
    virtual void validate ();

public:
    /// <summary>Gets the name of the feature provider.</summary>
    /// <returns>Returns the provider name</returns>
    FdoString* GetProviderName ();

    /// <summary>Gets the display name of the feature provider.</summary>
    /// <returns>Returns the provider's display name</returns>
    FdoString* GetProviderDisplayName();

    /// <summary>Gets the description of the feature provider.</summary>
    /// <returns>Returns the provider description</returns>
    FdoString* GetProviderDescription ();

    /// <summary>Gets the version of the feature provider.</summary>
    /// <returns>Returns provider version</returns>
    FdoString* GetProviderVersion ();

    /// <summary>Gets the version of the Feature Data Objects specification to which this provider conforms.</summary>
    /// <returns>Returns FDO version supported.</returns>
    FdoString* GetFeatureDataObjectsVersion ();

    /// <summary>Gets the FdoIConnectionPropertyDictionary interface that can be used to dynamically query and set the properties required to establish a connection.</summary>
    /// <returns>Returns the property dictionary</returns>
    FdoIConnectionPropertyDictionary* GetConnectionProperties ();
};

#endif // FDOWFSCONNECTIONINFO_H
