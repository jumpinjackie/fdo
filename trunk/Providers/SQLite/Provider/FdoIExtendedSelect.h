// 
//  
//  Copyright (C) 2008 Autodesk Inc.
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
//  

#ifndef FDOIEXTENDEDSELECT_H
#define FDOIEXTENDEDSELECT_H

class FdoIScrollableFeatureReader;

class FdoIExtendedSelect : public FdoISelect
{
public:
    // Expose the FdoISelect hidden functions
    using FdoISelect::GetOrderingOption;
    using FdoISelect::SetOrderingOption;

    virtual void                SetOrderingOption( FdoString* propertyName, FdoOrderingOption  option ) = 0;
    virtual FdoOrderingOption   GetOrderingOption( FdoString* propertyName ) = 0;
    virtual void                ClearOrderingOptions( ) = 0;

   // virtual void SetCompareHandler( SdfCompareHandler*  handler ) = 0;

    virtual FdoIScrollableFeatureReader* ExecuteScrollable() = 0;

    //virtual FdoIScrollableFeatureReader* ExecuteScrollable( const wchar_t* sdfCacheFile, FdoDataPropertyDefinitionCollection* extendedProps, FdoPropertyValueCollection*  extendedDefaults ) = 0;
};

#endif
