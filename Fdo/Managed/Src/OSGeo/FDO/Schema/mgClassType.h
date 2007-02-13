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

#pragma  once

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA

/// \brief
/// The ClassType enumeration lists all of the concrete class types defined in
/// the Schema package.
/// <param name="ClassType_Class">
/// Represents the concrete class type Class
/// </param>
/// <param name="ClassType_FeatureClass">
/// Represents the concrete class type FeatureClass
/// </param>
/// <param name="ClassType_NetworkClass">
/// Represents the concrete class type NetworkClass
/// </param>
/// <param name="ClassType_NetworkLayerClass">
/// Represents the concrete class type NetworkLayerClass
/// </param>
/// <param name="ClassType_NetworkNodeClass">
/// Represents the concrete class type NetworkNodeClass
/// </param>
/// <param name="ClassType_NetworkLinkClass">
/// Represents the concrete class type NetworkLinkClass
/// </param>
public __value enum ClassType
{
    /// Represents the concrete class type Class.
	ClassType_Class = FdoClassType_Class,

    /// Represents the concrete class type FeatureClass.
	ClassType_FeatureClass = FdoClassType_FeatureClass,

    /// Represents the concrete class type NetworkClass.
	ClassType_NetworkClass = FdoClassType_NetworkClass,

    /// Represents the concrete class type NetworkLayerClass.
	ClassType_NetworkLayerClass = FdoClassType_NetworkLayerClass,

    /// Represents the concrete class type NetworkNodeClass.
	ClassType_NetworkNodeClass = FdoClassType_NetworkNodeClass,

    /// Represents the concrete class type NetworkLinkClass.
	ClassType_NetworkLinkClass = FdoClassType_NetworkLinkClass
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


