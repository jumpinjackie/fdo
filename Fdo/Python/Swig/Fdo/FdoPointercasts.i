//
// Copyright (C) 2004-2007  Autodesk, Inc.
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


/* Define typecasting for FdoICommand objects */
%pointer_cast(FdoICommand*, FdoISelectAggregates*, FdoICommandToFdoISelectAggregates);
%pointer_cast(FdoIFeatureCommand*, FdoISelectAggregates*, FdoIFeatureCommandToFdoISelectAggregates);
%pointer_cast(FdoICommand*, FdoIFeatureCommand*, FdoICommandToFdoIFeatureCommand);
%pointer_cast(FdoICommand*, FdoIInsert*, FdoICommandToFdoIInsert);
%pointer_cast(FdoICommand*, FdoISQLCommand*, FdoICommandToFdoISQLCommand);
%pointer_cast(FdoICommand*, FdoISelect*, FdoICommandToFdoISelect);
%pointer_cast(FdoIFeatureCommand*, FdoISelect*, FdoIFeatureCommandToFdoISelect);
%pointer_cast(FdoICommand*, FdoIDelete*, FdoICommandToFdoIDelete);
%pointer_cast(FdoIFeatureCommand*, FdoIDelete*, FdoIFeatureCommandToFdoIDelete);
%pointer_cast(FdoICommand*, FdoIUpdate*, FdoICommandToFdoIUpdate);
%pointer_cast(FdoIFeatureCommand*, FdoIUpdate*, FdoIFeatureCommandToFdoIUpdate);
%pointer_cast(FdoICommand*, FdoIDescribeSchema*, FdoICommandToFdoIDescribeSchema);
%pointer_cast(FdoICommand*, FdoIDestroySchema*, FdoICommandToFdoIDestroySchema);
%pointer_cast(FdoICommand*, FdoIApplySchema*, FdoICommandToFdoIApplySchema);
%pointer_cast(FdoICommand*, FdoIDescribeSchemaMapping*, FdoICommandToFdoIDescribeSchemaMapping);

/* Define typecasting between FdoICommand and the Data Store commands */
%pointer_cast(FdoICommand*, FdoICreateDataStore*, FdoICommandToFdoICreateDataStore);
%pointer_cast(FdoICommand*, FdoIDestroyDataStore*, FdoICommandToFdoIDestroyDataStore);
%pointer_cast(FdoICommand*, FdoIListDataStores*, FdoICommandToFdoIListDataStores); 

/* Define typecasting between FdoICommand and the locking commands */
%pointer_cast(FdoICommand*, FdoIAcquireLock*, FdoICommandToFdoIAcquireLock);
%pointer_cast(FdoIFeatureCommand*, FdoIAcquireLock*, FdoIFeatureCommandToFdoIAcquireLock);
%pointer_cast(FdoICommand*, FdoIReleaseLock*, FdoICommandToFdoIReleaseLock);
%pointer_cast(FdoIFeatureCommand*, FdoIReleaseLock*, FdoIFeatureCommandFdoIReleaseLock);
%pointer_cast(FdoICommand*, FdoIGetLockedObjects*, FdoICommandToFdoIGetLockedObjects);
%pointer_cast(FdoICommand*, FdoIGetLockInfo*, FdoICommandToFdoIGetLockInfo);
%pointer_cast(FdoIFeatureCommand*, FdoIGetLockInfo*, FdoIFeatureCommandToFdoIGetLockInfo);
%pointer_cast(FdoICommand*, FdoIGetLockOwners*, FdoICommandToFdoIGetLockOwners);

/* Define typecasting for Unit of measure commands */
%pointer_cast(FdoICommand*, FdoIDestroyMeasureUnit*, FdoICommandToFdoIDestroyMeasureUnit);
%pointer_cast(FdoICommand*, FdoICreateMeasureUnit*, FdoICommandToFdoICreateMeasureUnit);
%pointer_cast(FdoICommand*, FdoIGetMeasureUnits*, FdoICommandToFdoIGetMeatureUnits);

/* Define typecasting for Spatial Contexts */
%pointer_cast(FdoICommand*, FdoIActivateSpatialContext*, FdoICommandToFdoIActivateSpatialContext);
%pointer_cast(FdoICommand*, FdoIGetSpatialContexts*, FdoICommandToFdoIGetSpatialContexts);
%pointer_cast(FdoICommand*, FdoIDestroySpatialContext*, FdoICommandToFdoIDestroySpatialContext);
%pointer_cast(FdoICommand*, FdoICreateSpatialContext*, FdoICommandToFdoICreateSpatialContext);

