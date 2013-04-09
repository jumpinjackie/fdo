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
#ifndef FDOPOSTGISOVPROPERTYDEFINITIONCOLLECTION_H
#define FDOPOSTGISOVPROPERTYDEFINITIONCOLLECTION_H

#include <Rdbms/Override/PostGis/PostGisOv.h>
#include <Rdbms/Override/PostGis/PostGisOvPropertyDefinition.h>
#include <Rdbms/Override/RdbmsOvPropertyDefinitionCollection.h>
#include <Fdo/Commands/Schema/PhysicalElementMappingCollection.h>

/// \brief
/// Local alias of base type for FdoPostGISOvPropertyDefinitionCollection.
///
typedef FdoRestrictedNamedCollection
    <
    FdoRdbmsOvPropertyDefinition,
    FdoPostGISOvPropertyDefinition,
    FdoRdbmsOvPropertyDefinitionCollection
    >
    FdoPostGISOvPropertyDefinitionCollectionBase;

/// \brief
/// Concrete class defining physical schema overrides for collection
/// of property definitions.
/// This class is used in PostGIS schema overrides.
/// 
class FdoPostGISOvPropertyDefinitionCollection : 
    public FdoPostGISOvPropertyDefinitionCollectionBase
{
public:

    /// \brief
    /// Constructs a default of an FdoPostGISOvPropertyDefinitionCollection
    /// 
    /// \return
    /// Returns FdoPostGISOvPropertyDefinitionCollection
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvPropertyDefinitionCollection* Create();

    /// \brief
    /// Named constructor.
    /// Creates new instance based on given collection.
    /// 
    /// \param
    /// base [in] - pointer to base collection.
    /// 
    /// \return
    /// Pointer to new FdoPostGISOvPropertyDefinitionCollection instance.
    /// 

    FDOPOSTGIS_OV_API static FdoPostGISOvPropertyDefinitionCollection* Create(
        FdoRdbmsOvPropertyDefinitionCollection* base);

protected:

    /// Default constructor.
    FdoPostGISOvPropertyDefinitionCollection();
    
    /// Alternative constructor.
    /// base [in] - pointer to base collection.
    FdoPostGISOvPropertyDefinitionCollection(
        FdoRdbmsOvPropertyDefinitionCollection* base);
    
    /// Destructor.
    virtual ~FdoPostGISOvPropertyDefinitionCollection();

    /// Named destructor.
    /// Responsible for final destruction calling
    /// \code delete this \endcode.
    virtual void Dispose();

    virtual FdoPostGISOvPropertyDefinition* DownCast(
        FdoRdbmsOvPropertyDefinition* value) const;

};

/// \brief
/// Declaration of smart-pointer type for 
/// FdoPostGISOvPropertyDefinitionCollection.
/// Provided for convenience.
///
typedef FdoPtr<FdoPostGISOvPropertyDefinitionCollection> 
    FdoPostGISOvPropertiesP;

#endif // FDOPOSTGISOVPROPERTYDEFINITIONCOLLECTION_H
