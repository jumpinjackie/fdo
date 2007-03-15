//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#ifndef FDOPOSTGIS_SELECTCOMMAND_H_INCLUDED
#define FDOPOSTGIS_SELECTCOMMAND_H_INCLUDED

#include "FeatureCommand.h"
#include <Fdo/Commands/Feature/ISelect.h>

namespace fdo { namespace postgis {

// Forward declarations of internal types
class Connection;

/// Implementation of Select command interface for PostGIS provider.
/// The Select command queries for features of a given class that
/// match the specified criteria.
///
class SelectCommand : public FeatureCommand<FdoISelect>
{
public:

    /// Constructor creates Select command for given connection.
    SelectCommand(Connection* conn);

    //
    // FdoIBaseSelect interface
    //

    /// Get interface to collection of list of property names to return with the result.
    virtual FdoIdentifierCollection* GetPropertyNames();
 
 /// Get FdoIdentifierCollection that holds the list of order by property names.
    virtual FdoIdentifierCollection* GetOrdering();

    /// Set ordering option of the selection.
    virtual void SetOrderingOption(FdoOrderingOption option);
 
    /// Get ordering option. 
    virtual FdoOrderingOption GetOrderingOption();

    //
    // FdoISelect interface
    //

    /// Get type of command locking (see "Locking Commands").
    virtual FdoLockType GetLockType();
    
    /// Set type of command locking (see "Locking Commands").
    virtual void SetLockType(FdoLockType type);
    
    /// Get command locking strategy enumerator (see "Locking Commands").
    virtual FdoLockStrategy GetLockStrategy();
    
    /// Set command locking strategy enumerator (see "Locking Commands").
    virtual void SetLockStrategy(FdoLockStrategy strategy);
    
    /// Execute select command and return pointer to FdoIFeatureReader interface.
    virtual FdoIFeatureReader* Execute();
    
    /// Executes select command and return pointer to FdoIFeatureReader interface.
    virtual FdoIFeatureReader* ExecuteWithLock();
    
    /// When executing the operation ExecuteWithLock lock conflicts may occur.
    virtual FdoILockConflictReader* GetLockConflicts();

protected:

    /// Destructor.
    virtual ~SelectCommand();

    //
    // FdoIDisposable interface
    //

    virtual void Dispose();
    
private:

    //
    // Private data members
    //
    
    typedef FeatureCommand<FdoISelect> Base;

    // List of property names to return with the result.
    FdoPtr<FdoIdentifierCollection> mProperties;
        
    // List of property names of order by clause.
    FdoPtr<FdoIdentifierCollection> mOrderingProperties;
    
    // Type of ordering option.
    // FdoOrderingOption_Ascending is the default value.
    FdoOrderingOption mOrderingOption;
    
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_SELECTCOMMAND_H_INCLUDED
