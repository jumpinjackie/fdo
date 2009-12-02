// fdoinfo.cpp
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
#include "fdoinfo.h"

FdoInfo::FdoInfo(Logger * log, Conversions * convert,
				 ExerciseFdoUtilities * utilities, FdoFgfGeometryFactory * geometryFactory) {
	this->log = log;
	this->convert = convert;
	this->utilities = utilities;
	this->geometryFactory = geometryFactory;
	numRasterDataModels = 6;
	unknownRasterDataModel = FdoRasterDataModel::Create();
	unknownRasterDataModel->SetDataModelType(FdoRasterDataModelType_Unknown);
	this->LogRasterDataModelProperties(unknownRasterDataModel);
	rasterDataModels[FdoRasterDataModelType_Unknown] = unknownRasterDataModel;
	bitonalRasterDataModel = FdoRasterDataModel::Create();
	bitonalRasterDataModel->SetDataModelType(FdoRasterDataModelType_Bitonal);
	this->LogRasterDataModelProperties(bitonalRasterDataModel);
	rasterDataModels[FdoRasterDataModelType_Bitonal] = bitonalRasterDataModel;
	grayRasterDataModel = FdoRasterDataModel::Create();
	grayRasterDataModel->SetDataModelType(FdoRasterDataModelType_Gray);
	this->LogRasterDataModelProperties(grayRasterDataModel);
	rasterDataModels[FdoRasterDataModelType_Gray] = grayRasterDataModel;
	rgbRasterDataModel = FdoRasterDataModel::Create();
	rgbRasterDataModel->SetDataModelType(FdoRasterDataModelType_RGB);
	this->LogRasterDataModelProperties(rgbRasterDataModel);
	rasterDataModels[FdoRasterDataModelType_RGB] = rgbRasterDataModel;
	rgbaRasterDataModel = FdoRasterDataModel::Create();
	rgbaRasterDataModel->SetDataModelType(FdoRasterDataModelType_RGBA);
	this->LogRasterDataModelProperties(rgbaRasterDataModel);
	rasterDataModels[FdoRasterDataModelType_RGBA] = rgbaRasterDataModel;
	paletteRasterDataModel = FdoRasterDataModel::Create();
	paletteRasterDataModel->SetDataModelType(FdoRasterDataModelType_Palette);
	paletteRasterDataModel->SetBitsPerPixel(64);
	paletteRasterDataModel->SetOrganization(FdoRasterDataOrganization_Image);
	paletteRasterDataModel->SetTileSizeX(64);
	paletteRasterDataModel->SetTileSizeY(64);
	this->LogRasterDataModelProperties(paletteRasterDataModel);
	rasterDataModels[FdoRasterDataModelType_Palette] = paletteRasterDataModel;
}

void
FdoInfo::LogArgumentDefinition(FdoArgumentDefinition * argDefinition) {
	log->write("Argument Name:", convert->wchar_tStarToCharStar(argDefinition->GetName()));
	log->write("Argument Description:", convert->wchar_tStarToCharStar(argDefinition->GetDescription()));
	log->write("Argument Data Type:", this->GetDataTypeName(argDefinition->GetDataType()));
}

void
FdoInfo::LogAssociationPropertyDefinition(FdoAssociationPropertyDefinition * associationPropertyDefinition) {
	log->write("Association Property Definition");
	log->write("Property Type:", this->GetPropertyTypeName(associationPropertyDefinition->GetPropertyType()));
	FdoClassDefinition * associatedClass = associationPropertyDefinition->GetAssociatedClass();
	log->write("Associated Class");
	this->LogClassDefinition(associatedClass);
	FdoDataPropertyDefinitionCollection * identityPropDefCollection = associationPropertyDefinition->GetIdentityProperties();
	log->write("Identity Properties");
	FdoInt32 numIdentityProperties = identityPropDefCollection->GetCount();
	log->write("Number of identity properties:", convert->FdoInt32ToCharStar(numIdentityProperties));
	for(int i = 0; i < numIdentityProperties; i++) {
		this->LogDataPropertyDefinition(identityPropDefCollection->GetItem(i));
	}
	FdoDataPropertyDefinitionCollection * revIdentityPropDefCollection = associationPropertyDefinition->GetReverseIdentityProperties();
	log->write("Identity Properties");
	FdoInt32 numRevIdentityProperties = revIdentityPropDefCollection->GetCount();
	log->write("Number of reverse identity properties:", convert->FdoInt32ToCharStar(numRevIdentityProperties));
	for(int i = 0; i < numRevIdentityProperties; i++) {
		this->LogDataPropertyDefinition(revIdentityPropDefCollection->GetItem(i));
	}
	log->write("Reverse name:", convert->wchar_tStarToCharStar(associationPropertyDefinition->GetReverseName()));
	log->write("Delete rule:", this->GetDeleteRuleTypeName(associationPropertyDefinition->GetDeleteRule()));
	log->write("Supports lock cascade:", convert->boolToCharStar(associationPropertyDefinition->GetLockCascade()));
	log->write("Read-Only:", convert->boolToCharStar(associationPropertyDefinition->GetIsReadOnly()));
	log->write("Multiplicy:", convert->wchar_tStarToCharStar(associationPropertyDefinition->GetMultiplicity()));
	log->write("Reverse multiplicy:", convert->wchar_tStarToCharStar(associationPropertyDefinition->GetReverseMultiplicity()));
}

void
FdoInfo::LogClassCapabilities(FdoClassCapabilities * classCapabilities) {
	log->write("Supports Locking:", convert->boolToCharStar(classCapabilities->SupportsLocking()));
	FdoInt32 numLockTypes = 0;
	FdoLockType * lockTypes = classCapabilities->GetLockTypes(numLockTypes);
	log->write("Number of lock types:", convert->FdoInt32ToCharStar(numLockTypes));
	for(int i = 0; i < numLockTypes; i++) {
		log->write("Lock type:", this->GetLockTypeName(lockTypes[i]));
	}
	log->write("Supports Long Transactions:", convert->boolToCharStar(classCapabilities->SupportsLongTransactions()));
	FdoClassDefinition * parentClass = classCapabilities->GetParent();
	log->write("Name of parent of this class capabilities object:", convert->wchar_tStarToCharStar(parentClass->GetName()));
}


void
FdoInfo::LogClassDefinition(FdoClassDefinition * classDefinition) {
	try {
	log->write("LogClassDefinition");
	log->write("Qualified Class Name:", convert->wchar_tStarToCharStar(classDefinition->GetQualifiedName()));
	log->write("Class Type:", this->GetClassTypeName(classDefinition->GetClassType()));
	log->write("Is Abstract:", convert->boolToCharStar(classDefinition->GetIsAbstract()));
	log->write("Is Computed:", convert->boolToCharStar(classDefinition->GetIsComputed()));
	FdoClassCapabilities * classCapabilities = classDefinition->GetCapabilities();
	if (classCapabilities != NULL) {
		log->write("Class Capabilities");
		this->LogClassCapabilities(classCapabilities);
	} else {
		log->write("GetCapabilities() returned NULL");
	}
	FdoPropertyDefinitionCollection * propertyDefinitionCollection;
	propertyDefinitionCollection = classDefinition->GetProperties();
	if (propertyDefinitionCollection->GetCount() != 0 ) {
		log->write("Class Properties");
		this->LogPropertyDefinitionCollection(propertyDefinitionCollection);
	} else {
		log->write("Class GetProperties count is 0");
	}
	FdoClassDefinition * baseClass = classDefinition->GetBaseClass();
	FdoDataPropertyDefinitionCollection * identityDataPropertyDefinitionCollection;
	if (baseClass == NULL) {
		log->write("This class is a base class");
		identityDataPropertyDefinitionCollection = classDefinition->GetIdentityProperties();
		log->write("Identity Properties");
		this->LogPropertyDefinitionCollection((FdoPropertyDefinitionCollection *)identityDataPropertyDefinitionCollection);
	} else {
//		log->write("This class has a base class and its definition is as follows:");
//		this->LogClassDefinition(baseClass);
		FdoReadOnlyPropertyDefinitionCollection * baseProperties = classDefinition->GetBaseProperties();
		log->write("This class has a base class and a call to GetBaseProperties() returns the following:");
		this->LogReadOnlyPropertyDefinitionCollection(baseProperties);
	}
	}
	catch (FdoException * ex) {
		log->write("DescribeSchema checkpoint exception message:");
		utilities->PrintException(ex);
		ex->Release();
	}
}

