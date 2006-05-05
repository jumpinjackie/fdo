#ifndef SHPCREATEDATASTORE_H_
#define SHPCREATEDATASTORE_H_
// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  

#ifdef _WIN32
#pragma once
#endif

#include "Fdo/Commands/DataStore/ICreateDataStore.h"
#include "SdfDataStorePropertyDictionary.h"
#include "SdfConnection.h"
#include "SdfCommand.h"

/// <summary>This command creates a new datastore.</summary>
class SdfCreateDataStore: public SdfCommand<FdoICreateDataStore>
{
	friend class SdfConnection;

protected:

	SdfCreateDataStore(SdfConnection* connection);

public:

	///<summary>Gets the FdoIDataStorePropertyDictionary interface that	
	///can be used to dynamically query	and	set	the	properties required	
	///to create a new datastore.</summary>
	///	<returns>Returns the property dictionary</returns>
	virtual	FdoIDataStorePropertyDictionary* GetDataStoreProperties() 
	{ 
		return (FdoIDataStorePropertyDictionary *)(m_dataStorePropertyDictionary);
	}

	///	<summary>Executes the FdoICreateDataStore command.</summary>
	///	<returns>Returns nothing</returns> 
	virtual	void Execute(); 

protected:
	SdfDataStorePropertyDictionary *m_dataStorePropertyDictionary;
};

#endif


