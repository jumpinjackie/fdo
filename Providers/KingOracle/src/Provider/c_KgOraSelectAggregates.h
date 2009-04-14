/*
* Copyright (C) 2008  SL-King d.o.o
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


#ifndef _c_KgOraSelectAggregates_
#define _c_KgOraSelectAggregates_

#include "c_KgOraFilterProcessor.h"

class c_KgOraSelectAggregates :
    public c_KgOraFdoFeatureCommand<FdoISelectAggregates>
{
    friend class c_KgOraConnection;

protected:
    FdoPtr<FdoIdentifierCollection> m_PropertyNames;
    FdoOrderingOption m_OrderingOption;
    FdoPtr<FdoIdentifierCollection> m_OrderingIdentifiers;
    
    FdoPtr<FdoIdentifierCollection> m_Grouping;
    FdoPtr<FdoFilter> m_GroupingFilter;
    
    bool m_Distinct;
    
protected:
    c_KgOraSelectAggregates (c_KgOraConnection* Connection);
    virtual ~c_KgOraSelectAggregates (void);

public:
    //
    // Prevent the use of the Assignment Operation by defining it and not implementing it.
    // DO NOT IMPLEMENT
    c_KgOraSelectCommand & operator= (const c_KgOraSelectCommand &right);

    void Dispose ();

    /// <summary>Gets the FdoIdentifierCollection that holds the list of property names to 
    /// return with the result. If empty all properties of the specified class
    /// are returned.</summary>
    /// <returns>Returns the list of property names.</returns> 
    virtual FdoIdentifierCollection* GetPropertyNames ();

    

    /// <summary>Executes the select command and returns a reference to an FdoIFeatureReader.</summary>
    /// <returns>Returns the feature reader.</returns> 
    virtual FdoIDataReader* Execute ();

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
    /// \note
	/// Grouping criteria is not supported with Distinct. 
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
    
protected:
  std::wstring CreateSqlString(c_KgOraFilterProcessor& FilterProc,int& GeomSqlColumnIndex,FdoStringCollection* SqlColumns);
  void CreateFilterSqlString(FdoFilter* Filter,string& WhereBuff);
  FdoStringP ToSqlString(FdoIdentifierCollection* Idents);
};

#endif 

