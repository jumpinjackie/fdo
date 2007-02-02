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
#ifndef FDOPOSTGIS_COMMANDCAPABILITIES_H_INCLUDED
#define FDOPOSTGIS_COMMANDCAPABILITIES_H_INCLUDED


namespace fdo { namespace postgis {


/// Implementation of interface describing command capabilities
/// supported by PostGIS provider.
///
class CommandCapabilities : public FdoICommandCapabilities
{
public:

    /// Default constructor.
    CommandCapabilities();

    //
    // FdoICommandCapabilities interface
    //

    /// Get array of the FdoCommandType values supported by the feature provider.
    FdoInt32* GetCommands(FdoInt32& size);
 	
    /// Determine if commands support parameterization.
    bool SupportsParameters();
 	
    /// Determine if the feature provider supports command execution timeout.
    bool SupportsTimeout();
 	
    /// Determine if the feature provider can use expressions for properties.
    /// Expressions are used with commands Select and SelectAggregate.
    bool SupportsSelectExpressions();
 	
    /// Determine if simple functions in select commands.
    /// Functions are used with commands Select and SelectAggregates.
    bool SupportsSelectFunctions();
 	
    /// Determine if Distinct can be used with SelectAggregates.
    bool SupportsSelectDistinct();
 	
    /// Determine if ordering is supported by select commands.
    /// Ordering is used with commands Select and SelectAggregates.
    bool SupportsSelectOrdering();
 	
    /// Determine if grouping criteria use is supported.
    /// Grouping is available to use with SelectAggregates command.
    bool SupportsSelectGrouping();

protected:

    /// Destructor.
    virtual ~CommandCapabilities();

    //
    // FdoIDisposable interface
    //

    /// Dispose this object.
    void Dispose();

private:

};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_COMMANDCAPABILITIES_H_INCLUDED
