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
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpConnectionInfo.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */
#ifndef FDORFPCONNECTIONINFO_H
#define FDORFPCONNECTIONINFO_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

class FdoRfpConnectionPropertyDictionary;
class FdoRfpConnection;

class FdoRfpConnectionInfo :
    public FdoIConnectionInfo
{
    friend class FdoRfpConnection;
private:
    FdoRfpConnection* m_connection;
    /**
     * The dictionary of connection parameters.
     */
    FdoPtr<FdoCommonConnPropDictionary> mPropertyDictionary;

public:
    FdoRfpConnectionInfo(FdoRfpConnection* connection);
protected:
    virtual ~FdoRfpConnectionInfo(void);
    virtual void Dispose ();
    virtual void validate ();

public:
    /// <summary>Gets the name of the feature provider.</summary>
    /// <returns>Returns the provider name</returns>
    FdoString* GetProviderName ();

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

    /// <summary>Gets the display name of the feature provider.</summary>
    /// <returns>Returns the provider's display name.</returns>
    FdoString* GetProviderDisplayName();
};

#endif

