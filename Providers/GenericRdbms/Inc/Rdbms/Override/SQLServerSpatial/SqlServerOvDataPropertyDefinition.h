#ifndef FDO_SqlServerOVDATAPROPERTYDEFINITION_H
#define FDO_SqlServerOVDATAPROPERTYDEFINITION_H
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

#include <FdoCommon.h>
#include <Rdbms/Override/RdbmsOvDataPropertyDefinition.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOv.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvColumn.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPropertyDefinition.h>

/// \brief
/// Concrete class defining physical schema overrides for a data property definition.
class FdoSqlServerOvDataPropertyDefinition : public FdoRdbmsOvDataPropertyDefinition, public FdoSqlServerOvPropertyDefinition
{
public:
    /// \brief
    /// Constructs a default of an FdoSqlServerOvDataPropertyDefinition
    /// 
    /// \return
    /// Returns FdoSqlServerOvDataPropertyDefinition
    /// 
    FDOSQLSERVER_OV_API static FdoSqlServerOvDataPropertyDefinition* Create();

    /// \brief
    /// Constructs an instance of an FdoSqlServerOvDataPropertyDefinition using the specified arguments
    /// 
    /// \param name 
    /// Input name
    /// 
    /// \return
    /// Returns FdoSqlServerOvDataPropertyDefinition
    /// 
    FDOSQLSERVER_OV_API static FdoSqlServerOvDataPropertyDefinition* Create(FdoString* name);

    /// \brief
    /// Gets the SqlServer column
    /// 
    /// \return
    /// Returns the SqlServer column
    /// 
    FDOSQLSERVER_OV_API FdoSqlServerOvColumn* GetColumn();

    /// \brief
    /// Sets the SqlServer column
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOSQLSERVER_OV_API void SetColumn(FdoSqlServerOvColumn *column);

/* //TODO: enable this code when FdoSQLServerOvValidPropertyValueConstraint is ready

    /// \brief
    /// Get the single ValidPropertyValueConstraint
    /// 
    /// \return
    /// Returns the single ValidPropertyValueConstraint
    /// 
    FDOSQLSERVER_OV_API FdoSQLServerOvValidPropertyValueConstraint* GetValidPropertyValueConstraint();

    /// \brief
    /// Set the single ValidPropertyValueConstraint
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOSQLSERVER_OV_API void SetValidPropertyValueConstraint(FdoSQLServerOvValidPropertyValueConstraint* validPropertyValueConstraint);
*/

/// \cond DOXYGEN-IGNORE

/// \endcond

protected:
    FdoSqlServerOvDataPropertyDefinition();
    FdoSqlServerOvDataPropertyDefinition(FdoString* name);
    virtual ~FdoSqlServerOvDataPropertyDefinition();
    virtual void Dispose();

private:
};


/// \brief
/// FdoSqlServerOvDataPropertyP is a FdoPtr on FdoSqlServerOvDataPropertyDefinition, provided for convenience.
typedef FdoPtr<FdoSqlServerOvDataPropertyDefinition> FdoSqlServerOvDataPropertyP;

#endif


