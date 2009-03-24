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

class FdoMySQLOvDataPropertyDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL

ref class OvColumn;

///<summary>Concrete class defining physical schema overrides for a data property definition.</summary>
public ref class OvDataPropertyDefinition : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvDataPropertyDefinition, public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::IOvPropertyDefinition
{
internal:
	OvDataPropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);
	FdoMySQLOvDataPropertyDefinition* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;

public:
    ///<summary>Constructs a default of an OvDataPropertyDefinition</summary>
    /// <returns>Returns OvDataPropertyDefinition</returns>
    OvDataPropertyDefinition();

    ///<summary>Constructs an instance of an OvDataPropertyDefinition using the specified arguments</summary>
    /// <param name="name">Input name</param>
    /// <returns>Returns OvDataPropertyDefinition</returns>
	OvDataPropertyDefinition(System::String^ name);

    ///<summary>Gets the MySQL column</summary>
    /// <returns>Returns the MySQL column</returns>
    ///<summary>Sets the MySQL column</summary>
    /// <returns>Returns nothing</returns>
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvColumn^ Column
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvColumn^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvColumn^ column);
    }
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL

