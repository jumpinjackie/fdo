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
#include <stdlib.h>
#include <Fdo/Schema/ReadOnlyPropertyDefinitionCollection.h>
#include <Fdo/Schema/SchemaException.h>

FdoReadOnlyPropertyDefinitionCollection::FdoReadOnlyPropertyDefinitionCollection(FdoPropertyDefinitionCollection* properties)
{
    FdoIDisposable*   item = NULL;
    FdoIDisposableCollection*   collection = NULL;

    if (properties)
    {
        try
        {
            // Create a copy of the collection passed in
            collection = FdoIDisposableCollection::Create();
            for (int i=0; i < properties->GetCount(); i++)
            {
                item = properties->GetItem(i);
                collection->Add(item);
                FDO_SAFE_RELEASE(item);
            }

            SetBaseCollection(collection);
            FDO_SAFE_RELEASE(collection);
        }
        catch (FdoException* pExcept)
        {
            FDO_SAFE_RELEASE(item);
            FDO_SAFE_RELEASE(collection);
            throw pExcept;
        }
    }
    else
        SetBaseCollection(NULL);
}

FdoReadOnlyPropertyDefinitionCollection* FdoReadOnlyPropertyDefinitionCollection::Create(FdoPropertyDefinitionCollection* collection)
{
    return new FdoReadOnlyPropertyDefinitionCollection(collection);
}

void FdoReadOnlyPropertyDefinitionCollection::Dispose()
{
    delete this;
}

FdoInt32 FdoReadOnlyPropertyDefinitionCollection::GetCount()
{
    return FdoReadOnlyCollection<FdoPropertyDefinition, FdoIDisposableCollection, FdoSchemaException>::GetCount();
}

FdoPropertyDefinition* FdoReadOnlyPropertyDefinitionCollection::GetItem(FdoInt32 index)
{
    return FdoReadOnlyCollection<FdoPropertyDefinition, FdoIDisposableCollection, FdoSchemaException>::GetItem(index);
}

FdoPropertyDefinition* FdoReadOnlyPropertyDefinitionCollection::GetItem(FdoString* name)
{
    return FdoReadOnlyCollection<FdoPropertyDefinition, FdoIDisposableCollection, FdoSchemaException>::GetItem(name);
}

bool FdoReadOnlyPropertyDefinitionCollection::Contains(FdoPropertyDefinition* value)
{
    return FdoReadOnlyCollection<FdoPropertyDefinition, FdoIDisposableCollection, FdoSchemaException>::Contains(value);
}

FdoInt32 FdoReadOnlyPropertyDefinitionCollection::IndexOf(FdoPropertyDefinition* value)
{
    return FdoReadOnlyCollection<FdoPropertyDefinition, FdoIDisposableCollection, FdoSchemaException>::IndexOf(value);
}


