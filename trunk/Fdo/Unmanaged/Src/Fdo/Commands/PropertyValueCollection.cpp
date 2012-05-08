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
#include <Fdo/Commands/PropertyValueCollection.h>
#include <Fdo/Commands/CommandException.h>
#include "StringUtility.h"

FdoPropertyValueCollection* FdoPropertyValueCollection::Create()
{
	return new FdoPropertyValueCollection();
}

FdoPropertyValue* FdoPropertyValueCollection::GetItem(FdoString* name)
{
    FdoPropertyValue*	pVal = FindItem( name );
    
    if( pVal == NULL )
        throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(COMMANDS_3_INVALIDPROPERTYVALUENAME), name ));

    return pVal;
}

FdoPropertyValue* FdoPropertyValueCollection::FindItem(FdoString* name)
{
    FdoInt32	i;
    FdoPropertyValue*	pVal = NULL;
    FdoIdentifier*      pname = NULL;

    try
    {
	    for (i = 0; i < GetCount(); i++)
	    {
		    FdoPropertyValue*	pVal = FdoCollection<FdoPropertyValue, FdoCommandException>::GetItem(i);

		    if ( pVal != NULL)
            {
                pname = pVal->GetName();
                bool            bFound = (pname->GetText() && FdoStringUtility::StringCompare(pname->GetText(), name) == 0);
                FDO_SAFE_RELEASE(pname);
                if (bFound)
			        return pVal;
            }
            FDO_SAFE_RELEASE(pVal);
	    }
    }
    catch (FdoException* pExcept)
    {
        FDO_SAFE_RELEASE(pname);
        FDO_SAFE_RELEASE(pVal);
        throw pExcept;
    }

    return NULL;
}

FdoPropertyValue* FdoPropertyValueCollection::GetItem(FdoInt32 index)
{
   return	FdoCollection<FdoPropertyValue, FdoCommandException>::GetItem(index);
}

