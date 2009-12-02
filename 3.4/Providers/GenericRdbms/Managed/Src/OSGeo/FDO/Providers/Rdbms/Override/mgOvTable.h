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

class FdoRdbmsOvTable;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA
public __gc class PhysicalElementMapping;
END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE

///<summary>Abstract class defining physical schema overrides for a table.</summary>
public __gc class OvTable : public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping
{
public private:
	FdoRdbmsOvTable* GetImpObj();
	
public protected:
	OvTable(System::IntPtr unmanaged, System::Boolean autoDelete);

public:
    ///<summary>Gets the primary key nane</summary>
    ///<returns>Returns the primary key name</returns>
	__property System::String * get_PKeyName();

    ///<summary>Sets the primary key name</summary>
    /// <param name="keyName">Input the primary key name</param>
    ///<returns>Returns nothing</returns>
	__property System::Void set_PKeyName(System::String *keyName);
    
    // Makes SetParent function public.
	__property System::Void set_Parent(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping* value);
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE
