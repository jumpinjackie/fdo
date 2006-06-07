#ifndef _IDENTIFIERCOLLECTION_H_
#define _IDENTIFIERCOLLECTION_H_
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
#include <Fdo/Expression/Identifier.h>
#include <Fdo/Commands/CommandException.h>

/// \brief
/// The FdoIdentifierCollection class represents a collection of FdoIdentifier objects.
class FdoIdentifierCollection : public FdoCollection<FdoIdentifier, FdoCommandException>
{
protected:
/// \cond DOXYGEN-IGNORE
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
    /// Gets the item in the collection with the specified name. Throws an invalid 
    /// argument exception if an item with the specified name does not exist in 
    /// the collection.
    /// 
    /// \param name 
    /// Input the item name
    /// 
    /// \return
    /// Returns FdoIdentifier
    /// 
    FDO_API FdoIdentifier* GetItem(FdoString* name);

    /// \brief
    /// Finds the item in the collection with the specified name. Returns NULL 
    /// if an item with the specified name does not exist in the collection.
    /// 
    /// \param name 
    /// Input the item name
    /// 
    /// \return
    /// Returns FdoIdentifier
    /// 
    FDO_API FdoIdentifier* FindItem(FdoString* name);

    /// \brief
    /// Gets the item in the collection at the specified index. Throws an
    /// invalid argument exception if the index is out of range.
    /// 
    /// \param index 
    /// Input index of item
    /// 
    /// \return
    /// Returns FdoIdentifier
    /// 
    FDO_API FdoIdentifier* GetItem(FdoInt32 index);
};
#endif