void
FdoInfo::LogCommandCapabilities(FdoICommandCapabilities * capabilities) {
	FdoInt32 numCommandTypes = 0;
	FdoInt32 * commandTypes;
	commandTypes = capabilities->GetCommands(numCommandTypes);
	log->write("Number of Command Types:", convert->FdoInt32ToCharStar(numCommandTypes));
	for (int i = 0; i < numCommandTypes; i++) {
		log->write("Command Type:", this->GetCommandTypeName(commandTypes[i]));
	}
	log->write("Supports Parameters:", convert->boolToCharStar(capabilities->SupportsParameters()));
	log->write("Supports Timeout:", convert->boolToCharStar(capabilities->SupportsTimeout()));
	log->write("Supports Select Expressions:", convert->boolToCharStar(capabilities->SupportsSelectExpressions()));
	log->write("Supports Select Functions:", convert->boolToCharStar(capabilities->SupportsSelectFunctions()));
	log->write("Supports Select Distinct:", convert->boolToCharStar(capabilities->SupportsSelectDistinct()));
	log->write("Supports Select Ordering:", convert->boolToCharStar(capabilities->SupportsSelectOrdering()));
	log->write("Supports Select Grouping:", convert->boolToCharStar(capabilities->SupportsSelectGrouping()));
}


void
FdoInfo::LogConnectionCapabilities(FdoIConnectionCapabilities * capabilities) {
	FdoInt32 numSpatialContexts = 0;
	FdoSpatialContextExtentType * spatialContextTypes;
	spatialContextTypes = capabilities->GetSpatialContextTypes(numSpatialContexts);
	log->write("Number of Spatial Contexts:", convert->FdoInt32ToCharStar(numSpatialContexts));
	for (int i = 0; i < numSpatialContexts; i++) {
		log->write("Spatial Context:", this->GetSpatialContextExtentTypeName(spatialContextTypes[i]));
	}
	FdoInt32 numLockTypes = 0;
	FdoLockType * lockTypes;
	lockTypes = capabilities->GetLockTypes(numLockTypes);
	log->write("Number of Lock Types:", convert->FdoInt32ToCharStar(numLockTypes));
	for (int i = 0; i < numLockTypes; i++) {
		log->write("Lock Type:", this->GetLockTypeName(lockTypes[i]));
	}
	log->write("Thread Capability:", this->GetThreadCapabilityName(capabilities->GetThreadCapability()));
	bool supportsLocking = capabilities->SupportsLocking();
	log->write("Supports Locking:", convert->boolToCharStar(supportsLocking));
	log->write("Supports Timeout:", convert->boolToCharStar(capabilities->SupportsTimeout()));
	log->write("Supports Transactions:", convert->boolToCharStar(capabilities->SupportsTransactions()));
	log->write("Supports Long Transactions:", convert->boolToCharStar(capabilities->SupportsLongTransactions()));
	log->write("Supports SQL:", convert->boolToCharStar(capabilities->SupportsSQL()));
	log->write("Supports Configuration:", convert->boolToCharStar(capabilities->SupportsConfiguration()));
}

void
FdoInfo::LogConnectionInfo(FdoIConnectionInfo * connectionInfo) {
	FdoIConnectionPropertyDictionary * connectionPropertyDictionary;
	FdoString ** connectionPropertyNames;
	FdoInt32 propertyNameCount = 0;
	log->write("FdoIConnectionInfo");
	log->write("Provider Name:", convert->wchar_tStarToCharStar(connectionInfo->GetProviderName()));
	log->write("Provider Description:", convert->wchar_tStarToCharStar(connectionInfo->GetProviderDescription()));
	log->write("Provider Version:", convert->wchar_tStarToCharStar(connectionInfo->GetProviderVersion()));
	log->write("Feature Data Objects Version:", convert->wchar_tStarToCharStar(connectionInfo->GetFeatureDataObjectsVersion()));
	connectionPropertyDictionary = connectionInfo->GetConnectionProperties();
	connectionPropertyNames = connectionPropertyDictionary->GetPropertyNames(propertyNameCount);
	for(int i = 0; i < propertyNameCount; i++) {
		this->LogConnectionPropertyAttributes(connectionPropertyDictionary, connectionPropertyNames[i]);
	}
}

void
FdoInfo::LogConnectionPropertyAttributes(FdoIConnectionPropertyDictionary * dictionary,
	FdoString * propertyName) {
	log->write("Property Name:", convert->wchar_tStarToCharStar(propertyName));
	log->write("Localized Name:", convert->wchar_tStarToCharStar(dictionary->GetLocalizedName(propertyName)));
	log->write("Property Default Value:", convert->wchar_tStarToCharStar(dictionary->GetPropertyDefault(propertyName)));
	log->write("Property Actual Value:", convert->wchar_tStarToCharStar(dictionary->GetProperty(propertyName)));
	log->write("Is Property Required:", convert->boolToCharStar(dictionary->IsPropertyRequired(propertyName)));
	log->write("Is Property Protected:", convert->boolToCharStar(dictionary->IsPropertyProtected(propertyName)));
	bool isPropertyEnumerable = dictionary->IsPropertyEnumerable(propertyName);
	log->write("Is Property Enumerable:", convert->boolToCharStar(isPropertyEnumerable));
	if (isPropertyEnumerable) {
		FdoInt32 enumCount = 0;
		FdoString ** enumNames;
		enumNames = dictionary->EnumeratePropertyValues(propertyName, enumCount);
		log->write("Enum count:", convert->FdoInt32ToCharStar(enumCount));
		for (int i = 0; i < enumCount; i++) {
			log->write("Enumeration Name:", convert->wchar_tStarToCharStar(enumNames[i]));
		}
	}
}

void
FdoInfo::LogDataPropertyDefinition(FdoDataPropertyDefinition * dataPropertyDefinition) {
	log->write("Data Property Definition");
	log->write("Property Type:", this->GetPropertyTypeName(dataPropertyDefinition->GetPropertyType()));
	FdoDataType dataType = dataPropertyDefinition->GetDataType();
	log->write("Data Type:", this->GetDataTypeName(dataType));
	if (dataType == FdoDataType_String) {
		log->write("String Length:", convert->FdoInt32ToCharStar(dataPropertyDefinition->GetLength()));
	}
	if (dataType == FdoDataType_Decimal) {
		log->write("Precision:", convert->FdoInt32ToCharStar(dataPropertyDefinition->GetPrecision()));
		log->write("Scale:", convert->FdoInt32ToCharStar(dataPropertyDefinition->GetScale()));
	}
	log->write("Read-Only:", convert->boolToCharStar(dataPropertyDefinition->GetReadOnly()));
	log->write("Can be null:", convert->boolToCharStar(dataPropertyDefinition->GetNullable()));
	log->write("Default value:", convert->wchar_tStarToCharStar(dataPropertyDefinition->GetDefaultValue()));
	log->write("Is auto-generated:", convert->boolToCharStar(dataPropertyDefinition->GetIsAutoGenerated()));
}

void
FdoInfo::LogExpressionCapabilities(FdoIExpressionCapabilities * capabilities) {
	FdoInt32 numExpressionTypes = 0;
	FdoExpressionType * expressionTypes;
	expressionTypes = capabilities->GetExpressionTypes(numExpressionTypes);
	log->write("Number of Expression Types:", convert->FdoInt32ToCharStar(numExpressionTypes));
	for (int i = 0; i < numExpressionTypes; i++) {
		log->write("Expression Type:", this->GetExpressionTypeName(expressionTypes[i]));
	}
	FdoInt32 numFunctionDefinitions = 0;
	FdoFunctionDefinitionCollection * functions;
	FdoFunctionDefinition * funcDefinition;
	functions = capabilities->GetFunctions();
	numFunctionDefinitions = functions->GetCount();
	log->write("Number of Function Definitions:", convert->FdoInt32ToCharStar(numFunctionDefinitions));
	for (int i = 0; i < numFunctionDefinitions; i++) {
		funcDefinition = functions->GetItem(i);
		this->LogFunctionDefinition(funcDefinition);
	}
}

void
FdoInfo::LogFeatureReader(FdoIFeatureReader * featureReader) {
	FdoPtr<FdoClassDefinition> classDef;
	FdoPropertyDefinition * propertyDef;
	FdoPtr<FdoPropertyDefinitionCollection> properties;
	FdoInt32 numProperties = 0;
	FdoString * propertyName = NULL;
	FdoPropertyType propertyType;
	FdoDataType dataType;
	FdoDataPropertyDefinition * dataPropertyDef = NULL;
	FdoPtr<FdoByteArray> byteArray;
	FdoIGeometry * geometry;
	FdoGeometryType geometryType = FdoGeometryType_None;
	FdoIPolygon * polygon = NULL;
	FdoPtr<FdoILinearRing> exteriorRing;
	FdoPtr<FdoILinearRing> interiorRing;
	FdoPtr<FdoIDirectPosition> position;
	try {
		while (featureReader->ReadNext()) {
			// get the feature class object and its properties
			classDef = featureReader->GetClassDefinition();
			properties = classDef->GetProperties();
			// loop through the properties
			numProperties = properties->GetCount();
			for(int i = 0; i < numProperties; i++) {
				propertyDef = properties->GetItem(i);
				// get the property name and property type
				propertyName = propertyDef->GetName();
				log->write("Fully qualified property name:", convert->wchar_tStarToCharStar(propertyDef->GetQualifiedName()));
				propertyType = propertyDef->GetPropertyType();
				switch (propertyType) {
					// it's a data property
					case FdoPropertyType_DataProperty:
						dataPropertyDef = dynamic_cast<FdoDataPropertyDefinition *>(propertyDef);
						dataType = dataPropertyDef->GetDataType();
						switch (dataType) {
							case FdoDataType_Boolean:
								log->write("Boolean value:", convert->boolToCharStar(featureReader->GetBoolean(propertyName)));
								break;
							case FdoDataType_Int32:
								log->write("Int32 value:", convert->FdoInt32ToCharStar(featureReader->GetInt32(propertyName)));
								break;
							case FdoDataType_Int64:
								log->write("Int64 value:", convert->FdoInt64ToCharStar(featureReader->GetInt64(propertyName)));
								break;
							case FdoDataType_String:
								log->write("String value:", convert->wchar_tStarToCharStar(featureReader->GetString(propertyName)));
								break;
							default:
								log->write("Unknown FdoDataType");
						}
						break;
						// it's a geometric property
						// convert the byte array into a geometry
						// and determine the derived type of the geometry
					case FdoPropertyType_GeometricProperty:
						byteArray = featureReader->GetGeometry(propertyName);
						geometry  = geometryFactory->CreateGeometryFromFgf(byteArray);
						geometryType = geometry->GetDerivedType();
						// resolve the derived type into a list of ordinates
						switch (geometryType) {
							case FdoGeometryType_None:
								log->write("FdoGeometryType is None");
								break;
							case FdoGeometryType_Point:
								log->write("FdoGeometryType is a Point");
								break;
							case FdoGeometryType_LineString:
								log->write("FdoGeometryType is a Linestring");
								break;
							case FdoGeometryType_Polygon:
								log->write("FdoGeometryType is a Polygon");
								polygon = dynamic_cast<FdoIPolygon *>(geometry);
								LogPolygon(polygon);
								break;
							case FdoGeometryType_MultiPoint:
								log->write("FdoGeometryType is a MultiPoint");
								break;
							case FdoGeometryType_MultiLineString:
								log->write("FdoGeometryType is a MultiLineString");
								break;
							case FdoGeometryType_MultiPolygon:
								log->write("FdoGeometryType is a MultiPolygon");
								break;
							case FdoGeometryType_MultiGeometry:
								log->write("FdoGeometryType is a MultiGeometry");
								break;
							case FdoGeometryType_CurveString:
								log->write("FdoGeometryType is a CurveString");
								break;
							case FdoGeometryType_CurvePolygon:
								log->write("FdoGeometryType is a CurvePolygon");
								break;
							case FdoGeometryType_MultiCurveString:
								log->write("FdoGeometryType is a MultiCurveString");
								break;
							case FdoGeometryType_MultiCurvePolygon:
								log->write("FdoGeometryType is a MultiCurvePolygon");
								break;
							default:
								log->write("unknown FdoGeometryType");
						}
						break;
					default:
						log->write("Unknown FdoPropertyType");
				}
			}
		}
	} 
	catch (FdoException* ex) {
	   log->write("Connection close checkpoint exception message:");
	   utilities->PrintException(ex);
	   ex->Release();
   }
}

void
FdoInfo::LogFeatureSchemaCollection(FdoFeatureSchemaCollection * featureSchemaCollection) {
	FdoFeatureSchema * featureSchema;
	FdoInt32 numFeatureSchema = 0;
	numFeatureSchema = featureSchemaCollection->GetCount();
	log->write("Feature Schema Collection");
	for(int i = 0; i < numFeatureSchema; i++) {
		featureSchema = featureSchemaCollection->GetItem(i);
		this->LogFeatureSchema(featureSchema);
	}
}

void
FdoInfo::LogFeatureSchema(FdoFeatureSchema * featureSchema) {
	LogFeatureSchemaElement((FdoSchemaElement *) featureSchema);
	FdoClassCollection * classCollection;
	classCollection = featureSchema->GetClasses();
	if (classCollection != NULL) {
		FdoInt32 numClasses = classCollection->GetCount();
		log->write("Number of classes:", convert->FdoInt32ToCharStar(numClasses));
		FdoClassDefinition * classDefinition;
		for(int i = 0; i < numClasses; i++) {
			classDefinition = classCollection->GetItem(i);
			this->LogFeatureSchemaElement((FdoFeatureSchema *)classDefinition);
			this->LogClassDefinition(classDefinition);
		}
	} else {
		log->write("GetClasses() returned NULL");
	}
}

