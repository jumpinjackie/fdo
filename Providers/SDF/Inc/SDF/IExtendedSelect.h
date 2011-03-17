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

#include "Fdo.h"
#include "SDF/IScrollableFeatureReader.h"
#include "SDF/SdfCompareHandler.h"

class SdfIExtendedSelect : public FdoIExtendedSelect
{		
public:
	// Expose the FdoISelect hidden functions
	using FdoISelect::GetOrderingOption;
	using FdoISelect::SetOrderingOption;
	// Expose the FdoIExtendedSelect hidden function
	using FdoIExtendedSelect::ExecuteScrollable;
	using FdoIExtendedSelect::GetOrderingOption;
	using FdoIExtendedSelect::SetOrderingOption;

	/// \brief
	/// Executes the select command and returns a SdfIScrollableFeatureReader.
	/// \remarks
	/// Creates a temporary SDF file that contains the selected properties.
	/// If ordering is enabled, then the created temporary SDF file
	/// is sorted according to the ordering collection.
	/// In this case the sorting is file based where the SQLite BTree is used. 
	//  The extendedProps and extendedDefaults can be used to extend the class definition of the selected data. 
	/// Both collections have to be in-sync otherwise an exception will be thrown.
	///
	/// \param sdfCacheFile
	/// File path and name used to store the result of the query.
	///
	/// \param extendedProps
	/// A collection of data property definition that extends the class definition defined by the selected properties.
	///
	/// \param extendedDefaults
	/// A collection of property values used as defaults for extended properties defined by extendedProps parameter.
	///
    /// \return
	 /// Returns a SdfIScrollableFeatureReader object.
    virtual SdfIScrollableFeatureReader* ExecuteScrollable( const wchar_t* sdfCacheFile, FdoDataPropertyDefinitionCollection* extendedProps, FdoPropertyValueCollection*  extendedDefaults ) = 0;
};

#endif

