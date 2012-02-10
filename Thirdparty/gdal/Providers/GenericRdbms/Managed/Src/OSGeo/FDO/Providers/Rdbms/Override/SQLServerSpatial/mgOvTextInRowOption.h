//
// Copyright (C) 2004-2008  Autodesk, Inc.
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

#pragma once

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL

/// <summary>Enumerated type representing the possible text-in-row options for SQL Server overrides</summary>
/// <param name="OvTextInRowOption_InRow">Text, ntext and image data are stored within the row's 8K page.</param>
/// <param name="OvTextInRowOption_NotInRow">Text, ntext and image data are not stored within the row's 8K page.</param>
/// <param name="OvTextInRowOption_Default">The text-in-row override is inherited from somewhere else.</param>
public enum class OvTextInRowOption 
{
	OvTextInRowOption_InRow = SqlServerOvTextInRowOption_InRow,
    OvTextInRowOption_NotInRow = SqlServerOvTextInRowOption_NotInRow,
	OvTextInRowOption_Default = SqlServerOvTextInRowOption_Default
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_SQLSERVERSPATIAL
