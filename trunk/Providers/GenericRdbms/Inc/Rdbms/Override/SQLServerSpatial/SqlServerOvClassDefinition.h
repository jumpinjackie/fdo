#ifndef FDO_SqlServerOVCLASSDEFINITION_H
#define FDO_SqlServerOVCLASSDEFINITION_H
/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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

#include <Rdbms/Override/RdbmsOvClassDefinition.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOv.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvDataPropertyDefinition.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvGeometricPropertyDefinition.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvObjectPropertyDefinition.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPropertyDefinitionCollection.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvTable.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvGeometricColumn.h>

/// \brief
/// Concrete class defining physical schema overrides for a class definition.
class FdoSqlServerOvClassDefinition : public FdoRdbmsOvClassDefinition
{

public:
    /// \brief
    /// Constructs a default of an FdoSqlServerOvClassDefinition
    /// 
    /// \return
    /// Returns FdoSqlServerOvClassDefinition
    /// 
    FDOSQLSERVER_OV_API static FdoSqlServerOvClassDefinition* Create();

    /// \brief
    /// Constructs an instance of an FdoSqlServerOvClassDefinition using the specified arguments
    /// 
    /// \param name 
    /// Input name
    /// 
    /// \return
    /// Returns FdoSqlServerOvClassDefinition
    /// 
    FDOSQLSERVER_OV_API static FdoSqlServerOvClassDefinition* Create(FdoString* name);

    /// \brief
    /// Gets a collection of SqlServer property overrides
    /// 
    /// \return
    /// Returns the collection of SqlServer property overrides
    /// 
    FDOSQLSERVER_OV_API FdoSqlServerOvPropertyDefinitionCollection* GetProperties();

    /// \brief
    /// Gets the SqlServer table override for this class override
    /// 
    /// \return
    /// Returns FdoSqlServerOvTable
    /// 
    FDOSQLSERVER_OV_API FdoSqlServerOvTable* GetTable();

    /// \brief
    /// Sets the SqlServer table override for this class override
    /// 
    /// \param name 
    /// Input SqlServer table override
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOSQLSERVER_OV_API void SetTable(FdoSqlServerOvTable* table);

    /// \brief
    /// Sets the SqlServer geometry column override for this class override
    /// 
    /// \param column 
    /// Input SqlServer Geometric Column override
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOSQLSERVER_OV_API void SetGeometryColumn(FdoSqlServerOvGeometricColumn* column);

    /// \brief
    /// Specifies which FDO property corresponds to the single SqlServer IDENTITY column.
    /// This property corresponds to the singleton IDENTITY column of the SQL Server table.
    /// This is not to be confused with the usual FDO identity properties, which instead 
    /// correspond to the primary key of a SQL Server table.
    /// 
    /// \param name 
    /// Input SqlServer identity property name override.
    /// An empty string means there is no identity column for this class.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOSQLSERVER_OV_API void SetIdentityPropertyName(FdoString* identityPropertyName);

    /// \brief
    /// Gets which FDO property corresponds to the single SqlServer IDENTITY column
    /// 
    /// \return
    /// Returns the FDO property corresponding to the single SqlServer IDENTITY column,
    /// or an empty string if there is no such property
    /// 
    FDOSQLSERVER_OV_API FdoString* GetIdentityPropertyName(void);

    /// \brief
    /// Retrieves the identity column's type, whether it is a non-globally-unique
    /// identity column or a globally-unique identity column.
    /// 
    /// \remarks
    /// Non-globally-unique identity columns have their value
    /// automatically populated by SQL Server on insert; the first value is set to the specified
    /// IdentitySeed, and all subsequent values are set using the specified IdentityIncrement.
    /// If defining such a column during ApplySchema, it must be one of the following SQL Server
    /// native data types: decimal, int, numeric, smallint, bigint, or tinyint.  Note that the
    /// “not for replication” identity column flag is not exposed.  Also note that the identifier
    /// column must not be nullable and must not contain a default value.
    /// Globally-unique identity columns do not have their value automatically populated by SQL Server.
    /// Instead SQL Server relies on the default value or the user to populate this value.  To have
    /// this value populated automatically on insert, the user should set the default value to use the
    /// NEWID() function to obtain a globally-unique identity value.
    /// This override is ignored on ApplySchema if this column belongs to an existing table in a
    /// foreign database.  This override is also ignored if IdentityPropertyName is not specified.
    /// 
    /// \return
    /// Returns the type of the single IDENTITY column, if it exists
    /// 
    FDOSQLSERVER_OV_API bool GetIdentityIsGloballyUnique(void);

    /// \brief
    /// Set the type of the single IDENTITY column, if it exists
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOSQLSERVER_OV_API void SetIdentityIsGloballyUnique(bool identityisGloballyUnique);

    /// \brief
    /// Sets the first value to use for the identity column, if there is one.
    /// 
    /// \remarks
    /// This override defaults to 1.  This override is ignored on ApplySchema if
    /// this column belongs to an existing table in a foreign database.  This override is also
    /// ignored if IdentityPropertyName is not specified.
    /// 
    /// \return
    /// Returns the first value to use for the identity column
    /// 
    FDOSQLSERVER_OV_API FdoInt32 GetIdentitySeed(void);

    /// \brief
    /// Gets the first value to use for the identity column, if there is one.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOSQLSERVER_OV_API void SetIdentitySeed(FdoInt32 identitySeed);

    /// \brief
    /// Gets the increment value to use for the identity column, if there is one.
    /// 
    /// \remarks
    /// This override defaults to 1.  This override is ignored on ApplySchema if this
    /// column belongs to an existing table in a foreign database.  This override is also ignored
    /// if IdentityPropertyName is not specified.
    /// 
    /// \return
    /// Returns the increment value to use for the identity column
    /// 
    FDOSQLSERVER_OV_API FdoInt32 GetIdentityIncrement(void);

    /// \brief
    /// Sets the increment value to use for the identity column, if there is one.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOSQLSERVER_OV_API void SetIdentityIncrement(FdoInt32 identityIncrement);

/// \cond DOXYGEN-IGNORE

    /// Sets the parent of this object. Not part of the API.
    void SetParent(FdoPhysicalElementMapping* value);

    /// Initialize this element from its XML attributes
    virtual void InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs);

    /// Serialize the XML attributes of this class:
    virtual void _writeXmlAttributes(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags);

/// \endcond

protected:
    FdoSqlServerOvClassDefinition();
    FdoSqlServerOvClassDefinition(FdoString *name);

    virtual ~FdoSqlServerOvClassDefinition();
    void Init();
    virtual void Dispose();

    /// Create table override from XML attributes
    virtual void SetTable(
        FdoXmlSaxContext* context,
        FdoXmlAttributeCollection* atts
    );

    /// Create data property override from XML attributes
    virtual FdoRdbmsOvDataPropertyDefinition* CreateDataProperty( 
        FdoXmlSaxContext* context,
        FdoXmlAttributeCollection* propAtts,
        FdoXmlAttributeCollection* colAtts
    );

    /// Create geometric property override from XML attributes
    virtual FdoRdbmsOvGeometricPropertyDefinition* CreateGeometricProperty( 
        FdoXmlSaxContext* context,
        FdoXmlAttributeCollection* propAtts,
        FdoXmlAttributeCollection* colAtts
    );

    /// Create object property override from XML attributes
    virtual FdoRdbmsOvObjectPropertyDefinition* CreateObjectProperty( 
        FdoXmlSaxContext* context,
        FdoXmlAttributeCollection* propAtts,
        FdoString* mappingType,
        FdoXmlAttributeCollection* mappingAtts
    );

    /// Add the given property override to this class override
    virtual void AddProperty( 
        FdoRdbmsOvPropertyDefinition* pProp
    );

private:
    FdoSqlServerOvPropertiesP  mPropertyDefinitions;
    bool  mIdentityIsGloballyUnique;
    FdoInt32  mIdentitySeed;
    FdoInt32  mIdentityIncrement;
    FdoStringP  mIdentityPropertyName;
};


/// \brief
/// FdoSqlServerOvClassP is a FdoPtr on FdoSqlServerOvClassDefinition, provided for convenience.
typedef FdoPtr<FdoSqlServerOvClassDefinition> FdoSqlServerOvClassP;

#endif


