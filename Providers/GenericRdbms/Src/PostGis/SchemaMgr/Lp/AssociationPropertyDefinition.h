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
#ifndef FDOSMLPPOSTGISASSOCIATIONPROPERTYDEFINITION_H
#define FDOSMLPPOSTGISASSOCIATIONPROPERTYDEFINITION_H

#include <Sm/Lp/AssociationPropertyDefinition.h>

/// Defines association property for LogicalPhysical.
/// Used by PostGIS provider.
///
class FdoSmLpPostGisAssociationPropertyDefinition :
    public FdoSmLpAssociationPropertyDefinition
{
public:

    /// Construct an AssociationPropertyDefinition from property reader.
    ///
    /// \param
    /// propReader [in] - property reader.
    ///
    /// \param
    /// parent [in] - 
    ///
    FdoSmLpPostGisAssociationPropertyDefinition(
        FdoSmPhClassPropertyReaderP propReader,
        FdoSmLpClassDefinition* parent);

    /// Construct an AssociationPropertyDefinition from
    /// FDO property definition.
    ///
    /// \param
    /// fdoPropDef [in] - FDO property definition.
    ///
    /// \param
    /// ignoreStates [in] - 
    ///
    /// \param
    /// parent [in] - pointer to instance of parent class definition.
    ///
    FdoSmLpPostGisAssociationPropertyDefinition(
        FdoAssociationPropertyDefinition* fdoPropDef,
        bool ignoreStates,
        FdoSmLpClassDefinition* parent);

    /// Constructs an instance of a AssociationPropertyDefinition from
    /// the given parameters.
    /// Used mainly by CreateInherited() and CreateCopy().
    ///
    /// \param
    /// baseProperty [in] - base property used to initialize new property.
    ///
    /// \param
    /// targetClass [in] - class that will contain the new property.
    ///
    /// \param
    /// logicalName [in] - name for the new property. If empty L"",
    /// then the new property has the same name as this property.
    ///
    /// \param
    /// physicalName [in] - name of the new property's column. If empty L"",
    /// then the column is the same as the base property column name.
    ///
    /// \param
    /// inherit [in] - if true, new property inherits from the base property,
    /// fales - new property is just a copy of the base property.
    ///
    /// \param
    /// propOverrides [in] - FDO to physical mapping overrides for property.
    ///
    FdoSmLpPostGisAssociationPropertyDefinition(
        FdoPtr<FdoSmLpAssociationPropertyDefinition> baseProperty,
        FdoSmLpClassDefinition* targetClass,
        FdoStringP logicalName,
        FdoStringP physicalName,
        bool inherit,
        FdoPhysicalPropertyMapping* propOverrides = NULL);

protected:

    /// Destructor.
    virtual ~FdoSmLpPostGisAssociationPropertyDefinition();

    /// Create an inherited copy of this property.
    ///
    /// \param
    /// subClass [in] -
    ///
    virtual FdoSmLpPropertyP NewInherited(
        FdoSmLpClassDefinition* subClass) const;

    /// Create a non-inherited copy of this property.
    /// \param
    /// targetClass [in] - class that will contain the new property.
    ///
    /// \param
    /// logicalName [in] - name for the new property. If empty L"",
    /// then the new property has the same name as this property.
    ///
    /// \param
    /// physicalName [in] - name of the new property's column. If empty L"",
    /// then the column is the same as the base property column name.
    ///
    /// \param
    /// propOverrides [in] - FDO to physical mapping overrides for property.
    ///
    virtual FdoSmLpPropertyP NewCopy(FdoSmLpClassDefinition* targetClass,
        FdoStringP logicalName,
        FdoStringP physicalName,
        FdoPhysicalPropertyMapping* propOverrides) const;

};

#endif // FDOSMLPPOSTGISASSOCIATIONPROPERTYDEFINITION_H