void
FdoInfo::LogFeatureSchemaElement(FdoSchemaElement * schemaElement) {
	log->write("Feature Schema Element Name:", convert->wchar_tStarToCharStar(schemaElement->GetName()));
	log->write("Fully Qualified Name:", convert->wchar_tStarToCharStar(schemaElement->GetQualifiedName()));
	log->write("Description:", convert->wchar_tStarToCharStar(schemaElement->GetDescription()));
	log->write("State:", this->GetSchemaElementStateName(schemaElement->GetElementState()));
	FdoFeatureSchema * partOf = schemaElement->GetFeatureSchema();
	if (partOf != NULL) {
		log->write("Feature Schema it is a part of:", convert->wchar_tStarToCharStar(partOf->GetName()));
	} else {
		log->write("This schema element is not a part of another feature schema.");
	}
	FdoSchemaElement * parent = schemaElement->GetParent();
	if (parent != NULL) {
		log->write("Parent Schema Element:", convert->wchar_tStarToCharStar(parent->GetName()));
	} else {
		log->write("This schema element does not have a parent.");
	}
	FdoSchemaAttributeDictionary * attributeDictionary;
	attributeDictionary = schemaElement->GetAttributes();
	FdoInt32 numAttributes = attributeDictionary->GetCount();
	log->write("Number of attributes:", convert->FdoInt32ToCharStar(numAttributes));
	FdoInt32 numAttrNames = 0;
	FdoString ** attributeNames = attributeDictionary->GetAttributeNames(numAttrNames);
	for(int i = 0; i < numAttrNames; i++) {
		log->write("Attribute Name:", convert->wchar_tStarToCharStar(attributeNames[i]));
		log->write("Attribute Value:", convert->wchar_tStarToCharStar(attributeDictionary->GetAttributeValue(attributeNames[i])));
	}
}

void
FdoInfo::LogFilterCapabilities(FdoIFilterCapabilities * capabilities) {
	FdoInt32 numConditionTypes = 0;
	FdoConditionType * conditionTypes;
	conditionTypes = capabilities->GetConditionTypes(numConditionTypes);
	log->write("Number of Condition Types:", convert->FdoInt32ToCharStar(numConditionTypes));
	for (int i = 0; i < numConditionTypes; i++) {
		log->write("Condition Type:", this->GetConditionTypeName(conditionTypes[i]));
	}
	FdoInt32 numSpatialOperations = 0;
	FdoSpatialOperations * spatialOperations;
	spatialOperations = capabilities->GetSpatialOperations(numSpatialOperations);
	log->write("Number of Spatial Operations:", convert->FdoInt32ToCharStar(numSpatialOperations));
	for (int i = 0; i < numSpatialOperations; i++) {
		log->write("Spatial Operation:", this->GetSpatialOperationName(spatialOperations[i]));
	}
	FdoInt32 numDistanceOperations = 0;
	FdoDistanceOperations * distanceOperations;
	distanceOperations = capabilities->GetDistanceOperations(numDistanceOperations);
	log->write("Number of Distance Operations:", convert->FdoInt32ToCharStar(numDistanceOperations));
	for (int i = 0; i < numDistanceOperations; i++) {
		log->write("Distance Operation:", this->GetDistanceOperationName(distanceOperations[i]));
	}
	bool supportsGeodesicDistance;
	supportsGeodesicDistance = capabilities->SupportsGeodesicDistance();
	log->write("Supports Geodesic Distance:", convert->boolToCharStar(supportsGeodesicDistance));
	log->write("Supports Non Literal Geometric Operations:", convert->boolToCharStar(capabilities->SupportsNonLiteralGeometricOperations()));
}

void
FdoInfo::LogFunctionDefinition(FdoFunctionDefinition * funcDefinition) {
	log->write("Function Name:", convert->wchar_tStarToCharStar(funcDefinition->GetName()));
	log->write("Function Description:", convert->wchar_tStarToCharStar(funcDefinition->GetDescription()));
	log->write("Function Arguments");
	FdoReadOnlyArgumentDefinitionCollection * arguments;
	FdoArgumentDefinition * argDefinition;
	arguments = funcDefinition->GetArguments();
	FdoInt32 numArgumentDefinitions = 0;
	numArgumentDefinitions = arguments->GetCount();
	log->write("Number of Argument Definitions:", convert->FdoInt32ToCharStar(numArgumentDefinitions));
	for (int i = 0; i < numArgumentDefinitions; i++) {
		argDefinition = arguments->GetItem(i);
		this->LogArgumentDefinition(argDefinition);
	}
	log->write("Return Data Type:", this->GetDataTypeName(funcDefinition->GetReturnType()));
}

void
FdoInfo::LogGeometryCapabilities(FdoIGeometryCapabilities * capabilities) {
	FdoInt32 numGeometryTypes = 0;
	FdoGeometryType * geometryTypes;
	geometryTypes = capabilities->GetGeometryTypes(numGeometryTypes);
	log->write("Number of Geometry Types:", convert->FdoInt32ToCharStar(numGeometryTypes));
	for (int i = 0; i < numGeometryTypes; i++) {
		log->write("Geometry Type:", this->GetGeometryTypeName(geometryTypes[i]));
	}
	FdoInt32 numGeometryComponentTypes = 0;
	FdoGeometryComponentType * geometryComponentTypes;
	geometryComponentTypes = capabilities->GetGeometryComponentTypes(numGeometryComponentTypes);
	log->write("Number of Geometry Component Types:", convert->FdoInt32ToCharStar(numGeometryComponentTypes));
	for (int i = 0; i < numGeometryComponentTypes; i++) {
		log->write("Geometry Component Type:", this->GetGeometryComponentTypeName(geometryComponentTypes[i]));
	}
	FdoInt32 dimensionality = 0;
	dimensionality = capabilities->GetDimensionalities();
	log->write("Dimensionalities");
	log->write("Dimensionality:", this->GetDimensionalityName(FdoDimensionality_XY));
	if (dimensionality & FdoDimensionality_Z) {
		log->write("Dimensionality:", this->GetDimensionalityName(FdoDimensionality_Z));
	}
	if (dimensionality & FdoDimensionality_M) {
		log->write("Dimensionality:", this->GetDimensionalityName(FdoDimensionality_M));
	}
}

void
FdoInfo::LogGeometricPropertyDefinition(FdoGeometricPropertyDefinition * geometricPropertyDefinition) {
	log->write("Geometric Property Definition");
	log->write("Property Type:", this->GetPropertyTypeName(geometricPropertyDefinition->GetPropertyType()));
	FdoInt32 geometricTypes = geometricPropertyDefinition->GetGeometryTypes();
	this->LogFdoGeometricTypes(geometricTypes);
	log->write("Read-Only:", convert->boolToCharStar(geometricPropertyDefinition->GetReadOnly()));
	log->write("Has elevation:", convert->boolToCharStar(geometricPropertyDefinition->GetHasElevation()));
	log->write("Has measure:", convert->boolToCharStar(geometricPropertyDefinition->GetHasMeasure()));
	log->write("Spatial Context Association:", convert->wchar_tStarToCharStar(geometricPropertyDefinition->GetSpatialContextAssociation()));
}

void
FdoInfo::LogFdoGeometricTypes(FdoInt32 geometricTypes) {
	if (geometricTypes & FdoGeometricType_Point) {
		log->write("FdoGeometricType_Point");
	}
	if (geometricTypes & FdoGeometricType_Curve) {
		log->write("FdoGeometricType_Curve");
	}
	if (geometricTypes & FdoGeometricType_Surface) {
		log->write("FdoGeometricType_Surface");
	}
	if (geometricTypes & FdoGeometricType_Solid) {
		log->write("FdoGeometricType_Solid");
	}
}

void
FdoInfo::LogLinearRing(FdoILinearRing * linearRing) {
	FdoIDirectPosition * position = NULL;
	FdoInt32 dimensionality = linearRing->GetDimensionality();
	log->write("Linear Ring Dimensionality:", convert->FdoInt32ToCharStar(dimensionality));
	FdoInt32 numItems = linearRing->GetCount();
	strcpy(sprintfBuf, "No ordinates");
	for(int i=0; i<numItems; i++) {
		position = linearRing->GetItem(i);
		if (dimensionality & FdoDimensionality_Z && dimensionality & FdoDimensionality_M) {
			sprintf(sprintfBuf, "X: %f, Y: %f, Z: %f, M: %f", position->GetX(), position->GetY(), position->GetZ(), position->GetM());
		} else if (dimensionality & FdoDimensionality_Z && !(dimensionality & FdoDimensionality_M)) {
			sprintf(sprintfBuf, "X: %f, Y: %f, Z: %f", position->GetX(), position->GetY(), position->GetZ());
		} else {
			sprintf(sprintfBuf, "X: %f, Y: %f", position->GetX(), position->GetY());
		}
		log->write(sprintfBuf);
	}
}


void
FdoInfo::LogObjectPropertyDefinition(FdoObjectPropertyDefinition * objectPropertyDefinition) {
	log->write("Object Property Definition");
	log->write("Property Type:", this->GetPropertyTypeName(objectPropertyDefinition->GetPropertyType()));
	FdoClassDefinition * classDefinition = objectPropertyDefinition->GetClass();
	this->LogClassDefinition(classDefinition);
	FdoObjectType objectType = objectPropertyDefinition->GetObjectType();
	log->write("Object type:", this->GetObjectTypeName(objectType));
	FdoDataPropertyDefinition * dataPropertyDefinition;
	if (objectType == FdoObjectType_Collection || objectType == FdoObjectType_OrderedCollection) {
		dataPropertyDefinition = objectPropertyDefinition->GetIdentityProperty();
		log->write("Identity Property");
		this->LogDataPropertyDefinition(dataPropertyDefinition);
	}
	FdoOrderType orderType;
	if (objectType == FdoObjectType_OrderedCollection) {
		orderType = objectPropertyDefinition->GetOrderType();
		log->write("Order type:", this->GetOrderTypeName(orderType));
	}
}

void
FdoInfo::LogPolygon(FdoIPolygon * polygon) {
	char buf[64];
	FdoILinearRing * exteriorRing = polygon->GetExteriorRing();
	log->write("Polygon exterior linear ring");
	LogLinearRing(exteriorRing);
	FdoInt32 numInteriorRings = polygon->GetInteriorRingCount();
	FdoILinearRing * interiorRing = NULL;
	for(int i=0; i<numInteriorRings; i++) {
		interiorRing = polygon->GetInteriorRing(i);
#ifdef _WIN32
		_itoa(i, buf, 10);
#else
		sprintf(buf, "%d", i);
#endif
		log->write("Polygon Interior Ring number:", buf);
		LogLinearRing(interiorRing);
	}
}


void
FdoInfo::LogPropertyDefinition(FdoPropertyDefinition * propertyDefinition) {
	log->write("Property Qualifed Name:", convert->wchar_tStarToCharStar(propertyDefinition->GetQualifiedName()));
	log->write("Is System Property:", convert->boolToCharStar(propertyDefinition->GetIsSystem()));
	FdoPropertyType propertyType = propertyDefinition->GetPropertyType();
	log->write("Property Type:", this->GetPropertyTypeName(propertyType));
	switch(propertyType) {
		case FdoPropertyType_DataProperty : this->LogDataPropertyDefinition((FdoDataPropertyDefinition *)propertyDefinition);
			break;
		case FdoPropertyType_ObjectProperty : this->LogObjectPropertyDefinition((FdoObjectPropertyDefinition *)propertyDefinition);
			break;
		case FdoPropertyType_GeometricProperty : this->LogGeometricPropertyDefinition((FdoGeometricPropertyDefinition *)propertyDefinition);
			break;
		case FdoPropertyType_AssociationProperty : this->LogAssociationPropertyDefinition((FdoAssociationPropertyDefinition *)propertyDefinition);
			break;
		case FdoPropertyType_RasterProperty : this->LogRasterPropertyDefinition((FdoRasterPropertyDefinition *)propertyDefinition);
			break;
		default : log->write("Unknown FdoPropertyType:", convert->FdoInt32ToCharStar((FdoInt32)propertyType));
	}
}


void
FdoInfo::LogPropertyDefinitionCollection(FdoPropertyDefinitionCollection * propertyDefinitionCollection) {
	try {
	FdoInt32 numProperties = propertyDefinitionCollection->GetCount();
	log->write("Number of Property Definitions:", convert->FdoInt32ToCharStar(numProperties));
	FdoPropertyDefinition * propertyDefinition;
	for(int i = 0; i < numProperties; i++) {
		propertyDefinition = propertyDefinitionCollection->GetItem(i);
		if (propertyDefinition != NULL) {
			this->LogPropertyDefinition(propertyDefinition);
		} else {
			log->write("LogPropertyDefinitionCollection: collection count > 0 but no item found");
			break;
		}
	}
	}
	catch (FdoException * ex) {
		log->write("LogPropertyDefinitionCollection exception message:");
		utilities->PrintException(ex);
		ex->Release();
	}
}

void
FdoInfo::LogReadOnlyPropertyDefinitionCollection(FdoReadOnlyPropertyDefinitionCollection * propertyDefinitionCollection) {
	try {
	FdoInt32 numProperties = propertyDefinitionCollection->GetCount();
	log->write("Number of Property Definitions:", convert->FdoInt32ToCharStar(numProperties));
	FdoPropertyDefinition * propertyDefinition;
	for(int i = 0; i < numProperties; i++) {
		propertyDefinition = propertyDefinitionCollection->GetItem(i);
		if (propertyDefinition != NULL) {
			this->LogPropertyDefinition(propertyDefinition);
		} else {
			log->write("LogPropertyDefinitionCollection: collection count > 0 but no item found");
			break;
		}
	}
	}
	catch (FdoException * ex) {
		log->write("LogPropertyDefinitionCollection exception message:");
		utilities->PrintException(ex);
		ex->Release();
	}
}

void
FdoInfo::LogProviderInfo(const FdoProvider * provider) {
	log->write("Name:", convert->wchar_tStarToCharStar(provider->GetName()));
	log->write("Display Name:", convert->wchar_tStarToCharStar(provider->GetDisplayName()));
	log->write("Description:", convert->wchar_tStarToCharStar(provider->GetDescription()));
	log->write("Version:", convert->wchar_tStarToCharStar(provider->GetVersion()));
	log->write("FDO Version:", convert->wchar_tStarToCharStar(provider->GetFeatureDataObjectsVersion()));
	log->write("Library Path:", convert->wchar_tStarToCharStar(provider->GetLibraryPath()));
	log->write("Is Managed:", convert->boolToCharStar(provider->GetIsManaged()));
}

void
FdoInfo::LogRasterPropertyDefinition(FdoRasterPropertyDefinition * rasterPropertyDefinition) {
	log->write("Raster Property Definition");
	log->write("Property Type:", this->GetPropertyTypeName(rasterPropertyDefinition->GetPropertyType()));
	log->write("Read-Only:", convert->boolToCharStar(rasterPropertyDefinition->GetReadOnly()));
	log->write("Can be null:", convert->boolToCharStar(rasterPropertyDefinition->GetNullable()));
	FdoRasterDataModel * dataModel = rasterPropertyDefinition->GetDefaultDataModel();
	log->write("Default Data Model");
	this->LogRasterDataModelProperties(dataModel);
	log->write("Default Image X Size:", convert->FdoInt32ToCharStar(rasterPropertyDefinition->GetDefaultImageXSize()));
	log->write("Default Image Y Size:", convert->FdoInt32ToCharStar(rasterPropertyDefinition->GetDefaultImageYSize()));
}


