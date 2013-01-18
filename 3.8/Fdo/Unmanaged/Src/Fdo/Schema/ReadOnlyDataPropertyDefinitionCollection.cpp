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
#include <Fdo/Schema/ReadOnlyDataPropertyDefinitionCollection.h>
#include <Fdo/Schema/SchemaException.h>

FdoReadOnlyDataPropertyDefinitionCollection::FdoReadOnlyDataPropertyDefinitionCollection(FdoDataPropertyDefinitionCollection* properties)
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

FdoReadOnlyDataPropertyDefinitionCollection* FdoReadOnlyDataPropertyDefinitionCollection::Create(FdoDataPropertyDefinitionCollection* collection)
{
    return new FdoReadOnlyDataPropertyDefinitionCollection(collection);
}

void FdoReadOnlyDataPropertyDefinitionCollection::Dispose()
{
    delete this;
}

FdoInt32 FdoReadOnlyDataPropertyDefinitionCollection::GetCount()
{
    return FdoReadOnlyCollection<FdoDataPropertyDefinition, FdoIDisposableCollection, FdoSchemaException>::GetCount();
}

FdoDataPropertyDefinition* FdoReadOnlyDataPropertyDefinitionCollection::GetItem(FdoInt32 index)
{
    return FdoReadOnlyCollection<FdoDataPropertyDefinition, FdoIDisposableCollection, FdoSchemaException>::GetItem(index);
}

FdoDataPropertyDefinition* FdoReadOnlyDataPropertyDefinitionCollection::GetItem(FdoString* name)
{
    return FdoReadOnlyCollection<FdoDataPropertyDefinition, FdoIDisposableCollection, FdoSchemaException>::GetItem(name);
}

bool FdoReadOnlyDataPropertyDefinitionCollection::Contains(FdoDataPropertyDefinition* value)
{
    return FdoReadOnlyCollection<FdoDataPropertyDefinition, FdoIDisposableCollection, FdoSchemaException>::Contains(value);
}

FdoInt32 FdoReadOnlyDataPropertyDefinitionCollection::IndexOf(FdoDataPropertyDefinition* value)
{
    return FdoReadOnlyCollection<FdoDataPropertyDefinition, FdoIDisposableCollection, FdoSchemaException>::IndexOf(value);
}