/* Define typecasting for Long Transaction commands */
%pointer_cast(FdoICommand*, FdoICommitLongTransaction*, FdoICommandToFdoICommitLongTransaction);
%pointer_cast(FdoICommand*, FdoIRollbackLongTransaction*, FdoICommandToFdoIRollbackLongTransaction);
%pointer_cast(FdoICommand*, FdoIChangeLongTransactionSet*, FdoICommandToFdoIChangeLongTransactionSet);
%pointer_cast(FdoICommand*, FdoIGetLongTransactionsInSet*, FdoICommandToFdoIGetLongTransactionsInSet);
%pointer_cast(FdoICommand*, FdoIActivateLongTransaction*, FdoICommandToFdoIActivateLongTransaction);
%pointer_cast(FdoICommand*, FdoIFreezeLongTransaction*, FdoICommandToFdoIFreezeLongTransaction);
%pointer_cast(FdoICommand*, FdoICreateLongTransaction*, FdoICommandToFdoICreateLongTransaction);
%pointer_cast(FdoICommand*, FdoIGetLongTransactions*, FdoICommandToFdoIGetLongTransactions);
%pointer_cast(FdoICommand*, FdoIDeactivateLongTransaction*, FdoICommandToFdoIDeactivateLongTransaction);
%pointer_cast(FdoIFeatureCommand*, FdoICommitLongTransaction*, FdoIFeatureCommandToFdoICommitLongTransaction);
%pointer_cast(FdoIFeatureCommand*, FdoIRollbackLongTransaction*, FdoIFeatureCommandToFdoIRollbackLongTransaction);

// The following long transaction commands are shown from API Marble doc
%pointer_cast(FdoICommand*, FdoIActivateLongTransactionCheckpoint*, FdoICommandToFdoIActivateLongTransactionCheckpoint);
%pointer_cast(FdoICommand*, FdoIGetLongTransactionPrivileges*, FdoICommandToFdoIGetLongTransactionPrivileges);
%pointer_cast(FdoICommand*, FdoICreateLongTransactionCheckpoint*, FdoICommandToFdoICreateLongTransactionCheckpoint);
%pointer_cast(FdoICommand*, FdoIGetLongTransactionCheckpoints*, FdoICommandToFdoIGetLongTransactionCheckpoints);
%pointer_cast(FdoICommand*, FdoIRollbackLongTransactionCheckpoint*, FdoICommandToFdoIRollbackLongTransactionCheckpoint);
%pointer_cast(FdoICommand*, FdoIChangeLongTransactionPrivileges*, FdoICommandToFdoILongChangeTransactionPrivileges);


/* Define typecasting for Network commands */
//%pointer_cast(FdoICommand*, FdoINetworkCommand*, FdoICommandToFdoINetworkCommand);
//%pointer_cast(FdoICommand*, FdoINetworkShortestPathCommand*, FdoICommandToFdoINetworkShortestPathCommand);
//%pointer_cast(FdoICommand*, FdoINetworkTSPCommand*, FdoICommandToFdoINetworkTSPCommand);
//%pointer_cast(FdoICommand*, FdoINetworkReachingNodesCommand*, FdoICommandToFdoINetworkReachingNodesCommand);
//%pointer_cast(FdoICommand*, FdoINetworkAllPathsCommand*, FdoICommandToFdoINetworkAllPathsCommand);
//%pointer_cast(FdoICommand*, FdoINetworkNearestNeighborsCommand*, FdoICommandToFdoINetworkNearestNeighborsCommand);
//%pointer_cast(FdoICommand*, FdoINetworkWithinCostCommand*, FdoICommandToFdoINetworkWithinCostCommand);
//%pointer_cast(FdoICommand*, FdoINetworkReachableNodesCommand*, FdoICommandToFdoINetworkReachableNodesCommand);

