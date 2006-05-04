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

#include "FDO\Commands\mgICommand.h"

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION
public __gc class Identifier;
END_NAMESPACE_OSGEO_FDO_EXPRESSION

BEGIN_NAMESPACE_OSGEO_FDO_FILTER
public __gc class Filter;
END_NAMESPACE_OSGEO_FDO_FILTER

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS

/// \brief
/// The FdoIFeatureCommand interface defines the behavior common 
/// to all commands that operate on feature instances. The features to 
/// be operated on are identified by their class and a filter. If the feature 
/// provider supports inheritance in the schema capabilities, then the command 
/// will operate on instances of that class and any derived classes. 
/// In addition, feature commands can operate on related features if one or 
/// more relation directives are specified. Each relation directive specifies 
/// the relation to follow and how many levels of depth to follow it.
public __gc __interface IFeatureCommand : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommand
{
public:
    /// \brief
    /// Gets the name of the class to be operated upon as an identifier.
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier* get_FeatureClassName();

    /// \brief
    /// Sets the name of the class to be operated upon as an identifier.
    /// 
    /// \param value 
    /// Input the class identifier
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_FeatureClassName(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier* value);

    /// \brief
    /// Sets the name of the class to be operated upon as an identifier.
    /// 
    /// \param value 
    /// Input the class name
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void SetFeatureClassName(System::String* value);

    /// \brief
    /// Gets the filter as a filter tree.
    /// 
    /// \return
    /// Returns the filter object
    /// 
	__property NAMESPACE_OSGEO_FDO_FILTER::Filter* get_Filter();

    /// \brief
    /// Sets the filter as a Filter tree.
    /// 
    /// \param value 
    /// Input the filter object
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Filter(NAMESPACE_OSGEO_FDO_FILTER::Filter* value);

    /// \brief
    /// Sets the filter as a Filter tree.
    /// 
    /// \param value 
    /// Input the filter expression string
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void SetFilter(System::String* value);
};

END_NAMESPACE_OSGEO_FDO_COMMANDS


