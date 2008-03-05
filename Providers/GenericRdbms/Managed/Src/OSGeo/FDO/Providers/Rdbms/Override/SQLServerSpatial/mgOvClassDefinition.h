//
// Copyright (C) 2004-2008  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#pragma once

class FdoSqlServerOvClassDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
public __gc class OvPropertyDefinitionCollection;
public __gc class OvTable;
public __gc class OvGeometricColumn;

///<summary>Concrete class defining physical schema overrides for a class definition.</summary>
public __gc class OvClassDefinition : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvClassDefinition
{
public:
    ///<summary>Constructs a default of an NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition</summary>
	OvClassDefinition();

    ///<summary>Constructs an instance of an NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvClassDefinition using the specified arguments</summary>
    /// <param name="name">Input name</param>
	OvClassDefinition(System::String* name);

    ///<summary>Gets a collection of SqlServer property overrides</summary>
    /// <returns>Returns the collection of SqlServer property overrides</returns>
    __property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvPropertyDefinitionCollection* get_Properties();

    ///<summary>Gets the SqlServer table override for this class override</summary>
    /// <returns>Returns NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvTable</returns>
    __property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvTable* get_Table();

    ///<summary>Sets the SqlServer table override for this class override</summary>
    /// <param name="name">Input SqlServer table override</param>
    /// <returns>Returns nothing</returns>
    __property System::Void set_Table(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL::OvTable* table);
   
    ///<summary>Specifies which FDO property corresponds to the single SqlServer IDENTITY column.
    /// This property corresponds to the singleton IDENTITY column of the SQL Server table.
    /// This is not to be confused with the usual FDO identity properties, which instead 
    /// correspond to the primary key of a SQL Server table.
    /// </summary>
    /// <param name="name">Input SqlServer identity property name override.
    /// An empty string means there is no identity column for this class.</param>
    /// <returns>Returns nothing</returns>
    __property System::Void set_IdentityPropertyName(System::String* identityPropertyName);

    ///<summary>Gets which FDO property corresponds to the single SqlServer IDENTITY column</summary>
    /// <returns>Returns the FDO property corresponding to the single SqlServer IDENTITY column,
    /// or an empty string if there is no such property</returns>
    __property System::String* get_IdentityPropertyName();

    ///<summary>Retrieves the identity column's type, whether it is a non-globally-unique
    /// identity column or a globally-unique identity column.</summary>
    /// <remarks>Non-globally-unique identity columns have their value
    /// automatically populated by SQL Server on insert; the first value is set to the specified
    /// IdentitySeed, and all subsequent values are set using the specified IdentityIncrement.
    /// If defining such a column during ApplySchema, it must be one of the following SQL Server
    /// native data types: decimal, int, numeric, smallint, bigint, or tinyint.  Note that the
    /// “not for replication?identity column flag is not exposed.  Also note that the identifier
    /// column must not be nullable and must not contain a default value.
    /// Globally-unique identity columns do not have their value automatically populated by SQL Server.
    /// Instead SQL Server relies on the default value or the user to populate this value.  To have
    /// this value populated automatically on insert, the user should set the default value to use the
    /// NEWID() function to obtain a globally-unique identity value.
    /// This override is ignored on ApplySchema if this column belongs to an existing table in a
    /// foreign database.  This override is also ignored if IdentityPropertyName is not specified.
    /// </remarks>
    /// <returns>Returns the type of the single IDENTITY column, if it exists</returns>
    __property System::Boolean get_IdentityIsGloballyUnique();

    ///<summary>Set the type of the single IDENTITY column, if it exists</summary>
    /// <returns>Returns nothing</returns>
    __property System::Void set_IdentityIsGloballyUnique(System::Boolean isGloballyUnique);

    ///<summary>Sets the first value to use for the identity column, if there is one.</summary>
    /// <remarks>This override defaults to 1.  This override is ignored on ApplySchema if
    /// this column belongs to an existing table in a foreign database.  This override is also
    /// ignored if IdentityPropertyName is not specified.</remarks>
    /// <returns>Returns the first value to use for the identity column</returns>
    __property System::Int32 get_IdentitySeed();

    ///<summary>Gets the first value to use for the identity column, if there is one.</summary>
    /// <returns>Returns nothing</returns>
    __property System::Void set_IdentitySeed(System::Int32 identitySeed);

    ///<summary>Gets the increment value to use for the identity column, if there is one.</summary>
    /// <remarks>This override defaults to 1.  This override is ignored on ApplySchema if this
    /// column belongs to an existing table in a foreign database.  This override is also ignored
    /// if IdentityPropertyName is not specified.</remarks>
    /// <returns>Returns the increment value to use for the identity column</returns>
    __property System::Int32 get_IdentityIncrement();

    ///<summary>Sets the increment value to use for the identity column, if there is one.</summary>
    /// <returns>Returns nothing</returns>
    __property System::Void set_IdentityIncrement(System::Int32 identityIncrement);

public private:
	OvClassDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoSqlServerOvClassDefinition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
