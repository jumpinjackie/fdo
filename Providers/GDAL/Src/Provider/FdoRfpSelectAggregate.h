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
 *
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpSelectAggregate.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#ifndef FDORFPSELECTAGGREGATE_H
#define FDORFPSELECTAGGREGATE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "FdoRfpFeatureCommand.h"

class FdoRfpSelectAggregate : public FdoRfpFeatureCommand<FdoISelectAggregates>
{
    friend class FdoRfpConnection; //Need to create the object of this type
//
// data members
//
private:
    FdoPtr<FdoIdentifierCollection> m_propertiesToSelect;
//
// constructor and destructor
//
protected:
    FdoRfpSelectAggregate (FdoIConnection *connection);
    virtual ~FdoRfpSelectAggregate (void);

//
// exposed functions
//
public:
    /// <summary>Gets the FdoIdentifierCollection that holds the list of property names to 
    /// return with the result. If empty all properties of the specified class
    /// are returned.</summary>
    /// <returns>Returns the list of property names.</returns> 
    virtual FdoIdentifierCollection* GetPropertyNames();

    /// <summary>Executes the select command and returns a reference to an FdoIDataReader.</summary>
    /// <returns>Returns the data reader.</returns> 
    virtual FdoIDataReader* Execute();

    /// <summary>Set the distinct option of the selection. Note that grouping criteria is not supported with Distinct. 
    /// Non-simple properties such as object properties, geometry properties, raster properties, association properties, etc. will not be supported with Distinct.</summary>
    /// <param name="value">true or false; when set to true, only distinct values are returned. Otherwise all values are returned</param> 
    /// <returns>Returns nothing</returns>  
    virtual void SetDistinct( bool value );

    /// <summary>Get the distinct option.</summary>
    /// <returns>Returns true if distinct is set, false otherwise.</returns> 
    virtual bool GetDistinct( );

    /// <summary>Gets the FdoIdentifierCollection that holds the list of group by property names. If empty no grouping is used. This list is initially
    /// empty and the caller need to add the property that the command should use as a group by criteria. No LOB or Geometry type properties
    /// can be used for ordering.</summary>
    /// <returns>Returns the list of group by property names.</returns> 
    virtual FdoIdentifierCollection* GetGrouping();

    /// <summary>Gets the FdoIdentifierCollection that holds the list of order by property names. If empty no ordering is used. This list is initially
    /// empty and the caller need to add the property that the command should use as a order by criteria.</summary>
    /// <returns>Returns the list of group by property names.</returns> 
    virtual FdoIdentifierCollection* GetOrdering();

    /// <summary>Set the ordering option of the selection. This is only used if the ordering collection is not empty.</summary>
    /// <param name="option">Is the ordering option and should be set to one of FdoOrderingOption_Ascending or FdoOrderingOption_Descending.
    /// FdoOrderingOption_Ascending is the default value.</param> 
    /// <returns>Returns nothing</returns> 
    virtual void SetOrderingOption( FdoOrderingOption  option );

    /// <summary>Gets the ordering option.</summary>
    /// <returns>Returns the ordering option.</returns> 
    virtual FdoOrderingOption GetOrderingOption( );

    /// <summary>Set the grouping by filter. Use the grouping filter to restrict the groups of returned properties to those groups for 
    /// which the specified filter is TRUE. For example "order by city" and  "min(lanes) = 2". The FdoFilter have to evalute to a 
    /// binary value(true or false).</summary>
    /// <param name="filter">The grouping filter.</param> 
    /// <returns>Returns nothing</returns> 
    virtual void SetGroupingFilter( FdoFilter* filter );

    /// <summary>Gets the grouping by filter.</summary>
    /// <returns>Returns the grouping filter.</returns> 
    virtual FdoFilter* GetGroupingFilter( );


};


#endif
