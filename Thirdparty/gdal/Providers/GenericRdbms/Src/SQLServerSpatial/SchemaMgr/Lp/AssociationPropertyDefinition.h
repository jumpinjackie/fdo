#ifndef FDOSMLPSQSASSOCIATIONPROPERTYDEFINITION_H
#define FDOSMLPSQSASSOCIATIONPROPERTYDEFINITION_H       1
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


#include <Sm/Lp/AssociationPropertyDefinition.h>

// SqlServer Provider's LogicalPhysical AssociationPropertyDefinition
class FdoSmLpSqsAssociationPropertyDefinition : public FdoSmLpAssociationPropertyDefinition
{
public:
    // Construct an AssociationPropertyDefinition from a property reader.
    FdoSmLpSqsAssociationPropertyDefinition(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent);

    // Construct an AssociationPropertyDefinition from an FDO property definition.
    FdoSmLpSqsAssociationPropertyDefinition(
        FdoAssociationPropertyDefinition* pFdoProp, 
        bool bIgnoreStates,
        FdoSmLpClassDefinition* parent
    );

    // Constructs an instance of a AssociationPropertyDefinition from the given parameters.
    // Used mainly by CreateInherited() and CreateCopy().
    //
    // Parameters:
    //      pBaseProperty: initialize the new property from this base property.
    //      pTargetClass: the class that will contain the new property.
    //      logicalName: The name for the new property.
    //          If L"" then the new property has the same name as this
    //          property.
    //      physicalName: the name of the new property's column.
    //          If L"" then the column is the same as the base property column name.
    //      bInherit:
    //          True: the new property inherits from the base property.
    //          False: the new property is just a copy of the base property.
    //      pPropOverrides: Fdo to physical mapping overrides for created property.
    FdoSmLpSqsAssociationPropertyDefinition(
        FdoPtr<FdoSmLpAssociationPropertyDefinition> pBaseProperty,
        FdoSmLpClassDefinition* pTargetClass,
        FdoStringP logicalName,
        FdoStringP physicalName,
        bool bInherit,
        FdoPhysicalPropertyMapping* pPropOverrides = NULL
    );

protected:
    virtual ~FdoSmLpSqsAssociationPropertyDefinition(void);

    virtual FdoSmLpPropertyP NewInherited( FdoSmLpClassDefinition* pSubClass ) const;

    // Create a non-inherited copy of this property.
    //
    // Parameters:
    //      pTargetClass: The class that will contain the new property.
    //      logicalName: The name for the new property.
    //          If L"" then the new property has the same name as this
    //          property.
    //      physicalName: the name of the new property's column.
    //          If L"" then the column is the same as the property name.
    //      pPropOverrides: Fdo to physical mapping overrides for created property.
    virtual FdoSmLpPropertyP NewCopy(
        FdoSmLpClassDefinition* pTargetClass,
        FdoStringP logicalName,
        FdoStringP physicalName,
        FdoPhysicalPropertyMapping* pPropOverrides = NULL
    ) const;

};

#endif


