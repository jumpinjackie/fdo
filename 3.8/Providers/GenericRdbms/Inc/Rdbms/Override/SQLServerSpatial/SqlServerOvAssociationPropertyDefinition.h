#ifndef FDO_SqlServerOVASSOCIATIONPROPERTYDEFINITION_H
#define FDO_SqlServerOVASSOCIATIONPROPERTYDEFINITION_H
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
#include <Rdbms/Override/SQLServerSpatial/SqlServerOv.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPropertyDefinitionCollection.h>
#include <Rdbms/Override/RdbmsOvAssociationPropertyDefinition.h>


/// \brief
/// Concrete class defining physical schema overrides for an association property
/// definition.
class FdoSqlServerOvAssociationPropertyDefinition : public FdoSqlServerOvPropertyDefinition, public FdoRdbmsOvAssociationPropertyDefinition
{
public:
    /// \brief
    /// Constructs a default of an FdoSqlServerOvAssociationPropertyDefinition
    /// 
    /// \return
    /// Returns FdoSqlServerOvAssociationPropertyDefinition
    /// 
    FDOSQLSERVER_OV_API static FdoSqlServerOvAssociationPropertyDefinition* Create();

    /// \brief
    /// Constructs an instance of an FdoSqlServerOvAssociationPropertyDefinition using the specified arguments
    /// 
    /// \param name 
    /// Input name
    /// 
    /// \return
    /// Returns FdoSqlServerOvAssociationPropertyDefinition
    /// 
    FDOSQLSERVER_OV_API static FdoSqlServerOvAssociationPropertyDefinition* Create(FdoString* name);

    /// \brief
    /// Gets a collection of property definitions
    /// 
    /// \return
    /// Returns a collection of property definitions
    /// 
    FDOSQLSERVER_OV_API FdoSqlServerOvPropertyDefinitionCollection *GetIdentityProperties();

protected:
    FdoSqlServerOvAssociationPropertyDefinition();
    FdoSqlServerOvAssociationPropertyDefinition(FdoString* name);
    virtual ~FdoSqlServerOvAssociationPropertyDefinition();
    virtual void Dispose();

private:
    void Init();
    FdoSqlServerOvPropertiesP mPropertyDefinitions;
};

#endif


