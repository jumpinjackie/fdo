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

#ifndef SHPIEXTENDEDSELECT_H
#define SHPIEXTENDEDSELECT_H

#include "Fdo.h"
#include "SHP/IScrollableFeatureReader.h"
#include "SHP/ShpCompareHandler.h"

class ShpIExtendedSelect : public FdoIExtendedSelect
{		
	
public:
	// Expose the FdoISelect hidden functions
	using FdoISelect::GetOrderingOption;
	using FdoISelect::SetOrderingOption;
	// Expose the FdoIExtendedSelect hidden function
	using FdoIExtendedSelect::ExecuteScrollable;
	using FdoIExtendedSelect::GetOrderingOption;
	using FdoIExtendedSelect::SetOrderingOption;

	/// <summary>Set the compare handler. This method is used to override the default Shp compare handler.</summary>
    /// <param name="handler">The compare handle that should be derived from the ShpCompareHandler class.</param> 
    /// <returns>Returns nothing</returns> 
	virtual void SetCompareHandler( ShpCompareHandler*  handler ) = 0;
};

#endif

