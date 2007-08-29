/*
* Copyright (C) 2004-2007  Autodesk, Inc.
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

class FdoWmsIGetFeatureClassCRSNames;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_WMS

///<summary>
/// This command get the CRS names list supportted by specified FeatureClass
/// from WMS service.
///</summary>
private __gc class IGetFeatureClassCRSNamesImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, public NAMESPACE_OSGEO_FDO_PROVIDERS_WMS::IGetFeatureClassCRSNames
{
public:
    ///<summary>
    /// Executes the command. An exception is thrown if the feature class is NULL.
    ///</summary>
    /// <returns>Returns a list of CRS names supportted by specific featureclass.</returns>
    NAMESPACE_OSGEO_COMMON::StringCollection* Execute();
    
    /// <summary>
    /// Sets the name of the FeatureClass to get all supported CRS names.
    /// This function is mandatory; if not specified, 
    /// execution of the command will throw exception. 
    /// </summary>
    /// <param name="name">Input the FeatureClass name.</param> 
    /// <returns>Returns nothing.</returns> 
    System::Void set_FeatureClassName(System::String* value);
    
    /// <summary>
    /// Gets the name of the FeatureClass to get all supported CRS names.
    /// </summary>
    /// <returns>Returns the FeatureClass name.</returns> 
    System::String* get_FeatureClassName();

	System::Void Dispose();

public private:
	IGetFeatureClassCRSNamesImp(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoWmsIGetFeatureClassCRSNames* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_WMS
