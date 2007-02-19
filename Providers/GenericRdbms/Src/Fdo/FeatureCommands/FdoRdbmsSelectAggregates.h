#ifndef _FDORDBMSSELECTAGGREGATES_H_
#define _FDORDBMSSELECTAGGREGATES_H_
//
 //
 // Copyright (C) 2004-2006  Autodesk, Inc.
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
 // Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
/// <library>FDO.lib</library>

#ifdef _WIN32
#pragma once
#endif

#include "FdoRdbms.h"

// FeatureCommand
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsFeatureCommand.h"
#include "FdoRdbmsDataReader.h"

// the macro EXECUTE_METHOD is defined in FdoRdbmsDataReader. To re-use that macro pReader need to be replaced by the pSelect
#define pReader  pSelect

/// <summary>The FdoISelectAggregate interface defines a Select command that handle selection of properties that include
/// the use of aggregate functions. This command differs from the FdoISelect command in a number of ways.
/// It handles selection of properties that are aggregate functions and supports selecting distinct property values.
/// It also includes grouping criteria. The execute method returns an IDataReader which is not tied to a specific class.
/// Unlike Select, FdoISelectAggregate does not include any locking functions.</summary>
class FdoRdbmsSelectAggregates : public FdoISelectAggregates
{
    friend class FdoRdbmsConnection;
protected:
     //
      // Prevent the use of the copy constructor by definning it and not implemeting it.
      // DO NOT IMPLEMENT
      FdoRdbmsSelectAggregates(const FdoRdbmsSelectAggregates &right);

      //    Constructs a default instance of a FdoRdbmsSelectAggregates.
      FdoRdbmsSelectAggregates (): mbDistinct(false)
      {
            pSelect = new FdoRdbmsSelectCommand();
      }

      //    Constructs an instance of a FdoRdbmsSelectAggregates using the
      //    specified arguments.
      FdoRdbmsSelectAggregates (FdoIConnection *connection):mbDistinct(false)
      {
           pSelect = new FdoRdbmsSelectCommand( connection );
      }

      virtual ~FdoRdbmsSelectAggregates() { }

      virtual void Dispose() { delete this; }

public:
    /// <summary>Gets the FdoIdentifierCollection that holds the list of property names to
    /// return with the result. If empty all properties of the specified class
    /// are returned.</summary>
    /// <returns>Returns the list of property names.</returns>
    virtual FdoIdentifierCollection* GetPropertyNames()
    {
        EXECUTE_METHOD(GetPropertyNames());
    }

