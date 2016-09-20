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

#pragma  once

/// \cond DOXYGEN-IGNORE
BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES
/// \endcond

/// \ingroup (OSGeoFDOConnectionsCapabilities)

/// \brief
/// The FdoFunctionCategoryType enumeration lists all types an function may
/// be classified to.
public enum class FunctionCategoryType
{

    /// Represents the identifier used to categorize aggregate functions.
    FunctionCategoryType_Aggregate = FdoFunctionCategoryType_Aggregate,

    /// Represents the identifier used to categorize conversion functions.
    FunctionCategoryType_Conversion = FdoFunctionCategoryType_Conversion,

    /// Represents the identifier used to categorize custom functions.
    FunctionCategoryType_Custom = FdoFunctionCategoryType_Custom,

    /// Represents the identifier used to categorize date functions.
    FunctionCategoryType_Date = FdoFunctionCategoryType_Date,

    /// Represents the identifier used to categorize geometry functions.
    FunctionCategoryType_Geometry = FdoFunctionCategoryType_Geometry,

    /// Represents the identifier used to categorize mathematic functions.
    FunctionCategoryType_Math = FdoFunctionCategoryType_Math,

    /// Represents the identifier used to categorize numeric functions.
    FunctionCategoryType_Numeric = FdoFunctionCategoryType_Numeric,

    /// Represents the identifier used to categorize string functions.
    FunctionCategoryType_String = FdoFunctionCategoryType_String,

    /// Represents the identifier used to categorize unspecified functions.
    FunctionCategoryType_Unspecified = FdoFunctionCategoryType_Unspecified,

};  //  FunctionCategoryType

/// \cond DOXYGEN-IGNORE
END_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES
/// \endcond