void
FdoInfo::LogRasterCapabilities(FdoIRasterCapabilities * capabilities) {
	log->write("Supports Raster:", convert->boolToCharStar(capabilities->SupportsRaster()));
	log->write("Supports Stitching:", convert->boolToCharStar(capabilities->SupportsStitching()));
	log->write("Supports Subsampling:", convert->boolToCharStar(capabilities->SupportsSubsampling()));
	for(int i = 0; i < numRasterDataModels; i++) {
		log->timestr();
		log->str("Supports");
		log->str(this->GetRasterDataModelTypeName((FdoRasterDataModelType)i));
		log->str(":");
		log->str(convert->boolToCharStar(capabilities->SupportsDataModel(rasterDataModels[i])));
		log->endline();
	}
}


void
FdoInfo::LogRasterDataModelProperties(FdoRasterDataModel * rasterDataModel) {
	log->write("Raster Data Model Type:", this->GetRasterDataModelTypeName(rasterDataModel->GetDataModelType()));
	log->write("Bits Per Pixel:", convert->FdoInt32ToCharStar(rasterDataModel->GetBitsPerPixel()));
	log->write("Raster Data Organization:", this->GetRasterDataOrganizationName(rasterDataModel->GetOrganization()));
	log->write("Tile Size X:", convert->FdoInt32ToCharStar(rasterDataModel->GetTileSizeX()));
	log->write("Tile Size Y:", convert->FdoInt32ToCharStar(rasterDataModel->GetTileSizeY()));
}

void
FdoInfo::LogSchemaCapabilities(FdoISchemaCapabilities * capabilities){
	FdoInt32 numClassTypes = 0;
	FdoClassType * classTypes;
	classTypes = capabilities->GetClassTypes(numClassTypes);
	log->write("Number of Class Types:", convert->FdoInt32ToCharStar(numClassTypes));
	for (int i = 0; i < numClassTypes; i++) {
		log->write("Class Type:", this->GetClassTypeName(classTypes[i]));
	}
	FdoInt32 numDataTypes = 0;
	FdoDataType * dataTypes;
	dataTypes = capabilities->GetDataTypes(numDataTypes);
	log->write("Number of Data Types:", convert->FdoInt32ToCharStar(numDataTypes));
	for (int i = 0; i < numDataTypes; i++) {
		log->write("Data Type:", this->GetDataTypeName(dataTypes[i]));
	}
	log->write("Supports Inheritance:", convert->boolToCharStar(capabilities->SupportsInheritance()));
	log->write("Supports Multiple Schemas:", convert->boolToCharStar(capabilities->SupportsMultipleSchemas()));
	log->write("Supports Object Properties:", convert->boolToCharStar(capabilities->SupportsObjectProperties()));
	log->write("Supports Association Properties:", convert->boolToCharStar(capabilities->SupportsAssociationProperties()));
	log->write("Supports Schema Overrides:", convert->boolToCharStar(capabilities->SupportsSchemaOverrides()));
	log->write("Supports Network Model:", convert->boolToCharStar(capabilities->SupportsNetworkModel()));
	log->write("Supports Auto Id Generation:", convert->boolToCharStar(capabilities->SupportsAutoIdGeneration()));
	log->write("Supports Data Store Scope Unique Id Generation:", convert->boolToCharStar(capabilities->SupportsDataStoreScopeUniqueIdGeneration()));
	dataTypes = capabilities->GetSupportedAutoGeneratedTypes(numDataTypes);
	log->write("Number of Auto Generated Data Types:", convert->FdoInt32ToCharStar(numDataTypes));
	for (int i = 0; i < numDataTypes; i++) {
		log->write("Data Type:", this->GetDataTypeName(dataTypes[i]));
	}
	log->write("Supports Schema Modification:", convert->boolToCharStar(capabilities->SupportsSchemaModification()));

}

void
FdoInfo::LogTopologyCapabilities(FdoITopologyCapabilities * capabilities) {
	log->write("Supports Topology:", convert->boolToCharStar(capabilities->SupportsTopology()));
	log->write("Supports Topological Hierarchy:", convert->boolToCharStar(capabilities->SupportsTopologicalHierarchy()));
	log->write("Breaks Curve Crossings Automatically:", convert->boolToCharStar(capabilities->BreaksCurveCrossingsAutomatically()));
	log->write("Activates Topology By Area:", convert->boolToCharStar(capabilities->ActivatesTopologyByArea()));
	log->write("Constrains Feature Movements:", convert->boolToCharStar(capabilities->ConstrainsFeatureMovements()));
}




char *
FdoInfo::GetConnectionStateName(FdoConnectionState connectionStateEnum) {
	switch (connectionStateEnum) {
		case FdoConnectionState_Busy : return "FdoConnectionState_Busy";
		case FdoConnectionState_Closed : return "FdoConnectionState_Closed";
		case FdoConnectionState_Open : return "FdoConnectionState_Open";
		case FdoConnectionState_Pending : return "FdoConnectionState_Pending";
		default : return "UNKNOWN_ENUM_VALUE";
	}
}

char *
FdoInfo::GetThreadCapabilityName(FdoThreadCapability threadCapabilityEnum) {
	switch (threadCapabilityEnum) {
		case FdoThreadCapability_SingleThreaded : return "FdoThreadCapability_SingleThreaded";
		case FdoThreadCapability_PerConnectionThreaded : return "FdoThreadCapability_PerConnectionThreaded";
		case FdoThreadCapability_PerCommandThreaded : return "FdoThreadCapability_PerCommandThreaded";
		case FdoThreadCapability_MultiThreaded : return "FdoThreadCapability_MultiThreaded";
		default : return "UNKNOWN_ENUM_VALUE";
	}
}

char *
FdoInfo::GetSpatialContextExtentTypeName(FdoSpatialContextExtentType spatialContextExtentTypeEnum) {
	switch (spatialContextExtentTypeEnum) {
		case FdoSpatialContextExtentType_Static : return "FdoSpatialContextExtentType_Static";
		case FdoSpatialContextExtentType_Dynamic : return "FdoSpatialContextExtentType_Dynamic";
		default : return "UNKNOWN_ENUM_VALUE";
	}
}

char *
FdoInfo::GetLockTypeName(FdoLockType lockTypeEnum) {
	switch (lockTypeEnum) {
		case FdoLockType_None : return "FdoLockType_None";
		case FdoLockType_Shared : return "FdoLockType_Shared";
		case FdoLockType_Exclusive : return "FdoLockType_Exclusive";
		case FdoLockType_Transaction : return "FdoLockType_Transaction";
		case FdoLockType_Unsupported : return "FdoLockType_Unsupported";
		case FdoLockType_AllLongTransactionExclusive : return "FdoLockType_AllLongTransactionExclusive";
		case FdoLockType_LongTransactionExclusive : return "FdoLockType_LongTransactionExclusive";
		default : return "UNKNOWN_ENUM_VALUE";
	}
}

char *
FdoInfo::GetPropertyTypeName(FdoPropertyType propertyTypeEnum) {
	switch (propertyTypeEnum) {
		case FdoPropertyType_DataProperty : return "FdoPropertyType_DataProperty";
		case FdoPropertyType_ObjectProperty : return "FdoPropertyType_ObjectProperty";
		case FdoPropertyType_GeometricProperty : return "FdoPropertyType_GeometricProperty";
		case FdoPropertyType_AssociationProperty : return "FdoPropertyType_AssociationProperty";
		case FdoPropertyType_RasterProperty : return "FdoPropertyType_RasterProperty";
		default : return "UNKNOWN_ENUM_VALUE";
	}
}