//%pointer_cast(FdoINetworkCommand*, FdoINetworkShortestPathCommand*, FdoINetworkCommandToFdoINetworkShortestPathCommand);
//%pointer_cast(FdoINetworkCommand*, FdoINetworkTSPCommand*, FdoINetworkCommandToFdoINetworkTSPCommand);
//%pointer_cast(FdoINetworkCommand*, FdoINetworkReachingNodesCommand*, FdoINetworkCommandToFdoINetworkReachingNodesCommand);
//%pointer_cast(FdoINetworkCommand*, FdoINetworkAllPathsCommand*, FdoINetworkCommandToFdoINetworkAllPathsCommand);
//%pointer_cast(FdoINetworkCommand*, FdoINetworkNearestNeighborsCommand*, FdoINetworkCommandToFdoINetworkNearestNeighborsCommand);
//%pointer_cast(FdoINetworkCommand*, FdoINetworkWithinCostCommand*, FdoINetworkCommandToFdoINetworkWithinCostCommand);
//%pointer_cast(FdoINetworkCommand*, FdoINetworkReachableNodesCommand*, FdoINetworkCommandToFdoINetworkReachableNodesCommand);

/* Define typecasting for Topology commands */
// %pointer_cast(FdoICommand*, FdoIReconnectTopoEdge*, FdoICommandToFdoIReconnectTopoEdge);
// %pointer_cast(FdoICommand*, FdoIMoveTopoNode*, FdoICommandToFdoIMoveTopoNode);
// %pointer_cast(FdoICommand*, FdoIDeactivateTopologyInCommandResults*, FdoICommandToFdoIDeactivateTopologyInCommandResults);
// %pointer_cast(FdoICommand*, FdoIActivateTopologyInCommandResults*, FdoICommandToFdoIActivateTopologyInCommandResults);
// %pointer_cast(FdoICommand*, FdoIActivateTopologyArea*, FdoICommandToFdoIActivateTopologyArea);
// %pointer_cast(FdoICommand*, FdoIDeactivateTopologyArea*, FdoICommandToFdoIDeactivateTopologyArea);

/* Define typecasting for FdoDataValue subclasses */
%pointer_cast(FdoDataValue*, FdoInt16Value*, FdoDataValueToFdoInt16Value);
%pointer_cast(FdoDataValue*, FdoInt32Value*, FdoDataValueToFdoInt32Value);
%pointer_cast(FdoDataValue*, FdoInt64Value*, FdoDataValueToFdoInt64Value);
%pointer_cast(FdoDataValue*, FdoDoubleValue*, FdoDataValueToFdoDoubleValue);
%pointer_cast(FdoDataValue*, FdoCLOBValue*, FdoDataValueToFdoCLOBValue);
%pointer_cast(FdoDataValue*, FdoBLOBValue*, FdoDataValueToFdoBLOBValue);
%pointer_cast(FdoDataValue*, FdoDateTimeValue*, FdoDataValueToFdoDateTimeValue);
%pointer_cast(FdoDataValue*, FdoDecimalValue*, FdoDataValueToFdoDecimalValue);
%pointer_cast(FdoDataValue*, FdoSingleValue*, FdoDataValueToFdoSingleValue);
%pointer_cast(FdoDataValue*, FdoBooleanValue*, FdoDataValueToFdoBooleanValue);
%pointer_cast(FdoDataValue*, FdoByteValue*, FdoDataValueToFdoByteValue);
%pointer_cast(FdoDataValue*, FdoStringValue*, FdoDataValueToFdoStringValue);

/* Define typecasting for FdoExpression subclasses */
%pointer_cast(FdoExpression*, FdoFunction*, FdoExpressionToFdoFunction);
%pointer_cast(FdoExpression*, FdoIdentifier*, FdoExpressionToFdoIdentifier);
%pointer_cast(FdoExpression*, FdoValueExpression*, FdoExpressionToFdoValueExpression);
%pointer_cast(FdoExpression*, FdoBinaryExpression*, FdoExpressionToFdoBinaryExpression);
%pointer_cast(FdoExpression*, FdoUnaryExpression*, FdoExpressionToFdoUnaryExpression);

/* Define typecasting for FdoValueExpression subclasses */
%pointer_cast(FdoValueExpression*, FdoParameter*, FdoValueExpressionToFdoParameter);
%pointer_cast(FdoValueExpression*, FdoLiteralValue*, FdoValueExpressionToFdoLiteralValue);
%pointer_cast(FdoExpression*, FdoParameter*, FdoExpressionToFdoParameter);
%pointer_cast(FdoExpression*, FdoLiteralValue*, FdoExpressionToFdoLiteralValue);

