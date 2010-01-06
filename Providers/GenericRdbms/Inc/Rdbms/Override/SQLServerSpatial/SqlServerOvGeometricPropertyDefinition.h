#ifndef FDO_SqlServerOVGEOMETRICPROPERTYDEFINITION_H
#define FDO_SqlServerOVGEOMETRICPROPERTYDEFINITION_H
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

#include <Rdbms/Override/RdbmsOvGeometricPropertyDefinition.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOv.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvGeometricColumn.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPropertyDefinition.h>

/// \brief
/// Concrete class defining physical schema overrides for a geometric property definition.
class FdoSqlServerOvGeometricPropertyDefinition : public FdoRdbmsOvGeometricPropertyDefinition, public FdoSqlServerOvPropertyDefinition
{
public:
    /// \brief
    /// Constructs a default of an FdoSqlServerOvGeometricPropertyDefinition
    /// 
    /// \return
    /// Returns FdoSqlServerOvGeometricPropertyDefinition
    /// 
    FDOSQLSERVER_OV_API static FdoSqlServerOvGeometricPropertyDefinition* Create();

    /// \brief
    /// Constructs an instance of an FdoSqlServerOvGeometricPropertyDefinition using the specified arguments
    /// 
    /// \param name 
    /// Input name
    /// 
    /// \return
    /// Returns FdoSqlServerOvGeometricPropertyDefinition
    /// 
    FDOSQLSERVER_OV_API static FdoSqlServerOvGeometricPropertyDefinition* Create(FdoString* name);

    /// \brief
    /// Gets the column
    /// 
    /// \return
    /// Returns the column
    /// 
    FDOSQLSERVER_OV_API FdoSqlServerOvGeometricColumn* GetColumn();

    /// \brief
    /// Sets the column
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOSQLSERVER_OV_API void SetColumn(FdoSqlServerOvGeometricColumn* geometricColumn);


protected:
/// \cond DOXYGEN-IGNORE

    FdoSqlServerOvGeometricPropertyDefinition();
    FdoSqlServerOvGeometricPropertyDefinition(FdoString* name);
    virtual ~FdoSqlServerOvGeometricPropertyDefinition();
    virtual void Dispose();
/// \endcond

private:
};

/// \brief
/// FdoSqlServerOvGeometricPropertyP is a FdoPtr on FdoSqlServerOvGeometricPropertyDefinition, provided for convenience.
typedef FdoPtr<FdoSqlServerOvGeometricPropertyDefinition> FdoSqlServerOvGeometricPropertyP;

#endif


