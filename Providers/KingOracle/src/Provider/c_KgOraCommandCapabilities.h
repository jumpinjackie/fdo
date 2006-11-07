/*
* Copyright (C) 2006  SL-King d.o.o
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


#ifndef _c_KGORACOMMANDCAPABILITIES_H
#define _c_KGORACOMMANDCAPABILITIES_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

class c_KgOraCommandCapabilities : public FdoICommandCapabilities
{
public:
    c_KgOraCommandCapabilities ();

protected:
    virtual ~c_KgOraCommandCapabilities ();

protected:
    virtual void Dispose();

public:
    /// \brief
    /// Gets an array of the FdoCommandType values supported by the feature provider.
    /// 
    /// \param size 
    /// Output the number of commands
    /// 
    /// \return
    /// Returns the list of commands
    /// 
    FDOKGORA_API virtual GisInt32* GetCommands(GisInt32& Size);

    /// \brief
    /// Determines if commands support parameterization.
    /// 
    /// \return
    /// Returns true if commands support parameterization
    /// 
    FDOKGORA_API virtual bool SupportsParameters();

    /// \brief
    /// Determines if the feature provider supports command execution timeout.
    /// 
    /// \return
    /// Returns true if the feature provider supports timeout.
    /// 
    FDOKGORA_API virtual bool SupportsTimeout();

    /// \brief
    /// Determines if the feature provider can use expressions for properties with Select and SelectAggregate commands.
    /// 
    /// \return
    /// Returns true if the feature provider supports select expressions.
    /// 
    FDOKGORA_API virtual bool SupportsSelectExpressions();

    /// \brief
    /// Determines if simple functions can be used in Select and SelectAggregates command. 
    /// Aggregate functions can only be used in the SelectAggregates command. Capability for using SelectAggregates is found in 
    /// CommandCapabilities.
    /// 
    /// \return
    /// Returns true if the feature provider supports select simple functions.
    /// 
    FDOKGORA_API virtual bool SupportsSelectFunctions();

    /// \brief
    /// Determines if Distinct can be used with SelectAggregates. This can be true only 
    /// if the SelectAggregates command is supported by the provider.
    /// 
    /// \return
    /// Returns true if the feature provider supports select distinct.
    /// 
    FDOKGORA_API virtual bool SupportsSelectDistinct();

    /// \brief
    /// Determines  if ordering is available in the Select and SelectAggregates command.
    /// 
    /// \return
    /// Returns true if the feature provider supports select ordering.
    /// 
    FDOKGORA_API virtual bool SupportsSelectOrdering();

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
    FDOKGORA_API virtual bool SupportsSelectGrouping();
};

#endif // _c_KGORACOMMANDCAPABILITIES_H

