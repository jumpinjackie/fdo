// ExerciseFdoData.cpp
// 
//
// 
// Copyright (C) 2007  Autodesk, Inc.
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
//

#include "stdafx.h"
#include "ExerciseFdoData.h"

ExerciseFdoData::ExerciseFdoData(FdoIConnection * connection,
								   ExerciseFdoCapabilities * capabilities) {
	this->connection = connection;
	this->capabilities = capabilities;
	if (capabilities->SupportsSelect()) {
		selectCmd = (FdoISelect *)connection->CreateCommand(FdoCommandType_Select);
	}
	if (capabilities->SupportsInsert()) {
		insertCmd = (FdoIInsert *)connection->CreateCommand(FdoCommandType_Insert);
	}
	if (capabilities->SupportsDelete()) {
		deleteCmd = (FdoIDelete *)connection->CreateCommand(FdoCommandType_Delete);
	}
	if (capabilities->SupportsUpdate()) {
		updateCmd = (FdoIUpdate *)connection->CreateCommand(FdoCommandType_Update);
	}
}

FdoIFeatureReader *
ExerciseFdoData::query(const wchar_t * className, const wchar_t * filter) {
	// point the select command at the target FdoFeatureClass SampleFeatureClass
	selectCmd->SetFeatureClassName(className);
	// point the select command at the filter
	selectCmd->SetFilter(filter);
	// execute the select command
	FdoPtr<FdoIFeatureReader> queryResults;
	queryResults = selectCmd->Execute();
	return queryResults;
}




