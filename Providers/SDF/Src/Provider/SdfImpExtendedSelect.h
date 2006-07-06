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
#ifndef _SdfImpExtendedSelect_H_
#define _SdfImpExtendedSelect_H_

#include "SdfSelect.h"
#include "SDF\SdfCompareHandler.h"
#include "SDF\IScrollableFeatureReader.h"

class PropertyIndex;

class SdfImpExtendedSelect : public SdfSelect
{

    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

    public:
        // constructs a Select command using the specified connection
        SDF_API SdfImpExtendedSelect(SdfConnection* connection);

    protected:
        // default constructor
        SdfImpExtendedSelect();
        // default destructor
        virtual ~SdfImpExtendedSelect();

    //-------------------------------------------------------
    // FdoISelect implementation
    //-------------------------------------------------------

    public:

		// Expose the FdoISelect hidden functions
		using SdfSelect::GetOrderingOption;
		using SdfSelect::SetOrderingOption;

		// Executes the select command and returns a reference to an SdfIScrollableFeatureReader.
        virtual SdfIScrollableFeatureReader* ExecuteScrollable( );

        // Executes the select command and returns a reference to an SdfIScrollableFeatureReader.
        virtual SdfIScrollableFeatureReader* ExecuteScrollable( const wchar_t* sdfCacheFile, FdoDataPropertyDefinitionCollection* extendedProps, FdoPropertyValueCollection*  extendedDefaults );

		// This method takes a source Fdo feature reader, caches the content to an sdfCacheFile and returns a SdfIScrollableFeatureReader.
        virtual SdfIScrollableFeatureReader* ExecuteScrollable( FdoIFeatureReader* reader, const wchar_t* sdfCacheFile, FdoDataPropertyDefinitionCollection* extendedProps, FdoPropertyValueCollection*  extendedDefaults );

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

		/// <summary>Set the compare handler. This method is used to override the default sdf compare handler.</summary>
        /// <param name="handler">The compare handle that should be derived from the SdfCompareHandler class.</param> 
        /// <returns>Returns nothing</returns> 
        virtual void SetCompareHandler( SdfCompareHandler*  handler );

    private:
		
		SdfConnection* CreateCacheFile( FdoClassDefinition* fdoclass, const wchar_t* sdfCacheFile  );

		void BuildCacheFile( SdfConnection* conn, FdoClassDefinition* fdoclass, FdoIFeatureReader* reader, FdoPropertyValueCollection* defaultPvc );

    //-------------------------------------------------------
    // Variables
    //-------------------------------------------------------

    private:
		FdoPtr<FdoIdentifierCollection> m_orderingProperties;

        FdoOrderingOption m_orderingOption;

		FdoPtr<SdfCompareHandler> m_compareHandler;

		std::map<std::wstring, int>* m_orderingOptions;

};

#endif
