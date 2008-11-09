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

#include "FDO\Commands\Feature\mgISelectImp.h"
#include "FDO\Commands\Feature\mgIExtendedSelect.h"

class FdoISelect;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE

/// \ingroup (OSGeoFDOCommandsFeature)
/// \brief
/// The IExtendedSelectImp class is a concrete implementation of 
/// interface IExtendedSelect.
private __gc class IExtendedSelectImp : public NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectImp, public NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IExtendedSelect
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
	void SetOrderingOption( System::String* propertyName, NAMESPACE_OSGEO_FDO_COMMANDS::OrderingOption  option );

    /// \brief
	/// Gets the ordering option for a given property.
	///
	/// \param propertyName
	/// Is the property name for which the ordering should be applied. 
    /// This property should be in the ordering collection.
	///
    /// \return
	/// Returns the ordering option.
    NAMESPACE_OSGEO_FDO_COMMANDS::OrderingOption GetOrderingOption( System::String* propertyName );

	/// \brief
	/// Clears the internal list of property/ordering option list and re-sets the ordering option for all 
	/// the ordering properties to the default FdoOrderingOption_Ascending or to the ordering option specified by the 
	/// FdoIBaseSelect interface.
	void ClearOrderingOptions( );

	/// \brief
	/// Executes the select command and returns an IScrollableFeatureReader.
	///
	/// \remarks  
	/// If ordering is enabled, then the returned reader is sorted according to 
	/// the ordering collection. This method performs and allocates an integer value for each row.
    /// \return
	/// Returns a FdoIScrollableFeatureReader object
	NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IScrollableFeatureReader* ExecuteScrollable();

public private:
	IExtendedSelectImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectImp(unmanaged, autoDelete)
	{

	}

	inline FdoIExtendedSelect* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE


