#ifndef _IEXTENDEDSELECT_H_
#define _IEXTENDEDSELECT_H_
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

#ifdef _WIN32
#pragma once
#endif
 
#include <Fdo/Commands/Feature/ISelect.h>
#include <Fdo/Commands/Feature/IScrollableFeatureReader.h>

/// \brief
/// The FdoIExtendedSelect interface defines a Select command with extended 
/// capabilities. It provides the same functionality as FdoISelect, plus the
/// ability to create a scrollable reader that allows forward, reverse or 
/// random access to the results set. 
class FdoIExtendedSelect : public FdoISelect
{		
	
public:
	/// \brief
	/// Set the ordering option of the selection.
	/// 
	/// \remarks
	/// This is only used if the ordering collection is not empty.
	///
	/// \param propertyName
	/// Is the property name for which the ordering should be applied. 
    /// This property should be in the ordering collection.
	///
    /// \param option
	/// Is the ordering option and should be set to one of FdoOrderingOption_Ascending or FdoOrderingOption_Descending.
    /// FdoOrderingOption_Ascending is the default value.
	virtual void SetOrderingOption( FdoString* propertyName, FdoOrderingOption  option ) = 0;

    /// \brief
	/// Gets the ordering option for a given property.
	///
	/// \param propertyName
	/// Is the property name for which the ordering should be applied. 
    /// This property should be in the ordering collection.
	///
    /// \return
	/// Returns the ordering option.
	virtual FdoOrderingOption GetOrderingOption( FdoString* propertyName ) = 0;

	/// \brief
	/// Clears the internal list of property/ordering option list and re-sets the ordering option for all 
	/// the ordering properties to the default FdoOrderingOption_Ascending or to the ordering option specified by the 
	/// FdoIBaseSelect interface.
	virtual void ClearOrderingOptions( ) = 0;

	/// \brief
	/// Executes the select command and returns an FdoIScrollableFeatureReader.
	///
	/// \remarks  
	/// If ordering is enabled, then the returned reader is sorted according to 
	/// the ordering collection. This method performs sorting and allocates an integer value for each row.
    /// \return
	/// Returns a FdoIScrollableFeatureReader object
	virtual FdoIScrollableFeatureReader* ExecuteScrollable() = 0;
};
#endif


