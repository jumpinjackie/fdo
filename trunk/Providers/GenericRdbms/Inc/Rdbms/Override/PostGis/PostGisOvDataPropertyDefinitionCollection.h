//
// Copyright (C) 2006 Refractions Research, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#ifndef FDOPOSTGISOVDATAPROPERTYDEFINITIONCOLLECTION_H
#define FDOPOSTGISOVDATAPROPERTYDEFINITIONCOLLECTION_H

#include <Fdo/Schema/SchemaException.h>
#include <Fdo/Commands/Schema/PhysicalElementMappingCollection.h>
#include <Rdbms/Override/PostGis/PostGisOv.h>
#include <Rdbms/Override/PostGis/PostGisOvDataPropertyDefinition.h>

/// \brief
/// Local alias of base type for FdoPostGISOvDataPropertyDefinitionCollection.
///
typedef FdoPhysicalElementMappingCollection
    <
    FdoPostGISOvDataPropertyDefinition
    >
    FdoPostGISOvDataPropertyDefinitionCollectionBase;


/// \brief
/// Concrete class defining physical schema overrides for collection of
/// data properties.
///
class FdoPostGISOvDataPropertyDefinitionCollection :
    public FdoPostGISOvDataPropertyDefinitionCollectionBase
{
public:

    /// \brief
    /// Named constructor.
    /// Creates instance with default state initialized.
    /// 
    /// \return
    /// Pointer to new FdoPostGISOvDataPropertyDefinitionCollection instance.
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvDataPropertyDefinitionCollection* Create();

    /// \brief
    /// Named constructor.
    /// Creates new instance based on given parent mapping.
    /// 
    /// \param
    /// parent [in] - pointer to parent from physical mapping definition.
    /// 
    /// \return
    /// Pointer to new FdoPostGISOvDataPropertyDefinitionCollection instance.
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvDataPropertyDefinitionCollection* Create(
        FdoPhysicalElementMapping* parent);

protected:

    /// Default constructor.
    FdoPostGISOvDataPropertyDefinitionCollection();

    /// Alternative constructor.
    /// parent [in] - pointer to parent from physical mapping definition.
    FdoPostGISOvDataPropertyDefinitionCollection(FdoPhysicalElementMapping* parent);

    /// Destructor.
    virtual ~FdoPostGISOvDataPropertyDefinitionCollection();

    /// Named destructor.
    virtual void Dispose();
};

#endif // FDOPOSTGISOVDATAPROPERTYDEFINITIONCOLLECTION_H
