#ifndef FDOSMNAMEDCOLLECTION_H
#define FDOSMNAMEDCOLLECTION_H		1
//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifdef _WIN32
#pragma once
#endif

#include <Sm/SchemaElement.h>

template <class OBJ> class FdoSmNamedCollection : private FdoNamedCollection<OBJ, FdoException>
{
public:
    /// Get constant pointer to item by index
    /// Note, this pointer must not be held onto longer than necessary since it can become
    /// invalid if this collection is destroyed.
	const OBJ* RefItem(FdoInt32 index) const
	{
		OBJ* pItem = ((FdoNamedCollection<OBJ,FdoException>*) this)->GetItem(index);
        const OBJ* pRetItem = pItem;

    /// Const pointer is volatile so release now. Caller can't release
		FDO_SAFE_RELEASE(pItem);

		return pRetItem;
	}

    /// Get constant pointer to item by name
    /// Note, this pointer must not be held onto longer than necessary since it can become
    /// invalid if this collection is destroyed.
	const OBJ* RefItem(FdoString* name) const
	{
		OBJ* pItem = FdoNamedCollection<OBJ,FdoException>::FindItem(name);

		if ( pItem )
			pItem->Release();

		return pItem;
	}

	OBJ* GetItem(FdoInt32 index)
	{
		return FdoNamedCollection<OBJ,FdoException>::GetItem(index);
	}

    /// Get item by name. Item type must supply a GetName()
    /// function.
	OBJ* GetItem(const wchar_t* name)
	{
		return FdoNamedCollection<OBJ,FdoException>::GetItem(name);
	}

    /// Get item by name. Item type must supply a GetName()
    /// function.
	OBJ* FindItem(const wchar_t* name)
	{
		return FdoNamedCollection<OBJ,FdoException>::FindItem(name);
	}

    virtual FdoInt32 IndexOf(const OBJ* value) const
    {
        return(FdoNamedCollection<OBJ,FdoException>::IndexOf(value));
    }

    virtual FdoInt32 IndexOf(FdoString* name) const
    {
        return(FdoNamedCollection<OBJ,FdoException>::IndexOf(name));
    }

    virtual FdoInt32 GetCount() const
    {
        return FdoNamedCollection<OBJ,FdoException>::GetCount();
    }

    virtual void SetItem(FdoInt32 index, OBJ* value)
	{
		FdoNamedCollection<OBJ,FdoException>::SetItem(index, value);
	}

    /// adds an item to the list
    virtual FdoInt32 Add( OBJ* value)
	{
		return FdoNamedCollection<OBJ,FdoException>::Add(value);
	}

    virtual void Insert( FdoInt32 item, OBJ* value)
	{
		FdoNamedCollection<OBJ,FdoException>::Insert(item, value);
	}

	virtual void Clear()
    {
		FdoNamedCollection<OBJ,FdoException>::Clear();
	}

    virtual void Remove(const OBJ* value)
    {
		FdoNamedCollection<OBJ,FdoException>::Remove(value);
	}

    virtual void RemoveAt(FdoInt32 index)
    {
		FdoNamedCollection<OBJ,FdoException>::RemoveAt(index);
	}

    virtual bool Contains(const OBJ* value) const
    {
		return FdoNamedCollection<OBJ,FdoException>::Contains(value);
	}

    virtual FdoStringP ToString( FdoString* separator = L" " ) const
    {
        FdoStringsP names = FdoStringCollection::Create();
        const OBJ* item;
        FdoInt32 idx;

        for ( idx = 0; idx < GetCount(); idx++ ) {
            item = RefItem( idx );
            names->Add( item->GetName() );
        }

        return names->ToString( separator);
    }

    const FdoSmSchemaElement* GetParent() const
    {
        return mpParent;
    }

    FdoInt32 AddRef() 
    { 
        return FdoNamedCollection<OBJ,FdoException>::AddRef();
    }

    FdoInt32 Release() 
    { 
        return FdoNamedCollection<OBJ,FdoException>::Release();
    }

protected:
    FdoSmNamedCollection( const FdoSmSchemaElement* pParent = NULL, bool caseSensitive = true ) :
        FdoNamedCollection<OBJ,FdoException>( caseSensitive ),
        mpParent(pParent)
    {
    }

    virtual ~FdoSmNamedCollection(void)
    {
    }

    void Dispose()
    {
	    delete this;
    }

private:
    const FdoSmSchemaElement* mpParent;
};

#endif


