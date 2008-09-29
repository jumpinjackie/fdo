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
#include <Fdo/Connections/Capabilities/ArgumentDefinitionCollection.h>

FdoArgumentDefinitionCollection* FdoArgumentDefinitionCollection::Create()
{
	return new FdoArgumentDefinitionCollection();
}

FdoArgumentDefinitionCollection* FdoArgumentDefinitionCollection::Create(FdoArgumentDefinition** arguments, FdoInt32 length)
{
    FdoArgumentDefinitionCollection* pColl = new FdoArgumentDefinitionCollection();

    try
    {
        for (FdoInt32 i = 0; i < length; i++)
            pColl->Add(arguments[i]);
    }
    catch (FdoException* pExcept)
    {
        FDO_SAFE_RELEASE(pColl);
        throw pExcept;
    }

    return pColl;
}

void FdoArgumentDefinitionCollection::Dispose()
{
    delete this;
}

