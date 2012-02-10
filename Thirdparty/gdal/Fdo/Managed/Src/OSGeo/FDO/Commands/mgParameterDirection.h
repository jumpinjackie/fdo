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

/// \cond DOXYGEN-IGNORE
BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS
/// \endcond

/// \ingroup (OSGeoFDOCommands)
/// \brief
/// The ParameterDirection enumeration defines the direction in which a
/// parameter value will be used within the context of an FDO Command. Parameter 
/// direction values are typically Input, Output, InputOutput and Return. 
/// The default value is FdoParameterDirection_Input.
public enum class ParameterDirection{
    // Specifies that the parameter is an input parameter.
    ParameterDirection_Input = FdoParameterDirection_Input,

    // Specifies that the parameter is an output parameter.
    ParameterDirection_Ouput = FdoParameterDirection_Output,

    // Specifies that the parameter is an input & output parameter.
    ParameterDirection_InputOutput = FdoParameterDirection_InputOutput,

    // Specifies that the parameter is a return parameter.
    ParameterDirection_Return = FdoParameterDirection_Return
};

/// \cond DOXYGEN-IGNORE
END_NAMESPACE_OSGEO_FDO_COMMANDS
/// \endcond


