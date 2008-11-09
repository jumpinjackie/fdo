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
#include <Fdo/Commands/ParameterValueCollection.h>
#include "StringUtility.h"

FdoParameterValueCollection* FdoParameterValueCollection::Create()
{
	return new FdoParameterValueCollection();
}

void FdoParameterValueCollection::Dispose()
{
    delete this;
}

// Gets the item in the collection with the specified name. Throws an invalid 
// argument exception if an item with the specified name does not exist in 
// the collection.
FdoParameterValue* FdoParameterValueCollection::GetItem(FdoString* name)
{
    FdoParameterValue*	pVal = FindItem( name );

    if( pVal == NULL )
        throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(COMMANDS_2_INVALIDPARAMETERVALUENAME), name));

    return pVal;
}

// Gets the item in the collection with the specified name. return NULL 
// if an item with the specified name does not exist in  the collection.
FdoParameterValue* FdoParameterValueCollection::FindItem(FdoString* name)
{
	FdoInt32	i;
    FdoParameterValue*	pVal = NULL;

    try
    {
	    for (i = 0; i < GetCount(); i++)
	    {
		    pVal = FdoCollection<FdoParameterValue, FdoCommandException>::GetItem(i);

		    if ( pVal->GetName() != NULL && FdoStringUtility::StringCompare(pVal->GetName(), name) == 0)
			    return pVal;
            FDO_SAFE_RELEASE(pVal);
	    }
    }
    catch (FdoException* pExcept)
    {
        FDO_SAFE_RELEASE(pVal);
        throw pExcept;
    }

    return NULL;
}

FdoParameterValue* FdoParameterValueCollection::GetItem(FdoInt32 index)
{
   return FdoCollection<FdoParameterValue, FdoCommandException>::GetItem(index);
}

