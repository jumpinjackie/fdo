//ExerciseFdoConnection.cpp
//
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
//

#include "stdafx.h"
#include "ExerciseFdoConnection.h"

ExerciseFdoConnection::ExerciseFdoConnection(Logger * log,
										   Conversions * convert,
										   ProviderConfigInfo * configInfo,
										   IProviderRegistry * registry,
										   IConnectionManager * connectionManager,
										   FdoIConnection * connection) {
	this->log = log;
	this->convert = convert;
	this->configInfo = configInfo;
	this->registry = registry;
	this->connectionManager = connectionManager;
	this->connection = connection;
}


int
ExerciseFdoConnection::ConnectToProvider(FdoString * providerMenuName) {
	FdoString * providerInternalName = MapProviderMenuNameToInternalName(providerMenuName);
	if (providerInternalName == NULL) {
		return 1;
	}
	FdoPtr<FdoIConnectionInfo> connectionInfo;
	FdoPtr<FdoIConnectionPropertyDictionary> connectionPropertyDictionary;
	Dialog * connectDialog = new Dialog();
	FdoConnectionState connectState;
	int retval = 0;
	try {
		connection = connectionManager->CreateConnection(providerInternalName);
		connectionInfo = connection->GetConnectionInfo();
		connectionPropertyDictionary = connectionInfo->GetConnectionProperties();
		retval = GetConnectionPropertyValues(connectionPropertyDictionary, connectDialog);
		if (retval == 0) {
			connectState = connection->Open();
			switch (connectState) {
				case FdoConnectionState_Busy: break;
				case FdoConnectionState_Closed: break;
				case FdoConnectionState_Open : break;
				case FdoConnectionState_Pending : 
					retval = GetConnectionPropertyValues(connectionPropertyDictionary, connectDialog);
					if (retval == 0) {
						connectState = connection->Open();
					}
					break;
				default : 
					FdoException * ex = FdoException::Create(L"connection->Open() return unknown connection state");
					throw ex;
			}
		}
	}
	catch (FdoException * ex){
		// error handling
		ex->Release();
		if (connection) {
			connection->Close();
		} 
		return 1;
	}

	if (connectState != FdoConnectionState_Open) {
		// error handling
		return 1;
	}
	return 0;
}

