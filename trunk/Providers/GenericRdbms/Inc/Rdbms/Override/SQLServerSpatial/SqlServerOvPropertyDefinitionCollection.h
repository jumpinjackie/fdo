#ifndef FDO_SqlServerOVPROPERTYDEFINITIONCOLLECTION_H
#define FDO_SqlServerOVPROPERTYDEFINITIONCOLLECTION_H
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
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPropertyDefinition.h>
#include <Rdbms/Override/RdbmsOvPropertyDefinitionCollection.h>
#include <Fdo/Commands/Schema/PhysicalElementMappingCollection.h>

/// \brief
/// Concrete class defining physical schema overrides for a collection of property
/// definitions.
class FdoSqlServerOvPropertyDefinitionCollection : public FdoRestrictedNamedCollection<FdoRdbmsOvPropertyDefinition,FdoSqlServerOvPropertyDefinition,FdoRdbmsOvPropertyDefinitionCollection>
{
public:
    /// \brief
    /// Constructs a default of an FdoSqlServerOvPropertyDefinitionCollection
    /// 
    /// \return
    /// Returns FdoSqlServerOvPropertyDefinitionCollection
    /// 
    FDOSQLSERVER_OV_API static FdoSqlServerOvPropertyDefinitionCollection* Create();

    /// \brief
    /// Constructs an instance of an FdoSqlServerOvPropertyDefinitionCollection using the specified arguments
    /// 
    /// \param parent 
    /// Input parent
    /// 
    /// \return
    /// Returns FdoSqlServerOvPropertyDefinitionCollection
    /// 
    FDOSQLSERVER_OV_API static FdoSqlServerOvPropertyDefinitionCollection* Create(FdoRdbmsOvPropertyDefinitionCollection* baseCollection );

protected:
    FdoSqlServerOvPropertyDefinitionCollection();
    FdoSqlServerOvPropertyDefinitionCollection(FdoRdbmsOvPropertyDefinitionCollection* baseCollection);
    virtual ~FdoSqlServerOvPropertyDefinitionCollection();
    virtual void Dispose();

    virtual FdoSqlServerOvPropertyDefinition* DownCast( FdoRdbmsOvPropertyDefinition* value ) const;
};

/// \brief
/// FdoSqlServerOvPropertiesP is a FdoPtr on FdoSqlServerOvPropertyDefinitionCollection, provided for convenience.
typedef FdoPtr<FdoSqlServerOvPropertyDefinitionCollection> FdoSqlServerOvPropertiesP;


#endif


