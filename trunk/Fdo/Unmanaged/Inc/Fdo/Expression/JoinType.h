#ifndef _JOINTYPE_H_
#define _JOINTYPE_H_
// 

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

#ifdef _WIN32
#pragma once
#endif

/// Join type enumeration used to specify the join type
enum FdoJoinType
{
    /// Default join type. This value should be returned by 
    /// FdoIConnectionCapabilities::GetJoinTypes() if joins are not supported by the provider.
    /// Rather than provider throw an exception can return this value
    /// in case join are not  supported (this is like NAN for doubles)
    FdoJoinType_None = 0x00,
    /// inner join
    FdoJoinType_Inner = 0x01,
    /// right outer join
    FdoJoinType_RightOuter = 0x02,
    /// left outer join
    FdoJoinType_LeftOuter = 0x04,
    /// full outer join
    FdoJoinType_FullOuter = 0x08,
    /// cross join (cartesian join)
    /// can be used for cases like SELECT * FROM A, B
    FdoJoinType_Cross = 0x10
};

#endif
