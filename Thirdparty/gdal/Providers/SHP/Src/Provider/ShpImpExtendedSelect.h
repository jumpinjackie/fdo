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
#ifndef _ShpImpExtendedSelect_H_
#define _ShpImpExtendedSelect_H_

#include "ShpSelectCommand.h"
#include "SHP/ShpCompareHandler.h"
#include "SHP/IScrollableFeatureReader.h"


//class PropertyIndex;

class ShpImpExtendedSelect : public ShpSelectCommand
{

    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

    public:
        // constructs a Select command using the specified connection
        FDOSHP_API ShpImpExtendedSelect(ShpConnection* connection);

    protected:
        // default constructor
        ShpImpExtendedSelect();
        // default destructor
        virtual ~ShpImpExtendedSelect();

    //-------------------------------------------------------
    // FdoISelect implementation
    //-------------------------------------------------------

    public:

		// Expose the FdoISelect hidden functions
		using ShpSelectCommand::GetOrderingOption;
		using ShpSelectCommand::SetOrderingOption;

		// Executes the select command and returns a reference to an ShpIScrollableFeatureReader.
        virtual ShpIScrollableFeatureReader* ExecuteScrollable( );

        /// <summary>Gets the FdoIdentifierCollection that holds the list of order by property names. If empty no ordering is used. This list is initially
        /// empty and the caller need to add the property that the command should use as a order by criteria.</summary>
        /// <returns>Returns the list of group by property names.</returns> 
        virtual FdoIdentifierCollection* GetOrdering();

        /// <summary>Set the ordering option of the selection. This is only used if the ordering collection is not empty.</summary>
		/// <param name="propertyName">Is the property name for which the ordering should be applied. This property should be in the ordering collection.</param> 
        /// <param name="option">Is the ordering option and should be set to one of FdoOrderingOption_Ascending or FdoOrderingOption_Descending.
        /// FdoOrderingOption_Ascending is the default value.</param> 
        /// <returns>Returns nothing</returns> 
        virtual void SetOrderingOption( FdoString* propertyName, FdoOrderingOption  option );

        /// <summary>Gets the ordering option for a given property.</summary>
		/// <param name="propertyName">Is the property name for which the ordering should be applied. This property should be in the ordering collection.</param> 
        /// <returns>Returns the ordering option.</returns> 
        virtual FdoOrderingOption GetOrderingOption( FdoString* propertyName );

		virtual void ClearOrderingOptions();

		/// <summary>Set the compare handler. This method is used to override the default shp compare handler.</summary>
        /// <param name="handler">The compare handle that should be derived from the ShpCompareHandler class.</param> 
        /// <returns>Returns nothing</returns> 
        virtual void SetCompareHandler( ShpCompareHandler*  handler );

    private:
		
//		ShpConnection* CreateCacheFile( FdoClassDefinition* fdoclass, const wchar_t* sdfCacheFile  );


    //-------------------------------------------------------
    // Variables
    //-------------------------------------------------------

    private:
		FdoPtr<FdoIdentifierCollection> m_orderingProperties;

        FdoOrderingOption m_orderingOption;

		FdoPtr<ShpCompareHandler> m_compareHandler;

		std::map<std::wstring, int>* m_orderingOptions;

};

#endif
