/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
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


#include "stdafx.h"
#include "AssociationPropertyDefinition.h"

FdoSmLpMySqlAssociationPropertyDefinition::FdoSmLpMySqlAssociationPropertyDefinition(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent):
    FdoSmLpAssociationPropertyDefinition(propReader, parent)
{
}

FdoSmLpMySqlAssociationPropertyDefinition::FdoSmLpMySqlAssociationPropertyDefinition(
    FdoAssociationPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
) :
    FdoSmLpAssociationPropertyDefinition(pFdoProp, bIgnoreStates, parent)
{
}

FdoSmLpMySqlAssociationPropertyDefinition::FdoSmLpMySqlAssociationPropertyDefinition(
    FdoSmLpAssociationPropertyP pBaseProperty,
    FdoSmLpClassDefinition* pTargetClass,
    FdoStringP logicalName,
    FdoStringP physicalName,
    bool bInherit,
    FdoPhysicalPropertyMapping* pPropOverrides
):
    FdoSmLpAssociationPropertyDefinition(
        pBaseProperty,
        pTargetClass,
        logicalName,
        physicalName,
        bInherit,
        pPropOverrides
    )
{
}

FdoSmLpMySqlAssociationPropertyDefinition::~FdoSmLpMySqlAssociationPropertyDefinition(void)
{
}

FdoSmLpPropertyP FdoSmLpMySqlAssociationPropertyDefinition::NewInherited( FdoSmLpClassDefinition* pSubClass ) const
{
    return new FdoSmLpMySqlAssociationPropertyDefinition( FDO_SAFE_ADDREF((FdoSmLpAssociationPropertyDefinition*) this), pSubClass, L"", L"", true );
}

FdoSmLpPropertyP FdoSmLpMySqlAssociationPropertyDefinition::NewCopy(
    FdoSmLpClassDefinition* pTargetClass,
    FdoStringP logicalName,
    FdoStringP physicalName,
    FdoPhysicalPropertyMapping* pPropOverrides
) const
{
    return
        new FdoSmLpMySqlAssociationPropertyDefinition(
            FDO_SAFE_ADDREF((FdoSmLpAssociationPropertyDefinition*) this),
            pTargetClass,
            logicalName,
            physicalName,
            false,
            pPropOverrides
        );
}
