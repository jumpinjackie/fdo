#ifndef FDOSMOBJECTSTATE_H
#define FDOSMOBJECTSTATE_H		1
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

#ifdef _WIN32
#pragma once
#endif

// Describes the initialization state of a schema object.
enum FdoSmObjectState
{
    /// Object is in initial (uninitialized) state
    FdoSmObjectState_Initial,

    /// Object is currently being finalized
    FdoSmObjectState_Finalizing,

    /// Object is final ( fully initialized ).
    FdoSmObjectState_Final

};

#endif


