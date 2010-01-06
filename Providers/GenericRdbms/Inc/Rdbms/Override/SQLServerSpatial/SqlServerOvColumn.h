#ifndef FDO_SqlServerOvColumn_H
#define FDO_SqlServerOvColumn_H
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

#include <Rdbms/Override/SQLServerSpatial/SqlServerOv.h>
#include <Rdbms/Override/RdbmsOvColumn.h>

/// \brief
/// Concrete class defining physical schema overrides for a column.
class FdoSqlServerOvColumn : public FdoRdbmsOvColumn
{

public:
    /// \brief
    /// Constructs a default of an FdoSqlServerOvColumn
    /// 
    /// \return
    /// Returns FdoSqlServerOvColumn
    /// 
    FDOSQLSERVER_OV_API static FdoSqlServerOvColumn* Create();

    /// \brief
    /// Constructs an instance of an FdoSqlServerOvColumn using the specified arguments
    /// 
    /// \param name 
    /// Input name
    /// 
    /// \return
    /// Returns FdoSqlServerOvColumn
    /// 
    FDOSQLSERVER_OV_API static FdoSqlServerOvColumn* Create(FdoString* name);

    /// \brief
    /// Gets the SQL Server formula for this column
    /// 
    /// \remarks
    /// An empty string denotes that there is no formula for this column;
    /// otherwise, the column's value is computed based on the formula.
    /// 
    /// \return
    /// Returns the SQL Server formula
    /// 
    FDOSQLSERVER_OV_API FdoString* GetFormula();

    /// \brief
    /// Sets the SQL Server formula for this column
    /// 
    /// \remarks
    /// An empty string denotes that there is no formula for this column;
    /// otherwise, the column's value is computed based on the formula.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOSQLSERVER_OV_API void SetFormula(FdoString* formula);

/// \cond DOXYGEN-IGNORE

    /// XML serialization functions, not part of the API.

    /// Initialize this element from its XML attributes
    virtual void InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs);

/// \endcond

protected:
    FdoSqlServerOvColumn();
    FdoSqlServerOvColumn(FdoString* name);
    virtual ~FdoSqlServerOvColumn();
    virtual void Dispose();

    /// Serialize this property to XML.
    virtual void _writeXmlContents(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags);

private:
    FdoStringP mFormula;
};

/// \brief
/// FdoSqlServerOvColumnP is a FdoPtr on FdoSqlServerOvColumn, provided for convenience.
typedef FdoPtr<FdoSqlServerOvColumn> FdoSqlServerOvColumnP;

#endif


