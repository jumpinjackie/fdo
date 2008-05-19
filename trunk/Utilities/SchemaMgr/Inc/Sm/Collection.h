#ifndef FDOSMCOLLECTION_H
#define FDOSMCOLLECTION_H		1
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

template <class OBJ> class FdoSmCollection : private FdoCollection<OBJ, FdoException>
{
public:
    /// Get constant pointer to item by index
    /// Note, this pointer must not be held onto longer than necessary since it can become
    /// invalid if this collection is destroyed.
	const OBJ* RefItem(FdoInt32 index) const
	{
		OBJ* pItem = ((FdoCollection<OBJ,FdoException>*) this)->GetItem(index);
        const OBJ* pRetItem = pItem;

    /// Const pointer is volatile so release now. Caller can't release
		FDO_SAFE_RELEASE(pItem);

		return pRetItem;
	}

	OBJ* GetItem(FdoInt32 index)
	{
		return FdoCollection<OBJ,FdoException>::GetItem(index);
	}

    virtual FdoInt32 GetCount() const
    {
        return FdoCollection<OBJ,FdoException>::GetCount();
    }

    virtual void SetItem(FdoInt32 index, OBJ* value)
	{
		FdoCollection<OBJ,FdoException>::SetItem(index, value);
	}

    /// adds an item to the list
    virtual FdoInt32 Add( OBJ* value)
	{
		return FdoCollection<OBJ,FdoException>::Add(value);
	}

    virtual void Insert( FdoInt32 item, OBJ* value)
	{
		FdoCollection<OBJ,FdoException>::Insert(item, value);
	}

	virtual void Clear()
    {
		FdoCollection<OBJ,FdoException>::Clear();
	}

    virtual void Remove(const OBJ* value)
    {
		FdoCollection<OBJ,FdoException>::Remove(value);
	}

    virtual void RemoveAt(FdoInt32 index)
    {
		FdoCollection<OBJ,FdoException>::RemoveAt(index);
	}

    virtual bool Contains(const OBJ* value) const
    {
		return FdoCollection<OBJ,FdoException>::Contains(value);
	}

    const FdoSmSchemaElement* GetParent() const
    {
        return mpParent;
    }

    FdoInt32 AddRef() 
    { 
        return FdoCollection<OBJ,FdoException>::AddRef();
    }

    FdoInt32 Release() 
    { 
        return FdoCollection<OBJ,FdoException>::Release();
    }

protected:
    FdoSmCollection( const FdoSmSchemaElement* pParent = NULL ) :
        FdoCollection<OBJ,FdoException>(),
        mpParent(pParent)
    {
    }

    virtual ~FdoSmCollection(void)
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