/* Define typecasting for FdoLiteralValue subclasses */
%pointer_cast(FdoLiteralValue*, FdoGeometryValue*, FdoLiteralValueToFdoGeometryValue);
%pointer_cast(FdoLiteralValue*, FdoDataValue*, FdoLiteralValueToFdoDataValue);
%pointer_cast(FdoValueExpression*, FdoGeometryValue*, FdoValueExpressionToFdoGeometryValue);
%pointer_cast(FdoValueExpression*, FdoDataValue*, FdoValueExpressionToFdoDataValue);
%pointer_cast(FdoExpression*, FdoGeometryValue*, FdoExpressionToFdoGeometryValue);
%pointer_cast(FdoExpression*, FdoDataValue*, FdoExpressionToFdoDataValue); 



/* Define typecasting for subclasses of FdoFilter */
%pointer_cast(FdoFilter*, FdoSearchCondition*, FdoFilterToFdoSearchCondition);
%pointer_cast(FdoFilter*, FdoLogicalOperator*, FdoFilterToFdoLogicalOperator);

/* Define typecasting for subclasses of FdoSearchCondition */
%pointer_cast(FdoSearchCondition*, FdoGeometricCondition*, FdoSearchConditionToFdoGeometricCondition);
%pointer_cast(FdoSearchCondition*, FdoNullCondition*, FdoSearchConditionToFdoNullCondition);
%pointer_cast(FdoSearchCondition*, FdoComparisonCondition*, FdoSearchConditionToFdoComparisonCondition);
%pointer_cast(FdoSearchCondition*, FdoInCondition*, FdoSearchConditionToFdoInCondition);
%pointer_cast(FdoFilter*, FdoGeometricCondition*, FdoFilterToFdoGeometricCondition);
%pointer_cast(FdoFilter*, FdoNullCondition*, FdoFilterToFdoNullCondition);
%pointer_cast(FdoFilter*, FdoComparisonCondition*, FdoFilterToFdoComparisonCondition);
%pointer_cast(FdoFilter*, FdoInCondition*, FdoFilterToFdoInCondition);

/* Define typecasting for subclasses of FdoLogicalOperator */
%pointer_cast(FdoLogicalOperator*, FdoUnaryLogicalOperator*, FdoLogicalOperatorToFdoUnaryLogicalOperator);
%pointer_cast(FdoLogicalOperator*, FdoBinaryLogicalOperator*, FdoLogicalOperatorToFdoBinaryLogicalOperator);
%pointer_cast(FdoFilter*, FdoUnaryLogicalOperator*, FdoFilterToFdoUnaryLogicalOperator);
%pointer_cast(FdoFilter*, FdoBinaryLogicalOperator*, FdoFilterToFdoBinaryLogicalOperator);
 
/* Define typecasting for subclasses of FdoGeometricCondition */
%pointer_cast(FdoGeometricCondition*, FdoDistanceCondition*, FdoGeometricConditionToFdoDistanceCondition);
%pointer_cast(FdoGeometricCondition*, FdoSpatialCondition*, FdoGeometricConditionToFdoSpatialCondition);
%pointer_cast(FdoFilter*, FdoDistanceCondition*, FdoFilterToFdoDistanceCondition);
%pointer_cast(FdoFilter*, FdoSpatialCondition*, FdoFilterToFdoSpatialCondition);


/* Define typecasting for FdoIGeometry subtypes */
%pointer_cast(FdoIGeometry*, FdoISurfaceAbstract*, FdoIGeometryToFdoISurfaceAbstract);
%pointer_cast(FdoIGeometry*, FdoIPoint*, FdoIGeometryToFdoIPoint);
%pointer_cast(FdoIGeometry*, FdoIGeometricAggregateAbstract*, FdoIGeometryToFdoIGeometricAggregateAbstract );
%pointer_cast(FdoIGeometry*, FdoICurveAbstract*, FdoIGeometryToFdoICurveAbstract );
%pointer_cast(FdoIGeometry*, FdoIMultiPolygon*, FdoIGeometryToFdoIMultiPolygon );
%pointer_cast(FdoIGeometry*, FdoIMultiGeometry*, FdoIGeometryToFdoIMultiGeometry );
%pointer_cast(FdoIGeometry*, FdoIMultiCurveString*, FdoIGeometryToFdoIMultiCurveString );
%pointer_cast(FdoIGeometry*, FdoIMultiLineString*, FdoIGeometryToFdoIMultiLineString );
%pointer_cast(FdoIGeometry*, FdoIMultiPoint*, FdoIGeometryToFdoIMultiPoint );
%pointer_cast(FdoIGeometry*, FdoIMultiCurvePolygon*, FdoIGeometryToFdoIMultiCurvePolygon );
%pointer_cast(FdoIGeometry*, FdoICurvePolygon*, FdoIGeometryToFdoICurvePolygon );
%pointer_cast(FdoIGeometry*, FdoIPolygon*, FdoIGeometryToFdoIPolygon );
%pointer_cast(FdoIGeometry*, FdoILineString*, FdoIGeometryToFdoILineString );
%pointer_cast(FdoIGeometry*, FdoICurveString*, FdoIGeometryToFdoICurveString );

