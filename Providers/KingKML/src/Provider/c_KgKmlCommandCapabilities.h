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


#ifndef _c_KGOKMLCOMMANDCAPABILITIES_H
#define _c_KGOKMLCOMMANDCAPABILITIES_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

class c_KgKmlCommandCapabilities : public FdoICommandCapabilities
{
public:
    c_KgKmlCommandCapabilities ();

protected:
    virtual ~c_KgKmlCommandCapabilities ();

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
    FDOKGKML_API virtual FdoInt32* GetCommands(FdoInt32& Size);

    /// \brief
    /// Determines if commands support parameterization.
    /// 
    /// \return
    /// Returns true if commands support parameterization
    /// 
    FDOKGKML_API virtual bool SupportsParameters();

    /// \brief
    /// Determines if the feature provider supports command execution timeout.
    /// 
    /// \return
    /// Returns true if the feature provider supports timeout.
    /// 
    FDOKGKML_API virtual bool SupportsTimeout();

    /// \brief
    /// Determines if the feature provider can use expressions for properties with Select and SelectAggregate commands.
    /// 
    /// \return
    /// Returns true if the feature provider supports select expressions.
    /// 
    FDOKGKML_API virtual bool SupportsSelectExpressions();

    /// \brief
    /// Determines if simple functions can be used in Select and SelectAggregates command. 
    /// Aggregate functions can only be used in the SelectAggregates command. Capability for using SelectAggregates is found in 
    /// CommandCapabilities.
    /// 
    /// \return
    /// Returns true if the feature provider supports select simple functions.
    /// 
    FDOKGKML_API virtual bool SupportsSelectFunctions();

    /// \brief
    /// Determines if Distinct can be used with SelectAggregates. This can be true only 
    /// if the SelectAggregates command is supported by the provider.
    /// 
    /// \return
    /// Returns true if the feature provider supports select distinct.
    /// 
    FDOKGKML_API virtual bool SupportsSelectDistinct();

    /// \brief
    /// Determines  if ordering is available in the Select and SelectAggregates command.
    /// 
    /// \return
    /// Returns true if the feature provider supports select ordering.
    /// 
    FDOKGKML_API virtual bool SupportsSelectOrdering();

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
    FDOKGKML_API virtual bool SupportsSelectGrouping();
};

#endif // _c_KGOKMLCOMMANDCAPABILITIES_H

