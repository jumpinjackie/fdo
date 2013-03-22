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

#pragma once

#include "mgIDestroySpatialIndex.h"

class FdoIDestroySpatialIndex;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS

/// <summary>The IDestroySpatialIndex interface defines the DestroySpatialIndex
/// command, which drops an existing spatial index. Input to the command is
/// the name of the index to drop. </summary>
private ref class IDestroySpatialIndexImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::IDestroySpatialIndex
{
internal:
	IDestroySpatialIndexImp(System::IntPtr unmanaged, System::Boolean autoDelete);   

	FdoIDestroySpatialIndex* GetImpObj();
	
public:
    virtual IntPtr GetDisposableObject() override;

public:
    /// <summary>Gets the name of the spatial index to destroy as a String.</summary>
    /// <returns>Returns the name of the spatial index to be drop</returns>
    /// <summary>Sets the name of the spatial index to drop as a String.</summary>
    /// <param name="value">Input the name of the spatial index to be drop</param>
    /// <returns>Returns nothing</returns>
    virtual property System::String^ Name
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// <summary>Sets the desired value of the geometric property.</summary>
    /// <param name="value">Input the geometric property definition </param>
    /// <returns>Returns nothing</returns>
    /// <summary>Gets the geometric property previously set, if any.</summary>
    /// <returns>Returns NULL if not set or a pointer to a geometry property definition</returns>
    virtual property NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition^ GeometricProperty
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition^ value);
    }

    /// <summary>Executes the DestroySpatialIndex command dropping the specified
    /// spatial index. An exception is thrown if the spatial index does not exist.</summary>
    /// <returns>Returns nothing</returns>
    virtual System::Void Execute();
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS

