// ExerciseFdoData.h

#ifndef _EXERCISEFDODATA_H_
#define _EXERCISEFDODATA_H_
//
// Copyright (C) 2008  Autodesk, Inc.
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

#include <Fdo.h>
#include "ExerciseFdoCapabilities.h"

class ExerciseFdoData {
public:
	ExerciseFdoData(FdoIConnection * connection, ExerciseFdoCapabilities * capabilities);

	FdoIFeatureReader * query(const wchar_t * className, const wchar_t * filter);

private:
	FdoIConnection * connection;
	ExerciseFdoCapabilities * capabilities;
	FdoPtr<FdoISelect> selectCmd;
	FdoPtr<FdoIInsert> insertCmd;
	FdoPtr<FdoIUpdate> updateCmd;
	FdoPtr<FdoIDelete> deleteCmd;

};

#endif