/* Define typecasting for subclasses of FdoSchemaElement */
%pointer_cast(FdoSchemaElement*, FdoFeatureSchema*, FdoSchemaElementToFdoFeatureSchema);
// %pointer_cast(FdoSchemaElement*, FdoRelation*, FdoSchemaElementToFdoRelation);
%pointer_cast(FdoSchemaElement*, FdoPropertyDefinition*, FdoSchemaElementToFdoPropertyDefinition);
// %pointer_cast(FdoSchemaElement*, FdoRelationEnd*, FdoSchemaElementToFdoRelationEnd);
%pointer_cast(FdoSchemaElement*, FdoClassDefinition*, FdoSchemaElementToFdoClassDefinition);

/* Define typecasting for subclasses of FdoClassDefinition */
/* This is required for the IFeatureReader */
%pointer_cast(FdoClassDefinition*, FdoFeatureClass*, FdoClassDefinitionToFdoFeatureClass);
%pointer_cast(FdoClassDefinition*, FdoClass*, FdoClassDefinitionToFdoClass);
// %pointer_cast(FdoClassDefinition*, FdoTopology*, FdoClassDefinitionToFdoTopology);
%pointer_cast(FdoClassDefinition*, FdoNetworkLayerClass*, FdoClassDefinitionToFdoNetworkLayerClass);
%pointer_cast(FdoClassDefinition*, FdoNetworkClass*, FdoClassDefinitionToFdoNetworkClass);

/* Define typecasting for subclasses of FdoPropertyDefinition */
%pointer_cast(FdoPropertyDefinition*, FdoDataPropertyDefinition*, FdoPropertyDefinitionToFdoDataPropertyDefinition);
%pointer_cast(FdoPropertyDefinition*, FdoGeometricPropertyDefinition*, FdoPropertyDefinitionToFdoGeometricPropertyDefinition);
%pointer_cast(FdoPropertyDefinition*, FdoObjectPropertyDefinition*, FdoPropertyDefinitionToFdoObjectPropertyDefinition);
%pointer_cast(FdoPropertyDefinition*, FdoRasterPropertyDefinition*, FdoPropertyDefinitionToFdoRasterPropertyDefinition);
// %pointer_cast(FdoPropertyDefinition*, FdoTopoFeaturePropertyDefinition*, FdoPropertyDefinitionToFdoTopoFeaturePropertyDefinition);
%pointer_cast(FdoPropertyDefinition*, FdoAssociationPropertyDefinition*, FdoPropertyDefinitionToFdoAssociationPropertyDefinition);
%pointer_cast(FdoSchemaElement*, FdoDataPropertyDefinition*, FdoSchemaElementToFdoDataPropertyDefinition);
%pointer_cast(FdoSchemaElement*, FdoGeometricPropertyDefinition*, FdoSchemaElementToFdoGeometricPropertyDefinition);
%pointer_cast(FdoSchemaElement*, FdoRasterPropertyDefinition*, FdoSchemaElementToFdoRasterPropertyDefinition);
%pointer_cast(FdoSchemaElement*, FdoObjectPropertyDefinition*, FdoSchemaElementToFdoObjectPropertyDefinition);
// %pointer_cast(FdoSchemaElement*, FdoTopoFeaturePropertyDefinition*, FdoSchemaElementToFdoTopoFeaturePropertyDefinition);
%pointer_cast(FdoSchemaElement*, FdoAssociationPropertyDefinition*, FdoSchemaElementToFdoAssociationPropertyDefinition);

/* Define typecasting for subclasses of FdoGeometricPropertyDefinition */
// %pointer_cast(FdoGeometricPropertyDefinition*, FdoTopoGeometricPropertyDefinition*, FdoGeometricPropertyDefinitionToFdoTopoGeometricPropertyDefinition);
// %pointer_cast(FdoPropertyDefinition*, FdoTopoGeometricPropertyDefinition*, FdoPropertyDefinitionToFdoTopoGeometricPropertyDefinition);
// %pointer_cast(FdoSchemaElement*, FdoTopoGeometricPropertyDefinition*, FdoSchemaElementToFdoTopoGeometricPropertyDefinition);

