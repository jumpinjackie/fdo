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
#include "ArcSDEUtils.h"


ArcSDESchemaManager::ArcSDESchemaManager (ArcSDEConnection* connection) :
    mConnection (connection)
{
    FDO_SAFE_ADDREF (mConnection.p);
}

ArcSDESchemaManager::~ArcSDESchemaManager (void)
{
}

/** Do not implement the copy constructor. **/
//ArcSDESchemaManager::ArcSDESchemaManager (const ArcSDESchemaManager &right) { }

void ArcSDESchemaManager::Dispose ()
{
    delete this;
}


FdoClassDefinition* ArcSDESchemaManager::CloneAndPruneClass(FdoClassDefinition *fdoClassDef, FdoIdentifierCollection *idCollection)
{
    FdoPtr<FdoCommonSchemaCopyContext> copyContext;
    if (idCollection != NULL && idCollection->GetCount() > 0)
        copyContext = FdoCommonSchemaCopyContext::Create(idCollection);
    
    return FdoCommonSchemaUtil::DeepCopyFdoClassDefinition(fdoClassDef, copyContext);
}
