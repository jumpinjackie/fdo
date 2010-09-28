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
#ifndef FDOPOSTGIS_SELECTAGGREGATESCOMMAND_H_INCLUDED
#define FDOPOSTGIS_SELECTAGGREGATESCOMMAND_H_INCLUDED

#include "FeatureCommand.h"
#include <Fdo/Commands/Feature/ISelectAggregates.h>

namespace fdo { namespace postgis {

// Forward declarations of internal types
class Connection;

/// Implementation of Select command that use of aggregate functions.
/// This command differs from the FdoISelect command in a number of ways.
/// It handles selection of properties that are aggregate functions and
/// supports selecting distinct property values.
/// It also includes grouping criteria.
/// The execute method returns an IDataReader which is not tied to a specific class.
/// Unlike Select, FdoISelectAggregate does not include any locking functions.
///
class SelectAggregatesCommand : public FeatureCommand<FdoISelectAggregates>
{
public:

    /// Type of FDO smart pointer for the class.
    typedef FdoPtr<SelectAggregatesCommand> Ptr;

    /// Constructor creates Select command for given connection.
    SelectAggregatesCommand(Connection* conn);

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
    // FdoISelectAggregates interface
    //

    /// Set distinct option for current selection.
    virtual void SetDistinct(bool value);

    /// Get distinct option of current selection.
    virtual bool GetDistinct();

    /// Get collection of property names used in GROUP BY clause.
    virtual FdoIdentifierCollection* GetGrouping();

    /// Set the grouping by filter.
    virtual void SetGroupingFilter(FdoFilter* filter);

    /// Get the grouping by filter. 
    virtual FdoFilter* GetGroupingFilter();

    /// Execute the select with aggregates command and construct instance of FdoIDataReader class.
    /// \return 
    /// Reference to reader instance of FdoIDataReader type.
    virtual FdoIDataReader* Execute();

protected:

    /// Destructor.
    virtual ~SelectAggregatesCommand();

    //
    // FdoIDisposable interface
    //

    virtual void Dispose();
    
private:

    //
    // Private data members
    //
    
    typedef FeatureCommand<FdoISelectAggregates> Base;

    // List of property names to select into result set.
     FdoPtr<FdoIdentifierCollection> mProperties;
        
    // List of property names of order by clause.
    FdoPtr<FdoIdentifierCollection> mOrderingProperties;
    
    // Type of ordering option.
    // FdoOrderingOption_Ascending is the default value.
    FdoOrderingOption mOrderingOption;

    FdoPtr<FdoIdentifierCollection> mGroupingProperties;
    FdoPtr<FdoFilter> mGroupingFilter;
    
    bool mDistinct;
    
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_SELECTAGGREGATESCOMMAND_H_INCLUDED
