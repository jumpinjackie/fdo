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

#ifndef _SdfIExtendedSelect_H_
#define _SdfIExtendedSelect_H_

#include "FdoStd.h"
#include "Fdo.h"
#include "SDF\IScrollableFeatureReader.h"
#include "SDF\SdfCompareHandler.h"

class SdfIExtendedSelect : public FdoISelect
{		
	
public:
	// Expose the FdoISelect hidden functions
	using FdoISelect::GetOrderingOption;
	using FdoISelect::SetOrderingOption;

	/// <summary>Set the ordering option of the selection. This is only used if the ordering collection is not empty.</summary>
	/// <param name="propertyName">Is the property name for which the ordering should be applied. This property should be in the ordering collection.</param> 
    /// <param name="option">Is the ordering option and should be set to one of FdoOrderingOption_Ascending or FdoOrderingOption_Descending.
    /// FdoOrderingOption_Ascending is the default value.</param> 
    /// <returns>Returns nothing</returns> 
	virtual void SetOrderingOption( FdoString* propertyName, FdoOrderingOption  option ) = 0;

    /// <summary>Gets the ordering option for a given property.</summary>
	/// <param name="propertyName">Is the property name for which the ordering should be applied. This property should be in the ordering collection.</param> 
    /// <returns>Returns the ordering option.</returns> 
	virtual FdoOrderingOption GetOrderingOption( FdoString* propertyName ) = 0;

	/// <summary>This method clears the internal list of property/ordering option list and re-sets the ordering option for all 
	/// the ordering properties to the default FdoOrderingOption_Ascending or to the ordering option specified by the 
	/// FdoIBaseSelect interface.</summary>
    /// <returns>Returns nothing.</returns> 
	virtual void ClearOrderingOptions( ) = 0;

	/// <summary>Set the compare handler. This method is used to override the default sdf compare handler.</summary>
    /// <param name="handler">The compare handle that should be derived from the SdfCompareHandler class.</param> 
    /// <returns>Returns nothing</returns> 
	virtual void SetCompareHandler( SdfCompareHandler*  handler ) = 0;

	/// <summary> Executes the select command and returns a SdfIScrollableFeatureReader.  
	/// If ordering is enabled, then the returned reader is sorted according to 
	/// the ordering collection. This method perform sorting in memory and allocates an integer value for each row.</summary>
    /// <returns>Returns a SdfIScrollableFeatureReader object</returns> 
	virtual SdfIScrollableFeatureReader* ExecuteScrollable() = 0;

	/// <summary> Executes the select command and returns a SdfIScrollableFeatureReader. This method creates a temporary SDF file 
	/// that contains the selected properties. If ordering is enabled, then the created temporary SDF file is sorted according to 
	/// the ordering collection. In this case the sorting is file based where the SQLite BTree is used. 
	//  The extendedProps and extendedDefaults can be used to extend the class definition of the selected data. 
	/// Both collections have to be in-sync otherwise an exception will be thrown.</summary>
	/// <param name="sdfCacheFile">File path and name used to store the result of the query.</param> 
	/// <param name="extendedProps">A collection of data property definition that extends the class definition defined by the selected properties.</param> 
	/// <param name="extendedDefaults">A collection of property values used as defaults for extended properties defined by extendedProps parameter.</param> 
    /// <returns>Returns a SdfIScrollableFeatureReader object</returns> 
    virtual SdfIScrollableFeatureReader* ExecuteScrollable( const wchar_t* sdfCacheFile, FdoDataPropertyDefinitionCollection* extendedProps, FdoPropertyValueCollection*  extendedDefaults ) = 0;
};

#endif