char *
FdoInfo::GetClassTypeName(FdoClassType classTypeEnum) {
	switch (classTypeEnum) {
		case FdoClassType_Class : return "FdoClassType_Class";
		case FdoClassType_FeatureClass : return "FdoClassType_FeatureClass";
		case FdoClassType_NetworkClass : return "FdoClassType_networkClass";
		case FdoClassType_NetworkLayerClass : return "FdoClassType_NetworkLayerClass";
		case FdoClassType_NetworkNodeClass : return "FdoClassType_NetworkNodeClass";
		case FdoClassType_NetworkLinkClass : return "FdoClassType_NetworkLinkClass";
		default : return "UNKNOWN_ENUM_VALUE";
	}
}


char *
FdoInfo::GetCommandTypeName(FdoInt32 commandTypeInt) {
	switch (commandTypeInt) {
		case FdoCommandType_Select : return "FdoCommandType_Select";
		case FdoCommandType_Insert : return "FdoCommandType_Insert";
		case FdoCommandType_Delete : return "FdoCommandType_Delete";
		case FdoCommandType_Update : return "FdoCommandType_Update";
		case FdoCommandType_DescribeSchema : return "FdoCommandType_DescribeSchema";
		case FdoCommandType_DescribeSchemaMapping : return "FdoCommandType_DescribeSchemaMapping";
		case FdoCommandType_ApplySchema : return "FdoCommandType_ApplySchema";
		case FdoCommandType_DestroySchema : return "FdoCommandType_DestroySchema";
		case FdoCommandType_ActivateSpatialContext : return "FdoCommandType_ActivateSpatialContext";
		case FdoCommandType_CreateSpatialContext : return "FdoCommandType_CreateSpatialContext";
		case FdoCommandType_DestroySpatialContext : return "FdoCommandType_DestroySpatialContext";
		case FdoCommandType_GetSpatialContexts : return "FdoCommandType_GetSpatialContexts";
		case FdoCommandType_CreateMeasureUnit : return "FdoCommandType_CreateMeasureUnit";
		case FdoCommandType_DestroyMeasureUnit : return "FdoCommandType_DestroyMeasureUnit";
		case FdoCommandType_GetMeasureUnits : return "FdoCommandType_GetMeasureUnits";
		case FdoCommandType_SQLCommand : return "FdoCommandType_SQLCommand";
		case FdoCommandType_AcquireLock : return "FdoCommandType_AcquireLock";
		case FdoCommandType_GetLockInfo : return "FdoCommandType_GetLockInfo";
		case FdoCommandType_GetLockedObjects : return "FdoCommandType_GetLockedObjects";
		case FdoCommandType_GetLockOwners : return "FdoCommandType_GetLockOwners";
		case FdoCommandType_ReleaseLock : return "FdoCommandType_ReleaseLock";
		case FdoCommandType_ActivateLongTransaction : return "FdoCommandType_ActivateLongTransaction";
		case FdoCommandType_DeactivateLongTransaction : return "FdoCommandType_DeactivateLongTransaction";
		case FdoCommandType_CommitLongTransaction : return "FdoCommandType_CommitLongTransaction";
		case FdoCommandType_CreateLongTransaction : return "FdoCommandType_CreateLongTransaction";
		case FdoCommandType_GetLongTransactions : return "FdoCommandType_GetLongTransactions";
		case FdoCommandType_FreezeLongTransaction : return "FdoCommandType_FreezeLongTransaction";
		case FdoCommandType_RollbackLongTransaction : return "FdoCommandType_RollbackLongTransaction";
		case FdoCommandType_ActivateLongTransactionCheckpoint : return "FdoCommandType_ActivateLongTransactionCheckpoint";
		case FdoCommandType_CreateLongTransactionCheckpoint : return "FdoCommandType_CreateLongTransactionCheckpoint";
		case FdoCommandType_GetLongTransactionCheckpoints : return "FdoCommandType_GetLongTransactionCheckpoints";
		case FdoCommandType_RollbackLongTransactionCheckpoint : return "FdoCommandType_RollbackLongTransactionCheckpoint";
		case FdoCommandType_ChangeLongTransactionPrivileges : return "FdoCommandType_ChangeLongTransactionPrivileges";
		case FdoCommandType_GetLongTransactionPrivileges : return "FdoCommandType_GetLongTransactionPrivileges";
		case FdoCommandType_ChangeLongTransactionSet : return "FdoCommandType_ChangeLongTransactionSet";
		case FdoCommandType_GetLongTransactionsInSet : return "FdoCommandType_GetLongTransactionsInSet";
		case FdoCommandType_NetworkShortestPath : return "FdoCommandType_NetworkShortestPath";
		case FdoCommandType_NetworkAllPaths : return "FdoCommandType_NetworkAllPaths";
		case FdoCommandType_NetworkReachableNodes : return "FdoCommandType_NetworkReachableNodes";
		case FdoCommandType_NetworkReachingNodes : return "FdoCommandType_NetworkReachingNodes";
		case FdoCommandType_NetworkNearestNeighbors : return "FdoCommandType_NetworkNearestNeighbors";
		case FdoCommandType_NetworkWithinCost : return "FdoCommandType_NetworkWithinCost";
		case FdoCommandType_NetworkTSP : return "FdoCommandType_NetworkTSP";
		case FdoCommandType_ActivateTopologyArea : return "FdoCommandType_ActivateTopologyArea";
		case FdoCommandType_DeactivateTopologyArea : return "FdoCommandType_DeactivateTopologyArea";
		case FdoCommandType_ActivateTopologyInCommandResult : return "FdoCommandType_ActivateTopologyInCommandResult";
		case FdoCommandType_DeactivateTopologyInCommandResults : return "FdoCommandType_DeactivateTopologyInCommandResults";
		case FdoCommandType_SelectAggregates : return "FdoCommandType_SelectAggregates";
		case FdoCommandType_FirstProviderCommand : return "FdoCommandType_FirstProviderCommand";
		default : sprintf(errString, "%hs: %d", "UNKNOWN_ENUM_VALUE", commandTypeInt);
			return errString;
	}
}

char *
FdoInfo::GetConditionTypeName(FdoConditionType conditionTypeEnum) {
	switch (conditionTypeEnum) {
		case FdoConditionType_Comparison : return "FdoConditionType_Comparison";
		case FdoConditionType_Like : return "FdoConditionType_Like";
		case FdoConditionType_In : return "FdoConditionType_In";
		case FdoConditionType_Null : return "FdoConditionType_Null";
		case FdoConditionType_Spatial : return "FdoConditionType_Spatial";
		case FdoConditionType_Distance : return "FdoConditionType_Distance";
		default : return "UNKNOWN_ENUM_VALUE";
	}
}

char *
FdoInfo::GetDataTypeName(FdoDataType dataTypeEnum) {
	switch (dataTypeEnum) {
		case FdoDataType_Boolean : return "FdoDataType_Boolean";
		case FdoDataType_Byte : return "FdoDataType_Byte";
		case FdoDataType_DateTime : return "FdoDataType_DateTime";
		case FdoDataType_Decimal : return "FdoDataType_Decimal";
		case FdoDataType_Double : return "FdoDataType_Double";
		case FdoDataType_Int16 : return "FdoDataType_Int16";
		case FdoDataType_Int32 : return "FdoDataType_Int32";
		case FdoDataType_Int64 : return "FdoDataType_Int64";
		case FdoDataType_Single : return "FdoDataType_Single";
		case FdoDataType_String : return "FdoDataType_String";
		case FdoDataType_BLOB : return "FdoDataType_BLOB";
		case FdoDataType_CLOB : return "FdoDataType_CLOB";
		default : return "UNKNOWN_ENUM_VALUE";
	}
}

