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
#include <Fdo/Schema/PropertyDefinitionCollection.h>

FDO_API FdoPropertyDefinitionCollection* FdoPropertyDefinitionCollection::Create(FdoSchemaElement* parent)
{
    return new FdoPropertyDefinitionCollection(parent);
}

FDO_API void FdoPropertyDefinitionCollection::Remove(const FdoSchemaElement* value)
{
    FdoSchemaElement*   parent = ((FdoSchemaElement*)value)->GetParent();

    try
    {
        FdoSchemaCollection<FdoPropertyDefinition>::Remove((const FdoPropertyDefinition*)value);
        if (parent)
        {
            if (!(m_changeInfoState & CHANGEINFO_PROCESSING)) // don't notify during Accept/RejectChanges()
                parent->PropertyRemoved((const FdoPropertyDefinition*)value);
            FDO_SAFE_RELEASE(parent);
        }
    }
    catch (FdoException* pExcept)
    {
        FDO_SAFE_RELEASE(parent);
        throw pExcept;
    }
}

FDO_API void FdoPropertyDefinitionCollection::RemoveAt(FdoInt32 index)
{
    FdoSchemaElement*   item = NULL;
    FdoSchemaElement*   parent = NULL;

    try
    {
        item = GetItem(index);
        if (item)
            parent = item->GetParent();

        FdoSchemaCollection<FdoPropertyDefinition>::RemoveAt(index);

        if (parent)
        {
            if (!(m_changeInfoState & CHANGEINFO_PROCESSING)) // don't notify during Accept/RejectChanges()
                parent->PropertyRemoved((const FdoPropertyDefinition*)item);
            FDO_SAFE_RELEASE(parent);
        }
    }
    catch (FdoException* pExcept)
    {
        FDO_SAFE_RELEASE(parent);
        FDO_SAFE_RELEASE(item);
        throw pExcept;
    }

    FDO_SAFE_RELEASE(item);
}

