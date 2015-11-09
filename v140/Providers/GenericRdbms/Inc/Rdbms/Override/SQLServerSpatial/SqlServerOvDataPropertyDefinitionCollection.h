#ifndef FDO_SqlServerOVDATAPROPERTYDEFINITIONCOLLECTION_H
#define FDO_SqlServerOVDATAPROPERTYDEFINITIONCOLLECTION_H

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

#include <Fdo/Schema/SchemaException.h>
#include <Fdo/Commands/Schema/PhysicalElementMappingCollection.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOv.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvDataPropertyDefinition.h>

/// \brief
/// Concrete class defining physical schema overrides for a collection of
/// data properties.
class FdoSqlServerOvDataPropertyDefinitionCollection : public FdoPhysicalElementMappingCollection<FdoSqlServerOvDataPropertyDefinition>
{
public:
    /// \brief
    /// Constructs a default of an FdoSqlServerOvDataPropertyDefinitionCollection
    /// 
    /// \return
    /// Returns FdoSqlServerOvDataPropertyDefinitionCollection
    /// 
    FDOSQLSERVER_OV_API static FdoSqlServerOvDataPropertyDefinitionCollection* Create();

    /// \brief
    /// Constructs an instance of an FdoSqlServerOvDataPropertyDefinitionCollection using the specified arguments
    /// 
    /// \param name 
    /// Input name
    /// 
    /// \return
    /// Returns FdoSqlServerOvDataPropertyDefinitionCollection
    /// 
    FDOSQLSERVER_OV_API static FdoSqlServerOvDataPropertyDefinitionCollection* Create(FdoPhysicalElementMapping* parent);

protected:
    FdoSqlServerOvDataPropertyDefinitionCollection();
    FdoSqlServerOvDataPropertyDefinitionCollection(FdoPhysicalElementMapping* parent);
    virtual ~FdoSqlServerOvDataPropertyDefinitionCollection();
    virtual void Dispose();
};

#endif