char *
FdoInfo::GetDeleteRuleTypeName(FdoDeleteRule deleteRuleTypeEnum) {
	switch (deleteRuleTypeEnum) {
		case FdoDeleteRule_Cascade : return "FdoDeleteRule_Cascade";
		case FdoDeleteRule_Prevent : return "FdoDeleteRule_Prevent";
		case FdoDeleteRule_Break : return "FdoDeleteRule_Break";
		default : return "UNKNOWN_ENUM_VALUE";
	}
}

char *
FdoInfo::GetDistanceOperationName(FdoDistanceOperations distanceOperationEnum) {
	switch (distanceOperationEnum) {
		case FdoDistanceOperations_Beyond : return "FdoDistanceOperations_Beyond";
		case FdoDistanceOperations_Within : return "FdoDistanceOperations_Within";
		default : return "UNKNOWN_ENUM_VALUE";
	}
}

char *
FdoInfo::GetSpatialOperationName(FdoSpatialOperations spatialOperationEnum) {
	switch (spatialOperationEnum) {
		case FdoSpatialOperations_Contains : return "FdoSpatialOperations_Contains";
		case FdoSpatialOperations_Crosses : return "FdoSpatialOperations_Crosses";
		case FdoSpatialOperations_Disjoint : return "FdoSpatialOperations_Disjoint";
		case FdoSpatialOperations_Equals : return "FdoSpatialOperations_Equals";
		case FdoSpatialOperations_Intersects : return "FdoSpatialOperations_Intersects";
		case FdoSpatialOperations_Overlaps : return "FdoSpatialOperations_Overlaps";
		case FdoSpatialOperations_Touches : return "FdoSpatialOperations_Touches";
		case FdoSpatialOperations_Within : return "FdoSpatialOperations_Within";
		case FdoSpatialOperations_CoveredBy : return "FdoSpatialOperations_CoveredBy";
		case FdoSpatialOperations_Inside : return "FdoSpatialOperations_Inside";
		case FdoSpatialOperations_EnvelopeIntersects : return "FdoSpatialOperations_EnvelopeIntersects";
		default : return "UNKNOWN_ENUM_VALUE";
	}
}

char *
FdoInfo::GetSchemaElementStateName(FdoSchemaElementState state) {
	switch (state) {
		case FdoSchemaElementState_Added : return "FdoSchemaElementState_Added";
		case FdoSchemaElementState_Deleted : return "FdoSchemaElementState_Deleted";
		case FdoSchemaElementState_Detached : return "FdoSchemaElementState_Detached";
		case FdoSchemaElementState_Modified : return "FdoSchemaElementState_Modified";
		case FdoSchemaElementState_Unchanged : return "FdoSchemaElementState_Unchanged";
		default : return "UNKNOWN_ENUM_VALUE";
	}
}


char *
FdoInfo::GetGeometryTypeName(FdoGeometryType geometryTypeEnum) {
	switch (geometryTypeEnum) {
		case FdoGeometryType_None : return "FdoGeometryType_None";
		case FdoGeometryType_Point : return "FdoGeometryType_Point";
		case FdoGeometryType_LineString : return "FdoGeometryType_LineString";
		case FdoGeometryType_Polygon : return "FdoGeometryType_Polygon";
		case FdoGeometryType_MultiPoint : return "FdoGeometryType_MultiPoint";
		case FdoGeometryType_MultiLineString : return "FdoGeometryType_MultiLineString";
		case FdoGeometryType_MultiPolygon : return "FdoGeometryType_MultiPolygon";
		case FdoGeometryType_MultiGeometry : return "FdoGeometryType_MultiGeometry";
		case FdoGeometryType_CurveString : return "FdoGeometryType_CurveString";
		case FdoGeometryType_CurvePolygon : return "FdoGeometryType_CurvePolygon";
		case FdoGeometryType_MultiCurveString : return "FdoGeometryType_MultiCurveString";
		case FdoGeometryType_MultiCurvePolygon : return "FdoGeometryType_MultiCurvePolygon";
		default : return "UNKNOWN_ENUM_VALUE";
	}
}

char *
FdoInfo::GetDimensionalityName(FdoDimensionality dimensionalityEnum) {
	switch (dimensionalityEnum) {
		case FdoDimensionality_XY : return "FdoDimensionality_XY";
		case FdoDimensionality_Z : return "FdoDimensionality_Z";
		case FdoDimensionality_M : return "FdoDimensionality_M";
		default : return "UNKNOWN_ENUM_VALUE";
	}
}

char *
FdoInfo::GetExpressionTypeName(FdoExpressionType expressionTypeEnum) {
	switch (expressionTypeEnum) {
		case FdoExpressionType_Basic : return "FdoExpressionType_Basic";
		case FdoExpressionType_Function : return "FdoExpressionType_Function";
		case FdoExpressionType_Parameter : return "FdoExpressionType_Parameter";
		default : return "UNKNOWN_ENUM_VALUE";
	}
}

char *
FdoInfo::GetGeometryComponentTypeName(FdoGeometryComponentType geometryComponentTypeEnum) {
	switch (geometryComponentTypeEnum) {
		case FdoGeometryComponentType_LinearRing : return "FdoGeometryComponentType_LinearRing";
		case FdoGeometryComponentType_CircularArcSegment : return "FdoGeometryComponentType_CircularArcSegment";
		case FdoGeometryComponentType_LineStringSegment : return "FdoGeometryComponentType_LineStringSegment";
		case FdoGeometryComponentType_Ring : return "FdoGeometryComponentType_Ring";
		default : return "UNKNOWN_ENUM_VALUE";
	}
}

char *
FdoInfo::GetObjectTypeName(FdoObjectType objectTypeEnum) {
	switch (objectTypeEnum) {
		case FdoObjectType_Value : return "FdoObjectType_Value";
		case FdoObjectType_Collection : return "FdoObjectType_Collection";
		case FdoObjectType_OrderedCollection : return "FdoObjectType_OrderedCollection";
		default : return "UNKNOWN_ENUM_VALUE";
	}
}

char *
FdoInfo::GetOrderTypeName(FdoOrderType orderTypeEnum) {
	switch (orderTypeEnum) {
		case FdoOrderType_Ascending : return "FdoOrderType_Ascending";
		case FdoOrderType_Descending : return "FdoOrderType_Descending";
		default : return "UNKNOWN_ENUM_VALUE";
	}
}


char *
FdoInfo::GetRasterDataModelTypeName(FdoRasterDataModelType rasterDataModelTypeEnum) {
	switch (rasterDataModelTypeEnum) {
		case FdoRasterDataModelType_Unknown : return "FdoRasterDataModelType_Unknown";
		case FdoRasterDataModelType_Bitonal : return "FdoRasterDataModelType_Bitonal";
		case FdoRasterDataModelType_Gray : return "FdoRasterDataModelType_Gray";
		case FdoRasterDataModelType_RGB : return "FdoRasterDataModelType_RGB";
		case FdoRasterDataModelType_RGBA : return "FdoRasterDataModelType_RGBA";
		case FdoRasterDataModelType_Palette : return "FdoRasterDataModelType_Palette";
		default : return "UNKNOWN_ENUM_VALUE";
	} 
}

char *
FdoInfo::GetRasterDataOrganizationName(FdoRasterDataOrganization rasterDataOrganizationEnum) {
	switch (rasterDataOrganizationEnum) {
		case FdoRasterDataOrganization_Pixel : return "FdoRasterDataOrganization_Pixel";
		case FdoRasterDataOrganization_Row : return "FdoRasterDataOrganization_Row";
		case FdoRasterDataOrganization_Image : return "FdoRasterDataOrganization_Image";
		default : return "UNKNOWN_ENUM_VALUE";
	}
}



