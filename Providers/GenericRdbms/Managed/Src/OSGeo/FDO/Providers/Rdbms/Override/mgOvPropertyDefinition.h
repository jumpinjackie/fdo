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

#include "mgIOvPropertyDefinition.h"
class FdoRdbmsOvPropertyDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA
ref class PhysicalElementMapping;
END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE

///<summary>Abstract class defining physical schema overrides for a property
///definition.</summary>
public ref class OvPropertyDefinition : public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalPropertyMapping, public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::IOvPropertyDefinition
{	
internal:
	FdoRdbmsOvPropertyDefinition* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
	
public protected:
	OvPropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

public:
    // Makes SetParent function public
    virtual property NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping^ Parent
    {
        System::Void set(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping^ value);
    }
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE

