#ifndef FDO_SqlServerOVPHYSICALSCHEMAMAPPING_H
#define FDO_SqlServerOVPHYSICALSCHEMAMAPPING_H
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

#include <Rdbms/Override/RdbmsOvPhysicalSchemaMapping.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOv.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvClassCollection.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvTextInRowOption.h>

/// \brief
/// Concrete class offering schema overrides for
/// the SqlServer Provider.
class FdoSqlServerOvPhysicalSchemaMapping : public FdoRdbmsOvPhysicalSchemaMapping
{

public:

    /// \brief
    /// Constructs a default of an FdoSqlServerOvPhysicalSchemaMapping
    /// 
    /// \return
    /// Returns FdoSqlServerOvPhysicalSchemaMapping
    /// 
    FDOSQLSERVER_OV_API static FdoSqlServerOvPhysicalSchemaMapping* Create();

    /// \brief
    /// Constructs an instance of an FdoSqlServerOvPhysicalSchemaMapping using the specified arguments
    /// 
    /// \param name 
    /// Input name
    /// 
    /// \return
    /// Returns FdoSqlServerOvPhysicalSchemaMapping
    /// 
    FDOSQLSERVER_OV_API static FdoSqlServerOvPhysicalSchemaMapping* Create(FdoString* name);

    /// \brief
    /// Gets the classes
    /// 
    /// \return
    /// Returns the classes
    /// 
    FDOSQLSERVER_OV_API FdoSqlServerOvClassCollection* GetClasses();

    /// \brief
    /// Gets the table Filegroup
    /// 
    /// \return
    /// Returns the table Filegroup
    /// 
    FDOSQLSERVER_OV_API FdoString *GetTableFilegroup();

    /// \brief
    /// Sets the table Filegroup
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOSQLSERVER_OV_API void SetTableFilegroup(FdoString *tableFilegroup);

    /// \brief
    /// Gets the text Filegroup
    /// 
    /// \return
    /// Returns the text Filegroup
    /// 
    FDOSQLSERVER_OV_API FdoString *GetTextFilegroup();

    /// \brief
    /// Sets the text Filegroup
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOSQLSERVER_OV_API void SetTextFilegroup(FdoString *textFilegroup);

    /// \brief
    /// Gets the index Filegroup
    /// 
    /// \return
    /// Returns the index Filegroup
    /// 
    FDOSQLSERVER_OV_API FdoString *GetIndexFilegroup();

    /// \brief
    /// Sets the index Filegroup
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOSQLSERVER_OV_API void SetIndexFilegroup(FdoString *indexFilegroup);

    /// \brief
    /// Gets the text-in-row option
    /// 
    /// \return
    /// Returns the text-in-row option
    /// 
    FDOSQLSERVER_OV_API SqlServerOvTextInRowOption GetTextInRow();

    /// \brief
    /// Sets the text-in-row option
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOSQLSERVER_OV_API void SetTextInRow(SqlServerOvTextInRowOption textInRow);

    /// \brief
    /// Gets the database
    /// 
    /// \return
    /// Returns the database
    /// 
    FDOSQLSERVER_OV_API FdoString *GetDatabase();

    /// \brief
    /// Sets the database
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOSQLSERVER_OV_API void SetDatabase(FdoString *database);

    /// \brief
    /// Gets the owner
    /// 
    /// \return
    /// Returns the owner
    /// 
    FDOSQLSERVER_OV_API FdoString *GetOwner();

    /// \brief
    /// Sets the owner
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOSQLSERVER_OV_API void SetOwner(FdoString *owner);

    /// \brief
    /// Gets the provider name
    /// 
    /// \return
    /// Returns the provider name
    /// 
    FDOSQLSERVER_OV_API virtual FdoString *GetProvider(void);

/// \cond DOXYGEN-IGNORE

    /// XML Serialization functions, not part of API.

    /// Serialize this property to XML.
    virtual void _writeXmlAttributes(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags);

    /// Initialize this element from its XML attributes
    virtual void InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs);

/// \endcond

protected:
    FdoSqlServerOvPhysicalSchemaMapping();
    FdoSqlServerOvPhysicalSchemaMapping(FdoString* name);
    virtual ~FdoSqlServerOvPhysicalSchemaMapping();
    virtual void Dispose();

    /// Create a class override from XML attributes
    virtual FdoRdbmsOvClassDefinition* CreateClass( 
        FdoXmlSaxContext* context,
        FdoXmlAttributeCollection* atts
    );

    /// Add the given class override to this schema override set
    virtual void AddClass( 
        FdoRdbmsOvClassDefinition* pClass
    );

    /// Common initializer for all constructors:
    void Init();

private:
    FdoSqlServerOvClassesP      mClasses;
    FdoStringP                  mTableFilegroup;
    FdoStringP                  mTextFilegroup;
    FdoStringP                  mIndexFilegroup;
    SqlServerOvTextInRowOption  mTextInRow;
    FdoStringP                  mDatabase;
    FdoStringP                  mOwner;
};

/// \brief
/// FdoSqlServerOvSchemaMappingP is a FdoPtr on FdoSqlServerOvPhysicalSchemaMapping, provided for convenience.
typedef FdoPtr<FdoSqlServerOvPhysicalSchemaMapping> FdoSqlServerOvSchemaMappingP;

#endif


