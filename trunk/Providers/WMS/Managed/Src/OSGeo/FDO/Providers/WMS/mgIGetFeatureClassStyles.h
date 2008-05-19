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

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_WMS

///<summary>
/// This command gets the styles list supportted by specified FeatureClass
/// from WMS service.
///</summary>
public __gc __interface IGetFeatureClassStyles : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommand
{
public:
    /// <summary>
    /// Sets the name of the FeatureClass in WMS service.
    /// This function is mandatory; if not specified, 
    /// execution of the command will throw exception. 
    /// </summary>
    /// <param name="name">
    /// The FeatureClass name.
    /// </param>
    /// <returns>
    /// Returns nothing.
    /// </returns> 
    __property System::Void set_FeatureClassName(System::String* name);

    /// <summary>
    /// Gets the name of FeatureClass to get the supportted styles.
    /// </summary>
    /// <returns>
    ///  Returns the name of the FeatureClass name.
    /// </returns> 
    __property System::String* get_FeatureClassName();

    /// <summary>
    /// Executes the IGetFeatureClassStyles command and returns a StringCollection
    /// which contains all styles supported by specified feature class
    /// and all styles supported by its ancestors. WMS services support Style 
    /// declarations inherited by child Layers. 
    /// </summary>
    /// <returns>
    /// Returns a string collection of styles supportted by specific featureclass.
    /// </returns>
    NAMESPACE_OSGEO_COMMON::StringCollection* Execute();
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_WMS