FdoString *
ExerciseFdoConnection::MapProviderMenuNameToInternalName(FdoString * menuName) {
	try {
		const FdoProviderCollection * providers = registry->GetProviders();
		FdoPtr<FdoProvider> provider;
		FdoString * providerInternalName = NULL;
		FdoInt32 providerCount = providers->GetCount();
		for(int i = 0; i < providerCount; i++) {
			provider = providers->GetItem(i);
#ifdef _WIN32
			if (_wcsicmp(menuName,
#else
			if (wcscasecmp(menuName,
#endif
				provider->GetDisplayName()) == 0) {
				providerInternalName = provider->GetName();
				break;
			}
		}
		if (providerInternalName == NULL) {
			// error handling
			return NULL;
		} else {
			return providerInternalName;
		}
	}
	catch (FdoException* ex) {
		// exception handling
		ex->Release();
		return NULL;
	}
}

int
ExerciseFdoConnection::GetConnectionPropertyValues(FdoIConnectionPropertyDictionary
		*dictionary, Dialog * dialog) {
	int retval = 0;
	// get the list of property names in the dictionary
	FdoString ** propertyNames = NULL;
	FdoInt32 nameCount = 0;
	propertyNames = dictionary->GetPropertyNames(nameCount);
	FdoString * propertyName = NULL;

	// loop through the property names adding each property to the
	// dialog
	for(int i = 0; i < nameCount; i++) {
		// get the property name
		propertyName = propertyNames[i];

		// get the label to be used for the property input line in the
		// dialog
		FdoString * propertyLabel = dictionary->
			GetLocalizedName(propertyName);

		// determine whether to make the entry line required or optional
		bool IsRequired = dictionary->
			IsPropertyRequired(propertyName);

		// determine whether the user input has to be handled in a
		// secure way
		bool IsProtected = dictionary->
			IsPropertyProtected(propertyName);

		// get the actual and default values for the property
		// these could be the empty string, that is, (FdoString *)””
		FdoString * actualValue = dictionary->
			GetProperty(propertyName);
		FdoString * defaultValue = dictionary->
			GetPropertyDefault(propertyName);

		// determine whether the property values are enumerable
		bool IsEnumerable = dictionary->
			IsPropertyEnumerable(propertyName);

		FdoString ** EnumeratedValues = NULL;
		FdoInt32 numValues = 0;
		if (IsEnumerable) {
			// get the list of valid values
			EnumeratedValues = dictionary->EnumeratePropertyValues
				(propertyName, numValues);
		}

		// the dictionary entry for this property could possibly be
		// populatetd by a subsequent call to the Open() method
		bool greyOut = false;
		if (IsEnumerable && numValues == 0) {
			greyOut = true;
		}

		// the values are enumerable and there is only one
		else if (IsEnumerable && numValues == 1) {
			// add the line to the dialog,
			// setting the spin box value to EnumeratedValues[0]
		}

		// the values are enumerable and there is more than one
		else if (IsEnumerable && numValues > 1) {
			// add the line to the dialog,
			// setting the spin box value to the actualValue if not empty,
			// or setting it to the defaultValue if valid and not empty,
			// or setting it to one of the enumerated values
		}

		// set the field to the actual value if not empty
		else if ( wcscmp(actualValue, L"") != 0) {
			// add line to dialog
		}

		// set the field to whatever is the default value
		else {
			// add line to dialog
		}
	}

	// blocks until user clicks ok or cancel in dialog
	// returns 0 if user clicks ok and all required input is there and
	// valid
	// if user doesn't fill in required fields, dialog persists until user does so
	// or presses cancel
	// returns positive if user cancels
	retval = dialog->expose();
	if (retval == 0) {
		FdoString * value = NULL;
		for(int i = 0; i < nameCount; i++) {
			// get the property name
			propertyName = propertyNames[i];
			
			// get the value input by the user for this property
			value = dialog->GetValue(propertyName);

			dictionary->SetProperty(propertyName, value);
		}
	}
	return retval;
}

void
ExerciseFdoConnection::PopulateConnectionMenu(Menu * connectMenu) {
	const FdoProviderCollection * providers;
	FdoPtr<FdoProvider> provider;
	try {
		providers = registry->GetProviders ();
		FdoInt32 providerCount = providers->GetCount();
		FdoString * providerDisplayName = NULL;
		for (int i = 0; i < providerCount; i++) {
			provider = providers->GetItem (i);
			providerDisplayName = provider->GetDisplayName();
			// add providerDisplayName to menu
			connectMenu->Add(providerDisplayName);
		}
	}
	catch (FdoException* ex) {
	// exception handling
	ex->Release();
	}
}


// loop through the connection properties
// using the information gathered from there to construct a
// connection dialog to present to the user for input
// use the GetLocalizedName method to get the locale-specifc name
// for use as a label for the entry field in the dialog
// use the IsPropertyRequired to determine whether to mark the field
// as required or optional
// if the IsPropertyEnumerable method returns true,
// use the EnumeratePropertyValues method to populate the display
// use the GetPropertyDefault method to populate the entry text
// field or list
// use the IsPropertyProtected method to determine if user input
// must be protected for security purposes
// a connection is 2-stage if the first stage retrieves a list of values that the user
// must choose one from in the second stage
void
ExerciseFdoConnection::SetConnectionString(FdoIConnectionPropertyDictionary *connectionPropertyDictionary) {
	FdoString ** connectionPropertyNames = NULL;
	FdoInt32 propertyNameCount = 0;
	connectionPropertyNames = connectionPropertyDictionary->GetPropertyNames(propertyNameCount);
	FdoString * propertyName = NULL;
	FdoString ** PropertyValueList = NULL;
	FdoInt32 enumValueCount = 0;
	bool IsRequired = false;
	bool IsProtected = false;
	bool IsEnumerable = false;
	FdoString * configValue = NULL;
	for(int i = 0; i < propertyNameCount; i++) {
		propertyName = connectionPropertyNames[i];
		IsRequired = connectionPropertyDictionary->IsPropertyRequired(propertyName);
		IsProtected = connectionPropertyDictionary->IsPropertyProtected(propertyName);
		IsEnumerable = connectionPropertyDictionary->IsPropertyEnumerable(propertyName);
		configValue = configInfo->getValue(propertyName);
		sprintf(this->msg, "Dictionary property name: %ls; config file property value: %ls", propertyName, configValue);
		log->write(this->msg);
		if (IsEnumerable) {
			enumValueCount = 0;
			PropertyValueList = connectionPropertyDictionary->EnumeratePropertyValues(propertyName, enumValueCount);
			if (enumValueCount > 0) {
				// search for configValue in enumerated value list				
				int cmpResult = -1;
				// wcsicmp converts both to lowercase and then compares
				for (int i = 0; i < enumValueCount && cmpResult != 0; i++) {
#ifdef _WIN32
					cmpResult = _wcsicmp(configValue, PropertyValueList[i]);
#else
					cmpResult = wcscasecmp(configValue, PropertyValueList[i]);
#endif
				}
				if (cmpResult == 0) {
					connectionPropertyDictionary->SetProperty(propertyName, configValue);
					log->write("Connection property value:", convert->wchar_tStarToCharStar(configValue));
				} else {
					log->write("config value not found in enumerable property value in property dictionary:", convert->wchar_tStarStarToCharStar(PropertyValueList, enumValueCount));
					FdoConnectionException * ex = FdoConnectionException::Create(L"Config value not found in enumerable property value in property dictionary");
					throw ex;
				}
			} else if (enumValueCount == 0) {
				// don't call SetProperty
				// the next call to this method will take the preceding branch
				log->write(convert->wchar_tStarToCharStar(propertyName), "is enumerable and count is 0");
			}
		} else {
				connectionPropertyDictionary->SetProperty(propertyName, configValue);
		}
	}
}


