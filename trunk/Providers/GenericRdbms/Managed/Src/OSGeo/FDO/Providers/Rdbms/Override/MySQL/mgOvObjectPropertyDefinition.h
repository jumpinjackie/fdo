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

#include "FDO\Providers\Rdbms\Override\MySQL\mgIOvPropertyDefinition.h"

class FdoMySQLOvObjectPropertyDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL

ref class OvPropertyMappingDefinition;
interface class IOvPropertyMappingDefinition;

///<summary>Concrete class defining physical schema overrides for an object property definition.</summary>
public ref class OvObjectPropertyDefinition : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvObjectPropertyDefinition, public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::IOvPropertyDefinition
{
internal:
	OvObjectPropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);
	FdoMySQLOvObjectPropertyDefinition* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;

public:
    ///<summary>Constructs a default of an OvObjectPropertyDefinition</summary>
    /// <returns>Returns OvObjectPropertyDefinition</returns>
    OvObjectPropertyDefinition();

    ///<summary>Constructs an instance of an OvObjectPropertyDefinition using the specified arguments</summary>
    /// <param name="name">Input name</param>
    /// <returns>Returns OvObjectPropertyDefinition</returns>
	OvObjectPropertyDefinition(System::String^ name);

    ///<summary>Gets the mapping definition</summary>
    /// <returns>Returns the mapping definition</returns>
    ///<summary>Sets the mapping definition</summary>
    /// <returns>Returns nothing</returns>
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::IOvPropertyMappingDefinition^ MappingDefinition
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::IOvPropertyMappingDefinition^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::IOvPropertyMappingDefinition^ value);
    }
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL

