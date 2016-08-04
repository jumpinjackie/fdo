/*
* Copyright (C) 2004-2011  Autodesk, Inc.
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
BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION
/// \endcond

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// The JoinType enumeration defines the join types available. This
/// enumeration is used in the capabilities metadata.
public enum class JoinType
{
    /// Default join type. This value can be returned by 
    /// FdoIConnectionCapabilities::GetJoinTypes()
    /// in case joins are not supported
    /// rather than provider throw an exception can return this value
    /// in case join are not  supported (this is like NAN for doubles)
	JoinType_None = FdoJoinType_None,

    /// inner join
	JoinType_Inner = FdoJoinType_Inner,

    /// right outer join
	JoinType_RightOuter = FdoJoinType_RightOuter,

    /// left outer join
	JoinType_LeftOuter = FdoJoinType_LeftOuter,

    /// full outer join
	JoinType_FullOuter = FdoJoinType_FullOuter,

    /// cross join (cartesian join)
    /// can be used for cases like SELECT * FROM A, B
	JoinType_Cross = FdoJoinType_Cross
};

/// \cond DOXYGEN-IGNORE
END_NAMESPACE_OSGEO_FDO_EXPRESSION
/// \endcond
