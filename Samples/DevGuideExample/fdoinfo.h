#ifndef _FDOINFO_H_
#define _FDOINFO_H_
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

#include "Logger.h"
#include "conversions.h"
#include <FdoCommon.h>
#include <Common/GeometryType.h>
#include <Common/Dimensionality.h>
#include "ExerciseFdoUtilities.h"

class FdoInfo {
public:
	FdoInfo(Logger * log, Conversions * convert,
		ExerciseFdoUtilities * utilities, FdoFgfGeometryFactory * geometryFactory);


	void LogArgumentDefinition(FdoArgumentDefinition * argDefinition);
	void LogAssociationPropertyDefinition(FdoAssociationPropertyDefinition * associationPropertyDefinition);
	// for now the dictionary arg is non-const because the Get methods are non-const
	// once the Get methods are made const, change the dictionary arg to const
	void LogConnectionPropertyAttributes(FdoIConnectionPropertyDictionary * dictionary,
		FdoString * propertyName);

	void LogClassCapabilities(FdoClassCapabilities * classCapabilities);
	void LogClassDefinition(FdoClassDefinition * classDefinition);
	void LogCommandCapabilities(FdoICommandCapabilities * capabilities);
	void LogConnectionCapabilities(FdoIConnectionCapabilities * capabilities);
	void LogConnectionInfo(FdoIConnectionInfo * connectionInfo);
	void LogDataPropertyDefinition(FdoDataPropertyDefinition * dataPropertyDefinition);
	void LogExpressionCapabilities(FdoIExpressionCapabilities * capabilities);
	void LogFdoGeometricTypes(FdoInt32 geometricTypes);
	void LogFilterCapabilities(FdoIFilterCapabilities * capabilities);
	void LogFeatureSchemaCollection(FdoFeatureSchemaCollection * featureSchemaCollection);
	void LogFeatureSchema(FdoFeatureSchema * featureSchema);
	void LogFeatureSchemaElement(FdoSchemaElement * schemaElement);
	void LogFunctionDefinition(FdoFunctionDefinition * definition);
	void LogGeometryCapabilities(FdoIGeometryCapabilities * capabilities);
	void LogGeometricPropertyDefinition(FdoGeometricPropertyDefinition * geometricPropertyDefinition);
	void LogLinearRing(FdoILinearRing * linearRing);
	void LogObjectPropertyDefinition(FdoObjectPropertyDefinition * objectPropertyDefinition);
	void LogPolygon(FdoIPolygon * polygon);
	void LogPropertyDefinition(FdoPropertyDefinition * propertyDefinition);
	void LogPropertyDefinitionCollection(FdoPropertyDefinitionCollection * propertyDefinitionCollection);
	void LogReadOnlyPropertyDefinitionCollection(FdoReadOnlyPropertyDefinitionCollection * propertyDefinitionCollection);
	void LogProviderInfo(const FdoProvider * provider);
	void LogRasterPropertyDefinition(FdoRasterPropertyDefinition * rasterPropertyDefinition);
	void LogRasterCapabilities(FdoIRasterCapabilities * capabilities);
	void LogRasterDataModelProperties(FdoRasterDataModel * rasterDataModel);
	void LogSchemaCapabilities(FdoISchemaCapabilities * capabilities);
	void LogTopologyCapabilities(FdoITopologyCapabilities * capabilities);

	void LogFeatureReader(FdoIFeatureReader * featureReader);


	char * GetClassTypeName(FdoClassType classTypeEnum);
	char * GetCommandTypeName(FdoInt32 commandTypeInt);
	char * GetConditionTypeName(FdoConditionType conditionTypeEnum);
	char * GetConnectionStateName(FdoConnectionState connectionStateEnum);
	char * GetDataTypeName(FdoDataType dataTypeEnum);
	char * GetDeleteRuleTypeName(FdoDeleteRule deleteRuleTypeEnum);
	char * GetDimensionalityName(FdoDimensionality dimensionalityEnum);
	char * GetDistanceOperationName(FdoDistanceOperations distanceOperationEnum);
	char * GetExpressionTypeName(FdoExpressionType expressionTypeEnum);
	char * GetGeometryComponentTypeName(FdoGeometryComponentType geometryComponentTypeEnum);
	char * GetGeometryTypeName(FdoGeometryType geometryTypeEnum);
	char * GetLockTypeName(FdoLockType lockTypeEnum);
	char * GetObjectTypeName(FdoObjectType objectTypeEnum);
	char * GetOrderTypeName(FdoOrderType orderTypeEnum);
	char * GetPropertyTypeName(FdoPropertyType propertyTypeEnum);
	char * GetRasterDataModelTypeName(FdoRasterDataModelType rasterDataModelTypeEnum);
	char * GetRasterDataOrganizationName(FdoRasterDataOrganization rasterDataOrganizationEnum);
	char * GetSchemaElementStateName(FdoSchemaElementState state);
	char * GetSpatialOperationName(FdoSpatialOperations spatialOperationEnum);
	char * GetSpatialContextExtentTypeName(FdoSpatialContextExtentType spatialContextExtentTypeEnum);
	char * GetThreadCapabilityName(FdoThreadCapability threadCapabilityEnum);


private:
	Logger * log;
	Conversions * convert;
	ExerciseFdoUtilities * utilities;
	FdoFgfGeometryFactory * geometryFactory;
	char errString[256];
	char sprintfBuf[1024];
	int numRasterDataModels;
	FdoRasterDataModel * rasterDataModels[6];
	FdoRasterDataModel * unknownRasterDataModel;
	FdoRasterDataModel * bitonalRasterDataModel;
	FdoRasterDataModel * grayRasterDataModel;
	FdoRasterDataModel * rgbRasterDataModel;
	FdoRasterDataModel * rgbaRasterDataModel;
	FdoRasterDataModel * paletteRasterDataModel;
};

#endif