/* Define typecasting for subclasses of FdoFeatureClass */
%pointer_cast(FdoFeatureClass*, FdoNetworkFeatureClass*, FdoFeatureClassToFdoNetworkFeatureClass);
%pointer_cast(FdoClassDefinition*, FdoNetworkFeatureClass*, FdoClassDefinitionToFdoNetworkFeatureClass);
%pointer_cast(FdoSchemaElement*, FdoNetworkFeatureClass*, FdoSchemaElementToFdoNetworkFeatureClass);

/* Define typecasting for subclasses of FdoNetworkFeatureClass */
%pointer_cast(FdoNetworkFeatureClass*, FdoNetworkNodeFeatureClass*, FdoNetworkFeatureClassToFdoNetworkNodeFeatureClass);
%pointer_cast(FdoNetworkFeatureClass*, FdoNetworkLinkFeatureClass*, FdoNetworkFeatureClassToFdoNetworkLinkFeatureClass);
%pointer_cast(FdoFeatureClass*, FdoNetworkNodeFeatureClass*, FdoFeatureClassToFdoNetworkNodeFeatureClass);
%pointer_cast(FdoFeatureClass*, FdoNetworkLinkFeatureClass*, FdoFeatureClassToFdoNetworkLinkFeatureClass);
%pointer_cast(FdoClassDefinition*, FdoNetworkNodeFeatureClass*, FdoClassDefinitionToFdoNetworkNodeFeatureClass);
%pointer_cast(FdoClassDefinition*, FdoNetworkLinkFeatureClass*, FdoClassDefinitionToFdoNetworkLinkFeatureClass);
%pointer_cast(FdoSchemaElement*, FdoNetworkNodeFeatureClass*, FdoSchemaElementToFdoNetworkNodeFeatureClass);
%pointer_cast(FdoSchemaElement*, FdoNetworkLinkFeatureClass*, FdoSchemaElementToFdoNetworkLinkFeatureClass);

/* Define typecasting for Physical Schema Mapping classes */
// %pointer_cast(FdoPhysicalElementMappingCollection*, FdoPhysicalSchemaMappingCollection*, FdoPhysicalElementMappingCollectionToFdoPhysicalSchemaMappingCollection);
// %pointer_cast(FdoPhysicalElementMapping*, FdoPhysicalSchemaMapping*, FdoPhysicalElementMappingToFdoPhysicalSchemaMapping);

/* Define typecasting for FdoIoStream classes */
%pointer_cast(FdoIoStream*, FdoIoBufferStream*, FdoIoStreamToFdoIoBufferStream);
%pointer_cast(FdoIoStream*, FdoIoMemoryStream*, FdoIoStreamToFdoIoMemoryStream);
%pointer_cast(FdoIoStream*, FdoIoFileStream*, FdoIoStreamToFdoIoFileStream);
%pointer_cast(FdoIoBufferStream*, FdoIoStream*, FdoIoBufferStreamToFdoIoStream);
%pointer_cast(FdoIoMemoryStream*, FdoIoStream*, FdoIoMemoryStreamToFdoIoStream);
%pointer_cast(FdoIoFileStream*, FdoIoStream*, FdoIoFileStreamToFdoIoStream);

/* Define typecasting for FdoIoObjectStreamReader classes */
%pointer_cast(FdoIoByteStreamReader*, FdoIStreamReader*, FdoIoByteStreamReaderToFdoIStreamReader);
%pointer_cast(FdoIStreamReader*, FdoBLOBStreamReader*, FdoIStreamReaderToFdoBLOBStreamReader);
%pointer_cast(FdoIStreamReader*, FdoIoByteStreamReader*, FdoIStreamReaderToFdoIoByteStreamReader);

/* Define typecasting for subclasses of FdoPropertyValueConstraint */
%pointer_cast(FdoPropertyValueConstraint*, FdoPropertyValueConstraintList*, FdoPropertyValueConstraintToFdoPropertyValueConstraintList);
%pointer_cast(FdoPropertyValueConstraint*, FdoPropertyValueConstraintRange*, FdoPropertyValueConstraintToFdoPropertyValueConstraintRange);