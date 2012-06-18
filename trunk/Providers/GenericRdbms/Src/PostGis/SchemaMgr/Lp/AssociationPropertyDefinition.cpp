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
#include "stdafx.h"
#include "AssociationPropertyDefinition.h"

FdoSmLpPostGisAssociationPropertyDefinition::FdoSmLpPostGisAssociationPropertyDefinition(
    FdoSmPhClassPropertyReaderP propReader,
    FdoSmLpClassDefinition* parent)
    : FdoSmLpAssociationPropertyDefinition(propReader, parent)
{
    // idle
}

FdoSmLpPostGisAssociationPropertyDefinition::FdoSmLpPostGisAssociationPropertyDefinition(
    FdoAssociationPropertyDefinition* fdoPropDef, 
    bool ignoreStates,
    FdoSmLpClassDefinition* parent)
    : FdoSmLpAssociationPropertyDefinition(fdoPropDef, ignoreStates, parent)
{
    // idle
}

FdoSmLpPostGisAssociationPropertyDefinition::FdoSmLpPostGisAssociationPropertyDefinition(
    FdoSmLpAssociationPropertyP baseProperty,
    FdoSmLpClassDefinition* targetClass,
    FdoStringP logicalName,
    FdoStringP physicalName,
    bool inherit,
    FdoPhysicalPropertyMapping* propOverrides)
    : FdoSmLpAssociationPropertyDefinition(
        baseProperty,
        targetClass,
        logicalName,
        physicalName,
        inherit,
        propOverrides)
{
    // idle
}

FdoSmLpPostGisAssociationPropertyDefinition::~FdoSmLpPostGisAssociationPropertyDefinition()
{
    // idle
}

FdoSmLpPropertyP FdoSmLpPostGisAssociationPropertyDefinition::NewInherited(
    FdoSmLpClassDefinition* subClass) const
{
    // Remove const qualifier
    FdoSmLpPostGisAssociationPropertyDefinition* thisProp = NULL;
    thisProp = const_cast<FdoSmLpPostGisAssociationPropertyDefinition*>(this);

    // Cast down the inheritance hierarchy
    FdoSmLpAssociationPropertyDefinition* propDef = NULL;
    propDef = static_cast<FdoSmLpAssociationPropertyDefinition*>(thisProp);
    
    return (new FdoSmLpPostGisAssociationPropertyDefinition(
            FDO_SAFE_ADDREF(propDef), subClass, L"", L"", true));
}

FdoSmLpPropertyP FdoSmLpPostGisAssociationPropertyDefinition::NewCopy(
    FdoSmLpClassDefinition* targetClass,
    FdoStringP logicalName,
    FdoStringP physicalName,
    FdoPhysicalPropertyMapping* propOverrides) const
{
    // Remove const qualifier
    FdoSmLpPostGisAssociationPropertyDefinition* thisProp = NULL;
    thisProp = const_cast<FdoSmLpPostGisAssociationPropertyDefinition*>(this);

    // Cast down the inheritance hierarchy
    FdoSmLpAssociationPropertyDefinition* propDef = NULL;
    propDef = static_cast<FdoSmLpAssociationPropertyDefinition*>(thisProp);

    return (new FdoSmLpPostGisAssociationPropertyDefinition(
            FDO_SAFE_ADDREF(propDef),
            targetClass, logicalName, physicalName,
            false, propOverrides));
}
