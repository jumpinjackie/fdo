// 
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
class SdfCommandCapabilities : public FdoICommandCapabilities
{
    //-------------------------------------------------------
    // Variables
    //-------------------------------------------------------


    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

    public:
        // constructs an SdfCommandCapabilities with the specified flag
        SDF_API SdfCommandCapabilities();

    protected:
        // default destructor
        SDF_API virtual ~SdfCommandCapabilities();

    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------

    protected:
        // dispose this object
        SDF_API virtual void Dispose();

    //-------------------------------------------------------
    // FdoICommandCapabilities implementation
    //-------------------------------------------------------

    public:
        // Returns an array of the CommandTypes supported by the feature provider.
        // The size parameter gives the number of commands in the array.
        SDF_API virtual FdoInt32* GetCommands(FdoInt32& size);

        // Returns true if commands support parameterization, false otherwise.
        SDF_API virtual bool SupportsParameters();

        // Returns true if the feature provider supports command execution timeout.
        SDF_API virtual bool SupportsTimeout();

        /// <summary>Determines if the feature provider can use expressions for properties with Select and SelectAggregate commands.</summary>
        /// <returns>Returns true if the feature provider supports select expressions.</returns> 
        SDF_API virtual bool SupportsSelectExpressions();

        /// <summary>Determines if simple functions can be used in Select and SelectAggregates command. 
        /// Aggregate functions can only be used in the SelectAggregates command. Capability for using SelectAggregates is found in 
        /// CommandCapabilities.</summary>
        /// <returns>Returns true if the feature provider supports select simple functions.</returns> 
        SDF_API virtual bool SupportsSelectFunctions();

        /// <summary>Determines if Distinct can be used with SelectAggregates. This can be true only 
        /// if the SelectAggregates command is supported by the provider.</summary>
        /// <returns>Returns true if the feature provider supports select distinct.</returns> 
        SDF_API virtual bool SupportsSelectDistinct();

        /// <summary>Determines  if ordering is available in the Select and SelectAggregates command.</summary>
        /// <returns>Returns true if the feature provider supports select ordering.</returns> 
        SDF_API virtual bool SupportsSelectOrdering();

        /// <summary>Determines if a grouping criteria is available in the SelectAggregates command. 
        /// This can be true only if the SelectAggregates command is supported by the provider. Note that aggregate 
        /// functions can be supported without also supporting grouping criteria (but not vice versa).</summary>
        /// <returns>Returns true if the feature provider supports select grouping.</returns> 
        SDF_API virtual bool SupportsSelectGrouping();
};