    /// <summary>Executes the select command and returns a reference to an FdoIDataReader.</summary>
    /// <returns>Returns the data reader.</returns>
    virtual FdoIDataReader* Execute()
    {
        if( NULL == pSelect.p )
            throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_22, errorMsg ));

        return new FdoRdbmsDataReader( FdoPtr<FdoRdbmsFeatureReader>((FdoRdbmsFeatureReader*)pSelect->Execute( mbDistinct, FdoCommandType_SelectAggregates ) ) );
    }

    /// <summary>Set the distinct option of the selection. Note that grouping criteria is not supported with Distinct.
    /// Non-simple properties such as object properties, geometry properties, raster properties, association properties, etc. will not be supported with Distinct.</summary>
    /// <param name="value">true or false; when set to true, only distinct values are returned. Otherwise all values are returned</param>
    /// <returns>Returns nothing</returns>
    virtual void SetDistinct( bool value ) { mbDistinct = value; }

    /// <summary>Get the distinct option.</summary>
    /// <returns>Returns true if distinct is set, false otherwise.</returns>
    virtual bool GetDistinct( ) { return mbDistinct; }

    /// <summary>Gets the FdoIdentifierCollection that holds the list of group by property names. If empty no grouping is used. This list is initially
    /// empty and the caller need to add the property that the command should use as a group by criteria. No LOB or Geometry type properties
    /// can be used for ordering.</summary>
    /// <returns>Returns the list of group by property names.</returns>
    virtual FdoIdentifierCollection* GetGrouping()
    {
        EXECUTE_METHOD( GetGrouping() );
    }

    /// <summary>Gets the FdoIdentifierCollection that holds the list of order by property names. If empty no ordering is used. This list is initially
    /// empty and the caller need to add the property that the command should use as a order by criteria.</summary>
    /// <returns>Returns the list of group by property names.</returns>
    virtual FdoIdentifierCollection* GetOrdering()
    {
        EXECUTE_METHOD( GetOrdering() );
    }

    /// <summary>Set the ordering option of the selection. This is only used if the ordering collection is not empty.</summary>
    /// <param name="option">Is the ordering option and should be set to one of FdoOrderingOption_Ascending or FdoOrderingOption_Descending.
    /// FdoOrderingOption_Ascending is the default value.</param>
    /// <returns>Returns nothing</returns>
    virtual void SetOrderingOption( FdoOrderingOption  option )
    {
        EXECUTE_METHOD( SetOrderingOption( option ) );
    }

    /// <summary>Gets the ordering option.</summary>
    /// <returns>Returns the ordering option.</returns>
    virtual FdoOrderingOption GetOrderingOption( )
    {
        EXECUTE_METHOD( GetOrderingOption() );
    }

    /// <summary>Set the grouping by filter. Use the grouping filter to restrict the groups of returned properties to those groups for
    /// which the specified filter is TRUE. For example "order by city" and  "min(lanes) = 2". The FdoFilter have to evalute to a
    /// binary value(true or false).</summary>
    /// <param name="filter">The grouping filter.</param>
    /// <returns>Returns nothing</returns>
    virtual void SetGroupingFilter( FdoFilter* filter )
    {
        EXECUTE_METHOD( SetGroupingFilter( filter ) );
    }

    /// <summary>Gets the grouping by filter.</summary>
    /// <returns>Returns the grouping filter.</returns>
    virtual FdoFilter* GetGroupingFilter( )
    {
        EXECUTE_METHOD( GetGroupingFilter() );
    }

    //
    // from FdoIFeatureCommand
    virtual FdoIdentifier* GetFeatureClassName()
    {
        EXECUTE_METHOD( GetFeatureClassName() );
    }

    virtual void SetFeatureClassName(FdoIdentifier* value)
    {
        EXECUTE_METHOD( SetFeatureClassName( value ) );
    }

    virtual void SetFeatureClassName(FdoString* value)
    {
        EXECUTE_METHOD( SetFeatureClassName( value) );
    }

    virtual FdoFilter* GetFilter()
    {
        EXECUTE_METHOD( GetFilter() );
    }

    virtual void SetFilter(FdoFilter* value)
    {
        EXECUTE_METHOD( SetFilter( value) );
    }

    virtual void SetFilter(FdoString* value)
    {
        EXECUTE_METHOD( SetFilter( value) );
    }
    //////////////////

    virtual FdoIConnection* GetConnection()
    {
        EXECUTE_METHOD( GetConnection() );
    }

    virtual FdoITransaction* GetTransaction()
    {
        EXECUTE_METHOD( GetTransaction() );
    }

    virtual void SetTransaction(FdoITransaction* value)
    {
        EXECUTE_METHOD( SetTransaction( value) );
    }

    virtual FdoInt32 GetCommandTimeout()
    {
        EXECUTE_METHOD( GetCommandTimeout() );
    }

    virtual void SetCommandTimeout(FdoInt32 value)
    {
        EXECUTE_METHOD( SetCommandTimeout( value) );
    }

    virtual FdoParameterValueCollection* GetParameterValues()
    {
        EXECUTE_METHOD( GetParameterValues() );
    }

    virtual void Prepare()
    {
        EXECUTE_METHOD( Prepare() );
    }

    virtual void Cancel()
    {
        EXECUTE_METHOD( Cancel() );
    }
    //////////////////////////////////////////

private:

    FdoPtr<FdoRdbmsSelectCommand>  pSelect;

    bool                           mbDistinct;
};
#endif
