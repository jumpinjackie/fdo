// ExerciseFdoApi.cpp
// a program to exercise the FDO API
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
#include <iostream>
#include <string>
#include "Logger.h"
#include "fdoinfo.h"
#include "conversions.h"
#include "ProviderConfigInfo.h"
#include <exception>
#include "ExerciseFdoUtilities.h"
#include "ExerciseFdoConnection.h"
#include "ParseSchemaDefinitionFile.h"
#include "ParseSchemaInsertionFile.h"
#include "ParseSchemaFilterFile.h"
#include "strToFdoType.h"
#include "ExerciseFdoSchema.h"
#include "ExerciseFdoCapabilities.h"

using namespace std;
int
main(int argc, char *argv[]) {
	bool doSampleFeatureSchemaBool = false;
	bool doCapabilitiesBool = false;
	bool doPartialSchemaBool = false;
	bool doXmlSchemaBool = false;
	bool undoXmlSchemaBool = false;
	bool doInsertDeleteBool = false;

	if (argc < 9) {
		printf("Usage: ExerciseFdoApi <provider>.data <logFileName> doSampleFeatureSchema=<T|F> doCapabilities=<T|F> doPartialSchema=<T|F> doXmlSchema=<T|F> undoXmlSchema=<T|F> doInsertDelete=<T|F>");
		exit(1);
	}


   char * configFilename = argv[1];
   char * logFilename = argv[2];
   int retval = 0;

   Logger * log;
   try {
	   log = new Logger(logFilename);
   } catch (char * str) {
	   cout << str << endl;
	   return 1;
   }

   	// remember argv[0] has program name in it
   log->write("argv[1]:", configFilename);
   log->write("argv[2]:", logFilename);

   char * doSampleFeatureSchemaStr = argv[3];
   log->write("argv[3]:", doSampleFeatureSchemaStr);
   char * name = strtok(doSampleFeatureSchemaStr, "=");
   log->write("name:", name);
   char * value = strtok((char*)NULL, "=");
   log->write("value:", value);
   if (strcmp(name, "doSampleFeatureSchema") == 0) {
	   if (strcmp(value, "T") == 0) {
			   doSampleFeatureSchemaBool = true;
	   }
   }

   char * doCapabilitiesStr = argv[4];
   log->write("argv[4]:", doCapabilitiesStr);
   name = strtok(doCapabilitiesStr, "=");
   log->write("name:", name);
   value = strtok((char*)NULL, "=");
   log->write("value:", value);
   if (strcmp(name, "doCapabilities") == 0) {
	   if (strcmp(value, "T") == 0) {
			   doCapabilitiesBool = true;
	   }
   }

   char * doPartialSchemaStr = argv[5];
   log->write("argv[5]:", doPartialSchemaStr);
   name = strtok(doPartialSchemaStr, "=");
   log->write("name:", name);
   value = strtok((char*)NULL, "=");
   log->write("value:", value);
   if (strcmp(name, "doPartialSchema") == 0) {
	   if (strcmp(value, "T") == 0) {
			   doPartialSchemaBool = true;
	   }
   }

   char * doXmlSchemaStr = argv[6];
   log->write("argv[6]:", doXmlSchemaStr);
   name = strtok(doXmlSchemaStr, "=");
   log->write("name:", name);
   value = strtok((char*)NULL, "=");
   log->write("value:", value);
   if (strcmp(name, "doXmlSchema") == 0) {
	   if (strcmp(value, "T") == 0) {
			   doXmlSchemaBool = true;
	   }
   }

   char * undoXmlSchemaStr = argv[7];
   log->write("argv[7]:", undoXmlSchemaStr);
   name = strtok(undoXmlSchemaStr, "=");
   log->write("name:", name);
   value = strtok((char*)NULL, "=");
   log->write("value:", value);
   if (strcmp(name, "undoXmlSchema") == 0) {
	   if (strcmp(value, "T") == 0) {
			   undoXmlSchemaBool = true;
	   }
   }

   char * doInsertDeleteStr = argv[8];
   log->write("argv[8]:", doInsertDeleteStr);
   name = strtok(doInsertDeleteStr, "=");
   log->write("name:", name);
   value = strtok((char*)NULL, "=");
   log->write("value:", value);
   if (strcmp(name, "doInsertDelete") == 0) {
	   if (strcmp(value, "T") == 0) {
			   doInsertDeleteBool = true;
	   }
   }

   char sprintfBuf[1024];

   // declarations used throughout the program
   FdoPtr<FdoFgfGeometryFactory> geometryFactory;
   geometryFactory = FdoFgfGeometryFactory::GetInstance();


   Conversions convert;
   ProviderConfigInfo * configInfo;
   StrToFdoType str2FdoType;

   try {
	configInfo = new ProviderConfigInfo(configFilename, log, &convert);
   }
   catch (char * errMsg) {
	   log->write("ERROR: constructing configuration file object:", errMsg);
	   return 1;
   }
   ExerciseFdoUtilities utilities(log, &convert);
   FdoInfo info(log, &convert, &utilities, geometryFactory);
   
   FdoString* providerName;
   FdoPtr<IProviderRegistry> registry;
   const FdoProviderCollection * providers;
   const FdoProvider * provider;
   bool containsProvider = false;
   FdoInt32 providerIndex = -1;

   // verity that the provider named in the config info file is already registered
   // log the entire registry
   try {
	   registry = FdoFeatureAccessManager::GetProviderRegistry();
	   providers = registry->GetProviders();
	   log->write("Complete Registry");
	   for (int i = 0; i < providers->GetCount(); i++) {
		   provider = providers->GetItem(i);
		   info.LogProviderInfo(provider);
	   }
	   providerName = configInfo->getName();
	   sprintf(sprintfBuf, "config file provider name: \"%s\"", convert.wchar_tStarToCharStar(providerName));
	   log->write(sprintfBuf);
	   containsProvider = providers->Contains(providerName);
	   log->write("Providers list contains provider:", convert.boolToCharStar(containsProvider));
	   if (containsProvider) {
		   providerIndex = providers->IndexOf(providerName);
		   log->write("The index of the provider in the providers list is:", convert.FdoInt32ToCharStar(providerIndex));
		   provider = providers->GetItem(providerIndex);
		   info.LogProviderInfo(provider);
	   } else {
		   log->write("Registry does not contain provider:", convert.wchar_tStarToCharStar(configInfo->getName()));
		   return 1;
	   }
   }
   catch (FdoException * ex) {
	   log->write("Registration checkpoint exception message:");
	   utilities.PrintException(ex);
		ex->Release();
   }

   // create a connection object
   // use the connection object's connection property dictionary to set the connection string
   ExerciseFdoConnection * connectUtilities;
   FdoPtr<FdoIConnection> connection;
   FdoPtr<FdoIConnectionInfo> connectionInfo;
   static IConnectionManager* connectionManager;
   FdoIConnectionPropertyDictionary * connectionPropertyDictionary;
   try {
	   connectionManager = FdoFeatureAccessManager::GetConnectionManager();
 	   connection = connectionManager->CreateConnection(providerName);
	   connectionInfo = connection->GetConnectionInfo();
	   info.LogConnectionInfo(connectionInfo);
	   connectUtilities = new ExerciseFdoConnection(log, &convert, configInfo, registry,
		   connectionManager, connection);
	   connectionPropertyDictionary = connectionInfo->GetConnectionProperties();
	   connectUtilities->SetConnectionString(connectionPropertyDictionary);
	   log->write("connection->GetConnectionString():", convert.wchar_tStarToCharStar(connection->GetConnectionString()));
   } catch(FdoException* ex) {
	   log->write("Setting connection string exception message:");
	   utilities.PrintException(ex);
		ex->Release();
	   return 1;
   }

   // some of the schema management calls are in this object
   ExerciseFdoSchema exerciseSchema(&str2FdoType, &utilities, connection);


   FdoConnectionState connectionState;
   try {
	   connectionState = connection->Open();
	   switch (connectionState) {
		case FdoConnectionState_Busy : 
			log->write("connection->Open() returned Busy:");
			return 1;
		case FdoConnectionState_Closed : 
			log->write("connection->Open() returned Closed:");
			return 1;
		case FdoConnectionState_Open : 
			break;
		case FdoConnectionState_Pending : 
			connectionInfo = connection->GetConnectionInfo();
			info.LogConnectionInfo(connectionInfo);
			connectionPropertyDictionary = connectionInfo->GetConnectionProperties();
			connectUtilities->SetConnectionString(connectionPropertyDictionary);
		   log->write("Connection string after setting datastore property:", convert.wchar_tStarToCharStar(connection->GetConnectionString()));
		   // the effect of the first Open() is to set the list of named datastores in the datastore property in the property dictionary
		   // get the datastore value from the config info and make sure it's in the list of named datastores
		   connectionState = connection->Open();
		   log->write("Connection State after second call to Open():", info.GetConnectionStateName(connectionState));
		   if (connectionState != FdoConnectionState_Open) {
			   log->write("Connection failed");
			   return 1;
		   }
			break;
		default :
			log->write("connection->Open() returned unknown enum:", convert.FdoInt32ToCharStar((FdoInt32)connectionState));
			return 1;
	   }
	   log->write("Connection State:", info.GetConnectionStateName(connectionState));
   }
   catch (FdoException* ex) {
	   log->write("Open connection checkpoint exception message:");
	   utilities.PrintException(ex);
		ex->Release();
	   return 1;
   }

   ExerciseFdoCapabilities * exerciseCapabilities = new ExerciseFdoCapabilities(connection);

   if (doCapabilitiesBool) {
	   FdoIConnectionCapabilities * connectionCapabilities;
	   FdoISchemaCapabilities * schemaCapabilities;
	   FdoICommandCapabilities * commandCapabilities;
	   FdoIExpressionCapabilities * expressionCapabilities;
	   FdoIFilterCapabilities * filterCapabilities;
	   FdoIGeometryCapabilities * geometryCapabilities;
	   FdoIRasterCapabilities * rasterCapabilities;
	   FdoITopologyCapabilities * topologyCapabilities;
	   try {
		   connectionCapabilities = connection->GetConnectionCapabilities();
		   log->write("Connection Capabilities");
		   info.LogConnectionCapabilities(connectionCapabilities);	
		   schemaCapabilities = connection->GetSchemaCapabilities();
		   log->write("Schema Capabilities");
		   info.LogSchemaCapabilities(schemaCapabilities);	
		   commandCapabilities = connection->GetCommandCapabilities();
		   log->write("Command Capabilities");
		   info.LogCommandCapabilities(commandCapabilities);	
		   expressionCapabilities = connection->GetExpressionCapabilities();
		   log->write("Expression Capabilities");
		   info.LogExpressionCapabilities(expressionCapabilities);	
		   filterCapabilities = connection->GetFilterCapabilities();
		   log->write("Filter Capabilities");
		   info.LogFilterCapabilities(filterCapabilities);	
		   geometryCapabilities = connection->GetGeometryCapabilities();
		   log->write("Geometry Capabilities");
		   info.LogGeometryCapabilities(geometryCapabilities);	
		   rasterCapabilities = connection->GetRasterCapabilities();
		   log->write("Raster Capabilities");
		   info.LogRasterCapabilities(rasterCapabilities);	
		   topologyCapabilities = connection->GetTopologyCapabilities();
		   // test for NULL to get around bug that exhibits itself when connected to Oracle
		   if (topologyCapabilities != NULL) {
			   log->write("Topology Capabilities");
			   info.LogTopologyCapabilities(topologyCapabilities);
		   }
	   }
	   catch (FdoException* ex) {
		   log->write("Capabilities checkpoint exception message:");
		   utilities.PrintException(ex);
			ex->Release();
	   }
   } // if (doCapabilitiesBool)

   if (doSampleFeatureSchemaBool) {
	   /*************************************************************************************
	   *	Sample Feature Schema operations
	   *	this code uses hard-coded values in method call arguments
	   *************************************************************************************/

	   // SampleFeatureSchema
	   // 1. create the commands
	   // 2. if provider supports the DestroySchema command, destroy the SampleFeatureSchema
	   // 3. if provider supports the ApplySchema command, apply the SampleFeatureSchema and write it to an xml file
	   // 4. if provider supports the DescribeSchema command, describe the schema
	   // 5. if provider supports the DestroySchema command, destroy the SampleFeatureSchema
	   // 6. if provider supports the ApplySchema command, read schema from xml file and apply it
	   // 7. create an instance of SampleFeatureClass class by inserting integer and string and polygon
	   // 8. change the value of the string in the SampleFeatureClass object
	   // 9. query the schema so it returns the SampleFeatureClass object created in step 7
	   // 10. delete the SampleFeatureClass object
	   // 11. if provider supports the DestroySchema command, destroy the SampleFeatureSchema

	   // declare the feature schema and the schema collection
	   // the latter holds the schemas returned by the FdoIDescribeSchema command
	   FdoPtr<FdoFeatureSchema> sampleFeatureSchema;
	   FdoPtr<FdoFeatureSchemaCollection> sampleFeatureSchemaCollection;


	   // create the commands for the sample schema code used in the Dev Guide chapter code	
		// the check for ArcSDE is because the debug build version of ArcSDE does support a version of ApplySchema and DestroySchema
	   // that I do not want to use
	
	   // special note for ArcSDE
	   // an FDO feature schema name corresponds to a username in Oracle
	   // Oracle is case insensitive; it returns uppercase names
	   // Fdo is case sensitive
	   // therefore, when searching through the schema collection returned by the
	   // FdoIDescribeSchema command, you must set the argument to the FindItem() method
	   // to all uppercase
	   FdoPtr<FdoIApplySchema> sampleApplySchema;
	   FdoPtr<FdoIDestroySchema> sampleDestroySchema;
	   FdoPtr<FdoIDescribeSchema> sampleDescribeSchema;
	   FdoPtr<FdoIInsert> sampleInsert;
	   FdoPtr<FdoIUpdate> sampleUpdate;
	   FdoPtr<FdoISelect> sampleSelect;
	   FdoPtr<FdoIDelete> sampleDelete;
		try {
	#ifdef _WIN32
		   if (exerciseCapabilities->SupportsApplySchema() && _wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#else
		   if (exerciseCapabilities->SupportsApplySchema() && wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#endif
			   sampleApplySchema = (FdoIApplySchema *)connection->CreateCommand(FdoCommandType_ApplySchema);
		   }
		   if (exerciseCapabilities->SupportsDescribeSchema()) {
			   sampleDescribeSchema = (FdoIDescribeSchema *)connection->CreateCommand(FdoCommandType_DescribeSchema);
		   }
	#ifdef _WIN32
		   if (exerciseCapabilities->SupportsDestroySchema() && _wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#else
		   if (exerciseCapabilities->SupportsDestroySchema() && wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#endif
			   sampleDestroySchema = (FdoIDestroySchema *)connection->CreateCommand(FdoCommandType_DestroySchema);
		   }
		   if (exerciseCapabilities->SupportsInsert()) {
			   sampleInsert = (FdoIInsert *)connection->CreateCommand(FdoCommandType_Insert);
		   }
		   if (exerciseCapabilities->SupportsUpdate()) {
			   sampleUpdate = (FdoIUpdate *)connection->CreateCommand(FdoCommandType_Update);
		   }
		   if (exerciseCapabilities->SupportsSelect()) {
			   sampleSelect = (FdoISelect *)connection->CreateCommand(FdoCommandType_Select);
		   }
		   if (exerciseCapabilities->SupportsDelete()) {
			   sampleDelete = (FdoIDelete *)connection->CreateCommand(FdoCommandType_Delete);
		   }
	   }
	   catch (FdoException* ex) {
		   log->write("Create sample commands checkpoint exception message:");
		   utilities.PrintException(ex);
		   ex->Release();
	   }

	   // destroy the schema in case it was not destroyed by previous program run
	   // if schema already exists, the FdoIApplySchema command happening next will fail
	#ifdef _WIN32
	   if (exerciseCapabilities->SupportsDestroySchema() && _wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#else
	   if (exerciseCapabilities->SupportsDestroySchema() && wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#endif
			log->write("Destroy the Schema Management chapter sample schema.");
		   try {
			   sampleDestroySchema->SetSchemaName(L"SampleFeatureSchema");
			   sampleDestroySchema->Execute();
		   }
		   catch (FdoException * ex) {
				log->write("Sample Feature Schema DestroySchema checkpoint exception message:");
				utilities.PrintException(ex);
				ex->Release();
		   }
	   }

	   FdoInt32 classCount = 0;
	   FdoPtr<FdoClassCollection> sampleClassCollection;
	   FdoPtr<FdoFeatureClass> sampleFeatureClass;
	   FdoPtr<FdoPropertyDefinitionCollection> sampleFeatureClassProperties;
	   FdoPtr<FdoDataPropertyDefinitionCollection> sampleFeatureClassIdentityProperties;
	   FdoPtr<FdoDataPropertyDefinition> sampleIdentityDataProperty;
	   FdoPtr<FdoDataPropertyDefinition> sampleNameDataProperty;
	   FdoPtr<FdoGeometricPropertyDefinition> sampleGeometryProperty;
	#ifdef _WIN32
		if (exerciseCapabilities->SupportsApplySchema() && _wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#else
	   if (exerciseCapabilities->SupportsApplySchema() && wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#endif
		   try {
				log->write("Apply the schema created for Schema Management chapter and serialize it to an xml file.");

			   // create the feature schema
			   sampleFeatureSchema = FdoFeatureSchema::Create(L"SampleFeatureSchema", L"Sample Feature Schema Description");

			   // get a pointer to the feature schema's class collection
			   // this object is used to add classes to the schema
			   sampleClassCollection = sampleFeatureSchema->GetClasses();
			   classCount = sampleClassCollection->GetCount();
			   log->write("Number of classes in sampleFeatureSchema:", convert.FdoInt32ToCharStar(classCount));

			   // create a feature class, i.e., a class containing a geometric property, 
			   // and set a class property
			   sampleFeatureClass = FdoFeatureClass::Create(L"SampleFeatureClass", L"Sample Feature Class Description");
			   sampleFeatureClass->SetIsAbstract(false);

			   // get a pointer to the feature class's class property collection
			   // this property is used to add properties to the feature class
			   sampleFeatureClassProperties = sampleFeatureClass->GetProperties();

			   // get a pointer to the feature class's identity property collection
			   // this property is used to add identity properties to the feature class
			   sampleFeatureClassIdentityProperties = sampleFeatureClass->GetIdentityProperties();

			   // create a data property that is of type Int32 and identifies the feature uniquely
			   sampleIdentityDataProperty = FdoDataPropertyDefinition::Create(L"SampleIdentityDataProperty", L"Sample Identity Data Property Description");
			   sampleIdentityDataProperty->SetDataType(FdoDataType_Int32);
			   sampleIdentityDataProperty->SetReadOnly(false);
			   sampleIdentityDataProperty->SetNullable(false);
			   sampleIdentityDataProperty->SetIsAutoGenerated(false);
			   // add the identity property to the sampleFeatureClass
			   sampleFeatureClassProperties->Add(sampleIdentityDataProperty);
			   // identify it as an identity property
			   sampleFeatureClassIdentityProperties->Add(sampleIdentityDataProperty);

			   // create a data property that is of type String and names the feature
			   sampleNameDataProperty = FdoDataPropertyDefinition::Create(L"SampleNameDataProperty", L"Sample Name Data Property Description");
			   sampleNameDataProperty->SetDataType(FdoDataType_String);
			   sampleNameDataProperty->SetLength(64);
			   sampleNameDataProperty->SetReadOnly(false);
			   sampleNameDataProperty->SetNullable(false);
			   sampleNameDataProperty->SetIsAutoGenerated(false);
			   // add the name property to the sampleFeatureClass
				sampleFeatureClassProperties->Add(sampleNameDataProperty);
			   classCount = sampleClassCollection->GetCount();
			   log->write("Number of classes in sampleFeatureSchema:", convert.FdoInt32ToCharStar(classCount));

			   // create a geometric property 
			   sampleGeometryProperty = FdoGeometricPropertyDefinition::Create(L"SampleGeometryProperty", L"Sample Geometry Property Description");
			   sampleGeometryProperty->SetGeometryTypes(FdoGeometricType_Surface);
			   sampleGeometryProperty->SetReadOnly(false);
				sampleGeometryProperty->SetHasMeasure(false);
			   sampleGeometryProperty->SetHasElevation(false);
				// add the geometric property to the sampleFeatureClass
			   sampleFeatureClassProperties->Add(sampleGeometryProperty);
				 // identify it as a geometry property
			   sampleFeatureClass->SetGeometryProperty(sampleGeometryProperty);

				// add the feature class to the schema
			   sampleClassCollection->Add(sampleFeatureClass);
			   classCount = sampleClassCollection->GetCount();
				log->write("Number of classes in sampleFeatureSchema:", convert.FdoInt32ToCharStar(classCount));

			   // point the ApplySchema command at the newly created feature schema and execute
				sampleApplySchema->SetFeatureSchema(sampleFeatureSchema);
			   sampleApplySchema->Execute();
	   
			   // write the schema out to an xml file
				sampleFeatureSchema->WriteXml(L"SampleFeatureSchema.xml");
			 }
			catch (FdoException* ex) {
				log->write("Sample Feature Schema ApplySchema code checkpoint exception message:");
				utilities.PrintException(ex);
				ex->Release();
				return 1;
			}
	   }

		if (exerciseCapabilities->SupportsDescribeSchema()) {
			log->write("Describe the schema created for Schema Management chapter.");
			try {
			   sampleFeatureSchemaCollection = sampleDescribeSchema->Execute();
		#ifdef _WIN32
				if (_wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
		#else
			   if (wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
		#endif
					sampleFeatureSchema = sampleFeatureSchemaCollection->FindItem(L"SAMPLEFEATURESCHEMA");
				} else {
					sampleFeatureSchema = sampleFeatureSchemaCollection->FindItem(L"SampleFeatureSchema");
				}
				if (sampleFeatureSchema != (FdoFeatureSchema *)0) {
					info.LogFeatureSchemaElement(sampleFeatureSchema);
				} else {
					log->write("sampleFeatureSchemaCollection->FindItem(...) returned (FdoFeatureSchema *)0 so no logging of its description");
				}
			   sampleFeatureSchemaCollection->Clear();
		   }
		   catch (FdoException * ex) {
				log->write("Sample Feature Schema DescribeSchema checkpoint exception message:");
				utilities.PrintException(ex);
				ex->Release();
		   }
	   }

	#ifdef _WIN32
	   if (exerciseCapabilities->SupportsDestroySchema() && _wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#else
		if (exerciseCapabilities->SupportsDestroySchema() && wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#endif
			log->write("Destroy the Schema Management chapter sample schema.");
			try {
			   sampleDestroySchema->SetSchemaName(L"SampleFeatureSchema");
			   sampleDestroySchema->Execute();
		   }
		   catch (FdoException * ex) {
				log->write("Sample Feature Schema DestroySchema checkpoint exception message:");
				utilities.PrintException(ex);
				ex->Release();
		   }
	   }

	#ifdef _WIN32
		if (exerciseCapabilities->SupportsApplySchema() && _wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#else
	   if (exerciseCapabilities->SupportsApplySchema() && wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#endif
			log->write("Create the Sample Feature Schema by reading from an xml file.");
			try {
			   sampleFeatureSchemaCollection->ReadXml(L"SampleFeatureSchema.xml");
			   sampleFeatureSchema = sampleFeatureSchemaCollection->FindItem(L"SampleFeatureSchema");
			   sampleApplySchema->SetFeatureSchema(sampleFeatureSchema);
			   sampleApplySchema->Execute();
			   sampleFeatureSchemaCollection->Clear();
		   }
		   catch (FdoException * ex) {
				log->write("Sample Feature Schema ApplySchema from xml checkpoint exception message:");
				utilities.PrintException(ex);
				ex->Release();
		   }
	   }

		FdoPtr<FdoPropertyValueCollection> samplePropertyValues;
		FdoPtr<FdoPropertyValue> sampleNamePropertyValue;
		FdoPtr<FdoIFeatureReader> sampleFeatureReader;
		const wchar_t * propertyName = NULL;
		if (exerciseCapabilities->SupportsInsert()) {
		   try {
			   log->write("Insert 1");
			   // index returned by the operation which adds a value to the value collection
			   FdoInt32 valueCollectionIndex = 0;
		   
			   log->write("Insert 2");
			   // point the Insert command to the target class
			#ifdef _WIN32
				if (_wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
			#else
				if (wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
			#endif
					sampleInsert->SetFeatureClassName(L"SAMPLEFEATURESCHEMA:SAMPLEFEATURECLASS");
				} else {
					sampleInsert->SetFeatureClassName(L"SampleFeatureSchema:SampleFeatureClass");
				}

			   log->write("Insert 3");
			   // get the pointer to the value collection used to add properties to the Insert command
			   samplePropertyValues = sampleInsert->GetPropertyValues();

			   log->write("Insert 4");
			   // create an FdoDataValue for the identity property value
			   FdoPtr<FdoDataValue> sampleIdentityDataValue;
			   sampleIdentityDataValue = FdoDataValue::Create(101);

			   log->write("Insert 5");
			   // add the FdoDataValue to the identity property value
				FdoPtr<FdoPropertyValue> sampleIdentityPropertyValue;
			#ifdef _WIN32
				if (_wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
			#else
				if (wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
			#endif
					propertyName = L"SAMPLEIDENTITYDATAPROPERTY";
				} else {
					propertyName = L"SampleIdentityDataProperty";
				}
			   sampleIdentityPropertyValue = FdoPropertyValue::Create(propertyName, sampleIdentityDataValue);

			   log->write("Insert 6");
			   // add the identity property value to the value collection
			   valueCollectionIndex = samplePropertyValues->Add(sampleIdentityPropertyValue);

			   log->write("Insert 7");
			   // create an FdoDataValue for the name property value
				FdoPtr<FdoDataValue> sampleNameDataValue;
			   sampleNameDataValue = FdoDataValue::Create(L"Blue Lake");

			   log->write("Insert 8");
			   // add the FdoDataValue to the name property value
			#ifdef _WIN32
				if (_wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
			#else
				if (wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
			#endif
					propertyName = L"SAMPLENAMEDATAPROPERTY";
				} else {
					propertyName = L"SampleNameDataProperty";
				}
				sampleNamePropertyValue = FdoPropertyValue::Create(propertyName, sampleNameDataValue);

			   log->write("Insert 9");
			   // add the name property value to the value collection
			   valueCollectionIndex = samplePropertyValues->Add(sampleNamePropertyValue);

				log->write("Insert 10");
			   // create an FdoGeometryValue for the geometry property value
			   // this polygon represents a lake which has an island
			   // the outer shoreline of the lake is defined as a linear ring
			   // the shoreline of the island is defined as a linear ring
			   // the outer shoreline is the external boundary of the polygon
			   // the island shoreline is one of the internal rings
				FdoPtr<FdoGeometryValue> sampleGeometryValue;
			   sampleGeometryValue = FdoGeometryValue::Create();

			   log->write("Insert 11");
			   // define the external boundary of the polygon, the shoreline of Blue Lake
				FdoPtr<FdoILinearRing> exteriorRingBlueLake;
			   FdoInt32 numBlueLakeShorelineOrdinates = 10;
			   double blueLakeExteriorRingOrdinates[] = {52.0, 18.0, 66.0, 23.0, 73.0, 9.0, 48.0, 6.0, 52.0, 18.0};
			   exteriorRingBlueLake = geometryFactory->CreateLinearRing(
				   FdoDimensionality_XY, numBlueLakeShorelineOrdinates, blueLakeExteriorRingOrdinates);

			   log->write("Insert 12");
			   // define the shoreline of Goose Island which is on Blue Lake
			   // this is the sole member of the list of interior rings
				FdoPtr<FdoILinearRing> linearRingGooseIsland;
			   FdoInt32 numGooseIslandShorelineOrdinates = 10;
			   double gooseIslandLinearRingOrdinates[] = {59.0, 18.0, 67.0, 18.0, 67.0, 13.0, 59.0, 13.0, 59.0, 18.0};
			   linearRingGooseIsland = geometryFactory->CreateLinearRing(
				   FdoDimensionality_XY, numGooseIslandShorelineOrdinates, gooseIslandLinearRingOrdinates);

			   log->write("Insert 13");
			   // add the Goose Island linear ring to the list of interior rings
			   FdoPtr<FdoLinearRingCollection> interiorRingsBlueLake;
			   interiorRingsBlueLake = FdoLinearRingCollection::Create();
			   interiorRingsBlueLake->Add(linearRingGooseIsland);

			   log->write("Insert 14");
			   // create the Blue Lake polygon
			   FdoPtr<FdoIPolygon> blueLake;
				blueLake = geometryFactory->CreatePolygon(exteriorRingBlueLake, interiorRingsBlueLake);

			   log->write("Insert 15");
			   // convert the Blue Lake polygon into a byte array
			   // and set the geometry value to this byte array
			   FdoByteArray * geometryByteArray = geometryFactory->GetFgf(blueLake);
			   sampleGeometryValue->SetGeometry(geometryByteArray);

			   log->write("Insert 16");
				// add the FdoGeometryValue to the geometry property value
			   FdoPtr<FdoPropertyValue> sampleGeometryPropertyValue;
			#ifdef _WIN32
				if (_wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
			#else
				if (wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
			#endif
					propertyName = L"SAMPLEGEOMETRYPROPERTY";
				} else {
					propertyName = L"SampleGeometryProperty";
				}
			   sampleGeometryPropertyValue = FdoPropertyValue::Create(propertyName, sampleGeometryValue);

			   log->write("Insert 17");
				// add the geometry property value to the value collection
				valueCollectionIndex = samplePropertyValues->Add(sampleGeometryPropertyValue);

				log->write("Insert 18");
			   // do the insertion
			   sampleFeatureReader = sampleInsert->Execute();
			   info.LogFeatureReader(sampleFeatureReader);
		   }
		   catch (FdoException * ex) {
				log->write("Sample Feature Schema Insert checkpoint exception message:");
				utilities.PrintException(ex);
				ex->Release();
		   }
	   }

	   // change the value of the SampleNameDataProperty from "Blue Lake" to "Green Lake"
		FdoInt32 numUpdated = 0;
		if (exerciseCapabilities->SupportsUpdate()) {
		   log->write("Update what has just been inserted");
		   try {
			   // point the Update command at the target feature class
			#ifdef _WIN32
				if (_wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
			#else
				if (wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
			#endif
					sampleUpdate->SetFeatureClassName(L"SAMPLEFEATURESCHEMA:SAMPLEFEATURECLASS");
				} else {
					sampleUpdate->SetFeatureClassName(L"SampleFeatureSchema:SampleFeatureClass");
				}

			   // set the filter to identify which set of properties to update
			   sampleUpdate->SetFilter(L"( SampleIdentityDataProperty = 101 )");

			   // get the pointer to the value collection used to add properties to the Update command
			   samplePropertyValues = sampleUpdate->GetPropertyValues();

			   // create an FdoDataValue for the name property value
			   FdoPtr<FdoDataValue> sampleNameDataValue;
			   sampleNameDataValue = FdoDataValue::Create(L"Green Lake");

			   // set the name and value of the property value
			#ifdef _WIN32
				if (_wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
			#else
				if (wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
			#endif
					propertyName = L"SAMPLENAMEDATAPROPERTY";
				} else {
					propertyName = L"SampleNameDataProperty";
				}
			   sampleNamePropertyValue->SetName(propertyName);
			   sampleNamePropertyValue->SetValue(sampleNameDataValue);

			   // add the name property value to the property value collection owned by the
			   // Update command
			   samplePropertyValues->Add(sampleNamePropertyValue);

				// execute the command
			   numUpdated = sampleUpdate->Execute();
			   log->write("Number of instances updated:", convert.FdoInt32ToCharStar(numUpdated));
		   }
		   catch (FdoException * ex) {
				log->write("Sample Feature Schema Update checkpoint exception message:");
				utilities.PrintException(ex);
				ex->Release();
			}
		}

	   // we have one FdoFeatureClass object in the datastore
		// create a query that returns this object
		if (exerciseCapabilities->SupportsSelect()) {
		   try {
			   // create the select command
		   
				// point the select command at the target FdoFeatureClass SampleFeatureClass
			#ifdef _WIN32
				if (_wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
			#else
				if (wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
			#endif
					sampleSelect->SetFeatureClassName(L"SAMPLEFEATURESCHEMA:SAMPLEFEATURECLASS");
				} else {
					sampleSelect->SetFeatureClassName(L"SampleFeatureSchema:SampleFeatureClass");
				}

			   // create the filter by
			   // 1. creating an FdoIdentifier object containing the name of 
			   //    the identity property
			   FdoPtr<FdoIdentifier> queryPropertyName;
			#ifdef _WIN32
				if (_wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
			#else
				if (wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
			#endif
					propertyName = L"SAMPLEIDENTITYDATAPROPERTY";
				} else {
					propertyName = L"SampleIdentityDataProperty";
				}
			   queryPropertyName = FdoIdentifier::Create(propertyName);

				// 2. creating an FdoDataValue object containing the value of the identity
			   //    property
			   FdoPtr<FdoDataValue> queryPropertyValue;
			   queryPropertyValue = FdoDataValue::Create(101);

			   // 3. calling FdoComparisonCondition::Create() passing in the
			   //    the queryPropertyName, an enumeration constant signifying an equals
			   //    comparison operation, and the queryPropertyValue
			   FdoPtr<FdoFilter> filter;
			   filter = FdoComparisonCondition::Create(queryPropertyName,
				   FdoComparisonOperations_EqualTo, queryPropertyValue);

			   // point the select command at the filter
			   sampleSelect->SetFilter(filter);

			   // execute the select command
			   sampleFeatureReader = sampleSelect->Execute();
			   info.LogFeatureReader(sampleFeatureReader);
			}
			catch (FdoException * ex) {
				log->write("Sample Feature Schema Select checkpoint exception message:");
				utilities.PrintException(ex);
				ex->Release();
			}
		}

		// cannot delete a schema after data has been inserted until data has been removed
		// do a delete and then a destroy schema
		FdoInt32 numDeleted = 0;
		if (exerciseCapabilities->SupportsDelete()) {
			log->write("Delete what has just been inserted");
			try {
			#ifdef _WIN32
				if (_wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
			#else
				if (wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
			#endif
					sampleDelete->SetFeatureClassName(L"SAMPLEFEATURESCHEMA:SAMPLEFEATURECLASS");
				} else {
					sampleDelete->SetFeatureClassName(L"SampleFeatureSchema:SampleFeatureClass");
				}
			#ifdef _WIN32
				if (_wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
			#else
				if (wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
			#endif
					sampleDelete->SetFilter(L"( SAMPLEIDENTITYDATAPROPERTY = 101 )");
				} else {
					sampleDelete->SetFilter(L"( SampleIdentityDataProperty = 101 )");
				}
				numDeleted = sampleDelete->Execute();
				log->write("Number of instances deleted:", convert.FdoInt32ToCharStar(numDeleted));
			}
			catch (FdoException * ex) {
				log->write("Sample Feature Schema Delete checkpoint exception message:");
				utilities.PrintException(ex);
				ex->Release();
			}
		}


	#ifdef _WIN32
		if (exerciseCapabilities->SupportsDestroySchema() && _wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#else
		if (exerciseCapabilities->SupportsDestroySchema() && wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#endif
			log->write("Destroy the Schema Management chapter sample schema.");
		   try {
			   sampleDestroySchema->SetSchemaName(L"SampleFeatureSchema");
			   sampleDestroySchema->Execute();
		   }
		   catch (FdoException * ex) {
				log->write("Sample Feature Schema DestroySchema checkpoint exception message:");
				utilities.PrintException(ex);
				ex->Release();
			}
		}
   } // if (doSampleFeatureSchemaBool)

   /*************************************************************************************
   *	OGC980461FS schema
   *	this code gets method call arguments from configuration files
   *************************************************************************************/

   // 1. create the commands which will be used in the OGC980461FS schema operations
   // 2. if provider supports the ApplySchema command, create part of the OGC980461FS schema
   //    by reading name/value pairs from a file
   // 3. write the schema in GML format to an XML file
   // 4. describe the schema
   // 5. if the provider supports the DesctroySchema command, destroy the schema
   // 6. if the provider supports the ApplySchema command, create the schema by reading
   //    from the XML file created in step 3
   // 7. describe the schema
   // 8. if the provider supports the DesctroySchema command, destroy the schema
   // 9. if the provider supports the ApplySchema command, create the entire OGC980461FS schema
   //     by reading from an XSD file that was created manually
   // 10. describe the schema
   // 11. if the provider supports insertion, insert the data in the OGC98046r1.insert or OGC98046r1.insert.ArcSDE file
   // 12. if the provider supports deletion, delete the feature objects specified in the filters contained in the OGC98046r1.delete file
   // 14. if the provider supports the DesctroySchema command, destroy the schema

   // this object will parse a file containing part of the schema contained
   // in the OGC 98-046r1 document
   // this partial schema will be used to demonstrate the FDO methods for
   // creating classes and properties.
   ParseSchemaDefinitionFile * schemaDefinitionFile;
   try {
	   const char * schemaDefFileName = configInfo->getSchemaDefFileName();
	   	schemaDefinitionFile = new ParseSchemaDefinitionFile(schemaDefFileName, log, &convert);
   }
   catch (char * errMsg) {
	   log->write("Caught error message:", errMsg);
	   return 1;
   }

   // feature schema declarations
   FdoPtr<FdoFeatureSchema> featureSchema;
   FdoPtr<FdoFeatureSchemaCollection> featureSchemaCollection = FdoFeatureSchemaCollection::Create(NULL);


  // create the commands that will be used over and over
   FdoPtr<FdoIApplySchema> applySchema;
   FdoPtr<FdoIDestroySchema> destroySchema;
   FdoPtr<FdoIDescribeSchema> describeSchema;
   FdoPtr<FdoIInsert> insertCmd;
   FdoPtr<FdoIDelete> deleteCmd;
   try {
#ifdef _WIN32
   if (exerciseCapabilities->SupportsApplySchema() && _wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
#else
   if (exerciseCapabilities->SupportsApplySchema() && wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
#endif
		   applySchema = (FdoIApplySchema *)connection->CreateCommand(FdoCommandType_ApplySchema);
	   }
	   if (exerciseCapabilities->SupportsDescribeSchema()) {
		   describeSchema = (FdoIDescribeSchema *)connection->CreateCommand(FdoCommandType_DescribeSchema);
	   }
#ifdef _WIN32
   if (exerciseCapabilities->SupportsDestroySchema() && _wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
#else
   if (exerciseCapabilities->SupportsDestroySchema() && wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
#endif
		   destroySchema = (FdoIDestroySchema *)connection->CreateCommand(FdoCommandType_DestroySchema);
	   }
	   if (exerciseCapabilities->SupportsInsert()) {
		   insertCmd = (FdoIInsert *)connection->CreateCommand(FdoCommandType_Insert);
	   }
	   if (exerciseCapabilities->SupportsDelete()) {
		   deleteCmd = (FdoIDelete *)connection->CreateCommand(FdoCommandType_Delete);
	   }
   }
   catch (FdoException* ex) {
	   log->write("Create commands checkpoint exception message:");
	   utilities.PrintException(ex);
	   ex->Release();
   }


   FdoPtr<FdoFeatureClass> featureClass;
   FdoPtr<FdoClass> nonSpatialClass;
   FdoPtr<FdoClassCollection> classCollection;
	
	if (doPartialSchemaBool) {
	#ifdef _WIN32
	   if (exerciseCapabilities->SupportsApplySchema() && _wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#else
	   if (exerciseCapabilities->SupportsApplySchema() && wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#endif
			int schemaCount = 0;
			int classCount = 0;
			int propertyCount = 0;
			FdoClassType classType;
			log->write("Create a schema by reading from a flat file containing name value pairs and calling FDO methods and serialize it to an xml file");
			try {	   
				retval = schemaDefinitionFile->getNameValue();
				if (retval) throw("ERROR reading schema file");
				if (strcmp(schemaDefinitionFile->GetCurrentName(), "schemaCount") == 0) {
					schemaCount = schemaDefinitionFile->GetSchemaCount();
				} else {
					throw("ERROR: expecting schema count");
				}
				for(int i = 0; i < schemaCount; i++) {
					retval = schemaDefinitionFile->GetNextSchema();
					if (retval) break;
					featureSchema = FdoFeatureSchema::Create(schemaDefinitionFile->GetSchemaName(), schemaDefinitionFile->GetSchemaDescription());
					classCollection = featureSchema->GetClasses();
					classCount = schemaDefinitionFile->GetClassCount();
					for (int i=0; i < classCount; i++) {
						schemaDefinitionFile->GetNextClass();
						classType = str2FdoType.convStrToFdoClassType(schemaDefinitionFile->GetClassType());
						switch (classType) {
							case FdoClassType_Class:
								nonSpatialClass = FdoClass::Create(schemaDefinitionFile->GetClassName(), schemaDefinitionFile->GetClassDescription());
								exerciseSchema.AddClassDefinitionProperties(nonSpatialClass, schemaDefinitionFile);
								exerciseSchema.AddProperties(nonSpatialClass, schemaDefinitionFile);
								classCollection->Add(nonSpatialClass);
								break;
							case FdoClassType_FeatureClass:
								featureClass = FdoFeatureClass::Create(schemaDefinitionFile->GetClassName(), schemaDefinitionFile->GetClassDescription());
								exerciseSchema.AddClassDefinitionProperties(featureClass, schemaDefinitionFile);
								exerciseSchema.AddProperties(featureClass, schemaDefinitionFile);
								classCollection->Add(featureClass);
								break;
							default: throw("unknown FdoClassType");
						}
					}
				}
			   applySchema->SetFeatureSchema(featureSchema);
			   applySchema->Execute();
			   featureSchema->WriteXml(configInfo->getGeneratedXmlSchemaFileName());
			}
			catch (char * errMsg) {
				log->write("Caught error message:", errMsg);
			}
			catch (FdoException * ex) {
				log->write("create schema checkpoint exception message:");
				utilities.PrintException(ex);
				ex->Release();
			}
	   }
   
	   if (exerciseCapabilities->SupportsDescribeSchema()) {
			log->write("Describe the schema.");
		   try {
			   featureSchemaCollection = describeSchema->Execute();
			   featureSchema = featureSchemaCollection->FindItem(configInfo->getEditedXmlSchemaName());
			   info.LogFeatureSchemaElement(featureSchema);
			   featureSchemaCollection->Clear();
		   }
		   catch (FdoException * ex) {
				log->write("DescribeSchema checkpoint exception message:");
				utilities.PrintException(ex);
				ex->Release();
		   }
	   }

	#ifdef _WIN32
	   if (exerciseCapabilities->SupportsDestroySchema() && _wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#else
	   if (exerciseCapabilities->SupportsDestroySchema() && wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#endif
			log->write("Destroy a schema.");
		   try {
			   destroySchema->SetSchemaName(configInfo->getEditedXmlSchemaName());
			   destroySchema->Execute();
		   }
		   catch (FdoException * ex) {
				log->write("DestroySchema checkpoint exception message:");
				utilities.PrintException(ex);
				ex->Release();
		   }
	   }

	#ifdef _WIN32
	   if (exerciseCapabilities->SupportsApplySchema() && _wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#else
	   if (exerciseCapabilities->SupportsApplySchema() && wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#endif
			log->write("Create a schema by reading from a generated xml file.");
		   try {
			   featureSchemaCollection->ReadXml(configInfo->getGeneratedXmlSchemaFileName());
			   featureSchema = featureSchemaCollection->FindItem(configInfo->getEditedXmlSchemaName());
			   applySchema->SetFeatureSchema(featureSchema);
			   applySchema->Execute();
			   featureSchemaCollection->Clear();
		   }
		   catch (FdoException * ex) {
				log->write("ApplySchema from xml checkpoint exception message:");
				utilities.PrintException(ex);
				ex->Release();
		   }
	   }

	   if (exerciseCapabilities->SupportsDescribeSchema()) {
			log->write("Describe a schema.");
		   try {
			   featureSchemaCollection = describeSchema->Execute();
			   featureSchema = featureSchemaCollection->FindItem(configInfo->getEditedXmlSchemaName());
			   info.LogFeatureSchemaElement(featureSchema);
			   featureSchemaCollection->Clear();
		   }
		   catch (FdoException * ex) {
				log->write("DescribeSchema checkpoint exception message:");
				utilities.PrintException(ex);
				ex->Release();
		   }
	   }

	#ifdef _WIN32
		if (exerciseCapabilities->SupportsDestroySchema() && _wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#else
		if (exerciseCapabilities->SupportsDestroySchema() && wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#endif
				log->write("Destroy a schema created by reading from a generated xml file.");
		   try {
			   destroySchema->SetSchemaName(configInfo->getEditedXmlSchemaName());
			   destroySchema->Execute();
		   }
		   catch (FdoException * ex) {
				log->write("DestroySchema checkpoint exception message:");
				utilities.PrintException(ex);
				ex->Release();
		   }
		}
   } // if (doPartialSchemaBool)

   if (doXmlSchemaBool) {
	   FdoPtr<FdoXmlFlags> xmlFlags;
	   FdoString * url = L"fdo.osgeo.org/schemas/feature";
	#ifdef _WIN32
	   if (exerciseCapabilities->SupportsApplySchema() && _wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#else
	   if (exerciseCapabilities->SupportsApplySchema() && wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#endif
			log->write("Create a schema by reading from an edited xml file.");
		   try {
			   const wchar_t* fileName = configInfo->getEditedXmlSchemaFileName();
			   xmlFlags = FdoXmlFlags::Create(url, FdoXmlFlags::ErrorLevel_High);
			   featureSchemaCollection->ReadXml(fileName);
			   featureSchema = featureSchemaCollection->FindItem(configInfo->getEditedXmlSchemaName());
			   applySchema->SetFeatureSchema(featureSchema);
			   applySchema->Execute();
			   featureSchemaCollection->Clear();
		   }
		   catch (FdoException * ex) {
				log->write("ApplySchema from edited xml checkpoint exception message:");
				utilities.PrintException(ex);
				ex->Release();
		   }
	   }

	   if (exerciseCapabilities->SupportsDescribeSchema()) {
			log->write("Describe a schema.");
		   try {
			   featureSchemaCollection = describeSchema->Execute();
				featureSchema = featureSchemaCollection->FindItem(configInfo->getEditedXmlSchemaName());
			   info.LogFeatureSchemaElement(featureSchema);
			   featureSchemaCollection->Clear();
		   }
		   catch (FdoException * ex) {
				log->write("DescribeSchema checkpoint exception message:");
				utilities.PrintException(ex);
				ex->Release();
			}
		}
   }

   ParseSchemaInsertionFile * schemaInsertFile;
   try {
	   const char * schemaInsertFileName = configInfo->getSchemaInsertFileName();
	   	schemaInsertFile = new ParseSchemaInsertionFile(schemaInsertFileName, log, &convert, &utilities);
   }
   catch (char * errMsg) {
	   log->write("Caught error message:", errMsg);
	   return 1;
   }

   ParseSchemaFilterFile * schemaDeleteFile;
   try {
	   const char * schemaDeleteFileName = configInfo->getSchemaDeleteFileName();
	   	schemaDeleteFile = new ParseSchemaFilterFile(schemaDeleteFileName, log, &convert);
   }
   catch (char * errMsg) {
	   log->write("Caught error message:", errMsg);
	   return 1;
   }

   if (doInsertDeleteBool) {
	   FdoPtr<FdoIFeatureReader> featureReader;
	   if (exerciseCapabilities->SupportsInsert()) {
			int schemaCount = 0;
			int classCount = 0;
			int propertyCount = 0;
			int classInstanceCount = 0;
			const wchar_t * qualifiedClassName = NULL;
			const wchar_t * propertyName = NULL;
			const char * filePropertyValue = NULL;
			FdoInt32 fdoInt32Value = 0;
			FdoPtr<FdoGeometryValue> geometryValue;
			FdoPtr<FdoPropertyValue> propertyValue;
			FdoPtr<FdoPropertyValueCollection> propertyValues;
			FdoInt32 propertyValueIndex = 0;
			FdoPtr<FdoDataValue> dataValue;
			FdoPropertyType propertyType;
			FdoDataType dataType;
			FdoGeometryType geometryType;
			double * linearRingOrdinates = NULL;
			FdoPtr<FdoILinearRing> exteriorLinearRing;
			FdoPtr<FdoILinearRing> interiorLinearRing;
			FdoPtr<FdoLinearRingCollection> interiorRings;
			int interiorRingCount = 0;
			FdoPtr<FdoIPolygon> polygon;
			FdoPtr<FdoIMultiPolygon> multiPolygon;
			int polygonCount = 0;
			FdoPtr<FdoPolygonCollection> polygons;
			FdoPtr<FdoILineString> lineString;
			FdoPtr<FdoLineStringCollection> lineStrings;
			int lineStringCount = 0;
			FdoPtr<FdoIMultiLineString> multiLineString;
			double * ordinateArr = NULL;
			int ordinateCount = 0;
			FdoPtr<FdoIPoint> point;
			FdoByteArray * geometryByteArray = NULL;
			log->write("Read the schema data from a file and insert it.");
			try {
				retval = schemaInsertFile->getNameValue();
				if (retval) throw("ERROR reading schema data file");
				if (strcmp(schemaInsertFile->GetCurrentName(), "schemaCount") == 0) {
					schemaCount = schemaInsertFile->GetSchemaCount();
				} else {
					throw("ERROR: expecting schema count");
				}
				interiorRings = FdoLinearRingCollection::Create();
				lineStrings = FdoLineStringCollection::Create();
				polygons = FdoPolygonCollection::Create();
				for(int i = 0; i < schemaCount; i++) {
					retval = schemaInsertFile->GetNextSchema();
					if (retval) throw("ERROR reading schema data file at GetNextSchema()");
					classCount = schemaInsertFile->GetClassCount();
					for (int j=0; j < classCount; j++) {
						retval = schemaInsertFile->GetNextClass();
						if (retval) throw("ERROR reading schema data file at GetNextClass()");
					#ifdef _WIN32
						if (_wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
					#else
						if (wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
					#endif
							qualifiedClassName = schemaInsertFile->GetQualifiedClassNameUpperCase();
						} else {
							qualifiedClassName = schemaInsertFile->GetQualifiedClassName();
						}
						log->write("Qualified class name:", convert.wchar_tStarToCharStar(qualifiedClassName));
						insertCmd->SetFeatureClassName(qualifiedClassName);
						propertyValues = insertCmd->GetPropertyValues();
						classInstanceCount = schemaInsertFile->GetClassInstanceCount();
						propertyCount = schemaInsertFile->GetPropertyCount();
						for(int k=0; k<classInstanceCount; k++) {
							for(int m=0; m<propertyCount; m++) {
								retval = schemaInsertFile->GetNextProperty();
								if (retval) throw("ERROR reading schema data file at GetNextProperty()");
							#ifdef _WIN32
								if (_wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
							#else
								if (wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
							#endif
									propertyName = schemaInsertFile->GetPropertyNameUpperCase();
								} else {
									propertyName = schemaInsertFile->GetPropertyName();
								}
								propertyType = str2FdoType.convStrToFdoPropertyType(schemaInsertFile->GetPropertyType());
								propertyValue = FdoPropertyValue::Create();
								switch (propertyType) {
									case FdoPropertyType_DataProperty:
										filePropertyValue = schemaInsertFile->GetPropertyValue();
										dataType = str2FdoType.convStrToFdoDataType(schemaInsertFile->GetPropertyDataType());
										switch (dataType) {
											case FdoDataType_Int32:
												fdoInt32Value = (FdoInt32)atoi(filePropertyValue);
												dataValue = FdoDataValue::Create(fdoInt32Value);
												break;
											case FdoDataType_String:
												dataValue = FdoDataValue::Create(convert.charStarToWchar_tStar(filePropertyValue));
												break;
											default: throw("Unknown FdoDataType");
										}
										propertyValue->SetValue(dataValue);
										propertyValue->SetName(propertyName);
										break;
									case FdoPropertyType_GeometricProperty:
										geometryType = str2FdoType.convStrToFdoGeometryType(schemaInsertFile->GetPropertyGeometryType());
										geometryValue = FdoGeometryValue::Create();
										switch (geometryType) {
											case FdoGeometryType_Point:
												retval = schemaInsertFile->GetNextPoint();
												if (retval) throw("ERROR reading schema data file at GetNextPoint()");
												ordinateArr = schemaInsertFile->GetPoint();
												point = geometryFactory->CreatePoint(FdoDimensionality_XY, ordinateArr);
												geometryByteArray = geometryFactory->GetFgf(point);
												break;
											case FdoGeometryType_Polygon:
												retval = schemaInsertFile->GetNextPolygon();
												if (retval) throw("ERROR reading schema data file at GetNextPolygon()");
												linearRingOrdinates = schemaInsertFile->GetExteriorRing();
												exteriorLinearRing = geometryFactory->CreateLinearRing(FdoDimensionality_XY, schemaInsertFile->GetOrdinateCount(), linearRingOrdinates);
												interiorRingCount = schemaInsertFile->GetInteriorRingCount();
												for (int n=0; n<interiorRingCount; n++) {
													retval = schemaInsertFile->GetNextInteriorRing();
													if (retval) throw("ERROR reading schema data file at GetNextInteriorRing()");
													linearRingOrdinates = schemaInsertFile->GetInteriorRing();
													interiorLinearRing = geometryFactory->CreateLinearRing(FdoDimensionality_XY, schemaInsertFile->GetOrdinateCount(), linearRingOrdinates);
													interiorRings->Add(interiorLinearRing);
												}
												polygon = geometryFactory->CreatePolygon(exteriorLinearRing, interiorRings);
												geometryByteArray = geometryFactory->GetFgf(polygon);
												break;
											case FdoGeometryType_MultiPolygon:
												polygonCount = schemaInsertFile->GetPolygonCount();
												for (int p=0; p<polygonCount; p++) {
													retval = schemaInsertFile->GetNextPolygon();
													if (retval) throw("ERROR reading schema data file at GetNextPolygon()");
													linearRingOrdinates = schemaInsertFile->GetExteriorRing();
													exteriorLinearRing = geometryFactory->CreateLinearRing(FdoDimensionality_XY, schemaInsertFile->GetOrdinateCount(), linearRingOrdinates);
													interiorRingCount = schemaInsertFile->GetInteriorRingCount();
													for (int q=0; q<interiorRingCount; q++) {
														retval = schemaInsertFile->GetNextInteriorRing();
														if (retval) throw("ERROR reading schema data file at GetNextInteriorRing()");
														linearRingOrdinates = schemaInsertFile->GetInteriorRing();
														interiorLinearRing = geometryFactory->CreateLinearRing(FdoDimensionality_XY, schemaInsertFile->GetOrdinateCount(), linearRingOrdinates);
														interiorRings->Add(interiorLinearRing);
													}
													polygon = geometryFactory->CreatePolygon(exteriorLinearRing, interiorRings);
													polygons->Add(polygon);
													interiorRings->Clear();
												}
												multiPolygon = geometryFactory->CreateMultiPolygon(polygons);
												geometryByteArray = geometryFactory->GetFgf(multiPolygon);
												polygons->Clear();
												break;
											case FdoGeometryType_LineString:
												retval = schemaInsertFile->GetNextLineString();
												if (retval) throw("ERROR reading schema data file at GetNextLineString()");
												ordinateArr = schemaInsertFile->GetLineString();
												lineString = geometryFactory->CreateLineString(FdoDimensionality_XY, schemaInsertFile->GetOrdinateCount(), ordinateArr);
												geometryByteArray = geometryFactory->GetFgf(lineString);
												break;
											case FdoGeometryType_MultiLineString:
												lineStringCount = schemaInsertFile->GetLineStringCount();
												for (int r=0; r<lineStringCount; r++) {
													retval = schemaInsertFile->GetNextLineString();
													if (retval) throw("ERROR reading schema data file at GetNextLineString()");
													ordinateArr = schemaInsertFile->GetLineString();
													ordinateCount = schemaInsertFile->GetOrdinateCount();
													lineString = geometryFactory->CreateLineString(FdoDimensionality_XY, ordinateCount, ordinateArr);
													lineStrings->Add(lineString);
												}
												multiLineString = geometryFactory->CreateMultiLineString(lineStrings);
												geometryByteArray = geometryFactory->GetFgf(multiLineString);
												lineStrings->Clear();
												break;
											default: throw("unknown FdoGeometryType");
										}
										geometryValue->SetGeometry(geometryByteArray);
										propertyValue->SetValue(geometryValue);
										propertyValue->SetName(propertyName);
										break;
									default: throw("unknown FdoPropertyType");
								}
								propertyValueIndex = propertyValues->Add(propertyValue);
								sprintf(sprintfBuf, "%ls added to property collection at index %d", propertyName, propertyValueIndex);
								log->write(sprintfBuf);
							}
							featureReader = insertCmd->Execute();
							log->write("Log the feature reader returned by the insert command");
							info.LogFeatureReader(featureReader);
							propertyValues->Clear();
						}
					}
				}
			}
			catch (char * errMsg) {
				log->write(errMsg);
			}
		   catch (FdoException * ex) {
				log->write("Insert checkpoint exception message:");
				utilities.PrintException(ex);
				ex->Release();
		   }
	   }
	
		FdoInt32 numDeleted = 0;
		if (exerciseCapabilities->SupportsDelete()) {
			log->write("Delete what has just been inserted");
			int schemaCount = 0;
			int filterCount = 0;
			const wchar_t * qualifiedClassName = NULL;
			const wchar_t * filterString = NULL;
			try {
				log->write("Read filters from a file and do deletions.");
				retval = schemaDeleteFile->getNameValue();
				if (retval) throw("ERROR reading schema filter file");
				if (strcmp(schemaDeleteFile->GetCurrentName(), "schemaCount") == 0) {
					schemaCount = schemaDeleteFile->GetSchemaCount();
				} else {
					throw("ERROR: expecting schema count");
				}
				for(int i = 0; i < schemaCount; i++) {
					retval = schemaDeleteFile->GetNextSchema();
					if (retval) throw("ERROR reading schema filter file at GetNextSchema()");
					filterCount = schemaDeleteFile->GetFilterCount();
					for (int i=0; i < filterCount; i++) {
						numDeleted = 0;
						retval = schemaDeleteFile->GetNextFilter();
						if (retval) throw("ERROR reading schema filter file at GetNextFilter()");
					#ifdef _WIN32
						if (_wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
					#else
						if (wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
					#endif
							qualifiedClassName = schemaDeleteFile->GetQualifiedClassNameUpperCase();
						} else {
							qualifiedClassName = schemaDeleteFile->GetQualifiedClassName();
						}
						log->write("Qualified class name:", convert.wchar_tStarToCharStar(qualifiedClassName));
						deleteCmd->SetFeatureClassName(qualifiedClassName);
					#ifdef _WIN32
						if (_wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
					#else
						if (wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) == 0) {
					#endif
							filterString = schemaDeleteFile->GetFilterStringUpperCase();
						} else {
							filterString = schemaDeleteFile->GetFilterString();
						}
						deleteCmd->SetFilter(filterString);
						numDeleted = deleteCmd->Execute();
						sprintf(sprintfBuf, "%d instance(s) of %ls deleted", numDeleted, qualifiedClassName);
						log->write(sprintfBuf);
					}
				}
			}
			catch (FdoException * ex) {
				log->write("Generic Delete checkpoint exception message:");
				utilities.PrintException(ex);
				ex->Release();
			}
		}
	}

	if (undoXmlSchemaBool) {
	#ifdef _WIN32
	   if (exerciseCapabilities->SupportsDestroySchema() && _wcsnicmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#else
	   if (exerciseCapabilities->SupportsDestroySchema() && wcsncasecmp(L"Autodesk.ArcSDE.", connectionInfo->GetProviderName(), 16) != 0) {
	#endif
			log->write("Destroy a schema created by reading from an edited xml file.");
		   try {
			   destroySchema->SetSchemaName(configInfo->getEditedXmlSchemaName());
			   destroySchema->Execute();
		   }
		   catch (FdoException * ex) {
				log->write("DestroySchema checkpoint exception message:");
				utilities.PrintException(ex);
				ex->Release();
		   }
	   }
	}
   try {
	   connection->Close();
	   connectionState = connection->GetConnectionState();
	   log->write("Connection State:", info.GetConnectionStateName(connectionState));
   }
   catch (FdoException* ex) {
	   log->write("Connection close checkpoint exception message:");
	   utilities.PrintException(ex);
		ex->Release();
   }

   delete log;
   delete configInfo;
   delete exerciseCapabilities;
   delete schemaDefinitionFile;
   delete schemaInsertFile;
   delete connectUtilities;
   return 0; 
}

