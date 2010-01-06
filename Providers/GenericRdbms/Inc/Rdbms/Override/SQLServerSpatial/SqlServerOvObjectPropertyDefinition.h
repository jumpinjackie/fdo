#ifndef FDO_SqlServerOVOBJECTPROPERTYDEFINITION_H
#define FDO_SqlServerOVOBJECTPROPERTYDEFINITION_H
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
#include <Fdo/Commands/Schema/PhysicalElementMapping.h>
#include <Rdbms/Override/RdbmsOvObjectPropertyDefinition.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOv.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPropertyMappingDefinition.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPropertyDefinition.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPropertyMappingClass.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPropertyMappingConcrete.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPropertyMappingSingle.h>

/// \brief
/// Concrete class defining physical schema overrides for an object property definition.
class FdoSqlServerOvObjectPropertyDefinition : public FdoRdbmsOvObjectPropertyDefinition, public FdoSqlServerOvPropertyDefinition
{
public:
    /// \brief
    /// Constructs a default of an FdoSqlServerOvObjectPropertyDefinition
    /// 
    /// \return
    /// Returns FdoSqlServerOvObjectPropertyDefinition
    /// 
    FDOSQLSERVER_OV_API static FdoSqlServerOvObjectPropertyDefinition* Create();

    /// \brief
    /// Constructs an instance of an FdoSqlServerOvObjectPropertyDefinition using the specified arguments
    /// 
    /// \param name 
    /// Input name
    /// 
    /// \return
    /// Returns FdoSqlServerOvObjectPropertyDefinition
    /// 
    FDOSQLSERVER_OV_API static FdoSqlServerOvObjectPropertyDefinition* Create(FdoString* name);

    /// \brief
    /// Gets the mapping definition
    /// 
    /// \return
    /// Returns the mapping definition
    /// 
    FDOSQLSERVER_OV_API FdoSqlServerOvPropertyMappingDefinition* GetMappingDefinition();

    /// \brief
    /// Sets the mapping definition
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOSQLSERVER_OV_API void SetMappingDefinition(FdoSqlServerOvPropertyMappingDefinition* propertyMappingDefinition);

protected:
    FdoSqlServerOvObjectPropertyDefinition();
    FdoSqlServerOvObjectPropertyDefinition(FdoString* name);
    virtual ~FdoSqlServerOvObjectPropertyDefinition();
    virtual void Dispose();

    /// Create and optionally attach a Single table property mapping.
    virtual FdoRdbmsOvPropertyMappingSingle* CreateSingleMapping(
        bool attach
    );

    /// Create and optionally attach a Class table property mapping.
    virtual FdoRdbmsOvPropertyMappingClass* CreateClassMapping(
        bool attach
    );

    /// Create and optionally attach a Concrete table property mapping.
    virtual FdoRdbmsOvPropertyMappingConcrete* CreateConcreteMapping(
        bool attach
    );

private:
};


/// \brief
/// FdoSqlServerOvObjectPropertyP is a FdoPtr on FdoSqlServerOvObjectPropertyDefinition, provided for convenience.
typedef FdoPtr<FdoSqlServerOvObjectPropertyDefinition> FdoSqlServerOvObjectPropertyP;

#endif


