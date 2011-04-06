#ifndef _IDENTIFIERCOLLECTION_H_
#define _IDENTIFIERCOLLECTION_H_

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
//

#ifdef _WIN32
#pragma once
#endif

#include <FdoStd.h>
#include <Fdo/Expression/Identifier.h>
#include <Fdo/Commands/CommandException.h>

class FdoIdentifierCollectionMap;

/// \brief
/// The FdoIdentifierCollection class represents a collection of FdoIdentifier objects.
class FdoIdentifierCollection : public FdoCollection<FdoIdentifier, FdoCommandException>
{
/// \cond DOXYGEN-IGNORE
protected:
    virtual void Dispose();
/// \endcond

public:
    /// \brief
    /// Constructs a default empty instance of an FdoIdentifierCollection.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API static FdoIdentifierCollection* Create();

    /// \brief
    /// Gets the item in the collection at the specified index. 
    /// Throws an invalid argument exception if the index is out of range.
    /// 
    /// \param index 
    /// Input index
    /// 
    /// \return
    /// Returns the item in the collection at the specified index
    /// 
    FDO_API FdoIdentifier* GetItem(FdoInt32 index);

    /// \brief
    /// Gets the item in the collection with the specified name. 
    /// Throws an exception if the item is not found.
    /// 
    /// \param name 
    /// Input item name
    /// 
    /// \return
    /// Returns the item in the collection with the specified name
    /// 
    FDO_API FdoIdentifier* GetItem(FdoString* name);

    /// \brief
    /// Finds the item in the collection with the specified name.
    /// 
    /// \param name 
    /// Input item name
    /// 
    /// \return
    /// Returns the item in the collection with the specified name.
    /// Returns NULL if the item was not found.
    /// 
    FDO_API FdoIdentifier* FindItem(FdoString* name);

    /// \brief
    /// Sets the item in the collection at the specified index to the specified value. 
    /// Throws an invalid argument exception if the index is out of range.
    /// 
    /// \param index 
    /// Input index
    /// \param value 
    /// Input value
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API virtual void SetItem(FdoInt32 index, FdoIdentifier* value);

    /// \brief
    /// Adds the specified item to the end of the collection. 
    /// Returns the index of the newly added item.
    /// 
    /// \param value 
    /// Input value
    /// 
    /// \return
    /// Returns the index of the newly added item
    /// 
    FDO_API virtual FdoInt32 Add( FdoIdentifier* value);

    /// \brief
    /// Inserts the specified item at the specified index within the collection. 
    /// Items following the insertion point are moved down to accommodate the new item. 
    /// Throws an invalid argument exception if the specified index is out of range.
    /// 
    /// \param item 
    /// Input index
    /// \param value 
    /// Input value
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API virtual void Insert( FdoInt32 item, FdoIdentifier* value);

    /// \brief
    /// Removes all items from the collection.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API virtual void Clear();

    /// \brief
    /// Removes the specified item from the collection. 
    /// Throws an invalid argument exception if the item does not exist within the collection.
    /// 
    /// \param value 
    /// Input value
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API virtual void Remove(const FdoIdentifier* value);

    /// \brief
    /// Removes the specified item from the collection. 
    /// Throws an invalid argument exception if the item does not exist within the collection.
    /// 
    /// \param index 
    /// Input index
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API virtual void RemoveAt(FdoInt32 index);

    /// \brief
    /// Returns true if the collection contains the specified item, false otherwise.
    /// 
    /// \param value 
    /// Input value
    /// 
    /// \return
    /// Returns true if the collection contains the specified item, false otherwise
    /// 
    FDO_API virtual bool Contains(const FdoIdentifier* value);
   
    /// \brief
    /// Returns true if the collection contains the specified item, false otherwise.
    /// 
    /// \param name 
    /// Input the item name
    /// 
    /// \return
    /// Returns true if the collection contains the specified item, false otherwise
    /// 
    FDO_API virtual bool Contains(FdoString* name);

    /// \brief
    /// Returns the index of the specified item in the collection or -1 if the item does not exist.
    /// 
    /// \param value 
    /// Input value
    /// 
    /// \return
    /// Returns the index of the specified item in the collection or -1 if the item does not exist
    /// 
    FDO_API virtual FdoInt32 IndexOf(const FdoIdentifier* value);
   
    /// \brief
    /// Returns the index of the specified item (by name) in the collection or -1 if the item does not exist.
    /// 
    /// \param name 
    /// Input the item name
    /// 
    /// \return
    /// Returns the index of the specified item in the collection or -1 if the item does not exist
    /// 
    FDO_API virtual FdoInt32 IndexOf(FdoString* name);

protected:
    FDO_API FdoIdentifierCollection( bool caseSensitive = true );
    FDO_API virtual ~FdoIdentifierCollection(void);

/// \cond DOXYGEN-IGNORE
    FDO_API int Compare( FdoString* str1, FdoString* str2 ) const;
    FDO_API void CheckDuplicate( FdoIdentifier* item, FdoInt32 index );
/// \endcond

private:
    void InitMap();

    void InsertMap( FdoIdentifier* value ) const;

    // Remove the element at the specified index, from the map
    void RemoveMapAt( FdoInt32 index );

    // Remove the given element from the map.
    void RemoveMap( const FdoIdentifier* value );

    // Get the named element from the map. Returns NULL if element not in map.
    FdoIdentifier* GetMap( FdoString* name ) const;

private:
    bool mbCaseSensitive;
    FdoIdentifierCollectionMap* mpMap;
};

#endif

