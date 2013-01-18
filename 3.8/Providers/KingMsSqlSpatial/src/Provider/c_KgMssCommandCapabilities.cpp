/*
* Copyright (C) 2007  Haris Kurtagic
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


#include "stdafx.h"

c_KgMssCommandCapabilities::c_KgMssCommandCapabilities ()
{
}

c_KgMssCommandCapabilities::~c_KgMssCommandCapabilities ()
{
}

void c_KgMssCommandCapabilities::Dispose()
{
    delete this;
}

/// \brief
/// Gets an array of the FdoCommandType values supported by the feature provider.
/// 
/// \param size 
/// Output the number of commands
/// 
/// \return
/// Returns the list of commands
/// 
FdoInt32* c_KgMssCommandCapabilities::GetCommands (FdoInt32& Size)
{
    static const FdoCommandType commands[] =
    {
        FdoCommandType_Select,
        FdoCommandType_SQLCommand,
        FdoCommandType_SelectAggregates,
        FdoCommandType_DescribeSchema,
        //FdoCommandType_DescribeSchemaMapping,
        
        FdoCommandType_GetSpatialContexts,
        FdoCommandType_CreateSpatialContext,
        
        FdoCommandType_Insert,
        FdoCommandType_Update,
        FdoCommandType_Delete,
        FdoCommandType_ApplySchema,
        FdoCommandType_CreateDataStore,
        //FdoCommandType_DestroySchema,
    };

    Size = sizeof (commands) / sizeof (FdoCommandType);

    return ((FdoInt32*)commands);
}//end of c_KgMssCommandCapabilities::GetCommands 

/// \brief
/// Determines if commands support parameterization.
/// 
/// \return
/// Returns true if commands support parameterization
///
bool c_KgMssCommandCapabilities::SupportsParameters ()
{
    return (true);
}

/// \brief
/// Determines if the feature provider supports command execution timeout.
/// 
/// \return
/// Returns true if the feature provider supports timeout.
/// 
bool c_KgMssCommandCapabilities::SupportsTimeout ()
{
    return (false);
}



/// \brief
/// Determines if the feature provider can use expressions for properties with Select and SelectAggregate commands.
/// 
/// \return
/// Returns true if the feature provider supports select expressions.
///
bool c_KgMssCommandCapabilities::SupportsSelectExpressions()
{
    return (true);
}

/// \brief
/// Determines if simple functions can be used in Select and SelectAggregates command. 
/// Aggregate functions can only be used in the SelectAggregates command. Capability for using SelectAggregates is found in 
/// CommandCapabilities.
/// 
/// \return
/// Returns true if the feature provider supports select simple functions.
/// 
bool c_KgMssCommandCapabilities::SupportsSelectFunctions()
{
    return (true);
}

/// \brief
/// Determines if Distinct can be used with SelectAggregates. This can be true only 
/// if the SelectAggregates command is supported by the provider.
/// 
/// \return
/// Returns true if the feature provider supports select distinct.
/// 
bool c_KgMssCommandCapabilities::SupportsSelectDistinct()
{
    return (true);
}

/// \brief
/// Determines  if ordering is available in the Select and SelectAggregates command.
/// 
/// \return
/// Returns true if the feature provider supports select ordering.
/// 
bool c_KgMssCommandCapabilities::SupportsSelectOrdering()
{
    return (true);
}

/// \brief
/// Determines if a grouping criteria is available in the SelectAggregates command. 
/// This can be true only if the SelectAggregates command is supported by the provider. 
/// 
/// \return
/// Returns true if the feature provider supports select grouping.
/// 
/// \note
/// Aggregate functions can be supported without also supporting grouping criteria 
/// (but not vice versa).
bool c_KgMssCommandCapabilities::SupportsSelectGrouping()
{
    return (true);
}

