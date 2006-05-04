// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include <Fdo/Commands/IdentifierCollection.h>
#include "StringUtility.h"

FdoIdentifierCollection* FdoIdentifierCollection::Create()
{
	return new FdoIdentifierCollection();
}

void FdoIdentifierCollection::Dispose()
{
    delete this;
}

FdoIdentifier* FdoIdentifierCollection::GetItem(FdoString* name)
{
	FdoIdentifier*	pIdent = FindItem( name );
	
    if( pIdent == NULL )
        throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(COMMANDS_1_INVALIDIDENTIFIERNAME), name));

    return pIdent;
}

FdoIdentifier* FdoIdentifierCollection::FindItem(FdoString* name)
{
	FdoIdentifier*	pIdent = NULL;
	FdoInt32		i;

    try
    {
	    for (i = 0; i < GetCount(); i++)
	    {
		    pIdent = FdoCollection<FdoIdentifier, FdoCommandException>::GetItem(i);
            if (pIdent->GetText() && FdoStringUtility::StringCompare(pIdent->GetText(), name) == 0)
			    return pIdent;
            FDO_SAFE_RELEASE(pIdent);
	    }
    }
    catch (FdoException* pExcept)
    {
        FDO_SAFE_RELEASE(pIdent);
        throw pExcept;
    }

    return NULL;
}

FdoIdentifier* FdoIdentifierCollection::GetItem(FdoInt32 index)
{
   return FdoCollection<FdoIdentifier, FdoCommandException>::GetItem(index);
}

