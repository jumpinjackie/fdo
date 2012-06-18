#ifndef FDO_SqlServerOVTABLE_H
#define FDO_SqlServerOVTABLE_H
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

#include <Rdbms/Override/RdbmsOvTable.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOv.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvTextInRowOption.h>


/// \brief
/// Concrete class defining physical schema overrides for a table.
class FdoSqlServerOvTable : public FdoRdbmsOvTable
{

public:
    /// \brief
    /// Constructs a default of an FdoSqlServerOvTable
    /// 
    /// \return
    /// Returns FdoSqlServerOvTable
    /// 
    FDOSQLSERVER_OV_API static FdoSqlServerOvTable* Create();

    /// \brief
    /// Constructs an instance of an FdoSqlServerOvTable using the specified arguments
    /// 
    /// \param name 
    /// Input name
    /// 
    /// \return
    /// Returns FdoSqlServerOvTable
    /// 
    FDOSQLSERVER_OV_API static FdoSqlServerOvTable* Create(FdoString *name);

    /// \brief
    /// Gets the table filegroup
    /// 
    /// \return
    /// Returns the table filegroup
    /// 
    FDOSQLSERVER_OV_API FdoString *GetTableFilegroup();

    /// \brief
    /// Sets the table filegroup
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOSQLSERVER_OV_API void SetTableFilegroup(FdoString *tableFilegroup);

    /// \brief
    /// Gets the text filegroup
    /// 
    /// \return
    /// Returns the text filegroup
    /// 
    FDOSQLSERVER_OV_API FdoString *GetTextFilegroup();

    /// \brief
    /// Sets the text filegroup
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOSQLSERVER_OV_API void SetTextFilegroup(FdoString *textFilegroup);

    /// \brief
    /// Gets the index filegroup
    /// 
    /// \return
    /// Returns the index filegroup
    /// 
    FDOSQLSERVER_OV_API FdoString *GetIndexFilegroup();

    /// \brief
    /// Sets the index filegroup
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


/// \cond DOXYGEN-IGNORE

    /// XML Serialization functions, not part of the API.

    /// Serialize this property to XML.
    virtual void _writeXmlContents(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags);

    /// Initialize this element from its XML attributes
    virtual void InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs);

    /// Sets the parent of this object. Not part of the API.
    void SetParent(FdoPhysicalElementMapping* value);

    /// Common initializer for all constructors:
    void Init();

/// \endcond

protected:
    FdoSqlServerOvTable();
    FdoSqlServerOvTable(FdoString *name);
    virtual ~FdoSqlServerOvTable();
    virtual void Dispose();

private:
    FdoStringP                  mTableFilegroup;
    FdoStringP                  mTextFilegroup;
    FdoStringP                  mIndexFilegroup;
    SqlServerOvTextInRowOption  mTextInRow;
    FdoStringP                  mDatabase;
    FdoStringP                  mOwner;
};

/// \brief
/// FdoSqlServerOvTableP is a FdoPtr on FdoSqlServerOvTable, provided for convenience.
typedef FdoPtr<FdoSqlServerOvTable> FdoSqlServerOvTableP;

#endif


