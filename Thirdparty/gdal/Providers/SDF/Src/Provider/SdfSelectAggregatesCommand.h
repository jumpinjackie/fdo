/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of version 2.1 of the GNU Lesser
* General Public License as published by the Free Software Foundation.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef SdfSelectAggregatesCommand_H
#define SdfSelectAggregatesCommand_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <FdoCommonFeatureCommand.h>


class SdfSelectAggregatesCommand :
    public FdoCommonFeatureCommand<FdoISelectAggregates, FdoIConnection>
{
protected:
    FdoPtr<FdoIdentifierCollection> mPropertiesToSelect;
    bool m_bDistinct;
    FdoOrderingOption m_eOrderingOption;
    FdoPtr<FdoIdentifierCollection> m_OrderingIds;
    FdoPtr<FdoFilter> m_GroupingFilter;
    FdoPtr<FdoIdentifierCollection> m_GroupingIds;

protected:
    virtual ~SdfSelectAggregatesCommand (void);

public:
    SdfSelectAggregatesCommand (FdoIConnection *connection);

    //
    /// Prevent the use of the Assignment Operation by defining it and not implementing it.
    /// DO NOT IMPLEMENT
    SdfSelectAggregatesCommand & operator= (const SdfSelectAggregatesCommand &right);

    /// \brief
    /// Gets the FdoIdentifierCollection that holds the list of property names to 
    /// return with the result. If empty all properties of the specified class
    /// are returned.
    /// 
    /// \return
    /// Returns the list of property names.
    /// 
    virtual FdoIdentifierCollection* GetPropertyNames ();

    /// \brief
    /// Executes the select command and returns a reference to an FdoIDataReader.
    /// 
    /// \return
    /// Returns the data reader.
    /// 
    virtual FdoIDataReader* Execute ();

    /// \brief
    /// Set the distinct option of the selection. Note that grouping criteria is not supported with Distinct. 
    /// Non-simple properties such as object properties, geometry properties, raster properties, association properties, etc. will not be supported with Distinct.
    /// 
    /// \param value 
    /// true or false; when set to true, only distinct values are returned. Otherwise all values are returned
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void SetDistinct( bool value );

    /// \brief
    /// Get the distinct option.
    /// 
    /// \return
    /// Returns true if distinct is set, false otherwise.
    /// 
    virtual bool GetDistinct( );

    /// \brief
    /// Gets the FdoIdentifierCollection that holds the list of group by property names. If empty no grouping is used. This list is initially
    /// empty and the caller need to add the property that the command should use as a group by criteria. No LOB or Geometry type properties
    /// can be used for ordering.
    /// 
    /// \return
    /// Returns the list of group by property names.
    /// 
    virtual FdoIdentifierCollection* GetGrouping();

    /// \brief
    /// Set the grouping by filter. Use the grouping filter to restrict the groups of returned properties to those groups for 
    /// which the specified filter is TRUE. For example "order by city" and  "min(lanes) = 2". The FdoFilter have to evalute to a 
    /// binary value(true or false).
    /// 
    /// \param filter 
    /// The grouping filter.
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void SetGroupingFilter( FdoFilter* filter );

    /// \brief
    /// Gets the grouping by filter.
    /// 
    /// \return
    /// Returns the grouping filter.
    /// 
    virtual FdoFilter* GetGroupingFilter( );

    /// \brief
    /// Gets the FdoIdentifierCollection that holds the list of order by property names. If empty no ordering is used. This list is initially
    /// empty and the caller need to add the property that the command should use as a order by criteria.
    /// 
    /// \return
    /// Returns the list of group by property names.
    /// 
    virtual FdoIdentifierCollection* GetOrdering();

    /// \brief
    /// Set the ordering option of the selection. This is only used if the ordering collection is not empty.
    /// 
    /// \param option 
    /// Is the ordering option and should be set to one of FdoOrderingOption_Ascending or FdoOrderingOption_Descending.
    /// FdoOrderingOption_Ascending is the default value.
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void SetOrderingOption( FdoOrderingOption  option );

    /// \brief
    /// Gets the ordering option.
    /// 
    /// \return
    /// Returns the ordering option.
    /// 
    virtual FdoOrderingOption GetOrderingOption( );
};

#endif // SdfSelectAggregatesCommand_H


