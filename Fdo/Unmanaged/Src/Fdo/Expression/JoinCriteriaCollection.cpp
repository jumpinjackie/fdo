// 
//  
//  Copyright (C) 2004-2011  Autodesk, Inc.
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
#include <Fdo/Expression/JoinCriteriaCollection.h>
#include "StringUtility.h"

FdoJoinCriteriaCollection* FdoJoinCriteriaCollection::Create()
{
	return new FdoJoinCriteriaCollection();
}

void FdoJoinCriteriaCollection::Dispose()
{
    delete this;
}

// Gets the item in the collection with the specified name. Throws an invalid 
// argument exception if an item with the specified name does not exist in 
// the collection.
FdoJoinCriteria* FdoJoinCriteriaCollection::GetItem(FdoString* name)
{
    FdoJoinCriteria*	pVal = FindItem( name );

    if( pVal == NULL )
        throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(COMMANDS_2_INVALIDPARAMETERVALUENAME), name));

    return pVal;
}

// Gets the item in the collection with the specified name. return NULL 
// if an item with the specified name does not exist in  the collection.
FdoJoinCriteria* FdoJoinCriteriaCollection::FindItem(FdoString* name)
{
	FdoInt32 i;
    FdoPtr<FdoJoinCriteria>	pVal;

    for (i = 0; i < GetCount(); i++)
    {
	    pVal = FdoCollection<FdoJoinCriteria, FdoCommandException>::GetItem(i);

	    if ( pVal->GetAlias() != NULL && FdoStringUtility::StringCompare(pVal->GetAlias(), name) == 0)
		    return FDO_SAFE_ADDREF(pVal.p);
    }
    return NULL;
}

FdoJoinCriteria* FdoJoinCriteriaCollection::GetItem(FdoInt32 index)
{
   return FdoCollection<FdoJoinCriteria, FdoCommandException>::GetItem(index);
}

void FdoJoinCriteriaCollection::Insert(FdoInt32 index, FdoJoinCriteria* value)
{
    value->EnsureValidCriteria();
    return FdoCollection<FdoJoinCriteria, FdoCommandException>::Insert(index, value);
}

FdoInt32 FdoJoinCriteriaCollection::Add(FdoJoinCriteria* value)
{
    value->EnsureValidCriteria();
    return FdoCollection<FdoJoinCriteria, FdoCommandException>::Add(value);
}

void FdoJoinCriteriaCollection::SetItem(FdoInt32 index, FdoJoinCriteria* value)
{
    value->EnsureValidCriteria();
    return FdoCollection<FdoJoinCriteria, FdoCommandException>::SetItem(index, value);
}
