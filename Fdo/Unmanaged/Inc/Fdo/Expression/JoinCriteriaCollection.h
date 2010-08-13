#ifndef _JOINCRITERIACOLLECTION_H_
#define _JOINCRITERIACOLLECTION_H_
// 

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
#include <Fdo/Expression/JoinCriteria.h>
#include <Fdo/Commands/CommandException.h>

/// \brief
/// The FdoJoinCriteriaCollection class represents a collection of FdoJoinCriteria objects.
class FdoJoinCriteriaCollection : public FdoCollection<FdoJoinCriteria, FdoCommandException>
{
protected:
/// \cond DOXYGEN-IGNORE
    virtual void Dispose();
/// \endcond

public:
    /// \brief
    /// Constructs a default empty instance of an FdoJoinCriteriaCollection.
    /// 
    /// \return
    /// Returns FdoJoinCriteriaCollection
    /// 
    FDO_API static FdoJoinCriteriaCollection* Create();

    /// \brief
    /// Gets the item in the collection with the specified name. Throws an
    /// invalid argument exception if an item with the specified name does not
    /// exist in the collection.
    /// 
    /// \param name 
    /// Input join criteria alias to search
    /// 
    /// \return
    /// Returns FdoJoinCriteria
    /// 
    FDO_API FdoJoinCriteria* GetItem(FdoString* name);
        
    /// \brief
    /// Gets the item in the collection with the specified name. Returns
    /// NULL if an item with the specified name does not exist in the collection.
    /// 
    /// \param name 
    /// Input join criteria alias to search
    /// 
    /// \return
    /// Returns FdoJoinCriteria
    /// 
    FDO_API FdoJoinCriteria* FindItem(FdoString* name);

    /// \brief
    /// Gets the item in the collection at the specified index. Throws an
    /// invalid argument exception if the index is out of range.
    /// 
    /// \param index 
    /// Input index of join criteria
    /// 
    /// \return
    /// Returns FdoJoinCriteria
    /// 
    FDO_API FdoJoinCriteria* GetItem(FdoInt32 index);

    /// \brief
    /// Inserts the FdoJoinCriteria at the specified index within the collection. 
    /// Items following the insertion point are moved down to accommodate the new FdoJoinCriteria. 
    /// Throws an invalid argument exception if the specified index is out of range.
    /// 
    /// \param index 
    /// Input index to insert at
    /// \param value 
    /// Input value join criteria
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API void Insert(FdoInt32 index, FdoJoinCriteria* value);
    
    /// \brief
    /// Adds the FdoJoinCriteria to the end of the collection. Returns the index of the newly added FdoJoinCriteria.
    /// 
    /// \param value 
    /// Input value join criteria
    /// 
    /// \return
    /// Returns the index of the newly added FdoJoinCriteria
    /// 
    FDO_API FdoInt32 Add(FdoJoinCriteria* value);

    /// \brief
    /// Sets the item in the collection at the specified index to the FdoJoinCriteria. Throws an invalid argument exception if the index is out of range.
    /// 
    /// \param index 
    /// Input index to set at
    /// \param value 
    /// Input value join criteria
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API void SetItem(FdoInt32 index, FdoJoinCriteria* value);
};
#endif
