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
#ifndef FDOPOSTGISOVCLASSCOLLECTION_H
#define FDOPOSTGISOVCLASSCOLLECTION_H

#include <Rdbms/Override/PostGis/PostGisOv.h>
#include <Rdbms/Override/PostGis/PostGisOvClassDefinition.h>
#include <Rdbms/Override/RdbmsOvClassCollection.h>
#include <Fdo/Commands/Schema/PhysicalElementMappingCollection.h>

/// \brief
/// Local alias of base type for FdoPostGISOvClassCollection.
///
typedef FdoRestrictedNamedCollection
    <
    FdoRdbmsOvClassDefinition,
    FdoPostGISOvClassDefinition,
    FdoRdbmsOvClassCollection
    >
    FdoPostGISOvClassCollectionBase;

/// \brief
/// Concrete class defining physical schema overrides for
/// collection of classes.
///
class FdoPostGISOvClassCollection : public FdoPostGISOvClassCollectionBase
{
public:

    /// \brief
    /// Named constructor.
    /// Creates instance with default state initialized.
    /// 
    /// \return
    /// Pointer to new FdoPostGISOvClassCollection instance.
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvClassCollection* Create();

    /// \brief
    /// Named constructor.
    /// Creates new instance based on given collection.
    /// 
    /// \param
    /// base [in] - pointer to base collection.
    /// 
    /// \return
    /// Pointer to new FdoPostGISOvClassCollection instance.
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvClassCollection* Create(
        FdoRdbmsOvClassCollection* base);

protected:

    /// Default constructor.
    FdoPostGISOvClassCollection();

    /// Alternative constructor.
    /// base [in] - pointer to base collection.
    FdoPostGISOvClassCollection(FdoRdbmsOvClassCollection* base);

    /// Destructor.
    virtual ~FdoPostGISOvClassCollection();

    /// Named destructor.
    virtual void Dispose();

    virtual FdoPostGISOvClassDefinition* DownCast(
        FdoRdbmsOvClassDefinition* value) const;

private:

};

/// \brief
/// Declaration of smart-pointer type for FdoPostGISOvClassCollection.
/// Provided for convenience.
///
typedef FdoPtr<FdoPostGISOvClassCollection> FdoPostGISOvClassesP;

#endif // FDOPOSTGISOVCLASSCOLLECTION_H
