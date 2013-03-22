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
// | The file contains the definition of the class FdoRdbmsLockInfoQueryHandler.
// | The class defines the interfaces and services the Lock Info Query handler
// | provides.
// =============================================================================


#ifndef FdoRdbmsLockConflictQueryHandler_h
#define FdoRdbmsLockConflictQueryHandler_h

#ifdef _WIN32
#pragma once
#endif

#include "stdafx.h"
#include "Disposable.h"
#include "LockManager.h"

class FdoRdbmsLockConflictQueryHandler : public FdoLkDisposable
{
public:
	

	virtual       bool               ReadNext                   () = 0;
	virtual		void					Close ()	=0;
    //virtual       long            GetLockConflictCount       () = 0;
    virtual FdoString                  *GetFeatureClassName       () = 0;
    virtual FdoString                  *GetLockOwner              () = 0;
    virtual FdoString                  *GetLongTransaction        () = 0;
    virtual FdoConflictType            GetConflictType            () = 0;
    virtual FdoPropertyValueCollection *GetIdentity               () = 0;
	virtual	FdoString				   *GetConflictDbObject		   () = 0;
	virtual bool						IsTableEmpty			  () = 0;

};

#endif

