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

// =============================================================================
// | The file defines various constants for the Long Transaction Manager.
// =============================================================================


#ifndef FdoRdbmsLongTransactionConstants_h
#define FdoRdbmsLongTransactionConstants_h

#ifdef _WIN32
#pragma once
#endif

//  FDORDBMSLONGTRANSACTIONCONSTANTS_LT_NAME_MAX_LENGTH:
//      The constant defines the maximum length of a long transaction name.

#define FDORDBMSLONGTRANSACTIONCONSTANTS_LT_NAME_MAX_LENGTH   30


//  FDORDBMSLONGTRANSACTIONCONSTANTS_ROOT_LT:
//      The constant represents the identifier used to address the root long
//      transaction.

static const FdoStringP FDORDBMSLONGTRANSACTIONCONSTANTS_ROOT_LT   = L"_ROOT_";

//  FDORDBMSLONGTRANSACTIONCONSTANTS_ACTIVE_LT:
//      The constant represents the identifier used to address the active long
//      transaction if the name is not known or to have a general addressing
//      method independent of names.

static const FdoStringP FDORDBMSLONGTRANSACTIONCONSTANTS_ACTIVE_LT = L"_ACTIVE_";

#endif

