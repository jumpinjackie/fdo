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

class FdoMySQLOvClassDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE
public __gc class OvClassDefinition;
END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL

public __gc class OvPropertyDefinitionCollection;
public __gc class OvTable;
public __gc class OvGeometricColumn;

///<summary>Concrete class defining physical schema overrides for a class definition.</summary>
public __gc class OvClassDefinition : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition
{
public private:
	FdoMySQLOvClassDefinition* GetImpObj();
	
	OvClassDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

public:
    ///<summary>Constructs a default of an OvClassDefinition</summary>
    /// <returns>Returns OvClassDefinition</returns>
    OvClassDefinition();

    ///<summary>Constructs an instance of an OvClassDefinition using the specified arguments</summary>
    /// <param name="name">Input name</param>
    /// <returns>Returns OvClassDefinition</returns>
	OvClassDefinition(System::String* name);

    ///<summary>Gets a collection of MySQL property overrides</summary>
    /// <returns>Returns the collection of MySQL property overrides</returns>
	__property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvPropertyDefinitionCollection* get_Properties();

    ///<summary>Gets the MySQL table override for this class override</summary>
    /// <returns>Returns OvTable</returns>
	__property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable* get_Table();

    ///<summary>Sets the MySQL table override for this class override</summary>
    /// <param name="name">Input MySQL table override</param>
    /// <returns>Returns nothing</returns>
	__property System::Void set_Table(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL::OvTable* table);
   
    ///<summary>Which FDO property maps to an MySQL auto-increment column, if any.</summary>
    /// <remarks>An empty string indicates
    /// there is no such column on this class’s table.  There can only be at most one auto-increment column
    /// per table, it must be indexed, and it cannot have a default value.  As of MySQL 3.23, an AutoIncrement
    /// column works properly only if it contains only positive values.  Inserting a negative number is
    /// regarded as inserting a very large positive number.  For MyISAM and BDB tables, you can specify an
    /// AUTO_INCREMENT secondary column in a multiple-column key.  The first value in an AutoIncrement column
    /// is always 1 and always increments by 1 for each new value.  This override is ignored on ApplySchema if
    /// this class refers to an existing table in a foreign database.
    /// </remarks>
    /// <param name="name"></param>
    /// <returns>Returns nothing</returns>
	__property System::Void set_AutoIncrementPropertyName(System::String* autoIncrementPropertyName);

    ///<summary>Gets which FDO property corresponds to the single MySQL auto-increment column</summary>
    /// <returns>Returns the FDO property corresponding to the single MySQL auto-increment column,
    /// or an empty string if there is no such property</returns>
	__property System::String* get_AutoIncrementPropertyName();

    ///<summary>The initial value to use for the first row in the auto-increment column, if any.</summary>
    /// <remarks>This override is only valid if the AutoIncrementProperty override is set to a non-empty string.
    /// This works only for the following storage engine types: MyISAM, MEMORY as of MySQL 4.1, and InnoDB
    /// as of MySQL 5.0.3.  This override is ignored on ApplySchema if this table is an existing table in
    /// a foreign database.
    /// </remarks>
    /// <returns>Returns the initial value to use for the first row in the auto-increment column</returns>
	__property System::Int64 get_AutoIncrementSeed();

    ///<summary>Sets the initial value to use for the first row in the auto-increment column, if any</summary>
    /// <returns>Returns nothing</returns>
	__property System::Void set_AutoIncrementSeed(System::Int64 autoIncrementSeed);

};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL
