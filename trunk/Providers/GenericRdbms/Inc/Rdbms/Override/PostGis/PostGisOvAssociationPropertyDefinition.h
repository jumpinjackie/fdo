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
#ifndef FDOPOSTGISOVASSOCIATIONPROPERTYDEFINITION_H
#define FDOPOSTGISOVASSOCIATIONPROPERTYDEFINITION_H

#include <FdoCommon.h>
#include <Rdbms/Override/PostGis/PostGisOv.h>
#include <Rdbms/Override/PostGis/PostGisOvPropertyDefinitionCollection.h>
#include <Rdbms/Override/RdbmsOvAssociationPropertyDefinition.h>

/// \brief
/// Concrete class defining physical schema overrides for association
/// property definition.
///
class FdoPostGISOvAssociationPropertyDefinition :
    public FdoPostGISOvPropertyDefinition,
    public FdoRdbmsOvAssociationPropertyDefinition
{
public:

    /// \brief
    /// Named constructor.
    /// Creates instance with default state initialized.
    /// 
    /// \return
    /// Pointer to new instance of FdoPostGISOvAssociationPropertyDefinition.
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvAssociationPropertyDefinition* Create();

    /// \brief
    /// Named constructor.
    /// Creates new instance with given name.
    /// 
    /// \param
    /// name [in] - name for new association.
    /// 
    /// \return
    /// Pointer to new instance of FdoPostGISOvAssociationPropertyDefinition.
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvAssociationPropertyDefinition* Create(
        FdoString* name);

    /// \brief
    /// Gets a collection of property definitions
    /// 
    /// \return
    /// Pointer to collection of property definitions.
    /// 
    FDOPOSTGIS_OV_API FdoPostGISOvPropertyDefinitionCollection *GetIdentityProperties();

protected:

    /// Default constructor.
    FdoPostGISOvAssociationPropertyDefinition();

    /// Alternative constructor.
    /// \param name [in] - name of association definition.
    FdoPostGISOvAssociationPropertyDefinition(FdoString* name);

    /// Destructor.
    virtual ~FdoPostGISOvAssociationPropertyDefinition();

    /// Named destructor.
    virtual void Dispose();

private:

    /// Named constructor.
    /// Called from class constructors.
    void Init();

    /// Collection of properties definition.
    FdoPostGISOvPropertiesP mPropertyDefinitions;
};

#endif // FDOPOSTGISOVASSOCIATIONPROPERTYDEFINITION_H
