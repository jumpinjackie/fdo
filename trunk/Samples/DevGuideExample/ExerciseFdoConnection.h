// ExerciseFdoConnection.h

#ifndef _EXERCISEFDOCONNECTION_H_
#define _EXERCISEFDOCONNECTION_H_
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

#include <FdoCommon.h>
#include <iostream>
#include <string>
#include "Logger.h"
#include "fdoinfo.h"
#include "conversions.h"
#include "ProviderConfigInfo.h"
#include <exception>
#include "Dialog.h"
#include "Menu.h"


class ExerciseFdoConnection {
public:
	ExerciseFdoConnection(
		Logger * log,
		Conversions * convert,
		ProviderConfigInfo * configInfo,
		IProviderRegistry * registry,
		IConnectionManager * connectionManager,
		FdoIConnection * connection);
   void SetConnectionString(FdoIConnectionPropertyDictionary * connectionPropertyDictionary);
   int GetConnectionPropertyValues(FdoIConnectionPropertyDictionary
		*dictionary, Dialog * connectDialog);
   void PopulateConnectionMenu(Menu * connectMenu);
   FdoString * MapProviderMenuNameToInternalName(FdoString * menuName);
   int ConnectToProvider(FdoString * providerMenuName);

private:
   Logger * log;
   Conversions * convert;
   ProviderConfigInfo * configInfo;

	IProviderRegistry  * registry;
	IConnectionManager * connectionManager;
	FdoIConnection * connection;

	char msg[256];

};

#endif

