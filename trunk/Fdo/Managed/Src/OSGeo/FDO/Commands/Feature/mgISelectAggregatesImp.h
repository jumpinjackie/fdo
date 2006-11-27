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
*/

#pragma once

#include "FDO\Commands\Feature\mgIBaseSelectImp.h"
#include "FDO\Commands\Feature\mgISelectAggregates.h"

class FdoISelectAggregates;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS
public __gc class IdentifierCollection;
END_NAMESPACE_OSGEO_FDO_COMMANDS

BEGIN_NAMESPACE_OSGEO_FDO_FILTER
public __gc class Filter;
END_NAMESPACE_OSGEO_FDO_FILTER

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE
public __gc __interface IDataReader;

/// \brief
/// The FdoISelectAggregate interface defines a Select command that handle selection of properties that include 
/// the use of aggregate functions. This command differs from the FdoISelect command in a number of ways. 
/// It handles selection of properties that are aggregate functions and supports selecting distinct property values. 
/// It also includes grouping criteria. The execute method returns an IDataReader which is not tied to a specific class. 
/// Unlike Select, FdoISelectAggregate does not include any locking functions.
private __gc class ISelectAggregatesImp : public NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IBaseSelectImp, public NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectAggregates
{
public:
    /// \brief
    /// Executes the select command and returns a reference to an FdoIDataReader.
    /// 
    /// \return
    /// Returns the data reader.
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IDataReader* Execute();

    /// \brief
    /// Set the distinct option of the selection. 
    /// Non-simple properties such as object properties, geometry properties, raster properties, association properties, etc. will not be supported with Distinct.
    /// 
    /// \param value 
    /// true or false; when set to true, only distinct values are returned. Otherwise all values are returned
    /// 
    /// \return
    /// Returns nothing
    /// 
    /// Note: Grouping criteria is not supported with Distinct. 
	__property System::Void set_Distinct(System::Boolean value);

    /// \brief
    /// Get the distinct option.
    /// 
    /// \return
    /// Returns true if distinct is set, false otherwise.
    /// 
	__property System::Boolean get_Distinct();

    /// \brief
    /// Gets the FdoIdentifierCollection that holds the list of group by property names. If empty no grouping is used. This list is initially
    /// empty and the caller need to add the property that the command should use as a group by criteria. No LOB or Geometry type properties
    /// can be used for ordering.
    /// 
    /// \return
    /// Returns the list of group by property names.
    /// 
	__property NAMESPACE_OSGEO_FDO_COMMANDS::IdentifierCollection* get_Grouping();

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
	__property System::Void set_GroupingFilter(NAMESPACE_OSGEO_FDO_FILTER::Filter* filter);

    /// \brief
    /// Gets the grouping by filter.
    /// 
    /// \return
    /// Returns the grouping filter.
    /// 
	__property NAMESPACE_OSGEO_FDO_FILTER::Filter* get_GroupingFilter();

public private:
	ISelectAggregatesImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IBaseSelectImp(unmanaged, autoDelete)
	{

	}

	inline FdoISelectAggregates* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE


